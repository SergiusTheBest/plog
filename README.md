#plog
Portable and simple log for C++ [![Build Status](https://travis-ci.org/SergiusTheBest/plog.svg?branch=master)](https://travis-ci.org/SergiusTheBest/plog)

#Introduction

##Hello
I was looking for a C++ logging library but couldn't find the one that suits my requirements: some libraries are too fat and bloated, others do not have the features I need (CSV format and automatic 'this' pointer capture). So this library was born. The main idea: keep things as simple as possible.

This is a minimal hello log sample:
```cpp
#include <plog/Log.h>

int main()
{
    plog::init("hello-log.csv", plog::debug);

    LOGD << "Hello log!";
    LOG_DEBUG << "Hello log!";
    LOG(plog::debug) << "Hello log!";

    return 0;
}
```

##Features
* Very small (less than 1000 LOC)
* Easy to use
* Headers only
* No 3rd-party dependencies
* Cross-platform: Windows, Linux, Mac OS X, Android
* Thread and type safe
* Formatters: TXT, CSV, FuncMessage
* Appenders: File, RollingFile, Console, Android
* Automatic 'this' pointer capture (supported only on Visual Studio)
* Small execution overhead if log message is filtered out
* Unicode aware, files are stored in UTF8

#Usage

##Step 1: Include
1. Add `plog/inlcude` to the project include paths
2. Add `#include <plog/Log.h>` into your cpp/h files (if you have precompiled headers it is a good place to add this include there)

##Step 2: Initialization

###Simple scenario
Simple scenario is writing a txt or csv log to a file with or without rolling behavior. For these cases use the following initialization functions:
```cpp
// single log file
Logger& init(const char/wchar_t* fileName, Level maxSeverity); 

// rolling log files
Logger& init(const char/wchar_t* fileName, Level maxSeverity, size_t maxFileSize, int maxFiles);
```
If a file extension is `.csv` then csv format is used. Otherwise txt format is used. Rolling behavior is controlled by `maxFileSize` and `maxFiles` parameters.

Sample:
```cpp
plog::init("c:\\logs\\log.csv", plog::warning, 1000000, 5); 
```
Write all log messages with up to warning level to a file in csv format. Maximum log file size is set to 1000000 bytes and 5 log files are kept.

###Complex scenario
Sample:
```cpp
static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("multi-log.csv", plog::debug, 8000, 3);
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender(plog::debug);
plog::init().addAppender(&fileAppender).addAppender(&consoleAppender);
```

##Step 3: Logging
Logging is performed as a stream output thus it is type-safe and extendendable.

###Main logging macros 
Use the following macros to perform logging:

| Long macro | Short macro |
|------------|-------------|
| LOG_VERBOSE << "verbose"; | LOGV << "verbose"; |
| LOG_DEBUG << "debug"; | LOGD << "debug"; |
| LOG_INFO << "info"; | LOGI << "info"; |
| LOG_WARNING << "warning"; | LOGW << "warning"; |
| LOG_ERROR << "error"; | LOGE << "error"; |
| LOG_FATAL << "fatal"; | LOGF << "fatal"; |

| Function-style macro |
|----------------------|
| LOG(severity) << "msg"; |

###Conditional logging macros
These macros check a condition and perform logging if it is true:

Long macro | Short macro | Function-style macro
-----------|-------------|---------------------
LOG_VERBOSE_IF(cond) << "verb"; | LOGV_IF(cond) << "verb"; | LOG_IF(plog::verbose, cond) << "verb";
LOG_DEBUG_IF(cond) << "debug"; | LOGD_IF(cond) << "debug"; | LOG_IF(plog::debug, cond) << "debug";
LOG_INFO_IF(cond) << "info"; | LOGI_IF(cond) << "info"; | LOG_IF(plog::info, cond) << "info";
LOG_WARNING_IF(cond) << "warn"; | LOGW_IF(cond) << "warn"; | LOG_IF(plog::warning, cond) << "warn";
LOG_ERROR_IF(cond) << "error"; | LOGE_IF(cond) << "error"; | LOG_IF(plog::error, cond) << "error";
LOG_FATAL_IF(cond) << "fatal"; | LOGF_IF(cond) << "fatal"; | LOG_IF(plog::fatal, cond) << "fatal";

###Log severity level checker
If you need to do some actions depending on log severity level there is a macro for that:
```cpp
IF_LOG(severity)
```

Example:
```cpp
IF_LOG(plog::debug) // we want to execute it only at debug level
{
    for (int i = 0; i < vec.size(); ++i)
    {
        LOGD << "vec[" << i << "]: " << vec[i];
    }
}
```

#Sample log output
##TXT formatter
```
2014-11-11 00:29:06.245 FATAL [4460] [main@22] fatal
2014-11-11 00:29:06.261 ERROR [4460] [main@23] error
2014-11-11 00:29:06.261 INFO  [4460] [main@24] info
2014-11-11 00:29:06.261 WARN  [4460] [main@25] warning
2014-11-11 00:29:06.261 DEBUG [4460] [main@26] debug
2014-11-11 00:29:06.261 INFO  [4460] [main@29] This
is
a
multiline!
2014-11-11 00:29:06.261 INFO  [4460] [main@32] This is a message with "quotes"!
2014-11-11 00:29:06.261 DEBUG [4460] [Object::Object@8] 
2014-11-11 00:29:06.261 DEBUG [4460] [Object::~Object@13] 
```
##CSV formatter
```
Date;Time;Severity;TID;This;Function;Message
2014/11/14;15:22:25.033;FATAL;4188;00000000;main@22;"fatal"
2014/11/14;15:22:25.033;ERROR;4188;00000000;main@23;"error"
2014/11/14;15:22:25.033;INFO;4188;00000000;main@24;"info"
2014/11/14;15:22:25.033;WARN;4188;00000000;main@25;"warning"
2014/11/14;15:22:25.048;DEBUG;4188;00000000;main@26;"debug"
2014/11/14;15:22:25.048;INFO;4188;00000000;main@29;"This
is
a
multiline!"
2014/11/14;15:22:25.048;INFO;4188;00000000;main@32;"This is a message with ""quotes""!"
2014/11/14;15:22:25.048;DEBUG;4188;002EF4E3;Object::Object@8;
2014/11/14;15:22:25.048;DEBUG;4188;002EF4E3;Object::~Object@13;
```

#Competing C++ log libraries
* [Boost::Log](http://www.boost.org/doc/libs/release/libs/log/)
* [EasyLogging++](https://github.com/easylogging/easyloggingpp)
* [g2log](http://www.codeproject.com/Articles/288827/g-log-An-efficient-asynchronous-logger-using-Cplus)
* [glog](https://code.google.com/p/google-glog/)
* [Log4cplus](http://sourceforge.net/projects/log4cplus/)
* [Log4cpp](http://log4cpp.sourceforge.net/)
* [Log4cxx](http://logging.apache.org/log4cxx/)
