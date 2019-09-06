#pragma once
#include <cstdarg>
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

#if defined(_WIN32) && defined(__BORLANDC__)
            stream << util::toWide(data);
#elif defined(_WIN32)
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

#ifdef __cplusplus_cli
        inline void operator<<(util::nostringstream& stream, System::String^ data)
        {
            cli::pin_ptr<const System::Char> ptr = PtrToStringChars(data);
            plog::detail::operator<<(stream, static_cast<const wchar_t*>(ptr));
        }
#endif

#ifdef _WIN32
        namespace meta
        {
            template<class T, class Stream>
            inline char operator<<(Stream&, const T&);

            template <class T, class Stream>
            struct isStreamable
            {
#ifdef __INTEL_COMPILER
#    pragma warning(suppress: 327) // NULL reference is not allowed
#endif
                enum { value = sizeof(operator<<(*reinterpret_cast<Stream*>(0), *reinterpret_cast<const T*>(0))) != sizeof(char) };
            };

            template <class Stream>
            struct isStreamable<std::ios_base& (std::ios_base&), Stream>
            {
                enum { value = true };
            };

            template <class Stream, size_t N>
            struct isStreamable<wchar_t[N], Stream>
            {
                enum { value = false };
            };

            template <class Stream, size_t N>
            struct isStreamable<const wchar_t[N], Stream>
            {
                enum { value = false };
            };

            template<bool B, class T = void>
            struct enableIf {};

            template<class T>
            struct enableIf<true, T> { typedef T type; };
        }

        template<class T>
        inline typename meta::enableIf<meta::isStreamable<T, std::ostream>::value && !meta::isStreamable<T, std::wostream>::value, void>::type operator<<(std::wostringstream& stream, const T& data)
        {
            std::ostringstream ss;
            ss << data;
            stream << ss.str();
        }
#endif
    }

    class Record
    {
    public:
        Record(Severity severity, const char* func, size_t line, const char* file, const void* object, int instanceId)
            : m_severity(severity), m_tid(util::gettid()), m_object(object), m_line(line), m_func(func), m_file(file), m_instanceId(instanceId)
        {
            util::ftime(&m_time);
        }

        Record& ref()
        {
            return *this;
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

#ifdef _WIN32
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
#   ifdef _WIN32
            return *this << data.toStdWString();
#   else
            return *this << data.toStdString();
#   endif
        }

        Record& operator<<(const QStringRef& data)
        {
            QString qstr;
            return *this << qstr.append(data);
        }
#endif

        template<typename T>
        Record& operator<<(const T& data)
        {
            using namespace plog::detail;

            m_message << data;
            return *this;
        }

#ifndef __cplusplus_cli
        Record& printf(const char* format, ...)
        {
            using namespace util;

            char* str = NULL;
            va_list ap;

            va_start(ap, format);
            int len = vasprintf(&str, format, ap);
            static_cast<void>(len);
            va_end(ap);

            *this << str;
            free(str);

            return *this;
        }

#ifdef _WIN32
        Record& printf(const wchar_t* format, ...)
        {
            using namespace util;

            wchar_t* str = NULL;
            va_list ap;

            va_start(ap, format);
            int len = vaswprintf(&str, format, ap);
            static_cast<void>(len);
            va_end(ap);

            *this << str;
            free(str);

            return *this;
        }
#endif
#endif //__cplusplus_cli

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

        virtual ~Record() // virtual destructor to satisfy -Wnon-virtual-dtor warning
        {
        }

        virtual int getInstanceId() const
        {
            return m_instanceId;
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
        const int               m_instanceId;
        mutable std::string     m_funcStr;
        mutable util::nstring   m_messageStr;
    };
}
