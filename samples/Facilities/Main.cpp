//
// Facilities - shows how to use logging per facilities via multiple logger instances (useful for big projects).
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

enum Facility // Define log facilities.
{
    Default, // The default is 0.
    Auth,
    FileIO,
    Sink = -1 // This is a log sink. Messages from other facilities go there.
};

int main()
{
    plog::init<Sink>(plog::debug, "Facility.csv"); // Initialize the sink logger.

    // Initialize all other loggers and set the sink logger as an appender. Each of the loggers can have their own severity level.
    plog::init<Default>(plog::debug, plog::get<Sink>());
    plog::init<Auth>(plog::warning, plog::get<Sink>());
    plog::init<FileIO>(plog::info, plog::get<Sink>());

    PLOGD_(Default) << "This is a message from the Default facility";
    PLOGD << "This is a message from the Default facility too because Default = 0";

    PLOGW_(Auth) << "This is a message from the Auth facility";
    PLOGI_(FileIO) << "This is a message from the FileIO facility";

    return 0;
}
