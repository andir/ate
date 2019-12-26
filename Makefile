CFLAGS := -Wall -O3 -ggdb $(shell pkg-config --cflags vte-2.91) $(shell pkg-config --cflags gtk+-3.0) $(CONFIG_CFLAGS)
#$(shell pkg-config --cflags glib-2.0)
LDFLAGS := $(shell pkg-config --libs vte-2.91) $(shell pkg-config --libs gtk+-3.0)
#$(shell pkg-config --libs glib-2.0)


all: main.c
	cc $(CFLAGS) $(LDFLAGS) $< -o ate

.PHONY: format
format:
	clang-format -i main.c config.default.h
