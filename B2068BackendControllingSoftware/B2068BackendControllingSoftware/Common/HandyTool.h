#pragma once

typedef unsigned long long hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

class CHandyTool
{
public:
	CHandyTool();
	~CHandyTool();

	static hash_t hash_(const char *str);
	static constexpr hash_t hash_compile_time(const char *str, hash_t lastValue = basis);

private:

};

constexpr unsigned long long hash_compile_time(const char *str, hash_t lastValue = basis)
{
	return *str ? hash_compile_time(str + 1, (*str ^ lastValue) * prime) : lastValue;
}

constexpr unsigned long long operator ""_hash(const char *str, size_t) {
	return hash_compile_time(str);
}
