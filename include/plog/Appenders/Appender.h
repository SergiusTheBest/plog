#pragma once

namespace plog
{
    class Appender
    {
    public:
        Appender(Level maxSeverity) : m_maxSeverity(maxSeverity)
        {
        }

        virtual ~Appender() 
        {
        }

        virtual void write(const Record& record) = 0;

        Level getMaxSeverity() const
        {
            return m_maxSeverity;
        }

        void setMaxSeverity(Level severity)
        {
            m_maxSeverity = severity;
        }

    private:
        Level m_maxSeverity;
    };
}