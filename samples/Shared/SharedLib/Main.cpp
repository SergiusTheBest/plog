//
// Shared - shows how to share logger instances across binary modules.
//

#include <plog/Log.h>
#include <plog/Init.h>

// Helper macro to mark functions exported from the library.
#ifdef _WIN32
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __attribute__ ((visibility ("default")))
#endif

// Function that produces a log message.
extern "C" void EXPORT foo()
{
    PLOGI << "Hello from shared lib!"; // The message will go to the logger initialized in the main module.
}
