#include "MyClass.h"

MyClass::MyClass()
{
    LOGD;
}

MyClass::~MyClass()
{
    LOGD;
}

void MyClass::method()
{
    LOGD;
}

void MyClass::staticMethod()
{
    LOGD;
}

MyClass::operator std::string() const
{
    return std::string("This is an implicit cast to string.");
}
