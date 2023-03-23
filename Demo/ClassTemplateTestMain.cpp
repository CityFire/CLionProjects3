//
// Created by wjc on 2023/3/23.
//
#include <iostream>
using namespace std;
#include "Stack.h"
#include "Stack2.h"

int main(void)
{
    Stack2<int, 3> s;
    s.Push(1);
    s.Push(2);
    s.Push(3);

    while (!s.Empty())
    {
        cout<<s.Top()<<endl;
        s.Pop();
    }

    return 0;
}

int mainClassTemplate(void)
{
    Stack<int> s(10);
    s.Push(1);
    s.Push(2);
    s.Push(3);

    while (!s.Empty())
    {
        cout<<s.Top()<<endl;
        s.Pop();
    }

    return 0;
}