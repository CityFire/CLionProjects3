//
// Created by wjc on 2023/3/26.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

void print_element(int n)
{
    cout<<n<<' ';
}

void add_3(int& n)
{
    n += 3;
}

bool greater_than_3(int n)
{
    return n > 3;
}

// 变动性算法
int main(void)
{
    int a[] = { 1, 2, 3, 4, 5 };
    vector<int> v(a, a+5);
    list<int> l(5);

    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    for_each(v.begin(), v.end(), add_3);

    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    for_each(l.begin(), l.end(), print_element);
    cout<<endl;

    copy(v.begin(), v.end(), l.begin());
    for_each(l.begin(), l.end(), print_element);
    cout<<endl;

    copy_backward(v.begin(), v.end(), l.end());
    for_each(l.begin(), l.end(), print_element);
    cout<<endl;

    return 0;
}

// 非变动性算法
int mainAlgorithm(void)
{
    int a[] = { 1, 2, 3, 4, 5 };
    vector<int> v(a, a+5);

    vector<int>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        cout<<*it<<' ';
    }
    cout<<endl;
// 非变动性算法
    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    for (it = v.begin(); it != v.end(); ++it)
    {
        cout<<*it<<' ';
    }

    cout<<endl;

    it = min_element(v.begin(), v.end());
    if (it != v.end())
    {
        cout<<*it<<endl;
    }
    it = max_element(v.begin(), v.end());
    if (it != v.end())
    {
        cout<<*it<<endl;
    }
    it = find(v.begin(), v.end(), 3);
    if (it != v.end())
    {
        cout<<it-v.begin()<<endl;
    }
    else
    {
        cout<<"not found"<<endl;
    }

    it = find_if(v.begin(), v.end(), greater_than_3);
    if (it != v.end())
    {
        cout<<it-v.begin()<<endl;
    }
    else
    {
        cout<<"not found"<<endl;
    }

    return 0;
}