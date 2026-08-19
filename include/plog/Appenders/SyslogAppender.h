#pragma once
#include <plog/Appenders/IAppender.h>
#include <syslog.h>

namespace plog
{
    template<class Formatter>
    class PLOG_LINKAGE_HIDDEN SyslogAppender : public IAppender
    {
    public:
        virtual void write(const Record& record) PLOG_OVERRIDE
        {
            std::string str = Formatter::format(record);

            syslog(toPriority(record.getSeverity()), "%s", str.c_str());
        }

    private:
        static int toPriority(Severity severity)
        {
            switch (severity)
            {
            case fatal:
                return LOG_EMERG;
            case error:
                return LOG_ERR;
            case warning:
                return LOG_WARNING;
            case info:
                return LOG_NOTICE;
            case debug:
                return LOG_INFO;
            case verbose:
            default:
                return LOG_DEBUG;
            }
        }
    };
}
