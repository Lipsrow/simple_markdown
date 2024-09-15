#pragma once
#include <cstdio>
#include <wchar.h>
#include <cstring>

class Abstract_text {
public:
	wchar_t* text;
	virtual unsigned long int count_symbols() const = 0;
	virtual unsigned long int count_words() const = 0;
	virtual int print(FILE* file) const = 0;
	virtual ~Abstract_text();
};