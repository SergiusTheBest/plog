#pragma once

namespace plog
{
    template<class Formatter>
    class FileAppender : public Appender
    {
    public:
        FileAppender(const char* fileName, Level maxSeverity) : Appender(maxSeverity)
        {
            m_file = _fsopen(fileName, L"a", _SH_DENYWR);
            writeHeader();
        }

        FileAppender(const wchar_t* fileName, Level maxSeverity) : Appender(maxSeverity)
        {
            m_file = _wfsopen(fileName, L"a", _SH_DENYWR);
            writeHeader();
        }

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