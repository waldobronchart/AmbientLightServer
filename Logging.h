#pragma once

#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using namespace log4cplus;

class LogUtil
{
public:
	static void Setup(const int logLevel);
	static Logger Get();
};


#define LOG_TRACE(text) LOG4CPLUS_TRACE(LogUtil::Get(), text)
#define LOG_DEBUG(text) LOG4CPLUS_DEBUG(LogUtil::Get(), text)
#define LOG_INFO(text) LOG4CPLUS_INFO(LogUtil::Get(), text)
#define LOG_WARN(text) LOG4CPLUS_WARN(LogUtil::Get(), text)
#define LOG_ERROR(text) LOG4CPLUS_ERROR(LogUtil::Get(), text)
#define LOG_FATAL(text) LOG4CPLUS_FATAL(LogUtil::Get(), text)