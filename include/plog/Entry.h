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

        Entry& operator<<(const char* data)
        {
            m_stream << (data ? data : "(null)");
            return *this;
        }

        Entry& operator<<(char* data)
        {
            *this << const_cast<const char*>(data);
            return *this;
        }

        Entry& operator<<(const wchar_t* data)
        {
            m_stream << (data ? data : L"(null)");
            return *this;
        }

        Entry& operator<<(wchar_t* data)
        {
            *this << const_cast<const wchar_t*>(data);
            return *this;
        }

        Entry& operator<<(const std::string& data)
        {
            m_stream << data.c_str();
            return *this;
        }

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
        std::wstringstream  m_stream;
    };
}