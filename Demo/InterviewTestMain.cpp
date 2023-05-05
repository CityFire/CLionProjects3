//
// Created by wjc on 2023/4/1.
//
#include <iostream>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <array>
#include <functional>

using namespace std;

int MyStrlen(char str[]) // 指针变量
{

    return (int)(sizeof(str)-1);

}

template <typename T>
T fun(T t) {
    return t;
}

void GetMemory(char **p)
{

    *p = new char[100];

    strcpy(*p,"hello world");

}

// 将 str 中的小写字母转换成大写字母
void UpperCase(char str[])
{
    // 函数内的str实际只是一个指向字符串的指针，没有任何额外的与数组相关的信息，
    // 因此sizeof作用于上只将其当指针看，一个指针为4个字节，因此返回4。
    size_t size = sizeof(str) / sizeof(str[0]); // 8
    for (size_t i = 0; i < size; ++i)
    {
        if ('a' <= str[i] && str[i] <= 'z')
        {
            str[i] -= ('a' - 'A');
            cout<<str[i]<<endl;
        }
    }
}

void test()
{
    int a = 248, b = 4;

    int const c = 21;

    const int *d = &a;

    int *const e = &b;

//    int const *f const = 0x329f;

//    *c = 32;
    d = &b;
//    *d = 43;
//    e = 34;
//    e = &a;
//    f = 0x321f;




}

class Drink {
public:
    void packing() {
        //
    }
};

class DrinkApple : public Drink {

};

// 桶排序
// 题目描述
//给定一个数组，求前 k 个最频繁的数字。 输入输出样例
//输入是一个数组和一个目标值 k。输出是一个长度为 k 的数组。 Input: nums = [1,1,1,1,2,2,3,4], k = 2
//Output: [1,2]
//在这个样例中，最频繁的两个数是 1 和 2。
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int, int> counts;
    int max_count = 0;
    for (const int & num : nums) {
        max_count = max(max_count, ++counts[num]);
    }

    vector<vector<int>> buckets(max_count + 1);
    for (const auto & p : counts) {
        buckets[p.second].push_back(p.first);
    }

}

auto func2(int a, double b) -> decltype(a + b)
{
    return a + b;
}

template<class T1, class T2>
auto mul(const T1 &t1, const T2 &t2) -> decltype(t1*t2)
{
    return t1*t2;
}

class A
{
public:
    A(int i) : a(i) // 参数列表初始化
    {
        // a = i;
    }

    int a;
};

class B
{
public:
    int data{ 1 };
    int data2 = 1;
    A tmp{ 10 };

    string name{ "mike" };
};

//void func33(int a[])  // 形参中的数组不是数组，是指针变量，无法确定元素个数
//{
//    // 基于范围的for，这个范围要确定后，才能使用
//    for (int & tmp : a) {
//       cout << tmp << endl;
//    }
//}

void func3(int a)
{
    cout<<__LINE__<<endl;
}

void func3(int* p)
{
    cout<<__LINE__<<endl;
}

int GetNum()
{
    return 3;
}

const int GetNum2()
{
    return 3;
}

constexpr int GetNum3()
{// 常量表达式，发生在编译阶段
    return 6;
}

/*
 * constexpr函数的限制：
 * 函数中只能有一个return语句（有极少特例）
 * 函数必须返回值（不能是void函数
 * 在使用前必须已有定义
 * return返回语句表达式中不能使用非常量表达式的函数、全局数据，且必须是一个常量表达式
 */
constexpr int func01()
{
    // err,函数中只能有一个return语句
    constexpr int a = 1;
    return a;
}

constexpr int func02()
{
    // 允许包含typedef, using指令，静态断言
    static_assert(1, "fail");

    return 3;
}

// read of non-const variable 'aaa' is not allowed in a constant expression declared here
// err, return返回语句表达式中不能使用非常量表达式的函数、全局数据，且必须是一个常量表达式
//int aaa = 3;
//constexpr int func03()
//{
//    return aaa;
//}

//int test01()
//{
//    return 10;
//}
//
//constexpr int func04()
//{// non-constexpr function 'test01' cannot be used in a constant expression declared here
//    // err, 不能使用非常量表达式的函数
//    return test01();
//}

class Date
{
public:
    constexpr Date(int year, int month, int day) : year_(year), month_(month), day_(day)
    {

    }

    constexpr int getYear()
    {
        return year_;
    }

    constexpr int getMonth()
    {
        return month_;
    };

    constexpr int getDay()
    {
        return day_;
    }

private:
    int year_;
    int month_;
    int day_;
};

class AA
{
public:
    AA(int x, int y)
    {
        a = x;
        b = y;
    }
protected:
    int a;
    int b;
};

class BB : public AA
{
public:
#if 0
    // 通过参数列表给基类构造函数传参
    BB(int x, int y) : AA(x, y)
    {

    }
#endif
    // 继承构造 （C++11允许派生类继承基类的构造函数（默认构造函数、复制构造函数、移动构造函数除外）
    // 注意：
    // 继承的构造函数只能初始化基类中的成员变量，不能初始化派生类的成员变量
    // 如果基类的构造函数被声明为私有，或者派生类是从基类中虚继承，那么不能继承构造函数
    // 一旦使用继承构造函数，编译期不会再为派生类生成默认构造函数
    using AA::AA;

    void display()
    {
        cout<<"a = "<<a<<", b = "<<b<<endl;
    }
};

class Test
{
public:
    // 委托构造，一定要通过初始化列表方式
    Test() : Test(1, 'a')
    {

    }

    Test(int x) : a(x)
    {

    }

    Test(char x) : b(x)
    {

    }

    Test(int x, char y) : a(x), b(y)
    {

    }

    int a;
    char b;
};

// final阻止类的进一步派生，虚函数的进一步重写
class A1 final
{
    int a;
};

//class A2 : public A1
//{
//
//};

class B1
{
public:
    B1() = default;  // 让编译器提供一个默认的构造函数，效率比用户写的高
    // default只能修饰类中默认提供的成员函数：无参构造，拷贝构造，赋值运算符重载，析构函数等
    B1(int i)
    {
        // 写了带参的构造函数，编译器不会提供无参的构造函数
        a = i;
    }
    B1(const B1&) = delete;
    B1& operator=(const B1&) = delete; // 赋值运算符重载函数
    B1(int, int) = delete;
    void *operator new(size_t) = delete;
    void *operator new[](size_t) = delete;
    virtual void func() final {}
    virtual void func(int a) {}
    int a;
};

class B2 : public B1
{
public:
//    virtual void func() {}
    virtual void func(int a) override {}
};

/*
 * char const *
 * unsigned long long
 * long double
 * char const *, size_t
 * wchar_t const *, size_t
 * char16_t const *, size_t
 * char32_t const *, size_t
 */

// 自定义字面量，名字要求 operator"" xxx
// 只需给第一个参数传参，第二个参数自动推算，测第一个参数的长度，给第二个参数赋值
size_t operator"" _len(char const *str, size_t n)
{
    return n;
}

char const * operator"" _str(char const *buf, size_t n)
{
    return buf;
}

char const * operator"" _test(char const *tmp)
{
    return tmp;
}

// 通过typedef给一个类型起别名，不能新建类型
typedef int int32;
using my_int = int;  // C++11方式

// 类模板支持默认的模板参数
template<class T, class T2=int> //类模板的模板参数必须是从右往左
class A3
{

};

// C++11才支持，函数模板带默认的模板参数
template<class T = int, class T2> void fun05(T a, T2 b) {}

// 可变参数的模板函数
template<class ... T> // T叫模板参数包
void func08(T... args) // args叫函数参数包
{
    // 获取可变参数的个数
    cout<<"num="<< sizeof...(args)<<endl;
}

//#if 1
// 递归终止函数
void debug()
{
    cout<<"empty"<<endl;
}
//#endif

#if 0
// 递归终止函数2
template<class T>
void debug(T tmp)
{
    cout<<"tmp = "<<tmp<<endl;
}
#endif

// 可变参数的模板函数
// 参数包展开函数
template<class T1, class ... T2>
void debug(T1 first, T2... last)
{
    cout<<first<<endl;
    // 递归调用函数本身
    debug(last...);
}

// 非递归方式展开参数包
template<class T>
void print(T tmp)
{
    cout<<tmp<<endl;
}

//可变参数的模板函数
template<class ... T>
void expand(T ... args)
{
    // 逗号运算符
    // 初始化列表
    int a[] = { (print(args), 0)... };
}

// 继承方式展开可变参数模板类
// 1、可变参数模板声明
// 2、递归继承模板类
// 3、边界条件

//  1、可变参数模板声明
template<class ...T> class Car {};

// 2、递归继承模板类
template<class Head, class ... Tail>
class Car<Head, Tail...> : public Car<Tail...>
{// 递归继承本身
public:
    Car()
    {
        cout<<"type = "<< typeid(Head).name()<<endl;
    }
};

// 3、边界条件
template<> class Car<> {};

// 1、变长模板声明
// 2、变长模板类定义
// 3、边界条件

// 1、变长模板声明
template<int ... last>
class Test09
{

};

// 2、变长模板类定义
template<int first, int ... last>
class Test09<first, last...>
{
public:
    static const int val = first * Test09<last...>::val;
};

// 3、边界条件
template<>
class Test09<>
{
public:
    static const int val = 1;
};

int & func10()
{
    static int tmp;
    return tmp;
}

int func11()
{
    return 11;
}

void test9(int &tmp)
{
    cout<<"左值="<<tmp<<endl;
}

void test10(int && tmp)
{
    cout<<"右值="<<tmp<<endl;
}

class CCPPMyString
{
public:
    CCPPMyString(char *tmp = "abc")
    {// 普通构造函数
        len = strlen(tmp); // 长度
        str = new char[len+1]; // 堆区申请空间
        strcpy(str, tmp); // 拷贝内容

        cout<<"普通构造函数 str = "<<str<<endl;
    }
    CCPPMyString(const CCPPMyString &tmp)
    {// 拷贝构造函数
        len = tmp.len;
        str = new char[len+1];
        strcpy(str, tmp.str);

        cout<<"拷贝构造函数 tmp.str = "<<tmp.str<<endl;
    }
    // 移动构造函数
    // 参数是非const的右值引用
    CCPPMyString(CCPPMyString && t)
    {
        str = t.str; // 拷贝地址，没有重新申请内存
        len = t.len;

        // 原来指针置空
        t.str = nullptr;
        cout<<"移动构造函数"<<endl;
    }
    CCPPMyString& operator=(const CCPPMyString& tmp)
    {// 赋值运算符重载函数
        if (&tmp == this)
        {
            return *this;
        }

        // 先释放原来的内容
        len = 0;
        delete []str;

        // 重新申请内容
        len = tmp.len;
        str = new char[len + 1];
        strcpy(str, tmp.str);

        cout<<"赋值运算符重载函数 tmp.str = "<<tmp.str<<endl;

        return *this;
    }
    // 移动赋值函数
    // 参数为非const的右值引用
    CCPPMyString& operator=(CCPPMyString&& tmp)
    {// 移动赋值函数
        if (&tmp == this)
        {
            return *this;
        }

        // 先释放原来的内容
        len = 0;
        delete []str;

        // 无需重新申请堆区空间
        len = tmp.len;
        str = tmp.str; // 地址赋值
        tmp.str = nullptr;

        cout<<"移动赋值函数 str = "<<str<<endl;

        return *this;
    }
    ~CCPPMyString()
    {
        cout<<"析构函数:";
        if (str != nullptr)
        {
            delete []str;
            str = nullptr;
            len = 0;
            cout<<"已操作delete";
        }
        cout<<endl;
    }
private:
    char *str = nullptr;
    int len = 0;
};

CCPPMyString funcString() // 返回普通对象，不是引用
{
    CCPPMyString obj("mikejson");
    cout<<"&obj:"<<(void *)&obj<<endl;
    // 返回值优化技术
    // 防止返回值优化选项
    // g++ xxx.cpp -fno-elide-constructors -std=c++11
    return obj;
}

template<class T> void func(const T &)
{
   cout<<"const T &"<<endl;
}

template<class T> void func(T &)
{
    cout<<"T &"<<endl;
}

// add 2023-5-2 :  T &&
template<class T> void func(T &&)
{
    cout<<"T &&"<<endl;
}

//template<class T> void forward_val(const T &tmp) // const T &
//{
//    func(tmp); // 定义
//}
//
//template<class T> void forward_val(T &tmp) // T &
//{
//    func(tmp); // 定义
//}

// 等价于上面两个函数  引用折叠
template<class T> void forward_val(T &&tmp) // 参数为右值引用
{
    // std::forward保存参数的左值、右值属性
    func(std::forward<T>(tmp)); // 定义
}

template<typename T>
void f(T param) // name是一个数组，但是 T被推导为 const char *
{
    cout<< typeid(param).name()<<" "<< typeid(T).name()<<endl;
}

template<typename T>
void funf(T& param) // T被推导为const char[13] param被推导为const char(&)[13]
{
    cout<< typeid(param).name()<<" "<< typeid(T).name()<<endl;
}

//void myFunc(int param[]);
//void myFunc(int *param);

//在编译期间返回一个数组大小的常量值（
//数组形参没有名字，因为我们只关心数组
//的大小）
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
    return N;
}

void someFunc(int, double) {}	//someFunc是一个函数，类型是void(int, double)

template<typename T>
void f1f(T param) {}		// 传值

template<typename T>
void f2f(T & param) {}		// 传引用





int main(void) // Interview
{

    // 在C++中不止是数组会退化为指针，函数类型也会退化为一个函数指针
    f1f(someFunc);			//param被推导为指向函数的指针，类型是void(*)(int, double)
    f2f(someFunc);			//param被推导为指向函数的引用，类型为void(&)(int, double)

    int keyVals[] = {1,3,5,7,9,11,22,25};	// keyVals有七个元素
    int mappedVals[arraySize(keyVals)]; // mappedVals也有七个

    std::array<int, arraySize(keyVals)> mappedvalss; // mappedVals的size为7

    int va = 10;
    function<int(int)> f1 = [](int a) {
        return a + 1;
    };
    cout<<f1(9)<<endl;

    function<int(int)> f2 = bind(
            [](int a) { return a + 1; },
            std::placeholders::_1
          );
    cout<<f2(9)<<endl;

    auto f4 = [](int x, int y) -> int {
        return x + y;
    };
//    auto f4 = [=](int x, int y) -> int {
//        cout<<"va = "<<va<<endl;
//        return x + y;
//    };
    cout<<f4(1, 3)<<endl;

    decltype(f4) tmpp = f4;
    cout<<tmpp(2, 2)<<endl;

    // 定义一个函数指针类型
    typedef int (*PFUNC)(int, int);
    PFUNC  pp1 = f4; // lambda表达式转换为函数指针， ok lambda表达式不允许捕获外部变量
    cout<<pp1(10, 10)<<endl;


    const char name[] = "J. P. Briggs"; //name 的类型是 const char[13]
    const char * ptrToName = name; // 数组退化为指针
    f(name);
    funf(name);

    int h = 0;
    const int &g = 1;

    // 需要给forward_val()重载2个版本， const T &, T &
    forward_val(h); // T &
    forward_val(g); // const T &
    forward_val(111); // const T &  // add 2023-5-2 :  T &&

    int zz = 10; // zz为左值
    // Rvalue reference to type 'int' cannot bind to lvalue of type 'int'
    //int && yy = zz; // err, 左值不能绑定到右值引用
    int && xx = std::move(zz); // 将一个左值转移到右值
    cout<<"xx="<<xx<<endl;

//    CCPPMyString tmpMove("abc"); // 实例化一个对象
//    tmpMove = funcString();

//    CCPPMyString tmpst = funcString();
//    cout<<"&tmpst:"<<(void *)&tmpst<<endl;

    CCPPMyString &&objtmp = funcString();
    cout<<"&objtmp:"<<(void *)&objtmp<<endl;


// 相对于左值，右值表示字面常量、表达式、函数的非引用返回值等。
// 引用：给一个内存起一个别名，定义时必须初始化
// 左值引用是对一个左值进行引用的类型，右值引用则是对一个右值进行引用的类型。
    // 左值引用
    int aaaa;
    int &bbbb = aaaa; // ok
    //int &c = 1; // err
    const int &dd = aaaa; // ok
    const int &ee = 1; // ok 常量引用
    const int &ff = func10(); // ok
    const int temp1 = 10;
    const int &gg = temp1;

    // const int & 万能引用

    int abc = 10;
    test9(abc);
    test10(12);

    /*
    {
        // 右值引用
        int && a = 10;
        int && b = func11();
        int i = 10;
        int j = 20;
        int && c = i + j;
    }
     */

    cout<<Test09<2, 3, 4, 5>::val<<endl;

    Car<int, char *, double> bmw;

    expand(1, 2, 3, 4);

    debug(1, 2, 3, 4);
    /**
     * 函数递归调用过程：
     * debug(1, 2, 3, 4);
     * debug(2, 3, 4);
     * debug(3, 4);
     * debug(4);
     * debug();
     */
    debug(1, "mike", "hello", 3.55, 'a');

    func08<int>(10);
    func08<int, int>(10, 20);
    func08<char, int>('a', 10);
    func08<char, char *, int>('a', "abc", 10);


    // 判断2个类型是否一致
    cout<<is_same<int32, my_int>::value<<endl;

    Test tobj;
    cout<<tobj.a<<endl;
    cout<<tobj.b<<endl;

    BB objb(10, 20);
    objb.display();

    B1 objb1;
//    B1 objb2 = objb1; // 拷贝构造

//    objb2 = objb1; // 赋值运算符重载函数

    B1 objb3(10);

//    B1 *bbp1 = new B1;  // operator new被禁用
//    B1 *bbp1 = new B1[10];  // operator new被禁用

    cout<< "abc"_len <<endl;
    cout<< "abc"_str<<endl;
    cout<< 123_test<<endl;

    // 原生字符串字面值
    cout<<R"(hello, \n world)"<<endl;
    string strhello = R"(hello \4 \r
            abc, mike
            hello\n)";
    cout<<endl;
    cout<<strhello<<endl;

    // New string literals
    // 新的字符串字面值
    // 增加了char16_t和char32_t类型
    // 可以直接定义UTF字面值
    // 支持原始字面值(不转义)
    char16_t string[] = uR"*(This is a "raw UTF-16" string.)*";
    std::cout << u8"This is a Unicode Character: \u56fd." << std::endl;
    std::cout << u8R"XXX(I'm a "raw UTF-8" string. )XXX" << std::endl;
    std::wcout << uR"aa(This is a "raw UTF-16" string.)aa" << std::endl;
    std::cout << UR"(This is a"raw UTF-32" string.)" << std::endl;

    // non-constexpr function 'GetNum' cannot be used in a constant expression declared here
    // 枚举成员初始化，必须是整型常量
    //enum {e1 = GetNum(), e2}; // 枚举类型， err
    //enum {e1 = GetNum2(), e2}; // 枚举类型， err

    enum {e1=GetNum3(), e2};

    constexpr int temp = GetNum3(); // ok, 发生在编译阶段
    enum {ta1 = temp, ta2}; // ok

    int *p1 = NULL;
    int *p2 = 0;

    func3(0);
    func3(nullptr);

//    bool tflag = true;
    // 运行时，检查条件，如果条件为真，往下执行，如果条件为假，中断，提示错误
//    assert(tflag == true);

    // 静态断言
//    static_assert("常量条件表达式条件，"提示的字符串");
//    static_assert(sizeof(void *) == 4, "64位系统不支持");

    B obj;
    cout<<obj.data<<endl;
    cout<<obj.data2<<endl;
    cout<<obj.tmp.a<<endl;
    cout<<obj.name<<endl;

//    int &abc = &1;

    auto ia = 10;
    auto ja = 11.2;
    auto jk = mul(ia, ja);
    cout<<"jk = "<<jk<<endl;

    int aaa = 10;
    double bbb = 11.1;
    auto c = func2(aaa, bbb);
    cout<<"c ="<<c<<endl;
    int ii;
    decltype(ii) jj = 0;
    cout<<typeid(jj).name()<<endl;

    float ca;
    double cb;
    decltype(ca + cb) cc;
    cout<<typeid(cc).name()<<endl;
    
    vector<int> tmp;
    decltype(tmp.begin()) kk;
    for (kk = tmp.begin(); kk != tmp.end(); ++kk) {
        cout<<typeid(kk).name()<<endl;
    }

    // 强类型枚举，enum 后面加上class或struct修饰
    enum class Status {Ok, Error};
    enum struct Status2 {Ok, Error};

    //Status sflag = Ok; // err,必须枚举类型的作用域
    Status sflag = Status::Ok;

    // 强类型枚举，可以指定成员变量的类型
    enum struct Status3:char {Ok, Error};
    cout<< sizeof(Status3::Ok)<<endl;

    enum struct Status4:long long {Ok, Error};
    cout<< sizeof(Status4::Ok)<<endl;

    enum{OK, ERROR} flag; // 匿名类型的枚举变量
    decltype(flag) flag2;


    char str1[] = "abc";

    char str2[] = "abc";

    const char str3[] = "abc";

    const char str4[] = "abc";

    const char *str5 = "abc";

    const char *str6 = "abc";

    char *str7 = "abc";

    char *str8 = "abc";

    cout << ( str1 == str2 ) << endl; // 0

    cout << ( str3 == str4 ) << endl; // 0

    cout << ( str5 == str6 ) << endl; // 1

    cout << ( str7 == str8 ) << endl; // 1

    // 因为这是一个深拷贝/浅拷贝的问题.当字符串是数组形式声明并初始化,
    // 编译器认为是新数组,分配新空间,但不是深拷贝,因为根本就不算拷贝.而如果
    // 是相同的字符串,用指针声明,那就是比较如果有一样的字符串,就直接把新指针
    // 指过去,这是正宗的浅拷贝.


    cout<<MyStrlen("hello world")<<endl;   // 7  当用函数传递的数组指针的时候就自动退化为指针了

    char str[] = "hello world";
    int len = sizeof(str) - 1; // 记得减1哦,最后有'\0'结尾
    cout<<len<<endl;   // 11

    int a[5] = {1, 2, 3, 4, 5};

    int *ptr =(int *)(&a + 1);  // &a+1不是首地址+1，系统会认为加一个a数组的偏移，
                                // 是偏移了一个数组的大小（本例是5个int）则ptr实际是&(a[5]),也就是a+5
                                // 原因如下：
    //
    //&a是数组指针，其类型为 int (*)[5];
    //而指针加1要根据指针类型加上一定的值，不同类型的指针+1之后增加的大小不同。
    //a是长度为5的int数组指针，所以要加 5*sizeof(int)
    //所以ptr实际是a[5]
    //但是prt与(&a+1)类型是不一样的(这点很重要)
    //所以prt-1只会减去sizeof(int*)

    //a,&a的地址是一样的，但意思不一样

    //a是数组首地址，也就是a[0]的地址，&a是对象（数组）首地址，

    //a+1是数组下一元素的地址，即a[1],&a+1是下一个对象的地址，即a[5].

    cout<<*(a+1)<<" "<<*(ptr-1)<<endl;  // 2 5
    // 首先,a是一个数组,所以编译器解释&a就是a的全部长度,
    // 就是说(&a+1)也就是说移动了一个数组,指向了并不存在
    // 的a[5],所以ptr-1才会指向a数组的最后一个元素a[4],
    // 而a+1和a[1]是一样的...所以答案是2 5,

    int *ptr2 = (int *)&a + 1;
    cout<<*(ptr2-1)<<endl;     // 1

//    char a2;
//
//    char *str22 = &a2;
//
//    strcpy(str22, "hello");
//
//    cout<<str22;

//    char* s="AAA";
//    "AAA"是字符串常量。s是指针，指向这个字符串常量，所以声明s的时候就有问题。
//    cosnt char* s="AAA";
//
//    cout<<s<<endl;
//
//    s[0]='B';
//
//    cout<<s<<endl;
// 输出AAA后就崩溃了..为什么?因为当你在第一句初始化的时候,编译器就认为
// 这是个字符串常量了...再做数组操作的时候肯定错了罗...最好的习惯是声明
// 一个指针,用new分配空间,然后用库函数操作,比如strcpy,strcat等等...

    int (*s[10])(int);

    s[0] = fun;
    s[1] = fun;
    s[2] = fun;

    cout<<s[0]<<s[1]<<s[2]<<endl;

    // 咦...这是什么?其实这是一个函数指针数组,指向了一组int fun(int)的函数,第一眼确实让人有点迷糊...但是请习惯这样...

    char *str11 = NULL;

    GetMemory(&str11);

    cout<<str11<<endl;  // hello world

    delete []str11;  // delete 只是释放的str指向的内存空间,它本身的值还是存在的.所以delete之后，有一个好的习惯就是将str=NULL.

    str11 = NULL;

    char a3[10];

    cout<<strlen(a3)<<endl;

    char (*str12)[20]; //str12是一个数组指针，即指向数组的指针．

    char *str13[20]; //str13是一个指针数组，其元素为指针型数据．

    // char * const p; //常量指针，p的值不可以修改

    // char const * p；//指向常量的指针，指向的常量值不可以改

    // const char *p； //和char const *p

    char str14[] = "aBcDe";

    cout << "str字符长度为: " << sizeof(str14)/sizeof(str14[0]) << endl;

    UpperCase(str14);

    cout<<str14<<endl;

//    char szstr[10];

//    strcpy(szstr,"0123456789");

//    cout<<szstr<<endl;

//    typedef void(*)() voidFuncPtr;
//
//    *((voidFuncPtr)0x100000)();

//    *((void (*)( ))0x100000 ) ( );

    char aa[10];

    printf("%d", strlen(aa));
    // sizeof()和初不初始化，没有关系；
    // strlen()和初始化有关。

    return 0;
}