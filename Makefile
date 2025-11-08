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

.PHONY: all build run docs clean distclean

all: build docs

%_linux.umi: %.c Makefile
	$(CC) $(CFLAGS) -shared $(LDFLAGS) $< -o $@

build: $(TARGETS)

run: build docs
	$(UMKA) examples/hello.um

docs:
	mkdir -p doc
	$(UMKA) mmdoc/mmdoc.um -l umka -u "../$(NAME).um#L%d" $(NAME).um > doc/$(NAME).md

clean:
	rm $(TARGETS) -rf

distclean: clean
	rm doc/ -rf
