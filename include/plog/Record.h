#pragma once
#include <plog/Severity.h>
#include <plog/Util.h>

#ifdef __cplusplus_cli
#include <vcclr.h>  // For PtrToStringChars
#endif

namespace plog
{
    namespace detail
    {
        //////////////////////////////////////////////////////////////////////////
        // Stream output operators as free functions

        inline void operator<<(util::nostringstream& stream, const char* data)
        {
            data = data ? data : "(null)";

#if defined(_WIN32) && defined(__BORLANDC__) && PLOG_ENABLE_WCHAR_INPUT
            stream << util::toWide(data);
#elif defined(_WIN32) && PLOG_ENABLE_WCHAR_INPUT
            std::operator<<(stream, util::toWide(data));
#else
            std::operator<<(stream, data);
#endif
        }

        inline void operator<<(util::nostringstream& stream, const std::string& data)
        {
            plog::detail::operator<<(stream, data.c_str());
        }

#if PLOG_ENABLE_WCHAR_INPUT
        inline void operator<<(util::nostringstream& stream, const wchar_t* data)
        {
            data = data ? data : L"(null)";

#   ifdef _WIN32
            std::operator<<(stream, data);
#   else
            std::operator<<(stream, util::toNarrow(data));
#   endif
        }

        inline void operator<<(util::nostringstream& stream, const std::wstring& data)
        {
            plog::detail::operator<<(stream, data.c_str());
        }
#endif
    }

    class Record
    {
    public:
        Record(Severity severity, const char* func, size_t line, const char* file, const void* object)
            : m_severity(severity), m_tid(util::gettid()), m_object(object), m_line(line), m_func(func), m_file(file)
        {
            util::ftime(&m_time);
        }

        //////////////////////////////////////////////////////////////////////////
        // Stream output operators

        Record& operator<<(char data)
        {
            char str[] = { data, 0 };
            return *this << str;
        }

#if PLOG_ENABLE_WCHAR_INPUT
        Record& operator<<(wchar_t data)
        {
            wchar_t str[] = { data, 0 };
            return *this << str;
        }
#endif

#if defined(_WIN32) && PLOG_ENABLE_WCHAR_INPUT
        Record& operator<<(std::wostream& (*data)(std::wostream&))
#else
        Record& operator<<(std::ostream& (*data)(std::ostream&))
#endif
        {
            m_message << data;
            return *this;
        }

#ifdef QT_VERSION
        Record& operator<<(const QString& data)
        {
#if defined(_WIN32) && PLOG_ENABLE_WCHAR_INPUT
            return *this << data.toStdWString();
#else
            return *this << data.toStdString();
#endif
        }
#endif

#ifdef __cplusplus_cli
        Record& operator<<(System::String^ data)
        {
            cli::pin_ptr<const System::Char> ptr = PtrToStringChars(data);
            return *this << static_cast<const wchar_t*>(ptr);
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

        virtual const util::Time& getTime() const
        {
            return m_time;
        }

        virtual Severity getSeverity() const
        {
            return m_severity;
        }

        virtual unsigned int getTid() const
        {
            return m_tid;
        }

        virtual const void* getObject() const
        {
            return m_object;
        }

        virtual size_t getLine() const
        {
            return m_line;
        }

        virtual const util::nchar* getMessage() const
        {
            m_messageStr = m_message.str();
            return m_messageStr.c_str();
        }

        virtual const char* getFunc() const
        {
            m_funcStr = util::processFuncName(m_func);
            return m_funcStr.c_str();
        }

        virtual const char* getFile() const
        {
            return m_file;
        }

    private:
        util::Time              m_time;
        const Severity          m_severity;
        const unsigned int      m_tid;
        const void* const       m_object;
        const size_t            m_line;
        util::nostringstream    m_message;
        const char* const       m_func;
        const char* const       m_file;
        mutable std::string     m_funcStr;
        mutable util::nstring   m_messageStr;
    };
}
