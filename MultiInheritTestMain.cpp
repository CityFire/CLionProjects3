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
        
    }
    int weigth_;
};

class Bed
{
public:
    Bed(int weight) : weigth_(weight)
    {

    }
    void Sleep()
    {
        cout<<"Sleep..."<<endl;
    }
    int weigth_;
};

class Sofa
{
public:
    Sofa(int weight) : weigth_(weight)
    {

    }
    void WatchTV()
    {
        cout<<"WatchTv..."<<endl;
    }
    int weigth_;
};

class SofaBed : public Bed, public Sofa
{
public:
    SofaBed() : Bed(0), Sofa(0)
    {
        FoldIn();
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
    SofaBed sofaBed;
    //sofaBed.weight_ = 10;
    //sofaBed.weight_ = 20;

    // 二义性
    //sofaBed.Bed::weight_ = 10;
    //sofaBed.Sofa::weight_ = 20;

    sofaBed.WatchTV();
    sofaBed.FoldOut();
    sofaBed.Sleep();

    return 0;
}