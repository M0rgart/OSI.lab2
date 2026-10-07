CC = gcc
CFLAGS = -Wall -Wextra -Iinc

ifeq ($(OS),Windows_NT)
    TARGET_EXT = .exe
    PLATFORM_SRC = src/platform_win.c
    RM = del
else
    TARGET_EXT = 
    PLATFORM_SRC = src/platform_linux.c
    RM = rm -f
endif

all: parent$(TARGET_EXT) child$(TARGET_EXT)

parent$(TARGET_EXT): src/parent.c src/common.c $(PLATFORM_SRC)
	$(CC) $(CFLAGS) -o $@ $^

child$(TARGET_EXT): src/child.c src/common.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) parent$(TARGET_EXT) child$(TARGET_EXT)