#pragma once
#include <cassert>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#   include <time.h>
#   include <sys/timeb.h>
#   include <io.h>
#   include <share.h>
#else
#   include <unistd.h>
#   include <sys/syscall.h>
#   include <sys/time.h>
#   include <pthread.h>
#   ifndef __ANDROID__
#       include <iconv.h>
#   endif
#endif

#ifdef _WIN32
#   define _PLOG_NSTR(x)   L##x
#   define PLOG_NSTR(x)    _PLOG_NSTR(x)
#else
#   define PLOG_NSTR(x)    x
#endif

namespace plog
{
#ifdef _WIN32
    namespace winapi
    {
        typedef unsigned long DWORD;
        typedef unsigned short WORD;
        typedef unsigned int UINT;
        typedef char* LPSTR;
        typedef wchar_t* LPWSTR;
        typedef const char* LPCSTR;
        typedef const wchar_t* LPCWSTR;
        typedef void* HANDLE;
        typedef int BOOL;
        typedef size_t ULONG_PTR;

        const UINT kActiveCodePage = 0;
        const UINT kUTF8CodePage = 65001;

        struct CRITICAL_SECTION
        {
            void* DebugInfo;
            long LockCount;
            long RecursionCount;
            HANDLE OwningThread;
            HANDLE LockSemaphore;
            ULONG_PTR SpinCount;
        };

        struct COORD
        {
            short X;
            short Y;
        };

        struct SMALL_RECT
        {
            short Left;
            short Top;
            short Right;
            short Bottom;
        };

        struct CONSOLE_SCREEN_BUFFER_INFO
        {
            COORD dwSize;
            COORD dwCursorPosition;
            WORD  wAttributes;
            SMALL_RECT srWindow;
            COORD dwMaximumWindowSize;
        };

        const DWORD kStdOutputHandle = static_cast<DWORD>(-11);

        enum
        {
            kForegroundBlue = 0x0001,
            kForegroundGreen = 0x0002,
            kForegroundRed = 0x0004,
            kForegroundIntensity = 0x0008,
            kBackgroundBlue = 0x0010,
            kBackgroundGreen = 0x0020,
            kBackgroundRed = 0x0040,
            kBackgroundIntensity = 0x0080
        };

        extern "C"
        {
            __declspec(dllimport) int __stdcall MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
            __declspec(dllimport) int __stdcall WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, const char* lpDefaultChar, BOOL* lpUsedDefaultChar);
            __declspec(dllimport) DWORD __stdcall GetCurrentThreadId();
            __declspec(dllimport) BOOL __stdcall MoveFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName);
            __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION* lpCriticalSection);
            __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* lpCriticalSection);
            __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* lpCriticalSection);
            __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION* lpCriticalSection);
            __declspec(dllimport) HANDLE __stdcall GetStdHandle(DWORD nStdHandle);
            __declspec(dllimport) BOOL __stdcall GetConsoleScreenBufferInfo(HANDLE hConsoleOutput, CONSOLE_SCREEN_BUFFER_INFO* lpConsoleScreenBufferInfo);
            __declspec(dllimport) BOOL __stdcall SetConsoleTextAttribute(HANDLE hConsoleOutput, WORD wAttributes);
        }
    }
#endif

    namespace util
    {
#ifdef _WIN32
        typedef std::wstring nstring;
        typedef std::wstringstream nstringstream;
        typedef wchar_t nchar;
#else
        typedef std::string nstring;
        typedef std::stringstream nstringstream;
        typedef char nchar;
#endif

        inline void localtime_s(struct tm* t, const time_t* time)
        {
#if defined(_WIN32) && defined(__BORLANDC__)
            ::localtime_s(time, t);
#elif defined(_WIN32) && defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
            *t = *::localtime(time);
#elif defined(_WIN32)
            ::localtime_s(t, time);
#else
            ::localtime_r(time, t);
#endif
        }

#ifdef _WIN32
        typedef timeb Time;

        inline void ftime(Time* t)
        {
            ::ftime(t);
        }
#else
        struct Time
        {
            time_t time;
            unsigned short millitm;
        };

        inline void ftime(Time* t)
        {
            timeval tv;
            ::gettimeofday(&tv, NULL);

            t->time = tv.tv_sec;
            t->millitm = static_cast<unsigned short>(tv.tv_usec / 1000);
        }
#endif

        inline unsigned int gettid()
        {
#ifdef _WIN32
            return winapi::GetCurrentThreadId();
#elif defined(__unix__)
            return static_cast<unsigned int>(::syscall(__NR_gettid));
#elif defined(__APPLE__)
            uint64_t tid64;
            pthread_threadid_np(NULL, &tid64);
            return static_cast<unsigned int>(tid64);
#endif
        }

#if !defined(__ANDROID__) && !defined(_WIN32)
        inline std::string toNarrow(const wchar_t* wstr)
        {
            size_t wlen = ::wcslen(wstr);
            std::string str(wlen * sizeof(wchar_t), 0);

            if (!str.empty())
            {
                const char* in = reinterpret_cast<const char*>(&wstr[0]);
                char* out = &str[0];
                size_t inBytes = wlen * sizeof(wchar_t);
                size_t outBytes = str.size();

                iconv_t cd = ::iconv_open("UTF-8", "WCHAR_T");
                ::iconv(cd, const_cast<char**>(&in), &inBytes, &out, &outBytes);
                ::iconv_close(cd);

                str.resize(str.size() - outBytes);
            }

            return str;
        }
#endif

#ifdef _WIN32
        inline std::wstring toWide(const char* str)
        {
            size_t len = ::strlen(str);
            std::wstring wstr(len, 0);

            if (!wstr.empty())
            {
                int wlen = winapi::MultiByteToWideChar(winapi::kActiveCodePage, 0, str, static_cast<int>(len), &wstr[0], static_cast<int>(wstr.size()));
                wstr.resize(wlen);
            }

            return wstr;
        }

        inline std::string toUTF8(const std::wstring& wstr)
        {
            std::string str(wstr.size() * sizeof(wchar_t), 0);

            if (!str.empty())
            {
                int len = winapi::WideCharToMultiByte(winapi::kUTF8CodePage, 0, wstr.c_str(), static_cast<int>(wstr.size()), &str[0], static_cast<int>(str.size()), 0, 0);
                str.resize(len);
            }

            return str;
        }
#endif

        inline std::string processFuncName(const char* func)
        {
#if (defined(_WIN32) && !defined(__MINGW32__)) || defined(__OBJC__)
            return std::string(func);
#else
            const char* funcBegin = func;
            const char* funcEnd = ::strchr(funcBegin, '(');

            if (!funcEnd)
            {
                return std::string(func);
            }

            for (const char* i = funcEnd - 1; i >= funcBegin; --i) // search backwards for the first space char
            {
                if (*i == ' ')
                {
                    funcBegin = i + 1;
                    break;
                }
            }

            return std::string(funcBegin, funcEnd);
#endif
        }

        inline const nchar* findExtensionDot(const nchar* fileName)
        {
#ifdef _WIN32
            return std::wcsrchr(fileName, L'.');
#else
            return std::strrchr(fileName, '.');
#endif
        }

        inline void splitFileName(const nchar* fileName, nstring& fileNameNoExt, nstring& fileExt)
        {
            const nchar* dot = findExtensionDot(fileName);

            if (dot)
            {
                fileNameNoExt.assign(fileName, dot);
                fileExt.assign(dot + 1);
            }
            else
            {
                fileNameNoExt.assign(fileName);
                fileExt.clear();
            }
        }

        class NonCopyable
        {
        protected:
            NonCopyable()
            {
            }

        private:
            NonCopyable(const NonCopyable&);
            NonCopyable& operator=(const NonCopyable&);
        };

        class File : NonCopyable
        {
        public:
            File() : m_file(-1)
            {
            }

            File(const nchar* fileName) : m_file(-1)
            {
                open(fileName);
            }

            ~File()
            {
                close();
            }

            off_t open(const nchar* fileName)
            {
#if defined(_WIN32) && (defined(__BORLANDC__) || defined(__MINGW32__))
                m_file = ::_wsopen(fileName, _O_CREAT | _O_WRONLY | _O_BINARY, SH_DENYWR, _S_IREAD | _S_IWRITE);
#elif defined(_WIN32)
                ::_wsopen_s(&m_file, fileName, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE);
#else
                m_file = ::open(fileName, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
                return seek(0, SEEK_END);
            }

            int write(const void* buf, size_t count)
            {
#ifdef _WIN32
                return m_file != -1 ? ::_write(m_file, buf, static_cast<unsigned int>(count)) : -1;
#else
                return m_file != -1 ? static_cast<int>(::write(m_file, buf, count)) : -1;
#endif
            }

            template<class CharType>
            int write(const std::basic_string<CharType>& str)
            {
                return write(str.data(), str.size() * sizeof(CharType));
            }

            off_t seek(off_t offset, int whence)
            {
#ifdef _WIN32
                return m_file != -1 ? ::_lseek(m_file, offset, whence) : -1;
#else
                return m_file != -1 ? ::lseek(m_file, offset, whence) : -1;
#endif
            }

            void close()
            {
                if (m_file != -1)
                {
#ifdef _WIN32
                    ::_close(m_file);
#else
                    ::close(m_file);
#endif
                    m_file = -1;
                }
            }

            static int unlink(const nchar* fileName)
            {
#ifdef _WIN32
                return ::_wunlink(fileName);
#else
                return ::unlink(fileName);
#endif
            }

            static int rename(const nchar* oldFilename, const nchar* newFilename)
            {
#ifdef _WIN32
                return winapi::MoveFileW(oldFilename, newFilename);
#else
                return ::rename(oldFilename, newFilename);
#endif
            }

        private:
            int m_file;
        };

        class Mutex : NonCopyable
        {
        public:
            Mutex()
            {
#ifdef _WIN32
                winapi::InitializeCriticalSection(&m_sync);
#else
                ::pthread_mutex_init(&m_sync, 0);
#endif
            }

            ~Mutex()
            {
#ifdef _WIN32
                winapi::DeleteCriticalSection(&m_sync);
#else
                ::pthread_mutex_destroy(&m_sync);
#endif
            }

            friend class MutexLock;

        private:
            void lock()
            {
#ifdef _WIN32
                winapi::EnterCriticalSection(&m_sync);
#else
                ::pthread_mutex_lock(&m_sync);
#endif
            }

            void unlock()
            {
#ifdef _WIN32
                winapi::LeaveCriticalSection(&m_sync);
#else
                ::pthread_mutex_unlock(&m_sync);
#endif
            }

        private:
#ifdef _WIN32
            winapi::CRITICAL_SECTION m_sync;
#else
            pthread_mutex_t m_sync;
#endif
        };

        class MutexLock : NonCopyable
        {
        public:
            MutexLock(Mutex& mutex) : m_mutex(mutex)
            {
                m_mutex.lock();
            }

            ~MutexLock()
            {
                m_mutex.unlock();
            }

        private:
            Mutex& m_mutex;
        };

        template<class T>
        class Singleton : NonCopyable
        {
        public:
            Singleton()
            {
                assert(!m_instance);
                m_instance = static_cast<T*>(this);
            }

            ~Singleton()
            {
                assert(m_instance);
                m_instance = 0;
            }

            static T* getInstance()
            {
                return m_instance;
            }

        private:
            static T* m_instance;
        };

        template<class T>
        T* Singleton<T>::m_instance = NULL;
    }
}
