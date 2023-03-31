//
// Created by wjc on 2023/3/29.
//
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_array.hpp>

using namespace std;

class X
{
public:
    X()
    {
        cout <<"X..."<<endl;
    }
    ~X()
    {
        cout<<"~X..."<<endl;
    }
    void Fun()
    {
        cout<<"Fun..."<<endl;
    }
};

void weakPtr()
{
    boost::weak_ptr<X> p;
    {
        boost::shared_ptr<X> p2(new X);
        cout<<p2.use_count()<<endl;
        p = p2;
        cout<<p2.use_count()<<endl;

        boost::shared_ptr<X> p3 = p.lock();
        if (!p3) {
            cout<<"p3 object is destroyed"<<endl;
        }
        else {
            p3->Fun();
        }
    }
    boost::shared_ptr<X> p4 = p.lock();
    if (!p4) {
        cout<<"p4 object is destroyed"<<endl;
    }
    else {
        p4->Fun();
    }
}

void f(boost::shared_ptr<int>, int) {}
int g() {}

void ok()
{
    boost::shared_ptr<int> p(new int(2));
    f(p, g());
}

void bad()
{
    f(boost::shared_ptr<int>(new int(2)), g());
}

int main(void)
{
    cout<<"Entering main..."<<endl;
    {
        boost::scoped_ptr<X> p(new X);

//        boost::scoped_ptr<X> p2(p);
    }

    cout<<"Exiting main..."<<endl;


    weakPtr();


    boost::scoped_array<X> p(new X[2]);
    boost::scoped_array<X> xx(new X[3]);


    return 0;
}