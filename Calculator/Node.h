//
// Created by wjc on 2023/3/20.
//

#ifndef CALCULATOR_NODE_H
#define CALCULATOR_NODE_H

// 值语义和对象语义  对象语义 禁止拷贝
// 值语义对象通常以类对象的方式来使用       基于对象编程
// 对象语义对象通常以指针或者引用方式来使用  面向对象编程

class Noncopyable
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}
private:
    Noncopyable(const Noncopyable&);
    const Noncopyable& operator=(const Noncopyable&);
};

// 接口继承 实现继承
class Node : private Noncopyable { // 实现继承
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
