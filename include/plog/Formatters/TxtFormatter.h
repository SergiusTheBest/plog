#pragma once
#include <iomanip>
#include <plog/Util.h>

namespace plog
{
    class TxtFormatter
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            tm t;
            util::localtime_s(&t, &record.getTime().time);

			char timeBuffer[32];
			strftime(timeBuffer, _countof(timeBuffer), "%Y-%m-%d %T", &t);

            util::nstringstream ss;
			ss << timeBuffer << PLOG_NSTR('.') << std::setfill(PLOG_NSTR('0')) << std::setw(3) 
			   << record.getTime().millitm << PLOG_NSTR(' ')
               << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << getSeverityName(record.getSeverity()) 
			   << PLOG_NSTR(' ')
               << PLOG_NSTR('[') << record.getTid() << PLOG_NSTR("] ")
               << PLOG_NSTR('[') << record.getFunc().c_str() << PLOG_NSTR('@') << record.getLine() << PLOG_NSTR("] ")
               << record.getMessage().c_str() << PLOG_NSTR('\n');

            return ss.str();
        }
    };
}
