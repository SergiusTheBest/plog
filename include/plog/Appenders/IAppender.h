#pragma once
#include <plog/Record.h>
#include <plog/Severity.h>

namespace plog
{
    class IAppender
    {
    public:
        IAppender()
            : m_maxSeverity(verbose)
        {

        }

        virtual ~IAppender()
        {
        }

        void setMaxSeverity(Severity maxSeverity) 
        {
            m_maxSeverity = maxSeverity;
        }

        void writeLog(const Record& record) 
        {
            if (record.getSeverity() <= m_maxSeverity) 
                write(record);
        }

        virtual void write(const Record& record) = 0;

    private:
        Severity m_maxSeverity;
    };
}
