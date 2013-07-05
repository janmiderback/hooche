/**
 * Copyright 2009 Jan Miderbäck
 *
 * This file is part of Hooce.
 *
 * Hooce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hooce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hooce.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hooche.h"
#include "move.h"
#include "board.h"
#include "fen.h"
#include "game.h"

void game_init(t_game* me)
{
    (void)game_init_fen(me, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}


t_bool game_init_fen(t_game* me, const char* fen)
{
    assert(me != NULL);
    assert(fen != NULL);

    movestack_init(&(me->movestack));
    nodestack_init(&(me->nodestack));

    if (fen_parse(fen, me)) {
        board_init_bbs(&(me->board));
        return TRUE;
    }
    
    /*TODO: Fix*/
    printf("FEN parse error in %s\n", fen);
    game_init(me);
    board_init_bbs(&(me->board));
    return FALSE;
}


t_bool game_play_moves(t_game* me, const t_move moves[], unsigned int nmoves)
{
    /*TODO*/
    return TRUE;
}


void game_set_stm(t_game* me, t_color color)
{
    me->side_tm = color;
    me->side_ntm = COLOR_FLIP(color);
}


t_bool game_islegal_move(t_game* me, t_move move)
{
    t_board* p_board = &(me->board);
    t_bool checked;
    t_piece cap;
    t_piece piece;
    t_square ep_square;
    t_square from;
    t_square to;
    t_square castle_from;
    t_square castle_to;

    from = move.from;
    to = move.to;
    cap = p_board->piece[to];
    piece = p_board->piece[from];
    /* Suppress stupid compiler warnings!! */
    ep_square = OFF_BOARD;
    castle_from = OFF_BOARD;
    castle_to = OFF_BOARD;

    if (cap != NO_PIECE)
    {
        BB_CLEAR_SQ(p_board->color_bbs[me->side_ntm], to);
        BB_CLEAR_SQ(p_board->piece_bbs[cap], to); 
    }
    else
    {
        BB_SET_SQ(p_board->occupied_bb, to);
    }
    BB_CLEAR_SQ(p_board->occupied_bb, from);
    
    /* ep captures */
    if (piece == PAWN && to == p_board->ep_square)
    {
        ep_square = to - PAWN_DIR(me->side_tm);
        BB_CLEAR_SQ(p_board->occupied_bb, ep_square);
        BB_CLEAR_SQ(p_board->color_bbs[me->side_ntm], ep_square);
        BB_CLEAR_SQ(p_board->piece_bbs[PAWN], ep_square);
    }
    else if (piece == KING)
    {
        p_board->king_square[me->side_tm] = to;
        /* castling */
        if (abs(to - from) == 2)
        {
            if (to > from)
            {
                castle_from = from + 3;
                castle_to = from + 1;
            }
            else
            {
                castle_from = from - 4;
                castle_to = from - 1;
            }
            BB_CLEAR_SQ(p_board->occupied_bb, castle_from);
            BB_SET_SQ(p_board->occupied_bb, castle_to);
        }
    }
    /* We have changed all of the internal bitboards, now see if the king is in check. */
    checked = game_stm_incheck(me);

    BB_SET_SQ(p_board->occupied_bb, from);
    if (cap != NO_PIECE)
    {
        BB_SET_SQ(p_board->color_bbs[me->side_ntm], to);
        BB_SET_SQ(p_board->piece_bbs[cap], to); 
    }
    else
        BB_CLEAR_SQ(p_board->occupied_bb, to);
    /* ep captures */
    if (piece == PAWN && to == p_board->ep_square)
    {
        BB_SET_SQ(p_board->occupied_bb, ep_square);
        BB_SET_SQ(p_board->color_bbs[me->side_ntm], ep_square);
        BB_SET_SQ(p_board->piece_bbs[PAWN], ep_square);
    }
    else if (piece == KING)
    {
        p_board->king_square[me->side_tm] = from;
        /* castling */
        if (abs(to - from) == 2)
        {
            BB_SET_SQ(p_board->occupied_bb, castle_from);
            BB_CLEAR_SQ(p_board->occupied_bb, castle_to);
        }
    }
    return !checked;
}


t_bool game_stm_incheck(t_game* me)
{
    return game_isattacked(me, me->board.king_square[me->side_tm], me->side_ntm);
}


t_bool game_isattacked(t_game* me, t_square square, t_color attacker)
{
    t_board* p_board = &(me->board);
    t_bitboard attack_bb;

	/* Pawns */
	attack_bb = pawn_caps_bbs[COLOR_FLIP(attacker)][square];
	if (attack_bb & p_board->piece_bbs[PAWN] & p_board->color_bbs[attacker])
    {
        return TRUE;
    }
	
    /* Knights */
	attack_bb = knight_move_bbs[square];
	if (attack_bb & p_board->piece_bbs[KNIGHT] & p_board->color_bbs[attacker])
    {
        return TRUE;
    }
	
    /* Kings */
	attack_bb = king_move_bbs[square];
	if (attack_bb & p_board->piece_bbs[KING] & p_board->color_bbs[attacker])
    {
        return TRUE;
    }
	
    /* Bishop sliders */
	attack_bb = bb_get_attack(p_board, BISHOP, square);
	if (attack_bb & (p_board->piece_bbs[BISHOP] | p_board->piece_bbs[QUEEN]) & p_board->color_bbs[attacker])
    {
        return TRUE;
    }

	/* Rook sliders */
	attack_bb = bb_get_attack(p_board, ROOK, square);
	if (attack_bb & (p_board->piece_bbs[ROOK] | p_board->piece_bbs[QUEEN]) & p_board->color_bbs[attacker])
    {
		return TRUE;
    }

	return FALSE;
}
