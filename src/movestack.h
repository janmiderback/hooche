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

#ifndef MOVESTACK_H_
#define MOVESTACK_H_

#include "hooche.h"
#include "move.h"

#define MOVESTACK_SIZE  (128 * 128)

typedef struct {
    t_move moves[MOVESTACK_SIZE];
    int    top;
} t_movestack;

void movestack_init(t_movestack* me);

t_move* movestack_top(t_movestack* me);

t_move* movestack_getarrayfromtop(t_movestack* me, int n);

void movestack_push(t_movestack* me);

void movestack_pop(t_movestack* me);

void movestack_popn(t_movestack* me, int n);

int movestack_size(t_movestack* me);


#endif /*MOVESTACK_H_*/
