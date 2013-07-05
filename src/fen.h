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


#ifndef FEN_H_
#define FEN_H_

#include "hooche.h"
#include "game.h"


/* Maximum allowed FEN string legth.
 * This is more than necessary, but allowes for some extra
 * whitespace if present
 */
#define MAX_FEN_LEN 127

t_bool  fen_parse(const char* fen, t_game* p_game);
char    fen_piece_char(t_color color, t_piece piece);
t_piece fen_piece(char c);
t_color fen_color(char c);

#endif /*FEN_H_*/
