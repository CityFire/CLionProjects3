//
// Created by wjc on 2023/3/17.
//

#ifndef DEMO_CLOCK_H
#define DEMO_CLOCK_H


class Clock {
public:
    Clock(int hour = 0, int minute = 0, int second = 0);
    void Display();
    void Update();

    ~Clock();
private:
    int hour_;
    int minute_;
    int second_;
};


#endif //DEMO_CLOCK_H
