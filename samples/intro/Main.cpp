#include <plog/Log.h>
#include "MyClass.h"

int main()
{
    plog::init("intro-log.txt", plog::debug, 1000, 3);

    // Log levels
    LOG_FATAL << "fatal";
    LOG_ERROR << "error";
    LOG_INFO << "info";
    LOG_WARNING << "warning";
    LOG_DEBUG << "debug";

    // Integer
    LOG_DEBUG << "One million: " << 1000000;

    // Null
    LOG_DEBUG << static_cast<char*>(NULL);
    LOG_DEBUG << static_cast<wchar_t*>(NULL);
    LOG_DEBUG << static_cast<const char*>(NULL);
    LOG_DEBUG << static_cast<const wchar_t*>(NULL);

    // Unicode
    LOG_DEBUG << "Wide - вайд";
    LOG_DEBUG << L"Wide - вайд";
    LOG_DEBUG << std::string("test тест");
    LOG_DEBUG << std::wstring(L"test тест");
    LOG_DEBUG << 'ы';
    LOG_DEBUG << L'ы';

    // Multiline
    LOG_INFO << "This\nis\na\nmultiline!";

    // Quotes
    LOG_INFO << "This is a message with \"quotes\"!";

    // Conditional logging
    int var = 0;
    LOG_IF(plog::debug, var != 0) << "true";
    LOG_IF(plog::debug, var == 0) << "false";

    // Log in a class (capture this pointer, c++ function names)
    MyClass obj;
    obj.inlineMethod();

    MyClass::staticMethod();

    return 0;
}