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
