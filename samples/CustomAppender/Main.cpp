//
// CustomAppender - this sample shows how to implement a custom appender that stores log messages in a memory.
//

#include <plog/Log.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <list>

namespace plog
{
    template<class Formatter> // Typically a formatter is passed as a template parameter.
    class MyAppender : public IAppender // All appenders MUST inherit IAppender interface.
    {
    public:
        virtual void write(const Record& record) // This is a method that an appender MUST implement.
        {
            util::nstring str = Formatter::format(record); // Use the formatter to get a string from a record.

            m_messageList.push_back(str); // Store a log message.
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
    plog::init(plog::debug, &myAppender); // Initialize plog with our appender.

    LOGD << "A debug message!";

    myAppender.getMessageList(); // This returns a list of stored log messages. 

    return 0;
}
