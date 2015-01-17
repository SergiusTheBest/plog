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

    // Integer
    LOG_DEBUG << "One million: " << 1000000;

    // Null
    LOG_DEBUG << static_cast<char*>(NULL);
    LOG_DEBUG << static_cast<wchar_t*>(NULL);
    LOG_DEBUG << static_cast<const char*>(NULL);
    LOG_DEBUG << static_cast<const wchar_t*>(NULL);

    // Unicode
    LOG_DEBUG << "Wide - вайд";
    LOG_DEBUG << L"Wide - вайд";
    LOG_DEBUG << std::string("test тест");
    LOG_DEBUG << std::wstring(L"test тест");
    LOG_DEBUG << L'ы';

    // Multiline
    LOG_INFO << "This\nis\na\nmultiline!";

    // Quotes
    LOG_INFO << "This is a message with \"quotes\"!";

    // A lot of lines (to test log rolling)
    for (int i = 0; i < 100; ++i)
    {
        LOG_INFO << "i: " << i;
    }

    return 0;
}