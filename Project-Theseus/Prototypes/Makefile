CC=g++
CFLAGS=-c -Wall 
LDFLAGS= -lwiringPi
SOURCES=GPIO_TESTING.cpp Driver.cpp Receiver.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=REC_TEST

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
