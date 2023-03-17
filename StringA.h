//
// Created by wjc on 2023/3/17.
//

#ifndef DEMO_STRINGA_H
#define DEMO_STRINGA_H


class StringA {
public:
    StringA(char* str = "");
    StringA(const StringA& other);
    StringA& operator=(const StringA& other);

    void Display();
    ~StringA();

private:
    char* AllocAndCopy(char* str);
    char* str_;
};


#endif //DEMO_STRINGA_H
