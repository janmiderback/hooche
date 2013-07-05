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

#ifndef BOARD_H_
#define BOARD_H_

#include "board.h"
#include "square.h"
#include "piece.h"
#include "color.h"
#include "types.h"

/*
void board_gen_moves(
*/

/*
 * Types
 */

typedef signed char t_square;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    OFF_BOARD
};

typedef uint64_t t_bitboard;

typedef struct
{
    /**
     * Array of piece bitboards.
     * Gives, per piece, the occupation status of the board.
     */
	t_bitboard piece_bbs[6];
    
	t_bitboard color_bbs[2];
	t_bitboard occupied_bb;
	t_piece    piece[64];
	t_color    color[64];
	t_square   king_square[2];
	int        piece_count[2];
	t_square   ep_square;

    /*
	HASHKEY hashkey;
	HASHKEY path_hashkey;
	
	GAME_ENTRY *game_entry;
	GAME_ENTRY game_stack[GAME_STACK_SIZE];
	MOVE move_stack[MOVESTACK_SIZE];
	MOVE pv_stack[MAX_PLY][MAX_PLY];
	SEARCH_BLOCK search_stack[MAX_PLY];
	MOVE threat_move[MAX_PLY + 1];
	EVAL_BLOCK eval_stack[MAX_PLY];
	PAWN_HASH_ENTRY pawn_hash_entry;
    */
} t_board;

/**
 * Array of square names.
 */
extern const char* square_names[64];

/**
 * Array of color names.
 * "WHITE", "BLACK", "NONE".
 */
extern const char* color_names[3];

/*extern const char* piece_names[*/
extern const char* attack_dir_names[4];


/* Some board-transforming stuff */
#define RANK_FILE_TO_SQUARE(r, f) ((t_square)((r) << 3) | (f))
#define FILE_OF_SQ(s)  ((t_file)(s) & 7)
#define RANK_OF_SQ(s)  ((t_rank)(s) >> 3)

#define BOARD_STR_SIZE 1024

/**
 * TODO
 */
void board_init(t_board* me);

/**
 * TODO
 */
t_bool board_init_fen(t_board* me, const char* fen);

/**
 * TODO
 */
void board_init_bbs(t_board* me);

/**
 * TODO
 */
void board_tostr(t_board* me, char* s);

/**
 * TODO
 */
const char* square_tostr(const t_square me);



/* Single square bitboards */
extern t_bitboard square_bbs[64];

/* Pawn capture bitboards */
extern t_bitboard pawn_caps_bbs[2][64];

/* Knight move bitboards */
extern t_bitboard knight_move_bbs[64];

/* King move bitboards */
extern t_bitboard king_move_bbs[64];

/* TODO */
extern t_bitboard castle_ks_bbs[2];

/*TODO */
extern t_bitboard castle_qs_bbs[2];


/**
 * Print a bitboard
 */
void bb_print(
    t_bitboard bb,
    const char* bbname,
    const char* i1,
    const char* i2,
    const char* i3);

/**
 * Returns an attack bitboard for the given sliding piece on the given square.
 * Created 070305; last modified 110607
 */
t_bitboard bb_get_attack(t_board* p_board, t_piece piece, t_square square);


/**
 * TODO
 */
t_bitboard horizontal_attacks(t_square from, t_bitboard occupied);


/**
 * TODO
 */
t_bitboard vertical_attacks(t_square from, t_bitboard occupied);


/**
 * TODO
 */
t_bitboard diagA1H8_attacks(t_square from, t_bitboard occupied);


/**
 * TODO
 */
t_bitboard diagA8H1_attacks(t_square from, t_bitboard occupied);


/**
first_square():
Returns the first square that is set in a bitboard. This 32-bit friendly routine was devised by Matt Taylor.
Created 070105; last modified 120205
**/
t_square first_square(t_bitboard bitboard);


/* Mask for square s. One single 1 on square */
#define SQ_MASK(s)  (square_bbs[s])

#define BB_SET_SQ(b, s)   ((b) |= SQ_MASK(s))
#define BB_CLEAR_SQ(b, s) ((b) &= ~SQ_MASK(s))
#define BB_TEST_SQ(b, s)  (((b) & SQ_MASK(s)) != 0)

#define MASK_RANK(r)			(0x00000000000000FFull << ((r) << 3))
#define MASK_FILE(f)			(0x0101010101010101ull << (f))
#define MASK_RANK_COLOR(r, c)	(0x00000000000000FFull << (((r) ^ 7 * (c)) << 3))

#define MASK_SHL				(~MASK_FILE(FILE_H))
#define MASK_SHR				(~MASK_FILE(FILE_A))

#define SHIFT_UL(b)				(((b) << 7) & MASK_SHL)
#define SHIFT_UP(b)				((b) << 8)
#define SHIFT_UR(b)				(((b) << 9) & MASK_SHR)
#define SHIFT_RT(b)				(((b) << 1) & MASK_SHR)
#define SHIFT_DR(b)				(((b) >> 7) & MASK_SHR)
#define SHIFT_DN(b)				((b) >> 8)
#define SHIFT_DL(b)				(((b) >> 9) & MASK_SHL)
#define SHIFT_LF(b)				(((b) >> 1) & MASK_SHL)
#define SHIFT_FORWARD(b, s)		((s) == WHITE ? SHIFT_UP(b) : SHIFT_DN(b))

#define BISHOP_ATTACKS(s, o)	(diagA1H8_attacks((s), (o)) | diagA8H1_attacks((s), (o)))
#define ROOK_ATTACKS(s, o)		(horizontal_attacks((s), (o)) | vertical_attacks((s), (o)))
#define QUEEN_ATTACKS(s, o)		(BISHOP_ATTACKS((s), (o)) | ROOK_ATTACKS((s), (o)))


#endif /*BOARD_H_*/
