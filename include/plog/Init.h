#pragma once
#include <plog/Logger.h>
#include <plog/CsvFormatter.h>
#include <plog/FileAppender.h>

namespace plog
{
    inline void init_csv(const char* fileName, Level maxSeverity)
    {
        static FileAppender<CsvFormatter> fileAppender(fileName, maxSeverity);
        static Logger logger;

        logger.addAppender(&fileAppender);
    }

    inline void init_csv(const wchar_t* fileName, Level maxSeverity)
    {
        init_csv(util::toString(fileName).c_str(), maxSeverity);
    }
    
    inline void init(const char* fileName, Level maxSeverity)
    {
        init_csv(fileName, maxSeverity);
    }

    inline void init(const wchar_t* fileName, Level maxSeverity)
    {
        init_csv(fileName, maxSeverity);
    }

    // For backward compatibility
    inline void free(bool = true)
    {
    }
}