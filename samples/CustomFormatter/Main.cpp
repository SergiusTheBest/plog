#include <plog/Log.h>

namespace plog
{
    class MyFormatter
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            util::nstringstream ss;
            ss << record.getMessage().c_str() << "\n";

            return ss.str();
        }
    };
}

int main()
{
    plog::init<plog::MyFormatter>(plog::debug, "CustomFormatter.txt");

    LOGD << "A debug message!";

    return 0;
}
