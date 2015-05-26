#pragma once
#include "Util.h"

namespace plog
{
    enum Severity
    {
        none = 0,
        fatal = 1,
        error = 2,
        warning = 3,
        info = 4,
        debug = 5,
        verbose = 6
    };

	inline util::nstring::const_pointer getSeverityName(Severity severity)
	{
		switch (severity)
		{
		case fatal:
			return PLOG_NSTR("FATAL");
		case error:
			return PLOG_NSTR("ERROR");
		case warning:
			return PLOG_NSTR("WARN");
		case info:
			return PLOG_NSTR("INFO");
		case debug:
			return PLOG_NSTR("DEBUG");
		case verbose:
			return PLOG_NSTR("VERB");
		default:
			return PLOG_NSTR("NONE");
		}
	}
}
