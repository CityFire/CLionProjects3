//
// Created by wjc on 2023/3/20.
//

#ifndef CALCULATOR_NODE_H
#define CALCULATOR_NODE_H

#include <vector>
#include <memory>
#include "FunctionTable.h"
#include "ptr_vector.h"

class Storage;

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
    virtual bool IsLvalue() const
    {
        return false;
    }
    virtual void Assign(double val)
    {
        assert(!"Assign called incorrectlly.");
    }
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
    BinaryNode(std::auto_ptr<Node>& left, std::auto_ptr<Node>& right) : left_(left), right_(right) {}
    ~BinaryNode();
protected:
//    Node* const left_;
//    Node* const right_;
    std::auto_ptr<Node> left_;
    std::auto_ptr<Node> right_;
};

// 单目
class UnaryNode : public Node
{
public:
    UnaryNode(std::auto_ptr<Node>& child) : child_(child) {}
    ~UnaryNode();
protected:
    std::auto_ptr<Node> child_;
};

class FunctionNode : public UnaryNode {
public:
    FunctionNode(std::auto_ptr<Node>& child, PtrFun pFun) : UnaryNode(child), pFun_(pFun) {}
    double Calc() const;
private:
    PtrFun pFun_;
};

//class AddNode : public BinaryNode
//{
//public:
//    AddNode(Node* left, Node* right) : BinaryNode(left, right) {}
//    double Calc() const;
//};
//
//class SubNode : public BinaryNode
//{
//public:
//    SubNode(Node* left, Node* right) : BinaryNode(left, right) {}
//    double Calc() const;
//};
//
//class MultiplyNode : public BinaryNode
//{
//public:
//    MultiplyNode(Node* left, Node* right) : BinaryNode(left, right) {}
//    double Calc() const;
//};
//
//class DivideNode : public BinaryNode
//{
//public:
//    DivideNode(Node* left, Node* right) : BinaryNode(left, right) {}
//    double Calc() const;
//};

class UMinusNode : public UnaryNode
{
public:
    UMinusNode(std::auto_ptr<Node> child) : UnaryNode(child) {}
    double Calc() const;
};

class MultipleNode :public Node
{
public:
    MultipleNode(std::auto_ptr<Node>& node)
    {
        AppendChild(node, true);
    }
    void AppendChild(std::auto_ptr<Node>& node, bool positive)
    {
//        childs_.push_back(node.release()); // 这里需要注意  operator new
        childs_.push_back(node);
        positives_.push_back(positive);
    }
    ~MultipleNode();
protected:
//    std::vector<Node*> childs_;
    ptr_vector<Node> childs_;
    std::vector<bool> positives_;
};

class SumNode : public MultipleNode
{
public:
    SumNode(std::auto_ptr<Node>& node) : MultipleNode(node) {}
    double Calc() const;
};

class ProductNode : public  MultipleNode
{
public:
    ProductNode(std::auto_ptr<Node>& node) : MultipleNode(node) {}
    double Calc() const;
};

class VariableNode : public Node
{
public:
    VariableNode(unsigned int id, Storage& storage) : id_(id), storage_(storage) {}
    double Calc() const;
    bool IsLvalue() const;
    void Assign(double val);
private:
    const unsigned int id_;
    Storage& storage_;
};

class AssignNode : public BinaryNode
{
public:
    AssignNode(std::auto_ptr<Node>& left, std::auto_ptr<Node>& right) : BinaryNode(left, right)
    {
        assert(left_->IsLvalue());  // a = 7 赋值时会有问题
    }
    double Calc() const;
};

#endif //CALCULATOR_NODE_H
