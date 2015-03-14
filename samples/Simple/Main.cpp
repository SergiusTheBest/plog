#include <plog/Log.h>
#include "MyClass.h"

int main()
{
    plog::init("Simple-log.txt", plog::debug, 5000, 3);

    // Log severity levels
    LOG_VERBOSE << "This is a VERBOSE message";
    LOG_DEBUG << "This is a DEBUG message";
    LOG_INFO << "This is an INFO message";
    LOG_WARNING << "This is a WARNING message";
    LOG_ERROR << "This is an ERROR message";
    LOG_FATAL << "This is a FATAL message";

    // Integer
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

    // Null strings
    LOG_DEBUG << static_cast<char*>(NULL);
    LOG_DEBUG << static_cast<wchar_t*>(NULL);
    LOG_DEBUG << static_cast<const char*>(NULL);
    LOG_DEBUG << static_cast<const wchar_t*>(NULL);

    // Unicode
    LOG_DEBUG << "Wide - вайд";
    LOG_DEBUG << L"Wide - вайд";
    LOG_DEBUG << std::string("test тест");
    LOG_DEBUG << std::wstring(L"test тест");
    LOG_DEBUG << L'ы';

    // Multiline
    LOG_INFO << "This\nis\na\nmultiline message!";

    // Quotes
    LOG_INFO << "This is a message with \"quotes\"!";

    // Conditional logging
    int var = 0;
    LOG_DEBUG_IF(var != 0) << "You shouldn't see this message";
    LOG_DEBUG_IF(var == 0) << "This is a conditional log message";

    // Log in a class (capture this pointer, c++ function names)
    MyClass obj;
    obj.inlineMethod();

    MyClass::staticMethod();

    return 0;
}
