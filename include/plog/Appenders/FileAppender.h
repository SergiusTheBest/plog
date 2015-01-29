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
            util::nstring str = Formatter::header();
            m_file.writeAsUTF8(str);
        }

        virtual void write(const Entry& entry)
        {
            util::nstring str = Formatter::format(entry);
            m_file.writeAsUTF8(str);
        }

    private:
        util::File m_file;
    };
}