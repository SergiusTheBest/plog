#pragma once
#include <plog/Record.h>
#include <plog/Logger.h>
#include <plog/Init.h>
#include <plog/Compatibility.h>

#ifdef _MSC_BUILD
#define PLOG_GET_THIS() __if_exists(this) { this } __if_not_exists(this) { 0 } 
#define PLOG_GET_FUNC() __FUNCTION__
#else
#define PLOG_GET_THIS() 0
#define PLOG_GET_FUNC() __PRETTY_FUNCTION__
#endif

#define LOG(severity) \
    if (plog::Logger::getInstance().checkSeverity(severity)) plog::Logger::getInstance() += plog::Record(severity, PLOG_GET_FUNC(), __LINE__, PLOG_GET_THIS())

#define LOG_DEBUG   LOG(plog::debug)
#define LOG_INFO    LOG(plog::info)
#define LOG_WARNING LOG(plog::warning)
#define LOG_ERROR   LOG(plog::error)
#define LOG_FATAL   LOG(plog::fatal)

#define LOGD        LOG_DEBUG
#define LOGI        LOG_INFO
#define LOGW        LOG_WARNING
#define LOGE        LOG_ERROR
#define LOGF        LOG_FATAL

#define LOG_IF(severity, condition) if (condition) LOG(severity)