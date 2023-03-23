//
// Created by wjc on 2023/3/24.
//

#ifndef DEMO_STACK3_H
#define DEMO_STACK3_H

#include <exception>
#include <deque>
using namespace std;

template <typename T, typename CONT = deque<T> >
class Stack3
{
public:
    Stack3() : c_()
    {
    }

    ~Stack3()
    {
    }

    void Push(const T &elem)
    {
        c_.push_back(elem);
    }

    void Pop()
    {
        c_.pop_back();
    }

    T& Top()
    {
        return c_.back();
    }

    const T& Top() const
    {
        return c_.back();
    }

    bool Empty() const
    {
        return c_.empty();
    }

private:
    CONT c_;
    int top_;
};


#endif //DEMO_STACK3_H
