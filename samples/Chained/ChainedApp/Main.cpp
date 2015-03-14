#include <plog/Log.h>

extern "C" void initialize(plog::Severity severity, plog::IAppender* appender);
extern "C" void foo();

int main()
{
    plog::init(plog::debug, "ChainedApp.txt");

    LOGD << "Hello from app!";

    initialize(plog::debug, plog::get());
    foo();

    return 0;
}
