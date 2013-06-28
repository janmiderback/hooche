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
#include "eng.h"
#include "cmd.h"
#include "io.h"

int main(void)
{
#ifndef UNITTEST
    t_bool init_ok;
    int    retval;

    /*
     * Initialize the modules.
     */
    init_ok = modinit();

    if (init_ok)
    {
        /*
         * Start the engine
         */
        retval = eng_start();
    }
    else
    {
        fprintf(stderr, "FATAL ERROR: Failed to initialize engine\n");
        retval = 1;
    }

    return retval;
#else
    char* result = all_tests();

    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }
    
    printf("Tests run: %d\n", tests_run);
    retval = (result != 0);
#endif

    return retval;
}
