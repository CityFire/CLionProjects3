//
// Created by wjc on 2023/3/22.
//

#include <iostream>
using namespace std;

class SingletonAutoPtr
{
public:
    static SingletonAutoPtr* GetInstance()
    {
        if (!instance_.get())
        {
            instance_ = auto_ptr<SingletonAutoPtr>(new SingletonAutoPtr);
        }
        return instance_.get();
    }
    ~SingletonAutoPtr()
    {
        cout<<"~SingletonAutoPtr..."<<endl;
    }
//    static void Free()
//    {
//        if (instance_ != NULL)
//        {
//            delete instance_;
//        }
//    }
private:
    // 禁止拷贝
    SingletonAutoPtr(const SingletonAutoPtr& other);
    SingletonAutoPtr& operator=(const SingletonAutoPtr& other);
    // 将构造函数说明为私有的
    SingletonAutoPtr()
    {
        cout<<"SingletonAutoPtr..."<<endl;
    }
    static auto_ptr<SingletonAutoPtr> instance_;
};

auto_ptr<SingletonAutoPtr> SingletonAutoPtr::instance_;

int main(void)
{
//    SingletonAutoPtr s1;
//    SingletonAutoPtr s2;

    //auto_ptr //智能指针

    SingletonAutoPtr* s1 = SingletonAutoPtr::GetInstance();
    SingletonAutoPtr* s2 = SingletonAutoPtr::GetInstance();

//    SingletonAutoPtr s3(*s1);  // 调用拷贝构造函数

//    SingletonAutoPtr s3 = *s1;

//    SingletonAutoPtr::Free();

    return 0;
}