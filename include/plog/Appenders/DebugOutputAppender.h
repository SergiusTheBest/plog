#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/WinApi.h>

namespace plog
{
    template<class Formatter>
    class DebugOutputAppender : public IAppender
    {
    public:
        virtual void write(const Record& record)
        {

#if defined(PLOG_DISABLE_WCHAR_T)
            std::wostringstream _str ;
            _str << Formatter::format(record).c_str();
            std::wstring str = _str.str();
            OutputDebugStringW(str.c_str());
#else
            OutputDebugStringW(Formatter::format(record).c_str());
#endif
        }
    };
}
