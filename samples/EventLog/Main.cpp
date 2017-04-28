//
// EventLog - shows how to use EventLogAppender to write to the windows event log.
//

#include <plog/Log.h>
#include <plog/Appenders/EventLogAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <iostream>

using namespace std;

int main(int argc, const char* argv[])
{
    const wchar_t kEventSourceName[] = L"EventLogSample";

    if (argc == 2)
    {
        //
        // Note: register/unregister operations require admin rights.
        //

        if (0 == strcmp(argv[1], "--register"))
        {
            if (!plog::EventLogAppenderRegistry::add(kEventSourceName))
            {
                cerr << "Failed to register eventlog source." << endl;
                return -1;
            }

            cout << "Successfully registered eventlog source." << endl;
        }
        else if (0 == strcmp(argv[1], "--unregister"))
        {
            plog::EventLogAppenderRegistry::remove(kEventSourceName);
            cout << "Successfully unregistered eventlog source." << endl;
        }
        else if (0 == strcmp(argv[1], "--query"))
        {
            cout << "Eventlog source exists: " << plog::EventLogAppenderRegistry::exists(kEventSourceName) << endl;
        }

        return 0;
    }

    //
    // Note: eventlog source must be registered prior to creating its appender.
    //

    static plog::EventLogAppender<plog::MessageOnlyFormatter> eventLogAppender(kEventSourceName);
    plog::init(plog::verbose, &eventLogAppender);

    LOG_VERBOSE << "This is a VERBOSE message";
    LOG_DEBUG << "This is a DEBUG message";
    LOG_INFO << "This is an INFO message";
    LOG_WARNING << "This is a WARNING message";
    LOG_ERROR << "This is an ERROR message";
    LOG_FATAL << "This is a FATAL message";

    return 0;
}
