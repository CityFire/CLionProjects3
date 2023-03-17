//
// Created by wjc on 2023/3/17.
//
#include <iostream>
#include "TestA.h"
using namespace std;

class Outer
{
    struct InnerStruct
    {
        void Func()
        {
            cout<<"InnerStruct::Func..."<<endl;
        }
    };
public:
    class Inner
    {
    public:
        void Func();
//        {
//            cout<<"Inner::Fun..."<<endl;
//        }
    };
public:
    Inner obj_;
    InnerStruct* objStruct_;
    void Fun()
    {
        cout<<"Outer::Fun..."<<endl;
        obj_.Func();
    }
    void FunStruct()
    {
        cout<<"Outer::FunStruct..."<<endl;
        objStruct_->Func();
    }
};

//void Inner::Func() // 嵌套类 从作用域的角度看，嵌套类被隐藏在外围类之中，该类名只能在外围类中使用。如果在外围类的作用域使用该类名时，需要加名字限定。
// 嵌套类中的成员函数可以在它的类体外定义。
// 嵌套类的成员函数对外围类的成员没有访问权，反之亦然。
// 嵌套类仅仅只是语法上的嵌入。
void Outer::Inner::Func()
{
    cout<<"Inner::Fun..."<<endl;
}

void Fun()
{
    class LocalClass
    {
    public:
        int num_;
        void Init(int num)
        {
            num_ = num;
        }
        void Display()
        {
            cout<<"num="<<num_<<endl;
        }

        //static int num2_;//error: static data member 'num2_' not allowed in local class 'LocalClass'
        //static int num2_; //局部类中不能有静态成员
    };
    LocalClass lc;
    lc.Init(10);
    lc.Display();
}

//TestA t(30); //全局对象先于main函数
int main(void)
{
    cout<<"Entering main..."<<endl;
//    Outer o;
//    o.Fun();
//    o.FunStruct();
//
//    Outer::Inner i; // note: implicitly declared private here
//    i.Func();
//
//    Fun();
    //LocalClass lc;   // Error,局部类只能在定义它的函数体中使用

//    TestA t;  // error: no matching constructor for initialization of 'TestA'
//    t.Display();
//
//    TestA t2(10);
//    t2.Display();

//    TestA* t3 = new TestA(20); // new operation
//    t3->Display();
//
//    delete t3;
//
//    TestA t4[2] = {100, 200};
//
//    TestA* t5 = new TestA(300);
//    delete t5;
//
//    TestA* t6 = new TestA[2]; //数组
//    //delete t6; //Demo(43373,0x1185abe00) malloc: *** error for object 0x7fcbe0c058d8: pointer being freed was not allocated
//    delete[] t6;

    TestA t7;
    t7.~TestA(); //析构函数可以显式调用，但一般很少用。

    cout<<"Exiting main..."<<endl;

    return 0;
}


