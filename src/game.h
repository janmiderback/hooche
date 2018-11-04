#ifndef GAME_H_
#define GAME_H_

#include <hooche.h>
#include <nodestack.h>
#include <movestack.h>


#define TREENODE_STACK_SIZE (1024)       // TODO: Why?

typedef struct
{
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
bool game_init_fen(t_game* me, const char* fen);


/**
 * TODO
 */
bool game_play_moves(t_game* me, const t_move moves[], unsigned int nmoves);


/**
 * TODO
 */
void game_set_stm(t_game* me, t_color color);

/**
 * Determine whether a move is legal.
 */
bool game_islegal_move(t_game* me, t_move move);

/**
 * Determine if the side to move is in check.
 */
bool game_stm_incheck(t_game* me);

/**
 * TODO
 */
bool game_isattacked(t_game* me, t_square square, t_color attacker);


#endif /*GAME_H_*/
