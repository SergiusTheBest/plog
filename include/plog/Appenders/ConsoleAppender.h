#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/Util.h>
#include <plog/WinApi.h>
#include <iostream>

namespace plog
{
    template<class Formatter>
    class ConsoleAppender : public IAppender
    {
    public:
#ifdef _WIN32
        ConsoleAppender() : m_isatty(!!_isatty(_fileno(stdout))), m_stdoutHandle()
        {
            if (m_isatty)
            {
                m_stdoutHandle = GetStdHandle(stdHandle::kOutput);
            }
        }
#else
        ConsoleAppender() : m_isatty(!!isatty(fileno(stdout))) {}
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
                WriteConsoleW(m_stdoutHandle, str.c_str(), static_cast<DWORD>(str.size()), NULL, NULL);
            }
            else
            {
                std::cout << util::toNarrow(str, codePage::kActive) << std::flush;
            }
#else
            std::cout << str << std::flush;
#endif
        }

    private:
#ifdef __BORLANDC__
        static int _isatty(int fd) { return ::isatty(fd); }
#endif

    protected:
        util::Mutex m_mutex;
        const bool  m_isatty;
#ifdef _WIN32
        HANDLE      m_stdoutHandle;
#endif
    };
}
