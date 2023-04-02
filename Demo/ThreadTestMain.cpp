//
// Created by wjc on 2023/3/30.
//
#include <vector>
//#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <iostream>
//#include <random>
//#include <utility>
#include <future>
#include <atomic>
#include <boost/timer.hpp>
using namespace std;

class ThreadSingleton {
public:
    static ThreadSingleton& instance()
    {
        static ThreadSingleton s;
        return s;
    }
    void add(int n)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        v_.emplace_back(n);
    }
    int count() const
    {
        return v_.size();
    }
private:
    ThreadSingleton() {}
public:
    ThreadSingleton(ThreadSingleton const&) = delete;
    void operator=(ThreadSingleton const&) = delete;
private:
    std::mutex mutex_;
    std::vector<int> v_;
};

// 以使用任何可Callable的类型，来构造std::thread，比如函数对象
class background_task
{
public:
    background_task()
    {
        cout<<"background_task..."<<endl;
    }
    background_task(const background_task& other) {
        cout<<"copy background_task..."<<endl;
    }
    ~background_task()
    {
        cout<<"~background_task..."<<endl;
    }
    void operator()() const
    {
        do_something();
        do_something_else();
    }
    void do_something() const
    {
        cout<<"do_something..."<<endl;
    }
    void do_something_else() const
    {
        cout<<"do_something_else..."<<endl;
    }
};

void do_some_work(int num) {
    std::cout<<"thread: "<<this_thread::get_id()<<" "<<num<<std::endl;
}

void hello()
{
    std::cout << "Hello std::thread!" << std::endl;
}

void do_something()
{
    cout<<"do_something"<<endl;
}
void do_something_else()
{
    cout<<"do_something_else"<<endl;
}

void foo(int m, int n)
{
    std::cout<<m<<" "<<n<<std::endl;
}

struct Foo {
    void bar(int n) {
        std::cout<<n<<std::endl;
    }
};

void example01()
{
    // 返回值
    // 我们如何从线程中拿到返回值呢？
    // Lambda + 引用捕获
    int r = 0;
    std::thread t([&r]() {
        r = 10;
    });
    t.join();
    std::cout<<r<<std::endl;
}

void foo2(int m, int n, std::promise<int>&& p)
{
    p.set_value(m * n);
}

int foo3(int m, int n)
{
    return m * n;
}

void example02()
{
    // 使用std::promise和std::future
    // std::promise和std::future提供了一种可以从异步方法中拿到执行结果的机制。
    std::promise<int> p;
    auto f = p.get_future();
    std::thread t(&foo2, 5, 6, std::move(p));
    t.join();
    int i = f.get();
    std::cout<<i<<std::endl; // 30

    // 使用std::async
    // 使用std::async可以简化上面例子的写法，它异步的执行一个函数，返回std::future类型的结果
    auto f2 = std::async(foo3, 5, 6);
    std::cout<<f2.get()<<std::endl;
}

std::atomic<bool> stop_flag {false};
void fooo()
{
    while (true) {
        cout<<"fooo start loop..."<<endl;
        if (stop_flag) {
            cout<<"fooo end loop..."<<endl;
            return;
        }
        // do something
        cout<<"fooo do something..."<<endl;
    }
}

std::atomic<long> total{0};
void func(int ) {
    for (long long i = 0; i < 100000LL; ++i) {
        total += i;
        cout<<i<<endl;
    }
}

int mainThread(void)
{
    int threadNums = 3;
    std::vector<std::thread> threadList;
    threadList.reserve(threadNums);

//    std::thread::id

    // 1 创建threadNums个线程
    for (int idx = 0; idx < threadNums; ++idx) {
//        std::thread trd{do_some_work, idx};
//        threadList.push_back(trd);  // error
//        threadList.push_back(std::move(trd));    // ok
//        threadList.emplace_back(std::move(trd)); // ok
        threadList.emplace_back(std::thread{do_some_work, idx});
    }



    std::cout<<"work in main thread"<<std::endl;

    // 2.终止threadNums个线程
    for (int idx = 0; idx < threadNums; ++idx) {
        threadList[idx].join();
    }

    std::cout<<"main thread end"<<std::endl;


    std::thread t(hello);
    t.join();


    background_task f;
    std::thread my_thread(f);
    my_thread.join();
//    my_thread.detach();

//  使用Lambda
//  或者我们可以使用匿名函数
    std::thread my_thread2([]() {
         do_something();
         do_something_else();
    });
    my_thread2.detach();

    std::thread t1([]() {
        for (int i = 0; i < 10; ++i) {
            std::cout<<"in t1: "<<i + 1<<std::endl;
        }
    });
//    t1.join();  // join会等待线程直到执行完成
    t1.detach();  // detach用来在后台运行线程
    std::cout<<"t1 done."<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 在std::thread构造函数中添加额外的参数就好了
    std::thread t2(foo, 5, 4);
    t2.join();

    // 给类的成员函数传参，要加上对象指针
    Foo foo;
    std::thread t3(&Foo::bar, &foo, 5);
    t3.join();

    example01();

    example02();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(thread([](int i) {
            ThreadSingleton::instance().add(i);
        }, i));
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout<<ThreadSingleton::instance().count()<<std::endl;




    std::thread t5(fooo);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 让子弹飞一会儿
    stop_flag = true;
    t5.join();


    // 同时跑几个线程合适？
    // 推荐使用std::thread::hardware_concurrency(), 在多核架构的运行环境上，这个返回值一般对应核的颗数。
    cout<<"hardware_concurrency: "<<std::thread::hardware_concurrency()<<endl;

    // 获取当前线程的标识
    // 使用std::this_thread::get_id()

    // std::async支持两种策略
    // 默认使用std::launch::async, 惰性调用时使用std::launch::deferred

    boost::timer tm;
    std::cout << tm.elapsed() << std::endl;


    thread th1(func, 0);
    thread th2(func, 0);
    th1.join();
    th2.join();
    cout<<total<<endl;

    return 0;
}

