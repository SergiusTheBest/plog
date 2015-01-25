#pragma once
#include <sstream>
#include <sys/timeb.h>
#include <plog/Util.h>
#include <plog/Level.h>

namespace plog
{
    class Entry
    {
    public:
        Entry(Level severity, const char* func, size_t line, const void* object) 
            : m_severity(severity), m_tid(util::gettid()), m_object(object), m_func(func), m_line(line)
        {
            ftime(&m_time);
        }

        Entry& operator<<(char data)
        {
            char str[] = { data, 0 };
            *this << str;
            return *this;
        }

        Entry& operator<<(const char* data)
        {
            data = data ? data : "(null)";

#ifdef _WIN32
            m_stream << util::toUnicode(data);
#else            
            m_stream << data;
#endif

            return *this;
        }

        Entry& operator<<(char* data)
        {
            *this << const_cast<const char*>(data);
            return *this;
        }

        Entry& operator<<(const std::string& data)
        {
            *this << data.c_str();
            return *this;
        }

#ifndef __ANDROID__
        Entry& operator<<(wchar_t data)
        {
            wchar_t str[] = { data, 0 };
            *this << str;
            return *this;
        }

        Entry& operator<<(const wchar_t* data)
        {
            data = data ? data : L"(null)";

#ifdef _WIN32
            m_stream << data;
#else
            *this << util::toString(data);
#endif

            return *this;
        }

        Entry& operator<<(wchar_t* data)
        {
            *this << const_cast<const wchar_t*>(data);
            return *this;
        }

        Entry& operator<<(const std::wstring& data)
        {
            *this << data.c_str();
            return *this;
        }
#endif

        template<typename T>
        Entry& operator<<(const T& data)
        {
            m_stream << data;
            return *this;
        }

    public:
        timeb               m_time;
        Level               m_severity;
        unsigned int        m_tid;
        const void*         m_object;
        const char*         m_func;
        size_t              m_line;
        util::nstringstream m_stream;
    };
}