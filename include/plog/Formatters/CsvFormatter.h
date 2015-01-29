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

        static util::nstring format(const Record& record)
        {
            tm t;
            util::localtime_s(&t, &record.m_time.time);

            util::nstringstream ss;
            ss << t.tm_year + 1900 << "/" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon << "/" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << ";";
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << "." << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.m_time.millitm << ";";
            ss << getLevelName(record.m_severity) << ";";
            ss << record.m_tid << ";";
            ss << record.m_object << ";";
            ss << record.func().c_str() << "@" << record.m_line << ";";

            util::nstringstream split(record.m_stream.str());
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