//
// Syslog - shows how to use the syslog appender/formatter.
// For the mappings between plog and syslog severities, refer to SyslogAppender
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/SyslogFormatter.h>
#include <plog/Appenders/SyslogAppender.h>
#if __cplusplus >= 201103L
#include <thread>
#endif

static void log_messages()
{
    PLOG_VERBOSE << "This is a VERBOSE message";
    PLOG_DEBUG << "This is a DEBUG message";
    PLOG_INFO << "This is an INFO message";
    PLOG_WARNING << "This is a WARNING message";
    PLOG_ERROR << "This is an ERROR message";
    PLOG_FATAL << "This is a FATAL message";
}

int main()
{
    static plog::SyslogAppender<plog::SyslogFormatter> syslogAppender;
    plog::init(plog::verbose, &syslogAppender);

    // Calling openlog() is optional. If omitted, the application name is
    // used (depends on the used C library) and the PID is not included.
    openlog("MyApp", LOG_PID, LOG_USER);

    // optional: log only messages up to PLOG_INFO (maps to LOG_NOTICE)
    setlogmask(LOG_UPTO(LOG_NOTICE));

#if __cplusplus >= 201103L
    // optional: start a 2nd thread which also generates log messages
    // you'll get the same PID but different TID values in the syslog file
    std::thread myThread(log_messages);
#endif

    log_messages();

#if __cplusplus >= 201103L
    // wait until the 2nd thread has finished
    myThread.join();
#endif

    // Calling closelog() is optional
    closelog();

    return 0;
}
