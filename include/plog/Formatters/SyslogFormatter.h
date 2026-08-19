#pragma once
#include <plog/Record.h>
#include <plog/Util.h>
#include <utility>

namespace plog
{
    class SyslogFormatter
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            util::nostringstream ss;
            ss << PLOG_NSTR("[") << record.getTid() << PLOG_NSTR("] ")
               << record.getFunc() << PLOG_NSTR("@") << record.getLine()
               << PLOG_NSTR(" ") << record.getMessage() /* no line feed */;

#if __cplusplus > 201703L
            return std::move(ss).str();
#else
            return ss.str();
#endif
        }
    };
}
