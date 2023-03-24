//
// Created by wjc on 2023/3/24.
//

#ifndef CALCULATORTEST_PTR_VECTOR_H
#define CALCULATORTEST_PTR_VECTOR_H

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

#endif //CALCULATORTEST_PTR_VECTOR_H
