#ifndef SHBK_COMMON_INICONGIG_H_
#define SHBK_COMMON_INICONGIG_H_
#include <string>
#include "configdef.h"
class Iniconfig
{
private:
    /* data */
    st_env_config _config;
    bool _isloaded;

public:
    Iniconfig(/* args */);
    ~Iniconfig();
    bool loadfile(const std::string &path);
    const st_env_config &getconfig();
};

#endif
