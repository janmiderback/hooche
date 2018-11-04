#include "hooche.h"
#include "util.h"
#include "io.h"
#include "move.h"

#include "cmd.h"


static const char* WS = " \t";


/*******************************************************************************
 * Local function declarations.
 */

static t_uicmd_uci*      cmd_parse_uci(void);
static t_uicmd_debug*    cmd_parse_debug(void);
static t_uicmd_position* cmd_parse_position(void);
static t_uicmd*          cmd_alloc(int cmd_id, size_t size);


/*******************************************************************************
 * External function definitions.
 */

int cmd_parse(const char* cmdstr, t_uicmd** pp_uicmd)
{
    char* cmd_field;
    char* cpystr = malloc(strlen(cmdstr) + 1);
    t_bool done = FALSE;
    
    *pp_uicmd = NULL;
    strcpy(cpystr, cmdstr);
    cmd_field = strtok(cpystr, WS);
    done = (cmd_field == NULL);

    while (!done) {
        done = TRUE;  /* Assume we're done */

        /*
         * uci
         */
        if (str_icmp(cmd_field, "uci") == 0) {
            *pp_uicmd = (t_uicmd*)cmd_parse_uci();
        }
        /*
         * debug
         */
        else if (str_icmp(cmd_field, "debug") == 0) {
            *pp_uicmd = (t_uicmd*)cmd_parse_debug();
        }
        /*
         * isready
         */
        else if (strcmp(cmd_field, "isready") == 0) {
            *pp_uicmd = cmd_alloc(UICMD_ISREADY, sizeof(t_uicmd_isready));
        }
        /*
         * setoption
         */
        else if (strcmp(cmd_field, "setoption") == 0) {
            *pp_uicmd = cmd_alloc(UICMD_REGISTER, sizeof(t_uicmd_setoption));
        }
        /*
         * isready
         */
        else if (strcmp(cmd_field, "register") == 0) {
            *pp_uicmd = cmd_alloc(UICMD_REGISTER, sizeof(t_uicmd_register));
        }
        /*
         * isready
         */
        else if (strcmp(cmd_field, "ucinewgame") == 0) {
            *pp_uicmd = cmd_alloc(UICMD_UCINEWGAME, sizeof(t_uicmd_ucinewgame));
        }
        /*
         * position
         */
        else if (strcmp(cmd_field, "position") == 0) {
            *pp_uicmd = (t_uicmd*)cmd_parse_position();
        }
        /*
         * isready
         */
        else if (strcmp(cmd_field, "go") == 0) {
            *pp_uicmd = cmd_alloc(UICMD_GO, sizeof(t_uicmd_go));
        }
        /*
         * isready
         */
        else if (strcmp(cmd_field, "stop") == 0) {
            *pp_uicmd = cmd_alloc(UICMD_STOP, sizeof(t_uicmd_stop));
        }
        /*
         * isready
         */
        else if (strcmp(cmd_field, "ponderhit") == 0) {
        }
        /*
         * isready
         */
        else if (strcmp(cmd_field, "quit") == 0) {
            *pp_uicmd = cmd_alloc(UICMD_QUIT, sizeof(t_uicmd_quit));
        }
        /*
         * Unknown
         */
        else {
            /* Did not recognize first token. Try next if any. */
            cmd_field = strtok(NULL, " \t");
            done = (cmd_field == NULL);
        }
    }
    
    free(cpystr);

    if (*pp_uicmd == NULL) return UICMD_NONE;
    else                   return (*pp_uicmd)->cmd_id;
}


void cmd_send_raw(const char* cmdstr)
{
    io_tell_gui(cmdstr);
}


/*******************************************************************************
 * Local function definitions.
 */

static t_uicmd_uci* cmd_parse_uci(void)
{   
    char* field;
    t_uicmd_uci* p_cmd = NULL;

    p_cmd = (t_uicmd_uci*)cmd_alloc(UICMD_UCI, sizeof(t_uicmd_uci));
    p_cmd->on = TRUE;

    if ((field = strtok(NULL, WS)) != NULL) {
        p_cmd->on = (str_icmp(field, "off") != 0);
    }

    return p_cmd;
}


static t_uicmd_debug* cmd_parse_debug(void)
{
    char* field;
    t_uicmd_debug* p_cmd = NULL;
    t_bool parseok = (field = strtok(NULL, WS)) != NULL;
    
    if (parseok) {
        const t_bool on = (str_icmp(field, "on") == 0);
        const t_bool off = (str_icmp(field, "off") == 0);
        parseok = (on || off);
        
        if (parseok) {
            p_cmd = (t_uicmd_debug*)cmd_alloc(UICMD_DEBUG, sizeof(t_uicmd_debug));
            p_cmd->on = on;
        }
    }
    
    return p_cmd;
}


static t_uicmd_position* cmd_parse_position(void)
{
    char* field;
    t_uicmd_position* p_cmd = NULL;
    t_bool parseok = ((field = strtok(NULL, WS)) != NULL);

    if (parseok) {
        t_bool startpos = (str_icmp(field, "startpos") == 0);
        t_bool fen = (str_icmp(field, "fen") == 0);
        t_bool movefound = FALSE;
        char   fenstr[MAX_FEN_LEN + 1];
        int    nmoves = 0;
        t_move moves[UICMD_POSITION_MAX_MOVES];

        parseok = (fen || startpos);

        if (parseok && fen) {
            /* Must have a FEN string */
            /* Read up to 'moves' or end of string */
            t_bool parsingfen = TRUE;
            fenstr[0] = 0;
            while (parsingfen) {
                field = strtok(NULL, WS);
                if ((field == NULL) || (str_icmp(field, "moves") == 0)) {
                    parsingfen = FALSE;
                    movefound = TRUE;  /* Remember */
                }
                else if (strlen(fenstr) + strlen(field) + 1 <= MAX_FEN_LEN) {
                    if (strlen(fenstr) != 0) strcat(fenstr, " ");
                    strcat(fenstr, field);
                }
                else {
                    parseok = FALSE;
                    parsingfen = FALSE;
                }
            }
        }
    
        if (parseok && movefound) {
            field = strtok(NULL, WS);
            if (field != NULL) {
                /* <move1> ... <movei> */
                while (parseok && ((field = strtok(NULL, WS)) != NULL)) {
                    t_move move;
                    move_fromstr(&move, field);
                    parseok = (!IS_NULL_MOVE(move)) && (nmoves < UICMD_POSITION_MAX_MOVES);
                    if (parseok) moves[nmoves++] = move;
                }
            }
        }

        if (parseok) {
            p_cmd = (t_uicmd_position*)cmd_alloc(UICMD_POSITION, sizeof(t_uicmd_position));
            p_cmd->startpos = startpos;
            if (fen) strcpy(p_cmd->fenstring, fenstr);
            p_cmd->nmoves = nmoves;
            if (nmoves > 0) memcpy(p_cmd->moves, moves, sizeof(moves));
        }
    }

    return p_cmd;
}


static t_uicmd* cmd_alloc(int cmd_id, size_t size)
{
    t_uicmd* p_cmd = malloc(size);
    p_cmd->cmd_id = cmd_id;
    return p_cmd;
}
