#ifndef GENMOVES_H_
#define GENMOVES_H_

#include "types.h"

//unsigned int gen_root_moves(t_game* p_game, t_move* p_moves);

unsigned int gen_rootmoves(t_game* p_game);
unsigned int gen_pseudomoves(t_game* p_game);


#endif /*GENMOVES_H_*/
