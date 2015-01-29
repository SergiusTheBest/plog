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

        static util::nstring format(const Entry& entry)
        {
            util::nstringstream ss;
            ss << entry.func().c_str() << "@" << entry.m_line << ": ";
            ss << entry.m_stream.str() << "\n";

            return ss.str();
        }
    };
}