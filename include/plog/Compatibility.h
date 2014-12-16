#pragma once

// For backward compatibility with previous version of plog.

namespace plog
{
    const Level trace = debug;
    const Level minimum_log_level = none;
    const Level maximum_log_level = debug;
    
    typedef Level log_level;

    inline void free(bool = true)
    {
    }

    inline void init_csv(const char* fileName, Level maxSeverity)
    {
        initCsv(fileName, maxSeverity);
    }

    inline void init_csv(const wchar_t* fileName, Level maxSeverity)
    {
        initCsv(fileName, maxSeverity);
    }

    inline void init_txt(const char* fileName, Level maxSeverity)
    {
        initTxt(fileName, maxSeverity);
    }

    inline void init_txt(const wchar_t* fileName, Level maxSeverity)
    {
        initTxt(fileName, maxSeverity);
    }

    #define LOG_TRACE LOG(plog::debug)
}