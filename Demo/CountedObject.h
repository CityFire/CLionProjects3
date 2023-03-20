//
// Created by wjc on 2023/3/18.
//

#ifndef DEMO_COUNTEDOBJECT_H
#define DEMO_COUNTEDOBJECT_H


class CountedObject {
public:
    CountedObject();
    ~CountedObject();
public:
    static int GetCount();
private:
    static int count_;  // 静态成员的引用性声明
};


#endif //DEMO_COUNTEDOBJECT_H
