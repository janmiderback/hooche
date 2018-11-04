#ifndef CMD_H_
#define CMD_H_

#include "fen.h"

enum {
    UICMD_NONE,
    UICMD_UCI,
    UICMD_DEBUG,
    UICMD_ISREADY,
    UICMD_SETOPTION,
    UICMD_REGISTER,
    UICMD_UCINEWGAME,
    UICMD_POSITION,
    UICMD_GO,
    UICMD_STOP,
    UICMD_PONDERHIT,
    UICMD_QUIT
};


typedef struct {
    int cmd_id;
} t_uicmd;

typedef struct {
    int cmd_id;
    t_bool on;  /* UCI standard extension. Possibility to turn off UCI mode with "uci off". */
} t_uicmd_uci;

typedef struct {
    int    cmd_id;
    t_bool on;
} t_uicmd_debug;

typedef t_uicmd t_uicmd_isready;

typedef struct {
    int cmd_id;
    /*TODO*/
} t_uicmd_setoption;

typedef struct {
    int    cmd_id;
    t_bool later;
    char*  name;
    char*  code;
} t_uicmd_register;

typedef t_uicmd t_uicmd_ucinewgame;

#define UICMD_POSITION_MAX_MOVES 256
typedef struct {
    int    cmd_id;
    t_bool startpos;
    char   fenstring[MAX_FEN_LEN + 1];
    int    nmoves;
    t_move moves[UICMD_POSITION_MAX_MOVES];
} t_uicmd_position;

#define UICMD_GO_MAX_SEARCHMOVES 256
typedef struct {
    int    cmd_id;
    int    nsearchmoves;
    t_move searchmoves[UICMD_GO_MAX_SEARCHMOVES];
    t_bool ponder;
    int    wtime;
    int    btime;
    int    winc;
    int    binc;
    int    movestogo;
    int    depth;
    int    nodes;
    t_bool mate;
    int    movetime;
    t_bool infinite;
} t_uicmd_go;

typedef t_uicmd t_uicmd_stop;

typedef t_uicmd t_uicmd_ponderhit;

typedef t_uicmd t_uicmd_quit;


/*void cmd_handle(const char* cmdstr, t_game* p_game);*/
int  cmd_parse(const char* cmdstr, t_uicmd** pp_uicmd);
void cmd_send_raw(const char* cmdstr);



#endif /*CMD_H_*/