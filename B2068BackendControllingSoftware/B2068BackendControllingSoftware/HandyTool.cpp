#include "stdafx.h"
#include "HandyTool.h"


CHandyTool::CHandyTool()
{
}


CHandyTool::~CHandyTool()
{
}

hash_t CHandyTool::hash_(const char *str)
{
	hash_t ret(basis);

	while (*str) {
		ret ^= *str;
		ret *= prime;
		++str;
	}

	return ret;
}

constexpr hash_t CHandyTool::hash_compile_time(const char *str, hash_t lastValue /*= basis*/)
{
	return *str ? hash_compile_time(str + 1, (*str ^ lastValue) * prime) : lastValue;
}
