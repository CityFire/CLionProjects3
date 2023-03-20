//
// Created by wjc on 2023/3/20.
//
#include <iostream>
#include <vector>
#include "CShape.h"
#include "DynamicBase.h"
using namespace std;

void DrawAllShapes(const vector<CShape*>& v)
{
    vector<CShape*>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        (*it)->Draw();
    }
}

void DeleteAllShapes(const vector<CShape*>& v)
{
    vector<CShape*>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        delete(*it);
    }
}

int main(void)
{
    vector<CShape*> v;
    CShape* ps;
    // 对象动态创建
    ps = static_cast<CShape*>(DynObjcetFactory::CreateObject("CCircle"));
    v.push_back(ps);
    ps = static_cast<CShape*>(DynObjcetFactory::CreateObject("CSquare"));
    v.push_back(ps);
    ps = static_cast<CShape*>(DynObjcetFactory::CreateObject("CRectangle"));
    v.push_back(ps);

    DrawAllShapes(v);
    DeleteAllShapes(v);

    return 0;
}