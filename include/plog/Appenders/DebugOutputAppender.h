#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/WinApi.h>

namespace plog
{
    template<class Formatter>
    class PLOG_LINKAGE_HIDDEN DebugOutputAppender : public IAppender
    {
    public:
        virtual void write(const Record& record)
        {
            OutputDebugStringW(Formatter::format(record).c_str());
        }
    };
}
