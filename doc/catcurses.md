## [fn (^Terminal) makeCurrent](../catcurses.um#L31)

```umka
fn (t: ^Terminal) makeCurrent*(): bool
```

Makes `t` the "current" terminal; that is, the terminal whose associated window is shown on-screen.
Reports whether `t` was successfully set as the current terminal.


## [fn curTerminal](../catcurses.um#L42)

```umka
fn curTerminal*(): ^Terminal
```

Returns the current terminal.
See `fn (^Terminal) makeCurrent`.


## [fn sessionExists](../catcurses.um#L50)

```umka
fn sessionExists*(): bool
```

Reports whether a terminal is set.
(In most cases, this serves to check whether ncurses is initialized.)


## [fn stdTerminal](../catcurses.um#L59)

```umka
fn stdTerminal*(): ^Terminal
```

Returns the standard terminal.
If ncurses hasn't been initialized yet, this function will initialize it.


## [fn mkTerminal](../catcurses.um#L87)

```umka
fn mkTerminal*(termType: str = "", outf: std::File = null, inf: std::File = null): (^Terminal, bool)
```

Creates a new terminal.
If `termType` isn't empty, the created terminal is created with said type.
If `outf` and `inf` are null, they default to `std::stdout()` and `std::stdin()`, respectively.

If no terminal existed previously, initializes ncurses and returns a stub representing the standard terminal.
`termType`, `outf` and `inf` are ignored in this case.


## [fn (^Terminal) destroy](../catcurses.um#L111)

```umka
fn (t: ^Terminal) destroy*()
```

Destroys the underlying terminal. `t` is no longer valid for use after calling this method.
If `t` is the standard terminal, this method will finalize ncurses.


## [fn (^Terminal) raw](../catcurses.um#L135)

```umka
fn (t: ^Terminal) raw*(b: bool): bool
```

Enables/disables raw mode for the terminal.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the terminal was set/unset to raw mode.


## [fn (^Terminal) cbreak](../catcurses.um#L151)

```umka
fn (t: ^Terminal) cbreak*(b: bool): bool
```

Enables/disables cbreak mode (disables line buffering but still interprets signals) for the terminal.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the terminal was set/unset to cbreak mode.


## [fn (^Terminal) echo](../catcurses.um#L167)

```umka
fn (t: ^Terminal) echo*(b: bool): bool
```

Enables/disables character echoing after each keypress.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the echo was able to be set/unset.


## [fn (^Terminal) halfDelay](../catcurses.um#L183)

```umka
fn (t: ^Terminal) halfDelay*(n: int): bool
```

Sets the terminal's half-delay, in tenths of a second.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the half delay was able to be changed.


## [type Visibility](../catcurses.um#L195)

```umka
type Visibility* = enum { hidden; visible; veryVisible }
```

Cursor visibility specifiers. See `fn (^Terminal) cursorVisibility`.


## [fn (^Visibility) string](../catcurses.um#L201)

```umka
fn (v: ^Visibility) string*(): str
```

Returns the string representation of this visibility specifier.
For debugging purposes.


## [fn (^Terminal) cursorVisibility](../catcurses.um#L218)

```umka
fn (t: ^Terminal) cursorVisibility*(v: Visibility): (Visibility, bool)
```

Changes the cursor visibility to `vis` (see `type Visibility`).
Must only be called on the standard terminal (see `fn stdTerminal`).
Returns the previous visibility and whether it was able to change it at all.


## [type Key](../catcurses.um#L239)

```umka
type Key* = enum {
	escape = 0x1b

	// Function keys (every single one is supported for completion's sake)
	f0 = 1000;
	f1;  f2;  f3;  f4;  f5;  f6;  f7;  f8
	f9;  f10; f11; f12; f13; f14; f15; f16
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


## [fn (^Key) string](../catcurses.um#L261)

```umka
fn (k: ^Key) string*(): str
```

Returns the string representation of this key.
For debugging purposes.


## [fn keyF](../catcurses.um#L294)

```umka
fn keyF*(i: int): Key { return Key(int(Key.f0)+i) }
```

Similar to ncurses' `KEY_F(n)` macro.


## [fn (^Window) keypad](../catcurses.um#L300)

```umka
fn (w: ^Window) keypad*(enable: bool): bool
```

Attempts to enable/disable function key detection in `w`.
Reports whether it was able to do so.


## [fn (^Window) print](../catcurses.um#L310)

```umka
fn (w: ^Window) print*(fmt: str, a: ..any): int
```

Prints a formatted string on the current location of the cursor.
Returns how many characters were written.
The format string follows [fmt.um](https://umbox.tophat2d.dev/package/fmt/browse#:~:text=Syntax)'s syntax.


## [fn (^Window) printAt](../catcurses.um#L320)

```umka
fn (w: ^Window) printAt*(x, y: int, fmt: str, a: ..any): int
```

Moves the cursor to (`x`,`y`) and prints a formatted string there.
Returns how many characters were written.
The format string follows [fmt.um](https://umbox.tophat2d.dev/package/fmt/browse#:~:text=Syntax)'s syntax.


## [fn (^Window) getKey](../catcurses.um#L329)

```umka
fn (w: ^Window) getKey*(): (Key, bool)
```

Returns the current keystroke, if any.
Return values depend on the standard terminal's settings; check `man 3x getch` for information.


## [fn (^Window) refresh](../catcurses.um#L353)

```umka
fn (w: ^Window) refresh*(): bool
```

Redraws this window, if applicable.


## [type Attribute](../catcurses.um#L361)

```umka
type Attribute = enum {
	normal     // Normal display (no highlight)
	standOut   // Best highlighting mode of the terminal
	underline  // Underlining
	reverse    // Reverse video
	blink      // Blinking
	dim        // Half bright
	bold       // Extra bright or bold
	protect    // Protected mode
	invis      // Invisible or blank mode
	altCharset // Alternate character set
	italic     // Italics (non-X/Open extension)
}
```

All character attributes that can be toggled with the window attribute functions.


## [fn (^Attribute) string](../catcurses.um#L381)

```umka
fn (a: ^Attribute) string*(): str
```

Returns the string representation of this attribute.
For debugging purposes.


## [fn (^Window) attrOn](../catcurses.um#L412)

```umka
fn (w: ^Window) attrOn*(attrs: ..Attribute): (bool, Attribute)
```

Enables all the attributes listed in `attrs` (see `type Attribute`).
Reports whether any of the attributes could not be enabled, and which one.


## [fn (^Window) attrOff](../catcurses.um#L418)

```umka
fn (w: ^Window) attrOff*(attrs: ..Attribute): (bool, Attribute)
```

Disables all the attributes listed in `attrs` (see `type Attribute`).
Reports whether any of the attributes could not be disabled, and which one.


## [fn (^Window) attrListOn](../catcurses.um#L423)

```umka
fn (w: ^Window) attrListOn*(attrs: []Attribute): (bool, Attribute)
```

Alternate version of `fn (^Window) attrOn` that explicitly takes a list.


## [fn (^Window) attrListOff](../catcurses.um#L428)

```umka
fn (w: ^Window) attrListOff*(attrs: []Attribute): (bool, Attribute)
```

Alternate version of `fn (^Window) attrOff` that explicitly takes a list.


## [fn (^Window) withAttrs](../catcurses.um#L433)

```umka
fn (w: ^Window) withAttrs*(attrs: []Attribute, f: fn(w: ^Window))
```

Runs `f` with all attributes in `attrs` enabled, and disables them when the function ends.


## [fn (^Window) getAttributes](../catcurses.um#L458)

```umka
fn (w: ^Window) getAttributes*(): (^map[Attribute]bool, RawColorPair)
```

Returns the set of attributes and color pair currently applied to this window.
Returns `null` for the attributes map on error.

> [!NOTE]
> Currently the color pair is just passed through (which is why it's raw);
> I've yet to abstract it out somehow, which is also why there aren't any colors yet.
>
> You can ignore it for now, or pass it straight to `fn (^Window) setAttributes`.


## [fn (^Window) setAttributes](../catcurses.um#L466)

```umka
fn (w: ^Window) setAttributes*(attrs: map[Attribute]bool, pair: RawColorPair): bool
```

Overwrites the set attributes and color pair currently set for this window.

> [!NOTE]
> See `fn (^Window) getAttributes`.


## [fn (^Window) clear](../catcurses.um#L500)

```umka
fn (w: ^Window) clear*(): bool
```

Clears this window.


## [fn (^Window) erase](../catcurses.um#L508)

```umka
fn (w: ^Window) erase*(): bool
```

Fills this window with blank characters.


## [fn (^Window) getSize](../catcurses.um#L516)

```umka
fn (w: ^Window) getSize*(): (int, int)
```

Returns the x and y sizes of the window, in that order.


