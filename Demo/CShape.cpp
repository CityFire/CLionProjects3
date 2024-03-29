//
// Created by wjc on 2023/3/20.
//

#include "CShape.h"
#include "DynamicBase.h"
#include <iostream>
using namespace std;

void CCircle::Draw()
{
    cout<<"CCircle::Draw()..."<<endl;
}

CCircle::~CCircle()
{
    cout<<"~CCircle..."<<endl;
}

void CSquare::Draw()
{
    cout<<"CSquare::Draw()..."<<endl;
}

CSquare::~CSquare()
{
    cout<<"~CSquare..."<<endl;
}

void CRectangle::Draw()
{
    cout<<"CRectangle::Draw()..."<<endl;
}

CRectangle::~CRectangle()
{
    cout<<"~CRectangle..."<<endl;
}

REGISTER_CLASS(CCircle);
REGISTER_CLASS(CSquare);
REGISTER_CLASS(CRectangle);
/*
class CCircleRegister
{
public:
    static void* NewInstance()
    {
        return new CCircle;
    }
private:
    static Register reg_;
};
Register CCircleRegister::reg_("CCircle", CCircleRegister::NewInstance)

#define REGISTER_CLASS(class_name) \
class class_name##Register { \
public: \
    static void* NewInstance() \
    { \
        return new class_name; \
    } \
private: \
    static Register reg_; \
}; \
Register class_name##Register::reg_(#class_name, class_name##Register::NewInstance)
 */