#include <plog/Log.h>

enum Facility
{
    Default,
    Auth,
    FileIO,
    Sink = -1
};

int main()
{
    plog::init<Sink>(plog::debug, "Facility.csv");

    plog::init<Default>(plog::debug, plog::get<Sink>());
    plog::init<Auth>(plog::warning, plog::get<Sink>());
    plog::init<FileIO>(plog::info, plog::get<Sink>());

    
    LOGD_(Default) << "This is a message from the Default facility";
    LOGD << "This is a message from the Default facility too because Default = 0";

    LOGW_(Auth) << "This is a message from the Auth facility";
    LOGI_(FileIO) << "This is a message from the FileIO facility";

    return 0;
}
