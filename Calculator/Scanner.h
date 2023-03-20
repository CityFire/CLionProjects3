//
// Created by wjc on 2023/3/20.
//

#ifndef CALCULATOR_SCANNER_H
#define CALCULATOR_SCANNER_H
#include <string>

enum EToken
{
    TOKEN_END,
    TOKEN_ERROR,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPARENTHESIS,
    TOKEN_RPARENTHESIS,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN
};

class Scanner
{
public:
    Scanner(const std::string& buf);
    void Accept();
    double Number() const;
    EToken Token() const;
private:
    void SkipWhite();
    const std::string buf_;
    unsigned int curPos_;
    EToken token_;
    double number_;
};


#endif //CALCULATOR_SCANNER_H
