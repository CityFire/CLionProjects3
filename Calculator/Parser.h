//
// Created by wjc on 2023/3/20.
//

#ifndef CALCULATOR_PARSER_H
#define CALCULATOR_PARSER_H

#include <memory>

class Scanner;
class Node;
class Calc;

enum STATUS
{
    STATUS_OK,
    STATUS_ERROR,
    STATUS_QUIT
};

class Parser {
public:
    Parser(Scanner& scanner, Calc& calc);
    ~Parser(); // { delete tree_; }
    STATUS Parse();
    std::auto_ptr<Node> Expr();  // 表达式
    std::auto_ptr<Node> Term();  // Item项
    std::auto_ptr<Node> Factor();// 因子
    double Calculate() const;
private:
    Scanner& scanner_;
    Calc& calc_;
    std::auto_ptr<Node> tree_;
    STATUS status_;
};


#endif //CALCULATOR_PARSER_H
