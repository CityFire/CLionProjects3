//
// Created by wjc on 2023/3/22.
//

#include <iostream>
using namespace std;

class Singleton
{
public:
    static Singleton* GetInstance()
    {
        if (instance_ == NULL)
        {
            instance_ = new Singleton;
        }
        return instance_;
    }
    ~Singleton()
    {
        cout<<"~Singleton..."<<endl;
    }
//    static void Free()
//    {
//        if (instance_ != NULL)
//        {
//            delete instance_;
//        }
//    }
    class Garbo
    {
    public:
        ~Garbo()
        {
            if (Singleton::instance_ != NULL)
            {
                delete instance_;
            }
        }
    };
private:
    Singleton(const Singleton& other);
    Singleton& operator=(const Singleton& other);
    Singleton()
    {
        cout<<"Singleton..."<<endl;
    }
    static Singleton* instance_;

    static Garbo garbo_; // 利用对象的确定性析构
};

Singleton::Garbo Singleton::garbo_;
Singleton* Singleton::instance_;

int Singletonmain(void)
{
//    Singleton s1;
//    Singleton s2;

    //auto_ptr //智能指针

    Singleton* s1 = Singleton::GetInstance();
    Singleton* s2 = Singleton::GetInstance();

//    Singleton s3(*s1);  // 调用拷贝构造函数

//    Singleton s3 = *s1;

//    Singleton::Free();

    return 0;
}