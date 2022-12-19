//
// NotShared - shows how to make logger instances local across binary modules (this is the default behavior on Windows but not on other platforms, so be careful).
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

// Helper macro to mark functions exported from the library.
#ifdef _WIN32
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __attribute__ ((visibility ("default")))
#endif

// Function that produces a log message.
extern "C" void EXPORT foo1()
{
    plog::init(plog::debug, "NotSharedLib1.txt"); // Initialize the logger. It will be visible only in this module and not in other modules because PLOG_LOCAL is defined.
    PLOGI << "Hello from shared lib #1!"; // The message will go to the logger in this module.
}
