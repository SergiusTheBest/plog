//////////////////////////////////////////////////////////////////////////
//  Plog - portable and simple log for C++
//  Documentation and sources: https://github.com/SergiusTheBest/plog
//  License: MPL 2.0, http://mozilla.org/MPL/2.0/

#pragma once
#include <plog/Logger.h>
#include <plog/Init.h>

//////////////////////////////////////////////////////////////////////////
// Helper macros that get context info

#if defined(_MSC_VER) && _MSC_VER >= 1600 && !defined(__INTELLISENSE__) && !defined(__INTEL_COMPILER) && !defined(__llvm__) && !defined(__RESHARPER__) // >= Visual Studio 2010, skip IntelliSense, Intel Compiler, Clang Code Model and ReSharper
#   define PLOG_GET_THIS()      __if_exists(this) { this } __if_not_exists(this) { 0 }
#else
#   define PLOG_GET_THIS()      reinterpret_cast<void*>(0)
#endif

#ifdef _MSC_VER
#   define PLOG_GET_FUNC()      __FUNCTION__
#elif defined(__BORLANDC__)
#   define PLOG_GET_FUNC()      __FUNC__
#else
#   define PLOG_GET_FUNC()      __PRETTY_FUNCTION__
#endif

#ifdef PLOG_CAPTURE_FILE
#   define PLOG_GET_FILE()      __FILE__
#else
#   define PLOG_GET_FILE()      ""
#endif

//////////////////////////////////////////////////////////////////////////
// Log severity level checker

#define IF_PLOG_(instance, severity)     if (!plog::get<instance>() || !plog::get<instance>()->checkSeverity(severity)) {;} else
#define IF_PLOG(severity)                IF_PLOG_(PLOG_DEFAULT_INSTANCE, severity)

//////////////////////////////////////////////////////////////////////////
// Main logging macros

#define PLOG_(instance, severity)        IF_PLOG_(instance, severity) (*plog::get<instance>()) += plog::Record(severity, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()).ref()
#define PLOG(severity)                   PLOG_(PLOG_DEFAULT_INSTANCE, severity)

#define PLOG_VERBOSE                     PLOG(plog::verbose)
#define PLOG_DEBUG                       PLOG(plog::debug)
#define PLOG_INFO                        PLOG(plog::info)
#define PLOG_WARNING                     PLOG(plog::warning)
#define PLOG_ERROR                       PLOG(plog::error)
#define PLOG_FATAL                       PLOG(plog::fatal)
#define PLOG_NONE                        PLOG(plog::none)

#define PLOG_VERBOSE_(instance)          PLOG_(instance, plog::verbose)
#define PLOG_DEBUG_(instance)            PLOG_(instance, plog::debug)
#define PLOG_INFO_(instance)             PLOG_(instance, plog::info)
#define PLOG_WARNING_(instance)          PLOG_(instance, plog::warning)
#define PLOG_ERROR_(instance)            PLOG_(instance, plog::error)
#define PLOG_FATAL_(instance)            PLOG_(instance, plog::fatal)
#define PLOG_NONE_(instance)             PLOG_(instance, plog::none)

#define PLOGV                            PLOG_VERBOSE
#define PLOGD                            PLOG_DEBUG
#define PLOGI                            PLOG_INFO
#define PLOGW                            PLOG_WARNING
#define PLOGE                            PLOG_ERROR
#define PLOGF                            PLOG_FATAL
#define PLOGN                            PLOG_NONE

#define PLOGV_(instance)                 PLOG_VERBOSE_(instance)
#define PLOGD_(instance)                 PLOG_DEBUG_(instance)
#define PLOGI_(instance)                 PLOG_INFO_(instance)
#define PLOGW_(instance)                 PLOG_WARNING_(instance)
#define PLOGE_(instance)                 PLOG_ERROR_(instance)
#define PLOGF_(instance)                 PLOG_FATAL_(instance)
#define PLOGN_(instance)                 PLOG_NONE_(instance)

//////////////////////////////////////////////////////////////////////////
// Conditional logging macros

#define PLOG_IF_(instance, severity, condition)  if (!(condition)) {;} else PLOG_(instance, severity)
#define PLOG_IF(severity, condition)             PLOG_IF_(PLOG_DEFAULT_INSTANCE, severity, condition)

#define PLOG_VERBOSE_IF(condition)               PLOG_IF(plog::verbose, condition)
#define PLOG_DEBUG_IF(condition)                 PLOG_IF(plog::debug, condition)
#define PLOG_INFO_IF(condition)                  PLOG_IF(plog::info, condition)
#define PLOG_WARNING_IF(condition)               PLOG_IF(plog::warning, condition)
#define PLOG_ERROR_IF(condition)                 PLOG_IF(plog::error, condition)
#define PLOG_FATAL_IF(condition)                 PLOG_IF(plog::fatal, condition)
#define PLOG_NONE_IF(condition)                  PLOG_IF(plog::none, condition)

#define PLOG_VERBOSE_IF_(instance, condition)    PLOG_IF_(instance, plog::verbose, condition)
#define PLOG_DEBUG_IF_(instance, condition)      PLOG_IF_(instance, plog::debug, condition)
#define PLOG_INFO_IF_(instance, condition)       PLOG_IF_(instance, plog::info, condition)
#define PLOG_WARNING_IF_(instance, condition)    PLOG_IF_(instance, plog::warning, condition)
#define PLOG_ERROR_IF_(instance, condition)      PLOG_IF_(instance, plog::error, condition)
#define PLOG_FATAL_IF_(instance, condition)      PLOG_IF_(instance, plog::fatal, condition)
#define PLOG_NONE_IF_(instance, condition)       PLOG_IF_(instance, plog::none, condition)

#define PLOGV_IF(condition)                      PLOG_VERBOSE_IF(condition)
#define PLOGD_IF(condition)                      PLOG_DEBUG_IF(condition)
#define PLOGI_IF(condition)                      PLOG_INFO_IF(condition)
#define PLOGW_IF(condition)                      PLOG_WARNING_IF(condition)
#define PLOGE_IF(condition)                      PLOG_ERROR_IF(condition)
#define PLOGF_IF(condition)                      PLOG_FATAL_IF(condition)
#define PLOGN_IF(condition)                      PLOG_NONE_IF(condition)

#define PLOGV_IF_(instance, condition)           PLOG_VERBOSE_IF_(instance, condition)
#define PLOGD_IF_(instance, condition)           PLOG_DEBUG_IF_(instance, condition)
#define PLOGI_IF_(instance, condition)           PLOG_INFO_IF_(instance, condition)
#define PLOGW_IF_(instance, condition)           PLOG_WARNING_IF_(instance, condition)
#define PLOGE_IF_(instance, condition)           PLOG_ERROR_IF_(instance, condition)
#define PLOGF_IF_(instance, condition)           PLOG_FATAL_IF_(instance, condition)
#define PLOGN_IF_(instance, condition)           PLOG_NONE_IF_(instance, condition)

// Old macro names for downward compatibility. To bypass including these macro names, add
// #define PLOG_OMIT_LOG_DEFINES before #include <plog/Log.h>
#ifndef PLOG_OMIT_LOG_DEFINES
//////////////////////////////////////////////////////////////////////////
// Log severity level checker

#define IF_LOG_(instance, severity)     if (!plog::get<instance>() || !plog::get<instance>()->checkSeverity(severity)) {;} else
#define IF_LOG(severity)                IF_PLOG_(PLOG_DEFAULT_INSTANCE, severity)

//////////////////////////////////////////////////////////////////////////
// Main logging macros - can be changed later to point at macros for a different logging package

#define LOG_(instance, severity)        IF_PLOG_(instance, severity) (*plog::get<instance>()) += plog::Record(severity, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS()).ref()
#define LOG(severity)                   PLOG_(PLOG_DEFAULT_INSTANCE, severity)

#define LOG_VERBOSE                     PLOG(plog::verbose)
#define LOG_DEBUG                       PLOG(plog::debug)
#define LOG_INFO                        PLOG(plog::info)
#define LOG_WARNING                     PLOG(plog::warning)
#define LOG_ERROR                       PLOG(plog::error)
#define LOG_FATAL                       PLOG(plog::fatal)
#define LOG_NONE                        PLOG(plog::none)

#define LOG_VERBOSE_(instance)          PLOG_(instance, plog::verbose)
#define LOG_DEBUG_(instance)            PLOG_(instance, plog::debug)
#define LOG_INFO_(instance)             PLOG_(instance, plog::info)
#define LOG_WARNING_(instance)          PLOG_(instance, plog::warning)
#define LOG_ERROR_(instance)            PLOG_(instance, plog::error)
#define LOG_FATAL_(instance)            PLOG_(instance, plog::fatal)
#define LOG_NONE_(instance)             PLOG_(instance, plog::none)

#define LOGV                            PLOG_VERBOSE
#define LOGD                            PLOG_DEBUG
#define LOGI                            PLOG_INFO
#define LOGW                            PLOG_WARNING
#define LOGE                            PLOG_ERROR
#define LOGF                            PLOG_FATAL
#define LOGN                            PLOG_NONE

#define LOGV_(instance)                 PLOG_VERBOSE_(instance)
#define LOGD_(instance)                 PLOG_DEBUG_(instance)
#define LOGI_(instance)                 PLOG_INFO_(instance)
#define LOGW_(instance)                 PLOG_WARNING_(instance)
#define LOGE_(instance)                 PLOG_ERROR_(instance)
#define LOGF_(instance)                 PLOG_FATAL_(instance)
#define LOGN_(instance)                 PLOG_NONE_(instance)

//////////////////////////////////////////////////////////////////////////
// Conditional logging macros

#define LOG_IF_(instance, severity, condition)  if (!(condition)) {;} else PLOG_(instance, severity)
#define LOG_IF(severity, condition)             PLOG_IF_(PLOG_DEFAULT_INSTANCE, severity, condition)

#define LOG_VERBOSE_IF(condition)               PLOG_IF(plog::verbose, condition)
#define LOG_DEBUG_IF(condition)                 PLOG_IF(plog::debug, condition)
#define LOG_INFO_IF(condition)                  PLOG_IF(plog::info, condition)
#define LOG_WARNING_IF(condition)               PLOG_IF(plog::warning, condition)
#define LOG_ERROR_IF(condition)                 PLOG_IF(plog::error, condition)
#define LOG_FATAL_IF(condition)                 PLOG_IF(plog::fatal, condition)
#define LOG_NONE_IF(condition)                  PLOG_IF(plog::none, condition)

#define LOG_VERBOSE_IF_(instance, condition)    PLOG_IF_(instance, plog::verbose, condition)
#define LOG_DEBUG_IF_(instance, condition)      PLOG_IF_(instance, plog::debug, condition)
#define LOG_INFO_IF_(instance, condition)       PLOG_IF_(instance, plog::info, condition)
#define LOG_WARNING_IF_(instance, condition)    PLOG_IF_(instance, plog::warning, condition)
#define LOG_ERROR_IF_(instance, condition)      PLOG_IF_(instance, plog::error, condition)
#define LOG_FATAL_IF_(instance, condition)      PLOG_IF_(instance, plog::fatal, condition)
#define LOG_NONE_IF_(instance, condition)       PLOG_IF_(instance, plog::none, condition)

#define LOGV_IF(condition)                      PLOG_VERBOSE_IF(condition)
#define LOGD_IF(condition)                      PLOG_DEBUG_IF(condition)
#define LOGI_IF(condition)                      PLOG_INFO_IF(condition)
#define LOGW_IF(condition)                      PLOG_WARNING_IF(condition)
#define LOGE_IF(condition)                      PLOG_ERROR_IF(condition)
#define LOGF_IF(condition)                      PLOG_FATAL_IF(condition)
#define LOGN_IF(condition)                      PLOG_NONE_IF(condition)

#define LOGV_IF_(instance, condition)           PLOG_VERBOSE_IF_(instance, condition)
#define LOGD_IF_(instance, condition)           PLOG_DEBUG_IF_(instance, condition)
#define LOGI_IF_(instance, condition)           PLOG_INFO_IF_(instance, condition)
#define LOGW_IF_(instance, condition)           PLOG_WARNING_IF_(instance, condition)
#define LOGE_IF_(instance, condition)           PLOG_ERROR_IF_(instance, condition)
#define LOGF_IF_(instance, condition)           PLOG_FATAL_IF_(instance, condition)
#define LOGN_IF_(instance, condition)           PLOG_NONE_IF_(instance, condition)
#endif
