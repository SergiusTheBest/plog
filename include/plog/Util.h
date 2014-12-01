#pragma once
#include <time.h>

namespace plog
{
    namespace util
    {
        inline unsigned int gettid()
        {
#ifdef _WIN32
            return ::GetCurrentThreadId();
#elif defined(LINUX)
            return syscall(__NR_gettid);
#elif defined(MACOS)
            return static_cast<unsigned int>(syscall(SYS_thread_selfid));
#else
#error Unsupported platform
#endif
        }

        inline void localtime_s(struct tm* t, const time_t* time)
        {
#ifdef _WIN32
            ::localtime_s(t, time);
#elif defined(LINUX)
            ::localtime_r(time, t);
#error Unsupported platform
#endif
        }

        //TODO: add file open func
        //HANDLE h = ::CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, 0, 0);
        //int fd = _open_osfhandle(reinterpret_cast<intptr_t>(h), _O_APPEND);
        //m_file = _fdopen(fd, "a");
    }
}