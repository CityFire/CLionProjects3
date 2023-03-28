//
// Created by wjc on 2023/3/26.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <numeric>
using namespace std;

void print_element(int n)
{
    cout<<n<<' ';
}

void add_3(int& n)
{
    n += 3;
}

bool greater_than_3(int n)
{
    return n > 3;
}

int fun(int a)
{
    return 2 * a;
}

int fun2(int a, int b)
{
    return a + b;
}

bool fun3(int a)
{
    return a < 10;
}

bool my_greater(int a, int b)
{
    return a > b;
}

int mult(int a, int b)
{
    return a * b;
}

const int MAX = 8;
vector<int> board(MAX);

void show_result()
{
    for (size_t i = 0; i < board.size(); i++) {
        cout<<"("<<i<<","<<board[i]<<")";
    }
    cout<<endl<<endl;
};

int check_cross()
{
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = i+1; j < board.size(); j++)
        {
            if ((j - i) == (size_t) abs(board[i] - board[j]))
                return 1;
        }
    }
    return 0;
}

void put_chess()
{
    while (next_permutation(board.begin(), board.end()))
    {
        if (!check_cross())
        {
            show_result();
        }
    }
}

int main8queen(void)
{
    for (size_t i = 0; i < board.size(); i++)
        board[i] = i;
    put_chess();
    return 0;
}

int permutationmain(void)
{
    int a[] = { 1, 2, 3, 4 };
    vector<int> v(a, a+4);
    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    // 全排列
    while (next_permutation(v.begin(), v.end()))
    {
        for_each(v.begin(), v.end(), print_element);
        cout<<endl;
    }

    return 0;
}

int mainalgorithm(void)
{
    int a[] = {1, 2, 3, 4, 5, 6 };
    vector<int> v(a, a + 6);

    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    rotate(v.begin(), v.begin()+2, v.end());  // 变序性算法
    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    sort(v.begin(), v.end());       // 排序算法
    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    sort(v.begin(), v.end(), my_greater);
    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    vector<int>::iterator it;
    it = lower_bound(v.begin(), v.end(), 10);  // 已序区间算法
    if (it != v.end())
    {
        cout<<it - v.begin()<<endl;
    }

    it = upper_bound(v.begin(), v.end(), 10);
    if (it != v.end())
    {
        cout<<it - v.begin()<<endl;
    }

    // 累加
    cout<<accumulate(v.begin(), v.end(), 0)<<endl;  // 数值算法

    // 累乘
    cout<<accumulate(v.begin(), v.end(), 1, mult)<<endl;

    return 0;
}

int removemain(void)
{
    int a[] = {1, 2, 3, 4, 5, 6 };
    vector<int> v(a, a + 6);

    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    remove(v.begin(), v.end(), 3);  // 移除性算法
    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    return 0;
}

char *GetMemory(void)
{
    char p[] = "hello world";
    return p;
}

char GetMemory2(char *p)
{
    p = (char *) malloc(100);
}

void GetMemory3(char **p, int num)
{
    *p = (char *) malloc(num);
}

char *strcpy2(char *strDest, const char *strSrc)
{
    assert((strDest != NULL) && (strSrc != NULL));
    char *address = strDest;
    while ((*strDest++ = *strSrc++) != '\0');
    return address;
}

// 变动性算法
int mainchangealgorithm(void)
{
    int a[] = { 1, 2, 3, 4, 5 };
    vector<int> v(a, a+5);
    list<int> l(5);

    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    for_each(v.begin(), v.end(), add_3);

    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    for_each(l.begin(), l.end(), print_element);
    cout<<endl;

    copy(v.begin(), v.end(), l.begin());
    for_each(l.begin(), l.end(), print_element);
    cout<<endl;

    copy_backward(v.begin(), v.end(), l.end());
    for_each(l.begin(), l.end(), print_element);
    cout<<endl;

    char *str = NULL;
    str = GetMemory();
    printf(str);
    cout<<endl;

//    char *str2 = NULL;
//    GetMemory2(str2);
//    strcpy(str2, "hello world");
//    printf(str2);
//    cout<<endl;

    char *str3 = NULL;
    GetMemory3(&str3, 100);
    strcpy(str3, "hello");
    printf(str3);
    cout<<endl;

    char *str4 = (char *) malloc(100);
    strcpy(str4, "hello");
    free(str4);
    if (str4 != NULL)
    {
        strcpy(str4, "hello world");
        printf(str4);
    }
    cout<<endl;

    char *destString = (char *)malloc(100);
    char *destString2 = strcpy2(destString, "world");
    cout<<destString2<<endl;

    int a2[] = {1, 2, 3, 4, 5};
    vector<int> v2(a2, a2 + 5);
    list<int> l2(5);
    list<int> l3(2);

    transform(v2.begin(), v2.end(), l2.begin(), fun);
    for_each(v2.begin(), v2.end(), print_element);
    cout<<endl;
    for_each(l2.begin(), l2.end(), print_element);
    cout<<endl;

    transform(v2.begin(), v2.begin()+2, v2.begin()+3, l3.begin(), fun2);
    for_each(l3.begin(), l3.end(), print_element);
    cout<<endl;

    replace(v2.begin(), v2.end(), 5, 13);
    for_each(v2.begin(), v2.end(), print_element);
    cout<<endl;

    replace_copy(v2.begin(), v2.end(), l2.begin(), 13, 3);
    for_each(v2.begin(), v2.end(), print_element);
    cout<<endl;
    for_each(l2.begin(), l2.end(), print_element);
    cout<<endl;

    replace_copy_if(v2.begin(), v2.end(), l2.begin(), fun3, 0);
    for_each(l2.begin(), l2.end(), print_element);
    cout<<endl;

    return 0;
}

// 非变动性算法
int mainAlgorithm(void)
{
    int a[] = { 1, 2, 3, 4, 5 };
    vector<int> v(a, a+5);

    vector<int>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        cout<<*it<<' ';
    }
    cout<<endl;
// 非变动性算法
    for_each(v.begin(), v.end(), print_element);
    cout<<endl;

    for (it = v.begin(); it != v.end(); ++it)
    {
        cout<<*it<<' ';
    }

    cout<<endl;

    it = min_element(v.begin(), v.end());
    if (it != v.end())
    {
        cout<<*it<<endl;
    }
    it = max_element(v.begin(), v.end());
    if (it != v.end())
    {
        cout<<*it<<endl;
    }
    it = find(v.begin(), v.end(), 3);
    if (it != v.end())
    {
        cout<<it-v.begin()<<endl;
    }
    else
    {
        cout<<"not found"<<endl;
    }

    it = find_if(v.begin(), v.end(), greater_than_3);
    if (it != v.end())
    {
        cout<<it-v.begin()<<endl;
    }
    else
    {
        cout<<"not found"<<endl;
    }

    return 0;
}