namespace HSHDefine{
#define PI ((float)  3.141592654f)
#define DEBUG_DRAW 1
#define USE_MEMORY_POOLING 1
}

namespace COLOR {
    const cocos2d::Color3B GOLD(255, 255, 166);                 //Gold(255, 255, 166)
    const cocos2d::Color3B DARKGRAY(36, 36, 36);                //Dark Gray(36, 36, 36)
    const cocos2d::Color3B BRIGHTGRAY(255, 255, 255 - 14);      //Bright Gray_1(255, 255, 255 - 14)
    
    const cocos2d::Color4B TRANSPARENT_ALPHA(255, 255, 255, 0);
    const cocos2d::Color4B DARKGRAY_ALPHA(0, 0, 0, 255 * 0.8f);
    const cocos2d::Color4B BRIGHTGRAY_ALPHA(0, 0, 0, 255 * 0.4f);
    const cocos2d::Color4B WHITEGRAY_ALPHA(255, 255, 255, 255 * 0.8f);
	const cocos2d::Color4B BRIGHT_WHITEGRAY_ALPHA(255, 255, 255, 255 * 0.3f);
    const cocos2d::Color4B BRIGHTRED_ALPHA(255, 48, 48, 255 * 0.8f);
}

namespace FONT {
	const std::string MALGUNBD("fonts/malgunbd.ttf");
	const std::string NUMBER("fonts/Number.ttf");
}