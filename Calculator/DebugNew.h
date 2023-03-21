//
// Created by wjc on 2023/3/22.
//

#ifndef CALCULATOR_DEBUGNEW_H
#define CALCULATOR_DEBUGNEW_H

#ifndef NDEBUG
#include "Tracer.h"
#define new new(__FILE__, __LINE__)
#endif

#endif //CALCULATOR_DEBUGNEW_H
