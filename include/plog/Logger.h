#pragma once
#include <cassert>
#include <vector>
#include <plog/Appender.h>

namespace plog
{
    static class Logger* g_instance;

    class Logger
    {
    public:
        Logger()
        {
            assert(!g_instance);
            g_instance = this;
        }

        ~Logger()
        {
            assert(g_instance);
            g_instance = 0;
        }

        static Logger& getInstance()
        {
            assert(g_instance);
            return *g_instance;
        }

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