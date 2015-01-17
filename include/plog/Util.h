#pragma once
#include <cassert>
#include <cstring>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#include <Windows.h>
#else
#include <stdio.h>
#include <sys/io.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <iconv.h>
#endif

#ifdef _WIN32
#define _PLOG_NSTR(x)   L##x
#define PLOG_NSTR(x)    _PLOG_NSTR(x)
#else
#define PLOG_NSTR(x)    x
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

            int len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), &str[0], str.size(), 0, 0);

            str.resize(len);
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
                m_fd = ::open(fileName, O_CREAT | O_APPEND | O_WRONLY, S_IREAD | S_IWRITE);
#endif
                if (0 == getSize())
                {
                    const unsigned char kBOM[] = { 0xEF, 0xBB, 0xBF };
                    write(kBOM, sizeof(kBOM));
                }
            }

            int write(const void* buf, size_t count)
            {
#ifdef _WIN32
                return ::_write(m_fd, buf, count);
#else
                return ::write(m_fd, buf, count);
#endif
            }

            int writeAsUTF8(const util::nstring& str)
            {
#ifdef _WIN32
                std::string utf8str = toUTF8(str);
                return write(utf8str.c_str(), utf8str.size());
#else
                return write(str.c_str(), str.size());
#endif
            }

            off_t getSize()
            {
                struct stat st = {};
                fstat(m_fd, &st);
                
                return st.st_size;
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
            int m_fd;
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