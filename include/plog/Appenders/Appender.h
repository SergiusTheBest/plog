#pragma once

namespace plog
{
    class Appender
    {
    public:
        Appender(Severity maxSeverity) : m_maxSeverity(maxSeverity)
        {
        }

        virtual ~Appender() 
        {
        }

        virtual void write(const Record& record) = 0;

        Severity getMaxSeverity() const
        {
            return m_maxSeverity;
        }

        void setMaxSeverity(Severity severity)
        {
            m_maxSeverity = severity;
        }

    private:
        Severity m_maxSeverity;
    };
}