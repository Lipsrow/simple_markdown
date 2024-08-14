#include "list.h"
#include <math.h>

List::List(const wchar_t* text, int level, int number) {
	this->text = new wchar_t[wcslen(text) + 1];
	wcscpy(this->text, text);
	this->level = level;
	this->number = number;
}

unsigned long int List::count_symbols() const {
	long x;
	if (number == -1)
		x = 1;
	else
		x = 2;
	return (unsigned long)(wcslen(text)) + x;
}

unsigned long int List::count_words() const {
	if (!wcslen(text))
		return 0;
	unsigned long int count = 0;
	for (size_t i = 0; i < wcslen(text); i++) {
		if ((text[i] == L' ') || (text[i] == L'\t') || (text[i] == L'\n')) {
			count++;
		}
	}
	return count;
}

int List::print(FILE* file) const {
	wchar_t* new_word = new wchar_t[width + 1];
	int word_len = 0;
	for (int i = 0; i < 2 * level; i++) {
		fwprintf(file, L"%ls", L" ");
	}
	if (file == stdout)
		fwprintf(stdout, L"\x1b[36m");
	if (number != -1) {
		fwprintf(file, L"%d.  ", number);
		position += 2 * level + 3 + (int)log10(number) + 1;
	}
	else {
		fwprintf(file, L"%lc   ", mark);
		position += 2 * level + 4;
	}
	if (file == stdout)
		fwprintf(stdout, L"\x1b[0m");
	for (size_t i = 0; i < wcslen(text); i++) {
		if ((text[i] == L' ') || (text[i] == L'\t') || (text[i] == L'\n')) {
			if ((size_t)(word_len + position) == width) {
				fwprintf(file, L"%ls\n", new_word);
				if (text[i] == L'\t') {
					for (int k = 0; k < 2 * level + 4; k++) {
						fwprintf(file, L" ");
					}
					for (int i = 0; i < tab; i++) {
						fwprintf(file, L" ");
					}
					position = tab + 2 * level + 4;
				}
				else if (text[i] == L' ') {
					for (int k = 0; k < 2 * level + 4; k++) {
						fwprintf(file, L" ");
					}
					position = 2 * level + 4;
				}
				else {
					fwprintf(file, L"%lc", text[i]);
					position = 0;
				}
			}
			else if ((size_t)(word_len + position) > width) {
				int first_size = width - position - 1;
				int second_size = word_len - first_size;
				if ((first_size > 1) && (second_size > 1)) {
					for (int j = 0; j < first_size; j++) {
						fwprintf(file, L"%lc", new_word[j]);
					}
					fwprintf(file, L"-\n");
					for (int k = 0; k < 2 * level + 4; k++) {
						fwprintf(file, L" ");
					}
					for (int j = first_size; j < word_len; j++) {
						fwprintf(file, L"%lc", new_word[j]);
					}
					position = second_size + 2 * level + 4;
					if (text[i] == L'\t') {
						for (int i = 0; i < tab; i++) {
							fwprintf(file, L" ");
						}
						position += tab;
					}
					else if (text[i] == L' ') {
						fwprintf(file, L"%lc", text[i]);
						position++;
					}
					else {
						fwprintf(file, L"%lc", text[i]);
						position = 0;
					}
				}
				else {
					fwprintf(file, L"\n");
					for (int k = 0; k < 2 * level + 4; k++) {
						fwprintf(file, L" ");
					}
					new_word[word_len] = L'\0';
					fwprintf(file, L"%ls", new_word);
					position = wcslen(new_word) + 2 * level + 4;
					if (text[i] == L'\t') {
						for (int i = 0; i < tab; i++) {
							fwprintf(file, L" ");
						}
						position += tab;
					}
					else if (text[i] == L' ') {
						fwprintf(file, L"%lc", text[i]);
						position++;
					}
					else {
						fwprintf(file, L"%lc", text[i]);
						position = 0;
					}
				}
			}
			else {
				new_word[word_len] = L'\0';
				fwprintf(file, L"%ls", new_word);
				position += wcslen(new_word);
				if (text[i] == L'\t') {
					for (int i = 0; i < tab; i++) {
						fwprintf(file, L" ");
					}
					position += tab;
					if ((size_t)(position) >= width) {
						for (int k = 0; k < 2 * level + 4; k++) {
							fwprintf(file, L" ");
						}
						position += 2 * level + 4;
					}
				}
				else if (text[i] == L' ') {
					fwprintf(file, L"%lc", text[i]);
					position++;
				}
				else {
					fwprintf(file, L"%lc", text[i]);
					position = 0;
				}
			}
			word_len = 0;
			memset(new_word, 0, wcslen(new_word));
		}
		else {
			new_word[word_len] = text[i];
			word_len++;
		}
		if ((size_t)(position) >= width) {
			fwprintf(file, L"\n");
			for (int k = 0; k < 2 * level + 4; k++) {
				fwprintf(file, L" ");
			}
			position = 2 * level + 4;
		}
	}
	delete[] new_word;
	return 0;
}

List::~List() {
	delete[] text;
}
