//
// Created by wjc on 2023/3/17.
//

#ifndef DEMO_STRINGA_H
#define DEMO_STRINGA_H


class StringA {
public:
    explicit StringA(const char* str = "");
    StringA(const StringA& other);
    StringA& operator=(const StringA& other);
    StringA& operator=(const char* str);
    // 非运算符重载
    bool operator!() const;

    void Display() const;
    ~StringA(void);

private:
    char* AllocAndCopy(const char* str);
    char* str_;
};


#endif //DEMO_STRINGA_H
