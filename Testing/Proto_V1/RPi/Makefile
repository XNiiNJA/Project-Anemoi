CC=g++
CFLAGS=-c -Wall 
LDFLAGS=
SOURCES=Receiver.cpp RPI_Connection_Manager.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=REC_TEST

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
