#pragma once
#include <cassert>
#include <cstring>
#include <cstdio>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#   include <Windows.h>
#else
#   include <unistd.h>
#   include <sys/syscall.h>
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
#else
        typedef std::string nstring;
        typedef std::stringstream nstringstream;
#endif

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

        inline std::string toString(const wchar_t* wstr)
        {
            size_t wlen = ::wcslen(wstr);
            std::string str(wlen * sizeof(wchar_t), 0);

#ifdef _WIN32
            int len = ::WideCharToMultiByte(CP_ACP, 0, wstr, wlen, &str[0], str.size(), 0, 0);
#elif defined(__ANDROID__)
            return std::string("<<not supported>>");
#else
            const char* in = reinterpret_cast<const char*>(&wstr[0]);
            char* out = &str[0];
            size_t inBytes = wlen * sizeof(wchar_t);
            size_t outBytes = str.size();

            iconv_t cd = ::iconv_open("UTF-8", "WCHAR_T");
            ::iconv(cd, const_cast<char**>(&in), &inBytes, &out, &outBytes);
            ::iconv_close(cd); 
            size_t len = str.size() - outBytes;
#endif
            str.resize(len);
            return str;
        }

#ifdef _WIN32
        inline std::wstring toUnicode(const char* str)
        {
            size_t len = ::strlen(str);
            std::wstring wstr(len, 0);

            int wlen = ::MultiByteToWideChar(CP_ACP, 0, str, len, &wstr[0], wstr.size());

            wstr.resize(wlen);
            return wstr;
        }
#endif

        inline std::string toUTF8(const std::wstring& wstr)
        {
#ifdef _WIN32
            std::string str(wstr.size() * sizeof(wchar_t), 0);

            if (!str.empty())
            {
                int len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), &str[0], str.size(), 0, 0);
                str.resize(len);
            }

            return str;
#else
            return toString(wstr.c_str());
#endif
        }

        inline void splitFileName(const char* fileName, std::string& fileNameNoExt, std::string& fileExt)
        {
            const char* dot = std::strrchr(fileName, '.');

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
            File() : m_file()
            {
            }

            File(const char* fileName) : m_file()
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
                m_file = ::_fsopen(fileName, "ab", _SH_DENYWR);
#else
                m_file = std::fopen(fileName, "a");
#endif
                if (0 == getSize())
                {
                    const unsigned char kBOM[] = { 0xEF, 0xBB, 0xBF };
                    write(kBOM, sizeof(kBOM));
                }
            }

            size_t write(const void* buf, size_t count)
            {
                size_t written = std::fwrite(buf, 1, count, m_file);
                std::fflush(m_file);

                return written;
            }

            int writeAsUTF8(const util::nstring& str)
            {
#ifdef _WIN32
                std::string utf8str = toUTF8(str);
#else
                const std::string& utf8str = str;
#endif
                return write(utf8str.c_str(), utf8str.size());
            }

            off_t getSize()
            {
                return std::ftell(m_file);
            }

            void close()
            {
                if (m_file)
                {
                    std::fclose(m_file);
                    m_file = NULL;
                }
            }

            static void unlink(const char* fileName)
            {
#ifdef _WIN32
                ::_unlink(fileName);
#else
                ::unlink(fileName);
#endif
            }

            static void rename(const char* oldFilename, const char* newFilename)
            {
                ::rename(oldFilename, newFilename);
            }

        private:
            FILE* m_file;
        };

        class Mutex : NonCopyable
        {
        public:
            Mutex()
            {
#ifdef _WIN32
                ::InitializeCriticalSection(&m_sync);
#else
                ::pthread_mutex_init(&m_sync, 0);
#endif
            }

            ~Mutex()
            {
#ifdef _WIN32
                ::DeleteCriticalSection(&m_sync);
#else
                ::pthread_mutex_destroy(&m_sync);
#endif
            }

            friend class MutexLock;

        private:
            void lock()
            {
#ifdef _WIN32
                ::EnterCriticalSection(&m_sync);
#else
                ::pthread_mutex_lock(&m_sync);
#endif
            }

            void unlock()
            {
#ifdef _WIN32
                ::LeaveCriticalSection(&m_sync);
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

            static T& getInstance()
            {
                assert(m_instance);
                return *m_instance;
            }

        private:
            static T* m_instance;
        };

        template<class T> 
        T* Singleton<T>::m_instance;
    }
}