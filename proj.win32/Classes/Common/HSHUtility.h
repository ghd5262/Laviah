#ifndef __HSHUTILITY_H__
#define __HSHUTILITY_H__

#include "cocos2d.h"
#include <stdarg.h >
#include "Defines.h"
#include "enums.h"

using namespace HSHDefine;
using namespace cocos2d;
static const int kMaxLogLen = 5 * 1024;
std::string MakeString(const char* str, ...);

#endif //__HSHUTILITY_H__