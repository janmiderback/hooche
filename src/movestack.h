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
