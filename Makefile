# General
CC = g++

# Names and flags
INC_DIR = .
CSOURCES = $(wildcard *.c pprzlink/*.c )
CPPSOURCES += $(wildcard *.cpp)

OBJECTS = $(CSOURCES:.c=.o) $(CPPSOURCES:.cpp=.o)
EXECUTABLE = pprzlink-integration
CFLAGS = -c -Wall -g -I$(INC_DIR) -g -DSTANDALONE
LIBS =




# Targets
all: $(CSOURCES) $(CPPSOURCES) messages.h $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -pthread $(OBJECTS) -o $@ $(LIBS)

messages.h: messages.xml
	$(Q) ./pprzlink/generator/gen_messages.py -o $@ $<  telemetry

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) -std=c++11 $(INCLUDES) $< -o $@

clean:
	@echo $OBJECTS = $(OBJECTS)
	rm -rf $(OBJECTS) $(EXECUTABLE)
