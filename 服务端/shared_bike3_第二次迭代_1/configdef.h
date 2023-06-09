#ifndef SHBK_COMMON_CONFIGDEF_H_
#define SHBK_COMMON_CONFIGDEF_H_

#include <string>
typedef struct st_env_config
{
    // 数据库的配置
    std::string db_ip;
    unsigned short db_port;
    std::string db_user;
    std::string db_pwd;
    std::string db_name;

    // 服务器的配置
    unsigned short svr_port;

    // 默认构造函数
    st_env_config(){};
    // 自定义的构造函数
    st_env_config(const std::string &db_ip, unsigned int db_port, const std::string &db_user,
                  const std::string &db_pwd, const std::string &db_name, unsigned short svr_port)
    {
        this->db_ip = db_ip;
        this->db_port = db_port;
        this->db_user = db_user;
        this->db_pwd = db_pwd;
        this->db_name = db_name;
        this->svr_port = svr_port;
    };

    st_env_config &operator=(const st_env_config &config)
    {
        if (this != &config) // 自己赋值给自己不允许
        {
            this->db_ip = config.db_ip;
            this->db_port = config.db_port;
            this->db_user = config.db_user;
            this->db_pwd = config.db_pwd;
            this->db_name = config.db_name;
            this->svr_port = config.svr_port;
        }
        return *this;
    }
} _st_env_config;

/*bool Iniconfig::loadfile(const std::string &path)
{
    dictionary *ini = NULL;

    ini = iniparser_load(path.c_str());
    if (ini == NULL)
    {
        LOG_ERROR("cannot parse file: %s\n", path.c_str());
        return false;
    }

    char *ip = iniparser_getstring(ini, "database:ip", "127.0.0.1");
    int port = iniparser_getint(ini, "database:port", 3306);
    char *user = iniparser_getstring(ini, "database:user", "root");
    char *pwd = iniparser_getstring(ini, "database:pwd", "123456");
    char *db = iniparser_getstring(ini, "database:db", "dongnaobike");
    int sport = iniparser_getint(ini, "server:port", 9090);

    _config = st_env_config(std::string(ip), port, std::string(user),
                            std::string(pwd), std::string(db), sport);

    iniparser_freedict(ini);

    _isloaded = true;

    return true;
}*/
#endif