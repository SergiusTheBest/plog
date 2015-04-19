//
// Chained - shows how to chain a logger (route messages) in a shared library with the main logger.
//

#include <plog/Log.h>

// Functions imported form the shared library.
extern "C" void initialize(plog::Severity severity, plog::IAppender* appender);
extern "C" void foo();

int main()
{
    plog::init(plog::debug, "ChainedApp.txt"); // Initialize the main logger.

    LOGD << "Hello from app!"; // Write a log message.

    initialize(plog::debug, plog::get()); // Initialize the logger in the shared library. Note that it has its own severity.
    foo(); // Call a function from the shared library that produces a log message.

    return 0;
}
