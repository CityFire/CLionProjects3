//
// Created by wjc on 2023/3/28.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

void ShowVec(const vector<int>& v)
{
    for (vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        cout<<*it<<' ';
    }
    cout<<endl;
}

void ShowList(const list<int>& v)
{
    for (list<int>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        cout<<*it<<' ';
    }
    cout<<endl;
}

int main(void)
{
    int a[] = {1, 2, 3, 4, 5};
    vector<int> v(a, a+5);

    // copy from cin to vector
    copy(istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(v));

    // copy from vector to cout
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
    cout<<endl;

    return 0;
}

int mainfront_insert_iterator(void)
{
    int a[] = {1, 2, 3, 4, 5};
    vector<int> v(a, a+5);
    //v.push_back(6);
    vector<int> v2;
    list<int> l(a, a+5);
//    list<int> l2(a, a+5);
    list<int> l2;

    front_insert_iterator<list<int>> fii(l);
    fii = 0;
    ShowList(l);

    copy(l.begin(), l.end(), front_inserter(l2));
    ShowList(l2);

    back_insert_iterator<vector<int>> bii(v);
    *bii = 6;
    ShowVec(v);

    back_insert_iterator<vector<int>> bii2(v2);
    copy(v.begin(), v.end(), bii2);
    ShowVec(v2);

    back_inserter(v) = 7;
    ShowVec(v);
    cout<<endl;

    copy(v.begin(), v.end(), back_inserter(v2));
    ShowVec(v2);

    return 0;
}