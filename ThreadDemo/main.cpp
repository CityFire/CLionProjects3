#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <sstream>
#include <list>
#include <queue>
#include <set>
#include <string>
#include <iomanip>
#include <math.h>
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

std::mutex mlock;

void work1(int& s)
{
    cout<<"s: "<<s<<endl;
    for (int i = 0; i <= 50; ++i) {
        std::unique_lock<std::mutex> munique(mlock, std::try_to_lock);
        if (munique.owns_lock() == true) {
            s += i;
        }
        else {
            // 执行一些没有共享内存的代码
        }
    }
}

void work2(int& s)
{
    cout<<"s: "<<s<<endl;
    for (int i = 51; i <= 100; ++i) {
        std::unique_lock<std::mutex> munique(mlock, std::try_to_lock);
        if (munique.owns_lock() == true) {
            s += i;
        }
        else {
            // 执行一些没有共享内存的代码
        }
    }
}

std::mutex mlock2;
void work3(int& s)
{
    cout<<"s: "<<s<<endl;
    //  当使用了defer_lock参数时，在创建了unique_lock的对象时就不会自动加锁，
    //  那么就需要借助lock这个成员函数来进行手动加锁，当然也有unlock来手动解锁。
    //  这个和mutex的lock和unlock使用方法一样
    for (int i = 1; i <= 50; i++) {
        std::unique_lock<std::mutex> munique(mlock, std::defer_lock);
        munique.lock();
        s += i;
        munique.unlock(); // 这里可以不用unlock，可以通过unique_lock的析构函数unlock
    }
}

void work4(int& s) {
    cout<<"s: "<<s<<endl;
    for (int i = 51; i <= 100; i++) {
        std::unique_lock<std::mutex> munique(mlock, std::defer_lock);
        munique.lock();
        s += i;
        munique.unlock();
    }
}

// 还有一个成员函数是try_lock，和上面的try_to_lock参数的作用差不多，判断当前是否能lock，
// 如果不能，先去执行其他的代码并返回false，如果可以，进行加锁并返回true
void work5(int& s) {
    for (int i = 1; i <= 5000; i++) {
        std::unique_lock<std::mutex> munique(mlock, std::defer_lock);
        if (munique.try_lock() == true) {
            s += i;
        }
        else {
            // 处理一些没有共享内存的代码
        }
    }
}

// release函数，解除unique_lock和mutex对象的联系，并将原mutex对象的指针返回出来。
// 如果之前的mutex已经加锁，需在后面自己手动unlock解锁
void work6(int& s) {
    for (int i = 1; i <= 5000; i++) {
        std::unique_lock<std::mutex> munique(mlock);   // 这里是自动lock
        std::mutex *m = munique.release();
        s += i;
        m->unlock();
    }
}

// unique_lock的所有权的传递
//       对越unique_lock的对象来说，一个对象只能和一个mutex锁唯一对应，
//       不能存在一对多或者多对一的情况，不然会造成死锁的出现。所以如果想
//       要传递两个unique_lock对象对mutex的权限，需要运用到移动语义或
//       者移动构造函数两种方法。
// 移动语义：
//std::unique_lock<std::mutex> munique1(mlock);
//std::unique_lock<std::mutex> munique2(std::move(munique1));
// 此时munique1失去mlock的权限，并指向空值，munique2获取mlock的权限

// 移动构造函数：
std::unique_lock<std::mutex> rtn_unique_lock()
{
    std::unique_lock<std::mutex> tmp(mlock);
    return tmp;
}

void work7(int& s) {
    for (int i = 1; i <= 5000; i++) {
        std::unique_lock<std::mutex> munique2 = rtn_unique_lock();
        s += i;
    }
}

void print_time() {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss<<put_time(localtime(&in_time_t), "%Y-%m-%d %X");
    cout<<"now is: "<<ss.str()<<endl;
}

void sleep_thread()
{
    this_thread::sleep_for(chrono::seconds(3));
    cout << "[thread-" << this_thread::get_id() << "] is waking up" << endl;
}

void loop_thread() {
    for (int i = 0; i < 10; i++) {
        cout << "[thread-" << this_thread::get_id() << "] print: " << i << endl;
    }
}

void init() {
    cout << "Initialing..." << endl;
    // Do something...
}

void worker(once_flag* flag) {
    call_once(*flag, init);         // 第一种写法
//    static std::once_flag flag2;  // 第二种写法 与call_once配合使用
//    std::call_once(flag2, init);
}

static const int MAX = 10e8; // ①
static double sum = 0; // ②

void serial_worker(int min, int max) { // ③
    for (int i = min; i <= max; i++) {
        sum += sqrt(i);
    }
}

void serial_task(int min, int max) {
    auto start_time = chrono::steady_clock::now();
    sum = 0;
    serial_worker(0, MAX);
    auto end_time = chrono::steady_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << "Serail task finish, " << ms << " ms consumed, Result: " << sum << endl;
}

// 我们用锁的粒度（granularity）来描述锁的范围。细粒度（fine-grained）是指锁保护较小的范围，
// 粗粒度（coarse-grained）是指锁保护较大的范围。出于性能的考虑，我们应该保证锁的粒度尽可能的细。
// 并且，不应该在获取锁的范围内执行耗时的操作，例如执行IO。如果是耗时的运算，也应该尽可能的移到锁的外面。
static mutex exclusive;

void concurrent_worker(int min, int max) {
    double tmp_sum = 0; // 通过一个局部变量保存当前线程的处理结果
    for (int i = min; i <= max; i++) {
        tmp_sum += sqrt(i);
    }
    exclusive.lock(); // 在汇总总结过的时候进行锁保护
    sum += tmp_sum;
    exclusive.unlock();
}

void concurrent_task(int min, int max) {
    auto start_time = chrono::steady_clock::now();

    unsigned concurrent_count = thread::hardware_concurrency(); // 获取到当前硬件支持多少个线程并行执行
    cout << "hardware_concurrency: " << concurrent_count << endl;
    vector<thread> threads;
    min = 0;
    sum = 0;
    for (int t = 0; t < concurrent_count; ++t) {  // 根据处理器的情况决定线程的数量
        int range = max / concurrent_count * (t + 1);
        threads.push_back(thread(concurrent_worker, min, range)); // 对于每一个线程都通过worker函数来完成任务，并划分一部分数据给它处理
        min = range + 1;
    }
    for (auto& t: threads) {
        t.join(); // 等待每一个线程执行结束
    }

    auto end_time = chrono::steady_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << "Concurrent task finish, " << ms << " ms consumed, Result: " << sum << endl;
}

/*
 * 通用锁定算法
主要API
API	        C++标准	说明
lock	    C++11	锁定指定的互斥体，若任何一个不可用则阻塞
try_lock	C++11	试图通过重复调用 try_lock 获得互斥体的所有权
要避免死锁，需要仔细的思考和设计业务逻辑。

有一个比较简单的原则可以避免死锁，即：对所有的锁进行排序，每次一定要按照顺序来获取锁，不允许乱序。
 例如：要获取某个玩具，一定要先拿到锁A，再拿到锁B，才能玩玩具。这样就不会死锁了。
 */
class Account
{
public:
    Account(string name, double money) : mName(name), mMoney(money) {}

public:
    void changeMoney(double amount)
    {
        mMoney += amount;
    }
    string getName() {
        return mName;
    }
    double getMoney() {
        return mMoney;
    }
    mutex* getLock() {
        return &mMoneyLock;
    }

private:
    string mName;
    double mMoney;
    mutex mMoneyLock;
};

class Bank
{
public:
    void addAccount(Account *account) {
        mAccounts.insert(account);
    }

    bool transferMoney(Account* accountA, Account* accountB, double amount) {
        lock(*accountA->getLock(), *accountB->getLock()); // 通过lock函数来获取两把锁，标准库的实现会保证不会发生死锁。
        lock_guard guardA(*accountA->getLock(), adopt_lock); // 在自身对象生命周期的范围内锁定互斥体
        lock_guard guardB(*accountB->getLock(), adopt_lock); // 创建lock_guard的目的是为了在transferMoney结束的时候释放锁，
        // lockB也是一样。但需要注意的是，这里传递了 adopt_lock表示：现在是已经获取到互斥体了的状态了，
        // 不用再次加锁（如果不加adopt_lock就是二次锁定了）

        if (amount > accountA->getMoney()) { // 判断转出账户金额是否足够，如果不够此次转账失败。
            return false;
        }

        accountA->changeMoney(-amount); // 进行转账
        accountB->changeMoney(amount);
        return true;
    }

    bool transferMoneyMaybeDeadLock(Account* accountA, Account* accountB, double amount) {
        lock_guard guardA(*accountA->getLock()); // 为了保证线程安全，在修改每个账号之前，需要获取相应的锁
        lock_guard guardB(*accountB->getLock());

        if (amount > accountA->getMoney()) { // 判断转出账户金额是否足够，如果不够此次转账失败。
            return false;
        }

        accountA->changeMoney(-amount); // 进行转账
        accountB->changeMoney(amount);
        return true;
    }

    double totalMoney() const {
        double sum = 0;
        for (auto a : mAccounts) {
            sum += a->getMoney();
        }
        return sum;
    }
private:
    set<Account*> mAccounts;
};

mutex sCoutLock;
void randomTransfer(Bank* bank, Account* accountA, Account* accountB)
{
    while (true) {
        double randomMoney = ((double)rand() / RAND_MAX) * 100;
        if (bank->transferMoney(accountA, accountB, randomMoney)) {
            sCoutLock.lock();
            cout << "Transfer " << randomMoney << " from " << accountA->getName()
                 << " to " << accountB->getName()
                 << ", Bank totalMoney: " << bank->totalMoney() << endl;
            sCoutLock.unlock();
        } else {
            sCoutLock.lock();
            cout << "Transfer failed, "
                 << accountA->getName() << " has only $" << accountA->getMoney() << ", but "
                 << randomMoney << " required" << endl;
            sCoutLock.unlock();
        }
    }
}

void randomTransferMaybeDeadLock(Bank* bank, Account* accountA, Account* accountB)
{
    while (true) {
        double randomMoney = ((double)rand() / RAND_MAX) * 100;
        if (bank->transferMoney(accountA, accountB, randomMoney)) {
            cout << "Transfer " << randomMoney << " from " << accountA->getName()
                 << " to " << accountB->getName()
                 << ", Bank totalMoney: " << bank->totalMoney() << endl;
        } else {
            cout << "Transfer failed, "
                 << accountA->getName() << " has only $" << accountA->getMoney() << ", but "
                 << randomMoney << " required" << endl;
        }
    }
}

/*
通用互斥管理
主要API
API	        C++标准	说明
lock_guard	C++11	实现严格基于作用域的互斥体所有权包装器
unique_lock	C++11	实现可移动的互斥体所有权包装器
shared_lock	C++14	实现可移动的共享互斥体所有权封装器
scoped_lock	C++17	用于多个互斥体的免死锁 RAII 封装器
锁定策略	    C++标准	说明
defer_lock	C++11	类型为 defer_lock_t，不获得互斥的所有权
try_to_lock	C++11	类型为try_to_lock_t，尝试获得互斥的所有权而不阻塞
adopt_lock	C++11	类型为adopt_lock_t，假设调用方已拥有互斥的所有权

互斥体（mutex相关类）提供了对于资源的保护功能，但是手动的锁定（调用lock或者try_lock）
 和解锁（调用unlock）互斥体是要耗费比较大的精力的，我们需要精心考虑和设计代码才行。因为
 我们需要保证，在任何情况下，解锁要和加锁配对，因为假设出现一条路径导致获取锁之后没有正常
 释放，就会影响整个系统。如果考虑方法还可以会抛出异常，这样的代码写起来会很费劲。

鉴于这个原因，标准库就提供了上面的这些API。它们都使用了叫做RAII的编程技巧，来简化我们手动加锁和解锁的“体力活”。
 */

int g_i = 0;
std::mutex g_i_mutex; // 全局的互斥体g_i_mutex用来保护全局变量g_i

void safe_increment()
{
    std::lock_guard<std::mutex> lock(g_i_mutex); // 这是一个设计为可以被多线程环境使用的方法。因此需要通过互斥体
    // 来进行保护。这里没有调用lock方法，而是直接使用lock_guard来锁定互斥体。
    ++g_i;

    std::cout << std::this_thread::get_id() << ": " << g_i << '\n';
    // 在方法结束的时候，局部变量std::lock_guard<std::mutex> lock会被销毁，它对互斥体的锁定也就解除了
}

// 主要API
//API	                C++标准	说明
//mutex	                C++11	提供基本互斥设施
//timed_mutex	        C++11	提供互斥设施，带有超时功能
//recursive_mutex	    C++11	提供能被同一线程递归锁定的互斥设施
//recursive_timed_mutex	C++11	提供能被同一线程递归锁定的互斥设施，带有超时功能
//shared_timed_mutex	C++14	提供共享互斥设施并带有超时功能
//shared_mutex	        C++17	提供共享互斥设施
// | 方法| 说明 |
//| lock|锁定互斥体，如果不可用，则阻塞 |
//| try_lock |尝试锁定互斥体，如果不可用，直接返回 |
//|unlock | 解锁互斥体|

/*
 超时：timed_mutex，recursive_timed_mutex，shared_timed_mutex的名称都带有timed，
 这意味着它们都支持超时功能。它们都提供了try_lock_for和try_lock_until方法，这两个方法
 分别可以指定超时的时间长度和时间点。如果在超时的时间范围内没有能获取到锁，则直接返回，不再继续等待。
可重入：recursive_mutex和recursive_timed_mutex的名称都带有recursive。可重入或者叫
 做可递归，是指在同一个线程中，同一把锁可以锁定多次。这就避免了一些不必要的死锁。
共享：shared_timed_mutex和shared_mutex提供了共享功能。对于这类互斥体，实际上是提供了两
 把锁：一把是共享锁，一把是互斥锁。一旦某个线程获取了互斥锁，任何其他线程都无法再获取互斥锁和
 共享锁；但是如果有某个线程获取到了共享锁，其他线程无法再获取到互斥锁，但是还有获取到共享锁。
 这里互斥锁的使用和其他的互斥体接口和功能一样。而共享锁可以同时被多个线程同时获取到（使用共享
 锁的接口见下面的表格）。共享锁通常用在读者写者模型上。
 使用共享锁的接口如下：
| 方法| 说明 |
|lock_shared | 获取互斥体的共享锁，如果无法获取则阻塞 |
| try_lock_shared| 尝试获取共享锁，如果不可用，直接返回 |
| unlock_shared| 解锁共享锁 |
 */

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

// 不过需要注意的是，参数是以拷贝的形式进行传递的。因此对于拷贝耗时的对象你
// 可能需要传递指针或者引用类型作为参数。但是，如果是传递指针或者引用，你还
// 需要考虑参数对象的生命周期。因为线程的运行长度很可能会超过参数的生命周期
// （见下文detach），这个时候如果线程还在访问一个已经被销毁的对象就会出现问题。

// join与detach
//主要API
//API	说明
//join	等待线程完成其执行
//detach	允许线程独立执行
//一旦启动线程之后，我们必须决定是要等待直到它结束（通过join），还是让它独立
// 运行（通过detach），我们必须二者选其一。如果在thread对象销毁的时候我们
// 还没有做决定，则thread对象在析构函数出将调用std::terminate()从而导致我们的进程异常退出。

//    thread th(ThreadWrite);
//    th.detach();
//    for (int i = 0; i < 3; ++i) {
//        this_thread::sleep_for(100ms);
//        thread th(ThreadRead, i + 1);
//        th.detach(); // detach是让目标线程成为守护线程（daemon threads）
//    }

    // 一个消费者线程，5个生产者线程，则生产者经常要等待消费者
//    Dispatch(1, 5);


    int a = 10;
    cout<<a<<","<<&a<<endl;
    thread t1(proc, ref(a));
    t1.join();

    int ans = 0;
//    std::thread th1(work1, std::ref(ans));
//    std::thread th2(work2, std::ref(ans));
    std::thread th1(work3, std::ref(ans));
    std::thread th2(work4, std::ref(ans));
    th1.join();
    std::cout<<ans<<std::endl;
    th2.join();
    std::cout<<ans<<std::endl;

    print_time();

    thread tr1(sleep_thread);
    thread tr2(loop_thread);

    tr1.join();
    tr2.detach();

    print_time();

    // 一次调用
    //主要API
    //API	C++标准	说明
    //call_once	C++11	即便在多线程环境下，也能保证只调用某个函数一次
    //once_flag	C++11	与call_once配合使用
    // 在一些情况下，我们有些任务需要执行一次，并且我们只希望它执行一次，例如
    // 资源的初始化任务。这个时候就可以用到上面的接口。这个接口会保证，即便
    // 在多线程的环境下，相应的函数也只会调用一次。
    once_flag flag;

    thread trd1(worker, &flag);
    thread trd2(worker, &flag);
    thread trd3(worker, &flag);

    trd1.join();
    trd2.join();
    trd3.join();

    serial_task(0, MAX);
    concurrent_task(0, MAX);

    {
//        Account accountA("Paul", 100);
//        Account accountB("Moira", 100);
//
//        Bank aBank;
//        aBank.addAccount(&accountA);
//        aBank.addAccount(&accountB);
//
//        thread thread1(randomTransfer, &aBank, &accountA, &accountB);
//        thread thread2(randomTransfer, &aBank, &accountB, &accountA);
//
//        thread1.join();
//        thread2.join();
    }

    {
        std::cout << "main: " << g_i << '\n';

        std::thread t1(safe_increment); // 在多个线程中使用这个方法
        std::thread t2(safe_increment);

        t1.join();
        t2.join();

        std::cout << "main: " << g_i << '\n';
    }


    getchar();

    return 0;
}
