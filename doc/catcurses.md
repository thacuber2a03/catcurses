## [fn (^Terminal) makeCurrent](../catcurses.um#L26)

```umka
fn (t: ^Terminal) makeCurrent*(): bool
```

Makes `t` the "current" terminal; that is, the terminal whose associated window is shown on-screen.
Reports whether `t` was successfully set as the current terminal.


## [fn curTerminal](../catcurses.um#L37)

```umka
fn curTerminal*(): ^Terminal
```

Returns the current terminal.
See `fn (^Terminal) makeCurrent`.


## [fn sessionExists](../catcurses.um#L45)

```umka
fn sessionExists*(): bool
```

Reports whether a terminal is set.
(In most cases, this serves to check whether ncurses is initialized.)


## [fn stdTerminal](../catcurses.um#L54)

```umka
fn stdTerminal*(): ^Terminal
```

Returns the standard terminal.
If ncurses hasn't been initialized yet, this function will initialize it.


## [fn mkTerminal](../catcurses.um#L82)

```umka
fn mkTerminal*(termType: str = "", outf: std::File = null, inf: std::File = null): (^Terminal, bool)
```

Creates a new terminal.
If `termType` isn't empty, the created terminal is created with said type.
If `outf` and `inf` are null, they default to `std::stdout()` and `std::stdin()`, respectively.

If no terminal existed previously, initializes ncurses and returns a stub representing the standard terminal.
`termType`, `outf` and `inf` are ignored in this case.


## [fn (^Terminal) destroy](../catcurses.um#L110)

```umka
fn (t: ^Terminal) destroy*()
```

Destroys the underlying terminal. `t` is no longer valid for use after calling this method.
If `t` is the standard terminal, this method will finalize ncurses.


## [fn (^Terminal) raw](../catcurses.um#L130)

```umka
fn (t: ^Terminal) raw*(b: bool)
```

Sets the terminal to raw mode.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the terminal was set/unset to raw mode.


## [fn (^Terminal) cbreak](../catcurses.um#L145)

```umka
fn (t: ^Terminal) cbreak*(b: bool)
```

Sets the terminal to cbreak mode (disables line buffering but still interprets signals).
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the terminal was set/unset to cbreak mode.


## [fn (^Terminal) echo](../catcurses.um#L160)

```umka
fn (t: ^Terminal) echo*(b: bool)
```

Enables/disables character echoing after each keypress.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the echo was able to be set/unset.


## [fn (^Terminal) setHalfDelay](../catcurses.um#L175)

```umka
fn (t: ^Terminal) setHalfDelay*(n: int)
```

Sets the terminal's half delay, in tenths of a second.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the half delay was able to be changed.


## [type Key](../catcurses.um#L198)

```umka
type Key* = enum {
	escape = 0x1b

	// Function keys (every single one is supported for completion)
	f0 = 1000;
	f1;  f2;  f3;  f4;  f5;  f6;  f7;  f8
	f9 ; f10; f11; f12; f13; f14; f15; f16
	f17; f18; f19; f20; f21; f22; f23; f24
	f25; f26; f27; f28; f29; f30; f31; f32
	f33; f34; f35; f36; f37; f38; f39; f40
	f41; f42; f43; f44; f45; f46; f47; f48
	f49; f50; f51; f52; f53; f54; f55; f56
	f57; f58; f59; f60; f61; f62; f63; f64

	// Arrow keys
	down; up; left; right
}
```

The enum wrapping the value returned by getChar.
If getChar returns an ASCII keypress, the value wrapped under this enum will be that key.

> [!WARNING]
> The function keys (F0..F64 and arrow keys, amongst others)
> *do not match* the values used by ncurses, and don't currently intend to do so.


## [fn keyF](../catcurses.um#L219)

```umka
fn keyF*(i: int): Key { return Key(int(Key.f0)+i) }
```

Similar to ncurses' `KEY_F(n)` macro.


## [fn (^Window) keypad](../catcurses.um#L225)

```umka
fn (w: ^Window) keypad*(enable: bool): bool
```

Attempts to enable/disable function key detection in `w`.
Reports whether it was able to do so.


## [fn (^Window) print](../catcurses.um#L235)

```umka
fn (w: ^Window) print*(fmt: str, a: ..any): int
```

Prints a formatted string on the current location of the cursor.
Returns how many characters were written.
The format string follows [fmt.um](https://umbox.tophat2d.dev/package/fmt/browse#:~:text=Syntax)'s syntax.


## [fn (^Window) printAt](../catcurses.um#L244)

```umka
fn (w: ^Window) printAt*(x, y: int, fmt: str, a: ..any): int
```

Moves the cursor to (`x`,`y`) and prints a formatted string there.
Returns how many characters were written.
The format string follows [fmt.um](https://umbox.tophat2d.dev/package/fmt/browse#:~:text=Syntax)'s syntax.


## [fn (^Window) getKey](../catcurses.um#L252)

```umka
fn (w: ^Window) getKey*(): (Key, bool)
```

Returns the current keystroke, if any.
Return values depend on the standard terminal's settings; check `man 3x getch` for information.


## [fn (^Window) refresh](../catcurses.um#L276)

```umka
fn (w: ^Window) refresh*(): bool { return umc__wrefresh(w._) }
```

Redraws this window, if applicable.


## [type Attribute](../catcurses.um#L281)

```umka
type Attribute = enum {
	normal     // Normal display (no highlight)
	standout   // Best highlighting mode of the terminal
	underline  // Underlining
	reverse    // Reverse video
	blink      // Blinking
	dim        // Half bright
	bold       // Extra bright or bold
	protect    // Protected mode
	invis      // Invisible or blank mode
	altcharset // Alternate character set
	italic     // Italics (non-X/Open extension)
}
```

All character attributes that can be toggled with `fn (^Window) attrOn`/`attrOff`.


## [fn (^Window) attrOn](../catcurses.um#L298)

```umka
fn (w: ^Window) attrOn*(attrs: ..Attribute)
```

Enables all the attributes listed in `attrs` (see `type Attribute`).


## [fn (^Window) attrOff](../catcurses.um#L309)

```umka
fn (w: ^Window) attrOff*(attrs: ..Attribute)
```

Disables all the attributes listed in `attrs` (see `type Attribute`).


## [fn (^Window) clear](../catcurses.um#L320)

```umka
fn (w: ^Window) clear*(): bool
```

Clears this window.


## [fn (^Window) erase](../catcurses.um#L329)

```umka
fn (w: ^Window) erase*(): bool
```

Fills this window with blank characters.


