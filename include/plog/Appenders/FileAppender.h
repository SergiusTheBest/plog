#pragma once
#include <plog/Util.h>

namespace plog
{
    template<class Formatter>
    class FileAppender : public Appender
    {
    public:
        FileAppender(const char* fileName, Severity maxSeverity) : Appender(maxSeverity), m_file(fileName)
        {
            util::nstring str = Formatter::header();
            m_file.writeAsUTF8(str);
        }

        virtual void write(const Record& record)
        {
            util::nstring str = Formatter::format(record);
            m_file.writeAsUTF8(str);
        }

    private:
        util::File m_file;
    };
}