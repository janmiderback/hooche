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
#include "game.h"
#include "board.h"
#include "move.h"
#include "genmoves.h"

t_rank pawn_promote_rank[2] = { RANK_8, RANK_1 };

//static unsigned int rem_illegal_moves(t_game* p_game, t_move* p_moves, unsigned int nmoves)
//{
//    t_move* p_first;
//
//    assert(p_game != NULL);
//    assert(p_moves != NULL);
//    assert(nmoves != 0);
//
//    p_first = &(p_moves[0]);
//    
//    if (nmoves == 1)
//    {
//        if (!game_islegal_move(p_game, *p_first)) nmoves = 0;
//    }
//    else
//    {
//        t_move* p_last = &(p_moves[nmoves-1]);
//
//	    while (p_first < p_last)
//	    {
//		    if (!game_islegal_move(p_game, *p_first)) 
//            {
//                *p_first = *p_last;
//                --p_first;
//                --p_last;
//                --nmoves;
//            }
//		    ++p_first;
//	    }
//    }
//
//    return nmoves;
//}


static unsigned int rem_illegal_moves(t_game* p_game, unsigned int nmoves)
{
    t_move* p_moves;
    t_move* p_first;
    unsigned int nremoved = 0;

    assert(p_game != NULL);
    assert(nmoves != 0);

    p_moves = movestack_getarrayfromtop(&(p_game->movestack), nmoves);
    p_first = &(p_moves[0]);
    
    if (nmoves == 1)
    {
        if (!game_islegal_move(p_game, *p_first))
        {
            nremoved = 1;
        }
    }
    else
    {
        t_move* p_last = &(p_moves[nmoves-1]);

	    while (p_first < p_last)
	    {
		    if (!game_islegal_move(p_game, *p_first)) 
            {
                *p_first = *p_last;
                --p_first;
                --p_last;
                ++nremoved;
            }
		    ++p_first;
	    }
    }

    // Moves has now been rearranged on the stack so that the
    // illegal moves can be popped of the stack.
    movestack_popn(&(p_game->movestack), nremoved);

    return nmoves - nremoved;
}


//unsigned int gen_root_moves(t_game* p_game, t_move* p_moves)
//{
//    unsigned int nmoves = gen_pseudo_moves(p_game, p_moves);
//    nmoves = rem_illegal_moves(p_game, p_moves, nmoves);
//    return nmoves;
//}


unsigned int gen_rootmoves(t_game* p_game)
{
    unsigned int nmoves = gen_pseudomoves(p_game);
    nmoves = rem_illegal_moves(p_game, nmoves);
    return nmoves;
}


//unsigned int gen_pseudo_moves(t_game* p_game, t_move* p_moves)
//{
//    t_board*     p_board = &(p_game->board);
//    t_bitboard   forward_pawns;
//	t_bitboard   moves;
//	t_bitboard   pieces;
//	t_bitboard   target;
//	t_piece      piece;
//	t_square     from;
//	t_square     to;
//    unsigned int nmove = 0;
//
//	/* Pawn moves */
//
//	pieces = p_board->piece_bbs[PAWN] & p_board->color_bbs[p_game->side_tm];
//
//	forward_pawns = SHIFT_FORWARD(pieces, p_game->side_tm);
//
//	/* pawn caps */
//	target = p_board->color_bbs[p_game->side_ntm];
//    if (p_board->ep_square != OFF_BOARD)
//    {
//		target |= SQ_MASK(p_board->ep_square);
//    }
//
//	moves = SHIFT_LF(forward_pawns) & target;
//	while (moves)
//	{
//		to = first_square(moves);
//		BB_CLEAR_SQ(moves, to);
//        from = to - PAWN_DIR(p_game->side_tm) + 1;
//		if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
//		{
//			move_set(&(p_moves[nmove++]), from, to, KNIGHT);
//            move_set(&(p_moves[nmove++]), from, to, BISHOP);
//            move_set(&(p_moves[nmove++]), from, to, ROOK);
//            move_set(&(p_moves[nmove++]), from, to, QUEEN);
//		}
//		else
//        {
//            move_set(&(p_moves[nmove++]), from, to, NO_PIECE);
//        }
//	}
//	moves = SHIFT_RT(forward_pawns) & target;
//	while (moves)
//	{
//		to = first_square(moves);
//		BB_CLEAR_SQ(moves, to);
//		from = to - PAWN_DIR(p_game->side_tm) - 1;
//		if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
//		{
//            move_set(&(p_moves[nmove++]), from, to, KNIGHT);
//            move_set(&(p_moves[nmove++]), from, to, BISHOP);
//            move_set(&(p_moves[nmove++]), from, to, ROOK);
//            move_set(&(p_moves[nmove++]), from, to, QUEEN);
//		}
//		else
//        {
//            move_set(&(p_moves[nmove++]), from, to, NO_PIECE);
//        }
//	}
//	
//    /* pawn single step */
//	target = ~p_board->occupied_bb;
//	moves = forward_pawns & target;
//	
//    /* Pawns that can move forward to the 3rd/6th rank can double step. */
//	pieces = moves & MASK_RANK_COLOR(RANK_3, p_game->side_tm);
//	while (moves)
//	{
//		to = first_square(moves);
//		BB_CLEAR_SQ(moves, to);
//		from = to - PAWN_DIR(p_game->side_tm);
//		if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
//		{
//			move_set(&(p_moves[nmove++]), from, to, KNIGHT);
//            move_set(&(p_moves[nmove++]), from, to, BISHOP);
//            move_set(&(p_moves[nmove++]), from, to, ROOK);
//            move_set(&(p_moves[nmove++]), from, to, QUEEN);
//		}
//		else
//        {
//			move_set(&(p_moves[nmove++]), from, to, NO_PIECE);
//        }
//	}
//	
//    /* pawn double step */
//	moves = SHIFT_FORWARD(pieces, p_game->side_tm) & target;
//	while (moves)
//	{
//		to = first_square(moves);
//		BB_CLEAR_SQ(moves, to);
//        move_set(&(p_moves[nmove++]), to - 2 * PAWN_DIR(p_game->side_tm), to, NO_PIECE);
//	}
//
//	/* other moves */
//	target = ~p_board->color_bbs[p_game->side_tm];
//	
//    for (piece = KNIGHT; piece <= KING; piece++)
//	{
//		pieces = p_board->color_bbs[p_game->side_tm] & p_board->piece_bbs[piece];
//		while (pieces)
//		{
//			from = first_square(pieces);
//			BB_CLEAR_SQ(pieces, from);
//
//			moves = bb_get_attack(p_board, piece, from) & target;
//			while (moves)
//			{
//				to = first_square(moves);
//				BB_CLEAR_SQ(moves, to);
//                move_set(&(p_moves[nmove++]), from, to, NO_PIECE);
//			}
//		}
//	}
//	
//    /* castling */
//	from = p_board->king_square[p_game->side_tm];
//	if ( CAN_CASTLE_KS(p_game->castle_rights, p_game->side_tm)   &&
//         !(p_board->occupied_bb & castle_ks_bbs[p_game->side_tm]) &&
//		 !game_isattacked(p_game, from, p_game->side_ntm) &&
//         !game_isattacked(p_game, from + 1, p_game->side_ntm) &&
//		 !game_isattacked(p_game, from + 2, p_game->side_ntm) )
//    {
//        move_set(&(p_moves[nmove++]), from, to, NO_PIECE);
//    }
//    
//	if ( CAN_CASTLE_QS(p_game->castle_rights, p_game->side_tm) &&
//         !(p_board->occupied_bb & castle_qs_bbs[p_game->side_tm]) &&
//		 !game_isattacked(p_game, from, p_game->side_ntm) &&
//         !game_isattacked(p_game, from - 1, p_game->side_ntm) &&
//		 !game_isattacked(p_game, from - 2, p_game->side_ntm) )
//    {
//        move_set(&(p_moves[nmove++]), from, to, NO_PIECE);		
//    }
//
//	return nmove;
//}


unsigned int gen_pseudomoves(t_game* p_game)
{
    t_movestack* p_ms = &(p_game->movestack);
    t_board*     p_board = &(p_game->board);
    t_bitboard   forward_pawns;
	t_bitboard   moves;
	t_bitboard   pieces;
	t_bitboard   target;
	t_piece      piece;
	t_square     from;
	t_square     to;
    unsigned int nmove = 0;

	/* Pawn moves */

	pieces = p_board->piece_bbs[PAWN] & p_board->color_bbs[p_game->side_tm];

	forward_pawns = SHIFT_FORWARD(pieces, p_game->side_tm);

	/* pawn caps */
	target = p_board->color_bbs[p_game->side_ntm];
    if (p_board->ep_square != OFF_BOARD)
    {
		target |= SQ_MASK(p_board->ep_square);
    }

	moves = SHIFT_LF(forward_pawns) & target;
	while (moves)
	{
		to = first_square(moves);
		BB_CLEAR_SQ(moves, to);
        from = to - PAWN_DIR(p_game->side_tm) + 1;
		if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
		{
			movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, KNIGHT);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, BISHOP);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, ROOK);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, QUEEN);
		}
		else
        {
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, NO_PIECE);
        }
	}
	moves = SHIFT_RT(forward_pawns) & target;
	while (moves)
	{
		to = first_square(moves);
		BB_CLEAR_SQ(moves, to);
		from = to - PAWN_DIR(p_game->side_tm) - 1;
		if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
		{
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, KNIGHT);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, BISHOP);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, ROOK);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, QUEEN);
		}
		else
        {
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, NO_PIECE);
        }
	}
	
    /* pawn single step */
	target = ~p_board->occupied_bb;
	moves = forward_pawns & target;
	
    /* Pawns that can move forward to the 3rd/6th rank can double step. */
	pieces = moves & MASK_RANK_COLOR(RANK_3, p_game->side_tm);
	while (moves)
	{
		to = first_square(moves);
		BB_CLEAR_SQ(moves, to);
		from = to - PAWN_DIR(p_game->side_tm);
		if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
		{
            movestack_push(p_ms);
			move_set(movestack_top(p_ms), from, to, KNIGHT);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, BISHOP);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, ROOK);
            movestack_push(p_ms);
            move_set(movestack_top(p_ms), from, to, QUEEN);
		}
		else
        {
            movestack_push(p_ms);
			move_set(movestack_top(p_ms), from, to, NO_PIECE);
        }
	}
	
    /* pawn double step */
	moves = SHIFT_FORWARD(pieces, p_game->side_tm) & target;
	while (moves)
	{
		to = first_square(moves);
		BB_CLEAR_SQ(moves, to);
        movestack_push(p_ms);
        move_set(movestack_top(p_ms), to - 2 * PAWN_DIR(p_game->side_tm), to, NO_PIECE);
	}

	/* other moves */
	target = ~p_board->color_bbs[p_game->side_tm];
	
    for (piece = KNIGHT; piece <= KING; piece++)
	{
		pieces = p_board->color_bbs[p_game->side_tm] & p_board->piece_bbs[piece];
		while (pieces)
		{
			from = first_square(pieces);
			BB_CLEAR_SQ(pieces, from);

			moves = bb_get_attack(p_board, piece, from) & target;
			while (moves)
			{
				to = first_square(moves);
				BB_CLEAR_SQ(moves, to);
                movestack_push(p_ms);
                move_set(movestack_top(p_ms), from, to, NO_PIECE);
			}
		}
	}
	
    /* castling */
	from = p_board->king_square[p_game->side_tm];
	if ( CAN_CASTLE_KS(p_game->castle_rights, p_game->side_tm)   &&
         !(p_board->occupied_bb & castle_ks_bbs[p_game->side_tm]) &&
		 !game_isattacked(p_game, from, p_game->side_ntm) &&
         !game_isattacked(p_game, from + 1, p_game->side_ntm) &&
		 !game_isattacked(p_game, from + 2, p_game->side_ntm) )
    {
        movestack_push(p_ms);
        move_set(movestack_top(p_ms), from, to, NO_PIECE);
    }
    
	if ( CAN_CASTLE_QS(p_game->castle_rights, p_game->side_tm) &&
         !(p_board->occupied_bb & castle_qs_bbs[p_game->side_tm]) &&
		 !game_isattacked(p_game, from, p_game->side_ntm) &&
         !game_isattacked(p_game, from - 1, p_game->side_ntm) &&
		 !game_isattacked(p_game, from - 2, p_game->side_ntm) )
    {
        movestack_push(p_ms);
        move_set(movestack_top(p_ms), from, to, NO_PIECE);
    }

	return movestack_size(p_ms);
}



/**
generate_captures():
Generates pseudo-legal captures and queen promotions to be used in quiescent search.
Created 101806; last modified 020608
**/
#if 0
t_move* generate_captures(t_move *next_move)
{
	t_bitboard forward_pawns;
	t_bitboard moves;
	t_bitboard pieces;
	t_bitboard target;
	t_piece piece;
	t_square from;
	t_square to;

	/* pawn caps */
	pieces = p_board->piece_bb[PAWN] & p_board->color_bb[p_game->side_tm];
	forward_pawns = SHIFT_FORWARD(pieces, p_game->side_tm);
	target = p_board->color_bb[p_game->side_ntm];
	if (p_board->ep_square != OFF_BOARD)
		target |= MASK(p_board->ep_square);
	moves = SHIFT_LF(forward_pawns) & target;
	while (moves)
	{
		to = first_square(moves);
		CLEAR_BIT(moves, to);
		from = to - pawn_dir[p_game->side_tm] + 1;
		if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
			*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(QUEEN);
		else
			*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
	}
	moves = SHIFT_RT(forward_pawns) & target;
	while (moves)
	{
		to = first_square(moves);
		CLEAR_BIT(moves, to);
		from = to - pawn_dir[p_game->side_tm] - 1;
		if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
			*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(QUEEN);
		else
			*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
	}
	/* non-capture promotions */
	moves = forward_pawns & ~p_board->occupied_bb & MASK_RANK(pawn_promote_rank[p_game->side_tm]);
	while (moves)
	{
		to = first_square(moves);
		CLEAR_BIT(moves, to);
		from = to - pawn_dir[p_game->side_tm];
		*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(QUEEN);
	}

	/* other moves */
	for (piece = KNIGHT; piece <= KING; piece++)
	{
		pieces = p_board->color_bb[p_game->side_tm] & p_board->piece_bb[piece];
		while (pieces)
		{
			from = first_square(pieces);
			CLEAR_BIT(pieces, from);
	
			moves = attacks_bb(piece, from) & target;
			while (moves)
			{
				to = first_square(moves);
				CLEAR_BIT(moves, to);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
			}
		}
	}

	return next_move;
}
#endif /*0*/

/**
generate_checks():
Generates pseudo-legal checking moves. Note that this does not generate any captures, as
it is intended to be used in quiescence after the captures have already been generated and tried.
Created 103006; last modified 052107
**/
#if 0
MOVE *generate_checks(MOVE *next_move)
{
	t_bitboard forward_pawns;
	t_bitboard moves;
	t_bitboard pieces;
	t_bitboard target;
	t_piece piece;
	t_square from;
	t_square to;
	t_square king_square;

	king_square = p_board->king_square[p_game->side_ntm];
	/* pawns */
	forward_pawns = SHIFT_FORWARD(p_board->piece_bb[PAWN] & p_board->color_bb[p_game->side_tm], p_game->side_tm);

	/* pawn single step */
	target = ~p_board->occupied_bb;
	moves = forward_pawns & target;
	/* Pawns that can move forward to the 3rd/6th rank can double step. */
	pieces = moves & MASK_RANK_COLOR(RANK_3, p_game->side_tm);
	target &= pawn_caps_bb[p_game->side_ntm][king_square];
	moves &= target & ~MASK_RANK(pawn_promote_rank[p_game->side_tm]);
	while (moves)
	{
		to = first_square(moves);
		CLEAR_BIT(moves, to);
		*next_move++ = MOVE_SET_FROM(to - pawn_dir[p_game->side_tm]) | MOVE_SET_TO(to);
	}
	/* pawn double step */
	moves = SHIFT_FORWARD(pieces, p_game->side_tm) & target;
	while (moves)
	{
		to = first_square(moves);
		CLEAR_BIT(moves, to);
		*next_move++ = MOVE_SET_FROM(to - 2 * pawn_dir[p_game->side_tm]) | MOVE_SET_TO(to);
	}

	/* other moves */
	for (piece = KNIGHT; piece <= QUEEN; piece++)
	{
		target = ~p_board->occupied_bb & attacks_bb(piece, king_square);
		pieces = p_board->color_bb[p_game->side_tm] & p_board->piece_bb[piece];
		while (pieces)
		{
			from = first_square(pieces);
			CLEAR_BIT(pieces, from);
	
			moves = attacks_bb(piece, from) & target;
			while (moves)
			{
				to = first_square(moves);
				CLEAR_BIT(moves, to);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
			}
		}
	}
	/* castling */
	target = MASK(king_square);
	from = p_board->king_square[p_game->side_tm];
	if (CAN_CASTLE_KS(p_board->castle_rights, p_game->side_tm) && !(p_board->occupied_bb & castle_ks_bbs[p_game->side_tm]) &&
		!is_attacked(from, p_game->side_ntm) && !is_attacked(from + 1, p_game->side_ntm) &&
		!is_attacked(from + 2, p_game->side_ntm))
	{
		pieces = p_board->occupied_bb ^ (MASK(from) | MASK(from + 1) | MASK(from + 2) | MASK(from + 3));
		if ((horizontal_attacks(from + 1, pieces) | vertical_attacks(from + 1, pieces)) & target)
			*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(from + 2);
	}
	if (CAN_CASTLE_QS(p_board->castle_rights, p_game->side_tm) && !(p_board->occupied_bb & castle_qs_bbs[p_game->side_tm]) &&
		!is_attacked(from, p_game->side_ntm) && !is_attacked(from - 1, p_game->side_ntm) &&
		!is_attacked(from - 2, p_game->side_ntm))
	{
		pieces = p_board->occupied_bb ^ (MASK(from) | MASK(from - 1) | MASK(from - 2) | MASK(from - 4));
		if ((horizontal_attacks(from - 1, pieces) | vertical_attacks(from - 1, pieces)) & target)
			*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(from - 2);
	}
	return next_move;
}
#endif /*0*/

/**
generate_evasions():
Generates evasions from a position with the side to move in check. The moves generated are guaranteed to be legal.
Created 052107; last modified 020608
**/
#if 0
MOVE *generate_evasions(MOVE *next_move, t_bitboard checkers)
{
	t_bitboard forward_pawns;
	t_bitboard moves;
	t_bitboard pieces;
	t_bitboard target;
	t_bitboard old_occupied_bb;
	t_piece piece;
	t_square from;
	t_square to;

	ASSERT(pop_count(checkers) > 0 && pop_count(checkers) <= 2);
	/* If there is only one checking piece, then we can try captures and interpositions. */
	if (pop_count(checkers) == 1)
	{
		/* pawns */
		pieces = p_board->piece_bb[PAWN] & p_board->color_bb[p_game->side_tm];
		forward_pawns = SHIFT_FORWARD(pieces, p_game->side_tm);

		/* pawn caps */
		target = checkers;
		if (p_board->ep_square != OFF_BOARD && !(target & MASK_RANK_COLOR(RANK_2, p_game->side_tm)))
			SET_BIT(target, p_board->ep_square);
		moves = SHIFT_LF(forward_pawns) & target;
		while (moves)
		{
			to = first_square(moves);
			CLEAR_BIT(moves, to);
			from = to - pawn_dir[p_game->side_tm] + 1;
			if (is_pinned(from, p_game->side_tm))
				continue;
			if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
			{
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(KNIGHT);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(BISHOP);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(ROOK);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(QUEEN);
			}
			else
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
		}
		moves = SHIFT_RT(forward_pawns) & target;
		while (moves)
		{
			to = first_square(moves);
			CLEAR_BIT(moves, to);
			from = to - pawn_dir[p_game->side_tm] - 1;
			if (is_pinned(from, p_game->side_tm))
				continue;
			if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
			{
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(QUEEN);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(KNIGHT);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(ROOK);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(BISHOP);
			}
			else
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
		}
		target = in_between_bb[p_board->king_square[p_game->side_tm]][first_square(checkers)];
		/* pawn single step interpositions */
		moves = forward_pawns & ~p_board->occupied_bb;
		/* Pawns that can move forward to the 3rd/6th rank can double step. */
		pieces = moves & MASK_RANK_COLOR(RANK_3, p_game->side_tm);
		moves &= target;
		while (moves)
		{
			to = first_square(moves);
			CLEAR_BIT(moves, to);
			from = to - pawn_dir[p_game->side_tm];
			if (is_pinned(from, p_game->side_tm))
				continue;
			if (RANK_OF_SQ(to) == pawn_promote_rank[p_game->side_tm])
			{
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(QUEEN);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(KNIGHT);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(ROOK);
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(BISHOP);
			}
			else
				*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
		}
		/* pawn double step interpositions */
		moves = SHIFT_FORWARD(pieces, p_game->side_tm) & target & ~p_board->occupied_bb;
		while (moves)
		{
			to = first_square(moves);
			CLEAR_BIT(moves, to);
			from = to - 2 * pawn_dir[p_game->side_tm];
			if (is_pinned(from, p_game->side_tm))
				continue;
			*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
		}
		/* piece interpositions and captures */
		target |= checkers;
		for (piece = KNIGHT; piece <= QUEEN; piece++)
		{
			pieces = p_board->color_bb[p_game->side_tm] & p_board->piece_bb[piece];
			while (pieces)
			{
				from = first_square(pieces);
				CLEAR_BIT(pieces, from);

				moves = attacks_bb(piece, from) & target;
				while (moves)
				{
					to = first_square(moves);
					CLEAR_BIT(moves, to);
					*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
				}
			}
		}
	}

	/* King moves */
	from = p_board->king_square[p_game->side_tm];
	moves = attacks_bb(KING, from) & ~p_board->color_bb[p_game->side_tm];
	old_occupied_bb = p_board->occupied_bb;
	while (moves)
	{
		to = first_square(moves);
		CLEAR_BIT(moves, to);
		/* Change the occupancy maps to reflect the king's new position. */
		CLEAR_BIT(p_board->occupied_bb, from);
		SET_BIT(p_board->occupied_bb, to);
		if (!is_attacked(to, p_game->side_ntm))
			*next_move++ = MOVE_SET_FROM(from) | MOVE_SET_TO(to);
		p_board->occupied_bb = old_occupied_bb;
	}
	
	return next_move;
}
#endif /*0*/

