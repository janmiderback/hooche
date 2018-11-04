#ifndef FEN_H_
#define FEN_H_

#include "hooche.h"
#include "game.h"


/* Maximum allowed FEN string legth.
 * This is more than necessary, but allowes for some extra
 * whitespace if present
 */
#define MAX_FEN_LEN 127

t_bool  fen_parse(const char* fen, t_game* p_game);
char    fen_piece_char(t_color color, t_piece piece);
t_piece fen_piece(char c);
t_color fen_color(char c);

#endif /*FEN_H_*/
