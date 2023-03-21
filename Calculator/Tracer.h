//
// Created by wjc on 2023/3/22.
//

#ifndef CALCULATOR_TRACER_H
#define CALCULATOR_TRACER_H


//#include <stddef.h>
#include <map>

#ifndef NDEBUG

void* operator new(size_t size, const char* file, long line);
void operator delete(void* p, const char*, long);

//void* operator new(size_t size);
void operator delete(void* p);
void* operator new[](size_t size, const char* file, long line);
void operator delete[](void*p, const char*, long);
void* operator new[](size_t size);
void operator delete[](void* p);

class Tracer {
private:
    class Entry
    {
    public:
        Entry(const char* file=0, long line=0)
                : file_(file), line_(line) {}
        const char* File() const { return file_; }
        long Line() const { return line_; }
    private:
        const char* file_;
        long line_;
    };

    class Lock
    {
    public:
        Lock(Tracer& tracer) : tracer_(tracer)
        {
            tracer_.lock();
        }
        ~Lock()
        {
            tracer_.unlock();
        }
    private:
        Tracer& tracer_;
    };

    friend class Lock;   // 友元类
    // 友元函数破坏了封装性，但是它提高了效率
    // 友元类恰恰是提高了类的封装性

public:
    Tracer();
    ~Tracer();
    static bool Ready;

    void Add(void* p, const char* file, long line);
    void Remove(void* p);
    void Dump();
private:
    std::map<void*, Entry> mapEntry_;
    int lockCount_;
    void lock() { ++lockCount_; }
    void unlock() { --lockCount_; }
};

extern Tracer NewTrace;
#endif // NDEBUG


#endif //CALCULATOR_TRACER_H
