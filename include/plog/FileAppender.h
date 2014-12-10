#pragma once
#include <plog/Util.h>

namespace plog
{
    template<class Formatter>
    class FileAppender : public Appender
    {
    public:
        FileAppender(const char* fileName, Level maxSeverity) : Appender(maxSeverity), m_file(fileName)
        {
            std::string str = Formatter::header();
            m_file.write(str.c_str(), str.size());
        }

        ~FileAppender()
        {
        }

        virtual void write(const Entry& entry)
        {
            std::string str = Formatter::format(entry);
            m_file.write(str.c_str(), str.size());
        }

    private:
        util::File m_file;
    };
}