#pragma once

namespace plog
{
    class CsvFormatter
    {
    public:
        static std::string header()
        {
            return "Date;Time;Severity;TID;This;Function;Message\n";
        }

        static std::string format(const Entry& entry)
        {
            tm t;
            util::localtime_s(&t, &entry.m_time.time);

            std::stringstream ss;
            ss << t.tm_year + 1900 << "/" << t.tm_mon << "/" << t.tm_mday << ";";
            ss << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << "." << entry.m_time.millitm << ";";
            ss << getLevelName(entry.m_severity) << ";";
            ss << entry.m_tid << ";";
            ss << entry.m_object << ";";
            ss << entry.m_func << "@" << entry.m_line << ";";
            ss << entry.m_stream.str() << "\n";
            
            return ss.str();
        }
    };
}