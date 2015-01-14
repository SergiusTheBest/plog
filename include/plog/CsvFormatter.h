#pragma once
#include <iomanip>

namespace plog
{
    class CsvFormatter
    {
    public:
        static std::wstring header()
        {
            return L"Date;Time;Severity;TID;This;Function;Message\n";
        }

        static std::wstring format(const Entry& entry)
        {
            tm t;
            util::localtime_s(&t, &entry.m_time.time);

            std::wstringstream ss;
            ss << t.tm_year + 1900 << "/" << std::setfill(L'0') << std::setw(2) << t.tm_mon << "/" << std::setfill(L'0') << std::setw(2) << t.tm_mday << ";";
            ss << std::setfill(L'0') << std::setw(2) << t.tm_hour << ":" << std::setfill(L'0') << std::setw(2) << t.tm_min << ":" << std::setfill(L'0') << std::setw(2) << t.tm_sec << "." << std::setfill(L'0') << std::setw(3) << entry.m_time.millitm << ";";
            ss << getLevelName(entry.m_severity) << ";";
            ss << entry.m_tid << ";";
            ss << entry.m_object << ";";
            ss << entry.m_func << "@" << entry.m_line << ";";

            std::wistringstream split(entry.m_stream.str());
            std::wstring token;

            while (!split.eof())
            {
                std::getline(split, token, L'"');
                ss << "\"" << token << "\"";
            }

            ss << "\n";

            return ss.str();
        }
    };
}