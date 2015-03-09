#pragma once
#include <iostream>

namespace plog
{
    template<class Formatter>
    class ConsoleAppender : public Appender
    {
    public:
        ConsoleAppender(Severity maxSeverity) : Appender(maxSeverity)
        {
#ifdef _WIN32
            ::setlocale(LC_ALL, "");
#endif
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