//
// Created by wjc on 2023/3/19.
//
#include <map>
#include <string>
#include <iostream>
using namespace  std;

int main33424(void)
{
    // 插入到map容器内部的元素默认是按照key从小到大来排序。
    // key类型一定要重载<运算符
    map<string,int> mapTest;
    mapTest["aaa"] = 100;  // int& operator[](const string& index)
    mapTest.insert(map<string,int>::value_type("bbb", 200));
    mapTest.insert(map<string,int>::value_type("bbb", 2000));
    mapTest.insert(pair<string,int>("ddd", 400));
    mapTest.insert(pair<string,int>("ddd", 4000));
    mapTest.insert(make_pair<string,int>("ccc", 300));
    mapTest.insert(make_pair<string,int>("ccc", 3000));
    mapTest["eee"] = 500;
    mapTest["eee"] = 300;

//    int n = mapTest["bbbb"];
    int n = mapTest["bbb"];
    cout<<n<<endl;
    mapTest["bbb"] = 2000;

    map<string,int>::iterator it;
//    it = mapTest.find("ccc");
    it = mapTest.find("cccc");

    mapTest.erase("bbb");

    if (it != mapTest.end())
    {
        it->second = 3000;
        mapTest.erase(it);
    }
    else
    {
        cout<<"not found"<<endl;
    }




//    map<string,int>::const_iterator it;
    for (it = mapTest.begin(); it != mapTest.end(); ++it)
    {
        cout<<it->first<<" "<<it->second<<endl;
    }

    return 0;
}