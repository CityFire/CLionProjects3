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
    TestA(int num);
    void Display();

    TestA& operator=(const TestA& other);

    ~TestA();
private:
    int num_;
};


#endif //DEMO_TESTA_H
