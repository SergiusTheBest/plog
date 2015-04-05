#include <plog/Log.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <list>

namespace plog
{
    template<class Formatter>
    class MyAppender : public IAppender
    {
    public:
        virtual void write(const Record& record)
        {
            util::nstring str = Formatter::format(record);

            m_messageList.push_back(str);
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
    static plog::MyAppender<plog::FuncMessageFormatter> myAppender;
    plog::init(plog::debug, &myAppender);

    LOGD << "A debug message!";

    myAppender.getMessageList();

    return 0;
}
