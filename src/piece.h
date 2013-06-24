/**
 * Copyright 2010 Jan Miderbäck
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

#ifndef PIECE_H_
#define PIECE_H_

#include "color.h"

typedef signed char t_piece;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NO_PIECE };

const char* piece_tostr(const t_piece me);
const char* piece_tostr_c(const t_piece me, t_color color);

#endif /*PIECE_H_*/
