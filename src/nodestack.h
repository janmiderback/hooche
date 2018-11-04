#ifndef NODESTACK_H_
#define NODESTACK_H_

#include "hooche.h"
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
