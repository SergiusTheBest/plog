#pragma once

// For backward compatibility with previous version of plog.

namespace plog
{
    const Severity trace = debug;
    const Severity minimum_log_level = none;
    const Severity maximum_log_level = debug;
    
    typedef Severity log_level;
    typedef Record message;

    inline void free(bool = true)
    {
    }

    template<class CharType>
    inline void init_csv(const CharType* fileName, Severity maxSeverity)
    {
        initCsv(fileName, maxSeverity);
    }

    template<class CharType>
    inline void init_txt(const CharType* fileName, Severity maxSeverity)
    {
        initTxt(fileName, maxSeverity);
    }

    #define LOG_TRACE LOG(plog::debug)
}