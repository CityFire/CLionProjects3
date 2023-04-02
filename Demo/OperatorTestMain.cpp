//
// Created by wjc on 2023/3/18.
//
#include "Integer.h"
#include "StringA.h"
#include <iostream>
using namespace std;

class TestBB
{
public:
    TestBB(int n) : n_(n)
    {
        cout<<"TestBB(int n) : n_(n)"<<endl;
    }
    TestBB(const TestBB& other)
    {
        cout<<"TestBB(const TestBB& other)"<<endl;
    }
    ~TestBB()
    {
        cout<<"~TestBB..."<<endl;
    }
    void* operator new(size_t size)
    {
        cout<<"void* operator new(size_t size)"<<endl;
        void* p = malloc(size);
        return p;
    }

    void operator delete(void* p)
    {
        cout<<"void operator delete(void* p)"<<endl;
        free(p);
    }

    void operator delete(void* p, size_t size)
    {
        cout<<"void operator delete(void* p, size_t size)"<<endl;
        free(p);
    }

    void* operator new(size_t size, const char* file, long line)
    {
        cout<<file<<":"<<line<<endl;
        void* p = malloc(size);
        return p;
    }

    void operator delete(void* p, const char* file, long line)
    {
        cout<<file<<":"<<line<<endl;
        free(p);
    }

    void operator delete(void* p, size_t size, const char* file, long line)
    {
        cout<<file<<":"<<line<<endl;
        free(p);
    }

    // placement new
    void* operator new(size_t size, void* p)
    {
        return p;
    }

    void operator delete(void *, void *)
    {

    }
//private:
    int n_;
};

/*
void* operator new(size_t size)
{
    cout<<"global void* operator new(size_t size)"<<endl;
    void* p = malloc(size);
    return p;
}

void operator delete(void* p)
{
    cout<<"global void operator delete(void* p)"<<endl;
    free(p);
}

void* operator new[](size_t size)
{
    cout<<"global void* operator new[](size_t size)"<<endl;
    void* p = malloc(size);
    return p;
}

void operator delete[](void* p)
{
    cout<<"global void operator delete[](void* p)"<<endl;
    free(p);
}

 */

int main7654(void)
{
    TestBB* p1 = new TestBB(10);  // new operator = operator new + 构造函数的调用
    delete p1;

    //char* str = new char;
    //delete str;
    char* str = new char[100];
    delete[] str;

    char chunk[10];

    TestBB* p2 = new (chunk) TestBB(200);   // operator new(size_t, void *_Where)
                                               // placement new 不分配内存
    // 什么是placement new
    //placement new就是在用户指定的内存位置上（这个内存是已经预先分配好的）构建新的对象，
    // 因此这个构建过程不需要额外分配内存，只需要调用对象的构造函数在该内存位置上构造对象即可
    //语法格式：
    //  address：placement new所指定的内存地址
    //  ClassConstruct：对象的构造函数
    //  Object * p = new (address) ClassConstruct(...);
    //优点：
    //在已分配好的内存上进行对象的构建，构建速度快
    //已分配好的内存可以反复利用，有效的避免内存碎片问题


    cout<<p2->n_<<endl;
    p2->~TestBB();                             // 显式调用析构函数

    //TestBB* p3 = (TestBB*)chunk;
    TestBB* p3 = reinterpret_cast<TestBB*>(chunk);
    cout<<p3->n_<<endl;

//#ifdef _DEBUG
//#define DEBUG_NEW new(__FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif
    TestBB* p4 = new(__FILE__, __LINE__) TestBB(300);
//    TestBB* p4 = new TestBB(300);
    delete p4;

    return 0;
}

class DBHelper
{
public:
    DBHelper()
    {
        cout<<"DB..."<<endl;
    }
    ~DBHelper()
    {
        cout<<"~DB..."<<endl;
    }
    void Open()
    {
        cout<<"Open..."<<endl;
    }
    void Query()
    {
        cout<<"Query..."<<endl;
    }
    void Close()
    {
        cout<<"Close..."<<endl;
    }
};

class DB
{
public:
    DB()
    {
        db_ = new DBHelper;
    }
    ~DB()
    {
        delete db_;
    }
    //指针->运算符重载
    DBHelper* operator->()
    {
        return db_;
    }
private:
    DBHelper* db_;
};

int main8765(void) {

    DB db;
    db->Open();
    db->Query();
    db->Close();

    return 0;
}

int add(int a, int b)
{
    return a + b;
}

int main111(void)
{
    Integer n(100);
    n = 200;
    n.Display();

    int sum = add(n, 100);
    cout<<sum<<endl;

    int x = n;
    int y = static_cast<int>(n);

    Integer n2 = ++n;
    n.Display();
    n2.Display();

    Integer n3 = n++;
    n.Display();
    n3.Display();


    StringA s1("AAAA");
    StringA s2(s1);   // 调用默认的拷贝构造函数
    // 系统提供的默认拷贝构造函数实施的是浅拷贝 s2.str_ = s1.str_  导致释放两次

    StringA s3;
    s3 = s1;
    s3.Display();

    s3 = "xxxx";
    s3.Display();

    StringA s4;
    bool notempty;
    notempty = !s4;
    cout<<notempty<<endl;

    s4 = "aaaa";
    notempty = !s4;
    cout<<notempty<<endl;

    StringA ss1("abcdefg");

    char ch = ss1[2];
    cout<<ch<<endl;

    ss1[2] = 'A';
    ss1.Display();

    const StringA ss2("background");
    ch = ss2[2];
    //ss2[2] = 'M';
    ss2.Display();

    StringA ss3 = "xxx";
    StringA ss4 = "yyy";

    StringA ss5 = ss3 + ss4;
    ss5.Display();

//    StringA ss6 = "bbbb" + "aaa" + ss5 + "rrdfddd";
    StringA ss6 = "aaa" + ss5 + "rrdfddd";
    ss6.Display();

    ss3 += ss4;
    ss3.Display();

    cout<<ss3<<endl;

    StringA ss7;
    cin>>ss7;
    cout<<ss7<<endl;

    return 0;
}