#ifndef SEARCH_H_
#define SEARCH_H_

#include "hooche.h"
#include "move.h"

typedef struct
{
    t_move          move;
	t_piece         capture;
	t_square        ep_square;
	t_castle_rights castle_rights;
	int             fifty_count;
    /*
    HASHKEY hashkey;
	HASHKEY path_hashkey;
	HASHKEY pawn_hashkey;
    */
} t_node;



/**
    Iterative deepening search.
 */
void search_ids(void);


/**
    Principal variantion search.
 */
void search_pvs(void);



/*
function alphabeta(node, depth, a, ß)
    (* ß represents previous player best choice - doesn't want it if a would worsen it *)
    if node is a terminal node or depth = 0
        return the heuristic value of node
    foreach child of node
        a := max(a, -alphabeta(child, depth-1, -ß, -a))
        (* use symmetry, -ß becomes subsequently pruned a *)
        if ß=a
            break                             (* Beta cut-off *)
    return a

(* Initial call *)
alphabeta(origin, depth, -infinity, +infinity)
*/


//int alphabeta(void* node, int depth, alpha, beta)
//{
//
//
//    /* beta represents previous player best choice - doesn't want it if a would worsen it */
//    if (depth == 0 /* || terminal node???*/)
//    {
//        return 123 ;/*eval(node);*/
//    }
//
//    nmoves = t_movegene
//
//}


#if 0
int pvSearch( int alpha, int beta, int depth ) {
   if( depth == 0 ) return quiesce(alpha, beta);
   bool bSearchPv = true;
   for ( all moves)  {
      make
      if ( bSearchPv ) {
         score = -pvSearch(-beta, -alpha, depth - 1);
      } else {
         score = -zwSearch(-alpha, depth - 1);
         if ( score > alpha ) // in fail-soft ... && score < beta ) is common
            score = -pvSearch(-beta, -alpha, depth - 1); // re-search
      }
      unmake
      if( score >= beta )
         return beta;   // fail-hard beta cutoff
      if( score > alpha ) {
         alpha = score; // alpha acts like max in MiniMax
         bSearchPv = false;   // *1)
      }
   }
   return alpha;
}
#endif


// fail-hard zero window search, returns either beta-1 or beta
#if 0
int zwSearch(int beta, int depth ) {
   // alpha == beta - 1
   // this is either a cut- or all-node
   if( depth == 0 ) return quiesce(beta-1, beta);
   for ( all moves)  {
     make
     score = -zwSearch(1-beta, depth - 1);
     unmake
     if( score >= beta )
        return beta;   // fail-hard beta cutoff
   }
   return beta-1; // fail-hard, return alpha
}
#endif



#endif /*SEARCH_H_*/
