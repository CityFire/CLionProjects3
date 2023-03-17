//
// Created by wjc on 2023/3/17.
//

#ifndef DEMO_TESTA_H
#define DEMO_TESTA_H


class TestA {
public:
    //如果类不提供任何一个构造函数，系统将为我们提供一个不带参数的
    //默认构造函数
    TestA();
    /*explicit*/ TestA(int num); //explicit 1.只提供给类的构造函数使用的关键字。2.编译器不会把声明为explicit的构造函数用于隐式转换，它只能在程序代码中显示创建对象
    void Display();

    TestA& operator=(const TestA& other);

    ~TestA();
private:
    int num_;
};


#endif //DEMO_TESTA_H
