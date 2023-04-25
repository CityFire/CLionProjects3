#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <future>
#include <sstream>
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <math.h>
#include <exception>
#include <numeric>
#include <algorithm>
#include <tuple>
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

/*
 * 管理当前线程
主要API
API	C++标准	说明
上面是一些在线程内部使用的API，它们用来对当前线程做一些控制。

yield 通常用在自己的主要任务已经完成的时候，此时希望让出处理器给其他任务使用。
get_id 返回当前线程的id，可以以此来标识不同的线程。
sleep_for 是让当前线程停止一段时间。
sleep_until 和sleep_for类似，但是是以具体的时间点为参数。这两个API都以chrono API（由于篇幅所限，这里不展开这方面内容）为基础。
 */
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

/*
 * 一次调用
主要API
API	C++标准	说明
在一些情况下，我们有些任务需要执行一次，并且我们只希望它执行一次，例如资源的初始化任务。
 这个时候就可以用到上面的接口。这个接口会保证，即便在多线程的环境下，相应的函数也只会调用一次。

下面就是一个示例：有三个线程都会使用init函数，但是只会有一个线程真正执行它。
 */
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
        unique_lock lock(mMoneyLock); // 这里使用的是unique_lock，这是为了与条件变量相配合。因为条件变量会解锁和重新锁定互斥体。
        mConditionVar.wait(lock, [this, amount] { // 这里是比较重要的一个地方：通过条件变量进行等待。
            // 此时：会通过后面的lambda表达式判断条件是否满足。如果满足则继续；如果不满足，则此处会解锁互斥
            // 体，并让当前线程等待。解锁这一点非常重要，因为只有这样，才能让其他线程获取互斥体。
            return mMoney + amount > 0; // 这里是条件变量等待的条件。
        });
        mMoney += amount;
        mConditionVar.notify_all(); // 此处也很重要。当金额发生变动之后，我们需要通知所有在条件变量上等待
        // 的其他线程。此时所有调用wait线程都会再次唤醒，然后尝试获取锁（当然，只有一个能获取到）并再次判断条件
        // 是否满足。除了notify_all还有notify_one，它只通知一个等待的线程。wait和notify就构成了线程间互相协作的工具。
    }
    // 请注意：wait和notify_all虽然是写在一个函数中的，但是在运行时它们是在多线程环境中执行的，因此对于这段代码，需要能够
    // 从不同线程的角度去思考代码的逻辑。这也是开发并发系统比较难的地方。
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
    condition_variable mConditionVar; // 声明了一个条件变量，用来在多个线程之间协作
};

class Bank
{
public:
    void addAccount(Account *account) {
        mAccounts.insert(account);
    }

    bool transferMoney_lock_guard(Account* accountA, Account* accountB, double amount) {
//        lock(*accountA->getLock(), *accountB->getLock()); // 通过lock函数来获取两把锁，标准库的实现会保证不会发生死锁。
//        lock_guard guardA(*accountA->getLock(), adopt_lock); // 在自身对象生命周期的范围内锁定互斥体
//        lock_guard guardB(*accountB->getLock(), adopt_lock); // 创建lock_guard的目的是为了在transferMoney结束的时候释放锁，
        // lockB也是一样。但需要注意的是，这里传递了 adopt_lock表示：现在是已经获取到互斥体了的状态了，
        // 不用再次加锁（如果不加adopt_lock就是二次锁定了）

        // 如果使用unique_lock这三行代码还有一种等价的写法：
        unique_lock lockA(*accountA->getLock(), defer_lock);
        unique_lock lockB(*accountB->getLock(), defer_lock);
        lock(lockA, lockB);
        // 请注意这里lock方法的调用位置。这里先定义unique_lock指定了defer_lock，因此实际没有锁定互斥体，而是到第三行才进行锁定。

        // 最后，借助scoped_lock，我们可以将三行代码合成一行，这种写法也是等价的。
//        scoped_lock lockAll(*accountA->getLock(), *accountB->getLock());
        // scoped_lock会在其生命周期范围内锁定互斥体，销毁的时候解锁。同时，它可以锁定多个互斥体，并且避免死锁。

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
    void transferMoney(Account* accountA, Account* accountB, double amount);
private:
    set<Account*> mAccounts;
};

void Bank::transferMoney(Account *accountA, Account *accountB, double amount) {
    accountA->changeMoney(-amount);
    accountB->changeMoney(amount);
}

mutex sCoutLock;
void randomTransfer(Bank* bank, Account* accountA, Account* accountB)
{
    while (true) {
        double randomMoney = ((double)rand() / RAND_MAX) * 100;
        {
            lock_guard guard(sCoutLock);
            cout << "Try to Transfer " << randomMoney
                 << " from " << accountA->getName() << "(" << accountA->getMoney()
                 << ") to " << accountB->getName() << "(" << accountB->getMoney()
                 << "), Bank totalMoney: " << bank->totalMoney() << endl;
        }
        bank->transferMoney(accountA, accountB, randomMoney);
    }
}

void randomTransfer_lock_guard(Bank* bank, Account* accountA, Account* accountB)
{
    while (true) {
        double randomMoney = ((double)rand() / RAND_MAX) * 100;
        if (bank->transferMoney_lock_guard(accountA, accountB, randomMoney)) {
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
        if (bank->transferMoney_lock_guard(accountA, accountB, randomMoney)) {
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

/*
 * RAII
上面的几个类（lock_guard，unique_lock，shared_lock，scoped_lock）都使用了一个叫做RAII的编程技巧。

RAII全称是Resource Acquisition Is Initialization，直译过来就是：资源获取即初始化。

RAII是一种C++编程技术，它将必须在使用前请求的资源（例如：分配的堆内存、执行线程、打开的套接字、
 打开的文件、锁定的互斥体、磁盘空间、数据库连接等——任何存在受限供给中的事物）的生命周期与一个对象的生存周期相绑定。

RAII保证资源可用于任何会访问该对象的函数。它亦保证所有资源在其控制对象的生存期结束时，以获取顺序的逆序释放。
 类似地，若资源获取失败（构造函数以异常退出），则为已构造完成的对象和基类子对象所获取的所有资源，会以初始化顺序
 的逆序释放。这有效地利用了语言特性以消除内存泄漏并保证异常安全。

RAII 可总结如下:

将每个资源封装入一个类，其中：

构造函数请求资源，并建立所有类不变式，或在它无法完成时抛出异常，
析构函数释放资源并决不抛出异常；
始终经由 RAII 类的实例使用满足要求的资源，该资源

自身拥有自动存储期或临时生存期，或
具有与自动或临时对象的生存期绑定的生存期

 */

/*
 * 条件变量
| API                       | C++标准 | 说明 |
| condition_variable        | C++ 11 | 提供与 std::unique_lock 关联的条件变量 |
| condition_variable_any    | C++ 11 | 提供与任何锁类型关联的条件变量 |
| notify_all_at_thread_exit | C++ 11 | 安排到在此线程完全结束时对 notify_all 的调用 |
| cv_status                 | C++ 11 | 列出条件变量上定时等待的可能结果 |

 条件变量提供了一个可以让多个线程间同步协作的功能。这对于生产者-消费者模型很有意义。在这个模型下：

    生产者和消费者共享一个工作区。这个区间的大小是有限的。
    生产者总是产生数据放入工作区中，当工作区满了。它就停下来等消费者消费一部分数据，然后继续工作。
    消费者总是从工作区中拿出数据使用。当工作区中的数据全部被消费空了之后，它也会停下来等待生产者往工作区中放入新的数据。

从上面可以看到，无论是生产者还是消费者，当它们工作的条件不满足时，它们并不是直接报错返回，而是停下来等待，直到条件满足。
 */

/*
future
API	            C++标准	说明
async	        C++11	异步运行一个函数，并返回保有其结果的std::future
future	        C++11	等待被异步设置的值
packaged_task	C++11	打包一个函数，存储其返回值以进行异步获取
promise	        C++11	存储一个值以进行异步获取
shared_future	C++11	等待被异步设置的值（可能为其他 future 所引用）

 这一小节中，我们来熟悉更多的可以在并发环境中使用的工具，它们都位于<future>头文件中。
 */
// 对于面向对象编程来说，很多时候肯定希望以对象的方法来指定异步任务。
class Worker {
public:
    Worker(int min, int max) : mMin(min), mMax(max) {} // 这里通过一个类来描述任务。这个类是对前面提到的任务的封装。它包含了任务的输入参数，和输出结果。
    double work() { // work函数是任务的主体逻辑。
        mResult = 0;
        for (int i = mMin; i <= mMax; ++i) {
            mResult += sqrt(i);
        }
        return mResult;
    }
    double getResult() {
        return mResult;
    }
private:
    int mMin;
    int mMax;
    double mResult;
};

/*
packaged_task
在一些业务中，我们可能会有很多的任务需要调度。这时我们常常会设计出任务队列和线程池的结构。此时，就可以使用packaged_task来包装任务。

如果你了解设计模式，你应该会知道命令模式。

packaged_task绑定到一个函数或者可调用对象上。当它被调用时，它就会调用其绑定的函数或者可调用对象。
 并且，可以通过与之相关联的future来获取任务的结果。调度程序只需要处理packaged_task，而非各个函数。

packaged_task对象是一个可调用对象，它可以被封装成一个std::fucntion，或者作为线程函数传递给std::thread，或者直接调用。
 */

double concurrent_future_worker(int min, int max) {
    double sum = 0;
    for (int i = min; i <= max; i++) {
        sum += sqrt(i);
    }
    return sum;
}

double concurrent_packaged_task(int min, int max) {
    vector<future<double>> results; // 创建一个集合来存储future对象。我们将用它来获取任务的结果。

    unsigned concurrent_count = thread::hardware_concurrency();
    min = 0;
    for (int i = 0; i < concurrent_count; i++) { // 同样的，根据CPU的情况来创建线程的数量。
        packaged_task<double(int, int)> task(concurrent_future_worker); // 将任务包装成packaged_task。
        // 请注意，由于concurrent_future_worker被包装成了任务，我们无法直接获取它的return值。而是要通过future对象来获取。
        results.push_back(task.get_future()); // 获取任务关联的future对象，并将其存入集合中。

        int range = max / concurrent_count * (i + 1);
        thread t(std::move(task), min, range); // 通过一个新的线程来执行任务，并传入需要的参数。
        t.detach();

        min = range + 1;
    }

    cout << "threads create finish" << endl;
    double sum = 0;
    for (auto& r : results) {
        sum += r.get(); // 通过future集合，逐个获取每个任务的计算结果，将其累加。
        // 这里r.get()获取到的就是每个任务中concurrent_worker的返回值。
    }
    return sum;
}

/*
promise与future
在上面的例子中，concurrent_task的结果是通过return返回的。但在一些时候，我们可能不能这么做：
 在得到任务结果之后，可能还有一些事情需要继续处理，例如清理工作。

这个时候，就可以将promise与future配对使用。这样就可以将返回结果和任务结束两个事情分开。
 */
// 下面是对上面代码示例的改写：
void concurrent_promise_task(int min, int max, promise<double>* result) {
    // concurrent_promise_task不再直接返回计算结果，而是增加了一个promise对象来存放结果。
    vector<future<double>> results;

    unsigned concurrent_count = thread::hardware_concurrency();
    min = 0;
    for (int i = 0; i < concurrent_count; i++) {
        packaged_task<double(int, int)> task(concurrent_future_worker);
        results.push_back(task.get_future());

        int range = max / concurrent_count * (i + 1);
        thread t(std::move(task), min, range);
        t.detach();

        try {
            min = range + 1;
            cout << "threads create finish" << endl;
            double sum = 0;
            for (auto& r : results) {
                sum += r.get();
            }
            result->set_value(sum); // 在任务计算完成之后，将总结过设置到promise对象上。
            // 一旦这里调用了set_value，其相关联的future对象就会就绪。
        } catch (...) {
            result->set_exception(current_exception());
        }
        cout << "concurrent_task finish" << endl;
    }
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

atomic<int> x;
void increment()
{
    cout<<"x before is "<<x<<endl;
    x++; // 不是x = x + 1
    cout<<"x after is "<<x<<endl;
}

/*
mutex mutex_x;
atomic<bool> init_x;  // 初始为false
int x_;

class F { // 传统函数对象
public:
    F(const vector<double>& vv, double* p) : v{vv}, res{p} { }
    void operator()();     // 将结果放入 *res
private:
    const vector<double>& v; // 输入源
    double* res;   // 输出目标
};

double f(const vector<double>& v); // 传统函数
void g(const vector<double>& v, double* res); // 将结果放入 *res
int comp(vector<double>& vec1, vector<double>& vec2, vector<double>& vec3)
{
    double res1;
    double res2;
    double res3;
    // ...
//    thread t1 {F{vec1, res1}};  // 函数对象
    thread t2 {[&](){res2 = f(vec2);}}; // lambda表达式
    thread t3 {g, ref(vec3), &res3}; // 普通函数

//    t1.join();
    t2.join();
    t3.join();

    cout<<res1<<' '<<res2<<' '<<res3<<'\n';
}
 */

//template <typename T>
//class stack {
//    std::atomic<node<T>*> head;
//public:
//    void push(const T& data)
//    {
//        node<T>* new_node = new node<T>(data);
//        new_node->next = head.load(std::memory_order_relaxed);
//        while(!head.compare_exchange_weak(new_node->next, new_node, std::memory_order_release, std::memory_order_relaxed));
//    }
//};

// 这些锁对象还提供了一种防止最常见形式的死锁的方法:
//void f()
//{
//    // ...
//    unique_lock<mutex> lck1 {m1, defer_lock};  // 还未得到 m1
//    unique_lock<mutex> lck2 {m2, defer_lock};
//    unique_lock<mutex> lck3 {m3, defer_lock};
//    // ...
//    lock(lck1, lck2, lck3);  // 获取所有三个互斥锁
//    // ...操作共享数据...
//} // 隐式释放所有互斥锁

//double comp4(vector<double>& v) // 如果v足够大则会产生多个任务
//{
//    if (v.size() < 10000) // 值得用并发机制吗？
//        return accum(v.begin(), v.end(), 0.0);
//    auto v0 = &v[0];
//    auto sz = v.size();
//
//    auto f0 = async(accum, v0, v0+sz/4, 0.0);
//    auto f1 = async(accum, v0+sz/4, v0+sz/2, 0.0);
//    auto f2 = async(accum, v0+sz/2, v0+sz*3/4, 0.0);
//    auto f3 = async(accum, v0+sz*3/4, v0+sz, 0.0);
//
//    return f0.get()+f1.get()+f2.get()+f3.get();
//}

//X f(Y);
template <typename X, typename Y>
void ff(Y y, promise<X>& p)
{
    try {
        X res = f(y);   // 异步执行 f(y)
        p.set_value(res); //... 给 res 计算结果 ...
    }
    catch (...) { // 哎呀：没能计算出 res
        p.set_exception(current_exception());
    }
}

template <typename X, typename Y>
[[noreturn]] void user(Y arg)
{
    auto pro = promise<X>{};
    auto fut = pro.get_future();
    thread t {ff, arg, ref(pro)}; // 在不同线程上运行 ff
    // ... 做一会别的事情 ...
    X x = fut.get();
    cout<<x.x<<'\n';
    t.join();
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
//using var_t = std::variant<int, long, double, std::string>; // variant类型

class Matrix {
    double* elements;   // 指向所有元素的指针
public:
    Matrix() {}
    Matrix (Matrix&& a) // 移动构造
    {
        elements = a.elements; // 复制句柄
        a.elements = nullptr;  // 现在 a 的析构函数不用做任何事情了
    }
};

class Vector
{

};

//auto SVD(const Matrix& A) -> tuple<Matrix, Vector, Matrix>
//{
//    Matrix U, V;
//    Vector S;
//    // ...
//    return make_tuple(U, S, V);
//    return {U, S, V};
//}
//
//void use()
//{
//    Matrix A, U, V;
//    Vector S;
//    // ...
//    tie(U, S, V) = SVD(A); // 使用元组形式
//    auto [U, S, V] = SVD(A); // 使用元组形式和结构化绑定
//}

//tuple SVD(const Matrix& A) // 从返回语句中推导出元组模板参数
//{
//    Matrix U, V;
//    Vector S;
//    // ...
//    return {U, S, V};
//}

template <class T, class A1>
std::shared_ptr<T> factory(A1&& a1)
{
    return std::shared_ptr<T>(new T(std::forward<A1>(a1)));// forward 告诉编译器将实参视为右值引用，
    // 因此 T 的移动 构造函数(而不是拷贝构造函数)会被调用，来窃取该参数。它本质上就是个右 值引用的类型转换。
}

template <class T>
class clone_ptr
{
private:
    T* ptr;
public:
    // ...
    clone_ptr(clone_ptr&& p)   // 移动构造函数
        : ptr(p.ptr)  // 拷贝数据的表示
    {
        p.ptr = 0;   //  把源数据的表示置空
    }
    clone_ptr& operator=(clone_ptr&& p) // 移动赋值
    {
        std::swap(ptr, p.ptr);
        return *this;   // 销毁目标的旧值
    }
};

template <typename T>
class vector_s {
public:
    vector_s(initializer_list<T>) {} // 初始化器列表构造函数
};

struct LengthInKM {
    constexpr explicit LengthInKM(double d) : val(d) { }
    constexpr double getValue() { return val; }
private:
    double val;
};

struct LengthInMile {
    constexpr explicit LengthInMile(double d) : val(d) { }
    constexpr double getValue() { return val; }
    constexpr operator LengthInKM() { return LengthInKM(1.609344 * val); }
private:
    double val;
};

//constexpr Imaginary operator""i(long double x) { return Imaginary(x); }

template <typename T>
typename remove_reference<T>::type&&
move(T&& param)
{
    using ReturnType = typename remove_reference<T>::type&&; // alias declaration
    return static_cast<ReturnType>(param);
}

template<typename T>
decltype(auto) move_(T&& param) // namespace std
{
    using ReturnType = remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}

template<typename T, typename A> class MyVector { };
//template<typename T> using Vec = MyVector<T, MyAlloc<T> >;

//typedef double (*analysis_fp)(const vector<Student_info>&);
//using analysis_fb = double(*)(const vector<Student_info>&);

struct empty_stack: std::exception
{
        const char* what() const throw();
};

template <typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m; // 互斥量m可保证线程安全，就是对每个成员函数进行加锁保护。
    // 保证在同一时间内，只 有一个线程可以访问到数据，所以能够保证修改数据结构的“不变量”时，不会被其他线程看 到。
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value)); // 调用可能会抛出一个异常，不是拷贝/移动数据值，就是内存不足。
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack(); //
        std::shared_ptr<T> const res(
                std::make_shared<T>(std::move(data.top()))); // 也可能会抛出一个异常，有两方面的原因:
        // 对 std::make_shared 的调用，可能无法分配出足够的内存去创建新的对象，并且内部数据需 要对新对象进行引用;
        // 或者在拷贝或移动构造到新分配的内存中返回时抛出异常。两种情况 下，C++运行库和标准库能确保不会出现内存泄露，
        // 并且新创建的对象(如果有的话)都能被正 确销毁。因为没有对栈进行任何修改，所以也不会有问题。
        data.pop(); //
        return res;
    }

    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack();
        value = std::move(data.top()); //
        data.pop(); //
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

template<class T>
class thread_safe_queue
{
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_cond;
public:
    thread_safe_queue() {}
    thread_safe_queue(thread_safe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    void push(T new_value)
    {
        std::shared_ptr<T> data(
                std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one(); //
    }

    void wait_and_pop(T& value) //
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {
            return !data_queue.empty();
        });
        value = std::move(*data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() //
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {
            return !data_queue.empty();
        }); //

//        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>(); //
//        std::shared_ptr<T> res(
//                std::make_shared<T>(std::move(data_queue.front())));
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

// 单线程版本
template<typename T>
class queue_
{
private:
    struct node
    {
        std::shared_ptr<T> data; //
        std::unique_ptr<node> next;

//        node(T data_);
//        data(std::move(data_)) {}
    };

    std::unique_ptr<node> head;
    node* tail;
public:
    queue_() : head(new node), tail(head.get()) {}//
    queue_(const queue_& other) = delete;
    queue_& operator=(const queue_& other) = delete;

    std::shared_ptr<T> try_pop()
    {
        if (head.get() == tail)   //
        {
            return std::shared_ptr<T>();
        }
//        std::shared_ptr<T> const res(
//                std::make_shared<T>(std::move(head->data)));
        std::shared_ptr<T> const res(head->data); //
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);//
        return res;//
    }

    void push(T new_value)
    {
//        std::unique_ptr<node> p(new node(std::move(new_value)));
        std::shared_ptr<T> new_data(
                std::make_shared<T>(std::move(new_value))); //
        std::unique_ptr<node> p(new node); //
        tail->data = new_data;  //
        node* const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
    }
};

template<typename T>
class threadsafe_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return nullptr;
        }
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }
public:
    threadsafe_queue() : head(new node), tail(head.get()) {}
    threadsafe_queue(const threadsafe_queue& other) = delete;
    threadsafe_queue& operator=(const threadsafe_queue& other) = delete;

    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        node* const new_tail = p.get();
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
    }
};

/*
class thread_pool
{
        std::atomic_bool  done;
        thread_safe_queue<std::function<void()> > work_queue;  // 1
        std::vector<std::thread> threads; // 2
        join_threads joiner; // 3

        void worker_thread()
        {
            while(!done) // 4
            {
                std::function<void()> task;
                if (work_queue.try_pop(task)) // 5
                {
                    task(); // 6
                }
                else
                {
                    std::this_thread::yield(); // 7
                }
            }
        }

public:
    thread_pool() : done(false), joiner(threads)
    {
            unsigned const thread_count = std::thread::hardware_concurrency(); // 8
            try
            {
                for (unsigned i = 0; i < thread_count; ++i) {
                    threads.push_back(&thread_pool::worker_thread, this); // 9
                }
            }
            catch (...)
            {
                done = true; // 10
                throw;
            }
    }

    ~thread_pool()
    {
            done = true; // 11
    }

    template<typename FunctionType>
    void submit(FunctionType f)
    {
            work_queue.push(std::function<void()>(f)); // 12
    }
};
 */

class Widget : public std::enable_shared_from_this<Widget>
{
public:
    // 完美转发参数的工厂方法
    template<typename... Ts>
    static std::shared_ptr<Widget> create(Ts&&... params);
    void process();
    std::vector<std::shared_ptr<Widget>> processedWidgets;
//    Widget(Widget&& rhs) : s(std::move(rhs.s)) {
//        ++moveCtorCalls;
//    }
//    Widget(Widget&& rhs) : s(std::forward<std::string>(rhs.s)) {
//        ++moveCtorCalls;
//    }
private:
    static  std::size_t moveCtorCalls;
    std::string s;
};

void Widget::process() {
    // 把指向当前对象的shared_ptr加入processedWidgets
    processedWidgets.emplace_back(shared_from_this());
}

class WidgetID
{

};

//std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
//{
//    static std::unordered_map<WidgetID, std::weak_ptr<const Widget>> cache;
//    auto objPtr = cache[id].lock();  // objPtr is std::shared_ptr
//                                     // to cached object
//                                     // (or null if object's not in cache)
//    if (!objPtr) {                // if not in cache
//        objPtr = loadWidget(id);  // load it
//        cache[id] = objPtr;       // cache it
//    }
//    return objPtr;
//}

template<typename T, typename... Ts>
std::unique_ptr<T> _make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

template<typename T, typename... Args>
void printf(const char* s, const T& value, const Args&... args)
{
    while (*s) {
        if (*s == '%' && *++s != '%') {
            std::cout<<value;
            printf(++s, args...);
        }
        std::cout<<*s++;
    }
    throw std::runtime_error("extra arguments provieded to printf");
}

template<typename T>
constexpr T pi = T(3.1415926535897932385);

template<typename T>
T circular_area(T r)
{
    return pi<T> * r * r;
}

//template<typename T> constexpr T pi_v = unspecified;
//constexpr double pi = pi_v<double>;

//template<typename T>
//auto size(const T& a) { return a.size(); }

auto get_size = [](auto& m) { return m.size(); };

constexpr int min(std::initializer_list<int> xs)
{
    int low = std::numeric_limits<int>::max();
    for (int x : xs)
        if (x < low)
            low = x;
    return low;
}

constexpr int m_ = min({1, 3, 2, 4});

//template<typename T>
//    where LessThanComparable<T>
//const T& min(const T& x, const T& y)
//{
//    return x < y ? x : y;
//}

//template<GreaterThanComparable>
//    const T& max(const T& x, const T& y)
//{
//    return x > y ? x : y;
//}

//generator<int> fibonacci()
//{
//    int a = 0;
//    int b = 1;
//
//    while (true) {
//        int next = a + b;
//        co_yield a;
//        a = b;
//        b = next;
//    }
//}

void do_something()
{
    cout<<"do_something"<<endl;
}

void do_something(int i)
{
    cout<<"do_something"<<i<<endl;
}

void do_something_else()
{
    cout<<"do_something_else"<<endl;
}

class background_task
{
public:
    void operator()() const
    {
        do_something();
        do_something_else();
    }
};

//background_task f;
//std::thread my_thread(f);
std::thread my_thread((background_task()));
std::thread my_thread1{background_task()};
std::thread my_thread2([] {
    do_something();
    do_something_else();
});


struct func
{
    int& i;
    func(int& i_) : i(i_) {}
    void operator()()
    {
        for (unsigned j=0 ; j<1000000 ; ++j)
        {
            do_something(i);  // 1 潜在访问隐患:悬空引用
        }
    }
};

void oops()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
//    my_thread.join(); // 调用join()的行为，还清理了线程相关的存储部分,这样 std::thread 对象将不再与已经
//    完成的线程有任何关联。这意味着，只能对一个线程使用一次join();一旦已经使用过
//join()， std::thread 对象就不能再次加入了，当对其使用joinable()时，将返回false。
    my_thread.detach(); // 2 不等待线程结束
}                       // 3 新线程可能还在运行  持续执行func::operator();
          // 可能会在do_something中调用 some_local_state的引用 --> 导致未定义行为

void do_something_in_current_thread()
{

}

void f_()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try
    {
        do_something_in_current_thread();
    }
    catch (...)
    {
        t.join(); //
        throw;
    }
    t.join(); //
}

class thread_guard
{
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_) : t(t_) {}
    ~thread_guard()
    {
        if (t.joinable()) // join()只能对给定的对象调用一次，所以对给已加入的线程再次进 行加入操作时，将会导致错误。
        {
            t.join();     //
        }
    }
    thread_guard(thread_guard const&)=delete;  // 为了不让编译器自动生成它们。直接 对一个对象进行拷贝或赋值是危险的，
    // 因为这可能会弄丢已经加入的线程。通过删除声明， 任何尝试给thread_guard对象赋值的操作都会引发一个编译错误。
    thread_guard& operator=(thread_guard const&)=delete;
};

void ff_()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
    do_something_in_current_thread();
} //

void edit_document(std::string const& filename)
{

}

void fff_(int i, std::string const& s)
{

}

void oops_(int some_param)
{
    char buffer[1024]; // buffer是一个指针变量，指向本地变量，然后本地变量通过buffer传递到新线 程中
    sprintf(buffer, "%i", some_param);
    std::thread t(fff_, 3, string(buffer)); // // 使用std::string，避免悬垂指针
    t.detach();
}

struct widget_id
{

};

struct widget_data
{

};

void display_status()
{

}

void process_widget_data(widget_data data)
{

}

void update_data_for_widget(widget_id w, widget_data& data)
{
    //
}

void oops_again(widget_id w)
{
    widget_data data;
//    std::thread t(update_data_for_widget, w, data); // x 错误
    std::thread t(update_data_for_widget, w, std::ref(data)); // 接收到一个data变量的引用，而非一个data变量拷贝 的引用
    display_status();
    t.join();
    process_widget_data(data);
//    std::bind()
}

class X
{
public:
    void do_lengthy_work(int num) {}
};

X my_x;
int num(0);
std::thread t(&X::do_lengthy_work, &my_x, num); // 可以传递一个成员函数指针作为线程函数， 并提供一个合适的对象指针作为第一个参数
// 提供的参数可以移动，但不能拷贝。"移动"是指:原始对象中的数据转移给另一对 象，而转移的这些数据就不再在原始对象中保存了

void some_function()
{
    cout<<"some_function...."<<endl;
}

void some_other_function()
{
    cout<<"some_other_function...."<<endl;
}

/*
std::thread func_f()
{
    return std::thread(some_function);
}

std::thread g_()
{
    void some_other_function(int);
    std::thread t(some_other_function, 42);
    return t;
}

void _f_(std::thread t) {}
void _g_()
{
    void some_function();
    _f_(std::thread(some_function));
    std::thread t(some_function);
    _f_(std::move(t));
}
 */

class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_) : t(std::move(t_))
    {
        if (!t.joinable())
            throw std::logic_error("No thread");
    }
    ~scoped_thread()
    {
        t.join();
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};

//void _ff_() {
//    int some_local_state;
//    scoped_thread t(std::thread(func(some_local_state)));
//    do_something_in_current_thread();
//}

class joining_thread
{
    std::thread t;
public:
    joining_thread() noexcept = default;
    template<typename Callable, typename ... Args>
    explicit joining_thread(Callable&& func, Args&& ... args) :
      t(std::forward<Callable>(func), std::forward<Args>(args)...) {}
    explicit joining_thread(std::thread t_) noexcept
      : t(std::move(t_)) {}
    joining_thread(joining_thread&& other) noexcept
      : t(std::move(other.t)) {}
    joining_thread& operator=(joining_thread&& other) noexcept
    {
        if (joinable()) {
            join();
        }
        t = std::move(other.t);
        return *this;
    }
    joining_thread& operator=(std::thread other) noexcept
    {
        if (joinable())
            join();
        t = std::move(other);
        return *this;
    }
    ~joining_thread() noexcept
    {
        if (joinable())
            join();
    }
    void swap(joining_thread& other) noexcept
    {
        t.swap(other.t);
    }
    std::thread::id get_id() const noexcept {
        return t.get_id();
    }
    bool joinable() const noexcept
    {
        return t.joinable();
    }
    void join()
    {
        t.join();
    }
    void detach()
    {
        t.detach();
    }
    std::thread& as_thread() noexcept
    {
        return t;
    }
    const std::thread& as_thread() const noexcept
    {
        return t;
    }
};

void do_work(unsigned id)
{

}

void __f__()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i)
    {
        threads.push_back(std::thread(do_work, i)); // 产生线程
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join)); // 对每个线程调用join()
    // 将 std::thread 放入 std::vector 是向线程自动化管理迈出的第一步:并非为这些线程创建独 立的变量，并且直接加入，而是
    // 把它们当做一个组。创建一组线程(数量在运行时确定)，可使 得这一步迈的更大
}

template<typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
    if (!length) //
        return init;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads =
            (length + min_per_thread-1) / min_per_thread; // 2
    unsigned long const hardware_threads =
            std::thread::hardware_concurrency();
    unsigned long const num_threads =  // 3
            std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads; // 每个线程中处理的元素数量,是范围中元素的总量除以线程的个数得出的
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);  // 需要注意的是，启动的线程数必须比num_threads 少1个，因为在启动之前已经有了一个线程(主线程)。
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);  // 使用简单的循环来启动线程:block_end迭代器指向当前块的末尾
        threads[i] = std::thread(     // 启动一个新线程为当 前块累加结果
                accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));// 需要注意的:因为不能直接从一个线程中
                // 返回一个值，所以 需要传递results容器的引用到线程中去。另一个办法，通过地址来获取线程执行的结果;第4 章中，我们将使用期望(futures)完成这种方案。
        block_start = block_end;  // 当迭代器指向当前块的末尾时，启动下一个块
    }
    accumulate_block<Iterator,T>() (
            block_start, last, results[num_threads - 1]); // 启动所有线程后，9中的线程会处理最终块的结果。对于分配不均，因为知道最终块是哪一 个，那么这个块中有多少个元素就无所谓了。
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join)); //

    return std::accumulate(results.begin(), results.end(), init); // 将所有结果进行累加
}

// 如果 std::thread 对象没有与任何执 行线程相关联， get_id() 将返回 std::thread::type 默认构造值，这个值表示“无线程”。
std::thread::id master_thread;
void some_core_part_of_algorithm()
{
    if (std::this_thread::get_id() == master_thread)
    {
//        do_master_thread_work();
    }
//    do_common_work();
}

std::list<int> some_list;  //
std::mutex some_mutex;     //
void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex); //
//    std::lock_guard guard(some_mutex); // C++17中添加了一个新特性，称为模板类参数推导，具体的模板参数类型推导则交给C++17的编译器完成
//    std::scoped_lock guard(some_mutex);// C++17
    some_list.push_back(new_value);
}
// 使用 std::lock_guard<std::mutex> ，使得这两个函数中对数据的访问 是互斥的:list_contains()
// 不可能看到正在被add_to_list()修改的列表
bool list_contains(int value_to_find)
{
    std::lock_guard<std::mutex> guard(some_mutex); //
//    std::lock_guard guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

int main(void)
{
//    user(Y{99});
//    auto x = make_unique<int>(arg); // x是std::unique_ptr<int>

    // C++ 20
//    std::jthread

    std::thread tt1(some_function); //
    std::thread tt2 = std::move(tt1); // t1的所有权就转移给 了t2 t1和执行线程已经没有关联了，执行some_function的函数线程与t2关联
    tt1 = std::thread(some_other_function); // 一个临时 std::thread 对象相关的线程启动了 所有者是一个临时对象——移动操作将会隐式的调用
    std::thread tt3; // t3使用默认构造方式创建4，与任何执行线程都没有关联
    tt3 = std::move(tt2);// 调用 std::move() 将与t2关联线程 的所有权转移到t3中5。因为t2是一个命名对象，需要显式的调用 std::move() 。
    //tt1 = std::move(tt3);// 将some_function线程的所有权转移6给t1。不过，t1已经有了一个关联 的线程(执行some_other_function的
    // 线程)，所以这里系统直接调用 std::terminate() 终止程 序继续运行。这样做(不抛出异常， std::terminate() 是noexcept函数)是
    // 为了保证与 std::thread 的析构函数的行为一致。进行赋值时也需要满足这些条件(说明:不能通过赋一个新值
    // 给 std::thread 对象的方式来"丢弃"一个线程

//    _LIBCPP_INLINE_VISIBILITY
//    thread& operator=(thread&& __t) _NOEXCEPT {
//        if (!__libcpp_thread_isnull(&__t_))
//            terminate();
//        __t_ = __t.__t_;
//        __t.__t_ = _LIBCPP_NULL_THREAD;
//        return *this;
//    }

    auto customDeleter1 = [](Widget *pw) { };
    auto customDeleter2 = [](Widget *pw) { };
    std::shared_ptr<Widget> pw1(new Widget, customDeleter1);
    std::shared_ptr<Widget> pw2(new Widget, customDeleter2);
    std::shared_ptr<Widget> pw3(pw1);
    std::vector<std::shared_ptr<Widget>> vpw{ pw1, pw2 };
    // `std::shared_ptr`不能处理的另一个东西是数组。和`std::unique_ptr`不同的是，
    // `std::shared_ptr`的API设计之初就是针对单个对象的，没有办法`std::shared_ptr<T[]>`。

    vector_s<int> v3 {1,2,3,4,5};

    vector<int> v = {7, 8};
    for(int a : v) {
        cout<<a<<endl;
    }

//    const char* msg = "The value of %s is about %g (unless you live in %s).\n";
//    printf(msg, std::string("pi"), 3.14159, "Indiana");

    constexpr LengthInKM marks[] = { LengthInMile(2.3), LengthInMile(0.76) };

    auto spw = std::make_shared<Widget>();
    cout<<spw.use_count()<<endl;
    std::weak_ptr<Widget> wpw(spw); // wpw points to same Widget as spw. RC remains 1
    cout<<spw.use_count()<<endl;
    cout<<wpw.use_count()<<endl;
    spw = nullptr;// RC goes to 0, and the
                  // Widget is destroyed.
                  // wpw now dangles

    if (wpw.expired()) {
        //  用expired来表示已经dangle。
        cout<<"wpw now dangles"<<endl;
    }

    std::shared_ptr<Widget> spw1 = wpw.lock(); // if wpw's expired, spw1 is null
    auto spw2 = wpw.lock(); // same as above, but uses auto
    if (spw1 != nullptr) {
        std::shared_ptr<Widget> spw3(wpw);// if wpw's expired, throw std::bad_weak_ptr
    }

    auto upw1(std::make_unique<Widget>()); // with make func
    std::unique_ptr<Widget> upw2(new Widget); // without make func
    auto spw11(std::make_shared<Widget>()); // with make func
    std::shared_ptr<Widget> spw22(new Widget); // without make func



    optional<int> var1 = 7;
//    std::variant<int,string> var2 = 7;
//    any var3 = 7;

    auto x1 = *var1; // 对 optional 解引用
//    auto x2 = get<int>(var2); // // 像访问 tuple 一样访问 variant
//    auto x3 = any_cast<int>(var3); // 转换 any


    thread td1(increment);
    thread td2(increment);
    td1.join();
    td2.join();

    cout<<"now x is "<<x<<endl;

//    if (!init_x) {
//        lock_guard<mutex> lck(mutex_x);
//        if (!init_x) x_ = 42;
//        init_x = true;
//    }  // 在此隐式释放 mutex_x（RAII)

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

/*
    {
        Account accountA("Paul", 100);
        Account accountB("Moira", 100);

        Bank aBank;
        aBank.addAccount(&accountA);
        aBank.addAccount(&accountB);

        thread thread1(randomTransfer, &aBank, &accountA, &accountB);
        thread thread2(randomTransfer, &aBank, &accountB, &accountA);

        thread1.join();
        thread2.join();
    }
*/

    {
        std::cout << "main: " << g_i << '\n';

        std::thread t1(safe_increment); // 在多个线程中使用这个方法
        std::thread t2(safe_increment);

        t1.join();
        t2.join();

        std::cout << "main: " << g_i << '\n';
    }

    // async
    // 很多语言都提供了异步的机制。异步使得耗时的操作不影响当前主线程的执行流。

    // 在C++11中，async便是完成这样的功能的。
    sum = 0;
    auto f1 = async(serial_worker, 0, MAX);
    cout << "Async task triggered" << endl;
    f1.wait();
    cout << "Async task finish, result: " << sum << endl << endl; // Async task finish, result: 2.10819e+13
    // 这里以异步的方式启动了任务。它会返回一个future对象。future用来存储异步任务的执行结果，
    // 关于future我们在后面packaged_task的例子中再详细说明。在这个例子中我们仅仅用它来等待任务执行完成。此处是等待异步任务执行完成

    // 需要注意的是，默认情况下，async是启动一个新的线程，还是以同步的方式（不启动新的线程）运行任务，这一点标准是没有指定的，
    // 由具体的编译器决定。如果希望一定要以新的线程来异步执行任务，可以通过launch::async来明确说明。launch中有两个常量：
    //
    // async：运行新线程，以异步执行任务。
    // deferred：调用方线程上第一次请求其结果时才执行任务，即惰性求值。

    // 除了通过函数来指定异步任务，还可以lambda表达式的方式来指定。
    double result = 0;
    cout << "Async task with lambda triggered, thread: " << this_thread::get_id() << endl;
    auto f2 = async(launch::async, [&result]() {
        cout << "Lambda task in thread: " << this_thread::get_id() << endl;
        for (int i = 0; i <= MAX; i++) {
            result += sqrt(i);
        }
    });
    f2.wait();
    cout << "Async task with lambda finish, result: " << result << endl << endl;
    // 在上面这段代码中，我们使用一个lambda表达式来编写异步任务的逻辑，并通过launch::async明确指定要
    // 通过独立的线程来执行任务，同时我们打印出了线程的id。
    // Async task with lambda triggered, thread: 0x111abae00
    // Lambda task in thread: 0x70000117f000
    // Async task with lambda finish, result: 2.10819e+13

    auto fut2 = std::async(std::launch::async | std::launch::deferred, [&result] { // run either async or defered
        cout << "Lambda task in thread: " << this_thread::get_id() << endl;
        for (int i = 0; i <= MAX; i++) {
            result += sqrt(i);
        }
    });


    // 对于面向对象编程来说，很多时候肯定希望以对象的方法来指定异步任务。
    Worker w(0, MAX);
    cout << "Task in class triggered" << endl;
    auto f3 = async(&Worker::work, &w); // 通过async执行任务：这里指定了具体的任务函数以及相应的对象。请注意这里是&w，
    // 因此传递的是对象的指针。如果不写&将传入w对象的临时复制。
    f3.wait();
    cout << "Task in class finish, result: " << w.getResult() << endl << endl;


    auto start_time = chrono::steady_clock::now();

    double r = concurrent_packaged_task(0, MAX);

    auto end_time = chrono::steady_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << "Concurrent task finish, " << ms << " ms consumed, Result: " << r << endl;

    // 在实际上的工程中，调用关系通常更复杂，你可以借助于packaged_task将任务组装成队列，然后通过线程池的方式进行调度

    {
        auto start_time = chrono::steady_clock::now();

        promise<double> sum;
        concurrent_promise_task(0, MAX, &sum);

        auto end_time = chrono::steady_clock::now();
        auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        cout << "Concurrent task finish, " << ms << " ms consumed." << endl;
        cout << "Result: " << sum.get_future().get() <<endl; // 通过sum.get_future().get()来获取结果。
        // 第2点中已经说了：一旦调用了set_value，其相关联的future对象就会就绪。

        // 需要注意的是，future对象只有被一个线程获取值。并且在调用get()之后，就没有可以获取的值了。
        // 如果从多个线程调用get()会出现数据竞争，其结果是未定义的。

        //如果真的需要在多个线程中获取future的结果，可以使用shared_future。
    }

    {
        /*
         * 从C++17开始。<algorithm>和<numeric> 头文件的中的很多算法都添加了一个新的参数：sequenced_policy。

借助这个参数，开发者可以直接使用这些算法的并行版本，不用再自己创建并发系统和划分数据来调度这些算法。

sequenced_policy可能的取值有三种，它们的说明如下：

| 变量 | 类型 | C++版本 | 说明
| --- | --- | --- | --- |
| execution::seq | execution::sequenced_policy | C++17 | 要求并行算法的执行可以不并行化 |
| execution::par | execution::parallel_policy | C++17 | 指
         */
    }

    getchar();

    return 0;
}
