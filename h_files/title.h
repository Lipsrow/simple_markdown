#pragma once
#include "abstract.h"


class Title : public Abstract_text {
public:
	int level;
	wchar_t* text;
	Title(const wchar_t* text, int level);
	unsigned long int count_symbols() const override;
	unsigned long int count_words() const override;
	int print(FILE* file) const override;
	~Title();
};
