#pragma once
#include <plog/Log.h>

class MyClass
{
public:
    MyClass();
    ~MyClass();

    void method();

    void inlineMethod()
    {
        PLOGD;
    }

    static void staticMethod();

    operator std::string() const;
};
