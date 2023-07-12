#ifndef SHBK_COMMON_INICONGIG_H_
#define SHBK_COMMON_INICONGIG_H_
#include <string>
#include "configdef.h"
class Iniconfig
{
protected:
    Iniconfig();//和他没有关系的类是无法调用它的
public:
    ~Iniconfig();
    bool loadfile(const std::string& path);
    const st_env_config& getconfig();
    static Iniconfig* getInstance();//单例
private:
    /* data */
    st_env_config _config;
    bool _isloaded;
    static Iniconfig* config;//单例


};

#endif
