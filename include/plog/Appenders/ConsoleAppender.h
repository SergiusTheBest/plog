#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/Util.h>
#include <plog/WinApi.h>
#include <iostream>

namespace plog
{
    enum class OutputStream
    {
        kCout,
        kCerr
    };

    template<class Formatter>
    class ConsoleAppender : public IAppender
    {
    public:
#ifdef _WIN32
        ConsoleAppender(OutputStream outStream = OutputStream::kCout)
            : m_isatty(!!_isatty(_fileno(outStream == OutputStream::kCout ? stdout : stderr)))
            , m_outputStream(outStream == OutputStream::kCout ? std::cout : std::cerr)
            , m_outputHandle()
        {
            if (m_isatty)
            {
                m_outputHandle = GetStdHandle(outStream == OutputStream::kCout ? stdHandle::kOutput : stdHandle::kErrorOutput);
            }
        }
#else
        ConsoleAppender(OutputStream outStream = OutputStream::kCout) 
            : m_isatty(!!isatty(fileno(outStream == OutputStream::kCout ? stdout : stderr))) 
            , m_outputStream(outStream == OutputStream::kCout ? std::cout : std::cerr)
        {}
#endif

        virtual void write(const Record& record)
        {
            util::nstring str = Formatter::format(record);
            util::MutexLock lock(m_mutex);

            writestr(str);
        }

    protected:
        void writestr(const util::nstring& str)
        {
#ifdef _WIN32
            if (m_isatty)
            {
                WriteConsoleW(m_outputHandle, str.c_str(), static_cast<DWORD>(str.size()), NULL, NULL);
            }
            else
            {
                m_outputStream << util::toNarrow(str, codePage::kActive) << std::flush;
            }
#else
            m_outputStream << str << std::flush;
#endif
        }

    private:
#ifdef __BORLANDC__
        static int _isatty(int fd) { return ::isatty(fd); }
#endif

    protected:
        util::Mutex m_mutex;
        const bool  m_isatty;
        std::ostream& m_outputStream;
#ifdef _WIN32
        HANDLE      m_outputHandle;
#endif
    };
}
