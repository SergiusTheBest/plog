//////////////////////////////////////////////////////////////////////////
//  Plog - portable and simple log for C++
//  Documentation and sources: https://github.com/SergiusTheBest/plog
//  License: MPL 2.0, http://mozilla.org/MPL/2.0/

#pragma once
#include <plog/Record.h>
#include <plog/Logger.h>
#include <plog/Init.h>

//////////////////////////////////////////////////////////////////////////
// Helper macros that get context info

#if _MSC_VER >= 1600 && !defined(__INTELLISENSE__) // >= Visual Studio 2010 and skip IntelliSense
#   define PLOG_GET_THIS()      __if_exists(this) { this } __if_not_exists(this) { 0 }
#else
#   define PLOG_GET_THIS()      0
#endif

#ifdef _MSC_BUILD
#   define PLOG_GET_FUNC()      __FUNCTION__
#elif defined(__BORLANDC__)
#   define PLOG_GET_FUNC()      __FUNC__
#else
#   define PLOG_GET_FUNC()      __PRETTY_FUNCTION__
#endif

//////////////////////////////////////////////////////////////////////////
// Log severity level checker

#define PLOG_IF_LOG_(instance, severity)     if (plog::get<instance>() && plog::get<instance>()->checkSeverity(severity))
#define PLOG_IF_LOG(severity)                PLOG_IF_LOG_(PLOG_DEFAULT_INSTANCE, severity)

//////////////////////////////////////////////////////////////////////////
// Main logging macros

#define PLOG_LOG_(instance, severity)        PLOG_IF_LOG_(instance, severity) (*plog::get<instance>()) += plog::Record(severity, PLOG_GET_FUNC(), __LINE__, PLOG_GET_THIS())
#define PLOG_LOG(severity)                   PLOG_LOG_(PLOG_DEFAULT_INSTANCE, severity)

#define PLOG_LOG_WHERE_(instance, severity, function, line)  PLOG_IF_LOG_(instance, severity) (*plog::get<instance>()) += plog::Record(severity, function, line, PLOG_GET_THIS())

#define PLOG_LOG_WHERE(severity, function, line)  PLOG_LOG_WHERE_(PLOG_DEFAULT_INSTANCE, severity, function, line)

#define PLOG_LOG_VERBOSE                     PLOG_LOG(plog::verbose)
#define PLOG_LOG_DEBUG                       PLOG_LOG(plog::debug)
#define PLOG_LOG_INFO                        PLOG_LOG(plog::info)
#define PLOG_LOG_WARNING                     PLOG_LOG(plog::warning)
#define PLOG_LOG_ERROR                       PLOG_LOG(plog::error)
#define PLOG_LOG_FATAL                       PLOG_LOG(plog::fatal)

#define PLOG_LOG_VERBOSE_(instance)          PLOG_LOG_(instance, plog::verbose)
#define PLOG_LOG_DEBUG_(instance)            PLOG_LOG_(instance, plog::debug)
#define PLOG_LOG_INFO_(instance)             PLOG_LOG_(instance, plog::info)
#define PLOG_LOG_WARNING_(instance)          PLOG_LOG_(instance, plog::warning)
#define PLOG_LOG_ERROR_(instance)            PLOG_LOG_(instance, plog::error)
#define PLOG_LOG_FATAL_(instance)            PLOG_LOG_(instance, plog::fatal)

#define PLOG_LOGV                            PLOG_LOG_VERBOSE
#define PLOG_LOGD                            PLOG_LOG_DEBUG
#define PLOG_LOGI                            PLOG_LOG_INFO
#define PLOG_LOGW                            PLOG_LOG_WARNING
#define PLOG_LOGE                            PLOG_LOG_ERROR
#define PLOG_LOGF                            PLOG_LOG_FATAL

#define PLOG_LOGV_(instance)                 PLOG_LOG_VERBOSE_(instance)
#define PLOG_LOGD_(instance)                 PLOG_LOG_DEBUG_(instance)
#define PLOG_LOGI_(instance)                 PLOG_LOG_INFO_(instance)
#define PLOG_LOGW_(instance)                 PLOG_LOG_WARNING_(instance)
#define PLOG_LOGE_(instance)                 PLOG_LOG_ERROR_(instance)
#define PLOG_LOGF_(instance)                 PLOG_LOG_FATAL_(instance)

//////////////////////////////////////////////////////////////////////////
// Conditional logging macros

#define PLOG_LOG_IF_(instance, severity, condition)  if (condition) PLOG_LOG_(instance, severity)
#define PLOG_LOG_IF(severity, condition)             PLOG_LOG_IF_(PLOG_DEFAULT_INSTANCE, severity, condition)

#define PLOG_LOG_VERBOSE_IF(condition)               PLOG_LOG_IF(plog::verbose, condition)
#define PLOG_LOG_DEBUG_IF(condition)                 PLOG_LOG_IF(plog::debug, condition)
#define PLOG_LOG_INFO_IF(condition)                  PLOG_LOG_IF(plog::info, condition)
#define PLOG_LOG_WARNING_IF(condition)               PLOG_LOG_IF(plog::warning, condition)
#define PLOG_LOG_ERROR_IF(condition)                 PLOG_LOG_IF(plog::error, condition)
#define PLOG_LOG_FATAL_IF(condition)                 PLOG_LOG_IF(plog::fatal, condition)

#define PLOG_LOG_VERBOSE_IF_(instance, condition)    PLOG_LOG_IF_(instance, plog::verbose, condition)
#define PLOG_LOG_DEBUG_IF_(instance, condition)      PLOG_LOG_IF_(instance, plog::debug, condition)
#define PLOG_LOG_INFO_IF_(instance, condition)       PLOG_LOG_IF_(instance, plog::info, condition)
#define PLOG_LOG_WARNING_IF_(instance, condition)    PLOG_LOG_IF_(instance, plog::warning, condition)
#define PLOG_LOG_ERROR_IF_(instance, condition)      PLOG_LOG_IF_(instance, plog::error, condition)
#define PLOG_LOG_FATAL_IF_(instance, condition)      PLOG_LOG_IF_(instance, plog::fatal, condition)

#define PLOG_LOGV_IF(condition)                      PLOG_LOG_VERBOSE_IF(condition)
#define PLOG_LOGD_IF(condition)                      PLOG_LOG_DEBUG_IF(condition)
#define PLOG_LOGI_IF(condition)                      PLOG_LOG_INFO_IF(condition)
#define PLOG_LOGW_IF(condition)                      PLOG_LOG_WARNING_IF(condition)
#define PLOG_LOGE_IF(condition)                      PLOG_LOG_ERROR_IF(condition)
#define PLOG_LOGF_IF(condition)                      PLOG_LOG_FATAL_IF(condition)

#define PLOG_LOGV_IF_(instance, condition)           PLOG_LOG_VERBOSE_IF_(instance, condition)
#define PLOG_LOGD_IF_(instance, condition)           PLOG_LOG_DEBUG_IF_(instance, condition)
#define PLOG_LOGI_IF_(instance, condition)           PLOG_LOG_INFO_IF_(instance, condition)
#define PLOG_LOGW_IF_(instance, condition)           PLOG_LOG_WARNING_IF_(instance, condition)
#define PLOG_LOGE_IF_(instance, condition)           PLOG_LOG_ERROR_IF_(instance, condition)
#define PLOG_LOGF_IF_(instance, condition)           PLOG_LOG_FATAL_IF_(instance, condition)
