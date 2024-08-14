#include "text_viewer.h"
#include "title.h"
#include "list.h"
#include "paragraph.h"
#include "code.h"

#include <locale>
#include <getopt.h>
#include <sys/ioctl.h>

#include <string.h>
#include <unistd.h>

#include <forward_list>
using namespace std;

int position = 0;
size_t width = 80;
int tab = 4;
wchar_t mark = L'*';
int red_line = 8;

forward_list<Abstract_text*> elements;
forward_list<Abstract_text*>::iterator next_element = elements.before_begin();


bool help = false;
bool version = false;
bool file = false;
bool output_file = false;

int pos = 0;
long words = 0;
long symbols = 0;



int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru_RU.UTF-8");
	const char *opts = "f:t:m:r:o:hv";
	int option;
	char filename[20];
	char output_name[20];
	FILE* output = stdout;
	while ((option = getopt(argc, argv, opts)) != -1) {
		switch (option) {
			case 'f':
				strcpy(filename, optarg);
				file = true;
				break;
			case 't':
				if (optarg != NULL) 
					tab = atoi(optarg);
				break;
			case 'm':
				if (optarg != NULL)
					mark = optarg[0];
				break;
			case 'r':
				if (optarg != NULL)
					red_line = atoi(optarg);
				break;
			case 'h':
				help = true;
				break;
			case 'v':
				version = true;
				break;
			case 'o':
				strcpy(output_name, optarg);
				output_file = true;
				break;
			default:
				fwprintf(stderr, L"Указан неправильный ключ\n");
				return -1;
		}
	}
	struct winsize ws;
	if (output_file) {
		output = fopen(output_name, "w");
	}
	if (output == NULL) {
		fwprintf(stderr, L"Ошибка открытия файла для записи\n");
	}
	if (output == stdout) {
		if ((ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)) != -1) 
			width = ws.ws_col;
	else 
		fwprintf(stderr, L"Ошибка при получении размеров окна\n"); 
	}
	else {
		width = 100;
	}
	if (version) {
		fwprintf(stdout, L"Версия программы - 2.4\n");
	}

	if (help) {
		Text_viewer t;
		t.read_file("help.txt", elements);
		t.print_file(elements, stdout);
		for (auto it = elements.begin(); it != elements.end(); ++it) {
			delete *it;
		}
		elements.clear();
		return 0;
	}
	
	if (file) {
		Text_viewer viewer;
		viewer.read_file(filename, elements);
		viewer.print_file(elements, output);
		for (auto it = elements.begin(); it != elements.end(); ++it) {
			delete *it;
		}
		elements.clear();
	}
	else {
		fwprintf(stderr, L"Вы не ввели название файла");
		return 1;
	}
	fwprintf(stderr, L"Всего напечатано %ld символов и %ld слов(а)\n", symbols, words);
	return 0;
}
