#include <Arduino.h>

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ArduinoAppender.h>

static plog::ArduinoAppender<plog::TxtFormatter> arduinoAppender(Serial);

void setup()
{
    plog::init(plog::verbose, &arduinoAppender);

    PLOG_VERBOSE << "verbose";
    PLOG_DEBUG << "debug";
    PLOG_INFO << "info";
    PLOG_WARNING << "warning";
    PLOG_ERROR << "error";
    PLOG_FATAL << "fatal";
}

void loop()
{
    delay(1000); // Wait for a second
}
