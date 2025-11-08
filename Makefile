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

.PHONY: all build run clean distclean

all: build doc/$(NAME).md

%_linux.umi: %.c Makefile
	$(CC) $(CFLAGS) -shared $(LDFLAGS) $< -o $@

build: $(TARGETS)

run: build doc/$(NAME).md
	$(UMKA) examples/hello.um

doc:
	mkdir -p doc
doc/$(NAME).md: doc $(NAME).um
	$(UMKA) mmdoc/mmdoc.um -l umka -u "../$(NAME).um#L%d" $(NAME).um > doc/$(NAME).md

clean:
	rm $(TARGETS) -rf

distclean: clean
	rm doc/ -rf
