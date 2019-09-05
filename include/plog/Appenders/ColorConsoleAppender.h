#pragma once
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/WinApi.h>

namespace plog
{
    template<class Formatter>
    class ColorConsoleAppender : public ConsoleAppender<Formatter>
    {
    public:
#ifdef _WIN32
        ColorConsoleAppender() : m_originalAttr()
        {
            if (this->m_isatty)
            {
                CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                GetConsoleScreenBufferInfo(this->m_stdoutHandle, &csbiInfo);

                m_originalAttr = csbiInfo.wAttributes;
            }
        }
#else
        ColorConsoleAppender() {}
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
            if (this->m_isatty)
            {
                switch (severity)
                {
#ifdef _WIN32
                case fatal:
                    SetConsoleTextAttribute(this->m_stdoutHandle, foreground::kRed | foreground::kGreen | foreground::kBlue | foreground::kIntensity | background::kRed); // white on red background
                    break;

                case error:
                    SetConsoleTextAttribute(this->m_stdoutHandle, static_cast<WORD>(foreground::kRed | foreground::kIntensity | (m_originalAttr & 0xf0))); // red
                    break;

                case warning:
                    SetConsoleTextAttribute(this->m_stdoutHandle, static_cast<WORD>(foreground::kRed | foreground::kGreen | foreground::kIntensity | (m_originalAttr & 0xf0))); // yellow
                    break;

                case debug:
                case verbose:
                    SetConsoleTextAttribute(this->m_stdoutHandle, static_cast<WORD>(foreground::kGreen | foreground::kBlue | foreground::kIntensity | (m_originalAttr & 0xf0))); // cyan
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
            if (this->m_isatty)
            {
#ifdef _WIN32
                SetConsoleTextAttribute(this->m_stdoutHandle, m_originalAttr);
#else
                std::cout << "\x1B[0m\x1B[0K";
#endif
            }
        }

    private:
#ifdef _WIN32
        WORD   m_originalAttr;
#endif
    };
}
