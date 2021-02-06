//
// Dynamic - a sample that shows how to use dynamic logger instances with raw pointers (when C++11 is not available).
//

#include <vector>
#include <sstream>
#include <plog/Log.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>

// This holds all required logger objects
struct LoggerContext
{
    LoggerContext(const char* filename, plog::Severity severity) : m_appender(filename), m_logger(severity, &m_appender)
    {
    }

    plog::RollingFileAppender<plog::TxtFormatter> m_appender;
    plog::Logger m_logger;
};

// Each worker receives its own logger
class Worker
{
public:
    Worker(plog::Logger* logger) : m_logger(logger)
    {
    }

    void work()
    {
        // Log macro types
        PLOGD_DYN_(m_logger) << "Hello log!"; // short macro
        PLOG_DEBUG_DYN_(m_logger) << "Hello log!"; // long macro
        PLOG_DYN_(m_logger, plog::debug) << "Hello log!"; // function-style macro

        // Conditional logging
        int var = 0;
        PLOG_DEBUG_IF_DYN_(m_logger, var != 0) << "You shouldn't see this message";
        PLOG_DEBUG_IF_DYN_(m_logger, var == 0) << "This is a conditional log message";

        // Executed only on log level >= debug
        IF_PLOG_DYN_(m_logger, plog::debug) var = 5; // one line
        IF_PLOG_DYN_(m_logger, plog::debug) // block
        {
            var++;
        }
    }

private:
    plog::Logger* m_logger;
};

int main()
{
    std::vector<LoggerContext*> loggerContexts;
    std::vector<Worker> workers;

    for (int i = 0; i < 3; ++i)
    {
        // Create a log file name
        std::stringstream ss;
        ss << "dynamic-" << i << "-worker.log";

        // Create and store a logger context
        LoggerContext* loggerCtx = new LoggerContext(ss.str().c_str(), plog::verbose);
        loggerContexts.push_back(loggerCtx);

        // Create a worker with logger context
        workers.push_back(Worker(&loggerCtx->m_logger));
    }

    // Empty logger is fine, no logs will be written in such case
    workers.push_back(Worker(NULL));

    // Do work
    for (size_t i = 0; i < workers.size(); ++i)
    {
        workers[i].work();
    }

    // Clean up logger contexts
    while (!loggerContexts.empty())
    {
        delete loggerContexts.back();
        loggerContexts.pop_back();
    }

    return 0;
}
