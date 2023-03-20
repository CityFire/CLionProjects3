//
// Created by wjc on 2023/3/18.
//

#include <iostream>
#include <math.h>
using namespace std;

class Point
{
    // 友元
    friend double Distance(const Point& p1, const Point& p2);
public:
    Point(int x, int y);
    //~Point();
private:
    int x_;
    int y_;
};

Point::Point(int x, int y) : x_(x), y_(y)
{

}

double Distance(const Point& p1, const Point& p2)
{
    double dx = p1.x_ - p2.x_;
    double dy = p1.y_ - p2.y_;
    return sqrt(dx * dx + dy * dy);
}

class Television
{
    friend class TeleController;
public:
    Television(int volume, int channel) : volume_(volume), channel_(channel)
    {

    }
private:
    int volume_;
    int channel_;
};

class TeleController
{
public:
    void VolumeUp(Television& tv)
    {
        tv.volume_ += 1;
    }
    void VolumeDown(Television& tv)
    {
        tv.volume_ -= 1;
    }
    void ChannelUp(Television& tv)
    {
        tv.channel_ += 1;
    }
    void ChannelDown(Television& tv)
    {
        tv.channel_ -= 1;
    }
private:
};

int main000(void)
{
    Point p1(3, 4);
    Point p2(6, 8);
    double distance = Distance(p1, p2);
    cout<<"distance:"<<distance<<endl;


    Television tv(1, 1);

    TeleController tc;
    tc.VolumeUp(tv);
    tc.ChannelUp(tv);


    return 0;
}
