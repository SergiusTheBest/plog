//
// Shared - shows how to share logger instances across binary modules (this is the default behavior on everything except Windows, so be careful).
//

#include <plog/Log.h>

// Helper macro to mark functions exported from the library.
#ifdef _WIN32
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __attribute__ ((visibility ("default")))
#endif

// Function that produces a log message.
extern "C" void EXPORT foo1()
{
    PLOGI << "Hello from shared lib #1!"; // The message will go to the logger initialized in the main module because PLOG_GLOBAL is defined.
    // On Windows the logger instance is imported from the main module because PLOG_IMPORT is defined.
}
