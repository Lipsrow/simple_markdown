#pragma once
#include "abstract.h"

class Code : public Abstract_text {
public:
	wchar_t* text;
	Code(const wchar_t* text);
	unsigned long int count_symbols() const override;
	unsigned long int count_words() const override;
	int print(FILE* file) const override;
	~Code();
};
