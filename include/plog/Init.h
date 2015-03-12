#pragma once
#include <cstring>
#include <plog/Logger.h>
#include <plog/Formatters/CsvFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/FileAppender.h>
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
    // Empty initializer

    template<int instance>
    inline Logger<instance>& init(Severity maxSeverity = none)
    {
        static Logger<instance> logger(maxSeverity);
        return logger;
    }

    inline Logger<0>& init(Severity maxSeverity = none)
    {
        return init<0>(maxSeverity);
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with any Formatter

    template<int instance, class Formatter>
    inline Logger<instance>& init(const char* fileName, Severity maxSeverity)
    {
        static FileAppender<Formatter> fileAppender(fileName);
        return init<instance>(maxSeverity).addAppender(&fileAppender);
    }

    template<int instance, class Formatter>
    inline Logger<instance>& init(const char* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        static RollingFileAppender<Formatter> rollingFileAppender(fileName, maxFileSize, maxFiles);
        return init<instance>(maxSeverity).addAppender(&rollingFileAppender);
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with CSV

    template<int instance>
    inline Logger<instance>& initCsv(const char* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return maxFileSize ? init<instance, CsvFormatter>(fileName, maxSeverity, maxFileSize, maxFiles) : init<instance, CsvFormatter>(fileName, maxSeverity);
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with TXT

    template<int instance>
    inline Logger<instance>& initTxt(const char* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return maxFileSize ? init<instance, TxtFormatter>(fileName, maxSeverity, maxFileSize, maxFiles) : init<instance, TxtFormatter>(fileName, maxSeverity);
    }
    
    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with TXT/CSV chosen by file extension

    template<int instance>
    inline Logger<instance>& init(const char* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return isCsv(fileName) ? initCsv<instance>(fileName, maxSeverity, maxFileSize, maxFiles) : initTxt<instance>(fileName, maxSeverity, maxFileSize, maxFiles);
    }

    inline Logger<0>& init(const char* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<0>(fileName, maxSeverity, maxFileSize, maxFiles);
    }

    //////////////////////////////////////////////////////////////////////////
    // WCHAR variants for Windows

#ifdef _WIN32
    template<int instance>
    inline Logger<instance>& initCsv(const wchar_t* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<instance, CsvFormatter>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    template<int instance>
    inline Logger<instance>& initTxt(const wchar_t* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<instance, TxtFormatter>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    template<int instance>
    inline Logger<instance>& init(const wchar_t* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<instance>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    inline Logger<0>& init(const wchar_t* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<0>(fileName, maxSeverity, maxFileSize, maxFiles);
    }
#endif
}