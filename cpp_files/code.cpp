#include "code.h"

extern size_t width;
extern int tab;
extern int position;

Code::Code(const wchar_t* text) {
	this->text = new wchar_t[wcslen(text) + 1];
	wcscpy(this->text, text);
}

unsigned long int Code::count_symbols() const {
	int pos = 0;
	while ((text[pos] == ' ') || (text[pos] == '\t'))
		pos++;
	return (long)wcslen(text) - pos;
}

unsigned long int Code::count_words() const {
	if ((text[0] == '\n') || (!wcslen(text)))
		return 0;
	unsigned long int count = 0;
	for (size_t i = 0; i < wcslen(text); i++) {
		if ((text[i] == L' ') || (text[i] == L'\t')) {
			count++;
		}
	}
	return ++count;
}

int Code::print(FILE* file) const {
	for (size_t i = 0; i < wcslen(text); i++) {
		if (position == 0) {
			if (file == stdout)
				fwprintf(file, L"\x1b[32m");
			fwprintf(file, L"@   ");
			if (file == stdout)
				fwprintf(file, L"\x1b[0m");
			position = 4;
			--i;
		}
		else if ((size_t)(position) < width) {
			if (text[i] == L'\n') {
				fwprintf(file, L"%lc", text[i]);
				position = 0;
			}
			else if (text[i] == L'\t') {
				if ((size_t)(position + tab) <= width) {
					for (int i = 0; i < tab; i++) {
						fwprintf(file, L" ");
					}
					position += tab;
				}
				else {
					for (size_t t = position; t <= width; t++) {
						fwprintf(file, L" ");
						position++;
					}
				}
			}
			else {
				fwprintf(file, L"%lc", text[i]);
				position++;
			}
		}
		else {
			if (text[i] == L'\n') {
				fwprintf(file, L"%lc", text[i]);
				position = 0;
			}
			else {
				continue;
			}
		}
	}
	return 0;
}

Code::~Code() {
	delete[] text;
}

