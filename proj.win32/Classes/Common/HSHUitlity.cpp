#include "HSHUtility.h"

std::string MakeString(const char* str, ...)
{

	char buf[kMaxLogLen] = { 0, };
	va_list args;
	va_start(args, str);
	vsprintf(buf, str, args);
	va_end(args);

	return buf;
}


std::string MakeCryptoString(std::string origin, std::string key)
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return origin;
//#else
    size_t keySize = key.size();
    size_t originSize = origin.size();
    std::string cryptoString = "";
    
    int keyCount = 0;
    for(int i = 0; i < originSize; i++)
    {
        cryptoString.push_back(origin.at(i) ^ key.at(keyCount++));
        if(keyCount == keySize)
            keyCount = 0;
    }
    return cryptoString;
//#endif
}