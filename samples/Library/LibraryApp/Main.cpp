//
// Library - shows plog usage in static libraries.
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

void foo(); // Function from the static library.

int main()
{
    plog::init(plog::debug, "LibraryApp.txt"); // Initialize the logger. The static library will use it.
    // Note that the main app is not required to use plog, the static library will be linked fine in any case.

    foo();

    PLOGD << "A message from the main application!";

    return 0;
}
