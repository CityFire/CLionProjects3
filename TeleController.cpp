//
// Created by wjc on 2023/3/18.
//

#include "TeleController.h"
#include "Television.h"

void TeleController::VolumeUp(Television& tv)
{
    tv.volume_ += 1;
}

void TeleController::VolumeDown(Television& tv)
{
    tv.volume_ -= 1;
}

void TeleController::ChannelUp(Television& tv)
{
    tv.channel_ += 1;
}

void TeleController::ChannelDown(Television& tv)
{
    tv.channel_ -= 1;
}