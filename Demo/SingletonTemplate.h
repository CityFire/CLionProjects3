//
// Created by wjc on 2023/3/24.
//

#ifndef DEMO_SINGLETONTEMPLATE_H
#define DEMO_SINGLETONTEMPLATE_H

template <typename T>
class SingletonTemplate
{
public:
    static T& GetInstance()
    {
        static T instance;    // 局部静态对象
        return instance;
    }
private:
    SingletonTemplate(const SingletonTemplate& other);
    SingletonTemplate& operator=(const SingletonTemplate& other);
    SingletonTemplate();
    ~SingletonTemplate();

    static T* instance_;
};

#endif //DEMO_SINGLETONTEMPLATE_H
