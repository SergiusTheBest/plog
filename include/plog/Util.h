#pragma once
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>

#ifndef PLOG_ENABLE_WCHAR_INPUT
#   ifdef _WIN32
#       define PLOG_ENABLE_WCHAR_INPUT 1
#   else
#       define PLOG_ENABLE_WCHAR_INPUT 0
#   endif
#endif

#ifdef _WIN32
#   if defined(PLOG_EXPORT)
#       define PLOG_LINKAGE __declspec(dllexport)
#   elif defined(PLOG_IMPORT)
#       define PLOG_LINKAGE __declspec(dllimport)
#   endif
#   if defined(PLOG_GLOBAL)
#       error "PLOG_GLOBAL isn't supported on Windows"
#   endif
#else
#   if defined(PLOG_GLOBAL)
#       define PLOG_LINKAGE __attribute__ ((visibility ("default")))
#   elif defined(PLOG_LOCAL)
#       define PLOG_LINKAGE __attribute__ ((visibility ("hidden")))
#   endif
#   if defined(PLOG_EXPORT) || defined(PLOG_IMPORT)
#       error "PLOG_EXPORT/PLOG_IMPORT is supported only on Windows"
#   endif
#endif

#ifndef PLOG_LINKAGE
#   define PLOG_LINKAGE
#endif

#ifdef _WIN32
#   include <plog/WinApi.h>
#   include <time.h>
#   include <sys/timeb.h>
#   include <io.h>
#   include <share.h>
#else
#   include <unistd.h>
#   include <sys/time.h>
#   if defined(__linux__) || defined(__FreeBSD__)
#       include <sys/syscall.h>
#   elif defined(__rtems__)
#       include <rtems.h>
#   endif
#   if defined(_POSIX_THREADS)
#       include <pthread.h>
#   endif
#   if PLOG_ENABLE_WCHAR_INPUT
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
    namespace util
    {
#ifdef _WIN32
        typedef std::wstring nstring;
        typedef std::wostringstream nostringstream;
        typedef std::wistringstream nistringstream;
        typedef wchar_t nchar;
#else
        typedef std::string nstring;
        typedef std::ostringstream nostringstream;
        typedef std::istringstream nistringstream;
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

        inline void gmtime_s(struct tm* t, const time_t* time)
        {
#if defined(_WIN32) && defined(__BORLANDC__)
            ::gmtime_s(time, t);
#elif defined(_WIN32) && defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
            *t = *::gmtime(time);
#elif defined(_WIN32)
            ::gmtime_s(t, time);
#else
            ::gmtime_r(time, t);
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
            return GetCurrentThreadId();
#elif defined(__linux__)
            return static_cast<unsigned int>(::syscall(__NR_gettid));
#elif defined(__FreeBSD__)
            long tid;
            syscall(SYS_thr_self, &tid);
            return static_cast<unsigned int>(tid);
#elif defined(__rtems__)
            return rtems_task_self();
#elif defined(__APPLE__)
            uint64_t tid64;
            pthread_threadid_np(NULL, &tid64);
            return static_cast<unsigned int>(tid64);
#else
            return 0;
#endif
        }

#ifdef _WIN32
    inline int vasprintf(char** strp, const char* format, va_list ap)
    {
        int charCount = _vscprintf(format, ap);
        if (charCount < 0)
        {
            return -1;
        }

        size_t bufferCharCount = static_cast<size_t>(charCount) + 1;

        char* str = static_cast<char*>(malloc(bufferCharCount));
        if (!str)
        {
            return -1;
        }

#if defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
        int retval = _vsnprintf(str, bufferCharCount, format, ap);
#else
        int retval = _vsnprintf_s(str, bufferCharCount, charCount, format, ap);
#endif
        if (retval < 0)
        {
            free(str);
            return -1;
        }

        *strp = str;
        return retval;
    }

    inline int vaswprintf(wchar_t** strp, const wchar_t* format, va_list ap)
    {
        int charCount = _vscwprintf(format, ap);
        if (charCount < 0)
        {
            return -1;
        }

        size_t bufferCharCount = static_cast<size_t>(charCount) + 1;

        wchar_t* str = static_cast<wchar_t*>(malloc(bufferCharCount * sizeof(wchar_t)));
        if (!str)
        {
            return -1;
        }

#if defined(__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
        int retval = _vsnwprintf(str, bufferCharCount, format, ap);
#else
        int retval = _vsnwprintf_s(str, bufferCharCount, charCount, format, ap);
#endif
        if (retval < 0)
        {
            free(str);
            return -1;
        }

        *strp = str;
        return retval;
    }
#endif

#if PLOG_ENABLE_WCHAR_INPUT && !defined(_WIN32)
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
                int wlen = MultiByteToWideChar(codePage::kActive, 0, str, static_cast<int>(len), &wstr[0], static_cast<int>(wstr.size()));
                wstr.resize(wlen);
            }

            return wstr;
        }

        inline std::string toNarrow(const std::wstring& wstr, long page)
        {
			int len = WideCharToMultiByte(page, 0, wstr.c_str(), static_cast<int>(wstr.size()), 0, 0, 0, 0);
            std::string str(len, 0);

            if (!str.empty())
            {
                WideCharToMultiByte(page, 0, wstr.c_str(), static_cast<int>(wstr.size()), &str[0], len, 0, 0);              
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

        class PLOG_LINKAGE NonCopyable
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

            size_t open(const nchar* fileName)
            {
#if defined(_WIN32) && (defined(__BORLANDC__) || defined(__MINGW32__))
                m_file = ::_wsopen(fileName, _O_CREAT | _O_WRONLY | _O_BINARY | _O_NOINHERIT, SH_DENYWR, _S_IREAD | _S_IWRITE);
#elif defined(_WIN32)
                ::_wsopen_s(&m_file, fileName, _O_CREAT | _O_WRONLY | _O_BINARY | _O_NOINHERIT, _SH_DENYWR, _S_IREAD | _S_IWRITE);
#elif defined(O_CLOEXEC)
                m_file = ::open(fileName, O_CREAT | O_WRONLY | O_CLOEXEC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#else
                m_file = ::open(fileName, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
                return seek(0, SEEK_END);
            }

            size_t write(const void* buf, size_t count)
            {
                return m_file != -1 ? static_cast<size_t>(
#ifdef _WIN32
                    ::_write(m_file, buf, static_cast<unsigned int>(count))
#else
                    ::write(m_file, buf, count)
#endif
                    ) : static_cast<size_t>(-1);
            }

            template<class CharType>
            size_t write(const std::basic_string<CharType>& str)
            {
                return write(str.data(), str.size() * sizeof(CharType));
            }

            size_t seek(size_t offset, int whence)
            {
                return m_file != -1 ? static_cast<size_t>(
#if defined(_WIN32) && (defined(__BORLANDC__) || defined(__MINGW32__))
                    ::_lseek(m_file, static_cast<off_t>(offset), whence)
#elif defined(_WIN32)
                    ::_lseeki64(m_file, static_cast<off_t>(offset), whence)
#else
                    ::lseek(m_file, static_cast<off_t>(offset), whence)
#endif
                    ) : static_cast<size_t>(-1);
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
                return MoveFileW(oldFilename, newFilename);
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
                InitializeCriticalSection(&m_sync);
#elif defined(__rtems__)
                rtems_semaphore_create(0, 1,
                            RTEMS_PRIORITY |
                            RTEMS_BINARY_SEMAPHORE |
                            RTEMS_INHERIT_PRIORITY, 1, &m_sync);
#elif defined(_POSIX_THREADS)
                ::pthread_mutex_init(&m_sync, 0);
#endif
            }

            ~Mutex()
            {
#ifdef _WIN32
                DeleteCriticalSection(&m_sync);
#elif defined(__rtems__)
                rtems_semaphore_delete(m_sync);
#elif defined(_POSIX_THREADS)
                ::pthread_mutex_destroy(&m_sync);
#endif
            }

            friend class MutexLock;

        private:
            void lock()
            {
#ifdef _WIN32
                EnterCriticalSection(&m_sync);
#elif defined(__rtems__)
                rtems_semaphore_obtain(m_sync, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
#elif defined(_POSIX_THREADS)
                ::pthread_mutex_lock(&m_sync);
#endif
            }

            void unlock()
            {
#ifdef _WIN32
                LeaveCriticalSection(&m_sync);
#elif defined(__rtems__)
                rtems_semaphore_release(m_sync);
#elif defined(_POSIX_THREADS)
                ::pthread_mutex_unlock(&m_sync);
#endif
            }

        private:
#ifdef _WIN32
            CRITICAL_SECTION m_sync;
#elif defined(__rtems__)
            rtems_id m_sync;
#elif defined(_POSIX_THREADS)
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
#ifdef _WIN32
        class Singleton : NonCopyable
#else
        class PLOG_LINKAGE Singleton : NonCopyable
#endif
        {
        public:
#if defined(__clang__) || __GNUC__ >= 8
            // This constructor is called before the `T` object is fully constructed, and
            // pointers are not dereferenced anyway, so UBSan shouldn't check vptrs.
            __attribute__((no_sanitize("vptr")))
#endif
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
