#include <plog/Log.h>

enum
{
    SecondLog = 1
};

int main()
{
    plog::init("MultiInstance-default.txt", plog::debug);
    plog::init<SecondLog>("MultiInstance-second.txt", plog::debug);

    LOGD << "Hello default log!";
    LOG_DEBUG << "Hello default log!";
    LOG(plog::debug) << "Hello default log!";

    LOGD_(SecondLog) << "Hello second log!";
    LOG_DEBUG_(SecondLog) << "Hello second log!";
    LOG_(SecondLog, plog::debug) << "Hello second log!";

    return 0;
}
