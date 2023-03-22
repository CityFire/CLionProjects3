//
// Created by wjc on 2023/3/22.
//

#include <iostream>
using namespace std;

// 两个指针相减，得到的是相隔几个元素
#define sizeof_v(x) (char*)(&x+1) - (char*)(&x)

#define sizeof_t(t) ((size_t)((t*)0 + 1))
// 对齐
#define ALIGN(v, b) ((v+b-1) & ~(b-1))

/*
   0011
   1111
  10000
   0000

  10000 = 16

  原理、思想

  某个数要对齐到16的整数倍    0000     32的整数倍00000
  1000

  向上对齐  3 + 15
  超出的部分我们要把它抹除掉（低4倍都置0）

  0
  ALIGN(0, 16) = 0
  ALIGN(16,16) =

  10000
   1111
  11111

  内存池
  3 7 9
  内存池中 内存块大小是规则的
  产生空隙 碎片

  */

class Empty
{

};

class Noncopyable
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}
private:
    Noncopyable(const Noncopyable&);
    const Noncopyable& operator=(const Noncopyable&);
};

class Parent : private  Noncopyable
{
public:
    Parent() : Noncopyable()
    {
        cout<<"Parent()..."<<endl;
    }
    Parent(const Parent& other)// : Noncopyable(other)   // 拷贝构造函数不规范引起 使用默认拷贝构造函数也会调用基类的拷贝构造函数
    // 不写上: Noncopyable(other) 这个 是不会调用基类拷贝构造函数的
    {
       cout<<"Parent(const Parent& other)..."<<endl;
    }
    ~Parent()
    {
        cout<<"~Parent()..."<<endl;
    }
};

class Child : public  Parent
{
public:
    Child() : Parent()
    {
        cout<<"Child()..."<<endl;
    }
    Child(const Child& other) : Parent(other)
    {
       cout<<"Child(const Child& other)..."<<endl;
    }
    ~Child()
    {
        cout<<"~Child()..."<<endl;
    }
};

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


    //////////////
//    Parent p1;
//    Parent p2(p1);  // 要调用Parent拷贝构造函数，Parent构造函数又调用Noncopyable的拷贝构造函数

    Child c1;
    Child c2(c1);



    /////////////
    Empty e;
    int n;
//    cout<<sizeof(e)<<endl;       // 1
//    cout<<sizeof(Empty)<<endl;   // 1

    cout<<sizeof_v(e)<<endl;
    cout<<sizeof_v(n)<<endl;
    cout<<sizeof_t(Empty)<<endl;
    cout<<sizeof_t(int)<<endl;


    cout<<ALIGN(3, 16)<<endl;
    cout<<ALIGN(31, 16)<<endl;
    cout<<ALIGN(0, 16)<<endl;
    cout<<ALIGN(4198, 4096)<<endl;

    return 0;
}