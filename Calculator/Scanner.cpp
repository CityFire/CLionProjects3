//
// Created by wjc on 2023/3/20.
//

#include <cctype>
#include <iostream>
#include "Scanner.h"

//Scanner::Scanner(const std::string& buf) : buf_(buf), curPos_(0)
//{
//    Accept();
//    isEmpty_ = (token_ == TOKEN_END);
//}

Scanner::Scanner(std::istream& in) : in_(in)//, curPos_(0)
{
    Accept();
    isEmpty_ = (token_ == TOKEN_END);
}

bool Scanner::IsEmpty() const
{
    return isEmpty_;
}

bool Scanner::isDone() const
{
    return token_ == TOKEN_END;
}

bool Scanner::IsCommand() const
{
    return token_ == TOKEN_COMMAND;
}

double Scanner::Number() const
{
    return number_;
}

std::string Scanner::GetSymbol() const
{
    return symbol_;
}

EToken Scanner::Token() const
{
    return token_;
}

int Scanner::ReadChar()
{
    look_ = in_.get();
    while (look_ == ' ' || look_ == '\t')
        look_ = in_.get();
}

//void Scanner::SkipWhite()
//{
//    while (isspace(buf_[curPos_]))
//        ++curPos_;
//}

void Scanner::Accept()
{
//    SkipWhite();
    ReadChar();
//    switch (buf_[curPos_]) {
    switch (look_) {
        case '!':
            token_ = TOKEN_COMMAND;
            break;
        case '+':
            token_ = TOKEN_PLUS;
//            ++curPos_;
            break;
        case '-':
            token_ = TOKEN_MINUS;
//            ++curPos_;
            break;
        case '*':
            token_ = TOKEN_MULTIPLY;
//            ++curPos_;
            break;
        case '/':
            token_ = TOKEN_DIVIDE;
//            ++curPos_;
            break;
        case '=':
            token_ = TOKEN_ASSIGN;
//            ++curPos_;
            break;
        case '(':
            token_ = TOKEN_LPARENTHESIS;
//            ++curPos_;
            break;
        case ')':
            token_ = TOKEN_RPARENTHESIS;
//            ++curPos_;
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '.':
            token_ = TOKEN_NUMBER;
            in_.putback(look_);
            in_>>number_;
//            char* p;
//            number_ = strtod(&buf_[curPos_], &p);
//            curPos_ = p - &buf_[0];
            break;
        case '\0': case '\n': case '\r': case EOF:
            token_ = TOKEN_END;
            break;
        default:
//            if (isalpha(buf_[curPos_]) || buf_[curPos_] == '_')
//            {
//                token_ = TOKEN_IDENTIFIER;
//                symbol_.erase();
//                char ch = buf_[curPos_];
//                do {
//                    symbol_ += ch;
//                    ++curPos_;
//                    ch = buf_[curPos_];
//                } while (isalnum(ch) || ch == '_');
//            }
            if (isalpha(look_) || look_ == '_')
            {
                token_ = TOKEN_IDENTIFIER;
                symbol_.erase();
                do {
                    symbol_ += look_;
                    look_ = in_.get();
                } while (isalnum(look_) || look_ == '_');
                in_.putback(look_);
            }
            else
                token_ = TOKEN_ERROR;
            break;
    }
}

void Scanner::AcceptCommand()
{
    ReadChar();
    symbol_.erase();
    while (!isspace(look_))
    {
        symbol_ += look_;
        look_ = in_.get();
    }
}