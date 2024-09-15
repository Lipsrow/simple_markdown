#pragma once
#include "abstract.h"
#include <forward_list>

extern int pos;
extern long words;
extern long symbols;
extern std::forward_list<Abstract_text*>::iterator next_element;

class Text_viewer {
public:
	Text_viewer();
	int read_file(const char* filename, std::forward_list<Abstract_text*>& elements);
	int print_file(std::forward_list<Abstract_text*>& elements, FILE* file);
	~Text_viewer();
};
