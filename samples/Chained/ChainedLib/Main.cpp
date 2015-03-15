#include <plog/Log.h>

#if defined _MSC_VER || defined __CYGWIN__
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __attribute__ ((visibility ("default")))
#endif

extern "C" void EXPORT initialize(plog::Severity severity, plog::IAppender* appender)
{
    plog::init(severity, appender);
}

extern "C" void EXPORT foo()
{
    LOGI << "Hello from shared lib!";
}
