#pragma once
#include <plog/LoggerHolder.h>

namespace plog
{
    template<int instanceId>
    inline Logger& init(Severity maxSeverity = none, IAppender* appender = NULL)
    {
        static Logger logger(maxSeverity);
        LoggerHolderAccessor<instanceId>::set(&logger);

        return appender ? logger.addAppender(appender) : logger;
    }

    inline Logger& init(Severity maxSeverity = none, IAppender* appender = NULL)
    {
        return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
    }
}
