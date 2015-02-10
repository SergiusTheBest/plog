#pragma once

// For backward compatibility with previous version of plog.

namespace plog
{
    const Level trace = debug;
    const Level minimum_log_level = none;
    const Level maximum_log_level = debug;
    
    typedef Level log_level;
    typedef Record message;

    inline void free(bool = true)
    {
    }

    template<class CharType>
    inline void init_csv(const CharType* fileName, Level maxSeverity)
    {
        initCsv(fileName, maxSeverity);
    }

    template<class CharType>
    inline void init_txt(const CharType* fileName, Level maxSeverity)
    {
        initTxt(fileName, maxSeverity);
    }

    #define LOG_TRACE LOG(plog::debug)
}