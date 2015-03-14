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
            util::localtime_s(&t, &record.m_time.time);

            util::nstringstream ss;
            ss << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << " ";
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << "." << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.m_time.millitm << " ";
            ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << getSeverityName(record.m_severity) << " ";
            ss << "[" << record.m_tid << "] ";
            ss << "[" << record.func().c_str() << "@" << record.m_line << "] ";
            ss << record.m_stream.str() << "\n";

            return ss.str();
        }
    };
}
