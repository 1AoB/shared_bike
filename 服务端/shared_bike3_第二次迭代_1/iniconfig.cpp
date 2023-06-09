#include "iniconfig.h"
#include <iniparser/iniparser.h>

Iniconfig::Iniconfig() : _isloaded(false)
{
}

Iniconfig::~Iniconfig() {}

bool Iniconfig::loadfile(const std::string &path)
{
    dictionary *ini = NULL;

    // if (_isloaded){

    ini = iniparser_load(path.c_str());
    if (ini == NULL)
    {
        fprintf(stderr, "cannot parse file :%s \n", path.c_str());
        return false;
    }
    /*
    [database]
    ip       = 127.0.0.1 ;
    port     = 3306 ;
    user     = root ;
    pwd      = 123456 ;
    db       = shared_bike;

    [server]
    port     = 9090;
    */
    const char *ip = iniparser_getstring(ini, "database:ip", "127.0.0.1");
    int port = iniparser_getint(ini, "database:port", 3306);
    const char *user = iniparser_getstring(ini, "database:user", "root");
    const char *pwd = iniparser_getstring(ini, "database:pwd", "123456");
    const char *name = iniparser_getstring(ini, "database:db", "shared_bike");
    int sport = iniparser_getint(ini, "server:port", 9090);

    // 赋值运算符重载
    _config = st_env_config(std::string(ip), port, std::string(user),
                            std::string(pwd), std::string(name), sport);

    iniparser_freedict(ini); // 释放字典

    _isloaded = true; // 加载成功
    //}
    return _isloaded;
}

const st_env_config &Iniconfig::getconfig()
{
    return _config;
}
