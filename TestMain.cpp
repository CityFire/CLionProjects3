//
// Created by wjc on 2023/3/17.
//
#include <iostream>
#include "TestA.h"
#include "Clock.h"
#include "StringA.h"

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

class Object
{
public:
    Object(int num) : num_(num)
    {
       cout<<"Object "<<num<<"..."<<endl;
    }
    ~Object()
    {
        cout<<"~Object "<<num_<<"..."<<endl;
    }
private:
    int num_;
};

class Container
{
public:
    Container(int obj1 = 0, int obj2 = 0) : obj1_(obj1), obj2_(obj2)
    {
        cout<<"Container..."<<endl;
    }
    ~Container()
    {
        cout<<"~Container..."<<endl;
    }
private:
    Object obj1_; //error: constructor for 'Container' must explicitly initialize the member 'obj_' which does not have a default constructor Container()
    Object obj2_;
};

// const成员的初始化只能在构造函数初始化列表中进行
// 引用成员的初始化也只能在构造函数初始化列表中进行
// 对象成员（对象所对应的类没有默认构造函数）的初始化，也只能在构造函数初始化列表中进行
class SubObject
{
public:
    enum E_TYPE
    {
        TYPE_A = 100,
        TYPE_B = 200,
    };
    SubObject(int num = 0) : num_(num), kNum_(num), refNum_(num_)
    {
        //kNum_ = 100;
        //refNum_ = num;
        cout<<"Object "<<num<<"..."<<endl;
        cout<<"kNum_ "<<kNum_<<"..."<<"refNum_ "<<refNum_<<"..."<<endl;
    }
    ~SubObject()
    {
        cout<<"~Object "<<num_<<"..."<<endl;
    }
    void DisplayKNum()
    {
        cout<<"KNum="<<kNum_<<endl;
    }
private:
    int num_;
    const int kNum_;
    int& refNum_;
};

void TestFun(const TestA t)
{

}

void TestFun2(const TestA& t)
{

}

TestA TestFunc3(const TestA& t)
{
    return t;
}

const TestA& TestFunc4(const TestA& t)
{
    //return const_cast<TestA&>(t);
    return t;
}

class Empty
{
public:
//    class Empty {}; // 空类默认产生的6个成员
//    Empty() {} // 默认构造函数
//    Empty(const Empty&);//默认拷贝构造函数
//    ~Empty();  //默认析构函数
//    Empty& operator=(const Empty& other); //默认复制运算符
    Empty* operator&()  // 取址运算符
    {
        cout<<"AAAA"<<endl;
        return this;
    }

    const Empty* operator&() const // 取址运算符const
    {
        cout<<"BBBB"<<endl;
        return this;
    }
};

int main(void)
{
    // 空类默认产生的成员
    Empty e;
    Empty* p = &e;   // 等价于e.operator&();  // 取值运算符

    const Empty e2;
    const Empty* p2 = &e2;    //取值运算符const

    cout<<sizeof(Empty)<<endl; // 1

    return 0;
}

int main999(void)
{
    StringA s1("AAAA");
    s1.Display();
    StringA s2 = s1;   // 调用默认的拷贝构造函数
                       // 系统提供的默认拷贝构造函数实施的是浅拷贝 s2.str_ = s1.str_  导致释放两次 error for object 0x7f8f8dc05c20: pointer being freed was not allocated

    StringA s3;
    s3.Display();
    s3 = s2;        // 调用等号运算符  赋值操作
                      // 系统提供的默认等号运算符实施的是浅拷贝  s3.str_ = s2.str_
                      // s3.operator=(s2);

    // 要让对象是独一无二的，我们要禁止拷贝
    // 方法是将拷贝构造函数与=运算符声明为私有，并且不提供他们的实现

    return 0;
}

int main9999(void)
{
    TestA t(10);

//    TestFun(t); // 会创建 调用拷贝构造函数
//    TestFun2(t);  // 引用 共用 不会创建

    //TestFunc3(t); //临时对象 立即销毁

    //t = TestFunc3(t); //临时对象 赋值给t后立即销毁
//    Initialzing 10
//    Initlializing with other 10
//    TestA::operator=
//    Destory 10
//    ........
//    Destory 10

    //TestA t2 = TestFunc3(t);

    //TestA& t2 = TestFunc3(t); // 编译不通过

    //TestA t2 = TestFunc4(t);
    //上面都是这种结果情况
//    Initialzing 10
//    Initlializing with other 10
//    ........
//    Destory 10
//    Destory 10

    const TestA& t2 = TestFunc4(t);
//    Initialzing 10
//    ........
//    Destory 10

    cout<<"........"<<endl;



    return 0;
}

int main99(void)
{
    TestA t(10);
 //   TestA t2(t);    // 调用拷贝构造函数 默认缺省拷贝构造函数
    TestA t2 = t;     // 等价于TestA t2(t);


    return 0;
}

int main88(void)
{
    SubObject obj1(10);
    SubObject obj2(20);
    obj1.DisplayKNum();
    obj2.DisplayKNum();

    cout<<obj1.TYPE_A<<endl;
    cout<<obj2.TYPE_A<<endl;
    cout<<SubObject::TYPE_A<<endl;

    return 0;
}


int main777(void)
{
    Container c(10, 20);

    return 0;
}

int main66(void)
{
    Clock c(23, 59, 59);
    c.Display();
    c.Update();
    c.Display();
    return 0;
}

int main55(void)
{
    /*
    TestA t = 10;   // 等价于Test t(10); 这里的=不是运算符，表示初始化

    t = 20;        // 赋值操作

//    Initialzing 10
//    Initialzing 20
//    TestA::operator=
//    Destory 20
//    Destory 20

    TestA t2;
    t = t2;   // 赋值操作 t.operator=(t2)
     */

//Initialzing 10
//Initialzing 20
//TestA::operator=
//Destory 20
//Initialzing Default
//TestA::operator=
//Destory 0
//Destory 0

    TestA t = 10;
    t = 20;

    return 0;
}

int main44(void)
{
    TestA t(10); // 带一个参数的构造函数，充当的是普通构造函数的功能

    t = 20;     // 将20这个整数赋值给t对象
    // 1、调用转换构造函数将20这个整数转换成类类型（生成一个临时对象）
    // 2、将临时对象赋值给t对象（调用的是=运算符）
    TestA t2;

//    Initialzing 10
//    Initialzing 20
//    Destory 20
//    Initialzing Default
//    Destory 0
//    Destory 20
    return 0;
}

int main33(void)
{
    TestA t7;
    t7.~TestA(); //析构函数可以显式调用，但一般很少用。
    return 0;
}

//TestA t(30); //全局对象的构造先于main函数
int main22(void)
{
    cout<<"Entering main..."<<endl;
    cout<<"Exiting main..."<<endl;
}

int main11(void )
{
    TestA t;  // error: no matching constructor for initialization of 'TestA'
    t.Display();

    TestA t2(10);
    t2.Display();

    TestA* t3 = new TestA(20); // new operation
    t3->Display();

    delete t3;

    TestA t4[2] = {100, 200};

    TestA* t5 = new TestA(300);
    delete t5;

    TestA* t6 = new TestA[2]; //数组
    //delete t6; //Demo(43373,0x1185abe00) malloc: *** error for object 0x7fcbe0c058d8: pointer being freed was not allocated
    delete[] t6;

}

int main00(void)
{

    Outer o;
    o.Fun();
    o.FunStruct();

    Outer::Inner i; // note: implicitly declared private here
    i.Func();

    Fun();
    //LocalClass lc;   // Error,局部类只能在定义它的函数体中使用

    return 0;
}


