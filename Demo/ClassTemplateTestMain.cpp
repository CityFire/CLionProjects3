//
// Created by wjc on 2023/3/23.
//
#include <iostream>
#include <vector>
using namespace std;
#include "Stack.h"
#include "Stack2.h"
#include "Stack3.h"

int main(void)
{
    Stack3<int, vector<int> > s;
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

int mainStack2(void)
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