#pragma once
#include <plog/Default.h>

//////////////////////////////////////////////////////////////////////////
// This file provides backward compatibility with the old version of plog.

#define LOG_TRACE LOG_VERBOSE

namespace plog
{
    const Severity trace = verbose;
    const Severity minimum_log_level = none;
    const Severity maximum_log_level = verbose;
    
    typedef Severity Level;
    typedef Severity log_level;
    typedef Record message;

    inline void free(bool = true)
    {
    }

    template<class CharType>
    inline void init_csv(const CharType* fileName, Severity maxSeverity)
    {
        init<CsvFormatter, PLOG_DEFAULT_INSTANCE>(maxSeverity, fileName);
    }

    template<class CharType>
    inline void init_txt(const CharType* fileName, Severity maxSeverity)
    {
        init<TxtFormatter, PLOG_DEFAULT_INSTANCE>(maxSeverity, fileName);
    }

    template<int instance, class CharType>
    inline Logger<instance>& init(const CharType* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<instance>(maxSeverity, fileName, maxFileSize, maxFiles);
    }

    template<class CharType>
    inline Logger<PLOG_DEFAULT_INSTANCE>& init(const CharType* fileName, Severity maxSeverity, size_t maxFileSize = 0, int maxFiles = 0)
    {
        return init<PLOG_DEFAULT_INSTANCE>(maxSeverity, fileName, maxFileSize, maxFiles);
    }
}
