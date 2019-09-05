#include "MyClass.h"

MyClass::MyClass()
{
    PLOGD;
}

MyClass::~MyClass()
{
    PLOGD;
}

void MyClass::method()
{
    PLOGD;
}

void MyClass::staticMethod()
{
    PLOGD;
}

MyClass::operator std::string() const
{
    return std::string("This is an implicit cast to string.");
}
