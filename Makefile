###############################################################################
##  Dictionary code
## 
##  Short istructions
##  make - create executable
##  make clean - clean objects and program
###############################################################################

TARGET = spellcheck
LIBS =
LINK = -pg
CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -Werror -O2
#CFLAGS += -DHASH_DICTIONARY
#CFLAGS += -pg

.PHONY: default all clean help

default: $(TARGET)
all: default

help:
	@echo "make - to compile spellcheck"
	@echo "make clean - to remove objs and bin"

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) $(LINK) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
