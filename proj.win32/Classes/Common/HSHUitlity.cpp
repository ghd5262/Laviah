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