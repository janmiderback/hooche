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

#ifndef MOVE_H_
#define MOVE_H_

#include "hooce.h"
#include "board.h"

typedef struct {
    t_square from;
    t_square to;
    t_piece  promote_piece;
    int      score;
} t_move;

#define IS_NULL_MOVE(m)  (m.from == -1 && m.to == -1)
#define SET_NULL_MOVE(m) do { m.from = -1; m.to = -1; } while (0)

/*
 * Macros to set bits in a move for the different fields of a move.
 * It is intended to be used as:
 *
 * move = MOVE_SET_FROM(from) | MOVE_SET_TO(to) | MOVE_SET_PROMOTE(KNIGHT);
 *
 */
#define MOVE_SET_TO(t)      (t)
#define MOVE_SET_FROM(f)    ((f) << 6)
#define MOVE_SET_PROMOTE(p)	((p) << 12)
#define MOVE_SET_SCORE(s)   ((s) << 15)

/*
 * Macros to get the different fields of a move.
 * It is intended to be used as:
 *
 * square_to = MOVE_GET_TO(move);
 * square_from = MOVE_GET_FROM(move);
 * promote_piece = MOVE_GET_PROMOTE(move);
 * 
 */
//#define MOVE_GET_TO(m)      ((m) & 0x3F)
//#define MOVE_GET_FROM(m)    (((m) >> 6) & 0x3F)
//#define MOVE_GET_PROMOTE(m) (((m) >> 12) & 0x07)
//#define MOVE_GET_SCORE(m)   ((m) >> 15)

#define MAX_SCORE ((1 << 16) - 1)

/* ??? */
#define MOVE_COMPARE(m) ((m) & 0x7FFF)
#define MOVE_KEY(m)     ((m) & 0x0FFF)

#define PAWN_DIR(c) (((c) == WHITE) ? 8 : -8)

void  move_set(t_move* me, t_square from, t_square to, t_piece promote_piece);
void  move_fromstr(t_move* me, const char* s);
char* move_tostr(const t_move* me, char* s);


#endif /*MOVE_H_*/
