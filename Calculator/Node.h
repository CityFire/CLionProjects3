//
// Created by wjc on 2023/3/20.
//

#ifndef CALCULATOR_NODE_H
#define CALCULATOR_NODE_H


class Node {
public:
    virtual double Calc() const = 0;  // 纯虚函数
    virtual ~Node() {}  // 虚析构函数
};

class NumberNode : public Node
{
public:
    NumberNode(int number) : number_(number) {}
    double Calc() const;
private:
    const double number_;
};

// ️二目节点
class BinaryNode : public Node
{
public:
    BinaryNode(Node* left, Node* right) : left_(left), right_(right) {}
    ~BinaryNode();
protected:
    Node* const left_;
    Node* const right_;
};

// 单目
class UnaryNode : public Node
{
public:
    UnaryNode(Node* child) : child_(child) {}
    ~UnaryNode();
protected:
    Node* const child_;
};

class AddNode : public BinaryNode
{
public:
    AddNode(Node* left, Node* right) : BinaryNode(left, right) {}
    double Calc() const;
};

class SubNode : public BinaryNode
{
public:
    SubNode(Node* left, Node* right) : BinaryNode(left, right) {}
    double Calc() const;
};

class MultiplyNode : public BinaryNode
{
public:
    MultiplyNode(Node* left, Node* right) : BinaryNode(left, right) {}
    double Calc() const;
};

class DivideNode : public BinaryNode
{
public:
    DivideNode(Node* left, Node* right) : BinaryNode(left, right) {}
    double Calc() const;
};

class UMinusNode : public UnaryNode
{
public:
    UMinusNode(Node* child) : UnaryNode(child) {}
    double Calc() const;
};



#endif //CALCULATOR_NODE_H
