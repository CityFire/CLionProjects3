//
// Created by wjc on 2023/3/19.
//
#include <iostream>
using namespace std;

class Manager; ////////////////////  基类对象转化派生类对象
class Employee
{
public:
    Employee(const string& name, const int age, const int deptno) : name_(name), age_(age), deptno_(deptno)
    {

    }
    ////////////////////  基类对象转化派生类对象
    operator Manager();
private:
    string name_;
    int age_;
    int deptno_;
};

// 派生类到基类的转换
// 当派生类以public方式继承基类时,编译器可自动执行的转换(向上转型upcasting 安全转换）
   // 派生类对象指针自动转化为基类对象指针
   // 派生类对象引用自动转化为基类对象引用
   // 派生类对象自动转换为基类对象(特有的成员消失）
class Manager : public Employee
{
public:
    Manager(const string& name, const int age, const int deptno, int level)
          : Employee(name, age, deptno), level_(level)
    {

    }
    // 从语法上来演示基类对象可以转化为派生类对象，但是没有意义
    Manager(const Manager& other) : Employee(other), level_(-1)
    {

    }
private:
    int level_;
};

// 当派生类以private/protect方式继承基类时
   // 派生类对象指针（引用）转化为基类对象指针（引用）需用强制类型转化，但不能用static_cast，要用reinterpret_cast
   // 不能把派生类对象强制转换为基类对象
class Manager2 : private Employee
{
public:
    Manager2(const string& name, const int age, const int deptno, int level)
          : Employee(name, age, deptno), level_(level)
    {

    }
private:
    int level_;
};

////////////////////  基类对象转化派生类对象
// // 仅仅只是从语法上来演示基类对象可以转化为派生类对象，但是没有意义
Employee::operator Manager() {
    return Manager(name_, age_, deptno_, -1);
}

int main(void)
{
    Employee e1("zhangsan", 20, 10330);
    Manager m1("lisi", 30, 10031, 12);
    Manager2 m2("wangwu", 38, 10090, 10);

    Employee* pe;
    Manager* pm;
    Manager2* pm2;

    pe = &e1;
    pm = &m1;
    pm2 = &m2;

    pe = &m1;   // 派生类对象指针可以转化为基类对象指针。将派生类对象看成基类对象
    //pm = &e1; // 基类对象指针无法转化为派生类对象指针。无法将基类对象看成是派生类对象
    e1 = m1;    // 派生类对象可以转化为基类对象。将派生类对象看成基类对象
                // 会产生对象切割（派生类特有成员消失）。object slicing

    //pe = pm2;   // 私有或保护继承的时候，派生类对象指针不可以自动转化为基类对象指针
    pe = reinterpret_cast<Employee*>(pm2);

    //e1 = m2;   // 私有或保护继承的时候，派生类对象不可以自动转化为基类对象
    //e1 = reinterpret_cast<Employee>(m2);  // 私有或保护继承的时候，派生类对象无法强制转化为基类对象

    pm = static_cast<Manager*>(pe);  //基类指针可以强制转化为派生类指针，但是不安全
    //pm->level_没有的
    //m1 = reinterpret_cast<Manager>e1;  // 基类对象无法强制转化为派生类对象

    ////////////////////

    m1 = e1;

    return 0;
}


class ObjectB
{
public:
    ObjectB(int objb) : objb_(objb)
    {
        cout<<"ObjectB()..."<<endl;
    }
    ~ObjectB()
    {
        cout<<"~ObjectB()..."<<endl;
    }
    int objb_;
};

class ObjectD
{
public:
    ObjectD(int objd) : objd_(objd)
    {
        cout<<"ObjectD()..."<<endl;
    }
    ~ObjectD()
    {
        cout<<"~ObjectD()..."<<endl;
    }
    int objd_;
};

class Base
{
public:
    Base(int b) : b_(b), objb_(222)
    {
        cout<<"Base()..."<<endl;
    }
    Base(const Base& other) : objb_(other.objb_), b_(other.b_)//, b_(other.b_)
    {
        cout<<"Base(const Base& other)..."<<endl;
    }
    ~Base()
    {
        cout<<"~Base()..."<<endl;
    }
    int b_;
    ObjectB objb_;
};


class Derived : public Base
{
public:
    Derived(int b, int d) : d_(d), Base(b), objd_(111) // 对继承来的基类成员的初始化（调用基类构造函数完成）派生类的构造函数需要给基类的构造函数传递参数
    {
        cout<<"Derived()..."<<endl;
    }
    Derived(const Derived& other) : d_(other.d_), objd_(other.objd_), Base(other)
    {

    }
    ~Derived()
    {
        cout<<"~Derived()..."<<endl;
    }

    int d_;
    ObjectD objd_;
};

int main87533e(void)
{

    Derived d(100, 10);
    cout<<d.b_<<" "<<d.d_<<endl;

    Base b1(100);
    Base b2(b1);
    cout<<b2.b_<<endl;

    Derived d2(d);

    return 0;
}