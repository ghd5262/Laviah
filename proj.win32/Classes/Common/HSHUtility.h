#ifndef __HSHUTILITY_H__
#define __HSHUTILITY_H__

#include <iostream>
#include "cocos2d.h"
#include <stdarg.h >
#include "Defines.h"
#include "enums.h"
#include <algorithm>
#include <memory>
#include <time.h>
#include "audio\include\AudioEngine.h"
#include "ui/UIButton.h"

using namespace HSHDefine;
using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace cocos2d::ui;
static const int kMaxLogLen = 5 * 1024;
std::string MakeString(const char* str, ...);
#endif //__HSHUTILITY_H__