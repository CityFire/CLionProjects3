//
// Created by wjc on 2023/3/19.
//
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

typedef vector<int> INTVEC;

//void showVec(const INTVEC& v)
//{
//    unsigned  int i;
//    for (i = 0; i < v.size(); ++i) {
//        cout<<v[i]<<" ";
//    }
//    cout<<endl;
//}

void showVec(INTVEC& v)
{
//    INTVEC::iterator it;
    INTVEC::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
}

int main3455(void)
{
    INTVEC v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.push_back(3);
    cout<<v.back()<<endl;
//    v.pop_back();

    showVec(v);

//    v.erase(v.begin()+2);
//    v.erase(v.begin(), v.begin()+1);

    remove(v.begin(), v.end(), 3);
    v.erase(remove(v.begin(), v.end(), 3), v.end());

    showVec(v);

    INTVEC::iterator it;
    for (it = v.begin(); it != v.end(); /*++it*/)
    {
        if (*it == 3)
        {
            it = v.erase(it); // erase返回的是当前删除元素的下一个元素
        } else {
            ++it;
        }
    }

    showVec(v);

    return 0;
}