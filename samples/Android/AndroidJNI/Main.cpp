//
// AndroidJNI - shows how to use the Android appender in JNI.
//

#include <jni.h>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/AndroidAppender.h>
#include <plog/Formatters/FuncMessageFormatter.h>

extern "C" jint JNI_Onload(JavaVM*, void*)
{
    static plog::AndroidAppender<plog::FuncMessageFormatter> appender("MyApp"); // Create an appender and set a log tag.
    plog::init(plog::debug, &appender); // Initialize the logger with the appender.

    return JNI_VERSION_1_6;
}

extern "C" void Java_com_github_sergius_myapp_Sample_foo(JNIEnv*, jobject)
{
    PLOGD << "Hello Android!";
}
