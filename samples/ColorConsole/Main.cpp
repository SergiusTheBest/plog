//
// ColorConsole - shows how to use a color console appender.
//

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    // Log severity levels are printed in different colors.
    LOG_VERBOSE << "This is a VERBOSE message";
    LOG_DEBUG << "This is a DEBUG message";
    LOG_INFO << "This is an INFO message";
    LOG_WARNING << "This is a WARNING message";
    LOG_ERROR << "This is an ERROR message";
    LOG_FATAL << "This is a FATAL message";

    return 0;
}
