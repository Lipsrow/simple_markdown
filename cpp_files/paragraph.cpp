#include "paragraph.h"

Paragraph::Paragraph(const wchar_t* text) {
	this->text = new wchar_t[wcslen(text) + 1];
	wcscpy(this->text, text);
}
unsigned long int Paragraph::count_symbols() const {
	return (unsigned long)(wcslen(text)) - 2;
}
unsigned long int Paragraph::count_words() const {
	if ((text[0] == L'\n') || (!wcslen(text)))
		return 0;
	unsigned long int count = 0;
	for (size_t i = 0; i < wcslen(text); i++) {
		if ((text[i] == L' ') || (text[i] == L'\t')) {
			count++;
		}
	}
	return count;
}
int Paragraph::print(FILE* file) const {
	wchar_t* new_word = new wchar_t[width + 1];
	int word_len = 0;
	for (int i = 0; i < red_line; i++) {
		fwprintf(file, L" ");
	}
	position += red_line;
	for (size_t i = 0; i < wcslen(text); i++) {
		if ((text[i] == L' ') || (text[i] == L'\t') || (text[i] == L'\n')) {
			if ((size_t)(word_len + position) == width) {
				new_word[word_len] = L'\0';
				fwprintf(file, L"%ls\n", new_word);
				if (text[i] == L'\t') {
					for (int i = 0; i < tab; i++) {
						fwprintf(file, L" ");
					}
					position = tab - 1;
				}
				else
					position = 0;
			}
			else if ((size_t)(word_len + position) > width) {
				int first_size = width - position - 1;
				int second_size = word_len - first_size;
				if ((first_size > 1) && (second_size > 1)) {
					for (int j = 0; j < first_size; j++) {
						fwprintf(file, L"%lc", new_word[j]);
					}
					fwprintf(file, L"%lc\n", '-');
					for (int j = first_size; j < word_len; j++) {
						fwprintf(file, L"%lc", new_word[j]);
					}
					position = second_size;
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
					new_word[word_len] = '\0';
					fwprintf(file, L"%ls", new_word);
					position = wcslen(new_word);
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
				}
				else if (text[i] == L' ') {
					fwprintf(file, L"%lc", text[i]);
					position++;
				}
				else {
					fwprintf(file, L"%lc", text[i]);;
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
			position = 0;
		}
	}
	delete[] new_word;
	return 0;
}
Paragraph::~Paragraph() {
	delete[] text;
}
