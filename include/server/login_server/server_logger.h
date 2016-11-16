#pragma once

#include "../../include/shared/log.h"

#ifdef NDEBUG
#  define LOG_DEBUG(_) //do {} while(0)
#else
#  define LOG_DEBUG(Message_) DETAILLOG(Logger::console(), "DEBUG: " + Message_) \
    DETAILLOG(Logger::fileServer(), "DEBUG: " + Message_)
#endif

#define LOG_INFO(Message_) LOG(Logger::console(), "INFO: " Message_) \
    LOG(Logger::fileServer(), "INFO: " Message_)
#define LOG_WARNING(Message_) LOG(Logger::console(), "WARNING: " Message_) \
    LOG(Logger::fileServer(), "WARNING: " Message_)
#define LOG_ERROR(Message_) DETAILLOG(Logger::console(), "ERROR: " Message_) \
    DETAILLOG(Logger::fileServer(), "ERROR: " Message_)
#define APP_INFO(Message_) LOG(Logger::console(), "APPINFO: " Message_) \
    LOG(Logger::fileServer(), "APPINFO: " Message_)
#define APP_ERROR(Message_)LOG(Logger::console(), "APPERROR: " Message_) \
    LOG(Logger::fileServer(), "APPERROR: " Message_)
