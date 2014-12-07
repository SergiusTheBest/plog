#include <plog/Log.h>

class Object
{
public:
    Object()
    {
        LOG_DEBUG;
    }

    ~Object()
    {
        LOG_DEBUG;
    }
};

int main()
{
    plog::init_csv("log.csv", plog::debug);

    // Log levels
    LOG_FATAL << "fatal";
    LOG_ERROR << "error";
    LOG_INFO << "info";
    LOG_WARNING << "warning";
    LOG_DEBUG << "debug";

    // Multiline
    LOG_INFO << "This\nis\na\nmultiline!";

    // Quotes
    LOG_INFO << "This is a message with \"quotes\"!";

    // Log in a class (capture this pointer, c++ function names)
    Object obj;

    return 0;
}