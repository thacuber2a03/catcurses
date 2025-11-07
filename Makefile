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

mmdoc:
	git clone https://git.sr.ht/~mrms/mmdoc

docs: mmdoc
	$(UMKA) mmdoc/mmdoc.um -l umka -u "../$(NAME).um#L%d" $(NAME).um > doc/$(NAME).md

clean:
	rm $(TARGETS) mmdoc -rf
