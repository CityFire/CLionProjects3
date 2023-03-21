//
// Created by wjc on 2023/3/21.
//
#include <iostream>
#include <stdio.h>
#include <setjmp.h>
#include <string>
using namespace std;

class TestException
{
public:
    TestException()
    {
        cout<<"TestException..."<<endl;
    }
    TestException(const TestException& other)
    {
        cout << "Copy TestException..." << endl;
    }
    ~TestException()
    {
        cout<<"~TestException..."<<endl;
    }
};

TestException func()
{
//    TestException t;
//    return t;
    // TestException...
    // ~TestException...

    return TestException();
}

int mainTest(void)
{
    func();
    return 0;
}

class MyException {
public:
    MyException(const char* message) : message_(message)
    {
        cout<<"MyException..."<<endl;
    }
    MyException(const MyException& other) : message_(other.message_)
    {
        cout<<"Copy MyException..."<<endl;
    }
    const char* what() const
    {
        return message_.c_str();
    }
    ~MyException()
    {
        cout<<"~MyException..."<<endl;
    }
private:
    string message_;
};

class Obj
{
public:
    Obj()
    {
        cout<<"Obj..."<<endl;
    }
    Obj(const Obj& other)
    {
        cout<<"Copy Obj..."<<endl;
    }
    ~Obj()
    {
        cout<<"~Obj..."<<endl;
    }
};

class TestException2
{
public:
    TestException2()
    {
        cout<<"TestException2..."<<endl;
        throw MyException("test Exception2...");
    }
    TestException2(const TestException2& other)
    {
        cout << "Copy TestException2..." << endl;
    }
    ~TestException2()
    {
        cout<<"~TestException2..."<<endl;
    }
private:
    Obj obj_;
};

class TestException3
{
public:
    TestException3()
    {
        obj_ = new Obj;
        cout<<"TestException3..."<<endl;
        throw MyException("test Exception3...");
    }
    TestException3(const TestException3& other)
    {
        cout << "Copy TestException3..." << endl;
    }
    ~TestException3()
    {
        delete obj_;
        cout<<"~TestException3..."<<endl;
    }
private:
    Obj* obj_;
};

/*
 * 栈展开
   1.沿着嵌套调用链接向上查找，直至为异常找到-个catch子句。这个过程称之为栈展开。
       a.为局部对象调用析构函数
       b.析构函数应该从不抛出异常
         b.1 栈展开期间会执行析构函数，在执行析构函数的时候，已经引发的异常但还没处理，如果这个过程中析构函数又拋出新的异
常，将会调用标准库的terminate函数。
       c.异常与构造函数
         c.1 构造函数中可以抛出异常。如果在构造函数函数中抛出异常，则可能该对象只是部分被构造。即使对象只是被部分构造，也
要保证销毁已构造的成员。
 */
class TestException4
{
public:
    TestException4()
    {
        cout<<"TestException4..."<<endl;
        throw MyException("test exception4...");
    }
    TestException4(const TestException4& other)
    {
        cout << "Copy TestException4..." << endl;
    }
    ~TestException4()
    {
        cout<<"~TestException4..."<<endl;
        throw 4;
    }
};

// 编译错误，即语法错误。程序就无法被生成运行代码。
// 运行时错误
   // 1.不可预料的逻辑错误
   // 2.可以预料的运行异常
   // 例如：1 动态分配空间时可能不会成功  2.打开文件可能会失败 3.除法运算时分母可能为0 4.整数相乘可能溢出  5.数组越界

//  throw异常抛出
//  1、可以抛出内置类型异常也可以抛出自定义类型异常
//  2.throw抛出一个类对象会调用拷贝构造函数
//  3.异常发生之前创建的局部对象被销毁，这一过程称为栈展开
double Divide(double a, double b)
{
    if (b == 0.0)
    {
//        MyException e("MyException division by zero");
//        throw e;         // throw
        throw 1;
    }
    else
        return a / b;
}

void myTerminate()
{
    cout<<"myTerminate..."<<endl;
}

int main(void)
{
    set_terminate(myTerminate);
    /*
    try                 // try
    {
//        cout<<"cplusplus division..."<<endl;
//        cout<<Divide(5.0, 1.0)<<endl;
        cout<<Divide(5.0, 0.0)<<endl;
    }
    catch (MyException& e)  // catch
    {
        cout<<e.what()<<endl;
    }
    catch (int)            // catch
    {
        cout<<"int exception..."<<endl;
    }
    catch (...)
    {
        cout<<"catch a exception..."<<endl;
        // 异常捕获
        // 1.一个异常处理器一般只捕捉一种类型的异常
        // 2.异常处理器的参数类型和抛出异常的类型相同
        // 3、...表示可以捕获任何异常
    }
    */

//    cplusplus division...
//    5
//    MyException...
//    Copy MyException...   // 会调用拷贝构造函数
//    ~MyException...
//    MyException division by zero
//    ~MyException...


/*
    异常传播

    1.try块可以嵌套
    2.程序按顺序寻找匹配的异常处理器，抛出的异常
            将被第一个类型符合的异常处理器捕获
    3.如果内层try块后面没有找到合适的异常处理器，
    该异常向外传播，到外层try块后面的catch块中寻
            找
    4.没有被捕获的异常将调用terminate函数，
    terminate函数默认调用abort终止程序的执行
    5.可以使用set terminate函数指定terminate函数将
    调用的函数
*/

    try
    {
        try
        {
            //TestException t;
            //throw MyException("test exception");

            //TestException2 t2;
            //TestException3 t3;


            TestException4 t4;
            throw MyException("test exception4");

//            Obj...
//            TestException2...
//            MyException...
//            ~Obj...
//            Outer...
//            test Exception2...
//            ~MyException...
        }
        catch (int)
        {
            cout<<"Inner..."<<endl;
            cout<<"catch a int exception"<<endl;
        }
//        catch (MyException& e)
//        {
//            cout<<"Inner..."<<endl;
//            cout<<e.what()<<endl;
//            throw e;
//        }
    }
    catch (MyException& e)
    {
        cout<<"Outer..."<<endl;
        cout<<e.what()<<endl;
    }

//    MyException...
//    Outer...
//    test exception
//    ~MyException...

//    MyException...
//    Inner...
//    test exception
//    Copy MyException...
//    ~MyException...
//    Outer...
//    test exception
//    ~MyException...

    return 0;
}



// C语言版本

//jmp_buf
//void longjmp(jmp_buf env, int val);
jmp_buf  buf;

double CDivide(double a, double b)
{
    if (b == 0.0)
    {
        longjmp(buf, 1);   // throw
    }
    else
        return a / b;
}

int Cmain(void)
{
    int ret;
    ret = setjmp(buf);
    if (ret == 0)        // try
    {
        printf("division...\n");
        printf("%f\n", CDivide(5.0, 1.0));
        printf("%f\n", CDivide(5.0, 0.0));
    }
    else if (ret == 1)   // catch
    {
        printf("division by zero\n");
    }
    return 0;
}