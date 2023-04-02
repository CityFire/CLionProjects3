//
// Created by wjc on 2023/4/1.
//
#include <iostream>
using namespace std;

int MyStrlen(char str[])
{

    return (int)(sizeof(str)-1);

}

template <typename T>
T fun(T t) {
    return t;
}

void GetMemory(char **p)
{

    *p = new char[100];

    strcpy(*p,"hello world");

}

// 将 str 中的小写字母转换成大写字母
void UpperCase(char str[])
{
    // 函数内的str实际只是一个指向字符串的指针，没有任何额外的与数组相关的信息，
    // 因此sizeof作用于上只将其当指针看，一个指针为4个字节，因此返回4。
    size_t size = sizeof(str) / sizeof(str[0]); // 8
    for (size_t i = 0; i < size; ++i)
    {
        if ('a' <= str[i] && str[i] <= 'z')
        {
            str[i] -= ('a' - 'A');
            cout<<str[i]<<endl;
        }
    }
}

void test()
{
    int a = 248, b = 4;

    int const c = 21;

    const int *d = &a;

    int *const e = &b;

//    int const *f const = 0x329f;

//    *c = 32;
    d = &b;
//    *d = 43;
//    e = 34;
//    e = &a;
//    f = 0x321f;




}

int main(void)
{
    char str1[] = "abc";

    char str2[] = "abc";

    const char str3[] = "abc";

    const char str4[] = "abc";

    const char *str5 = "abc";

    const char *str6 = "abc";

    char *str7 = "abc";

    char *str8 = "abc";

    cout << ( str1 == str2 ) << endl; // 0

    cout << ( str3 == str4 ) << endl; // 0

    cout << ( str5 == str6 ) << endl; // 1

    cout << ( str7 == str8 ) << endl; // 1

    // 因为这是一个深拷贝/浅拷贝的问题.当字符串是数组形式声明并初始化,
    // 编译器认为是新数组,分配新空间,但不是深拷贝,因为根本就不算拷贝.而如果
    // 是相同的字符串,用指针声明,那就是比较如果有一样的字符串,就直接把新指针
    // 指过去,这是正宗的浅拷贝.


    cout<<MyStrlen("hello world")<<endl;   // 7  当用函数传递的数组指针的时候就自动退化为指针了

    char str[] = "hello world";
    int len = sizeof(str) - 1; // 记得减1哦,最后有'\0'结尾
    cout<<len<<endl;   // 11

    int a[5] = {1, 2, 3, 4, 5};

    int *ptr =(int *)(&a + 1);  // &a+1不是首地址+1，系统会认为加一个a数组的偏移，
                                // 是偏移了一个数组的大小（本例是5个int）则ptr实际是&(a[5]),也就是a+5
                                // 原因如下：
    //
    //&a是数组指针，其类型为 int (*)[5];
    //而指针加1要根据指针类型加上一定的值，不同类型的指针+1之后增加的大小不同。
    //a是长度为5的int数组指针，所以要加 5*sizeof(int)
    //所以ptr实际是a[5]
    //但是prt与(&a+1)类型是不一样的(这点很重要)
    //所以prt-1只会减去sizeof(int*)

    //a,&a的地址是一样的，但意思不一样

    //a是数组首地址，也就是a[0]的地址，&a是对象（数组）首地址，

    //a+1是数组下一元素的地址，即a[1],&a+1是下一个对象的地址，即a[5].

    cout<<*(a+1)<<" "<<*(ptr-1)<<endl;  // 2 5
    // 首先,a是一个数组,所以编译器解释&a就是a的全部长度,
    // 就是说(&a+1)也就是说移动了一个数组,指向了并不存在
    // 的a[5],所以ptr-1才会指向a数组的最后一个元素a[4],
    // 而a+1和a[1]是一样的...所以答案是2 5,

    int *ptr2 = (int *)&a + 1;
    cout<<*(ptr2-1)<<endl;     // 1

//    char a2;
//
//    char *str22 = &a2;
//
//    strcpy(str22, "hello");
//
//    cout<<str22;

//    char* s="AAA";
//    "AAA"是字符串常量。s是指针，指向这个字符串常量，所以声明s的时候就有问题。
//    cosnt char* s="AAA";
//
//    cout<<s<<endl;
//
//    s[0]='B';
//
//    cout<<s<<endl;
// 输出AAA后就崩溃了..为什么?因为当你在第一句初始化的时候,编译器就认为
// 这是个字符串常量了...再做数组操作的时候肯定错了罗...最好的习惯是声明
// 一个指针,用new分配空间,然后用库函数操作,比如strcpy,strcat等等...

    int (*s[10])(int);

    s[0] = fun;
    s[1] = fun;
    s[2] = fun;

    cout<<s[0]<<s[1]<<s[2]<<endl;

    // 咦...这是什么?其实这是一个函数指针数组,指向了一组int fun(int)的函数,第一眼确实让人有点迷糊...但是请习惯这样...

    char *str11 = NULL;

    GetMemory(&str11);

    cout<<str11<<endl;  // hello world

    delete []str11;  // delete 只是释放的str指向的内存空间,它本身的值还是存在的.所以delete之后，有一个好的习惯就是将str=NULL.

    str11 = NULL;

    char a3[10];

    cout<<strlen(a3)<<endl;

    char (*str12)[20]; //str12是一个数组指针，即指向数组的指针．

    char *str13[20]; //str13是一个指针数组，其元素为指针型数据．

    // char * const p; //常量指针，p的值不可以修改

    // char const * p；//指向常量的指针，指向的常量值不可以改

    // const char *p； //和char const *p

    char str14[] = "aBcDe";

    cout << "str字符长度为: " << sizeof(str14)/sizeof(str14[0]) << endl;

    UpperCase(str14);

    cout<<str14<<endl;

//    char szstr[10];

//    strcpy(szstr,"0123456789");

//    cout<<szstr<<endl;

//    typedef void(*)() voidFuncPtr;
//
//    *((voidFuncPtr)0x100000)();

//    *((void (*)( ))0x100000 ) ( );

    char aa[10];

    printf("%d", strlen(aa));
    // sizeof()和初不初始化，没有关系；
    // strlen()和初始化有关。

    return 0;
}