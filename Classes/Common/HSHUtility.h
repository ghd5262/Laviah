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
#include "Global.hpp"
#include "../DataManager/TranslateManager.hpp"
#include "../DataManager/GameMetaDataManager.hpp"

static const int kMaxLogLen = 5 * 1024;
std::string MakeString(const char* str, ...); 
std::string MakeCryptoString(std::string origin, std::string key);

#endif //__HSHUTILITY_H__
