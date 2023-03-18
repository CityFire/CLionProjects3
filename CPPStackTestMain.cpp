//
// Created by wjc on 2023/3/19.
//
#include <iostream>
using namespace std;
//namespace A
//{
//
//}
//
//A::Stack

class Stack
{
    struct Link
    {
        int data_;
        Link* next_;
        Link(int data, Link* next) : data_(data), next_(next)
        {

        }
    };
public:
    Stack() : head_(0), size_(0)
    {

    }

    ~Stack()
    {
        cout<<"~Stack..."<<endl;
        Link* tmp;
        while (head_)
        {
            tmp = head_;
            head_ = head_->next_;
            delete tmp;
        }

        size_ = 0;
    }

    void Push(const int data)
    {
        Link* node = new Link(data, head_);
        head_ = node;
        ++size_;
    }

    bool Empty()
    {
        return (size_ == 0);
    }

    bool Pop(int& data)
    {
        if (Empty())
        {
            return false;
        }

        Link* tmp = head_;
        data = head_->data_;
        head_ = head_->next_;
        delete tmp;
        --size_;

        return true;
    }

private:
    Link* head_;
    int size_;
};

// 避免名称冲突
// 类型的扩充
// 数据封装、能够保护内部的数据结构不遭受外界破坏
int main(void)
{
    Stack stack;  // 抽象数据类型  类类型
    int i;
    for (i = 0; i < 5; i++)
    {
        stack.Push(i); // this = &stack
    }

    while (!stack.Empty())
    {
        stack.Pop(i);
        cout<<i<<" ";
    }
    cout<<endl;

    return 0;
}