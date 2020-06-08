//
// Shared - shows how to share logger instances across binary modules.
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

// Function imported form the shared library.
extern "C" void foo();

int main()
{
    plog::init(plog::debug, "Shared.txt"); // Initialize the logger. It will be shared across modules, so no need to call `plog::init` in them.

    PLOGD << "Hello from app!"; // Write a log message.

    foo(); // Call a function from the shared library that produces a log message.

    return 0;
}
