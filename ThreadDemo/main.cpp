#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <sstream>
#include <list>
#include <queue>
#include <string>
#include "XThread.h"
#include "XMsgServer.h"
using namespace std;

static mutex mux1;
static  mutex mux2;

void TestScope1()
{
    this_thread::sleep_for(1000ms); // 模拟死锁 停100ms等另一个线程锁mux2
    cout<<this_thread::get_id()<<" TestScope1 begin mux1 lock"<<endl;
//    mux1.lock();
    cout<<this_thread::get_id()<<" TestScope1 begin mux2 lock"<<endl;
//    mux2.lock(); // 死锁
    // c++11
//     lock(mux1, mux2);
    // c++17
    scoped_lock lock(mux1, mux2); // 解决死锁

    cout<<"TestScope1"<<endl;
    this_thread::sleep_for(1000ms);

//    mux1.unlock();
//    mux2.unlock();
}

void TestScope2()
{
    cout<<this_thread::get_id()<<" TestScope2 begin mux2 lock"<<endl;
    mux2.lock();
    this_thread::sleep_for(100ms);
    cout<<this_thread::get_id()<<" TestScope2 begin mux1 lock"<<endl;
    mux1.lock(); // 死锁
    cout<<"TestScope2"<<endl;
    this_thread::sleep_for(1500ms);
    mux1.unlock();
    mux2.unlock();
}

void TestScope3()
{
    this_thread::sleep_for(1000ms); // 模拟死锁 停100ms等另一个线程锁mux2
    cout<<this_thread::get_id()<<" TestScope1 begin mux1 lock"<<endl;
//    mux1.lock();
    cout<<this_thread::get_id()<<" TestScope1 begin mux2 lock"<<endl;
//    mux2.lock(); // 死锁
    // c++11
    lock(mux1, mux2);
    // c++17
//    scoped_lock lock(mux1, mux2); // 解决死锁

    cout<<"TestScope1"<<endl;
    this_thread::sleep_for(1000ms);

    mux1.unlock();
    mux2.unlock();
}

int mainScope() {

    cout<<"hello world!"<<endl;
    {
        // 演示死锁情况
        {
            thread th(TestScope1);
            th.join();
        }
        {
            thread th(TestScope2);
            th.join();
        }
    }
    return 0;
}



class TestXThread : public XThread
{
public:
    void Main() override
    {
        cout<<"TestXThread Main begin"<<endl;
        while (!is_exit())
        {
            this_thread::sleep_for(100ms);
            cout<<"."<<flush;
        }
        cout<<"TestXThread Main end"<<endl;
    }
    string name;
};

// 条件变量
list<string> msgs_;
mutex mux;
condition_variable cv;
void ThreadWrite()
{
    for (int i = 0;;i++)
    {
        stringstream ss;
        ss<<" Write msg "<<i;
        unique_lock<mutex> lock(mux);
        msgs_.push_back(ss.str());
        lock.unlock();
        cv.notify_one(); // 发送信号
//        cv.notify_all();
        this_thread::sleep_for(1s);
    }
}

void ThreadRead(int i)
{
    for (;;)
    {
        cout<<" read msg "<<endl;
        unique_lock<mutex> lock(mux);
//        cv.wait(lock); // 解锁、阻塞等待信号
        cv.wait(lock, [i]
            {
                cout<<i<<" wait"<<endl;
//                return false;
                return !msgs_.empty();
            });
        // 获取信号后锁定
        while (!msgs_.empty())
        {
            cout<<i<<" read "<<msgs_.front()<<endl;
            msgs_.pop_front();
        }
    }
}

// 缓冲区存储的数据类型
struct CacheData
{
    // 商品id
    int id;
    // 商品属性
    string data;
};

queue<CacheData> Q;
// 缓冲区最大空间
const int MAX_CACHEDATA_LENGTH = 10;
// 互斥量，生产者之间，消费者之间，生产者和消费者之间，同时都只能一个线程访问缓冲区
mutex m;
condition_variable condConsumer;
condition_variable condProducer;
// 全局商品id
int ID = 1;

// 消费者动作
void ConsumerActor()
{
    unique_lock<mutex> lockerConsumer(m);
    cout<<"["<<this_thread::get_id()<<"] 获取了锁"<<endl;
    while (Q.empty())
    {
        cout<<"因为队列为空，所以消费者Sleep"<<endl;
        cout<<"["<<this_thread::get_id()<<"] 不再持有锁"<<endl;
        condConsumer.wait(lockerConsumer);
        cout<<"["<<this_thread::get_id()<<"] Weak, 重新获取了锁"<<endl;
    }
    cout<<"["<<this_thread::get_id()<<"]";
    CacheData temp = Q.front();
    cout<<"消费者 - ID:"<<temp.id<<" Data:"<<temp.data<<endl;
    Q.pop();
    condProducer.notify_one();
    cout<<"["<<this_thread::get_id()<<"] 释放了锁"<<endl;
}

// 生产者动作
void ProducerActor()
{
    unique_lock<mutex> lockerProducer(m);
    cout<<"["<<this_thread::get_id()<<"] 获取了锁"<<endl;
    while (Q.size() > MAX_CACHEDATA_LENGTH)
    {
        cout<<"因为队列为满，所以生产者Sleep"<<endl;
        cout<<"["<<this_thread::get_id()<<"] 不再持有锁"<<endl;
        // 队列满，生产者停止，等待消费者唤醒
        condProducer.wait(lockerProducer);
        cout<<"["<<this_thread::get_id()<<"] Weak, 重新获取了锁"<<endl;
    }
    cout<<"["<<this_thread::get_id()<<"]";
    CacheData temp;
    temp.id = ID++;
    temp.data = "*****";
    cout<<"生产者 + ID:"<<temp.id<<" Data:"<<temp.data<<endl;
    Q.push(temp);
    condConsumer.notify_one();
    cout<<"["<<this_thread::get_id()<<"] 释放了锁"<<endl;
}

// 消费者
void ConsumerTask()
{
    while (1)
    {
        ConsumerActor();
    }
}

// 生产者
void ProducerTask()
{
    while (1)
    {
        ProducerActor();
    }
}

// 管理线程的函数
void Dispatch(int ConsumerNum, int ProducerNum)
{
    vector<thread> thsC;
    for (int i = 0; i < ConsumerNum; ++i) {
        thsC.push_back(thread(ConsumerTask));
    }

    vector<thread> thsP;
    for (int j = 0; j < ProducerNum; ++j) {
        thsP.push_back(thread(ProducerTask));
    }

    for (int i = 0; i < ConsumerNum; ++i) {
        if (thsC[i].joinable())
        {
            thsC[i].join();
        }
    }

    for (int j = 0; j < ProducerNum; ++j) {
        if (thsP[j].joinable())
        {
            thsP[j].join();
        }
    }
}

/*
 * 创建线程时的传参问题分析
如“std::thread th1(proc1)”,创建线程时需要传递函数名作为参数，提供的函数对象会复制到新的线程的内存空间中执行与调用。

如果用于创建线程的函数为含参函数，那么在创建线程时，要一并将函数的参数传入。常见的，传入的参数的形式
 有基本数据类型(int，char,string等)、引用、指针、对象这些，下面总结了传递不同形式的参数时std::thread类
 的处理机制，以及编写程序时候的注意事项。本章节只给出了部分示例代码，没有必要为了证明处理机制而举例大量简单代码
 而使得文章冗长，但是推荐新手自行编写程序研究。

总体来说，std::thread的构造函数会拷贝传入的参数:
1.当传入参数为基本数据类型(int，char,string等)时，会拷贝一份给创建的线程；
2.当传入参数为指针时，会浅拷贝一份给创建的线程，也就是说，只会拷贝对象的指针，不会拷贝指针指向的对象本身。
3.当传入的参数为引用时，实参必须用ref()函数处理后传递给形参，否则编译不通过，此时不存在“拷贝”行为。引用
 只是变量的别名，在线程中传递对象的引用，那么该对象始终只有一份，只是存在多个别名罢了（注意把引用与指针区
 别开：指针是一块内存指向另一块内存，指针侧重“指向”二字；引用是只有一块内存，存在多个别名。理解引用时不要
 想着别名“指向”内存，这是错误的理解，这样的理解会导致分不清指针和引用，别名与其本体侧重于“一体”二字，引用
 就是本体，本体就是引用，根本没有“指向”关系。）；
 4.当传入的参数为类对象时，会拷贝一份给创建的线程。此时会调用类对象的拷贝构造函数。

 使用detach()时，可能存在主线程比子线程先结束的情况，主线程结束后会释放掉自身的内存空间；在创建线程时，
 如果std::thread类传入的参数含有引用或指针，则子线程中的数据依赖于主线程中的内存，主线程结束后会释放
 掉自身的内存空间，则子线程会出现错误。
 */
void proc(int& x)
{
    cout<<x<<","<<&x<<endl;
}

int main(void)
{
    XMsgServer server;
    server.Start();
    for (int i = 0; i < 10; ++i) {
        stringstream ss;
        ss<<"msg:"<<i + 1;
        server.SendMsg(ss.str());
        this_thread::sleep_for(500ms);
    }
    server.Stop();

//    std::condition_variable cv; // 信号量
    // 1.获得std::mutex(常通过std::unique_lock)
//    unique_lock(mux);
    // 2.在获取锁时进行修改
    // 3.释放锁并通知读取线程
//    lock.unlock();
//    cv.notify_one(); // 通知一个等待信号线程
//    cv.notify_all(); // 通知所有等待信号线程

//    thread th(ThreadWrite);
//    th.detach();
//    for (int i = 0; i < 3; ++i) {
//        this_thread::sleep_for(100ms);
//        thread th(ThreadRead, i + 1);
//        th.detach();
//    }

    // 一个消费者线程，5个生产者线程，则生产者经常要等待消费者
//    Dispatch(1, 5);


    int a = 10;
    cout<<a<<","<<&a<<endl;
    thread t1(proc, ref(a));
    t1.join();

    getchar();

    return 0;
}
