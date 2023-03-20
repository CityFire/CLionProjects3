//
// Created by wjc on 2023/3/20.
//

#ifndef DEMO_DYNAMICBASE_H
#define DEMO_DYNAMICBASE_H

#include <map>
#include <string>
using namespace std;

typedef void* (*CREATE_FUNC)();  // 函数指针

class DynObjcetFactory
{
public:
    static void* CreateObject(const string& name)
    {
        map<string, CREATE_FUNC>::const_iterator it;
        it = mapCls_.find(name);
        if (it == mapCls_.end())
            return 0;
        else
            return it->second();
    }

    static void Register(const string& name, CREATE_FUNC func)
    {
        mapCls_[name] = func;
    }
private:
    static map<string, CREATE_FUNC> mapCls_;  // 声明
};

// g++
// __attribute((weak))
//__declspec(selectany) map<string, CREATE_FUNC> DynObjcetFactory::mapCls_;
__attribute((weak)) map<string, CREATE_FUNC> DynObjcetFactory::mapCls_;  // 防止多次头文件导入多次引用  静态成员的定义性说明

class Register
{
public:
    Register(const string& name, CREATE_FUNC func)
    {
        DynObjcetFactory::Register(name, func);
    }
};

#define REGISTER_CLASS(class_name) \
class class_name##Register { \
public: \
    static void* NewInstance() \
    { \
        return new class_name; \
    } \
private: \
    static Register reg_; \
}; \
Register class_name##Register::reg_(#class_name, class_name##Register::NewInstance)
//CircleRegister

#endif //DEMO_DYNAMICBASE_H
