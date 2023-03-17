//
// Created by wjc on 2023/3/17.
//

#include "TestA.h"
#include <iostream>
using namespace std;

// 不带参数的构造函数称为默认构造函数
TestA::TestA() {
    num_ = 0;
    cout<<"Initialzing Default"<<endl;
}

TestA::TestA(int num) {
    num_ = num;
    cout<<"Initialzing "<<num_<<endl;
}

void TestA::Display() {
    cout<<"num="<<num_<<endl;
}

TestA::~TestA() {
    cout<<"Destory "<<num_<<endl;
}