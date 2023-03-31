//
// Created by wjc on 2023/3/31.
//
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace std;

class Parent;
class Child;

typedef boost::shared_ptr<Parent> parent_ptr;
typedef boost::shared_ptr<Child> child_ptr;
class Child
{
public:
    Child() { cout<<"Child..."<<endl; }
    ~Child() { cout<<"~Child..."<<endl; }
//    parent_ptr parent_;
    boost::weak_ptr<Parent> parent_;
};

class Parent
{
public:
    Parent() { cout<<"Parent..."<<endl; }
    ~Parent() { cout<<"~Parent..."<<endl; }
//    child_ptr child_;
    boost::weak_ptr<Child> child_;  // 弱引用
};

int mainweakPtr(void)
{
    parent_ptr parent(new Parent);
    child_ptr child(new Child);
    parent->child_ = child;
    child->parent_ = parent;   // 循环引用
//    parent->child_.reset();    // 打破循环引用

    return 0;
}