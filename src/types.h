#ifndef TYPES_H_
#define TYPES_H_

/*
 * Chess-specific types.
 */




/*
 *   Bit 0-5:   'To' square
 *   Bit 6-11:  'From' square
 *   Bit 12-14: Promotion piece
 *   Bit 15-31: Move score
 */

/*typedef unsigned int t_move;*/


#define MAX_MOVES 256

/* Attack direction */
typedef int8_t t_attack_dir;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum
{
    HORIZONTAL_ATTACK,
    DIAG_A1H8_ATTACK,
    DIAG_A8H1_ATTACK,
    VERTICAL_ATTACK
};

/**
 * Castling rights bitfield.
 * 
 * Bit
 * 0    White can castle king-side
 * 1    Black can castle king-side
 * 2    White can castle queen-side
 * 3    Black can castle queen-side
 */
typedef uint8_t t_castle_rights;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum {
    CASTLE_NO = 0,
    CASTLE_WK = 1 << 0,
    CASTLE_BK = 1 << 1,
    CASTLE_WQ = 1 << 2,
    CASTLE_BQ = 1 << 3,
    CASTLE_WB = CASTLE_WK | CASTLE_WQ,
	CASTLE_BB = CASTLE_BK | CASTLE_BQ,
    CASTLE_ALL = CASTLE_WK | CASTLE_WQ | CASTLE_BK | CASTLE_BQ
};
#define CAN_CASTLE_KS(c, s) (((c) >> (s)) & 1)
#define CAN_CASTLE_QS(c, s) (((c) >> (s)) & 4)
#define CAN_CASTLE(c, s)    (((c) >> (s)) & 5)























/*
 * Engine types.
 */

typedef unsigned char t_eng_state;
enum {
    STATE_IDLE,
    STATE_SEARCHING
};

typedef unsigned char t_eng_iomode;
enum {
    IOMODE_NORMAL,
    IOMODE_UCI
};

typedef unsigned char t_eng_searchmode;
enum {
    SEARCHMODE_NORMAL,
    SEARCHMODE_PONDERING,
    SEARCHMODE_INFINITE
};





#endif /*TYPES_H_*/
