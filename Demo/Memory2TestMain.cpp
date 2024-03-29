//
// Created by wjc on 2023/3/31.
//
/*
#include<iostream>
using namespace std;

//---------------------------------------------------------------
// 内存记录
//---------------------------------------------------------------
class MemInfo {
private:
    void* ptr;
    const char* file;
    unsigned int line;
    MemInfo* link;
    friend class MemStack;
};
//---------------------------------------------------------------
// 内存记录栈
//---------------------------------------------------------------
class MemStack {
private:
    MemInfo* head;
public:
    MemStack():head(NULL) { }
    ~MemStack() {
        MemInfo* tmp;
        while(head != NULL) {
            free(head->ptr); // 释放泄漏的内存
            tmp = head->link;
            free(head);
            head = tmp;
        }
    }
    void Insert(void* ptr, const char* file, unsigned int line) {
        MemInfo* node = (MemInfo*)malloc(sizeof(MemInfo));
        node->ptr = ptr; node->file = file; node->line=line;
        node->link = head; head = node;
    }
    void Delete(void* ptr) {
        MemInfo* node = head;
        MemInfo* pre = NULL;
        while(node != NULL && node->ptr!=ptr) {
            pre = node;
            node = node->link;
        }
        if(node == NULL)
            cout << "删除一个没有开辟的内存" << endl;
        else {
            if(pre == NULL) // 删除的是head
                head = node->link;
            else
                pre->link = node->link;
            free(node);
        }
    }
    void Print() {
        if(head == NULL) {
            cout << "内存都释放掉了" << endl;
            return;
        }
        cout << "有内存泄露出现" << endl;
        MemInfo* node = head;
        while(node != NULL) {
            cout << "文件名: " << node->file << " , " << "行数: " << node->line << " , "
                 << "地址: " << node->ptr << endl;
            node = node->link;
        }
    }
};
//---------------------------------------------------------------
// 全局对象 mem_stack记录开辟的内存
//---------------------------------------------------------------
MemStack mem_stack;
//---------------------------------------------------------------
// 重载new,new[],delete,delete[]
//---------------------------------------------------------------
void* operator new(size_t size, const char* file, unsigned int line) {
    void* ptr = malloc(size);
    mem_stack.Insert(ptr, file, line);
    return ptr;
}
void* operator new[](size_t size, const char* file, unsigned int line) {
    return operator new(size, file, line); // 不能用new
}
void operator delete(void* ptr) {
    free(ptr);
    mem_stack.Delete(ptr);
}
void operator delete[](void* ptr) {
    operator delete(ptr);
}
//---------------------------------------------------------------
// 使用宏将带测试代码中的new和delte替换为重载的new和delete
//---------------------------------------------------------------
#define new new(__FILE__,__LINE__)
//---------------------------------------------------------------
// 待测试代码
//---------------------------------------------------------------
void bad_code() {
    int *p = new int;
    char *q = new char[5];
    delete []q;
}

void good_code() {
    int *p = new int;
    char *q = new char[5];
    delete p;
    delete []q;
}
//---------------------------------------------------------------
// 测试过程
//---------------------------------------------------------------
int main() {
    good_code();
    bad_code();
    mem_stack.Print();
    system("PAUSE");
    return 0;
}
*/
