//
// Hello - a minimal introduction sample, shows the basic 3 steps to start using plog.
//

#include <plog/Log.h> // Step1: include the header.
#include <plog/Initializers/RollingFileInitializer.h>

int main()
{
    plog::init(plog::debug, "Hello.txt"); // Step2: initialize the logger.

    // Step3: write log messages using a special macro. There are several log macros, use the macro you liked the most.

    PLOGD << "Hello log!"; // short macro
    PLOG_DEBUG << "Hello log!"; // long macro
    PLOG(plog::debug) << "Hello log!"; // function-style macro

    return 0;
}
