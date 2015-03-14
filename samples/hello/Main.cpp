#include <plog/Log.h>

int main()
{
    plog::init("hello-log.csv", plog::debug);

    LOGD << "Hello log!";
    LOG_DEBUG << "Hello log!";
    LOG(plog::debug) << "Hello log!";

    return 0;
}
