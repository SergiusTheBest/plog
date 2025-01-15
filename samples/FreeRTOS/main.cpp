// FreeRTOS includes
#include <FreeRTOS.h>
#include <task.h>

// PLOG includes
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

static void exampleTask(void* /*parameter*/)
{
    for (;;)
    {
        PLOGI << "tick";
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> appender;
    plog::init(plog::verbose, &appender);

    PLOG_VERBOSE << "verbose";
    PLOG_DEBUG << "debug";
    PLOG_INFO << "info";
    PLOG_WARNING << "warning";
    PLOG_ERROR << "error";
    PLOG_FATAL << "fatal";

    xTaskCreate(exampleTask, "example1", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1U, NULL);
    xTaskCreate(exampleTask, "example2", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1U, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Should not reach here
    for (;;)
    {
    }

    return 0;
}
