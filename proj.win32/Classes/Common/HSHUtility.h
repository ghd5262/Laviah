#ifndef __HSHUTILITY_H__
#define __HSHUTILITY_H__

#include <iostream>
#include "cocos2d.h"
#include "Defines.h"
#include <algorithm>
#include <memory>
#include <time.h>
#include "ui/UIButton.h"
#include "AudioManager.h"

using namespace HSHDefine;
using namespace cocos2d;
using namespace cocos2d::ui;
static const int kMaxLogLen = 5 * 1024;
std::string MakeString(const char* str, ...); 
std::string MakeCryptoString(std::string origin, std::string key);

const Color3B g_labelColor1(36, 36, 36);			//Dark Gray(36, 36, 36)
const Color3B g_labelColor2(255, 255, 255 - 14);	//Bright Gray_1(255, 255, 255 - 14)
const Color3B g_labelColor3(255, 255, 166);			//Bright Gray_2(255, 255, 166)

#endif //__HSHUTILITY_H__