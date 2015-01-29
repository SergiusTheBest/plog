#pragma once
#include <cstring>
#include <plog/Logger.h>
#include <plog/CsvFormatter.h>
#include <plog/TxtFormatter.h>
#include <plog/FileAppender.h>
#include <plog/RollingFileAppender.h>

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

    inline Logger& init()
    {
        static Logger logger;
        return logger;
    }

    template<class Formatter>
    inline Logger& init(const char* fileName, Level maxSeverity)
    {
        static FileAppender<Formatter> fileAppender(fileName, maxSeverity);
        return init().addAppender(&fileAppender);
    }

    template<class Formatter>
    inline Logger& init(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        static RollingFileAppender<Formatter> rollingFileAppender(fileName, maxSeverity, maxFileSize, maxFiles);
        return init().addAppender(&rollingFileAppender);
    }
    
    inline Logger& initCsv(const char* fileName, Level maxSeverity)
    {
        return init<CsvFormatter>(fileName, maxSeverity);
    }

    inline Logger& initCsv(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<CsvFormatter>(fileName, maxSeverity, maxFileSize, maxFiles);
    }

    inline Logger& initTxt(const char* fileName, Level maxSeverity)
    {
        return init<TxtFormatter>(fileName, maxSeverity);
    }

    inline Logger& initTxt(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<TxtFormatter>(fileName, maxSeverity, maxFileSize, maxFiles);
    }
    
    inline Logger& init(const char* fileName, Level maxSeverity)
    {
        if (isCsv(fileName))
        {
            return initCsv(fileName, maxSeverity);
        }
        else
        {
            return initTxt(fileName, maxSeverity);
        }
    }

    inline Logger& init(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        if (isCsv(fileName))
        {
            return initCsv(fileName, maxSeverity, maxFileSize, maxFiles);
        }
        else
        {
            return initTxt(fileName, maxSeverity, maxFileSize, maxFiles);
        }
    }

#ifdef _WIN32
    inline Logger& initCsv(const wchar_t* fileName, Level maxSeverity)
    {
        return initCsv(util::toString(fileName).c_str(), maxSeverity);
    }

    inline Logger& initCsv(const wchar_t* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<CsvFormatter>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    inline Logger& initTxt(const wchar_t* fileName, Level maxSeverity)
    {
        return init<TxtFormatter>(util::toString(fileName).c_str(), maxSeverity);
    }

    inline Logger& initTxt(const wchar_t* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init<TxtFormatter>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    inline Logger& init(const wchar_t* fileName, Level maxSeverity)
    {
        return init(util::toString(fileName).c_str(), maxSeverity);
    }

    inline Logger& init(const wchar_t* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        return init(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }
#endif
}