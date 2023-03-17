//
// Created by wjc on 2023/3/17.
//

#include "Clock.h"
#include <iostream>
using namespace std;

void Clock::Display() {
    cout<<hour_<<":"<<minute_<<":"<<second_<<endl;
}

Clock::Clock(int hour, int minute, int second) {
    hour_ = hour;
    minute_ = minute;
    second_ = second;
    cout<<"Clock::Clock"<<endl;
}

void Clock::Update() {
    second_++;
    if (second_ == 60)
    {
        minute_++;
        second_ = 0;
    }
    if (minute_ == 60)
    {
        hour_++;
        minute_ = 0;
    }
    if (hour_ == 24)
    {
        hour_ = 0;
    }
}

Clock::~Clock() {
    cout<<"Clock::~Clock"<<endl;
}