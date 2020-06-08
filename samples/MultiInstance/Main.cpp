//
// MultiInstance - shows how to use multiple logger instances, each instance has its own independent configuration.
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

enum // Define log instances. Default is 0 and is omitted from this enum.
{
    SecondLog = 1
};

int main()
{
    plog::init(plog::debug, "MultiInstance-default.txt"); // Initialize the default logger instance.
    plog::init<SecondLog>(plog::debug, "MultiInstance-second.txt"); // Initialize the 2nd logger instance.

    // Write some messages to the default log.
    PLOGD << "Hello default log!";
    PLOG_DEBUG << "Hello default log!";
    PLOG(plog::debug) << "Hello default log!";

    // Write some messages to the 2nd log.
    PLOGD_(SecondLog) << "Hello second log!";
    PLOG_DEBUG_(SecondLog) << "Hello second log!";
    PLOG_(SecondLog, plog::debug) << "Hello second log!";

    return 0;
}
