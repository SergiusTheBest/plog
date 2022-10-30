//
// AndroidNative - shows how to use an Android appender.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/AndroidAppender.h>

int main()
{
    static plog::AndroidAppender<plog::TxtFormatter> androidAppender("app");
    plog::init(plog::verbose, &androidAppender);

    PLOG_VERBOSE << "This is a VERBOSE message";
    PLOG_DEBUG << "This is a DEBUG message";
    PLOG_INFO << "This is an INFO message";
    PLOG_WARNING << "This is a WARNING message";
    PLOG_ERROR << "This is an ERROR message";
    PLOG_FATAL << "This is a FATAL message";

    return 0;
}
