//
// Created by wjc on 2023/3/21.
//

#ifndef CALCULATORTEST_DEBUGNEW_H
#define CALCULATORTEST_DEBUGNEW_H

#ifndef NDEBUG
#include "Tracer.h"
#define new new(__FILE__, __LINE__)
#endif

#endif //CALCULATORTEST_DEBUGNEW_H
