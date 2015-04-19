//
// MultiInstance - shows how to use multiple logger instances, each instance has its own independent configuration.
//

#include <plog/Log.h>

enum // Define log instances. Default is 0 and is omitted from this enum.
{
    SecondLog = 1
};

int main()
{
    plog::init(plog::debug, "MultiInstance-default.txt"); // Initialize the default logger instance.
    plog::init<SecondLog>(plog::debug, "MultiInstance-second.txt"); // Initialize the 2nd logger instance.

    // Write some messages to the default log.
    LOGD << "Hello default log!";
    LOG_DEBUG << "Hello default log!";
    LOG(plog::debug) << "Hello default log!";

    // Write some messages to the 2nd log.
    LOGD_(SecondLog) << "Hello second log!";
    LOG_DEBUG_(SecondLog) << "Hello second log!";
    LOG_(SecondLog, plog::debug) << "Hello second log!";

    return 0;
}
