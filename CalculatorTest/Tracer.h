//
// Created by wjc on 2023/3/21.
//

#ifndef CALCULATORTEST_TRACER_H
#define CALCULATORTEST_TRACER_H

#include <stddef.h>
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
    void Lock() { ++lockCount_; }
    void UnLock() { --lockCount_; }
};

extern Tracer NewTrace;
#endif // NDEBUG

#endif //CALCULATORTEST_TRACER_H
