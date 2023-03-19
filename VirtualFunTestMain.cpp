//
// Created by wjc on 2023/3/19.
//
#include <iostream>
using namespace std;

class VirtualBase
{
public:
    virtual void Fun1()
    {
        cout<<"VirtualBase::Fun1..."<<endl;
    }

    virtual void Fun2()
    {
        cout<<"VirtualBase::Fun2..."<<endl;
    }

    void Fun3()
    {
        cout<<"VirtualBase::Fun3..."<<endl;
    }

    VirtualBase()
    {
        cout<<"VirtualBase..."<<endl;
    }

    ~VirtualBase()
    {
        cout<<"~VirtualBase"<<endl;
    }
};

class VirtualDerived : public VirtualBase
{
public:
    /*virtual*/ void Fun1()
    {
        cout<<"VirtualDerived::Fun1..."<<endl;
    }

    virtual void Fun2()
    {
        cout<<"VirtualDerived::Fun2..."<<endl;
    }

    void Fun3()
    {
        cout<<"VirtualDerived::Fun3..."<<endl;
    }

    VirtualDerived()
    {
        cout<<"VirtualDerived..."<<endl;
    }

    ~VirtualDerived()
    {
        cout<<"~VirtualDerived"<<endl;
    }
};

int main(void)
{
//    VirtualBase* p;
//    VirtualDerived d;
//
//    p = &d;
//    p->Fun1();   // Fun1是虚函数，基类p指针指向派生类对象，调用的是派生类对象的虚函数
//    p->Fun2();
//    p->Fun3();   // Fun3非虚函数，根据p指针实际类型来调用相应类的成员函数


    VirtualBase* p;
    p = new VirtualDerived;

    p->Fun1();
    delete p;

    return 0;
}