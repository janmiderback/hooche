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

#ifndef COLOR_H_
#define COLOR_H_

typedef signed char t_color;
/* NOTE: These are not just named values. Lots of things are based on the
 * particular value of each entry in the enum. Be careful to change!
 */
enum { WHITE, BLACK, NO_COLOR };
/*#define COLOR_FLIP(s) ((s) ^ 1)*/

#define COLOR_FLIP(s) ((s == WHITE) ? BLACK : ((s == BLACK) ? WHITE : NO_COLOR))


#endif /*COLOR_H_*/
