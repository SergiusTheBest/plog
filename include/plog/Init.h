#pragma once
#include <plog/Logger.h>
#include <plog/CsvFormatter.h>
#include <plog/FileAppender.h>

namespace plog
{
    template<class CharType>
    inline void init_csv(const CharType* fileName, Level maxSeverity)
    {
        static FileAppender<CsvFormatter> fileAppender(fileName, maxSeverity);
        static Logger logger;

        logger.addAppender(&fileAppender);
    }

    // For backward compatibility    
    inline void free(bool = true)
    {
    }
}