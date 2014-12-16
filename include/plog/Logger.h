#pragma once
#include <vector>
#include <plog/Appender.h>
#include <plog/Util.h>

namespace plog
{
    class Logger : public util::Singleton<Logger>
    {
    public:
        void addAppender(Appender* appender)
        {
            m_appenders.push_back(appender);
        }

        bool checkSeverity(Level severity) const
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

        void operator+=(const Entry& entry)
        {
            for (std::vector<Appender*>::iterator it = m_appenders.begin(); it != m_appenders.end(); ++it)
            {
                if ((*it)->getMaxSeverity() >= entry.m_severity)
                {
                    (*it)->write(entry);
                }
            }
        }

    private:
        std::vector<Appender*> m_appenders;
    };
}