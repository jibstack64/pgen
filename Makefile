
VERSION = v1-1

CC = gcc
CFLAGS = -Wall -DPLATFORM_DESKTOP

LDFLAGS = -lm

SRC = main.c
TARGET = pgen

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@-$(VERSION) $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
