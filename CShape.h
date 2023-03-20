//
// Created by wjc on 2023/3/20.
//

#ifndef DEMO_CSHAPE_H
#define DEMO_CSHAPE_H

// 纯虚函数
class CShape
{
public:
    virtual void Draw() = 0;
    virtual ~CShape(){} // 不添加虚析构函数，派生类析构函数无法释放
};

class CCircle : public CShape
{
public:
    void Draw();
    ~CCircle();
};

class CSquare : public CShape
{
public:
    void Draw();
    ~CSquare();
};

class CRectangle : public CShape
{
    void Draw();
    ~CRectangle();
};

#endif //DEMO_CSHAPE_H
