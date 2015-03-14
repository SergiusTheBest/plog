#include <plog/Log.h>

int main()
{
    plog::init("Hello-log.txt", plog::debug);

    LOGD << "Hello log!";
    LOG_DEBUG << "Hello log!";
    LOG(plog::debug) << "Hello log!";

    return 0;
}
