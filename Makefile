CXX = g++
FLAGS = -g -Wall

SOURCES = main.cpp title.cpp list.cpp paragraph.cpp code.cpp text_viewer.cpp abstract.cpp
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(FLAGS) $(OBJECTS) -o $(EXECUTABLE)
	
.cpp.o:
	$(CXX) $(FLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
