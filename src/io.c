#include "hooche.h"
#include "eng.h"

#define MAX_LINE_LEN 255

#ifdef WIN32
static BOOL   conmode;
static HANDLE hstdin;
#endif /*WIN32*/


static io_out(char* fmt, ...);


t_bool modinit_io()
{
    static t_bool initialized = FALSE;

#ifdef WIN32
    if (!initialized) {
        DWORD mode;
        initialized = TRUE;
		hstdin = GetStdHandle(STD_INPUT_HANDLE);
		conmode = (GetConsoleMode(hstdin, &mode) != 0);
		if (conmode) {
			SetConsoleMode(hstdin, mode & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
			FlushConsoleInputBuffer(hstdin);
		}
    }
#endif /*WIN32*/

    return FALSE;
}


t_bool io_input_available(void)
{
#ifdef WIN32
    DWORD nevents;

    if (conmode) {
		GetNumberOfConsoleInputEvents(hstdin, &nevents);
		return nevents >= 1;
	}
	else {
        DWORD nbytes;
        if (!PeekNamedPipe(hstdin, NULL, 0, NULL, &nbytes, NULL))
        {
			return TRUE;
        }
		return nbytes != 0;
	}
#endif /*WIN32*/

    return FALSE;
}


char* io_getln(void)
{
    static char buf[MAX_LINE_LEN + 1];
    static int  n = 0;
    int         c;

    while ((c = fgetc(stdin)) != EOF) {
        if ((c == '\0') || (c == '\n')) {
            buf[n] = '\0';
            n = 0;
            return buf;
        }
        else if (n < MAX_LINE_LEN) {
            buf[n++] = c;
        }
    }

    return NULL;
}


void io_tell_gui(char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}


void io_print(char* fmt, ...)
{
    if (eng.iomode == IOMODE_NORMAL) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}


void io_println(char* fmt, ...)
{
    if (eng.iomode == IOMODE_NORMAL) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}


void io_prompt()
{
    io_print(">");
}
