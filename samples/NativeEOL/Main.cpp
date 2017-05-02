//
// NativeEOL - shows how to use NativeEOLConverter.
//

#include <plog/Log.h>
#include <plog/Converters/NativeEOLConverter.h>

int main()
{
    // NativeEOLConverter will use <CRLF> on Windows and <LF> on everything else as line endings. As a template parameter it accepts another converter (by default UTF8Converter).
    static plog::RollingFileAppender<plog::TxtFormatter, plog::NativeEOLConverter<> > fileAppender("NativeEOL.log", 8000, 3); // Create an appender with NativeEOLConverter.
    plog::init(plog::debug, &fileAppender); // Initialize the logger.

    // Write some data.
    for (int i = 0; i < 100; ++i)
    {
        LOGI << "i: " << i;
    }

    return 0;
}
