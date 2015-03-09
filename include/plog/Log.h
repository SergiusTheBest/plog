#pragma once
#include <plog/Record.h>
#include <plog/Logger.h>
#include <plog/Init.h>
#include <plog/Compatibility.h>

//////////////////////////////////////////////////////////////////////////
// Helper macros that get context info

#ifdef _MSC_BUILD
#   define PLOG_GET_THIS()     __if_exists(this) { this } __if_not_exists(this) { 0 } 
#   define PLOG_GET_FUNC()     __FUNCTION__
#else
#   define PLOG_GET_THIS()     0
#   define PLOG_GET_FUNC()     __PRETTY_FUNCTION__
#endif

//////////////////////////////////////////////////////////////////////////
// Log severity level checker

#define IF_LOG(severity)    if (plog::Logger::getInstance().checkSeverity(severity))

//////////////////////////////////////////////////////////////////////////
// Main logging macros

#define LOG(severity) \
    IF_LOG(severity) plog::Logger::getInstance() += plog::Record(severity, PLOG_GET_FUNC(), __LINE__, PLOG_GET_THIS())

#define LOG_DEBUG       LOG(plog::debug)
#define LOG_INFO        LOG(plog::info)
#define LOG_WARNING     LOG(plog::warning)
#define LOG_ERROR       LOG(plog::error)
#define LOG_FATAL       LOG(plog::fatal)

#define LOGD            LOG_DEBUG
#define LOGI            LOG_INFO
#define LOGW            LOG_WARNING
#define LOGE            LOG_ERROR
#define LOGF            LOG_FATAL

//////////////////////////////////////////////////////////////////////////
// Conditional logging macros

#define LOG_IF(severity, condition) if (condition) LOG(severity)

#define LOG_DEBUG_IF(condition)     LOG_IF(plog::debug, condition)
#define LOG_INFO_IF(condition)      LOG_IF(plog::info, condition)
#define LOG_WARNING_IF(condition)   LOG_IF(plog::warning, condition)
#define LOG_ERROR_IF(condition)     LOG_IF(plog::error, condition)
#define LOG_FATAL_IF(condition)     LOG_IF(plog::fatal, condition)

#define LOGD_IF(condition)          LOG_DEBUG_IF(condition)
#define LOGI_IF(condition)          LOG_INFO_IF(condition)
#define LOGW_IF(condition)          LOG_WARNING_IF(condition)
#define LOGE_IF(condition)          LOG_ERROR_IF(condition)
#define LOGF_IF(condition)          LOG_FATAL_IF(condition)