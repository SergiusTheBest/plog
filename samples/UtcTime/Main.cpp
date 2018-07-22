//
// UtcTime - shows how to use UTC time in logs.
//

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatterUtcTime> consoleAppender; // TxtFormatter in UTC
    static plog::RollingFileAppender<plog::CsvFormatterUtcTime> fileAppender("UtcTime.csv", 10000, 2); // CsvFormatter in UTC
    plog::init(plog::verbose, &consoleAppender).addAppender(&fileAppender);

    LOG_VERBOSE << "This is a VERBOSE message";
    LOG_DEBUG << "This is a DEBUG message";
    LOG_INFO << "This is an INFO message";
    LOG_WARNING << "This is a WARNING message";
    LOG_ERROR << "This is an ERROR message";
    LOG_FATAL << "This is a FATAL message";

    return 0;
}
