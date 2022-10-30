//
// SkipNativeEOL - shows how to skip NativeEOLConverter.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Converters/UTF8Converter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>

int main()
{
    // NativeEOLConverter will use <CRLF> on Windows and <LF> on everything else as line endings.
    // It's used by default.
    // If you want to always use <LF> you can skip NativeEOLConverter and specify UTF8Converter directly.
    static plog::RollingFileAppender<plog::TxtFormatter, plog::UTF8Converter> fileAppender("SkipNativeEOL.log", 8000, 3); // Create an appender without NativeEOLConverter.
    plog::init(plog::debug, &fileAppender); // Initialize the logger.

    // Write some data.
    for (int i = 0; i < 100; ++i)
    {
        PLOGI << "i: " << i;
    }

    return 0;
}
