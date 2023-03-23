//
// Created by wjc on 2023/3/24.
//

#ifndef DEMO_STACK_H
#define DEMO_STACK_H

#include <exception>

/*
 *
 * 类模板
   类模板:将类定义中的数据类型参数化
   类模板实际上是函数模板的推广，可以用相同的类模板来组建任意类型的对象集合

类模板的定义
template <类型形参表>
class <类名>
{  //类说明体 };
template <类型形参表>
<返回类型><类名><类型名表>::<成员函数1> (形参表)
{   //成员函数定义体   }
template <类型形参表>
<返回类型><类名><类型名表>::<成员函数2> (形参表)
{  //成员函数定义体  }
...
template <类型形参表>
<返回类型><类名><类型名表>::<成员函数n> (形参表)
{   //成员函数定义体   }
*/

template <typename T>
class Stack
{
public:
    explicit Stack(int maxSize);
    ~Stack();

    void Push(const T& elem);
    void Pop();
    T& Top();
    const T& Top() const;
    bool Empty() const;
private:
    T* elems_;
    int maxSize_;
    int top_;
};

template <typename T>
Stack<T>::Stack(int maxSize) : maxSize_(maxSize), top_(-1)
{
    elems_ = new T[maxSize_];
}

template <typename T>
Stack<T>::~Stack()
{
    delete[] elems_;
}

template <typename T>
void Stack<T>::Push(const T &elem)
{
    if (top_ + 1 >= maxSize_)
        throw out_of_range("Stack<>::Push() stack full");

    elems_[++top_] = elem;
}

template <typename T>
void Stack<T>::Pop()
{
    if (top_ + 1 == 0)
        throw out_of_range("Stack<>::Push() stack empty");

    --top_;
}

template <typename T>
T& Stack<T>::Top()
{
    if (top_ + 1 == 0)
        throw out_of_range("Stack<>::Push() stack empty");

    return elems_[top_];
}

template <typename T>
bool Stack<T>::Empty() const
{
    return top_ + 1 == 0;
}

#endif //DEMO_STACK_H
