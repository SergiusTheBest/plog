//
// CustomFormatter - shows how to implement a custom formatter.
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

namespace plog
{
    class MyFormatter
    {
    public:
        static util::nstring header() // This method returns a header for a new file. In our case it is empty.
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record) // This method returns a string from a record.
        {
            util::nostringstream ss;
            ss << record.getMessage() << "\n"; // Produce a simple string with a log message.

            return ss.str();
        }
    };
}

int main()
{
    plog::init<plog::MyFormatter>(plog::debug, "CustomFormatter.txt"); // Initialize the logger and pass our formatter as a template parameter to init function.

    PLOGD << "A debug message!";

    return 0;
}
