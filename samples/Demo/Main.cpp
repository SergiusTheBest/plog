//
// Demo - demonstrates log stream abilities, prints various types of messages.
//

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>

#include "MyClass.h"
#include "Customer.h"

#ifdef __cplusplus_cli
#pragma managed(push, off)
void unmanagedFunc()
{
    PLOGI << "Inside unmanaged function (char)";
    PLOGI << L"Inside unmanaged function (wchar_t)";
}
#pragma managed(pop)
#endif

int main()
{
    plog::init(plog::debug, "Demo.csv", 5000, 3); // Initialize logging to the file.

    plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::get()->addAppender(&consoleAppender); // Also add logging to the console.

    // Log macro types.
    PLOGD << "Hello log!"; // short macro
    PLOG_DEBUG << "Hello log!"; // long macro
    PLOG(plog::debug) << "Hello log!"; // function-style macro

    // Log severity levels.
    PLOG_VERBOSE << "This is a VERBOSE message";
    PLOG_DEBUG << "This is a DEBUG message";
    PLOG_INFO << "This is an INFO message";
    PLOG_WARNING << "This is a WARNING message";
    PLOG_ERROR << "This is an ERROR message";
    PLOG_FATAL << "This is a FATAL message";
    PLOG_NONE << "This is a NONE message";

    // Integers demo.
    PLOG_INFO << "This is a bool: " << std::boolalpha << true;
    PLOG_INFO << "This is a char: " << 'x';
    PLOG_INFO << "This is an unsigned char: " << (unsigned char)40;
    PLOG_INFO << "This is a short: " << (short)-1000;
    PLOG_INFO << "This is an unsigned short: " << (unsigned short)1000;
    PLOG_INFO << "This is an int: " << (int)-1000000;
    PLOG_INFO << "This is an unsigned int: " << (unsigned int)1000000;
    PLOG_INFO << "This is a long(hex): " << std::hex << (long)100000000;
    PLOG_INFO << "This is an unsigned long: " << (unsigned long)100000000;
    PLOG_INFO << "This is a float: " << 1.2345f;
    PLOG_INFO << "This is a double: " << std::setprecision(15) << 1.234512345;

#ifndef __cplusplus_cli
    PLOG_INFO.printf("This is a format %s %d", "message", 42);

#ifdef _WIN32
    PLOG_INFO.printf(L"This is a wide format %s %d", L"message", 42);
#endif
#endif //__cplusplus_cli

    // Managed string.
#ifdef __cplusplus_cli
    System::String^ managedStr = "This is a managed string";
    PLOG_INFO << managedStr;

    unmanagedFunc();
#endif

    // Null strings are safe.
    PLOG_DEBUG << static_cast<char*>(NULL);
    PLOG_DEBUG << static_cast<const char*>(NULL);

#if PLOG_ENABLE_WCHAR_INPUT
    PLOG_DEBUG << static_cast<wchar_t*>(NULL);
    PLOG_DEBUG << static_cast<const wchar_t*>(NULL);
#endif

    // Plog handles unicode and std::string/wstring.
#ifndef _WIN32 // On Windows the following code produces a warning C4566 if the codepage is not Cyrillic.
    PLOG_DEBUG << "test - тест";
    PLOG_DEBUG << std::string("test - тест");
#endif

#if PLOG_ENABLE_WCHAR_INPUT
    PLOG_DEBUG << L"test - тест";
    PLOG_DEBUG << std::wstring(L"test - тест");
    PLOG_DEBUG << L'ж';
#endif

#ifdef __cpp_char8_t
    PLOG_DEBUG << u8"Chinese: 中文";
    PLOG_DEBUG << const_cast<const char8_t*>(u8"Cyrillic: тест");
#endif

    // Multiline.
    PLOG_INFO << "This\nis\na" << std::endl << "multiline\nmessage!";

    // Quotes.
    PLOG_INFO << "This is a message with \"quotes\"!";

    // Conditional logging.
    int var = 0;
    PLOG_DEBUG_IF(var != 0) << "You shouldn't see this message";
    PLOG_DEBUG_IF(var == 0) << "This is a conditional log message";

    // Executed only on log level >= debug.
    IF_PLOG(plog::debug) var = 5; // one line
    IF_PLOG(plog::debug) // block
    {
        var++;
    }

    // Log macros don't break then-else clause without braces.
    if (var == 0) PLOGI << "then clause (condition is false, so it is skipped)"; else PLOGI << "else clase (should be visible)";

    // Log in a class (capture this pointer, c++ function names).
    MyClass obj;
    obj.method();
    obj.inlineMethod();

    MyClass::staticMethod();

    // Log in a template class.
    MyTemplateClass<int, int>(1, 2).inlineMethod();

    // Implicit cast to string.
    PLOG_INFO << obj;

    // ostream operator<< (on Windows wostream operator<< has priority but not required)
    Customer customer = { 10, "John" };
    PLOG_INFO << customer;

    // Std containers can be printed
    std::vector<int> vectorOfInts;
    vectorOfInts.push_back(1);
    vectorOfInts.push_back(2);
    vectorOfInts.push_back(3);
    PLOG_INFO << "std::vector<int>: " << vectorOfInts;

    std::deque<std::string> dequeOfStrings;
    dequeOfStrings.push_back("one");
    dequeOfStrings.push_back("two");
    dequeOfStrings.push_back("three");
    PLOG_INFO << "std::deque<std::string>: " << dequeOfStrings;

    std::list<const char*> listOfCharPointers;
    listOfCharPointers.push_back("one");
    listOfCharPointers.push_back("two");
    listOfCharPointers.push_back(NULL);
    PLOG_INFO << "std::list<const char*>: " << listOfCharPointers;

    std::set<int> setOfInts;
    setOfInts.insert(10);
    setOfInts.insert(20);
    setOfInts.insert(30);
    PLOG_INFO << "std::set<int>: " << setOfInts;

    std::map<std::string, int> mapStringToInt;
    mapStringToInt["red"] = 1;
    mapStringToInt["green"] = 2;
    mapStringToInt["blue"] = 4;
    PLOG_INFO << "std::map<std::string, int>: " << mapStringToInt;

    std::multimap<int, std::string> multimapIntToString;
    multimapIntToString.insert(std::make_pair(1, "one"));
    multimapIntToString.insert(std::make_pair(1, "uno"));
    multimapIntToString.insert(std::make_pair(2, "two"));
    multimapIntToString.insert(std::make_pair(2, "due"));
    PLOG_INFO << "std::multimap<int, std::string>: " << multimapIntToString;

    std::vector<std::vector<int> > vectorOfVectorsOfInts(3);
    vectorOfVectorsOfInts[0].push_back(1);
    vectorOfVectorsOfInts[0].push_back(2);
    vectorOfVectorsOfInts[1].push_back(-1);
    vectorOfVectorsOfInts[1].push_back(-2);
    PLOG_INFO << "std::vector<std::vector<int> >: " << vectorOfVectorsOfInts;

    return 0;
}
