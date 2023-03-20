//
// Created by wjc on 2023/3/19.
//
#include <iostream>
using namespace std;

class Base
{
public:
    Base() : x_(0)
    {
       cout<<"Base()..."<<endl;
    }
    ~Base()
    {
       cout<<"~Base()..."<<endl;
    }
    int GetBaseX() const
    {
        cout<<"GetBaseX()..."<<endl;
        return x_;
    }
    void Show()
    {
        cout<<"Base::Show..."<<endl;
    }
    virtual void Display()
    {
        cout<<"Base::Display..."<<endl;
    }
    int x_;
protected:
    int y_;
private:
    int z_;
};

class PublicInherit : public Base
{
public:
    void Test()
    {
        x_ = 10;
        y_ = 20;
        //z_ = 30;
    }
private:
    int a_;
};

class PublicPublicInherit : public PublicInherit
{
public:
    void Test()
    {
        y_ = 20;
    }
};

class PrivateInherit: private Base
{
public:
    void Test()
    {
        x_ = 10;
        y_ = 20;
        //z_ = 30;
    }
};

class Derived : public Base
{
public:
    Derived() : x_(0)
    {
        cout<<"Derived()..."<<endl;
    }
    ~Derived()
    {
        cout<<"~Derived()..."<<endl;
    }
    int GetDerivedX() const
    {
        cout<<"GetDerivedX()..."<<endl;
        return x_;
    }
    void Show(int n)    // overload 重载 作用域相同
    {
        cout<<"Derived::Show..."<<n<<endl;
    }
    void Show()   // overwrite 对基类成员函数的重定义分为两种 1.与基类完全相同  2.与基类成员函数名相同，参数不同
    {
        cout<<"Derived::Show..."<<endl;
    }
    virtual void Display()  // overried 覆盖 需要虚函数virtual
    {
        cout<<"Dervied::Display..."<<endl;
    }
    int x_;
};

int main99999(void)
{
//    Base b;
//    b.x_ = 100;
//    //b.y_ = 20;
//
//    PublicInherit pub;
//    pub.x_ = 20;
//
//    PrivateInherit pi;
    //pi.x_ = 10;

    Derived d;
    d.x_ = 200;
    d.Base::x_ = 100;
    cout<<d.GetBaseX()<<endl;
    cout<<d.GetDerivedX()<<endl;
    d.Show();
    d.Base::Show();
    d.Display();
    d.Base::Display();

    return 0;
}