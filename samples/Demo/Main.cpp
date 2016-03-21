//
// Demo - demonstrates log stream abilities, prints various types of messages.
//

#include <plog/Log.h>
#include "MyClass.h"

int main()
{
    plog::init(plog::debug, "Demo.csv", 5000, 3); // Initialize the logger.

    // Log macro types.
    LOGD << "Hello log!"; // short macro
    LOG_DEBUG << "Hello log!"; // long macro
    LOG(plog::debug) << "Hello log!"; // function-style macro

    // Log severity levels.
    LOG_VERBOSE << "This is a VERBOSE message";
    LOG_DEBUG << "This is a DEBUG message";
    LOG_INFO << "This is an INFO message";
    LOG_WARNING << "This is a WARNING message";
    LOG_ERROR << "This is an ERROR message";
    LOG_FATAL << "This is a FATAL message";

    // Integers demo.
    LOG_INFO << "This is a bool: " << std::boolalpha << true;
    LOG_INFO << "This is a char: " << 'x';
    LOG_INFO << "This is an unsigned char: " << (unsigned char)40;
    LOG_INFO << "This is a short: " << (short)-1000;
    LOG_INFO << "This is an unsigned short: " << (unsigned short)1000;
    LOG_INFO << "This is an int: " << (int)-1000000;
    LOG_INFO << "This is an unsigned int: " << (unsigned int)1000000;
    LOG_INFO << "This is a long(hex): " << std::hex << (long)100000000;
    LOG_INFO << "This is an unsigned long: " << (unsigned long)100000000;
    LOG_INFO << "This is a float: " << 1.2345f;
    LOG_INFO << "This is a double: " << std::setprecision(15) << 1.234512345;

    // Null strings are safe.
    LOG_DEBUG << static_cast<char*>(NULL);
    LOG_DEBUG << static_cast<wchar_t*>(NULL);
    LOG_DEBUG << static_cast<const char*>(NULL);
    LOG_DEBUG << static_cast<const wchar_t*>(NULL);

    // Plog handles unicode and std::string/wstring.
#ifndef _WIN32 // On Windows the following code produces a warning C4566 if the codepage is not Cyrillic.
    LOG_DEBUG << "Cat - котэ";
    LOG_DEBUG << std::string("test - тест");
#endif
    LOG_DEBUG << L"Cat - котэ";
    LOG_DEBUG << std::wstring(L"test - тест");
    LOG_DEBUG << L'ы';

    // Multiline.
    LOG_INFO << "This\nis\na\nmultiline message!";

    // Quotes.
    LOG_INFO << "This is a message with \"quotes\"!";

    // Conditional logging.
    int var = 0;
    LOG_DEBUG_IF(var != 0) << "You shouldn't see this message";
    LOG_DEBUG_IF(var == 0) << "This is a conditional log message";

    // Log in a class (capture this pointer, c++ function names).
    MyClass obj;
    obj.method();
    obj.inlineMethod();

    MyClass::staticMethod();

    // Implicit cast to string.
    LOG_INFO << obj;

    return 0;
}
