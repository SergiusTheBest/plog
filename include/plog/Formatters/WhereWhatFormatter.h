#pragma once
#include <iomanip>
#include <plog/Util.h>

namespace plog
{
    class WhereWhatFormatter
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            util::nstringstream ss;
            ss << record.func().c_str() << "@" << record.m_line << ": ";
            ss << record.m_stream.str() << "\n";

            return ss.str();
        }
    };
}