//
// Created by wjc on 2023/3/19.
//

#include <iostream>
using namespace  std;

class Furniture
{
public:
    Furniture(int weight) : weigth_(weight)
    {
        cout<<"Furniture..."<<endl;
    }
    ~Furniture()
    {
        cout<<"~Furniture..."<<endl;
    }
    int weigth_;
};

class Bed : virtual public Furniture
{
public:
    Bed(int weight) : Furniture(weight)
    {
        cout<<"Bed..."<<endl;
    }
    ~Bed()
    {
        cout<<"~Bed..."<<endl;
    }
    void Sleep()
    {
        cout<<"Sleep..."<<endl;
    }
};

class Sofa : virtual public Furniture
{
public:
    Sofa(int weight) : Furniture(weight)
    {
        cout<<"Sofa..."<<endl;
    }
    ~Sofa()
    {
        cout<<"~Sofa..."<<endl;
    }
    void WatchTV()
    {
        cout<<"WatchTv..."<<endl;
    }
};

class SofaBed : public Bed, public Sofa
{
public:
    SofaBed(int weight) : Bed(weight), Sofa(weight), Furniture(weight)
    {
        cout<<"SofaBed..."<<endl;
        FoldIn();
    }
    ~SofaBed()
    {
        cout<<"~SofaBed..."<<endl;
    }
    void FoldOut()
    {
        cout<<"FoldOut..."<<endl;
    }
    void FoldIn()
    {
        cout<<"FoldIn..."<<endl;
    }
};

int main(void)
{
    SofaBed sofaBed(5);
    sofaBed.weigth_ = 10;

    sofaBed.WatchTV();
    sofaBed.FoldOut();
    sofaBed.Sleep();

//    Furniture...
//    Bed...
//    Sofa...
//    SofaBed...
//    FoldIn...
//    WatchTv...
//    FoldOut...
//    Sleep...
//    ~SofaBed...
//    ~Sofa...
//    ~Bed...
//    ~Furniture...

    return 0;
}