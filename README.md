#Plog - portable and simple log for C++
Pretty powerful log in less than 1000 lines of code [![Build Status](https://travis-ci.org/SergiusTheBest/plog.svg?branch=master)](https://travis-ci.org/SergiusTheBest/plog) [![Build status](https://ci.appveyor.com/api/projects/status/rna5gwhqjb13wovr/branch/master?svg=true)](https://ci.appveyor.com/project/SergiusTheBest/plog/branch/master)

- [Introduction](#introduction)
  - [Hello log!](#hello-log)
  - [Features](#features)
- [Usage](#usage)
  - [Step 1: Adding includes](#step-1-adding-includes)
  - [Step 2: Initialization](#step-2-initialization)
  - [Step 3: Logging](#step-3-logging)
    - [Basic logging macros](#basic-logging-macros)
    - [Conditional logging macros](#conditional-logging-macros)
    - [Logger severity checker](#logger-severity-checker)
- [Advanced usage](#advanced-usage)
  - [Changing severity at runtime](#changing-severity-at-runtime)
  - [Custom initialization](#custom-initialization)
  - [Multiple appenders](#multiple-appenders)
  - [Multiple loggers](#multiple-loggers)
  - [Chained loggers](#chained-loggers)
- [Architecture](#architecture)
  - [Overview](#overview)
  - [Logger](#logger)
  - [Record](#record)
  - [Formatter](#formatter)
    - [TxtFormatter](#txtformatter)
    - [CsvFormatter](#csvformatter)
    - [FuncMessageFormatter](#funcmessageformatter)
  - [Converter](#converter)
    - [UTF8Converter](#utf8converter)
  - [Appender](#appender)
    - [RollingFileAppender](#rollingfileappender)
    - [ConsoleAppender](#consoleappender)
    - [ColorConsoleAppender](#colorconsoleappender)
    - [AndroidAppender](#androidappender)
- [Miscellaneous notes](#miscellaneous-notes)
  - [Lazy stream evaluation](#lazy-stream-evaluation)
  - [Stream improvements over std::ostream](#stream-improvements-over-stdostream)
  - [Automatic 'this' pointer capture](#automatic-this-pointer-capture)
  - [Headers to include](#headers-to-include)
  - [Unicode](#unicode)
  - [Performance](#performance)
- [Extending](#extending)
  - [Custom data type](#custom-data-type)
  - [Custom appender](#custom-appender)
  - [Custom formatter](#custom-formatter)
  - [Custom converter](#custom-converter)
- [Samples](#samples)
- [References](#references)
  - [Competing C++ log libraries](#competing-c-log-libraries)
  - [Tools and useful info](#tools-and-useful-info)
- [License](#license)
- [Version history](#version-history)

#Introduction

##Hello log!
Plog is a C++ logging library that is designed to be as simple, small and flexible as possible. It is created as an alternative to existing large libraries and provides some unique features as [CSV log format]((#csvformatter)) and [automatic 'this' pointer capture](#automatic-this-pointer-capture).

Here is a minimal hello log sample:

```cpp
#include <plog/Log.h> // Step1: include the header.

int main()
{
    plog::init(plog::debug, "Hello.txt"); // Step2: initialize the logger.

    // Step3: write log messages using a special macro. 
    // There are several log macros, use the macro you liked the most.

    LOGD << "Hello log!"; // short macro
    LOG_DEBUG << "Hello log!"; // long macro
    LOG(plog::debug) << "Hello log!"; // function-style macro

    return 0;
}
```

And its output:

```
2015-05-18 23:12:43.921 DEBUG [21428] [main@13] Hello log!
2015-05-18 23:12:43.968 DEBUG [21428] [main@14] Hello log!
2015-05-18 23:12:43.968 DEBUG [21428] [main@15] Hello log!
```

##Features
- Very small (less than 1000 LOC)
- Easy to use
- Headers only
- No 3rd-party dependencies
- Cross-platform: Windows, Linux, Mac OS X, Android (gcc, clang, msvc, mingw, mingw-w64)
- Thread and type safe
- Formatters: [TXT](#txtformatter), [CSV](#csvformatter), [FuncMessage](#funcmessageformatter)
- Appenders: [RollingFile](#rollingfileappender), [Console](#consoleappender), [ColorConsole](#colorconsoleappender), [Android](#androidappender)
- [Automatic 'this' pointer capture](#automatic-this-pointer-capture) (supported only on msvc)
- [Lazy stream evaluation](#lazy-stream-evaluation)
- [Unicode aware](#unicode), files are stored in UTF8
- Doesn't require C++11
- [Extendable](#extending)

#Usage
To start using plog you need to make 3 simple steps.

##Step 1: Adding includes
At first your project needs to know about plog. For that you have to:

1. Add `plog/inlcude` to the project include paths
2. Add `#include <plog/Log.h>` into your cpp/h files (if you have precompiled headers it is a good place to add this include there)

##Step 2: Initialization
The next step is to initialize the [Logger](#logger). This is done by the following `plog::init` function:

```cpp
Logger& init(Severity maxSeverity, const char/wchar_t* fileName, size_t maxFileSize = 0, int maxFiles = 0);
```

`maxSeverity` is the logger severity upper limit. All log messages have its own severity and if it is higher than the limit those messages are dropped. Plog defines the following severity levels:

```cpp
enum Severity
{
    none = 0,
    fatal = 1,
    error = 2,
    warning = 3,
    info = 4,
    debug = 5,
    verbose = 6
};
```

The log format is determined automatically by `fileName` file extension:

- .csv => [CSV format](#csvformatter)
- anyting else => [TXT format](#txtformatter)

The rolling behavior is controlled by `maxFileSize` and `maxFiles` parameters:

- `maxFileSize` - the maximum log file size in bytes
- `maxFiles` - a number of log files to keep

If one of them is zero then log rolling is disabled.

Sample:

```cpp
plog::init(plog::warning, "c:\\logs\\log.csv", 1000000, 5); 
```

Here the logger is initialized to write all messages with up to warning severity to a file in csv format. Maximum log file size is set to 1'000'000 bytes and 5 log files are kept.

*Note: see [Custom initialization](#custom-initialization) for advanced usage.*

##Step 3: Logging
Logging is performed with the help of special macros. A log message is constructed using stream output operators `<<`. Thus it is type-safe and extendable in contrast to a format string output.

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
In some cases there is a need to perform a group of actions depending on the current logger severity level. There is a special macro for that. It helps to minimize performance penalty when the logger is inactive.

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
It is possible to set the maximum severity not only at the logger initialization time but at any time later. There are special accessor methods:

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

You have to construct an [Appender](#appender) parameterized with a [Formatter](#formatter) and pass it to the `plog::init` function. 

*Note: a lifetime of the appender should be static!*

Sample:

```cpp
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
plog::init(plog::debug, &consoleAppender);
```

##Multiple appenders
It is possible to have multiple [Appenders](#appender) within a single [Logger](#logger). In such case log message will be written to all of them. Use the following method to accomplish that:

```cpp
Logger& Logger::addAppender(IAppender* appender);
```

Sample:

```cpp
static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("MultiAppender.csv", 8000, 3); // Create the 1st appender.
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender; // Create the 2nd appender.
plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender); // Initialize the logger with the both appenders.
```

Here the logger is initialized in the way when log messages are written to both a file and a console.

*Refer to [MultiAppender](samples/MultiAppender) for a complete sample.*

##Multiple loggers
Multiple [Loggers](#logger) can be used simultaneously each with their own separate configuration. The [Loggers](#logger) differ by their instance number (that is implemented as a template parameter). The default instance is zero. Initialization is done by the appropriate template `plog::init` functions:

```cpp
Logger<instance>& init<instance>(...);
```

To get a logger use `plog::get` function (returns `NULL` if the logger is not initialized):

```cpp
Logger<instance>* get<instance>();
```

All logging macros have their special versions that accept an instance parameter. These kind of macros have an underscore at the end:

```cpp
LOGD_(instance) << "debug";
LOGD_IF_(instance, condition) << "conditional debug";
IF_LOG_(instance, severity)
```

Sample:

```cpp
enum // Define log instances. Default is 0 and is omitted from this enum.
{
    SecondLog = 1
};

int main()
{
    plog::init(plog::debug, "MultiInstance-default.txt"); // Initialize the default logger instance.
    plog::init<SecondLog>(plog::debug, "MultiInstance-second.txt"); // Initialize the 2nd logger instance.

    // Write some messages to the default log.
    LOGD << "Hello default log!";

    // Write some messages to the 2nd log.
    LOGD_(SecondLog) << "Hello second log!";

    return 0;
}
```

*Refer to [MultiInstance](samples/MultiInstance) for a complete sample.*

##Chained loggers
A [Logger](#logger) can work as an [Appender](#appender) for another [Logger](#logger). So you can chain several loggers together. This is useful for streaming log messages from a shared library to the main application binary.

Sample:

```cpp
// shared library

// Function that initializes the logger in the shared library. 
extern "C" void EXPORT initialize(plog::Severity severity, plog::IAppender* appender)
{
    plog::init(severity, appender); // Initialize the shared library logger.
}

// Function that produces a log message.
extern "C" void EXPORT foo()
{
    LOGI << "Hello from shared lib!";
}
```

```cpp
// main app

// Functions imported form the shared library.
extern "C" void initialize(plog::Severity severity, plog::IAppender* appender);
extern "C" void foo();

int main()
{
    plog::init(plog::debug, "ChainedApp.txt"); // Initialize the main logger.

    LOGD << "Hello from app!"; // Write a log message.

    initialize(plog::debug, plog::get()); // Initialize the logger in the shared library. Note that it has its own severity.
    foo(); // Call a function from the shared library that produces a log message.

    return 0;
}
```

*Refer to [Chained](samples/Chained) for a complete sample.*

#Architecture

##Overview
Plog is designed to be small but flexible, so it prefers templates to interface inheritance. All main entities are shown on the following UML diagram:

![Plog class diagram](http://gravizo.com/g?@startuml;interface%20IAppender%20{;%20%20%20%20+write%28%29;};class%20Logger<int%20instance>%20<<singleton>>%20{;%20%20%20%20+addAppender%28%29;%20%20%20%20+getMaxSeverity%28%29;%20%20%20%20+setMaxSeverity%28%29;%20%20%20%20+checkSeverity%28%29;%20%20%20%20-maxSeverity;%20%20%20%20-appenders;};class%20RollingFileAppender<Formatter,%20Converter>;class%20ConsoleAppender<Formatter>;class%20ColorConsoleAppender<Formatter>;class%20AndroidAppender<Formatter>;ConsoleAppender%20<|--%20ColorConsoleAppender;IAppender%20<|-u-%20Logger;IAppender%20<|--%20RollingFileAppender;IAppender%20<|--%20ConsoleAppender;IAppender%20<|--%20AndroidAppender;Logger%20"1"%20o--%20"0..n"%20IAppender;class%20CsvFormatter%20{;%20%20%20%20{static}%20header%28%29;%20%20%20%20{static}%20format%28%29;};class%20TxtFormatter%20{;%20%20%20%20{static}%20header%28%29;%20%20%20%20{static}%20format%28%29;};class%20FuncMessageFormatter%20{;%20%20%20%20{static}%20header%28%29;%20%20%20%20{static}%20format%28%29;};class%20UTF8Converter%20{;%20%20%20%20{static}%20header%28%29;%20%20%20%20{static}%20convert%28%29;};enum%20Severity%20{;%20%20%20%20none,;%20%20%20%20fatal,;%20%20%20%20error,;%20%20%20%20warning,;%20%20%20%20info,;%20%20%20%20debug,;%20%20%20%20verbose;};class%20Record%20{;%20%20%20%20+operator<<%28%29;%20%20%20%20-time;%20%20%20%20-severity;%20%20%20%20-tid;%20%20%20%20-object;%20%20%20%20-line;%20%20%20%20-message;%20%20%20%20-func;};hide%20empty%20members;hide%20empty%20fields;@enduml)
<!-- 
@startuml
interface IAppender {
    +write();
}

class Logger<int instance> <<singleton>> {
    +addAppender();
    +getMaxSeverity();
    +setMaxSeverity();
    +checkSeverity();
    -maxSeverity;
    -appenders;
}

class RollingFileAppender<Formatter, Converter>
class ConsoleAppender<Formatter>
class ColorConsoleAppender<Formatter>
class AndroidAppender<Formatter>

ConsoleAppender <|-- ColorConsoleAppender
IAppender <|-u- Logger
IAppender <|-- RollingFileAppender
IAppender <|-- ConsoleAppender
IAppender <|-- AndroidAppender

Logger "1" o-- "0..n" IAppender

class CsvFormatter {
    {static} header();
    {static} format();
}

class TxtFormatter {
    {static} header();
    {static} format();
}

class FuncMessageFormatter {
    {static} header();
    {static} format();
}

class UTF8Converter {
    {static} header();
    {static} convert();
}

enum Severity {
    none,
    fatal,
    error,
    warning,
    info,
    debug,
    verbose
}

class Record {
    +operator<<();
    -time;
    -severity;
    -tid;
    -object;
    -line;
    -message;
    -func;
}

hide empty members
hide empty fields
@enduml
-->

There are 5 functional parts:

- [Logger](#logger) - the main object, implemented as singleton
- [Record](#record) - keeps log data: time, message, etc
- [Appender](#appender) - represents a log data destination: file, console, etc
- [Formatter](#formatter) - formats log data into a string
- [Converter](#converter) - converts formatter output into a raw buffer

The log data flow is shown below:

![Log data flow](http://gravizo.com/g?@startuml;%28*%29%20-r->%20"LOG%20macro";-r->%20"Record";-r->%20"Logger";-r-->%20"Appender";-d->%20"Formatter";-d->%20"Converter";-u->%20"Appender";-r->%20%28*%29;@enduml)
<!--
@startuml
(*) -r-> "LOG macro"
-r-> "Record"
-r-> "Logger"
-r-> "Appender"
-d-> "Formatter"
-d-> "Converter"
-u-> "Appender"
-r-> (*)
@enduml
-->

##Logger
[Logger](#logger) is a center object of the whole logging system. It is a singleton and thus it forms a known single entry point for configuration and processing log data. [Logger](#logger) can act as [Appender](#appender) for another [Logger](#logger) because it implements `IAppender` interface. Also there can be several independent loggers that are parameterized by an integer instance number. The default instance is 0.

```cpp
template<int instance>
class Logger : public util::Singleton<Logger<instance> >, public IAppender
{
public:
    Logger(Severity maxSeverity = none);
    
    Logger& addAppender(IAppender* appender);
    
    Severity getMaxSeverity() const;
    void setMaxSeverity(Severity severity);
    bool checkSeverity(Severity severity) const;
    
    virtual void write(const Record& record);
    void operator+=(const Record& record);
};
```

##Record
[Record](#record) stores all log data. It includes:

- time
- severity
- thread id
- 'this' pointer (if a log message is written from within an object)
- source line
- function name
- message

Also [Record](#record) has a number of overloaded stream output operators to construct a message.

```cpp
class Record
{
public:
    Record(Severity severity, const char* func, size_t line, const void* object);
    
    //////////////////////////////////////////////////////////////////////////
    // Stream output operators
        
    Record& operator<<(char data);
    Record& operator<<(wchar_t data);
    
    template<typename T>
    Record& operator<<(const T& data);
    
    //////////////////////////////////////////////////////////////////////////
    // Getters
    
    const util::Time& getTime() const;
    Severity getSeverity() const;
    unsigned int getTid() const;
    const void* getObject() const;
    size_t getLine() const;
    const util::nstring getMessage() const;
    std::string getFunc() const;
};
```

*See [Stream improvements over std::ostream](#stream-improvements-over-stdostream).*

*Refer to [Demo](samples/Demo) sample to see what can be written to the log stream.*

##Formatter
[Formatter](#formatter) is responsible for formatting log data from [Record](#record) into various string representations (binary forms can be used too). There is no base class for formatters, they are implemented as classes with static functions `format` and `header`:

```cpp
class Formatter
{
public:
    static util::nstring header();
    static util::nstring format(const Record& record);
};
```

*See [How to implement a custom formatter](#custom-formatter).*

###TxtFormatter
This is a classic log format available in almost any log library. It is good for console output and it is easy to read without any tools.

```
2014-11-11 00:29:06.245 FATAL [4460] [main@22] fatal
2014-11-11 00:29:06.261 ERROR [4460] [main@23] error
2014-11-11 00:29:06.261 INFO  [4460] [main@24] info
2014-11-11 00:29:06.261 WARN  [4460] [main@25] warning
2014-11-11 00:29:06.261 DEBUG [4460] [main@26] debug
2014-11-11 00:29:06.261 INFO  [4460] [main@32] This is a message with "quotes"!
2014-11-11 00:29:06.261 DEBUG [4460] [Object::Object@8] 
2014-11-11 00:29:06.261 DEBUG [4460] [Object::~Object@13] 
```

###CsvFormatter
This is the most powerful log format. It can be easily read without any tools (but slighlty harder than [TXT format](#txtformatter)) and can be heavily analyzed if it is opened with a CSV-aware tool (like Excel). One rows can be highlighted according to their cell values, another rows can be hidden, columns can be manipulated and you can even run SQL queries on log data! This is a recommended format if logs are big and require heavy analysis. Also 'this' pointer is shown so object instances can be told apart.

```
Date;Time;Severity;TID;This;Function;Message
2014/11/14;15:22:25.033;FATAL;4188;00000000;main@22;"fatal"
2014/11/14;15:22:25.033;ERROR;4188;00000000;main@23;"error"
2014/11/14;15:22:25.033;INFO;4188;00000000;main@24;"info"
2014/11/14;15:22:25.033;WARN;4188;00000000;main@25;"warning"
2014/11/14;15:22:25.048;DEBUG;4188;00000000;main@26;"debug"
2014/11/14;15:22:25.048;INFO;4188;00000000;main@32;"This is a message with ""quotes""!"
2014/11/14;15:22:25.048;DEBUG;4188;002EF4E3;Object::Object@8;
2014/11/14;15:22:25.048;DEBUG;4188;002EF4E3;Object::~Object@13;
```

*Note: message size is limited to 32000 chars.*

###FuncMessageFormatter
This format is designed to be used with appenders that provide their own timestamps (like [AndroidAppender](#androidappender) or linux syslog facility).

```
main@22: fatal
main@23: error
main@24: info
main@25: warning
main@26: debug
main@32: This is a message with "quotes"!
Object::Object@8: 
Object::~Object@13: 
```

##Converter
[Converter](#converter) is responsible for conversion of [Formatter](#formatter) output data to a raw buffer (represented as `std::string`). It is used by [RollingFileAppender](#rollingfileappender) to perform a conversion before writing to a file. There is no base class for converters, they are implemented as classes with static functions `convert` and `header`: 

```cpp
class Converter
{
public:
    static std::string header(const util::nstring& str);
    static std::string convert(const util::nstring& str);
};
```

*See [How to implement a custom converter](#custom-converter).*

###UTF8Converter
[UTF8Converter](#utf8converter) is the only converter available in plog out of the box. It converts string data to UTF-8 with BOM. 

##Appender
[Appender](#appender) uses [Formatter](#formatter) and [Converter](#converter) to get a desired representation of log data and outputs (appends) it to a file/console/etc. All appenders must implement `IAppender` interface (the only interface in plog):

```cpp
class IAppender
{
public:
    virtual ~IAppender();
    virtual void write(const Record& record) = 0;
};
```

*See [How to implement a custom appender](#custom-appender).*

###RollingFileAppender
This appender outputs log data to a file with rolling behaviour. As template parameters it accepts both [Formatter](#formatter) and [Converter](#converter).

```cpp
RollingFileAppender<Formatter, Converter>::RollingFileAppender(const util::nchar* fileName, size_t maxFileSize = 0, int maxFiles = 0);
```

- `fileName` - a log file name
- `maxFileSize` - the maximum log file size in bytes
- `maxFiles` - a number of log files to keep

If `maxFileSize` or `maxFiles` is 0 then rolling behaviour is turned off. 

The sample file names produced by this appender:

- mylog.log <== current log file (size < maxFileSize)
- mylog.1.log <== previous log file (size >= maxFileSize)
- mylog.2.log <== previous log file (size >= maxFileSize)

*Note: the lowest `maxFileSize` is 1000 bytes.*

*Note: a log file is created on the first log message.*

###ConsoleAppender
This appender outputs log data to `stdout`.  As a template parameter it accepts [Formatter](#formatter).

```cpp
ConsoleAppender<Formatter>::ConsoleAppender();
```

###ColorConsoleAppender
This appender outputs log data to `stdout` using colors that depends on a log message severity level.  As a template parameter it accepts [Formatter](#formatter).

```cpp
ColorConsoleAppender<Formatter>::ColorConsoleAppender();
```

###AndroidAppender
[AndroidAppender](#androidappender) uses Android logging system to output log data. It can be viewed with [logcat](http://developer.android.com/tools/help/logcat.html) or in a log window of Android IDEs. As a template parameter this appender accepts [Formatter](#formatter) (usually [FuncMessageFormatter](#funcmessageformatter)).

```cpp
AndroidAppender<Formatter>::AndroidAppender(const char* tag);
```

#Miscellaneous notes

##Lazy stream evaluation
Log messages are constructed using lazy stream evaluation. It means that if a log message will be dropped (because of its severity) then stream output operators are not executed. Thus performance penalty of unprinted log messages is negligible.

```cpp
LOGD << /* the following statements will be executed only when the logger severity is debug or higher */ ...
```

##Stream improvements over std::ostream
Stream output in plog has several improvements over the standard `std::ostream`:

- handles wide chars/strings: `wchar_t`, `wchar_t*`, `std::wstring`
- handles `NULL` values for C-strings: `char*` and `wchar_t*`
- implicitly casts objects to: `std::string` and `std::wstring` (if they have an appropriate cast operator)

##Automatic 'this' pointer capture
'This' pointer is captured automatically to log data and can be printed by [CsvFormatter](#csvformatter). Unfortunately this feature is supported only on msvc 2010 and higher.

##Headers to include
The core plog functionality is provided by inclusion of `plog/Log.h` file. Extra components require inclusion of corresponding extra headers after `plog/Log.h`.

![Plog core and extra components](http://gravizo.com/g?@startuml;package%20"Plog%20core\\n%28no%20additional%20include,%20just%20plog/Log.h%29"%20{;%20%20class%20TxtFormatter;%20%20class%20CsvFormatter;%20%20class%20UTF8Converter;%20%20class%20RollingFileAppender;};package%20"Plog%20extra\\n%28requires%20additional%20include%29"%20{;%20%20class%20FuncMessageFormatter;%20%20class%20ConsoleAppender;%20%20class%20ColorConsoleAppender;%20%20class%20AndroidAppender;};hide%20empty%20members;hide%20empty%20fields;@enduml)
<!--
@startuml
package "Plog core\n(no additional include, just plog/Log.h)" {
  class TxtFormatter
  class CsvFormatter
  class UTF8Converter
  class RollingFileAppender
}
package "Plog extra\n(requires additional include)" {
  class FuncMessageFormatter
  class ConsoleAppender
  class ColorConsoleAppender
  class AndroidAppender
}
hide empty members
hide empty fields
@enduml
-->

##Unicode
Plog is unicode aware and wide string friendly. All messages are converted to a system native char type:

- `wchar_t` - on Windows
- `char` - on all other systems

Also `char` is treated as:

- active code page - on Windows
- UTF-8 - on all other systems

Internally plog uses `nstring`, `nstringstream` and `nchar` ('n' for native) that are defined as:

```cpp
#ifdef _WIN32
    typedef std::wstring nstring;
    typedef std::wstringstream nstringstream;
    typedef wchar_t nchar;
#else
    typedef std::string nstring;
    typedef std::stringstream nstringstream;
    typedef char nchar;
#endif
```

By default all log files are stored in UTF-8 with BOM thanks to [UTF8Converter](#utf8converter).

*Note: on Android wide string support in plog is disabled.*

##Performance
Plog is not using any asynchronous techniques so it may slow down your application on large volumes of log messages. 

Producing a single log message takes the following amount of time:

|CPU|OS|Time per a log call, microsec|
|----|----|:----:|
|AMD Phenom II 1055T @3.5GHz|Windows 2008 R2|12|
|AMD Phenom II 1055T @3.5GHz|Linux Mint 17.1|8|
|Intel Core i3-3120M @2.5GHz|Windows 2012 R2|25|
|Intel Core i5-2500K @4.2GHz|Windows 2008 R2|8|
|Intel Atom N270 @1.6GHz|Windows 2003|68|

Assume 20 microsec per a log call then 500 log calls per a second will slow down an application by 1%. It is acceptable for the most use cases.

*Refer to [Performance](samples/Performance) for a complete sample.*

#Extending
Plog can be easily extended to support new:

- [custom data type](#custom-data-type)
- [custom appender](#custom-appender)
- [custom formatter](#custom-formatter)
- [custom converter](#custom-converter)

##Custom data type
To output a custom data type to a log message implement the following function:

```cpp
namespace plog
{
    Record& operator<<(Record& record, const MyType& t);
}
```

*Refer to [CustomType](samples/CustomType) for a complete sample.*

##Custom appender
A custom appender must implement `IAppender` interface. Also it may accept [Formatter](#formatter) and [Converter](#converter) as template parameters however this is optional.

```cpp
namespace plog
{
    template<class Formatter>
    class MyAppender : public IAppender
    {
    public:
        virtual void write(const Record& record);
    };
}
```

*Refer to [CustomAppender](samples/CustomAppender) for a complete sample.*

##Custom formatter
A formatter that is compatible with existing appenders must be a class with 2 static methods:

- `header` - returns a header for a new log
- `format` - formats [Record](#record) to a string

```cpp
namespace plog
{
    class MyFormatter
    {
    public:
        static util::nstring header();
        static util::nstring format(const Record& record);
    };
}
```

*Refer to [CustomFormatter](samples/CustomFormatter) for a complete sample.*

##Custom converter
A converter must be a class with 2 static methods:

- `header` - converts a header for a new log
- `convert` - converts log messages

```cpp
namespace plog
{
    class MyConverter
    {
    public:
        static std::string header(const util::nstring& str);
        static std::string convert(const util::nstring& str);
    };
}
```

*Refer to [CustomConverter](samples/CustomConverter) for a complete sample.*

#Samples
There are a number of samples that demonstrate various aspects of using plog. They can be found in the [samples](samples) folder:

|Sample|Description|
|------|-----------|
|[Android](samples/Android)|Shows how to use the android-specific appender.|
|[Chained](samples/Chained)|Shows how to chain a logger in a shared library with the main logger (route messages).|
|[Library](samples/Library)|Shows plog usage in static libraries.|
|[Hello](samples/Hello)|A minimal introduction sample, shows the basic 3 steps to start using plog.|
|[MultiAppender](samples/MultiAppender)|Shows how to use multiple appenders with the same logger.|
|[MultiInstance](samples/MultiInstance)|Shows how to use multiple logger instances, each instance has its own independent configuration.|
|[ObjectiveC](samples/ObjectiveC)|Shows that plog can be used in ObjectiveC++.|
|[Demo](samples/Demo)|Demonstrates log stream abilities, prints various types of messages.|
|[ColorConsole](samples/ColorConsole)|Shows how to use a color console appender.|
|[CustomAppender](samples/CustomAppender)|Shows how to implement a custom appender that stores log messages in memory.|
|[CustomFormatter](samples/CustomFormatter)|Shows how to implement a custom formatter.|
|[CustomConverter](samples/CustomConverter)|Shows how to implement a custom converter that encrypts log messages.|
|[CustomType](samples/CustomType)|Shows how to print a custom type to the log stream.|
|[Facilities](samples/Facilities)|Shows how to use logging per facilities via multiple logger instances (useful for big projects).|
|[Performance](samples/Performance)|Measures time per a log call.|

#References

##Competing C++ log libraries

- [Boost::Log](http://www.boost.org/doc/libs/release/libs/log/)
- [EasyLogging++](https://github.com/easylogging/easyloggingpp)
- [g2log](http://www.codeproject.com/Articles/288827/g-log-An-efficient-asynchronous-logger-using-Cplus)
- [glog](https://code.google.com/p/google-glog/)
- [Log4cplus](http://sourceforge.net/projects/log4cplus/)
- [Log4cpp](http://log4cpp.sourceforge.net/)
- [Log4cxx](http://logging.apache.org/log4cxx/)
- [Pantheios](http://pantheios.sourceforge.net/)
- [spdlog](https://github.com/gabime/spdlog/)
- [reckless](https://github.com/mattiasflodin/reckless)

##Tools and useful info

- [__if_exists Statement](https://msdn.microsoft.com/en-us/library/x7wy9xh3.aspx)
- [Controlling Symbol Visibility](https://developer.apple.com/library/mac/documentation/DeveloperTools/Conceptual/CppRuntimeEnv/Articles/SymbolVisibility.html)
- [Gravizo](http://gravizo.com)
- [PlantUML](http://plantuml.sourceforge.net)
- [DocToc](https://github.com/thlorenz/doctoc)
- [CMake](http://www.cmake.org)

#License
Plog is licensed under the [MPL version 2.0](http://mozilla.org/MPL/2.0/). You can freely use it in your commercial or opensource software.

#Version history

##Version 1.0.1 (01 Nov 2015)
- New: Added ColorConsoleAppender
- Fixed #6: Compatibility with [Mingw-w64](http://mingw-w64.org/)
- Fixed #7: Log file not created if file name contains Unicode characters in Windows
- Fixed #4: Flush stdout
- Fixed #3: IntelliSense error: expected an identifier

##Version 1.0.0 (19 May 2015)
- Initial public release
