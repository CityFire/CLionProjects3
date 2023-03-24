//
// Created by wjc on 2023/3/24.
//

#ifndef DEMO_SINGLETONTEMPLATE_H
#define DEMO_SINGLETONTEMPLATE_H

#include <cstdlib>
template <typename T>
class SingletonTemplate
{
public:
    static T& GetInstance()
    {
//        static T instance;    // 局部静态对象
//        return instance;

//        if (instance_ == 0)
//            instance_ = new T;

        Init();
        return *instance_;
    }
private:
    static void Init()
    {
        if (instance_ == 0)
        {
            instance_ = new T;
            atexit(Destroy);
        }
    }
    static void Destroy()
    {
        delete instance_;
    }
    SingletonTemplate(const SingletonTemplate& other);
    SingletonTemplate& operator=(const SingletonTemplate& other);
    SingletonTemplate();
    ~SingletonTemplate();

    static T* instance_;
};

template <typename T>
T* SingletonTemplate<T>::instance_ = 0;

#endif //DEMO_SINGLETONTEMPLATE_H
