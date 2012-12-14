#include "Logging.h"

using namespace log4cplus;

Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));

void LogUtil::Setup(const int logLevel)
{
    //BasicConfigurator config;
    //config.configure();
	
	// Log pattern
	log4cplus::tstring pattern = LOG4CPLUS_TEXT("[%D{%Y-%m-%d %H:%M:%S.%q}] [%p]\t%m%n");
	std::auto_ptr<Layout> patternLayout = std::auto_ptr<Layout>(new PatternLayout(pattern));

	// Console appender
	ConsoleAppender* consoleAppender = new ConsoleAppender();
	consoleAppender->setLayout(patternLayout);
	logger.addAppender(SharedAppenderPtr(consoleAppender));

	logger.setLogLevel(logLevel);
}

Logger LogUtil::Get()
{
	return logger;
}