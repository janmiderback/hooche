/**
 * Copyright 2009 Jan Miderbäck
 *
 * This file is part of Hooce.
 *
 * Hooce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hooce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hooce.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hooche.h"
#include "io.h"
#include "game.h"
#include "board.h"
#include "cmd.h"
#include "genmoves.h"

#include "eng.h"

t_eng eng;

static void eng_handle_uci(const t_uicmd_uci* p_cmd);
static void eng_handle_debug(const t_uicmd_debug* p_cmd);
static void eng_handle_isready(const t_uicmd_isready* p_cmd);
static void eng_handle_setoption(const t_uicmd_setoption* p_cmd);
static void eng_handle_register(const t_uicmd_register* p_cmd);
static void eng_handle_ucinewgame(const t_uicmd_ucinewgame* p_cmd);
static void eng_handle_position(const t_uicmd_position* p_cmd);
static void eng_handle_go(const t_uicmd_go* p_cmd);
static void eng_handle_stop(const t_uicmd_stop* p_cmd);
static void eng_handle_ponderhit(const t_uicmd_ponderhit* p_cmd);
static void eng_handle_quit(const t_uicmd_quit* p_cmd);


/**
 * External function definitions
 */

void modinit_eng(void)
{
    eng.iomode = IOMODE_NORMAL;
    eng.searchmode = SEARCHMODE_NORMAL;
    eng.debug = FALSE;
    eng.sig_exit = FALSE;
    eng.sig_newgame = FALSE;
    eng.sig_stop = FALSE;
    game_init(&(eng.game));
}


t_bool eng_start()
{
    /* Print engine start-up banner */
    io_println("Hooce v" HOOCHE_VER " - UCI chess engine");
    io_println("Written by Jan Miderbaeck (jan.miderback@gmail.com)");
    io_println("Entering normal mode.");
    io_println("Type \"uci\" to enter UCI mode.");
    io_println("Type \"quit\" to exit.");

    while (!eng.sig_exit) {
        char* cmdstr;

        if (eng.iomode == IOMODE_NORMAL) io_prompt();
        cmdstr = io_getln();
        eng_handle_cmd(cmdstr);
    }

    /* Need not reset sig_exit since we shall quit. */

    return TRUE;
}


void eng_handle_cmd(const char* cmdstr)
{
    t_uicmd*  p_uicmd;
    const int cmd_id = cmd_parse(cmdstr, &p_uicmd);

    switch (cmd_id) {
        case UICMD_UCI:
            eng_handle_uci((t_uicmd_uci*)p_uicmd);
            break;

        case UICMD_DEBUG:
            eng_handle_debug((t_uicmd_debug*)p_uicmd);
            break;

        case UICMD_ISREADY:
            eng_handle_isready((t_uicmd_isready*)p_uicmd);
            break;

        case UICMD_SETOPTION:
            eng_handle_setoption((t_uicmd_setoption*)p_uicmd);
            break;

        case UICMD_REGISTER:
            eng_handle_register((t_uicmd_register*)p_uicmd);
            break;

        case UICMD_UCINEWGAME:
            eng_handle_ucinewgame((t_uicmd_ucinewgame*)p_uicmd);
            break;

        case UICMD_POSITION:
            eng_handle_position((t_uicmd_position*)p_uicmd);
            break;

        case UICMD_GO:
            eng_handle_go((t_uicmd_go*)p_uicmd);
            break;

        case UICMD_STOP:
            eng_handle_stop((t_uicmd_stop*)p_uicmd);
            break;

        case UICMD_PONDERHIT:
            eng_handle_ponderhit((t_uicmd_ponderhit*)p_uicmd);
            break;

        case UICMD_QUIT:
            eng_handle_quit((t_uicmd_quit*)p_uicmd);
            break;

        default:
            io_println("Invalid command");
            /* Do nothing */
            break;
    }

    if (p_uicmd != NULL) free(p_uicmd);
}


void eng_newgame(void)
{
    // Is this needed?
    // Well, should not be to expensive so we can re-set
    // when getting position.
    game_init(&(eng.game));
}


void eng_search(void)
{
    t_movestack *p_ms;
    t_move      *p_move;
    char         movestr[10];
    //What to do here?

    // Search the position set.

    // 1. Generate the root moves.
    //move
    gen_rootmoves(&(eng.game));

    //TEST: Print the generated moves

    p_ms = &(eng.game.movestack);

    io_println("TEST VERSION - Generated moves:");
    while ((p_move = movestack_top(p_ms)) != NULL) {
        move_tostr(p_move, movestr);
        io_println("    %s", movestr);
        movestack_pop(p_ms);
    }

    // 2. Do alpha-beta iterative deepening search of these moves
    // Some things to consider:
    // - How to determine when to stop search?
    // - Shall we do quiecence search at each iteration?
    //     Seems logical to only do the quiecence search after the full search.
    //     ???
//    for (
//    search

    // 3. Do something with best move.
}


/**
 * Local function definitions
 */

static void eng_handle_uci(const t_uicmd_uci* p_cmd)
{
    if (p_cmd->on) {
        eng.iomode = IOMODE_UCI;
        cmd_send_raw("id name Hooce 0.1");
        cmd_send_raw("id author Jan Miderbaeck");
        cmd_send_raw("uciok");
    }
    else {
        eng.iomode = IOMODE_NORMAL;
        cmd_send_raw("UCI mode turned off");
    }
}


static void eng_handle_debug(const t_uicmd_debug* p_cmd)
{
    eng.debug = p_cmd->on;

    if (eng.iomode == IOMODE_NORMAL) {
        if (eng.debug) io_println("Debug mode turned on.");
        else           io_println("Debug mode turned off.");
    }
}


static void eng_handle_isready(const t_uicmd_isready* p_cmd)
{
    cmd_send_raw("readyok");
}


static void eng_handle_setoption(const t_uicmd_setoption* p_cmd)
{
    io_println("No options are supported yet.");
}


static void eng_handle_register(const t_uicmd_register* p_cmd)
{
    io_println("No registration is neccessary. Enjoy!");
}


static void eng_handle_ucinewgame(const t_uicmd_ucinewgame* p_cmd)
{
    if (eng.state == STATE_IDLE) {
        eng_newgame();
        cmd_send_raw("isready");
    }
    else {
        eng.sig_newgame = TRUE;
    }
}


static void eng_handle_position(const t_uicmd_position* p_cmd)
{
    if (eng.state == STATE_IDLE) {
        char boardstr[BOARD_STR_SIZE + 1];

        game_init(&(eng.game));
        if (!p_cmd->startpos) {
            game_init_fen(&(eng.game), p_cmd->fenstring);
        }
        if (p_cmd->nmoves > 0) {
            const t_bool legal = game_play_moves(&(eng.game), p_cmd->moves, p_cmd->nmoves);
            if (!legal) {
                io_println("Illegal move found. Moves after and including illegal move ignored");
            }
        }
        board_tostr(&(eng.game.board), boardstr);
        io_print(boardstr);
    }
}


static void eng_handle_go(const t_uicmd_go* p_cmd)
{
    if (eng.state == STATE_IDLE) {
        eng_search();
    }
    else {
        io_println("Command ignored. Already searching.");
    }

}


static void eng_handle_stop(const t_uicmd_stop* p_cmd)
{
    if (eng.state == STATE_SEARCHING) {
        eng.sig_stop = TRUE;
    }
    else {
        io_println("Command ignored. Not searching.");
    }
}


static void eng_handle_ponderhit(const t_uicmd_ponderhit* p_cmd)
{
    if ((eng.state == STATE_SEARCHING) &&
        (eng.searchmode == SEARCHMODE_PONDERING)) {
    }
    else {
        io_println("Command ignored. Not pondering.");
    }

}


static void eng_handle_quit(const t_uicmd_quit* p_cmd)
{
    eng.sig_exit = TRUE;
}
