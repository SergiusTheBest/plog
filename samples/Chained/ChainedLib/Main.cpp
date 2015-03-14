#include <plog/Log.h>

extern "C" void init(plog::Severity severity, plog::IAppender* appender)
{
    plog::init(severity).addAppender(appender);
}

extern "C" void foo()
{
    LOGI << "Hello from shared lib!";
}
