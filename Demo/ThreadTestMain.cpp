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
using namespace std;

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

int main(void)
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

    return 0;
}