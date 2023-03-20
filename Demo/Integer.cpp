//
// Created by wjc on 2023/3/18.
//

#include "Integer.h"
#include <iostream>
using namespace std;

Integer::Integer(int n) : n_(n)
{

}

Integer::~Integer()
{

}

Integer& Integer::operator++()
{
    cout<<"Integer& Integer::operator++()"<<endl;
    ++n_;
    return *this;
}

//Integer& operator++(Integer& i)
//{
//    cout<<"Integer& operator++(Integer& i)"<<endl;
//    ++i.n_;
//    return i;
//}

Integer Integer::operator++(int n)
{
    cout<<"Integer& Integer::operator++(int i) "<<endl;
    //n_++;
    Integer tmp(n_);
    n_++;
    return tmp;
}

//Integer operator++(Integer& i, int n)
//{
//    cout<<"operator++(Integer& i, int n) "<<endl;
//    Integer tmp(i.n_);
//    i.n_++;
//    return tmp;
//}

void Integer::Display() const
{
    cout<<n_<<endl;
}

Integer::operator int()
{
    return n_;
}