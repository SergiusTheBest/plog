#include <plog/Log.h>

extern "C" void initialize(plog::Severity severity, plog::IAppender* appender)
{
    plog::init(severity, appender);
}

extern "C" void foo()
{
    LOGI << "Hello from shared lib!";
}
