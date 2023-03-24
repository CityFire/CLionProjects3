//
// Created by wjc on 2023/3/25.
//

#ifndef CALCULATOR_PTR_VECTOR_H
#define CALCULATOR_PTR_VECTOR_H

// operator new 分配空间
// placement new 不分配空间，调用拷贝构造函数
// vector容器  值语义
// 往容器中插入指针，容器拷贝的仅仅只是指针，容器并不负责指针所指向的内存空间的释放
// 往容器中插入的是类对象，容器拷贝整个对象，（容器中拷贝的对象与原对象脱离关系）

#include <memory>
#include <vector>

template <typename T>
class ptr_vector : public std::vector<T*>
{
public:
    ~ptr_vector()
    {
        clear();
    }

    void clear()
    {
        typename std::vector<T*>::iterator it;
        for (it = std::vector<T*>::begin(); it != std::vector<T*>::end(); ++it)
            delete *it;

        std::vector<T*>::clear();
    }

    void push_back(T* const &val)
    {
        std::auto_ptr<T> ptr(val);       // 用auto_ptr接管val所有权
        std::vector<T*>::push_back(val); // operator new
        ptr.release();
    }

    void push_back(std::auto_ptr<T>& val)
    {
        std::vector<T*>::push_back(val.get());
        val.release();
    }
};

#endif //CALCULATOR_PTR_VECTOR_H
