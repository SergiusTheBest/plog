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
            : m_severity(severity), m_object(object), m_func(func), m_line(line), m_tid(util::gettid())
        {
            ftime(&m_time);
            m_stream.imbue(std::locale(""));
        }

        Entry& operator<<(const char* data)
        {
            data = data ? data : "(null)";
            m_stream << data;

            return *this;
        }

        Entry& operator<<(const wchar_t* data)
        {
            data = data ? data : L"(null)";

            for (; *data; ++data)
            {
                m_stream << std::use_facet<std::ctype<wchar_t>>(m_stream.getloc()).narrow(*data, '?');
            }

            return *this;
        }

        Entry& operator<<(const std::wstring& data)
        {
            return *this << data.c_str();
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
        std::stringstream   m_stream;
        std::locale         m_locale;
    };
}