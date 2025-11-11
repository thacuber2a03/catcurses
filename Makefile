NAME := catcurses
UMKA ?= umbox/umka/umka

CFLAGS := \
	-Wall        \
	-Wextra      \
	-Werror      \
	-pedantic    \
	-std=c11     \
	-Iumbox/umka \
	-fPIC        \
	-g

LDFLAGS := -lncurses

TARGETS=$(NAME)_linux.umi

.PHONY: all build run docs clean distclean

all: build doc/$(NAME).md

%_linux.umi: %.c Makefile
	$(CC) -shared $(CFLAGS) $< -o $@ $(LDFLAGS)

build: $(TARGETS)

run: build doc/$(NAME).md
	$(UMKA) examples/hello.um

mmdoc:
	mkdir -p doc
	git clone https://github.com/marekmaskarinec/mmdoc

doc/$(NAME).md: mmdoc $(NAME).um
	$(UMKA) mmdoc/mmdoc.um -l go -u "../$(NAME).um#L%d" $(NAME).um > doc/$(NAME).md

docs: doc/$(NAME).md

clean:
	rm $(TARGETS) -rf

distclean: clean
	rm doc mmdoc -rf
