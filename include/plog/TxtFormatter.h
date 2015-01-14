#pragma once
#include <iomanip>

namespace plog
{
    class TxtFormatter
    {
    public:
        static std::wstring header()
        {
            return std::wstring();
        }

        static std::wstring format(const Entry& entry)
        {
            tm t;
            util::localtime_s(&t, &entry.m_time.time);

            std::wstringstream ss;
            ss << t.tm_year + 1900 << "-" << std::setfill(L'0') << std::setw(2) << t.tm_mon << "-" << std::setfill(L'0') << std::setw(2) << t.tm_mday << " ";
            ss << std::setfill(L'0') << std::setw(2) << t.tm_hour << ":" << std::setfill(L'0') << std::setw(2) << t.tm_min << ":" << std::setfill(L'0') << std::setw(2) << t.tm_sec << "." << std::setfill(L'0') << std::setw(3) << entry.m_time.millitm << " ";
            ss << std::setfill(L' ') << std::setw(5) << std::left << getLevelName(entry.m_severity) << " ";
            ss << "[" << entry.m_tid << "] ";
            ss << "[" << entry.m_func << "@" << entry.m_line << "] ";
            ss << entry.m_stream.str() << "\n";

            return ss.str();
        }
    };
}