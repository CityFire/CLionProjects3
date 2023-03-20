//
// Created by wjc on 2023/3/18.
//

#ifndef DEMO_INTEGER_H
#define DEMO_INTEGER_H


class Integer {
public:
    Integer(int n);
    ~Integer();

    Integer& operator++();
//    friend Integer& operator++(Integer& i);

    Integer operator++(int n);
//    friend Integer operator++(Integer& i, int n);

    // 类型转换运算符
    operator int();

    void Display() const;
private:
    int n_;
};


#endif //DEMO_INTEGER_H
