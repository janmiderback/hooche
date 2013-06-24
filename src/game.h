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

#ifndef GAME_H_
#define GAME_H_

#include "nodestack.h"
#include "movestack.h"


#define TREENODE_STACK_SIZE (1024)       // TODO: Why?

typedef struct {
    t_color         side_tm;
	t_color         side_ntm;
    t_castle_rights castle_rights;
    int             fifty_count;
    int             move_number;
    t_board         board;

    t_nodestack nodestack;
    t_movestack movestack;
} t_game;


/**
 * TODO
 */
void game_init(t_game* me);

/**
 * TODO
 */
t_bool game_init_fen(t_game* me, const char* fen);


/**
 * TODO
 */
t_bool game_play_moves(t_game* me, const t_move moves[], unsigned int nmoves);


/**
 * TODO
 */
void game_set_stm(t_game* me, t_color color);

/**
 * Determine whether a move is legal.
 */
t_bool game_islegal_move(t_game* me, t_move move);

/**
 * Determine if the side to move is in check.
 */
t_bool game_stm_incheck(t_game* me);

/**
 * TODO
 */
t_bool game_isattacked(t_game* me, t_square square, t_color attacker);


#endif /*GAME_H_*/
