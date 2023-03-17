#include <iostream>
#include <vector>

void testFunc1();

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
