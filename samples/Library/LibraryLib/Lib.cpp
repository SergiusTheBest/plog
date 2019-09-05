//
// Library - shows plog usage in static libraries.
//

#include <plog/Log.h>

void foo()
{
    // The logger is initialized in the main app. It is safe not to do that and even not to use plog at all. The library will be linked fine.
    PLOGD << "A message from the static library!";
}
