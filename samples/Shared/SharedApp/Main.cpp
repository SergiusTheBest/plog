//
// Shared - shows how to share logger instances across binary modules (this is the default behavior on everything except Windows, so be careful).
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

// Functions imported form the shared libraries.
//
// NOTE:
// We use dynamic linking on Windows as DLL imports logger instances from EXE and static linking will make a dependency loop:
// DLL --(import logger)--> EXE, EXE --(import foo)--> DLL.
//
// On everything except Winows logger instances are shared across all modules, so there is no dependency loop.
#ifdef _WIN32
#   include <Windows.h>
typedef void (*Foo1Fn)();
typedef void (*Foo2Fn)();
#else
extern "C" void foo1();
extern "C" void foo2();
#endif

int main()
{
    plog::init(plog::debug, "Shared.txt"); // Initialize the logger.
    // It will be shared across modules because PLOG_GLOBAL is defined, so no need to call `plog::init` in them.
    // On Windows the logger will be exported because PLOG_EXPORT is defined.

    PLOGD << "Hello from app!"; // The message will go to the logger in this module.

#ifdef _WIN32
    HMODULE lib1 = LoadLibraryW(L"SharedLib1.dll");
    PLOGE_IF(!lib1) << "Couldn't load SharedLib1.dll";

    Foo1Fn foo1 = reinterpret_cast<Foo1Fn>(GetProcAddress(lib1, "foo1"));
    PLOGE_IF(!foo1) << "Couldn't get foo1 from SharedLib1.dll";

    HMODULE lib2 = LoadLibraryW(L"SharedLib2.dll");
    PLOGE_IF(!lib2) << "Couldn't load SharedLib2.dll";

    Foo1Fn foo2 = reinterpret_cast<Foo1Fn>(GetProcAddress(lib2, "foo2"));
    PLOGE_IF(!foo2) << "Couldn't get foo2 from SharedLib2.dll";
#endif

    foo1(); // Call a function from the shared library that produces a log message.
    foo2(); // Call a function from the shared library that produces a log message.

    return 0;
}
