#include <plog/Log.h>

int main()
{
    plog::init(plog::debug, "Hello.txt");

    LOGD << "Hello log!";
    LOG_DEBUG << "Hello log!";
    LOG(plog::debug) << "Hello log!";

    return 0;
}
