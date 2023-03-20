#include <iostream>
#include <vector>
using namespace std;

void testFunc1();

class Singleton2
{
public:
    static Singleton2& GetInstance()
    {
        static Singleton2 instance; // 局部静态对象
        return instance;
    }
    ~Singleton2()
    {
        cout<<"~Singleton2..."<<endl;
    }
private:
    Singleton2(const Singleton2& other);
    Singleton2& operator=(const Singleton2& other);
    Singleton2()
    {
        cout<<"Singleton2..."<<endl;
    }
    static Singleton2* instance_;
};

// 非线程安全的单例
int main8866(void)
{
    //auto_ptr //智能指针

    Singleton2& s1 = Singleton2::GetInstance();
    Singleton2& s2 = Singleton2::GetInstance();

    return 0;
}

//int main()
//{
//    std::cout<<"Hello, World!"<<std::endl;
//
//    testFunc1();
//
//    return 0;
//}

void testFunc1() {
    std::vector<int> nums{2,3,4};
    for (int i = 0; i < nums.size(); ++i) {
        std::cout<<"num = "<<nums[i]<<std::endl;
    }

    for (const auto &num: nums) {
        std::cout<<"num = "<<num<<std::endl;
    }

    for (auto begin = nums.begin(); begin != nums.end(); ++begin) {
        std::cout<<"num = "<<*begin<<std::endl;
    }
}
