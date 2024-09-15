#pragma once
#include "abstract.h"

extern int position;
extern size_t width;
extern int red_line;
extern int tab;

class Paragraph : public Abstract_text {
public:
	wchar_t* text;
	Paragraph(const wchar_t* text);
	unsigned long int count_symbols() const override;
	unsigned long int count_words() const override;
	int print(FILE* file) const override;
	~Paragraph();
};
