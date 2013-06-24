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

#ifndef NODESTACK_H_
#define NODESTACK_H_

#include "hooce.h"
#include "search.h"

#define NODESTACK_SIZE  (128 * 128)

typedef struct {
    t_node       nodes[NODESTACK_SIZE];
    unsigned int top;
} t_nodestack;

void nodestack_init(t_nodestack* me);

t_node* nodestack_top(t_nodestack* me);

void nodestack_push(t_nodestack* me);

void nodestack_pop(t_nodestack* me);

void nodestack_popn(t_nodestack* me, unsigned int n);


#endif /*NODESTACK_H_*/
