#pragma once
#include <vector>
#include <plog/Appenders/Appender.h>
#include <plog/Util.h>

namespace plog
{
    template<int instance>
    class Logger : public util::Singleton<Logger<instance> >
    {
    public:
        Logger& addAppender(Appender* appender)
        {
            m_appenders.push_back(appender);
            return *this;
        }

        bool checkSeverity(Severity severity) const
        {
            for (std::vector<Appender*>::const_iterator it = m_appenders.begin(); it != m_appenders.end(); ++it)
            {
                if ((*it)->getMaxSeverity() >= severity)
                {
                    return true;
                }
            }

            return false;
        }

        void operator+=(const Record& record)
        {
            for (std::vector<Appender*>::iterator it = m_appenders.begin(); it != m_appenders.end(); ++it)
            {
                if ((*it)->getMaxSeverity() >= record.m_severity)
                {
                    (*it)->write(record);
                }
            }
        }

    private:
        std::vector<Appender*> m_appenders;
    };
}