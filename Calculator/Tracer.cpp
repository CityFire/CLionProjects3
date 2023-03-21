//
// Created by wjc on 2023/3/22.
//

#include <iostream>
#include "Tracer.h"

bool Tracer::Ready = false;

Tracer::Tracer() : lockCount_(0)
{
    Ready = true;
}

Tracer::~Tracer()
{
    Ready = false;
    Dump();
}

Tracer NewTrace;

#ifndef NDEBUG
void* operator new(size_t size, const char* file, long line)
{
    void* p = std::malloc(size);
    if (Tracer::Ready)
    {
        NewTrace.Add(p, file, line);
    }
    return p;
}

void operator delete(void* p, const char*, long)
{
    if (Tracer::Ready)
    {
        NewTrace.Remove(p);
    }
    std::free(p);
}

//void* operator new(size_t size)
//{
//    void* p = std::malloc(size);
//    if (Tracer::Ready)
//    {
//        NewTrace.Add(p, "?", 0);
//    }
//    return p;
//}

void operator delete(void* p)
{
    if (Tracer::Ready)
    {
        NewTrace.Remove(p);
    }
    std::free(p);
}

void* operator new[](size_t size, const char* file, long line)
{
    void* p = std::malloc(size);
    if (Tracer::Ready)
    {
        NewTrace.Add(p, file, line);
    }
    return p;
}

void operator delete[](void*p, const char*, long)
{
    if (Tracer::Ready)
    {
        NewTrace.Remove(p);
    }
    std::free(p);
}

void* operator new[](size_t size)
{
    void* p = std::malloc(size);
    if (Tracer::Ready)
    {
        NewTrace.Add(p, "?", 0);
    }
    return p;
}

void operator delete[](void* p)
{
    if (Tracer::Ready)
    {
        NewTrace.Remove(p);
    }
    std::free(p);
}
#endif  // #ifndef NDEBUG

void Tracer::Add(void *p, const char *file, long line)
{
    if (lockCount_ > 0)
        return;

    Tracer::Lock Lock(*this);

    mapEntry_[p] = Entry(file, line);
}

// 死锁
// 局部对象析构函数调用的确定性
void Tracer::Remove(void *p)
{
    if (lockCount_ > 0)
        return;

    Tracer::Lock Lock(*this);
    std::map<void*, Entry>::iterator it;
    it = mapEntry_.find(p);
    if (it != mapEntry_.end())
    {
        mapEntry_.erase(it);
    }
}

void Tracer::Dump()
{
    if (mapEntry_.size() > 0)
    {
        std::cout<<"*** Memory leak(s):"<<std::endl;
        std::map<void*, Entry>::iterator it;

        for (it = mapEntry_.begin(); it != mapEntry_.end(); ++it) {
            const char* file = it->second.File();
            long line = it->second.Line();
            int addr = reinterpret_cast<uintptr_t>(it->first);
//            int addr = (uintptr_t)(it->first);
            std::cout<<"0x"<<std::hex<<addr<<": "
                     <<file<<", line "<<std::dec<<line<<std::endl;
        }
        std::cout<<std::endl;
    }
}