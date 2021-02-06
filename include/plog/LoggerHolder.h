#pragma once
#include <plog/Logger.h>

#ifdef PLOG_DEFAULT_INSTANCE // for backward compatibility
#   define PLOG_DEFAULT_INSTANCE_ID PLOG_DEFAULT_INSTANCE
#endif

#ifndef PLOG_DEFAULT_INSTANCE_ID
#   define PLOG_DEFAULT_INSTANCE_ID 0
#endif

namespace plog
{
    template<int instanceId>
#ifdef _WIN32
    struct LoggerHolder
#else
    struct PLOG_LINKAGE LoggerHolder
#endif
    {
        static Logger* m_logger;
    };

    template<int instanceId>
    Logger* LoggerHolder<instanceId>::m_logger = NULL;

    template<int instanceId>
    struct PLOG_LINKAGE LoggerHolderAccessor
    {
        static Logger* get()
        {
            return LoggerHolder<instanceId>::m_logger;
        }

        static void set(Logger* logger)
        {
            LoggerHolder<instanceId>::m_logger = logger;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Static instance getters

    template<int instanceId>
    inline Logger* get()
    {
        return LoggerHolderAccessor<instanceId>::get();
    }

    inline Logger* get()
    {
        return get<PLOG_DEFAULT_INSTANCE_ID>();
    }

    //////////////////////////////////////////////////////////////////////////
    // Dynamic instance getters

    inline plog::Logger* get(plog::Logger* logger)
    {
        return logger;
    }

    template<class T>
    inline plog::Logger* get(T* t)
    {
        return get(t->get());
    }

    template<class T>
    inline plog::Logger* get(T& t)
    {
        return get(t.get());
    }
}
