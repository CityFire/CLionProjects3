//
// Created by wjc on 2023/3/20.
//

#ifndef CALCULATOR_PARSER_H
#define CALCULATOR_PARSER_H

class Scanner;
class Node;

enum STATUS
{
    STATUS_OK,
    STATUS_ERROR,
    STATUS_QUIT
};

class Parser {
public:
    Parser(Scanner& scanner);
    void Parse();
    Node* Expr();  // 表达式
    Node* Term();  // Item项
    Node* Factor();// 因子
    double Calculate() const;
private:
    Scanner& scanner_;
    Node* tree_;
    STATUS status_;
};


#endif //CALCULATOR_PARSER_H
