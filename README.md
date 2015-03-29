#plog
Portable and simple log for C++ [![Build Status](https://travis-ci.org/SergiusTheBest/plog.svg?branch=master)](https://travis-ci.org/SergiusTheBest/plog)

- [Introduction](#introduction)
  - [Hello](#hello)
  - [Features](#features)
- [Usage](#usage)
  - [Step 1: Adding includes](#step-1-adding-includes)
  - [Step 2: Initialization](#step-2-initialization)
  - [Step 3: Logging](#step-3-logging)
    - [Basic logging macros](#basic-logging-macros)
      - [Long macros:](#long-macros)
      - [Short macros:](#short-macros)
      - [Function-style macros:](#function-style-macros)
    - [Conditional logging macros](#conditional-logging-macros)
      - [Long macros:](#long-macros-1)
      - [Short macros:](#short-macros-1)
      - [Function-style macros:](#function-style-macros-1)
    - [Logger severity checker](#logger-severity-checker)
- [Advanced usage](#advanced-usage)
  - [Changing severity at runtime](#changing-severity-at-runtime)
  - [Custom initialization](#custom-initialization)
  - [Multiple appenders](#multiple-appenders)
  - [Multiple loggers](#multiple-loggers)
  - [Chained loggers](#chained-loggers)
- [Samples](#samples)
- [Design](#design)
  - [Overview](#overview)
  - [Lazy stream evaluation](#lazy-stream-evaluation)
  - [Record](#record)
  - [Logger](#logger)
  - [Formatter](#formatter)
    - [TXT formatter](#txt-formatter)
    - [CSV formatter](#csv-formatter)
    - [FuncMessage formatter](#funcmessage-formatter)
  - [Appender](#appender)
    - [File appender](#file-appender)
    - [RollingFile appender](#rollingfile-appender)
    - [Console appender](#console-appender)
    - [Android appender](#android-appender)
- [Extending](#extending)
  - [Add custom type to stream output](#add-custom-type-to-stream-output)
  - [Write custom appender](#write-custom-appender)
  - [Write custom formatter](#write-custom-formatter)
- [Future plans](#future-plans)
- [References](#references)
  - [Competing C++ log libraries](#competing-c-log-libraries)

#Introduction

##Hello
I was looking for a C++ logging library but couldn't find the one that suits my requirements: some libraries are too fat and bloated, others do not have the features I need (CSV format and automatic 'this' pointer capture). So this library was born. The main idea: keep things as simple as possible.

This is a minimal hello log sample:

```cpp
#include <plog/Log.h>

int main()
{
    plog::init(plog::debug, "hello-log.csv");

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
* Cross-platform: Windows, Linux, Mac OS X, Android (gcc, clang, msvc)
* Thread and type safe
* Formatters: TXT, CSV, FuncMessage
* Appenders: File, RollingFile, Console, Android
* Automatic 'this' pointer capture (supported only on msvc)
* Lazy stream evaluation
* Unicode aware, files are stored in UTF8
* Doesn't require C++11

#Usage
There are 3 simple steps to start logging with plog.

##Step 1: Adding includes
First of all your project needs to know about plog. For that you have to:

1. Add `plog/inlcude` to the project include paths
2. Add `#include <plog/Log.h>` into your cpp/h files (if you have precompiled headers it is a good place to add this include there)

##Step 2: Initialization
The next step is to initialize plog. The basic scenario is writing a log to a file in txt or csv format with or without the rolling behavior. This kind of intialization is done by the following `plog::init` function:

```cpp
Logger& init(Severity maxSeverity, const char/wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0);
```

`maxSeverity` is the logger severity upper limit. All log messages have its own severity and if it is higher than the limit they are dropped. Plog defines the following severity levels (in ascending order):

* none
* fatal
* error
* warning
* info
* debug
* verbose

The log format is determined automatically by `fileName` file extension:

* .csv => csv format
* anyting else => txt format

The rolling behavior is controlled by `maxFileSize` and `maxFiles` parameters:

* `maxFileSize` - the maximum log file size in bytes
* `maxFiles` - the number of log files to keep

If one of them is zero then log rolling is disabled.

Sample:

```cpp
plog::init("c:\\logs\\log.csv", plog::warning, 1000000, 5); 
```

It initializes plog to write all messages with up to warning severity to a file in csv format. Maximum log file size is set to 1'000'000 bytes and 5 log files are kept.

##Step 3: Logging
Logging is performed with the help of special macros. A log message is constructed as a stream output thus it is type-safe and extendable.

###Basic logging macros 
This is the most used type of logging macros. They do unconditional logging.

####Long macros:

```cpp
LOG_VERBOSE << "verbose";
LOG_DEBUG << "debug";
LOG_INFO << "info";
LOG_WARNING << "warning";
LOG_ERROR << "error";
LOG_FATAL << "fatal";
```

####Short macros:

```cpp
LOGV << "verbose";
LOGD << "debug";
LOGI << "info";
LOGW << "warning";
LOGE << "error";
LOGF << "fatal";
```

####Function-style macros:

```cpp
LOG(severity) << "msg";
```

###Conditional logging macros
These macros are used to do a conditional logging. They accept a condition as a parameter and perform logging if the condition is true.

####Long macros:

```cpp
LOG_VERBOSE_IF(cond) << "verbose";
LOG_DEBUG_IF(cond) << "debug";
LOG_INFO_IF(cond) << "info";
LOG_WARNING_IF(cond) << "warning";
LOG_ERROR_IF(cond) << "error";
LOG_FATAL_IF(cond) << "fatal";
```

####Short macros:

```cpp
LOGV_IF(cond) << "verbose";
LOGD_IF(cond) << "debug";
LOGI_IF(cond) << "info";
LOGW_IF(cond) << "warning";
LOGE_IF(cond) << "error";
LOGF_IF(cond) << "fatal";
```

####Function-style macros:

```cpp
LOG_IF(severity, cond) << "msg";
```

###Logger severity checker
In some cases there is a need to perform a group of actions depending on the current logger severity level. There is a special macro for that. It helps to minimize performance penalty when the log is inactive.

```cpp
IF_LOG(severity)
```

Sample:

```cpp
IF_LOG(plog::debug) // we want to execute the following statements only at debug severity (and higher)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        LOGD << "vec[" << i << "]: " << vec[i];
    }
}
```

#Advanced usage

##Changing severity at runtime 
It is possible to set the maximum severity not only at the log initialization time but at any time later. There are special accessor methods for that:

```cpp
Severity Logger::getMaxSeverity() const;
Logger::setMaxSeverity(Severity severity);
```

To get the logger use `plog::get` function:

```cpp
Logger* get();
```

Sample:

```cpp
plog::get()->setMaxSeverity(plog::debug);
```

##Custom initialization
Non-typical log cases require the use of custom initialization. It is done by the following `plog::init` function:

```cpp
Logger& init(Severity maxSeverity = none, IAppender* appender = NULL);
```

You have to construct an appender parameterized with a formatter and pass it to the `plog::init` function. Note that a lifetime of the appender should be static!

Sample:

```cpp
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
plog::init(plog::debug, &consoleAppender);
```

##Multiple appenders
It is possible to have multiple appenders in a single logger. The following method is used for that:

```cpp
Logger& addAppender(IAppender* appender);
```

Sample:

```cpp
static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("MultiAppender.csv", 8000, 3);
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);
```

It initializes the log in the way when log messages are written to both a file and a console.

##Multiple loggers
Multiple loggers can be used simultaneously each with their own separate configuration. The loggers differ by their instance number (that is implemented as a template parameter). The default instance is zero. Initialization is done by the appropriate template `plog::init` functions:

```cpp
Logger<instance>& init<instance>(...);
```

All logging macros have their special versions that accept a logger instance parameter. These kind of macros have underscore at the end:

```cpp
LOGD_(instance) << "debug";
LOGD_IF(instance, condition) << "conditional debug";
```

Sample:

```cpp
enum
{
    SecondLog = 1
};

int main()
{
    plog::init(plog::debug, "MultiInstance-default.txt");
    plog::init<SecondLog>(plog::debug, "MultiInstance-second.txt");

    LOGD << "Hello default log!";
    LOGD_(SecondLog) << "Hello second log!";
    
    return 0;
}
```

##Chained loggers

#Samples
There are number of samples that demonstrate various aspects of using plog. They can be found in the samples folder:

* Android
* Chained
* Hello
* MultiAppender
* MultiInstance
* ObjectiveC
* Simple

#Design

##Overview

##Lazy stream evaluation

##Record

##Logger

##Formatter

###TXT formatter
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

###CSV formatter
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

###FuncMessage formatter

##Appender

###File appender

###RollingFile appender

###Console appender

###Android appender

#Extending

##Add custom type to stream output

##Write custom appender

##Write custom formatter

#Future plans
* Drop pre C++11 support when C++17 is released

#References

##Competing C++ log libraries
* [Boost::Log](http://www.boost.org/doc/libs/release/libs/log/)
* [EasyLogging++](https://github.com/easylogging/easyloggingpp)
* [g2log](http://www.codeproject.com/Articles/288827/g-log-An-efficient-asynchronous-logger-using-Cplus)
* [glog](https://code.google.com/p/google-glog/)
* [Log4cplus](http://sourceforge.net/projects/log4cplus/)
* [Log4cpp](http://log4cpp.sourceforge.net/)
* [Log4cxx](http://logging.apache.org/log4cxx/)
