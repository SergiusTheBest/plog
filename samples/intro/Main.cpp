#include <plog/Log.h>
#include "MyClass.h"

int main()
{
    plog::init("intro-log.txt", plog::debug, 1000, 3);

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

    // Conditional logging
    int var = 0;
    LOG_IF(plog::debug, var != 0) << "true";
    LOG_IF(plog::debug, var == 0) << "false";

    // Log in a class (capture this pointer, c++ function names)
    MyClass obj;
    obj.inlineMethod();

    MyClass::staticMethod();

    return 0;
}