#pragma once
#include <stdio.h>

namespace plog
{
    template<class Formatter>
    class FileAppender : public Appender
    {
    public:
        FileAppender(const char* fileName, Level maxSeverity) : Appender(maxSeverity)
        {
            m_file = util::fopen(fileName, "a");
            writeHeader();
        }

#ifdef _WIN32
        FileAppender(const wchar_t* fileName, Level maxSeverity) : Appender(maxSeverity)
        {
            m_file = util::wfopen(fileName, L"a");
            writeHeader();
        }
#endif

        ~FileAppender()
        {
            fclose(m_file);
        }

        virtual void write(const Entry& entry)
        {
            std::string str = Formatter::format(entry);
            fwrite(str.c_str(), str.size(), 1, m_file);
        }

    private:
        void writeHeader()
        {
            std::string str = Formatter::header();
            fwrite(str.c_str(), str.size(), 1, m_file);
        }

    private:
        FILE* m_file;
    };
}