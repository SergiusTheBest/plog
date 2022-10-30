//
// UtcTime - shows how to use UTC time in logs.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/CsvFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatterUtcTime> consoleAppender; // TxtFormatter in UTC
    static plog::RollingFileAppender<plog::CsvFormatterUtcTime> fileAppender("UtcTime.csv", 10000, 2); // CsvFormatter in UTC
    plog::init(plog::verbose, &consoleAppender).addAppender(&fileAppender);

    PLOG_VERBOSE << "This is a VERBOSE message";
    PLOG_DEBUG << "This is a DEBUG message";
    PLOG_INFO << "This is an INFO message";
    PLOG_WARNING << "This is a WARNING message";
    PLOG_ERROR << "This is an ERROR message";
    PLOG_FATAL << "This is a FATAL message";

    return 0;
}
