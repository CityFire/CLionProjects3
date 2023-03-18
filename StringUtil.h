//
// Created by wjc on 2023/3/18.
//

#ifndef DEMO_STRINGUTIL_H
#define DEMO_STRINGUTIL_H

#include <string>
using namespace std;

class StringUtil {
public:
    static void LTrim(string& s);
    static void RTrim(string& s);
    static void Trim(string& s);
};


#endif //DEMO_STRINGUTIL_H
