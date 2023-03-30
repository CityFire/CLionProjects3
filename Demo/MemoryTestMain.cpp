//
// Created by wjc on 2023/3/31.
//
/*
#include <cassert>
#include <vector>
#include <iostream>
using namespace std;

//INFO日志
#define log_info(...) do {\
printf("<%s %s> %s %s:%d INFO:",__DATE__, __TIME__,__FUNCTION__,__FILE__,__LINE__)&&\
printf(__VA_ARGS__)&&\
printf("\n");\
}while(0)

//ERROR日志
#define log_error(...) do {\
printf("<%s %s> %s %s:%d ERROR:",__DATE__, __TIME__,__FUNCTION__,__FILE__,__LINE__)&&\
printf(__VA_ARGS__)&&\
printf("\n");\
}while(0)

#define INVALID -1

//内存跟踪与日志开关
bool traceFlag = false;
bool memFlag = false;

#define TRACE_ON() traceFlag = true
#define TRACE_OFF() traceFlag = false

#define MEM_ON() memFlag = true
#define MEM_OFF() memFlag = false

//内存跟踪信息表
struct memInfo
{
  void* ptr;
  const char* file;
  long line;
};

//最大跟踪内存数
const int MAX_PTRS = 100;
//内存跟踪数组
memInfo memMap[MAX_PTRS] = {0};
//内存跟踪游标
int nPtrs = 0;

//(添加信息)内存跟踪信息表
void addPtr(void* p, const char* f, long l)
{
  memMap[nPtrs].ptr = p;
  memMap[nPtrs].file = f;
  memMap[nPtrs].line = l;
  ++nPtrs;
}

//(查找信息)内存跟踪信息表
int findPtr(void* p)
{
  for(size_t i = 0; i < nPtrs; ++i)
  {
    if(memMap[i].ptr == p)
      return i;
  }
  return INVALID;
}

//(删除信息)内存跟踪信息表
void delPtr(void* p)
{
  int pos = findPtr(p);
  assert(pos >= 0);

  for(size_t i = pos; i < nPtrs-1; ++i)
  {
    //当删除一个数据，后面的数据依此向前填充
    memMap[i] = memMap[i+1];
  }
  //移动数据后，最后一个位置置为0
  memMap[nPtrs-1].ptr = 0;
  memMap[nPtrs-1].file = 0;
  memMap[nPtrs-1].line = 0;

  --nPtrs;
}

//(输出信息)内存跟踪信息表
void memInfo_output()
{
  for(int i = 0; i < nPtrs; ++i)
  {
	log_info("memMap[%d].ptr= %p", i, memMap[i].ptr);
  }
}

//重载new
void* operator new(size_t size, const char* file, long line)
{
  //不管是否跟踪内存，都会malloc内存；相当于不使用重载new的情况
  void* p = malloc(size);
  if (p==NULL)
  {
    log_error("memory allocate failed");
    exit(1);
  }
  if(memFlag)
  {
    if(nPtrs == MAX_PTRS)
	{
	  log_error("memory map too small (increase MAX_PTRS)");
	  free(p);//内存跟踪失败，释放
      exit(1);
    }
	addPtr(p, file, line);
  }
  if(traceFlag)
  {
	log_info("Allocated %u bytes at address %p (Located file: %s, line: %ld)", size, p, file, line);
  }
  return p;
}

// 重载new数组
void* operator new[](size_t size, const char* file, long line)
{
  return operator new(size, file, line);
}

//宏定义new，在后续调用new时，使用new(__FILE__, __LINE__)
//void* operator new(size_t size, const char* file, long line) 中的第一个参数会被自动填充
#define new new(__FILE__, __LINE__)
//没有测试过带参数的delete重载
//#define delete delete(__FILE__, __LINE__)

//重载delete，可以调试下带参数的delete
//void operator delete(void* p, ***)
void operator delete(void* p)
{
  //不管memFlag是否打开，都会释放内存；就是说，不管跟踪内存与否，都会正常释放内存
  free(p);
  if(memFlag)
  {
    if(findPtr(p) >= 0)
    {
      assert(nPtrs > 0);
      delPtr(p);
    }
	//在内存跟踪信息表中找不到，要么为NULL指针，要么为unknow
	else if(!p)
	{
	  log_error("Delete NULL pointer: %p", p);
	  return;
	}
	else
	{
	  log_error("Attempt to delete Unknown pointer: %p", p);
	  p = NULL;
	  return;
	}
  }
  if(traceFlag)
    log_info("Deleted memory at address %p", p);

  //p = NULL;free(p) does not change the value of p itself, hence it still points to the same (now invalid) location
  //值为空，free(p) 不会改变指针p的值，p仍然指向相同(非法)的地址。
  p = NULL;
}

// Override array delete
void operator delete[](void* p)
{
  operator delete(p);
} ///:~

//fooTest类用于测试局部变量自动释放内存
class fooTest
{
  char* s;
public:
  fooTest(const char*s )
  {
    this->s = new char[strlen(s) + 1];
    strcpy(this->s, s);
  }
  ~fooTest()
  {
    delete [] s;
  }
};

//myTest类用于测试在调用重载new的情况下，也会自动调用构造函数与析构函数
class myTest
{
public:
  myTest()
  {
    cout<<"Construct my test..."<<endl;
  }
  ~myTest()
  {
    cout<<"Destruct my test..."<<endl;
  }
};

// Sentinel相当于内存守卫，全局的Sentinel对象将在程序退出前检测内存泄露情况
struct memGuard
{
  ~memGuard()
  {
    if(memFlag)
    {
      if(nPtrs > 0)
	  {
        for(size_t i = 0; i < nPtrs; ++i)
		  log_error("Leaked memory at: %p (file: %s, line %ld)", memMap[i].ptr, memMap[i].file, memMap[i].line);
      }
      else
	    log_info("No memory leaks!");
	}
  }
};
memGuard mG;

void test()
{
  //如果有undef new，没有undef delete，就不会调用重载的new，而调用重载的delete，内存跟踪会出错。
  //#undef new
  //#undef delete

  int* iPtr = new int;
  int* aPtr = new int[3];
  char * str = new char[9];

  memInfo_output();

  delete iPtr;
  //delete iPtr;   重复调用delete，程序crash
  delete [] aPtr;
  //delete[] str;
  delete str;

  //delete NULL 指针
  int* nPtr = NULL;
  delete nPtr;

  // 即使使用重载new,也会调用构造函数与析构函数
  myTest* mtPtr = new myTest;
  delete mtPtr;

  //vector 这里不会调到重载的new
  vector<int> v;
  v.push_back(1);

  //将调用2次重载new
  fooTest *ftPtr = new fooTest("goodbye");
  //内存泄露
  //delete ftPtr;

  //自动释放内存
  fooTest ft("goodbye");
}

int main()
{
  MEM_ON();
  TRACE_ON();

  test();

  //在这里不能够OFF，因为全局变量“析构”在此范围之外。
  //MEM_OFF();
  //TRACE_OFF();
}
*/
