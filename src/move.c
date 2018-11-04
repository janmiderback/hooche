#include "hooche.h"
#include "move.h"  /* Myself */


void move_set(t_move* me, t_square from, t_square to, t_piece promote_piece)
{
    assert(me != NULL);
    me->from = from;
    me->to = to;
    me->promote_piece = promote_piece;
}


void move_fromstr(t_move* me, const char* s)
{
    /*TODO*/
    assert(FALSE);
}


char* move_tostr(const t_move* me, char* s)
{
    const t_square from_square = me->from;
    const t_square to_square = me->to;
    const t_piece  promote_piece = me->promote_piece;

    strcpy(s, square_tostr(from_square));
    strcat(s, square_tostr(to_square));

    if (promote_piece != NO_PIECE) {
        strcat(s, piece_tostr(promote_piece));
    }

    return s;
}
