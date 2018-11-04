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
