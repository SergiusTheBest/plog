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
    inline Logger<instance>& init()
    {
        static Logger<instance> logger;
        return logger;
    }

    inline Logger<0>& init()
    {
        return init<0>();
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with any Formatter

    template<int instance, class Formatter>
    inline Logger<instance>& init(const char* fileName, Severity maxSeverity)
    {
        static FileAppender<Formatter> fileAppender(fileName, maxSeverity);
        return init<instance>().addAppender(&fileAppender);
    }

    template<int instance, class Formatter>
    inline Logger<instance>& init(const char* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        static RollingFileAppender<Formatter> rollingFileAppender(fileName, maxSeverity, maxFileSize, maxFiles);
        return init<instance>().addAppender(&rollingFileAppender);
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with CSV

    template<int instance>
    inline Logger<instance>& initCsv(const char* fileName, Severity maxSeverity)
    {
        return init<instance, CsvFormatter>(fileName, maxSeverity);
    }

    template<int instance>
    inline Logger<instance>& initCsv(const char* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<instance, CsvFormatter>(fileName, maxSeverity, maxFileSize, maxFiles);
    }

    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with TXT

    template<int instance>
    inline Logger<instance>& initTxt(const char* fileName, Severity maxSeverity)
    {
        return init<instance, TxtFormatter>(fileName, maxSeverity);
    }

    template<int instance>
    inline Logger<instance>& initTxt(const char* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<instance, TxtFormatter>(fileName, maxSeverity, maxFileSize, maxFiles);
    }
    
    //////////////////////////////////////////////////////////////////////////
    // File/RollingFile with TXT/CSV chosen by file extension

    template<int instance>
    inline Logger<instance>& init(const char* fileName, Severity maxSeverity)
    {
        if (isCsv(fileName))
        {
            return initCsv<instance>(fileName, maxSeverity);
        }
        else
        {
            return initTxt<instance>(fileName, maxSeverity);
        }
    }

    inline Logger<0>& init(const char* fileName, Severity maxSeverity)
    {
        return init<0>(fileName, maxSeverity);
    }

    template<int instance>
    inline Logger<instance>& init(const char* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        if (isCsv(fileName))
        {
            return initCsv<instance>(fileName, maxSeverity, maxFileSize, maxFiles);
        }
        else
        {
            return initTxt<instance>(fileName, maxSeverity, maxFileSize, maxFiles);
        }
    }

    inline Logger<0>& init(const char* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<0>(fileName, maxSeverity, maxFileSize, maxFiles);
    }

    //////////////////////////////////////////////////////////////////////////
    // WCHAR variants for Windows

#ifdef _WIN32
    template<int instance>
    inline Logger<instance>& initCsv(const wchar_t* fileName, Severity maxSeverity)
    {
        return initCsv<instance>(util::toString(fileName).c_str(), maxSeverity);
    }

    template<int instance>
    inline Logger<instance>& initCsv(const wchar_t* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<instance, CsvFormatter>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    template<int instance>
    inline Logger<instance>& initTxt(const wchar_t* fileName, Severity maxSeverity)
    {
        return init<instance, TxtFormatter>(util::toString(fileName).c_str(), maxSeverity);
    }

    template<int instance>
    inline Logger<instance>& initTxt(const wchar_t* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<instance, TxtFormatter>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    template<int instance>
    inline Logger<instance>& init(const wchar_t* fileName, Severity maxSeverity)
    {
        return init<instance>(util::toString(fileName).c_str(), maxSeverity);
    }

    inline Logger<0>& init(const wchar_t* fileName, Severity maxSeverity)
    {
        return init<0>(fileName, maxSeverity);
    }

    template<int instance>
    inline Logger<instance>& init(const wchar_t* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<instance>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    inline Logger<0>& init(const wchar_t* fileName, Severity maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<0>(fileName, maxSeverity, maxFileSize, maxFiles);
    }
#endif
}