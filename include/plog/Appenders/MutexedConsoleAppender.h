#pragma once
#include "ConsoleAppender.h"
#include <mutex>

namespace plog
{
    template<class Formatter>
    class MutexedConsoleAppender : public ConsoleAppender<Formatter>
    {
    public:
        MutexedConsoleAppender(std::mutex& mutex) : m_mutex(mutex) {}

        virtual void write(const Record& record)
        {
            m_mutex.lock();
            plog::ConsoleAppender<Formatter>::write(record);
            m_mutex.unlock();
        }

    protected:
        std::mutex& m_mutex;
    };
}
