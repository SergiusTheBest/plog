#pragma once
#include <time.h>
#include <stdio.h>

#ifdef _WIN32
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
#elif defined(__unix__)
#include <unistd.h>
#include <sys/syscall.h>
#endif

namespace plog
{
    namespace util
    {
        inline unsigned int gettid()
        {
#ifdef _WIN32
            return ::GetCurrentThreadId();
#elif defined(__unix__)
            return ::syscall(__NR_gettid);
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
#elif defined(__unix__)
            ::localtime_r(time, t);
#else
#error Unsupported platform
#endif
        }

		inline FILE* fopen(const char* fileName, const char* mode)
		{
#ifdef _WIN32
			return ::_fsopen(fileName, mode, _SH_DENYWR);
#elif defined(__unix__)
			return ::fopen(fileName, mode);
#else
#error Unsupported platform
#endif
		}

#ifdef _WIN32
		inline FILE* wfopen(const wchar_t* fileName, const wchar_t* mode)
		{
			return ::_wfsopen(fileName, mode, _SH_DENYWR);
		}
#endif

        //TODO: add file open func
        //HANDLE h = ::CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, 0, OPEN_ALWAYS, 0, 0);
        //int fd = _open_osfhandle(reinterpret_cast<intptr_t>(h), _O_APPEND);
        //m_file = _fdopen(fd, "a");
    }
}