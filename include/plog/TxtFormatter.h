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

        static util::nstring format(const Entry& entry)
        {
            tm t;
            util::localtime_s(&t, &entry.m_time.time);

            util::nstringstream ss;
            ss << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << " ";
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << "." << std::setfill(PLOG_NSTR('0')) << std::setw(3) << entry.m_time.millitm << " ";
            ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << getLevelName(entry.m_severity) << " ";
            ss << "[" << entry.m_tid << "] ";
            ss << "[" << entry.m_func << "@" << entry.m_line << "] ";
            ss << entry.m_stream.str() << "\n";

            return ss.str();
        }
    };
}