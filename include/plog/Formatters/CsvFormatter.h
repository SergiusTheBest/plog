#pragma once
#include <plog/Record.h>
#include <plog/Util.h>
#include <iomanip>

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
            util::localtime_s(&t, &record.getTime().time);

            util::nostringstream ss;
            util::nstring date, timeofday;
            util::formatTime(&record.getTime(), &date, &timeofday);
            ss << date << PLOG_NSTR(";") << timeofday << PLOG_NSTR(";");
            ss << severityToString(record.getSeverity()) << PLOG_NSTR(";");
            ss << record.getTid() << PLOG_NSTR(";");
            ss << record.getObject() << PLOG_NSTR(";");
            ss << record.getFunc() << PLOG_NSTR("@") << record.getLine() << PLOG_NSTR(";");

            util::nstring message = record.getMessage();

            if (message.size() > kMaxMessageSize)
            {
                message.resize(kMaxMessageSize);
                message.append(PLOG_NSTR("..."));
            }

            util::nistringstream split(message);
            util::nstring token;

            while (!split.eof())
            {
                std::getline(split, token, PLOG_NSTR('"'));
                ss << PLOG_NSTR("\"") << token << PLOG_NSTR("\"");
            }

            ss << PLOG_NSTR("\n");

            return ss.str();
        }

        static const size_t kMaxMessageSize = 32000;
    };
}
