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

template <typename T, typename U>
class MyTemplateClass
{
public:
    MyTemplateClass(T t1, U t2)
        : m_t1(t1)
        , m_t2(t2)
    {
    }

    inline void inlineMethod()
    {
        PLOGD;
    }
private:
    T m_t1;
    U m_t2;
};

