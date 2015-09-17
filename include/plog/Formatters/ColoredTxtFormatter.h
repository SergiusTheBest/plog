#pragma once
#include <iomanip>
#include <plog/Util.h>
#define COLOR_RESET          "\x1B[0m"
#define COLOR_RED            "\x1B[31m"
#define COLOR_RED_BACKG      "\e[97m\e[41m"
#define COLOR_YELLOW         "\x1B[33m"
#define COLOR_LIGHT_BLUE     "\x1B[94m"
#define COLOR_LIGHT_CYAN     "\x1B[96m"
#define COLOR_LIGHT_GRAY     "\x1B[97m"

namespace plog
{

    class ColoredTxtFormatter
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

            util::nstringstream ss;
            ss << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << " ";
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << ":" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << "." << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.getTime().millitm << " ";
            
#ifndef _WIN32
            switch (record.getSeverity())
            {
            case fatal:
                ss << COLOR_RED_BACKG; break;
            case error:
                ss << COLOR_RED; break;
            case warning:
                ss << COLOR_YELLOW; break;
            case debug:
                ss << COLOR_LIGHT_BLUE; break;
            case verbose:
                ss << COLOR_LIGHT_CYAN; break;
            case none:
            case info:
                ss << COLOR_LIGHT_GRAY;
            }

            ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << getSeverityName(record.getSeverity()) << COLOR_RESET << " ";
#else
            ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << getSeverityName(record.getSeverity()) << " ";
#endif

            ss << "[" << record.getTid() << "] ";
            ss << "[" << record.getFunc().c_str() << "@" << record.getLine() << "] ";
            ss << record.getMessage().c_str() << "\n";

            return ss.str();
        }
    };
}
