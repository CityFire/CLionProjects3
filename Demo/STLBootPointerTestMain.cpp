//
// Created by wjc on 2023/3/29.
//
#include <boost/scoped_ptr.hpp>
#include <vector>
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <random>
#include <utility>
using namespace std;

class X
{
public:
    X() { cout <<"X..."<<endl; }
    ~X() { cout<<"~X..."<<endl; }
};

/// 强类型枚举
enum class MyEum : char {
    NAME = 1,
    SEX = 2,
    AGE = 3,
};

MyEum val = MyEum::SEX;

class Base {
public:
    virtual int fun(int par1, float par2);
    virtual void fun2() final;
};
//override明确虚拟继承关系 final阻止类或方法被改写
class Drived final: public Base {
public:
    virtual int fun(int par1, float par2) override;
};

//SomeClass obj = {0.8f, 8};
//SOmeClass aobj[] = {{0.8f, 8}, {0.9f, 9}};

std::map<std::string, int> con = {{"pan", 8}, {"kai", 9}};

// 初始化列表和统一初始化
float fun(std::initializer_list<float> paras) {
    float res = 0.0f;
    std::for_each(paras.begin(), paras.end(), [&res](float x) { res += x; });
    return res;
}

// Type inference
// 类型推导
// auto - 自动推导变量的类型 decltype - 取得表达式的类型
float fun2(std::initializer_list<float> paras) {
    float res = 0.0f;
    auto sum = [&res](float x) { res += x; };
//    decltype(sum) avg;
    std::for_each(paras.begin(), paras.end(), sum);
    return res;
}

// Range-based for loop
// 基于区间的迭代
// C风格数组
// 有begin和end的容器
float fun3(std::initializer_list<float> paras) {
    float res = 0.0f;
    for (auto &x: paras) {
        res += x;
    }
    return res;
}

template <typename call_helper_t>
struct event_slot {
    void add_event_handler(void *handler, call_helper_t ch) {
        m_hander.push_back(std::make_pair(handler, ch));
    }
    template <typename... Args> bool fire_event(Args...args) {
        for_each(begin(m_hander), end(m_hander), [&](call_pair_t & val) {
            val.second(val.first, args...);
        });
        return true;
    }
private:
    typedef std::pair<void*, call_helper_t> call_pair_t;
    std::vector<call_pair_t> m_hander;
};

class some_producer {
public:
    typedef std::function<bool (void*, int, float)> event1_t;
    typedef std::function<bool (void*, char*)> event2_t;

    void do_sth() {
        m_event1_slot.fire_event(9, 10.0f);
        char * str = "test";
        m_event2_slot.fire_event(str);
    }

    event_slot<event1_t> m_event1_slot;
    event_slot<event2_t> m_event2_slot;
};

// Threading facilities
// 线程
std::mutex g_mutex;
std::vector<std::exception_ptr> g_exceptions;

void throw_function() {
    throw std::exception();
}

void func() {
    try {
        throw_function();
    } catch (...) {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_exceptions.push_back(std::current_exception());
    }
}

// Lambda函数和表达式
double eval(std::function<double(double)> f, double x = 2.0) { return f(x); }

struct MyClass {
    MyClass() = default;
    MyClass(int par);
    MyClass(const MyClass &) = delete;
    MyClass& operator=(const MyClass&) = delete;
    void fun(int par) { cout << par<<endl; }

    template <typename T> void fun(T) = delete;
    int m_val;
};
// Explicitly defaulted and deleted special member functions
// 缺省特殊成员的引用和禁用

void funct1(int par) {}
void funct2(char * par) {}

void printDouble(const double& rd)
{
    cout<<rd<<endl;
}

void printDouble(const double *pd)
{
    if (pd) {
        cout<<*pd<<endl;
    }
}

struct Testable
{
    explicit operator bool() const {
        return false;
    }
};

// Tuple types
// 元组
typedef std::tuple<double, char, std::string> student_t;
student_t get_student(int id) {
    if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2) return student_t(1.7, 'D', "Ralph Wiggm");
    throw std::invalid_argument("id");
}

// constexpr – Generalized constant expressions
// 泛化的常数表达式
// 可以定义非整型和枚举的常量
constexpr int get_five() { return 5; }

// Extern template
// 外部模板
// 避免没必要的模板实例化，节约编译时间
// 必须确保会有一个编译单元会实例化该模板
extern template class std::vector<MyClass>;

// Alternative function syntax
// 新语法-后置函数返回类型
// 适用于复杂模板函数的返回值推演(配合decltype)
template<class T, class U>
auto mul(T x, U y) -> decltype(x*y)
{
    return x*y;
}

// Alias templates
// 模板别名
// using的新语义
template <typename FIRST, typename SECOND, typename THIRD>
class MyTemp {};
template <typename SECOND>
using MyAlias = MyTemp<int, SECOND, float>;

// Unrestricted unions
// 无限制联合体
// 非默认构造类型可用于联合体
struct Point {
    Point() {}
    Point(int x, int y) : x_(x), y_(y) {}
    int x_, y_;
};

union U {
    int z;
    double w;
    Point p;
    U() {new(&p) Point();}
};

// User-defined literals
// 自定义字面值
//OutputType operator "" _suffix(unsigned long long);
//OutputType operator "" _suffix(long double);
//
//OutputType some_variable = 1234_suffix;
//OutputType another_variable = 3.1416_suffix;

// Thread-local storage
// 线程本地存储
// 设计良好的程序不应该需要此功能

//  Allow sizeof to work on members of classes without an explicit object
// sizeof可以用于不明确对象的成员变量
// 方便在模板中进行大小计算
//int s = sizeof(MyClass::m_val);
//s = sizeof(v1.m_val);

//  Allow garbage collected implementations
// 允许垃圾回收实现
// void declare_reachable(void *p)
// template <class T> T * undeclare_reachable(T *p)

// Attributes
// 属性
void funcs [[noreturn]] (int par) {
    throw "error";
}

struct foo* f [[carries_dependency]] (int i);  // hint to optimizer
int* g(int* x, int* y [[carries_dependency]]);

// Wrapper reference
// 引用包装
void functi (int &r) { r++; }
template <class F, class P> void g(F f, P t) { f(t); }

//  Uniform method for computing the return type of function objects
// 得到仿函数返回值类型
struct Confused {
    double operator()(int) const;    // The parameter type is not
    int    operator()(double) const; // equal to the return type.
};

template <class Obj>
class CalculusVer2 {
public:
    template <class Arg>
    typename std::result_of<Obj(Arg)>::type operator()(Arg& a) const {
        return member(a);
    }
private:
    Obj member;
};

int main(void)// CPlusPlus11
{
    Testable aa, bb;
    if (aa) {   }
//    if (aa > bb) {  }

//    scoped_lock

    // Multithreading memory model
    // 多线程内存模型
    // Given a program and an execution trace of that program, whether
    // the execution trace is a legal execution of the program.
    // 主要是规范编译器对代码的优化，确保可移植的多线程程序。

    // Static assertions
    // 编译时断言
    // 用于编译期进行断言
    // 主要用于在模板实例化时对类型参数进行检查


    auto student0 = get_student(0);
    std::cout<<"ID: 0, "<<"GPA: "<<std::get<0>(student0)<<", "<<"grade: "<<std::get<1>(student0)
            <<", "<<"name: "<<std::get<2>(student0)<<'\n';

    double gpa1;
    char grade1;
    std::string name1;
    std::tie(gpa1, grade1, name1) = get_student(1);
    std::cout<<"ID: 1"<<"GPA: "<<gpa1<<", "<<"grade: "<<grade1
    <<", "<<"name: "<<name1<<'\n';
    int size = std::tuple_size<student_t>::value;
    std::tuple_element<1, student_t>::type val = std::get<1>(student0);
    cout<<val<<endl;

    // Extensible random number facility
    // 可扩展随机数装置
    std::uniform_int_distribution<int> distribution(0, 99);
    std::mt19937 engine;
    auto generator = std::bind(distribution, engine);
    int random = generator();
    int random2 = distribution(engine);
    cout<<"random="<<random<<" "<<"random2="<<random2<<endl;

    /*
     * Remained Features
• constexpr – Generalized constant expressionsNull pointer constant
• Extern template
• Alternative function syntax
• Right angle bracket
• Alias templates
• Unrestricted unions
• User-defined literals
• Thread-local storage
• Allow sizeof to work on members of classes without an explicit object
• Control and query object alignment
• Allow garbage collected implementations
• Attributes
• Wrapper reference
• Type traits for metaprogramming
• Uniform method for computing the return type of function objects
     */

    // constexpr – Generalized constant expressions
    // 泛化的常数表达式
    // 可以定义非整型和枚举的常量
    int some_value[get_five() + 7];  // Create an array

    // Extern template
    // 外部模板
    // 避免没必要的模板实例化，节约编译时间
    // 必须确保会有一个编译单元会实例化该模板
//    extern template class std::vector<MyClass>;

//    auto multiply = mul(4 * 1.5);
//    cout<<multiply<<endl;

// Right angle bracket
// 右尖括号
// 编译器优先解释>>为模板结束符号，而不是位移运算符

// Alias templates
// 模板别名
// using的新语义
//    using MyAlias = MyTemp<int, SECOND, float>;

    MyAlias<char> valu;

    // Unrestricted unions
    // 无限制联合体
    // 非默认构造类型可用于联合体

    // User-defined literals
    // 自定义字面值
//    OutputType operator "" _suffix(unsigned long long);

// Control and query object alignment
// 对象对齐
// alignas
// alignof
// 更为方便的查询和控制到变量的内存对齐情况
    // array of characters, suitably aligned for doubles
    alignas(double) unsigned char c[1024];
//    alignas(16) char [100];    // align on 16 byte boundary

//  Allow garbage collected implementations
// 允许垃圾回收实现

// Wrapper reference
// 引用包装
   int i = 0;
    g(functi, i);// 'g<void(int &r), int>' is instantiated
    std::cout<<i<<std::endl;   // output -> 0

    g(functi, std::ref(i));  // 'g<void(int &r),reference_wrapper<int>>' is instantiated
    std::cout<<i<<std::endl;  // output -> 1

    // Type traits for metaprogramming
    // 元编程
    // 即程序生成程序，(机器学习等)
    // C++通过在编译期利用模板生成代码来实现

    //  Uniform method for computing the return type of function objects
    // 得到仿函数返回值类型

    // Meditation of Programming Language
    //• 向后兼容，编程语言不可承受之重
    //• 简洁的核心语言加丰富高效的标准库(C++心里永远的疼) • 运行时智能化，语言灵活化
    //• 越普适，越无用
    //• 了解其他编程语言，用最适合的工具解决问题


// Null pointer constant
// 空指针常数
// 避免0产生岐义
    funct1(NULL);
    funct2(nullptr);

    // New string literals
    // 新的字符串字面值
    // 增加了char16_t和char32_t类型
    // 可以直接定义UTF字面值
    // 支持原始字面值(不转义)
    char16_t str[] = uR"*(This is a "raw UTF-16" string.)*";
    std::cout<<u8"This is a Unicode Character: \u56fd."<<std::endl;
    std::cout<<u8R"XXX(I'm a "raw UTF-8" string. )XXX"<<std::endl;
    std::wcout<<uR"aa(This is a "raw UTF-16" string.)aa"<<std::endl;
    std::cout<<UR"(This is a"raw UTF-32" string.)"<<std::endl;

    // 总的来说，在以下情况下你应该使用指针，一是你考虑到存在不指向任何对象
    // 的可能(在 这种情况下，你能够设置指针为空)，二是你需要能够在不同的时
    // 刻指向不同的对象(在这 种情况下，你能改变指针的指向)。如果总是指向一
    // 个对象并且一旦指向一个对象后就不会 改变指向，那么你应该使用引用。
    string s1("Nancy");
    string s2("Clancy");
    string& rs = s1;  // rs引用s1
    string *ps = &s1; // ps指向s1
    rs = s2;          // rs仍旧引用s1, 但是s1的值现在是Clancy
    ps = &s2;         // ps现在指向s2  s1没有改变

    // Rvalue references & move constructor
    // 右值引用和移动构造
    // T &&
    // 允许右值临时变量在初始化完成后被改变
    // 避免复杂对象的深拷贝
    // 支持move构造的对象，在做为参数传递、结果值返回时可以避免不 必要的拷贝
//    std::vector<int> readFile();
//    std::vector<int> myInt = readFile();
//    for (int val: readFile()) {  }

    MyClass v1;
//    MyClass v2(v1);
//    MyClass v3 = v1;
    v1.fun(8);
//    short  i = 8;
//    v1.fun(i);

    const char *reg_esp = "[ ,.\\t\\n;:]";
//    std::regex rgx(reg_esp);
// Lambda函数和表达式
    std::function<double(double)> f0    = [](double x){ return 1; };
    auto                          f1    = [](double x){ return 1; };
    decltype(f0)                  fa[3] = {f0, f1, [](double x) { return x*x; }};
    std::vector<decltype(f0)>     fv    = {f0, f1};
    fv.push_back                  ([](double x) { return x*x; });
    for(int i = 0;i<fv.size();i++) std::cout<<fv[i](2.0)<<endl;
    for(int i = 0;i<3;i++)         std::cout<<fa[i](2.0)<<endl;
    for(auto &f : fv)              std::cout<<f(2.0)<<endl;
    for(auto &f : fa)              std::cout<<f(2.0)<<endl;
    std::cout<<eval(f0)<<endl;
    std::cout<<eval(f1)<<endl;
    std::cout<<eval([](double x){return x*x;})<<endl;

    some_producer p;
    p.do_sth();

    g_exceptions.clear();
    std::thread t(func);
    t.join();
    for (auto & e : g_exceptions) {
        try {
            if (e != nullptr) std::rethrow_exception(e);
        } catch (const std::exception & e) {
            std::cout<<e.what()<<std::endl;
        }
    }

    // Variadic templates
    // 变参模板
    // 模板类的类型参数可变
    // 模板函数的参数可变
    // 可以方便的实现元组容器


    float  f = fun3({1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f});
    cout<<f<<endl;

    cout<<"Entering main..."<<endl;
    {
//        boost::scoped_ptr<X> p(new X);
    }
    cout<<"Exiting main..."<<endl;
    return 0;
}