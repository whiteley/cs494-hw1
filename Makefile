CC=gcc
CFLAGS=-Wall -Werror -g
SOURCES=ethersend.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ethersend

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
