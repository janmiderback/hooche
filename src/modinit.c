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


#include "hooce.h"
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
