#include <jni.h>
#include <plog/Log.h>
#include <plog/Appenders/AndroidAppender.h>
#include <plog/Formatters/FuncMessageFormatter.h>

extern "C" void Java_com_github_sergius_myapp_Sample_foo(JNIEnv* env, jobject obj)
{
    static plog::AndroidAppender<plog::FuncMessageFormatter> appender("MyApp");
    static plog::Logger<0>& logger = plog::init(plog::debug, &appender);

    LOGD << "Hello Android!";
}