#pragma once
#include <plog/Appenders/ConsoleAppender.h>

namespace plog
{
    template<class Formatter>
    class ColorConsoleAppender : public ConsoleAppender<Formatter>
    {
    public:
#ifdef _WIN32
        ColorConsoleAppender() : m_isatty(!!_isatty(_fileno(stdout))), m_stdoutHandle(), m_originalAttr()
        {
            if (m_isatty)
            {
                m_stdoutHandle = winapi::GetStdHandle(winapi::kStdOutputHandle);

                winapi::CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                winapi::GetConsoleScreenBufferInfo(m_stdoutHandle, &csbiInfo);

                m_originalAttr = csbiInfo.wAttributes;
            }
        }
#else
        ColorConsoleAppender() : m_isatty(!!::isatty(::fileno(stdout))) {}
#endif

#ifdef __BORLANDC__
        static int _isatty(int fd) { return ::isatty(fd); }
#endif

        virtual void write(const Record& record)
        {
            util::nstring str = Formatter::format(record);
            util::MutexLock lock(this->m_mutex);

            setColor(record.getSeverity());
            this->writestr(str);
            resetColor();
        }

    private:
        void setColor(Severity severity)
        {
            if (m_isatty)
            {
                switch (severity)
                {
#ifdef _WIN32
                case fatal:
                    winapi::SetConsoleTextAttribute(m_stdoutHandle, winapi::kForegroundRed | winapi::kForegroundGreen | winapi::kForegroundBlue | winapi::kForegroundIntensity | winapi::kBackgroundRed); // white on red background
                    break;

                case error:
                    winapi::SetConsoleTextAttribute(m_stdoutHandle, winapi::kForegroundRed | winapi::kForegroundIntensity | (m_originalAttr & 0xf0)); // red
                    break;

                case warning:
                    winapi::SetConsoleTextAttribute(m_stdoutHandle, winapi::kForegroundRed | winapi::kForegroundGreen | winapi::kForegroundIntensity | (m_originalAttr & 0xf0)); // yellow
                    break;

                case debug:
                case verbose:
                    winapi::SetConsoleTextAttribute(m_stdoutHandle, winapi::kForegroundGreen | winapi::kForegroundBlue | winapi::kForegroundIntensity | (m_originalAttr & 0xf0)); // cyan
                    break;
#else
                case fatal:
                    std::cout << "\x1B[97m\x1B[41m"; // white on red background
                    break;

                case error:
                    std::cout << "\x1B[91m"; // red
                    break;

                case warning:
                    std::cout << "\x1B[93m"; // yellow
                    break;

                case debug:
                case verbose:
                    std::cout << "\x1B[96m"; // cyan
                    break;
#endif
                default:
                    break;
                }
            }
        }

        void resetColor()
        {
            if (m_isatty)
            {
#ifdef _WIN32
                winapi::SetConsoleTextAttribute(m_stdoutHandle, m_originalAttr);
#else
                std::cout << "\x1B[0m\x1B[0K";
#endif
            }
        }

    private:
        bool m_isatty;
#ifdef _WIN32
        winapi::HANDLE m_stdoutHandle;
        winapi::WORD   m_originalAttr;
#endif
    };
}
