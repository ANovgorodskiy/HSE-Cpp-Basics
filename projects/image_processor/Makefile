CC=g++-10
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=image_processor.cpp Image.cpp Filters.cpp FilterController.cpp ArgumentParser.cpp Filters.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=image_processor

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@