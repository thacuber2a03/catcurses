NAME := catcurses
UMKA?=umbox/umka/umka

CFLAGS := \
	-Wall        \
	-Wextra      \
	-pedantic    \
	-std=c11     \
	-Iumbox/umka \
	-fPIC        \
	-g

LDFLAGS := -lncurses

TARGETS=$(NAME)_linux.umi

.PHONY: all build docs clean run

all: build

%_linux.umi: %.c Makefile
	$(CC) $(CFLAGS) -shared $(LDFLAGS) $< -o $@

build: $(TARGETS)

run: build
	$(UMKA) main.um

docs: mmdoc


clean:
	rm $(TARGETS) -rf
