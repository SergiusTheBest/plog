//
// Facilities - this sample shows how to use logging per facilities via multiple log instances (useful for big projects).
//

#include <plog/Log.h>

enum Facility // Define log facilities.
{
    Default, // The default is 0.
    Auth,
    FileIO,
    Sink = -1 // This is a log sink. Messages from other facilities go there.
};

int main()
{
    plog::init<Sink>(plog::debug, "Facility.csv"); // Initialize the log sink.

    // Initialize all other logs and set the log sink as an appender. Each of the logs can have their own severity level.
    plog::init<Default>(plog::debug, plog::get<Sink>());
    plog::init<Auth>(plog::warning, plog::get<Sink>());
    plog::init<FileIO>(plog::info, plog::get<Sink>());

    LOGD_(Default) << "This is a message from the Default facility";
    LOGD << "This is a message from the Default facility too because Default = 0";

    LOGW_(Auth) << "This is a message from the Auth facility";
    LOGI_(FileIO) << "This is a message from the FileIO facility";

    return 0;
}
