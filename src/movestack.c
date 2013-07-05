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
#include "movestack.h"  /* Myself */


void movestack_init(t_movestack* me)
{
    memset(me, 0, sizeof(*me));
    me->top = -1;
}


t_move* movestack_top(t_movestack* me)
{
    assert(me != NULL);
    if (me->top >= 0) return &(me->moves[me->top]);
    else return NULL; 
}


t_move* movestack_getarrayfromtop(t_movestack* me, int n)
{
    assert(me != NULL);
    assert(me->top >= (n - 1));
    return &(me->moves[me->top - (n - 1)]);
}


void movestack_push(t_movestack* me)
{
    assert(me != NULL);
    assert(me->top < (MOVESTACK_SIZE - 1));
    (me->top)++;
}


void movestack_pop(t_movestack* me)
{
    assert(me != NULL);
    assert(me->top >= 0);
    (me->top)--;
}


void movestack_popn(t_movestack* me, int n)
{
    assert(me != NULL);
    assert(me->top >= (n - 1));
    me->top -= n;
}


int movestack_size(t_movestack* me)
{
    assert(me != NULL);
    return me->top + 1;
}
