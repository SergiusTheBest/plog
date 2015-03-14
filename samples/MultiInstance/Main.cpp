#include <plog/Log.h>

enum
{
    SecondLog = 1
};

int main()
{
    plog::init(plog::debug, "MultiInstance-default.txt");
    plog::init<SecondLog>(plog::debug, "MultiInstance-second.txt");

    LOGD << "Hello default log!";
    LOG_DEBUG << "Hello default log!";
    LOG(plog::debug) << "Hello default log!";

    LOGD_(SecondLog) << "Hello second log!";
    LOG_DEBUG_(SecondLog) << "Hello second log!";
    LOG_(SecondLog, plog::debug) << "Hello second log!";

    return 0;
}
