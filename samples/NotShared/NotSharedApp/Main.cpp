//
// NotShared - shows how to make logger instances local across binary modules (this is the default behavior on Windows but not on other platforms, so be careful).
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

// Functions imported form the shared libraries.
extern "C" void foo1();
extern "C" void foo2();

int main()
{
    plog::init(plog::debug, "NotSharedApp.txt"); // Initialize the logger. It will be visible only in this module and not in other modules because PLOG_LOCAL is defined.

    PLOGD << "Hello from app!"; // The message will go to the logger in this module.

    foo1(); // Call a function from the shared library that produces a log message.
    foo2(); // Call a function from the shared library that produces a log message.

    return 0;
}
