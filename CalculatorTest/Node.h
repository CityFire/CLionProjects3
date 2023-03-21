//
// Created by wjc on 2023/3/22.
//

#ifndef CALCULATORTEST_NODE_H
#define CALCULATORTEST_NODE_H


class Node {
public:
    Node();
    ~Node();
    void Calc() const;
};

class NodePtr
{
public:
    explicit  NodePtr(Node* ptr = 0) : ptr_(ptr) {}

    ~NodePtr() { delete ptr_; }

    NodePtr(NodePtr& other) : ptr_(other.Release()) {}

    NodePtr& operator=(NodePtr& other)
    {
        Reset(other.Release());
        return *this;
    }

//    Node& operator*() const { *Get(); }

    Node* operator->() const { Get(); }

    Node* Get() const { return  ptr_; }

    Node* Release()
    {
        Node* tmp = ptr_;
        ptr_ = 0;
        return tmp;
    }

    void Reset(Node* ptr = 0)
    {
        if (ptr_ != ptr)
        {
            delete ptr_;
        }
        ptr_ = ptr;
    }
private:
    Node* ptr_;   // 裸指针
};

#endif //CALCULATORTEST_NODE_H
