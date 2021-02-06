//
// Dynamic11 - a sample that shows how to use dynamic logger instances with C++11 smart pointers.
//

#include <memory>
#include <functional>
#include <vector>
#include <sstream>
#include <plog/Log.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>

// This holds all required logger objects
class LoggerContext
{
public:
    LoggerContext(const char* filename, plog::Severity severity) : m_appender(filename), m_logger(severity, &m_appender)
    {
    }

    // Need a getter method that returns plog::Logger*
    plog::Logger* get()
    {
        return &m_logger;
    }

private:
    plog::RollingFileAppender<plog::TxtFormatter> m_appender;
    plog::Logger m_logger;
};

// Each worker receives its own logger context
class Worker
{
public:
    Worker(std::shared_ptr<LoggerContext> loggerCtx) : m_loggerCtx(loggerCtx)
    {
    }

    void work()
    {
        // Log macro types
        PLOGD_DYN_(m_loggerCtx) << "Hello log!"; // short macro
        PLOG_DEBUG_DYN_(m_loggerCtx) << "Hello log!"; // long macro
        PLOG_DYN_(m_loggerCtx, plog::debug) << "Hello log!"; // function-style macro

        // Conditional logging
        int var = 0;
        PLOG_DEBUG_IF_DYN_(m_loggerCtx, var != 0) << "You shouldn't see this message";
        PLOG_DEBUG_IF_DYN_(m_loggerCtx, var == 0) << "This is a conditional log message";

        // Executed only on log level >= debug
        IF_PLOG_DYN_(m_loggerCtx, plog::debug) var = 5; // one line
        IF_PLOG_DYN_(m_loggerCtx, plog::debug) // block
        {
            var++;
        }
    }

private:
    std::shared_ptr<LoggerContext> m_loggerCtx;
};

int main()
{
    std::vector<Worker> workers;

    for (int i = 0; i < 3; ++i)
    {
        // Create a log file name
        std::stringstream ss;
        ss << "dynamic-" << i << "-worker.log";

        // Create a worker and a logger context
        workers.emplace_back(std::make_shared<LoggerContext>(ss.str().c_str(), plog::verbose));
    }

    // Empty logger context is fine, no logs will be written in such case
    workers.emplace_back(nullptr);

    // Do work
    std::for_each(workers.begin(), workers.end(), std::mem_fn(&Worker::work));

    return 0;
}
