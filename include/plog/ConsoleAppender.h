#pragma once
#include <iostream>

namespace plog
{
    template<class Formatter>
    class ConsoleAppender : public Appender
    {
    public:
        ConsoleAppender(Level maxSeverity) : Appender(maxSeverity)
        {
            ::setlocale(LC_ALL, "");
        }

        virtual void write(const Entry& entry)
        {
            std::cout << Formatter::format(entry);
        }
    };
}