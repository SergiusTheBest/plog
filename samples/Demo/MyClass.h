#include <plog/Log.h>

class MyClass
{
public:
    MyClass();
    ~MyClass();
    
    void inlineMethod()
    {
        LOG_DEBUG;
    }

    static void staticMethod()
    {
        LOG_DEBUG;
    }
};
