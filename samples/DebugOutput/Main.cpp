//
// DebugOutput - shows how to use DebugOutputAppender to write to the windows debug output.
//

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>

int main()
{
    static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
    plog::init(plog::verbose, &debugOutputAppender);

    LOGD << "Hello log!"; // short macro
    LOG_DEBUG << "Hello log!"; // long macro
    LOG(plog::debug) << "Hello log!"; // function-style macro

    return 0;
}
