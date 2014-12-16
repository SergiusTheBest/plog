#include <plog/Log.h>
#include <plog/ConsoleAppender.h>

int main()
{
    static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("multi-log.csv", plog::debug, 8000, 3);
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender(plog::debug);
    static plog::Logger logger;
    logger.addAppender(&fileAppender).addAppender(&consoleAppender);

    // Log levels
    LOG_FATAL << "fatal";
    LOG_ERROR << "error";
    LOG_INFO << "info";
    LOG_WARNING << "warning";
    LOG_DEBUG << "debug";

    // Multiline
    LOG_INFO << "This\nis\na\nmultiline!";

    // Quotes
    LOG_INFO << "This is a message with \"quotes\"!";

    for (int i = 0; i < 100; ++i)
    {
        LOG_INFO << "i: " << i;
    }

    return 0;
}