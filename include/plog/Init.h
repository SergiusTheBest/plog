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

    template<class Formatter>
    inline void init(const char* fileName, Level maxSeverity)
    {
        static FileAppender<Formatter> fileAppender(fileName, maxSeverity);
        static Logger logger;

        logger.addAppender(&fileAppender);
    }

    template<class Formatter>
    inline void init(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        static RollingFileAppender<Formatter> rollingFileAppender(fileName, maxSeverity, maxFileSize, maxFiles);
        static Logger logger;

        logger.addAppender(&rollingFileAppender);
    }
    
    inline void initCsv(const char* fileName, Level maxSeverity)
    {
        init<CsvFormatter>(fileName, maxSeverity);
    }

    inline void initCsv(const wchar_t* fileName, Level maxSeverity)
    {
        initCsv(util::toString(fileName).c_str(), maxSeverity);
    }

    inline void initCsv(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        init<CsvFormatter>(fileName, maxSeverity, maxFileSize, maxFiles);
    }

    inline void initCsv(const wchar_t* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        init<CsvFormatter>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }

    inline void initTxt(const char* fileName, Level maxSeverity)
    {
        init<TxtFormatter>(fileName, maxSeverity);
    }

    inline void initTxt(const wchar_t* fileName, Level maxSeverity)
    {
        init<TxtFormatter>(util::toString(fileName).c_str(), maxSeverity);
    }

    inline void initTxt(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        init<TxtFormatter>(fileName, maxSeverity, maxFileSize, maxFiles);
    }

    inline void initTxt(const wchar_t* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        init<TxtFormatter>(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }
    
    inline void init(const char* fileName, Level maxSeverity)
    {
        if (isCsv(fileName))
        {
            initCsv(fileName, maxSeverity);
        }
        else
        {
            initTxt(fileName, maxSeverity);
        }
    }

    inline void init(const wchar_t* fileName, Level maxSeverity)
    {
        init(util::toString(fileName).c_str(), maxSeverity);
    }

    inline void init(const char* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        if (isCsv(fileName))
        {
            initCsv(fileName, maxSeverity, maxFileSize, maxFiles);
        }
        else
        {
            initTxt(fileName, maxSeverity, maxFileSize, maxFiles);
        }
    }

    inline void init(const wchar_t* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles)
    {
        init(util::toString(fileName).c_str(), maxSeverity, maxFileSize, maxFiles);
    }    
}