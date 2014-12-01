#pragma once

namespace plog
{
    enum Level
    {
        none = 0,
        fatal = 1,
        error = 2,
        warning = 3,
        info = 4,
        debug = 5,
        trace = 5, // For backward compatibility (trace level is retired)
    };

    inline const char* getLevelName(Level level)
    {
        switch (level)
        {
        case fatal:
            return "FATAL";
        case error:
            return "ERROR";
        case warning:
            return "WARNING";
        case info:
            return "INFO";
        case debug:
            return "DEBUG";
        default:
            return "NONE";
        }
    }
}