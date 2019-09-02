//
// MultiAppender - shows how to use multiple appenders with the same logger. 
//

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

int main()
{
    static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("MultiAppender.csv", 8000, 3); // Create the 1st appender.
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender; // Create the 2nd appender.

    fileAppender.setMaxSeverity(plog::debug);
    consoleAppender.setMaxSeverity(plog::warning);

    plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender); // Initialize the logger with the both appenders.

    // A bunch of log lines that goes to the both appenders: to the file and to the console.
    for (int i = 0; i < 100; ++i)
    {
        LOG_VERBOSE << "This is a VERBOSE message";
        LOG_DEBUG << "This is a DEBUG message";
        LOG_INFO << "This is an INFO message";
        LOG_WARNING << "This is a WARNING message";
        LOG_ERROR << "This is an ERROR message";
    }

    return 0;
}
