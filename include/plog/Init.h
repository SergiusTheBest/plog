#pragma once
#include <plog/Logger.h>
#include <plog/CsvFormatter.h>
#include <plog/TxtFormatter.h>
#include <plog/FileAppender.h>

namespace plog
{
    namespace
    {
        bool isCsv(const char* fileName)
        {            
            const char kCsvExt[] = ".csv";
            const size_t kCsvExtLength = sizeof(kCsvExt) - 1;

            size_t len = ::strlen(fileName);
            return len >= kCsvExtLength && 0 == ::strcmp(&fileName[len - kCsvExtLength], kCsvExt);
        }
    }
    
    inline void initCsv(const char* fileName, Level maxSeverity)
    {
        static FileAppender<CsvFormatter> fileAppender(fileName, maxSeverity);
        static Logger logger;

        logger.addAppender(&fileAppender);
    }

    inline void initCsv(const wchar_t* fileName, Level maxSeverity)
    {
        initCsv(util::toString(fileName).c_str(), maxSeverity);
    }

    inline void initTxt(const char* fileName, Level maxSeverity)
    {
        static FileAppender<TxtFormatter> fileAppender(fileName, maxSeverity);
        static Logger logger;

        logger.addAppender(&fileAppender);
    }

    inline void initTxt(const wchar_t* fileName, Level maxSeverity)
    {
        initTxt(util::toString(fileName).c_str(), maxSeverity);
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
}