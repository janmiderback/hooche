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

#include "hooce.h"
#include "search.h"
#include "nodestack.h"


void nodestack_init(t_nodestack* me)
{
    memset(me, 0, sizeof(*me));
}


t_node* nodestack_top(t_nodestack* me)
{
    return &(me->nodes[me->top]);
}


void nodestack_push(t_nodestack* me)
{
    assert(me->top < (NODESTACK_SIZE - 1));
    (me->top)++;
}


void nodestack_pop(t_nodestack* me)
{
    assert(me->top > 0);
    (me->top)--;
}


void nodestack_popn(t_nodestack* me, unsigned int n)
{
    assert(me->top >= n);
    me->top -= n;
}
