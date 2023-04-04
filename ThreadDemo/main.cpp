#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <list>
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

    thread th(ThreadWrite);
    th.detach();
    for (int i = 0; i < 3; ++i) {
        this_thread::sleep_for(100ms);
        thread th(ThreadRead, i + 1);
        th.detach();
    }
    getchar();
    return 0;
}
