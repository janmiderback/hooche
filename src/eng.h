#ifndef ENG_H_
#define ENG_H_

#include "game.h"

/**
 * Main engine type. Root object of chess engine.
 */
typedef struct {
    t_eng_iomode     iomode;
    t_eng_searchmode searchmode;
    t_eng_state      state;
    t_bool           debug;
    
    /* 
     * The following fields are 'signals' that the engine should react on and
     * then reset (set to false).
     */
     
    t_bool sig_exit;     /* Exit the engine */
    t_bool sig_newgame;  /* Discard current game and prepare for new */
    t_bool sig_stop;     /* Stop calculating */
    
    t_game game;
} t_eng;

extern t_eng eng;

void   eng_init(void);
t_bool eng_start(void);
void   eng_handle_cmd(const char* cmdstr);

void eng_newgame(void);
void eng_set_position(const char* fen);
void eng_search(void);


#endif /*ENG_H_*/
