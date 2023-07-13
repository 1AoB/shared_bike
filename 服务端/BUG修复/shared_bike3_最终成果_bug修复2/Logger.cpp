// Logger.cpp
#include "Logger.h"

#include <iostream>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RemoteSyslogAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>

// 外部的static成员必须声明一下,不声明就会报错!!!
Logger Logger::instance_;

bool Logger::init(const std::string &log_conf_file)
{
    try
    {
        log4cpp::PropertyConfigurator::configure(log_conf_file);
    }
    catch (log4cpp::ConfigureFailure &f)
    {
        std::cerr << " load log config file " << log_conf_file.c_str() << " failed with result : " << f.what() << std::endl;
        return false;
    }

    category_ = &log4cpp::Category::getRoot(); // 输出日志category_

    return true;
}