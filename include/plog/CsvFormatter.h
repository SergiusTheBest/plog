#pragma once
#include <iomanip>
#include <string>
#include <plog/Util.h>

namespace plog
{
    class CsvFormatter
    {
    public:
        static util::nstring header()
        {
            return PLOG_NSTR("Date;Time;Severity;TID;This;Function;Message\n");
        }

        static util::nstring format(const Entry& entry)
        {
            tm t;
            util::localtime_s(&t, &entry.m_time.time);

            util::nstringstream ss;
            ss << t.tm_year + 1900 << "/" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon << "/" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << ";";
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << "." << std::setfill(PLOG_NSTR('0')) << std::setw(3) << entry.m_time.millitm << ";";
            ss << getLevelName(entry.m_severity) << ";";
            ss << entry.m_tid << ";";
            ss << entry.m_object << ";";
            ss << entry.func().c_str() << "@" << entry.m_line << ";";

            util::nstringstream split(entry.m_stream.str());
            util::nstring token;

            while (!split.eof())
            {
                std::getline(split, token, PLOG_NSTR('"'));
                ss << "\"" << token << "\"";
            }

            ss << "\n";

            return ss.str();
        }
    };
}