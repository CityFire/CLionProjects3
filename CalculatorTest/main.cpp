#include <iostream>
using namespace std;
#include "DebugNew.h"

// 自己实现malloc，在里面进行跟踪
// valgrind、dalloc、efence
// visual leak detector

// operator new  operator delete
// new operator <=> operator new + 构造函数

// 头文件包含次序
// C库、C++库、其他库.h、项目中.h

int main(void) {
    int*p = new int;
    delete p;

    int* p2 = new int[5];
    delete[] p2;

    return 0;
}
