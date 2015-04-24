#pragma once
#include <sstream>
#include <plog/Util.h>
#include <plog/Severity.h>

namespace plog
{
    class Record
    {
    public:
        Record(Severity severity, const char* func, size_t line, const void* object)
            : m_severity(severity), m_tid(util::gettid()), m_object(object), m_line(line), m_func(func)
        {
            util::ftime(&m_time);
        }

        //////////////////////////////////////////////////////////////////////////
        // Stream output operators

        Record& operator<<(char data)
        {
            char str[] = { data, 0 };
            *this << str;
            return *this;
        }

        Record& operator<<(const char* data)
        {
            data = data ? data : "(null)";

#ifdef _WIN32
            m_message << util::toUnicode(data);
#else            
            m_message << data;
#endif

            return *this;
        }

        Record& operator<<(char* data)
        {
            *this << const_cast<const char*>(data);
            return *this;
        }

#ifndef __ANDROID__
        Record& operator<<(wchar_t data)
        {
            wchar_t str[] = { data, 0 };
            *this << str;
            return *this;
        }

        Record& operator<<(const wchar_t* data)
        {
            data = data ? data : L"(null)";

#ifdef _WIN32
            m_message << data;
#else
            *this << util::toString(data);
#endif

            return *this;
        }

        Record& operator<<(wchar_t* data)
        {
            *this << const_cast<const wchar_t*>(data);
            return *this;
        }
#endif

        template<typename T>
        Record& operator<<(const T& data)
        {
            using namespace plog::detail;

            m_message << data;
            return *this;
        }

        //////////////////////////////////////////////////////////////////////////
        // Getters

        const util::Time& getTime() const
        {
            return m_time;
        }

        Severity getSeverity() const
        {
            return m_severity;
        }

        unsigned int getTid() const
        {
            return m_tid;
        }

        const void* getObject() const
        {
            return m_object;
        }

        size_t getLine() const
        {
            return m_line;
        }

        const util::nstring getMessage() const
        {
            return m_message.str();
        }

        std::string getFunc() const
        {
            return util::processFuncName(m_func);
        }

    private:
        util::Time          m_time;
        const Severity      m_severity;
        const unsigned int  m_tid;
        const void* const   m_object;
        const size_t        m_line;
        util::nstringstream m_message;
        const char* const   m_func;
    };

    namespace detail
    {
        // Allows implicit conversion to std::string
        inline void operator<<(util::nstringstream& stream, const std::string& data)
        {
            std::operator<<(stream, data.c_str());
        }

#ifndef __ANDROID__
        // Allows implicit conversion to std::wstring
        inline void operator<<(util::nstringstream& stream, const std::wstring& data)
        {
            std::operator<<(stream, data.c_str());
        }

        // Allows implicit conversion to const wchar_t* (const char* is handled by std implementation)
        inline void operator<<(util::nstringstream& stream, const wchar_t* data)
        {
            std::operator<<(stream, data);
        }
#endif
    }
}
