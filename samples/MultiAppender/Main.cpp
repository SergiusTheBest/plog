#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

int main()
{
    static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("MultiAppender.csv", 8000, 3);
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);

    // A lot of log lines that goes to the file and the console.
    for (int i = 0; i < 100; ++i)
    {
        LOG_INFO << "i: " << i;
    }

    return 0;
}
