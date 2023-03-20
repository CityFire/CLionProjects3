//
// Created by wjc on 2023/3/19.
//
#include <iostream>
using namespace std;

class VirtualTBase
{
public:
    virtual void Fun1()
    {
        cout<<"VirtualTBase::Fun1..."<<endl;
    }

    virtual void Fun2()
    {
        cout<<"VirtualTBase::Fun2..."<<endl;
    }

//    VirtualTBase()
//    {
//        cout<<"VirtualTBase..."<<endl;
//    }
//
//    // 如果一个类要作为多态基类，要将析构函数定义成虚函数
//    virtual ~VirtualTBase()
//    {
//        cout<<"~VirtualTBase"<<endl;
//    }
    int data1_;
};

class VirtualTDerived : public VirtualTBase
{
public:

    void Fun2()
    {
        cout<<"VirtualTDerived::Fun2..."<<endl;
    }

//    VirtualTDerived()
//    {
//        cout<<"VirtualTDerived..."<<endl;
//    }
//
//    ~VirtualTDerived()
//    {
//        cout<<"~VirtualTDerived"<<endl;
//    }
    int data2_;
};

typedef void (*FUNC)();
int maindde(void)
{
    cout<< sizeof(VirtualTBase)<<endl;
    cout<< sizeof(VirtualTDerived)<<endl;

    VirtualTBase* b;
    long** p = (long**)&b;
    FUNC fun = (FUNC)p[0][0];
    fun();
    fun = (FUNC)p[0][1];
    fun();

    return 0;
}