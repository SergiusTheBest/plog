#include <plog/Log.h>

class MyClass
{
public:
    MyClass();
    ~MyClass();
    
    void method();

    void inlineMethod()
    {
        LOGD;
    }

    static void staticMethod();

    operator std::string() const;
};
