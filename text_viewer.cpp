#include "text_viewer.h"
#include "paragraph.h"
#include "code.h"
#include "title.h"
#include "list.h"

#include <iostream>
#include <wchar.h>
#include <cstring>
#include <ctype.h>
using namespace std;

enum State {
	TITLE,
	LIST,
	PARAGRAPH,
	CODE,
	NEW_LINE
};

int list_numbers[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const size_t MAX_LEN = 2000;

int Text_viewer::read_file(const char* filename, forward_list<Abstract_text*>& elements) {
	enum State current = PARAGRAPH;
	FILE* file = fopen(filename, "r");
	wchar_t buffer[MAX_LEN];
	wchar_t* text = new wchar_t[MAX_LEN];
	int level = 0;
	int number = 0;
	int empty = 0;
	bool first = true;
	if (file == NULL) {
		fwprintf(stderr, L"Не удалось открыть файл!\n");
		return -1;
	}
	while (fgetws(buffer, MAX_LEN, file) != NULL) {
		if (!first) {
			if (!iswalpha(buffer[0]) && buffer[0] != '-') {
				switch (current) {
				case LIST:
				{
					List* list = new List(text, level, number);
					elements.insert_after(next_element, list);
					++next_element;
					break;
				}
				case PARAGRAPH:
				{
					text[pos++] = L'\n';
					text[pos] = L'\0';
					Paragraph* par = new Paragraph(text);
					elements.insert_after(next_element, par);
					++next_element;
					break;
				}
				default:
					break;
				}
			}
		}
		if (buffer[0] == L'\n') { //Пустая строка
			current = NEW_LINE;
			empty++;
			if (empty > 1) {
				Paragraph* emp = new Paragraph(L"\n");
				elements.insert_after(next_element, emp);
				++next_element;
			}
		}
		else if (buffer[0] == L'#') { //Title 1+ level
			current = TITLE;
			buffer[wcslen(buffer) - 1] = L'\0';
			int counter = 0;
			pos = 0;
			while (buffer[pos] == L'#') {
				counter++;
				pos++;
			}
			while (buffer[pos] == L' ') {
				pos++;
			}
			wcscpy(text, buffer + pos);
			Title* pointer = new Title(text, counter);
			elements.insert_after(next_element, pointer);
			++next_element;
		}
		else if (buffer[0] == L'-' || buffer[1] == L'-') { //Title 0 level
			current = TITLE;
			text[pos - 1] = L'\0';
			Title* pointer = new Title(text, 0);
			elements.insert_after(next_element, pointer);
			++next_element;
		}
		else if (iswdigit(buffer[0]) || buffer[0] == L'*') { //List 0 level
			current = LIST;
			pos = 0;
			level = 0;
			list_numbers[level]++;
			if (buffer[pos] == L'*') {
				number = -1;
			}
			else {
				if (!iswdigit(buffer[pos])) {
					fwprintf(stderr, L"Неправильный формат списка! После отступов должно идти число или символ *\n");
					return -1;
				}
				number = list_numbers[level];
			}
			if (number != - 1) {
				while (buffer[pos] != L'.') {
					pos++;
					if ((size_t)pos > MAX_LEN) {
						fwprintf(stderr, L"Неправильный формат списка! Отсутсвует точка!\n");
						return -1;
					}
				}
			}
			pos++;
			while (buffer[pos] == L' ') {
				pos++;
			}
			wcscpy(text, buffer + pos);
		}
		else if ((buffer[0] == L' ' && buffer[1] == L' ' && buffer[2] == L' ' && buffer[3] == L' '
			&& !iswdigit(buffer[4]) && buffer[4] != '*') || buffer[0] == '\t') { //Code
			if (current == CODE) {
				Code* code = new Code(text);
				elements.insert_after(next_element, code);
				++next_element;
				memset(text, 0, MAX_LEN);
				wcscpy(text, buffer);
				text[wcslen(text)] = L'\0';
			}
			else {
				pos = 0;
				if (buffer[0] == '\t') {
					pos++;
				}
				else {
					pos += 4;
				}
				wcscpy(text, buffer + pos);
			}
			current = CODE;
		}
		else if (buffer[0] == ' ') { //List 1+ level
			current = LIST;
			int counter = 0;
			pos = 0;
			while (buffer[pos] == L' ') {
				counter++;
				pos++;
			}
			level = counter / 2;
			list_numbers[level]++;
			if (buffer[pos] == L'*') {
				number = -1;
			}
			else {
				if (!iswdigit(buffer[pos])) {
					fwprintf(stderr, L"Неправильный формат списка! После отступов должно идти число или символ *\n");
					return -1;
				}
				number = list_numbers[level];
			}
			if (number != - 1) {
				while (buffer[pos] != L'.') {
					pos++;
					if ((size_t)pos > MAX_LEN) {
						fwprintf(stderr, L"Неправильный формат списка! Отсутсвует точка!\n");
						return -1;
					}
				}
			}
			pos++;
			while (buffer[pos] == L' ') {
				pos++;
			}
			wcscpy(text, buffer + pos);
		}
		else if (first || (empty > 0 && current != PARAGRAPH) || (empty > 1)) {
			current = PARAGRAPH;
			pos = 0;
				memset(text, 0, MAX_LEN);
				for (size_t i = 0; i < wcslen(buffer); i++) {
					if (buffer[i] == L'\n') {
						text[pos++] = L' ';
					}
					else if ((buffer[i]) == L' ' && (i > 0)) {
						if (buffer[i - 1] != L' ')
							text[pos++] = buffer[i];
					}
					else {
						text[pos++] = buffer[i];
					}
				}
		}
		else {
			if (current == PARAGRAPH) {
				for (size_t i = 0; i < wcslen(buffer); i++) {
						if (buffer[i] == L'\n') {
							text[pos++] = L' ';
						}
						else if ((buffer[i]) == L' ' && (i > 0)) {
							if (buffer[i - 1] != L' ')
								text[pos++] = buffer[i];
						}
						else {
							text[pos++] = buffer[i];
						}
					}
			}
			else if (current == CODE) {
					Code* code = new Code(text);
					elements.insert_after(next_element, code);
					++next_element;
					memset(text, 0, MAX_LEN);
					wcscat(text, buffer);
					text[wcslen(text)] = L'\0';
				}
			else {
				text[wcslen(text) - 1] = ' ';
				text[wcslen(text)] = L'\0';
				wcscat(text, buffer);
			}
		}
		if (current != LIST) {
			for (int i = 0; i < 10; i++) {
				list_numbers[i] = 0;
			}
		}
		if (wcscmp(buffer, L"\n"))
			empty = 0;
		first = false;
	}
	switch (current) {
	case LIST:
	{
		List* list = new List(text, level, number);
		elements.insert_after(next_element, list);
		++next_element;
		break;
	}
	case PARAGRAPH:
	{
		text[pos++] = L'\n';
		text[pos] = L'\0';
		Paragraph* par = new Paragraph(text);
		elements.insert_after(next_element, par);
		++next_element;
		break;
	}
	case CODE:
	{
		Code* code = new Code(text);
		elements.insert_after(next_element, code);
		++next_element;
		break;
	}
	default:
		break;
	}
	fclose(file);
	delete[] text;
	return 0;
}



int Text_viewer::print_file(forward_list<Abstract_text*>& elements, FILE* file) {
	for (auto it = elements.begin(); it != elements.end(); ++it) {
		if (((*it)->print(file)) == -1) {
			return -1;
		}
		symbols += (*it)->count_symbols();
		words += (*it)->count_words();
	}
	return 0;
}

Text_viewer::Text_viewer() {}
Text_viewer::~Text_viewer() {}
