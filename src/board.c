#include <hooche.h>
//#include <fen.h>
#include <board.h>


const char* color_names[3] =
{
    "WHITE",
    "BLACK",
    "NONE"
};

const char* square_names[64] =
{
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

const char* attack_dir_names[4] =
{
    "HORIZONTAL ATTACK",
    "DIAG A1H8 ATTACK",
    "DIAG A8H1 ATTACK",
    "VERTICAL ATTACK"
};

const char piece_chars[3][8] =
{
    "PNBRQK ",
    "pnbrqk ",
    "       "
};

const char* piece_names[7] =
{
    "PAWN", "KNIGHT", "BISHOP", "ROOK", "QUEEN", "KING", "NO PIECE"
};


void board_init_bbs(board_t* me)
{
	piece_t p;
	square_t square;

    /* Reset all bitboards to zero. */
	me->color_bbs[WHITE] = (bitboard_t)0;
	me->color_bbs[BLACK] = (bitboard_t)0;
	me->occupied_bb = (bitboard_t)0;
	
    for (p = 0; p < 6; p++)
    {
		me->piece_bbs[p] = (bitboard_t)0;
    }
	
    me->piece_count[WHITE] = 0;
	me->piece_count[BLACK] = 0;

    /* Set bitboards according piece and color information. */
	for (square = A1; square < OFF_BOARD; square++)
    {
        if (me->color[square] != NO_COLOR)
        {
			BB_SET_SQ(me->occupied_bb, square);
			BB_SET_SQ(me->color_bbs[me->color[square]], square);
			BB_SET_SQ(me->piece_bbs[me->piece[square]], square);
			
            if (me->piece[square] != PAWN)
            {
				me->piece_count[me->color[square]]++;
            }
		}
	}

#ifdef _DEBUG
    /*
    bb_print(me->occupied_bb, "board_t::occupied_bb", NULL, NULL, NULL);
    */
#endif

    /* Set king squares. */ 
	me->king_square[WHITE] = first_square(me->piece_bbs[KING] & me->color_bbs[WHITE]);
	me->king_square[BLACK] = first_square(me->piece_bbs[KING] & me->color_bbs[BLACK]);
}


const char* square_tostr(const square_t me)
{
    assert(A1 <= me && me <= H8);
    return square_names[me];
}


void board_tostr(board_t* me, char* s)
{
    rank_t rank;
    char rankstr[19];

    strcpy(s, "");
    sprintf(rankstr, "+---------------+\n");
    strcat(s, rankstr);
    for (rank = RANK_8; rank >= RANK_1; --rank) {
        /* A bit ugly but works... */
        sprintf(rankstr, "|%c|%c|%c|%c|%c|%c|%c|%c|\n",
                piece_chars[me->color[rank_file_to_square(rank, FILE_A)]][me->piece[rank_file_to_square(rank, FILE_A)]],
                piece_chars[me->color[rank_file_to_square(rank, FILE_B)]][me->piece[rank_file_to_square(rank, FILE_B)]],
                piece_chars[me->color[rank_file_to_square(rank, FILE_C)]][me->piece[rank_file_to_square(rank, FILE_C)]],
                piece_chars[me->color[rank_file_to_square(rank, FILE_D)]][me->piece[rank_file_to_square(rank, FILE_D)]],
                piece_chars[me->color[rank_file_to_square(rank, FILE_E)]][me->piece[rank_file_to_square(rank, FILE_E)]],
                piece_chars[me->color[rank_file_to_square(rank, FILE_F)]][me->piece[rank_file_to_square(rank, FILE_F)]],
                piece_chars[me->color[rank_file_to_square(rank, FILE_G)]][me->piece[rank_file_to_square(rank, FILE_G)]],
                piece_chars[me->color[rank_file_to_square(rank, FILE_H)]][me->piece[rank_file_to_square(rank, FILE_H)]]);
        strcat(s, rankstr);
        if (rank != RANK_1) strcat(s, "|-+-+-+-+-+-+-+-|\n");
        else                strcat(s, "+---------------+\n");
    }
}


bitboard_t square_bbs[64];
bitboard_t pawn_caps_bbs[2][64];
bitboard_t knight_move_bbs[64];
bitboard_t king_move_bbs[64];
bitboard_t in_between_bbs[64][64];
bitboard_t castle_ks_bbs[2];
bitboard_t castle_qs_bbs[2];

static bitboard_t attack_bbs[64][4][64];
static bitboard_t attack_masks[64][4];

static void initialize_attacks(void);

/**
 * Kogge-Stone fill for given direction.
 */
bitboard_t fill_up_right(bitboard_t piece, bitboard_t empty)
{
	empty &= MASK_SHR;
	piece |= empty & (piece << 9);
	empty &= (empty << 9);
	piece |= empty & (piece << 18);
	empty &= (empty << 18);
	return piece | (empty & (piece << 36));
}


bitboard_t fill_up(bitboard_t piece, bitboard_t empty)
{
	piece |= empty & (piece << 8);
	empty &= (empty << 8);
	piece |= empty & (piece << 16);
	empty &= (empty << 16);
	return piece | (empty & (piece << 32));
}


bitboard_t fill_up_left(bitboard_t piece, bitboard_t empty)
{
	empty &= MASK_SHL;
	piece |= empty & (piece << 7);
	empty &= (empty << 7);
	piece |= empty & (piece << 14);
	empty &= (empty << 14);
	return piece | (empty & (piece << 28));
}


bitboard_t fill_right(bitboard_t piece, bitboard_t empty)
{
	empty &= MASK_SHR;
	piece |= empty & (piece << 1);
	empty &= (empty << 1);
	piece |= empty & (piece << 2);
	empty &= (empty << 2);
	return piece | (empty & (piece << 4));
}


bitboard_t fill_down_right(bitboard_t piece, bitboard_t empty)
{
	empty &= MASK_SHR;
	piece |= empty & (piece >> 7);
	empty &= (empty >> 7);
	piece |= empty & (piece >> 14);
	empty &= (empty >> 14);
	return piece | (empty & (piece >> 28));
}


bitboard_t fill_down(bitboard_t piece, bitboard_t empty)
{
	piece |= empty & (piece >> 8);
	empty &= (empty >> 8);
	piece |= empty & (piece >> 16);
	empty &= (empty >> 16);
	return piece | (empty & (piece >> 32));
}


bitboard_t fill_down_left(bitboard_t piece, bitboard_t empty)
{
	empty &= MASK_SHL;
	piece |= empty & (piece >> 9);
	empty &= (empty >> 9);
	piece |= empty & (piece >> 18);
	empty &= (empty >> 18);
	return piece | (empty & (piece >> 36));
}


bitboard_t fill_left(bitboard_t piece, bitboard_t empty)
{
	empty &= MASK_SHL;
	piece |= empty & (piece >> 1);
	empty &= (empty >> 1);
	piece |= empty & (piece >> 2);
	empty &= (empty >> 2);
	return piece | (empty & (piece >> 4));
}


void modinit_bitboard(bool* p_ok)
{
    int x;
	int y;
	/*int z;*/
	bitboard_t mask;
	color_t c;
	/*piece_t p;*/
	square_t s;

	bitboard_t (*fill_dir[8])(bitboard_t pieces, bitboard_t empty) =
	{
        fill_up_left,
        fill_up,
        fill_up_right,
        fill_right,
        fill_down_right,
        fill_down,
        fill_down_left,
        fill_left
	};

    /* Init the single square mask bitboards */
    for (s = A1; s < OFF_BOARD; s++)
    {
		square_bbs[s] = (bitboard_t)1 << s;
#ifdef _DEBUG
        /*bb_print(square_bbs[s], "square_bbs", square_names[s], NULL, NULL);*/
#endif
    }

	/* Init the attack bitboards */
	for (s = A1; s < OFF_BOARD; s++)
	{
		mask = SQ_MASK(s);
        knight_move_bbs[s] = SHIFT_UP(SHIFT_UL(mask)) |
			                 SHIFT_UP(SHIFT_UR(mask)) |
			                 SHIFT_DN(SHIFT_DL(mask)) |
			                 SHIFT_DN(SHIFT_DR(mask)) |
			                 SHIFT_LF(SHIFT_UL(mask)) |
			                 SHIFT_LF(SHIFT_DL(mask)) |
			                 SHIFT_RT(SHIFT_UR(mask)) |
			                 SHIFT_RT(SHIFT_DR(mask));
#ifdef _DEBUG
        /*bb_print(knight_move_bbs[s], "knight_move_bbs", square_names[s], NULL, NULL);*/
#endif
        king_move_bbs[s] = SHIFT_UL(mask) |
                           SHIFT_UP(mask) |
			               SHIFT_UR(mask) |
                           SHIFT_RT(mask) |
                           SHIFT_DR(mask) |
			               SHIFT_DN(mask) |
                           SHIFT_DL(mask) |
                           SHIFT_LF(mask);
#ifdef _DEBUG
        /*bb_print(king_move_bbs[s], "king_move_bbs", square_names[s], NULL, NULL);*/
#endif

        for (x = A1; x < OFF_BOARD; x++)
		{
			mask = (bitboard_t)0;
			/* Inefficient: try all directions and only if they match up set the bitboard_t. */
			for (y = 0; y < 8; y++)
            {
                mask |= fill_dir[y](SQ_MASK(s), ~(bitboard_t)0) & fill_dir[(y + 4) & 7](SQ_MASK(x), ~(bitboard_t)0);
            }
			in_between_bbs[s][x] = mask & ~SQ_MASK(s) & ~SQ_MASK(x);
#ifdef _DEBUG
            /*bb_print(in_between_bbs[s][x], "in_between_bbs", square_names[s], square_names[x], NULL);*/
#endif
		}

		for (c = WHITE; c <= BLACK; c++)
		{
			mask = SQ_MASK(s);
			mask = SHIFT_FORWARD(mask, c);
			mask = SHIFT_LF(mask) | SHIFT_RT(mask);
            pawn_caps_bbs[c][s] = mask;
#ifdef _DEBUG
            /*bb_print(pawn_caps_bbs[c][s], "pawn_caps_bbs", color_names[c], square_names[s], NULL);*/
#endif
		}

	}

	castle_ks_bbs[WHITE] = SQ_MASK(F1) | SQ_MASK(G1);
	castle_ks_bbs[BLACK] = SQ_MASK(F8) | SQ_MASK(G8);
	castle_qs_bbs[WHITE] = SQ_MASK(D1) | SQ_MASK(C1) | SQ_MASK(B1);
	castle_qs_bbs[BLACK] = SQ_MASK(D8) | SQ_MASK(C8) | SQ_MASK(B8);

    initialize_attacks();

    (void)p_ok;
}


/**
    Returns the state of the masked rank or diagonal as a 6-bit integer.
 */
int horizontal_attack_index(bitboard_t occ)
{
	unsigned int u = (unsigned int)(occ | occ >> 32);
	return u * 0x02020202 >> 26;
}


/**
    Returns the state of the masked file as a 6-bit integer.
 */
int vertical_attack_index(bitboard_t occ)
{
	unsigned int u = (unsigned int)(occ | (occ >> 32) << 3);
	return u * 0x01041041 >> 26;
}


/**
    Sets up the attack_mask and attack_bb arrays for the bitboard attack generation functions.
 */
static void initialize_attacks(void)
{
	bitboard_t mask;
	bitboard_t submask;
	square_t square;

	for (square = 0; square < OFF_BOARD; square++)
	{
		/* Calculate horizontal attack masks. */
		attack_masks[square][HORIZONTAL_ATTACK] =
            MASK_RANK(square_get_rank(square)) & ~(MASK_FILE(FILE_A) | MASK_FILE(FILE_H));

#ifdef _DEBUG
        /*
        bb_print(attack_masks[square][HORIZONTAL_ATTACK],
                 "attack_masks",
                 square_names[square],
                 attack_dir_names[HORIZONTAL_ATTACK],
                 NULL);
        */
#endif /*_DEBUG*/

		/* Calculate A1H8 diagonal attack masks */
		if (square_get_file(square) > square_get_rank(square))
        {
			attack_masks[square][DIAG_A1H8_ATTACK] =
                0x0040201008040200ull >> ((FILE_OF_SQ(square) - RANK_OF_SQ(square)) * 8);
        }
		else
        {
			attack_masks[square][DIAG_A1H8_ATTACK] =
                0x0040201008040200ull << ((RANK_OF_SQ(square) - FILE_OF_SQ(square)) * 8);
        }
		attack_masks[square][DIAG_A1H8_ATTACK] &= ~(MASK_RANK(RANK_1) | MASK_RANK(RANK_8));

#ifdef _DEBUG
        /*
        bb_print(attack_masks[square][DIAG_A1H8_ATTACK],
                 "attack_masks",
                 square_names[square],
                 attack_dir_names[DIAG_A1H8_ATTACK],
                 NULL);
        */
#endif /*_DEBUG*/

		/* Calculate the attack mask for A8H1 diagonals. */
		if (FILE_OF_SQ(square) + RANK_OF_SQ(square) < 8)
        {
			attack_masks[square][DIAG_A8H1_ATTACK] =
                0x0002040810204000ull >> ((7 - FILE_OF_SQ(square) - RANK_OF_SQ(square)) * 8);
        }
		else
        {
			attack_masks[square][DIAG_A8H1_ATTACK] =
                0x0002040810204000ull << ((FILE_OF_SQ(square) + RANK_OF_SQ(square) - 7) * 8);
        }
		attack_masks[square][DIAG_A8H1_ATTACK] &= ~(MASK_RANK(RANK_1) | MASK_RANK(RANK_8));

#ifdef _DEBUG
        /*
        bb_print(attack_masks[square][DIAG_A8H1_ATTACK],
                 "attack_masks",
                 square_names[square],
                 attack_dir_names[DIAG_A8H1_ATTACK],
                 NULL);
        */
#endif /*_DEBUG*/

		/* Calculate vertical attack masks. */
		attack_masks[square][VERTICAL_ATTACK] =
            MASK_FILE(FILE_OF_SQ(square)) & ~(MASK_RANK(RANK_1) | MASK_RANK(RANK_8));

#ifdef _DEBUG
        /*
        bb_print(attack_masks[square][VERTICAL_ATTACK],
                 "attack_masks",
                 square_namesr[square],
                 attack_dir_names[VERTICAL_ATTACK],
                 NULL);
        */
#endif /*_DEBUG*/

        /*
		 * For each direction, loop over all possible bit-states, and produce an attack board. This uses
		 * Steffen Westcott's subset traversing algorithm.
         */

        /* Horizontal attacks. */
		mask = attack_masks[square][HORIZONTAL_ATTACK];
		submask = 0;
		do
		{
			submask = (submask - mask) & mask;
			attack_bbs[square][HORIZONTAL_ATTACK][horizontal_attack_index(submask)] =
                SHIFT_RT(fill_right(SQ_MASK(square), ~submask)) |
                SHIFT_LF(fill_left(SQ_MASK(square), ~submask));
		} while (submask);

        /* A8H1 attacks. */
		mask = attack_masks[square][DIAG_A1H8_ATTACK];
		submask = 0;
		do
		{
			submask = (submask - mask) & mask;
			attack_bbs[square][DIAG_A1H8_ATTACK][horizontal_attack_index(submask)] =
				SHIFT_DL(fill_down_left(SQ_MASK(square), ~submask)) |
                SHIFT_UR(fill_up_right(SQ_MASK(square), ~submask));
		} while (submask);

        /* A1H8 attacks. */
		mask = attack_masks[square][DIAG_A8H1_ATTACK];
		submask = 0;
		do
		{
			submask = (submask - mask) & mask;
			attack_bbs[square][DIAG_A8H1_ATTACK][horizontal_attack_index(submask)] =
				SHIFT_DR(fill_down_right(SQ_MASK(square), ~submask)) |
                SHIFT_UL(fill_up_left(SQ_MASK(square), ~submask));
		} while (submask);

        /* File attacks. */
		mask = attack_masks[square][VERTICAL_ATTACK];
		submask = 0;
		do
		{
			submask = (submask - mask) & mask;
			attack_bbs[square][VERTICAL_ATTACK][vertical_attack_index(submask)] =
				SHIFT_UP(fill_up(SQ_MASK(square), ~submask)) |
                SHIFT_DN(fill_down(SQ_MASK(square), ~submask));
		} while (submask);
	}
}


/**
    Returns an attack bitboard for the given sliding piece on the given square.
 */
bitboard_t bb_get_attack(board_t* p_board, piece_t piece, square_t square)
{
	switch (piece)
	{
		case PAWN:
			break;
		case KNIGHT:
			return knight_move_bbs[square];
		case BISHOP:
            return diagA1H8_attacks(square, p_board->occupied_bb) |
                   diagA8H1_attacks(square, p_board->occupied_bb);
		case ROOK:
			return horizontal_attacks(square, p_board->occupied_bb) |
                   vertical_attacks(square, p_board->occupied_bb);
		case QUEEN:
			return diagA1H8_attacks(square, p_board->occupied_bb) |
                   diagA8H1_attacks(square, p_board->occupied_bb) |
                   horizontal_attacks(square, p_board->occupied_bb) |
                   vertical_attacks(square, p_board->occupied_bb);
		case KING:
			return king_move_bbs[square];
		default:
			break;
	}
	return (bitboard_t)0;
}


bitboard_t horizontal_attacks(square_t from, bitboard_t occupied)
{
	return attack_bbs[from][HORIZONTAL_ATTACK][horizontal_attack_index(occupied & attack_masks[from][HORIZONTAL_ATTACK])];
}


bitboard_t vertical_attacks(square_t from, bitboard_t occupied)
{
	return attack_bbs[from][VERTICAL_ATTACK][vertical_attack_index(occupied & attack_masks[from][VERTICAL_ATTACK])];
}


bitboard_t diagA1H8_attacks(square_t from, bitboard_t occupied)
{
	return attack_bbs[from][DIAG_A1H8_ATTACK][horizontal_attack_index(occupied & attack_masks[from][DIAG_A1H8_ATTACK])];
}


bitboard_t diagA8H1_attacks(square_t from, bitboard_t occupied)
{
	return attack_bbs[from][DIAG_A8H1_ATTACK][horizontal_attack_index(occupied & attack_masks[from][DIAG_A8H1_ATTACK])];
}


/**
    Returns the first square that is set in a bitboard.
    This 32-bit friendly routine was devised by Matt Taylor.
 */
square_t first_square(bitboard_t bitboard)
{
	/* A DeBruijn index table. */
	static const square_t lsb_64_table[64] =
	{
		63, 30,  3, 32, 59, 14, 11, 33,
		60, 24, 50,  9, 55, 19, 21, 34,
		61, 29,  2, 53, 51, 23, 41, 18,
		56, 28,  1, 43, 46, 27,  0, 35,
		62, 31, 58,  4,  5, 49, 54,  6,
		15, 52, 12, 40,  7, 42, 45, 16,
		25, 57, 48, 13, 10, 39,  8, 44,
		20, 47, 38, 22, 17, 37, 36, 26
	};
	unsigned int folded;

	bitboard ^= (bitboard - 1);
	folded = (unsigned int)(bitboard ^ (bitboard >> 32));
	return lsb_64_table[folded * 0x78291ACF >> 26];
}


void bb_print(
    bitboard_t bb,
    const char* bbname,
    const char* i1,
    const char* i2,
    const char* i3)
{
    square_t s;

    printf("Bitboard %s", bbname);
    /* TODO: Do with varargs instead */
    if (i1 != NULL) printf("[%s]", i1);
    if (i2 != NULL) printf("[%s]", i2);
    if (i3 != NULL) printf("[%s]", i3);
    printf("\n");
    for (s = A8; s <= H8; ++s) printf("%d", BB_TEST_SQ(bb, s) ? 1 : 0);
    printf("\n");
    for (s = A7; s <= H7; ++s) printf("%d", BB_TEST_SQ(bb, s) ? 1 : 0);
    printf("\n");
    for (s = A6; s <= H6; ++s) printf("%d", BB_TEST_SQ(bb, s) ? 1 : 0);
    printf("\n");
    for (s = A5; s <= H5; ++s) printf("%d", BB_TEST_SQ(bb, s) ? 1 : 0);
    printf("\n");
    for (s = A4; s <= H4; ++s) printf("%d", BB_TEST_SQ(bb, s) ? 1 : 0);
    printf("\n");
    for (s = A3; s <= H3; ++s) printf("%d", BB_TEST_SQ(bb, s) ? 1 : 0);
    printf("\n");
    for (s = A2; s <= H2; ++s) printf("%d", BB_TEST_SQ(bb, s) ? 1 : 0);
    printf("\n");
    for (s = A1; s <= H1; ++s) printf("%d", BB_TEST_SQ(bb, s) ? 1 : 0);
    printf("\n");
    printf("\n");
}
