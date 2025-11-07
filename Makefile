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

run: build
	$(UMKA) examples/hello.um

mmdoc:
	git clone https://git.sr.ht/~mrms/mmdoc

docs: mmdoc
	# -u "../$(NAME).um#L%d"
	$(UMKA) mmdoc/mmdoc.um -l umka $(NAME).um > doc/$(NAME).md

clean:
	rm $(TARGETS) mmdoc -rf

distclean: clean
	rm doc/ -rf
