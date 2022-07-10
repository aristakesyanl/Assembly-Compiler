CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=AssemblyCompiler.cpp Source.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=AssemblyCompiler

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
