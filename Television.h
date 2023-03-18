//
// Created by wjc on 2023/3/18.
//

#ifndef DEMO_TELEVISION_H
#define DEMO_TELEVISION_H

class TeleController;

class Television {
    friend class TeleController;
public:
    Television(int volume, int channel);
private:
    int volume_;
    int channel_;
};

#endif //DEMO_TELEVISION_H
