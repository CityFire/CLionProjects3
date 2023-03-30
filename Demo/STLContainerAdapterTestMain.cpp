//
// Created by wjc on 2023/3/29.
//
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <queue>
#include <functional>
#include <set>

using namespace std;

int mainconst_iterator(void)
{
    int a[] = {3, 1, 2, 3, 4};
    vector<int> v(a, a+5);
//    for (vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
//    {
//        if (*it == 3)
//            v.erase(it);
//        else
//            cout<<*it<<' ';
//    }

    for (vector<int>::const_iterator it = v.begin(); it != v.end(); )
    {
        if (*it == 3)
            it = v.erase(it);
        else
        {
            cout<<*it<<' ';
            ++it;
        }
    }

    cout<<endl;

    return 0;
}

int mainmultiset(void)
{
    multiset<int, greater<int>> s;
    s.insert(3);
    s.insert(1);
    s.insert(2);
    s.insert(3);

    for (multiset<int, greater<int>>::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        cout<<*it<<' ';
    }
    cout<<endl;

//    3 3 2 1

    return 0;
}

int mainset02(void)
{
    set<int, greater<int>> s;
    s.insert(3);
    s.insert(1);
    s.insert(2);
    s.insert(3);

    for (set<int, greater<int>>::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        cout<<*it<<' ';
    }
    cout<<endl;

//    3 2 1

    return 0;
}

int mainset01(void)
{
    set<int> s;
    s.insert(3);
    s.insert(1);
    s.insert(2);

    for (set<int>::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        cout<<*it<<' ';
    }
    cout<<endl;
    return 0;
}

int mainmake_heap02(void) {
    int a[] = {5, 1, 2, 4, 3};
    make_heap(a, a + 5, less<int>());

    copy(a, a + 5, ostream_iterator<int>(cout, " "));
    cout << endl;

    sort_heap(a, a + 5, less<int>());
    copy(a, a + 5, ostream_iterator<int>(cout, " "));
    cout << endl;

//    5 4 2 1 3
//    1 2 3 4 5

    return 0;
}

int mainmake_heap01(void)
{
    int a[] = {5, 1, 2, 4, 3};
    make_heap(a, a + 5, greater<int>());

    copy(a, a+5, ostream_iterator<int>(cout, " "));
    cout<<endl;

    sort(a, a+5);
    copy(a, a+5, ostream_iterator<int>(cout, " "));
    cout<<endl;

    sort(a, a+5, greater<int>());
    copy(a, a+5, ostream_iterator<int>(cout, " "));
    cout<<endl;

//    1 3 2 4 5
//    1 2 3 4 5
//    5 4 3 2 1

    return 0;
}

int mainpriority_queue02(void)
{
    int a[] = {5, 1, 2, 4, 3};
    priority_queue<int, vector<int>, greater<int>> q(a, a + 5);

    while (!q.empty())
    {
        cout<<q.top()<<' ';
        q.pop();
    }
    cout<<endl;

    return 0;
}

int mainpriority_queue01(void)
{
    int a[] = {5, 1, 2, 4, 3};
    priority_queue<int> q(a, a + 5);

    while (!q.empty())
    {
        cout<<q.top()<<' ';
        q.pop();
    }
    cout<<endl;

    return 0;
}

int mainqueue(void)
{
    queue<int> q;
    for (int i = 0; i < 5; ++i) {
        q.push(i);
    }

    while (!q.empty())
    {
        cout<<q.front()<<' ';
        q.pop();
    }
    cout<<endl;

    return 0;
}

int mainstack02(void)
{
    stack<int, list<int>> s;
    for (int i = 0; i < 5; i++)
    {
        s.push(i);
    }

    for (stack<int, list<int>>::size_type i = 0; i < s.size(); i++)
    {
        cout<<s.top()<<' ';
        s.pop();
    }
    cout<<endl;
    return 0;
}

int mainstack01(void)
{
    stack<int> s;
    for (int i = 0; i < 5; i++)
    {
        s.push(i);
    }

    while (!s.empty())
    {
        cout<<s.top()<<' ';
        s.pop();
    }
    cout<<endl;
    return 0;
}