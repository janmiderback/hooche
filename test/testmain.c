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


#include "minunit.h"

/**
 * Helper macro to run a suite of minunit tests.
 */
#define run_test_suite(suite)         \
    do                                \
    {                                 \
        char* message = (suite)();    \
        if (message)                  \
        {                             \
            printf("%s\n", message);  \
            printf("TESTS FAILED\n"); \
            printf("Tests run: %d\n", mu_tests_run); \
            return 0;                 \
        }                             \
    } while (0)

/**
 * Need to define the minunit test counter.
 */
int mu_tests_run = 0;


/*
 * External declaration of the test suites to run.
 */
char* board_test();


/**
 * Test main entry.
 */
int main(void)
{
    run_test_suite(board_test);
    printf("ALL TESTS PASSED\n");    
    printf("Tests run: %d\n", mu_tests_run);
    return 1;
}
