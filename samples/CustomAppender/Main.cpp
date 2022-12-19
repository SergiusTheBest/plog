//
// CustomAppender - shows how to implement a custom appender that stores log messages in memory.
//

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <list>

namespace plog
{
    template<class Formatter> // Typically a formatter is passed as a template parameter.
    class MyAppender : public IAppender // All appenders MUST inherit IAppender interface.
    {
    public:
        virtual void write(const Record& record) PLOG_OVERRIDE // This is a method from IAppender that MUST be implemented.
        {
            util::nstring str = Formatter::format(record); // Use the formatter to get a string from a record.

            m_messageList.push_back(str); // Store a log message in a list.
        }

        std::list<util::nstring>& getMessageList()
        {
            return m_messageList;
        }

    private:
        std::list<util::nstring> m_messageList;
    };
}

int main()
{
    static plog::MyAppender<plog::FuncMessageFormatter> myAppender; // Create our custom appender.
    plog::init(plog::debug, &myAppender); // Initialize the logger with our appender.

    PLOGD << "A debug message!";

    myAppender.getMessageList(); // This returns a list of stored log messages.

    return 0;
}
