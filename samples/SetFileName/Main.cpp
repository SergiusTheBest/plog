//
// SetFileName - shows how to change a log file name at arbitrary moment.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>

int main()
{
    static plog::RollingFileAppender<plog::TxtFormatter> fileAppender("SetFileNameAAA.log");
    plog::init(plog::debug, &fileAppender);

    for (int i = 0; i < 100; ++i)
    {
        PLOG_INFO << "i: " << i;
    }

    fileAppender.setFileName("SetFileNameBBB.log");

    for (int i = 0; i < 100; ++i)
    {
        PLOG_INFO << "i: " << i;
    }

    return 0;
}
