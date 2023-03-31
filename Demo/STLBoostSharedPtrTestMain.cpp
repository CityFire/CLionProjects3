//
// Created by wjc on 2023/3/31.
//
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

using namespace std;

class XX
{
public:
    XX()
    {
        cout <<"XX..."<<endl;
    }
    ~XX()
    {
        cout<<"~XX..."<<endl;
    }
    void Fun()
    {
        cout<<"Fun..."<<endl;
    }
};

int mainshared_ptr(void)
{
    boost::shared_ptr<XX> p1(new XX);
    cout<<p1.use_count()<<endl;  // output -> 1
    boost::shared_ptr<XX> p2 = p1;
    boost::shared_ptr<XX> p3;
    p3 = p1;

    cout<<p2.use_count()<<endl;  // output -> 3
    p1.reset();
    cout<<p2.use_count()<<endl;  // output -> 2
    p2.reset();

//    vector<auto_ptr<XX>> v;
//    auto_ptr<XX> p(new XX);
//    v.push_back(p);

    vector<boost::shared_ptr<XX>> v;
    boost::shared_ptr<XX> p(new XX);
    v.push_back(p);
    cout<<p.use_count()<<endl;  // output -> 2

    return 0;
}