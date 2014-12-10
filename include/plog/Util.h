#pragma once
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int CodePage, unsigned int dwFlags, const wchar_t* lpWideCharStr, int cchWideChar, char* lpMultiByteStr, int cbMultiByte, const char* lpDefaultChar, int* lpUsedDefaultChar);
#elif
#include <unistd.h>
#include <sys/syscall.h>
#endif

namespace plog
{
    namespace util
    {
        inline void localtime_s(struct tm* t, const time_t* time)
        {
#ifdef _WIN32
            ::localtime_s(t, time);
#else
            ::localtime_r(time, t);
#endif
        }

        inline unsigned int gettid()
        {
#ifdef _WIN32
            return ::GetCurrentThreadId();
#elif defined(__unix__)
            return ::syscall(__NR_gettid);
#elif defined(__APPLE__)
            return static_cast<unsigned int>(::syscall(SYS_thread_selfid));
#endif
        }

        class File
        {
        public:
            File() : m_fd(-1)
            {
            }

            File(const char* fileName) : m_fd(-1)
            {
                open(fileName);
            }

            ~File()
            {
                close();
            }

            void open(const char* fileName)
            {
#ifdef _WIN32
                ::_sopen_s(&m_fd, fileName, O_CREAT | O_APPEND | O_TEXT | O_WRONLY, _SH_DENYWR, S_IREAD | S_IWRITE);
#else
                ::open(&m_fd, fileName, O_CREAT | O_APPEND | O_TEXT | O_WRONLY, S_IREAD | S_IWRITE);
#endif
            }

            void write(const void* buf, size_t count)
            {
#ifdef _WIN32
                ::_write(m_fd, buf, count);
#else
                ::write(m_fd, buf, count);
#endif
            }

            void close()
            {
                if (-1 != m_fd)
                {
#ifdef _WIN32
                    ::_close(m_fd);
#else
                    ::close(m_fd);
#endif
                    m_fd = -1;
                }
            }

        private:
            File(const File&);
            File& operator=(const File&);

        private:
            int m_fd;
        };

        inline std::string toString(const wchar_t* wstr)
        {
            //std::mbstate_t state = std::mbstate_t();
            //int len = 1 + std::wcsrtombs(0, &wstr, 0, &state);
            int len = ::WideCharToMultiByte(0, 0, wstr, wcslen(wstr), 0, 0, 0, 0);
                
            std::string str(len, 0);
            ::WideCharToMultiByte(0, 0, wstr, wcslen(wstr), &str[0], str.size(), 0, 0);
            //std::wcsrtombs(&str[0], &wstr, str.size(), &state);

            return str;
        }

        //TODO: add file open func
        //HANDLE h = ::CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, 0, 0);
        //int fd = _open_osfhandle(reinterpret_cast<intptr_t>(h), _O_APPEND);
        //m_file = _fdopen(fd, "a");
    }
}