#include "hooche.h"
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
