#include "hooche.h"
#include "modinit.h"
#include "eng.h"
#include "cmd.h"
#include "io.h"
#ifdef UNITTEST
#include "testrunner.h"
#endif

int main(void)
{
	int retval;
	
    bool init_ok;

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
}
