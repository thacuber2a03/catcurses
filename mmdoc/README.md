# mmdoc

`mmdoc` is an universal documentation generator.  It can export to markdown or
html.

## example:

* * *

```
//~~int add
Adds two ints and returns the result.
int add(int a, int b) {
//~~
	return a + b;
}
```

This will produce following result:

## int add

```
int add(int a, int b) {
```

Adds two ints and returns the result.

* * *

## install instructions

The script has no dependencies, so just run it with
[Umka](https://github.com/vtereshkov/umka-lang).

## license

Licensed under the terms in the LICENSE file in the root of the repo.
