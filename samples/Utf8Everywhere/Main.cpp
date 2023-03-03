//
// Utf8Everywhere - demonstrates using http://utf8everywhere.org on Windows.
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

void 中文()
{
    PLOGD << "Chinese: 中文";
    PLOGD << L"Chinese: 中文";
}

void тест()
{
    PLOGD << "Cyrillic: тест";
    PLOGD << L"Cyrillic: тест";
}

int main()
{
    plog::init(plog::debug, "Utf8Everywhere.log"); // Initialize logging to the file.

    plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::get()->addAppender(&consoleAppender); // Also add logging to the console.

    中文();
    тест();

    return 0;
}
