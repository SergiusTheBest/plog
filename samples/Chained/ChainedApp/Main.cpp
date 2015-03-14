#include <plog/Log.h>

extern "C" void init(plog::Severity severity, plog::IAppender* appender);
extern "C" void foo();

int main()
{
    plog::init("ChainedApp.txt", plog::debug);

    LOGD << "Hello from app!";

    init(plog::debug, plog::get());
    foo();

    return 0;
}
