//
// Created by wjc on 2023/3/18.
//

#ifndef DEMO_TELECONTROLLER_H
#define DEMO_TELECONTROLLER_H

class Television;

class TeleController {
public:
    void VolumeUp(Television& tv);
    void VolumeDown(Television& tv);
    void ChannelUp(Television& tv);
    void ChannelDown(Television& tv);
};

#endif //DEMO_TELECONTROLLER_H
