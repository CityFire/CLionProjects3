//
// Created by wjc on 2023/3/18.
//

#ifndef DEMO_COMPLEX_H
#define DEMO_COMPLEX_H


class Complex {
public:
    Complex(int real, int imag);
    Complex();
    ~Complex();

    Complex& Add(const Complex& other);
    // 运算符重载
    Complex operator+(const Complex& other);
    // 友元函数运算符重载
    friend Complex operator+(const Complex& c1, const Complex& c2);
    void Display() const;

private:
    int real_;
    int imag_;
};


#endif //DEMO_COMPLEX_H
