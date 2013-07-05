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
#include "piece.h"

static const char* white_piece_names[] = { "P", "N", "B", "R", "Q", "K", "" };
static const char* black_piece_names[] = { "p", "n", "b", "r", "q", "k", "" };

const char* piece_tostr(const t_piece me)
{
    return piece_tostr_c(me, BLACK);
}


const char* piece_tostr_c(const t_piece me, t_color color)
{
    assert(PAWN <= me && me <= NO_PIECE);
    assert(color == WHITE || color == BLACK);
    return (color == WHITE) ? white_piece_names[me] : black_piece_names[me];
}
