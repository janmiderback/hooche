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
#include "board.h"
#include "fen.h"

static t_bool fen_parse_pieces(const char* pieces, t_game* p_game);
static t_bool fen_parse_stm(const char* stm, t_game* p_game);
static t_bool fen_parse_castling(const char* castling, t_game* p_game);
static t_bool fen_parse_ep(const char* ep, t_game* p_game);
static t_bool fen_parse_halfmv(const char* halfmv, t_game* p_game);
static t_bool fen_parse_fullmv(const char* fullmv, t_game* p_game);
static t_square fen_square(const char* sq);


t_bool fen_parse(const char* fen, t_game* p_game)
{
    char       fen_cpy[MAX_FEN_LEN + 1];
    const char castle_string[5] = "KkQq";
    char*      fen_field;
    t_board*   p_board = &(p_game->board);
    t_square   sq;
    
    assert(fen != NULL);

    (void)castle_string;

	/* Initialize. */
	for (sq = A1; sq < OFF_BOARD; ++sq)
	{
		p_board->piece[sq] = NO_PIECE;
		p_board->color[sq] = NO_COLOR;
	}

	p_game->castle_rights = CASTLE_NO;
	p_board->ep_square = OFF_BOARD;
	p_game->fifty_count = 0;
	p_game->move_number = 1;

    if (strlen(fen) > MAX_FEN_LEN) return FALSE;
    
    /* Make a copy since fen_parse uses strtok to parse the string */
    strcpy(fen_cpy, fen);

    /* Pieces */
    fen_field = strtok(fen_cpy, " \t");
    if (!fen_parse_pieces(fen_field, p_game)) goto FEN_PARSE_ERROR;

    /* Active color (side to move) */
    fen_field = strtok(NULL, " \t");
    if (!fen_parse_stm(fen_field, p_game)) goto FEN_PARSE_ERROR;

    /* Castling availability */
    fen_field = strtok(NULL, " \t");
    if (!fen_parse_castling(fen_field, p_game)) goto FEN_PARSE_ERROR;

    /* En passant */
    fen_field = strtok(NULL, " \t");
    if (!fen_parse_ep(fen_field, p_game)) goto FEN_PARSE_ERROR;

    /* Halfmove clock */
    fen_field = strtok(NULL, " \t");
    if (!fen_parse_halfmv(fen_field, p_game)) goto FEN_PARSE_ERROR;

    /* Fullmove number */
    fen_field = strtok(NULL, " \t");
    if (!fen_parse_fullmv(fen_field, p_game)) goto FEN_PARSE_ERROR;

    return TRUE;

FEN_PARSE_ERROR:
    return FALSE;
}


static t_bool fen_parse_pieces(const char* pieces, t_game* p_game)
{
    t_board*    p_board = &(p_game->board);
    t_rank      rank = RANK_8;
    t_file      file = FILE_A;
    const char* p = pieces;
    t_piece     piece;
    t_square    sq = A1;
    char        c;

    while (c = *p++)
    {
        if (c == '/' || c == '\\') {
            /* New rank */
            if (rank == RANK_1) return FALSE;  /* Must not pass rank 1 */
            --rank;
            file = FILE_A;
        }
        else if ('1' <= c && c <= '8') {
            /* A number of empty squares */
            int n;
            for (n = c - '0'; n > 0; --n) {
                if (file > FILE_H) return FALSE;  /* Must not pass file h */
                sq = RANK_FILE_TO_SQUARE(rank, file);
                p_board->piece[sq] = NO_PIECE;
				p_board->color[sq] = NO_COLOR;
		        ++file;
            }
        }
        else if ((piece = fen_piece(c)) != NO_PIECE) {
            /* A piece character */
            if (file > FILE_H) return FALSE;  /* Must not pass file h */
            sq = RANK_FILE_TO_SQUARE(rank, file);
            p_board->piece[sq] = piece;
            p_board->color[sq] = fen_color(c);
            ++file;
        }
        else {
            /* Unknown char */
            return FALSE;
        }
    }

    return TRUE;
}


static t_bool fen_parse_stm(const char* stm, t_game* p_game)
{
    if (strcmp(stm, "w") == 0)     p_game->side_tm = WHITE;
    else if(strcmp(stm, "b") == 0) p_game->side_tm = BLACK;
    else return FALSE;
    p_game->side_ntm = COLOR_FLIP(p_game->side_tm);
    return TRUE;
}


static t_bool fen_parse_castling(const char* castling, t_game* p_game)
{
    t_bool white_ks = FALSE;
    t_bool white_qs = FALSE;
    t_bool black_ks = FALSE;
    t_bool black_qs = FALSE;
    t_bool none = FALSE;
    const char* p = castling;
    char c;

    assert(castling != NULL);
    assert(p_game != NULL);
    p_game->castle_rights = CASTLE_NO;

    while (c = *p++) {
        switch (c) {
            case 'K':
                white_ks = TRUE;
                break;
            case 'Q':
                white_qs = TRUE;
                break;
            case 'k':
                black_ks = TRUE;
                break;
            case 'q':
                black_qs = TRUE;
                break;
            case '-':
                none = TRUE;
                break;
            default:
                return FALSE;
                break;
        }
    }

    if (none && (white_ks || white_qs || black_ks || black_qs)) {
        /* Cannot have none together with some castling right. */
        return FALSE;
    }
    
    if (white_ks) p_game->castle_rights |= CASTLE_WK;
    if (white_qs) p_game->castle_rights |= CASTLE_WQ;
    if (black_ks) p_game->castle_rights |= CASTLE_BK;
    if (white_qs) p_game->castle_rights |= CASTLE_BQ;

    return TRUE;
}


static t_bool fen_parse_ep(const char* ep, t_game* p_game)
{
    if (strcmp(ep, "-") == 0) {
        p_game->board.ep_square = OFF_BOARD;
        return TRUE;
    }
    p_game->board.ep_square = fen_square(ep);
    return p_game->board.ep_square != OFF_BOARD;
}


static t_bool fen_parse_halfmv(const char* halfmv, t_game* p_game)
{
    /* atoi is not perfect. Consider rewrite */
    p_game->fifty_count = atoi(halfmv);
    return TRUE;
}


static t_bool fen_parse_fullmv(const char* fullmv, t_game* p_game)
{
    /* atoi is not perfect. Consider rewrite */
    p_game->move_number = atoi(fullmv);
    return TRUE;
}


char fen_piece_char(t_color color, t_piece piece)
{
    static const char white_chars[] = "PNBRQK";
    static const char black_chars[] = "pnbrqk";

    if (WHITE <= color && color <= BLACK &&
        PAWN <= piece && piece <= KING)
    {
        return (color == WHITE) ? white_chars[piece] : black_chars[piece];
    }

    return 0;
}


t_piece fen_piece(char c)
{
    if (c == 'P' || c == 'p') return PAWN;
    if (c == 'N' || c == 'n') return KNIGHT;
    if (c == 'B' || c == 'b') return BISHOP;
    if (c == 'R' || c == 'r') return ROOK;
    if (c == 'Q' || c == 'q') return QUEEN;
    if (c == 'K' || c == 'k') return KING;
    return NO_PIECE;
}


t_color fen_color(char c)
{
    if (c == 'P' ||
        c == 'N' ||
        c == 'B' ||
        c == 'R' ||
        c == 'Q' ||
        c == 'K')
    {
        return WHITE;
    }
    else if (c == 'p' ||
             c == 'n' ||
             c == 'b' ||
             c == 'r' ||
             c == 'q' ||
             c == 'k')
    {
        return BLACK;
    }

    return NO_COLOR;
}


static t_square fen_square(const char* sq)
{   
    const t_file file = sq[0] - 'a';
    const t_rank rank = sq[1] - '1';

    if ((FILE_A <= file) && (file <= FILE_H) &&
        (RANK_1 <= rank) && (rank <= RANK_8))
    {
        return RANK_FILE_TO_SQUARE(rank, file);
    }

    return OFF_BOARD;
}
