#include <stdio.h>

#include "minunit.h"

 
static int foo = 7;
static int bar = 4;


static char* test_foo()
{
    mu_assert("error, foo != 7", foo == 7);
    return 0;
}

/*
static char* test_bar()
{
    mu_assert("error, bar != 5", bar == 5);
    return 0;
}
*/

char* board_test()
{
    mu_run_test(test_foo);
 //   mu_run_test(test_bar);
    return 0;
 }
