#ifndef DISTRIBUTED_LOGGER_H_
#define DISTRIBUTED_LOGGER_H_

#include <string>
#include <log4cpp/Category.hh>

class Logger
{
public:
    // 传log的配置文件
    bool init(const std::string &log_conf_file);
    // 返回单例
    static Logger *instance()
    {
        return &instance_;
    }

    log4cpp::Category *GetHandle()
    {
        return category_;
    }

protected:
    // 单例
    static Logger instance_;
    // 利用category输出日志
    log4cpp::Category *category_;
};

// 进行宏的定义，后续方便输出
#define LOG_INFO Logger::instance()->GetHandle()->info
#define LOG_DEBUG Logger::instance()->GetHandle()->debug
#define LOG_ERROR Logger::instance()->GetHandle()->error
#define LOG_WARN Logger::instance()->GetHandle()->warn

#endif
