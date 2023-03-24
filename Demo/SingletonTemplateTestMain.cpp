//
// Created by wjc on 2023/3/24.
//
#include <iostream>
using namespace std;
#include "SingletonTemplate.h"

class ApplicationImpl
{
public:
    ApplicationImpl() { cout<<"ApplicaitonImpl..."<<endl; }
    ~ApplicationImpl() { cout<<"~ApplicationImpl..."<<endl; }
    void Run() { cout <<"Run.."<<endl; }
};

typedef SingletonTemplate<ApplicationImpl> Application;

int main(void)
{
    Application::GetInstance().Run();
    Application::GetInstance().Run();
    return 0;
}