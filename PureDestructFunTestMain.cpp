//
// Created by wjc on 2023/3/20.
//

#include <iostream>
using namespace std;

// 对于一个没有任何接口的类，如果想要将它定义成抽象类，只能将虚析构函数声明为纯虚的
// 通常情况下在基类中纯虚函数不需要实现
// 例外是纯虚析构函数要给出实现。（给出一个空的实现即可）
class DBase
{
public:
//    void Test() = 0;
//    virtual ~DBase() = 0;
    virtual ~DBase()
    {

    }
};

class DDerived : public DBase
{

};

int main887544(void)
{
    DDerived d;

    return 0;
}