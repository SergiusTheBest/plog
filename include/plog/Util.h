#pragma once
#include <cassert>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#   include <plog/WinApi.h>
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
            return GetCurrentThreadId();
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
                int wlen = MultiByteToWideChar(codePage::kActive, 0, str, static_cast<int>(len), &wstr[0], static_cast<int>(wstr.size()));
                wstr.resize(wlen);
            }

            return wstr;
        }

        inline std::string toUTF8(const std::wstring& wstr)
        {
            std::string str(wstr.size() * sizeof(wchar_t), 0);

            if (!str.empty())
            {
                int len = WideCharToMultiByte(codePage::kUTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &str[0], static_cast<int>(str.size()), 0, 0);
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
            File(bool binary = false) : m_file(NULL), m_binary(binary)
            {
            }

            File(const nchar* fileName, bool binary = false) : m_file(NULL), m_binary(binary)
            {
                open(fileName);
            }

            ~File()
            {
                close();
            }

            off_t open(const nchar* fileName)
            {
#if defined (_WIN32)
				const nchar* mode = m_binary ? L"ab" : L"a";
#else
				const nchar* mode = m_binary ? "ab" : "a";
#endif
#if defined(_WIN32) && (defined(__BORLANDC__) || defined(__MINGW32__))
                m_file = fopen(fileName, mode);
#elif defined(_WIN32)
                m_file = _wfsopen(fileName, mode, _SH_DENYNO);
#else
                m_file = fopen(fileName, mode);
#endif
                return seek(0, SEEK_END);
            }

            int write(const void* buf, size_t count)
            {
                if (m_file != NULL)
                {
                    size_t bytes_written = fwrite(buf, 1, static_cast<unsigned int>(count), m_file);
                    fflush(m_file);
                    return static_cast<int>(bytes_written);
                }
                else
                {
                    return -1;
                }
            }

            template<class CharType>
            int write(const std::basic_string<CharType>& str)
            {
                return write(str.data(), str.size() * sizeof(CharType));
            }

            off_t seek(off_t offset, int whence)
            {
                return m_file != NULL ? fseek(m_file, offset, whence) : -1;
            }

            void close()
            {
                if (m_file != NULL)
                {
                    fclose(m_file);
                    m_file = NULL;
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
            FILE* m_file;
            bool m_binary;
        };

        class Mutex : NonCopyable
        {
        public:
            Mutex()
            {
#ifdef _WIN32
                InitializeCriticalSection(&m_sync);
#else
                ::pthread_mutex_init(&m_sync, 0);
#endif
            }

            ~Mutex()
            {
#ifdef _WIN32
                DeleteCriticalSection(&m_sync);
#else
                ::pthread_mutex_destroy(&m_sync);
#endif
            }

            friend class MutexLock;

        private:
            void lock()
            {
#ifdef _WIN32
                EnterCriticalSection(&m_sync);
#else
                ::pthread_mutex_lock(&m_sync);
#endif
            }

            void unlock()
            {
#ifdef _WIN32
                LeaveCriticalSection(&m_sync);
#else
                ::pthread_mutex_unlock(&m_sync);
#endif
            }

        private:
#ifdef _WIN32
            CRITICAL_SECTION m_sync;
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
