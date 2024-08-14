#include "title.h"

extern size_t width;
extern int tab;
void print_sharp_line(FILE* file) {
	for (size_t i = 0; i < width; i++) {
		fwprintf(file, L"#");
	}
}

Title::Title(const wchar_t* text, int level) {
	this->text = new wchar_t[wcslen(text) + 1];
	wcscpy(this->text, text);
	this->level = level;
}
unsigned long int Title::count_symbols() const {
	return (long)(2 * (width + level) + wcslen(text));
}
unsigned long int Title::count_words() const {
	if (!wcslen(text))
		return 0;
	unsigned long int count = 0;
	for (size_t i = 0; i < wcslen(text); i++) {
		if ((text[i] == L' ') || (text[i] == L'\t')) {
			count++;
		}
	}
	return ++count;
}
int Title::print(FILE* file) const {
	fwprintf(file, L"\n");
	size_t length = wcslen(text);
	size_t real_len = length;
	for (size_t i = 0; i < length; i++) {
		if (text[i] == L'\t')
			real_len += tab - 1;
	}
	if (real_len + 2 * level > width) {
		fwprintf(stderr, L"Слишком длинный заголовок\n");
		return -1;
	}
	size_t margin = (width - real_len) / 2;
	size_t pointer = 0;
	print_sharp_line(file);
	fwprintf(file, L"\n");
	while (pointer != width - (real_len - length)) {
		if (pointer < (size_t)level) {
			fwprintf(file, L"%lc", L'#');
		}
		else if (pointer < margin) {
			fwprintf(file, L"%lc", L' ');;
		}
		else if (pointer < margin + length) {
			if (text[pointer - margin] == L'\t') {
				for (int i = 0; i < tab; i++) {
					fwprintf(file, L"%lc", ' ');
				}
			}
			else {
				if (file == stdout)
					fwprintf(stdout, L"\x1b[33m");
				fwprintf(file, L"%lc", text[pointer - margin]);
				if (file == stdout)
					fwprintf(stdout, L"\x1b[0m");
			}
		}
		else if (pointer < width - level) {
			fwprintf(file, L" ");
		}
		else {
			fwprintf(file, L"%lc", '#');
		}
		pointer++;
	}
	fwprintf(file, L"\n");
	print_sharp_line(file);
	fwprintf(file, L"\n");
	fwprintf(file, L"\n");
	return 0;
}
Title::~Title() {
	delete[] text;
}

