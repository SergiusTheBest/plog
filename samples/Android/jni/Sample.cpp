//
// Android - shows how to use the android-specific appender.
//

#include <jni.h>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/AndroidAppender.h>
#include <plog/Formatters/FuncMessageFormatter.h>

extern "C" void Java_com_github_sergius_myapp_Sample_foo(JNIEnv* env, jobject obj)
{
    // For simplicity the logger is initialized here. But the good place is JNI_OnLoad.
    static plog::AndroidAppender<plog::FuncMessageFormatter> appender("MyApp"); // Create an appender and set a log tag.
    static plog::Logger<0>& logger = plog::init(plog::debug, &appender); // Initialize the logger with the appender.

    PLOGD << "Hello Android!";
}
