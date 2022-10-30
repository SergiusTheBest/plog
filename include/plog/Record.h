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
        namespace meta
        {
            template<class T>
            inline T& declval()
            {
#ifdef __INTEL_COMPILER
#    pragma warning(suppress: 327) // NULL reference is not allowed
#endif
                return *reinterpret_cast<T*>(0);
            }

            template<bool B, class T = void>
            struct enableIf {};

            template<class T>
            struct enableIf<true, T> { typedef T type; };

            struct No  { char a[1]; };
            struct Yes { char a[2]; };

            template <class From, class To>
            struct isConvertible
            {
                // `+ sizeof(U*)` is required for GCC 4.5-4.7
                template<class U>
                static typename enableIf<!!(sizeof(static_cast<To>(meta::declval<U>())) + sizeof(U*)), Yes>::type test(int);

                template<class U>
                static No test(...);

                enum { value = sizeof(test<From>(0)) == sizeof(Yes) };
            };

            template <class T>
            struct isConvertibleToNString : isConvertible<T, util::nstring> {};

            template <class T>
            struct isConvertibleToString : isConvertible<T, std::string> {};

            template <class T>
            struct isContainer
            {
                template<class U>
                static typename meta::enableIf<!!(sizeof(
#if defined(_MSC_VER) && _MSC_VER < 1700 // MSVC 2010 doesn't understand `typename T::const_iterator`
                    meta::declval<U>().begin()) + sizeof(meta::declval<U>().end()
#else
                    typename U::const_iterator
#endif
                    )), Yes>::type test(int);

                template<class U>
                static No test(...);

                enum { value = sizeof(test<T>(0)) == sizeof(Yes) };
            };
        }

        //////////////////////////////////////////////////////////////////////////
        // Stream output operators as free functions

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

        inline void operator<<(util::nostringstream& stream, wchar_t* data)
        {
            plog::detail::operator<<(stream, const_cast<const wchar_t*>(data));
        }

        inline void operator<<(util::nostringstream& stream, const std::wstring& data)
        {
            plog::detail::operator<<(stream, data.c_str());
        }
#endif

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

        inline void operator<<(util::nostringstream& stream, char* data)
        {
            plog::detail::operator<<(stream, const_cast<const char*>(data));
        }

        inline void operator<<(util::nostringstream& stream, const std::string& data)
        {
            plog::detail::operator<<(stream, data.c_str());
        }

        // Print `std::pair`
        template<class T1, class T2>
        inline void operator<<(util::nostringstream& stream, const std::pair<T1, T2>& data)
        {
            stream << data.first;
            stream << ":";
            stream << data.second;
        }

#if defined(__clang__) || !defined(__GNUC__) || __GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ > 4 // skip for GCC < 4.5 due to https://gcc.gnu.org/bugzilla/show_bug.cgi?id=38600
        // Print data that can be casted to `std::basic_string`.
        template<class T>
        inline typename meta::enableIf<meta::isConvertibleToNString<T>::value, void>::type operator<<(util::nostringstream& stream, const T& data)
        {
            plog::detail::operator<<(stream, static_cast<util::nstring>(data));
        }

        // Print std containers
        template<class T>
        inline typename meta::enableIf<meta::isContainer<T>::value && !meta::isConvertibleToNString<T>::value && !meta::isConvertibleToString<T>::value, void>::type
            operator<<(util::nostringstream& stream, const T& data)
        {
            stream << "[";

            for (typename T::const_iterator it = data.begin(); it != data.end();)
            {
                stream << *it;

                if (++it == data.end())
                {
                    break;
                }

                stream << ", ";
            }

            stream << "]";
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
            inline No operator<<(Stream&, const T&);

            template <class T, class Stream>
            struct isStreamable
            {
                enum { value = sizeof(operator<<(meta::declval<Stream>(), meta::declval<const T>())) != sizeof(No) };
            };

            template <class Stream>
            struct isStreamable<std::ios_base& PLOG_CDECL (std::ios_base&), Stream>
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
        Record& operator<<(std::wostream& (PLOG_CDECL *data)(std::wostream&))
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

#   if QT_VERSION < 0x060000
        Record& operator<<(const QStringRef& data)
        {
            return *this << data.toString();
        }
#   endif

#   ifdef QSTRINGVIEW_H
        Record& operator<<(QStringView data)
        {
            return *this << data.toString();
        }
#   endif
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
