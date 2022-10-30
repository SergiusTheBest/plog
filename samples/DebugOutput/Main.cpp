//
// DebugOutput - shows how to use DebugOutputAppender to write to the windows debug output.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/DebugOutputAppender.h>

int main()
{
    static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
    plog::init(plog::verbose, &debugOutputAppender);

    PLOGD << "Hello log!"; // short macro
    PLOG_DEBUG << "Hello log!"; // long macro
    PLOG(plog::debug) << "Hello log!"; // function-style macro

    return 0;
}
