//
// Created by wjc on 2023/3/19.
//

#include <iostream>
using namespace  std;

class Furniture
{
public:
//    Furniture(int weight) : weigth_(weight)
//    {
//
//    }
    int weigth_;
};

class Bed : virtual public Furniture
{
public:
//    Bed(int weight) : Furniture(weight)
//    {
//
//    }
    void Sleep()
    {
        cout<<"Sleep..."<<endl;
    }
};

class Sofa : virtual public Furniture
{
public:
//    Sofa(int weight) : Furniture(weight)
//    {
//
//    }
    void WatchTV()
    {
        cout<<"WatchTv..."<<endl;
    }
};

class SofaBed : public Bed, public Sofa
{
public:
//    SofaBed(int weight) : Bed(weight), Sofa(weight)
//    {
//        FoldIn();
//    }
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
//    SofaBed sofaBed(10);
    SofaBed sofaBed;
    sofaBed.weigth_ = 10;
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