#pragma once
#include <cstring>
#include <plog/Logger.h>
#include <plog/Formatters/CsvFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/RollingFileAppender.h>

namespace plog
{
    namespace
    {
        bool isCsv(const char* fileName)
        {
            const char kCsvExt[] = ".csv";
            const size_t kCsvExtLength = sizeof(kCsvExt) - 1;

            size_t len = std::strlen(fileName);
            return len >= kCsvExtLength && 0 == std::strcmp(&fileName[len - kCsvExtLength], kCsvExt);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Empty initializer / one appender

    template<int instance>
    inline Logger<instance>& init(Severity maxSeverity = none, IAppender* appender = NULL)
    {
        static Logger<instance> logger(maxSeverity);
        return appender ? logger.addAppender(appender) : logger;
    }

    inline Logger<0>& init(Severity maxSeverity = none, IAppender* appender = NULL)
    {
        return init<0>(maxSeverity, appender);
    }

    //////////////////////////////////////////////////////////////////////////
    // RollingFileAppender with any Formatter

    template<class Formatter, int instance>
    inline Logger<instance>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        static RollingFileAppender<Formatter> rollingFileAppender(fileName, maxFileSize, maxFiles);
        return init<instance>(maxSeverity, &rollingFileAppender);
    }

    template<class Formatter>
    inline Logger<0>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<Formatter, 0>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    //////////////////////////////////////////////////////////////////////////
    // RollingFileAppender with TXT/CSV chosen by file extension

    template<int instance>
    inline Logger<instance>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return isCsv(fileName) ? init<CsvFormatter, instance>(maxSeverity, fileName, maxFileSize, maxFiles) : init<TxtFormatter, instance>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    inline Logger<0>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<0>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    //////////////////////////////////////////////////////////////////////////
    // WCHAR variants for Windows

#ifdef _WIN32
    template<class Formatter, int instance>
    inline Logger<instance>& init(Severity maxSeverity, const wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<Formatter, instance>(maxSeverity, util::toNarrow(fileName).c_str(), maxFileSize, maxFiles);
    }

    template<class Formatter>
    inline Logger<0>& init(Severity maxSeverity, const wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<Formatter, 0>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    template<int instance>
    inline Logger<instance>& init(Severity maxSeverity, const wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<instance>(maxSeverity, util::toNarrow(fileName).c_str(), maxFileSize, maxFiles);
    }

    inline Logger<0>& init(Severity maxSeverity, const wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<0>(maxSeverity, fileName, maxFileSize, maxFiles);
    }
#endif
}
