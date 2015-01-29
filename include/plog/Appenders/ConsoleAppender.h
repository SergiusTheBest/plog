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

        virtual void write(const Record& record)
        {
#ifdef _WIN32
            std::wcout << Formatter::format(record);
#else
            std::cout << Formatter::format(record);
#endif
        }
    };
}