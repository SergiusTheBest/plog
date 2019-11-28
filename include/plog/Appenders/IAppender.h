#pragma once
#include <plog/Record.h>
#include <plog/Util.h>

namespace plog
{
    class PLOG_DLL IAppender
    {
    public:
        virtual ~IAppender()
        {
        }

        virtual void write(const Record& record) = 0;
    };
}
