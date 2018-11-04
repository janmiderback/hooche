#include "hooche.h"
#include "modinit.h"

extern void modinit_bitboard(t_bool* ok);
extern void modinit_io(t_bool* ok);
extern void modinit_eng(t_bool* ok);

t_bool modinit(void)
{
    t_bool ok;
    modinit_io(&ok);
    modinit_bitboard(&ok);
    modinit_eng(&ok);
    return ok;
}
