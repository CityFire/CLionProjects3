#include <iostream>
using namespace std;
#include "DebugNew.h"
#include "Node.h"

// 自己实现malloc，在里面进行跟踪
// valgrind、dalloc、efence
// visual leak detector

// operator new  operator delete
// new operator <=> operator new + 构造函数

// 头文件包含次序
// C库、C++库、其他库.h、项目中.h


/*
 * 虛拟内存
void fun()
{
    Test t;
}
class A
{
private:
    B b;
}
int* fun()
{
    int* p = new int;
}
void fun2()
{
    int* p2 = fun() ;
}

int* p3 =p;

p
p2
p3 int* p3 =p;
delete p;

指针所指向的资源己被释放，但是指针未置空

内存、文件句柄、套接字、互斥量

裸指针
包装器

空悬指针
内存泄露
重复释放

np3. ptr_ = np2.ptr_

栈展开的过程中不会自动释放

应用RAII技巧 将裸指针转换为智能指针（是一个类）

将指针转换成对象

对象的析构函数在生存期结束的时候是确定被调用的。
 */
int main(void) {
//    int*p = new int;
//    delete p;
//
//    int* p2 = new int[5];
//    delete[] p2;

//    Node* pNode = new Node;
    //pNode->Calc();  // 内存泄露
//    NodePtr np(pNode);
    NodePtr np(new Node);
    np->Calc(); // 需要重载->运算符

    NodePtr np2(np);   // 初始化默认拷贝构造函数 浅拷贝  ~Node... ~Node... 释放两次  指向同一个内存空间
    NodePtr np3;
    np3 = np2;   // 重载赋值运算符  // np3. ptr_ = np2.ptr_

    return 0;
}

//std::auto_ptr<>