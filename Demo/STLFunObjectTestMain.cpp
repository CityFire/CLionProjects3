//
// Created by wjc on 2023/3/28.
//
#include <iostream>
using namespace std;

class CFunObj
{
public:
    void operator()()
    {
        cout<<"hello,function object!"<<endl;
    }
    void operator()(int n)
    {
        cout<<"hello2!"<<endl;
    }
};

int CFunObjmain(void)
{
    CFunObj fo;
    fo();
    CFunObj();
    fo(1);

    CFunObj()();
    return 0;
}