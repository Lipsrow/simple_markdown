#pragma once
#include "abstract.h"

extern int position;
extern size_t width;
extern wchar_t mark;
extern int tab;

class List : public Abstract_text {
public:
	wchar_t* text;
	int level;
	int number;
	List(const wchar_t* text, int level, int number);
	unsigned long int count_symbols() const override;
	unsigned long int count_words() const override;
	int print(FILE* file) const override;
	~List();
};

