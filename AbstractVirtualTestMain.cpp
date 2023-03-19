//
// Created by wjc on 2023/3/20.
//
#include <iostream>
#include <vector>
using namespace std;
// 纯虚函数
class Shape
{
public:
    virtual void Draw() = 0;
//    void Draw()
//    {
//        cout<<"Shape::Draw()..."<<endl;
//    }
    virtual ~Shape(){} // 不添加虚析构函数，派生类析构函数无法释放
};

class Circle : public Shape
{
public:
    void Draw()
    {
        cout<<"Circle::Draw()..."<<endl;
    }
    ~Circle()
    {
        cout<<"~Circle..."<<endl;
    }
};

class Square : public Shape
{
public:
    void Draw()
    {
        cout<<"Square::Draw()..."<<endl;
    }
    ~Square()
    {
        cout<<"~Square..."<<endl;
    }
};

// 多态
class Rectangle : public Shape
{
    void Draw()
    {
        cout<<"Rectangle::Draw()..."<<endl;
    }
    ~Rectangle()
    {
        cout<<"~Rectangle..."<<endl;
    }
};

void DrawAllShapes(const vector<Shape*>& v)
{
    vector<Shape*>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        (*it)->Draw();
    }
}

void DeleteAllShapes(const vector<Shape*>& v)
{
    vector<Shape*>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        delete(*it);
    }
}

// 简单工厂模式
class ShapeFactory
{
public:
    static Shape* CreateShape(const string& name)
    {
        Shape* ps = 0;
        if (name == "Circle")
        {
            ps = new Circle;
        }
        else if (name == "Square")
        {
            ps = new Square;
        }
        else if (name == "Rectangle")
        {
            ps = new Rectangle;
        }
        return ps;
    }
};

int main(void)
{
//    Shape s;  // Error 不能实例化抽象类
    vector<Shape*> v;
//    Shape* ps;
//    ps = new Circle;
//    v.push_back(ps);
//    ps = new Square;
//    v.push_back(ps);
//    ps = new Rectangle;
//    v.push_back(ps);

    Shape* ps;
    ps = ShapeFactory::CreateShape("Circle");
    v.push_back(ps);
    ps = ShapeFactory::CreateShape("Square");
    v.push_back(ps);
    ps = ShapeFactory::CreateShape("Rectangle");
    v.push_back(ps);

    DrawAllShapes(v);
    DeleteAllShapes(v);

    return 0;
}