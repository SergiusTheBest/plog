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
* Cross-platform: Windows, Linux, Mac OS X, Android
* Thread and type safe
* No 3rd-party dependencies
* Small execution overhead if log is disabled
* TXT and CSV formatters
* File, RollingFile and Console appenders
* Automatic 'this' pointer capture (supported only on Visual Studio)
* Unicode aware, files are stored as UTF8

#Usage

##Step 1: Include
1. Add `plog/inlcude` to the project include paths
2. Add `#include <plog/Log.h>` into your cpp/h files (if you have precompiled headers it is a good place to add this include there)

##Step 2: Initialization

###Simple scenario
Simple scenario means writing a txt or csv log to a file with or without rolling behavior. For these cases use the following initialization functions:
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
Logging is performed as a stream output. 

###Main logging macros 
These macros perform logging.
Use the following long macros:
```cpp
LOG_DEBUG << "debug";
LOG_INFO << "info";
LOG_WARNING << "warning";
LOG_ERROR << "error";
LOG_FATAL << "fatal";
```
or their short versions:
```cpp
LOGD << "debug";
LOGI << "info";
LOGW << "warning";
LOGE << "error";
LOGF << "fatal";
```
Also you can use a function-like macro:
```cpp
LOG(plog::debug) << "debug";
```
Choose what you like the best.

Note that there are 5 logging levels.

###Conditional logging macros
These macros check a condition and perform logging if it is true.
Use the following long macros:
```cpp
LOG_DEBUG_IF(condition) << "debug";
LOG_INFO_IF(condition) << "info";
LOG_WARNING_IF(condition) << "warning";
LOG_ERROR_IF(condition) << "error";
LOG_FATAL_IF(condition) << "fatal";
```
or their short versions:
```cpp
LOGD_IF(condition) << "debug";
LOGI_IF(condition) << "info";
LOGW_IF(condition) << "warning";
LOGE_IF(condition) << "error";
LOGF_IF(condition) << "fatal";
```
Also you can use a function-like macro:
```cpp
LOG_IF(plog::debug, condition) << "debug";
```

#Sample log output
##Txt formatter
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
##Csv formatter
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
