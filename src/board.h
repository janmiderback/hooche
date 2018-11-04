#ifndef BOARD_H_
#define BOARD_H_

#include "types.h"

/*
void board_gen_moves(
*/

extern const char* square_names[64];
extern const char* color_names[3];
extern const char  piece_chars[3][8];
extern const char* piece_names[7];
extern const char* attack_dir_names[4];


/**
 * Type representing a chessboard square.
 * Takes the integer enumeration values A1 to H8.
 */
typedef int8_t square_t;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum
{
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

/**
 * Returns the string representation of a square (e.g. "a1").
 */
#define square_to_str(me) (square_names[(me)])

/**
 * TODO
 */
#define square_get_file(me)  ((file_t)(me) & 7)

/**
 * TODO
 */
#define square_get_rank(me)  ((rank_t)(me) >> 3)


/**
 * TODO
 */
typedef int8_t file_t;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum
{
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_OFF
};


/**
 * TODO
 */
typedef int8_t rank_t;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum
{ 
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_OFF
};

/**
 * TODO
 */
#define rank_file_to_square(rank, file) ((square_t)((rank) << 3) | (file))





/**
 * Type representing square or piece color.
 * Takes the integer enumeration values WHITE, BLACK, or NO_COLOR.
 */
typedef int8_t color_t;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum
{
    WHITE,
    BLACK,
    NO_COLOR
};

/**
 * TODO
 */
#define color_flip(me) ((me) ^ 1)


typedef int8_t piece_t;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum
{
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NO_PIECE
};

/**
 * TODO
 */
#define piece_to_str(me) (piece_names[(me)])

/**
 * TODO
 */
#define piece_to_char(me, color) (piece_chars[(color)][(me)])


/**
 * Type for bitboard (64-bit).
 */
typedef uint64_t bitboard_t;


/**
 * Type representing a board position.
 */
typedef struct
{
    /**
     * Array of piece bitboards.
     * Gives, per piece, the occupation status of the board.
     */
	bitboard_t piece_bbs[6];
    
	bitboard_t color_bbs[2];
	bitboard_t occupied_bb;
	piece_t    piece[64];
	color_t    color[64];
	square_t   king_square[2];
	int        piece_count[2];
	square_t   ep_square;

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
} board_t;






#define BOARD_STR_SIZE 1024


/**
 * TODO
 */
void board_init(board_t* me);

/**
 * TODO
 */
bool board_init_fen(board_t* me, const char* fen);

/**
 * TODO
 */
void board_init_bbs(board_t* me);

/**
 * TODO
 */
void board_tostr(board_t* me, char* s);




/* Single square bitboards */
extern bitboard_t square_bbs[64];

/* Pawn capture bitboards */
extern bitboard_t pawn_caps_bbs[2][64];

/* Knight move bitboards */
extern bitboard_t knight_move_bbs[64];

/* King move bitboards */
extern bitboard_t king_move_bbs[64];

/* TODO */
extern bitboard_t castle_ks_bbs[2];

/*TODO */
extern bitboard_t castle_qs_bbs[2];


/**
 * Prints a bitboard
 */
void bb_print(
    bitboard_t bb,
    const char* bbname,
    const char* i1,
    const char* i2,
    const char* i3);

/**
 * Returns an attack bitboard for the given sliding piece on the given square.
 * Created 070305; last modified 110607
 */
bitboard_t bb_get_attack(board_t* p_board, piece_t piece, square_t square);


/**
 * TODO
 */
bitboard_t horizontal_attacks(square_t from, bitboard_t occupied);


/**
 * TODO
 */
bitboard_t vertical_attacks(square_t from, bitboard_t occupied);


/**
 * TODO
 */
bitboard_t diagA1H8_attacks(square_t from, bitboard_t occupied);


/**
 * TODO
 */
bitboard_t diagA8H1_attacks(square_t from, bitboard_t occupied);


/**
first_square():
Returns the first square that is set in a bitboard. This 32-bit friendly routine was devised by Matt Taylor.
Created 070105; last modified 120205
**/
square_t first_square(bitboard_t bitboard);


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
