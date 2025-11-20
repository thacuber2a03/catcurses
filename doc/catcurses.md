## [type Window and type Terminal](../catcurses.um#L17)

```go
type (
	Window* = struct { _: RawWindow
		// Reference back to the terminal owning this window.
		terminal: weak ^Terminal
	}

	Terminal* = struct { _: RawTerminal
		// The main window owned by this terminal.
		window: Window

		// The maximum amount of colors supported in this terminal.
		// Set on creation (see `fn mkTerminal`).
		maxColors: int

		// The amount of color pairs currently allocated.
		// Used internally, but can be modified.
		pairAmt: int
	}
)
```

The main abstractions for a terminal and a window in the library.


## [fn sessionExists](../catcurses.um#L60)

```go
fn sessionExists*(): bool
```

Reports whether a terminal is set.
(In most cases, this serves to check whether ncurses is initialized.)


## [fn curTerminal](../catcurses.um#L69)

```go
fn curTerminal*(): ^Terminal
```

Returns the current terminal.
See `fn (^Terminal) makeCurrent`.


## [fn (^Terminal) makeCurrent](../catcurses.um#L77)

```go
fn (t: ^Terminal) makeCurrent*(): bool
```

Makes `t` the "current" terminal; that is, the terminal whose associated window is shown on-screen.
Reports whether `t` was successfully set as the current terminal.


## [fn stdTerminal](../catcurses.um#L85)

```go
fn stdTerminal*(): ^Terminal
```

Returns the standard terminal.
If ncurses hasn't been initialized yet, this function will initialize it.


## [fn mkTerminal](../catcurses.um#L115)

```go
fn mkTerminal*(termType: str = "", outf: std::File = null, inf: std::File = null): (^Terminal, std::Err)
```

Creates a new terminal.
If `termType` isn't empty, the created terminal is created with said type.
If `outf` and `inf` are null, they default to `std::stdout()` and `std::stdin()`, respectively.

If no terminal existed previously, initializes ncurses and returns a stub representing the standard terminal.
`termType`, `outf` and `inf` are ignored in this case.


## [type ColorPairID](../catcurses.um#L141)

```go
type ColorPairID* = int
```

Identifies a specific, preinitialized color pair.


## [fn (^ColorPairID) string](../catcurses.um#L147)

```go
fn (c: ^ColorPairID) string*(): str { return sprintf("<color pair %d>", c^) }
```

Returns the string representation of this color pair identifier.
For debugging purposes.


## [type StandardColor](../catcurses.um#L156)

```go
type StandardColor* = enum {
	black
	red
	green
	yellow
	blue
	magenta
	cyan
	white

	// "Bright" colors
	brightBlack
	brightRed
	brightGreen
	brightYellow
	brightBlue
	brightMagenta
	brightCyan
	brightWhite
}

// Aliases
const (
	gray* = StandardColor.white
	darkGray* = StandardColor.brightBlack
)
```

Indices to the standard terminal colors.


## [fn (^Terminal) supportsColors](../catcurses.um#L188)

```go
fn (t: ^Terminal) supportsColors*(): bool
```

Reports whether this terminal supports colors.


## [fn (^Terminal) canChangeColors](../catcurses.um#L197)

```go
fn (t: ^Terminal) canChangeColors*(): bool
```

Reports whether this terminal can change its colors.


## [fn (^Terminal) enableColors](../catcurses.um#L206)

```go
fn (t: ^Terminal) enableColors*(): bool
```

Enables color capability. Returns whether it was enabled.


## [fn (^Terminal) setColorPair](../catcurses.um#L222)

```go
fn (t: ^Terminal) setColorPair*(pair: ColorPairID, bg, fg: StandardColor): bool
```

Associates `pair` with `bg` and `fg` as background/foreground colors.


## [fn (t: ^Terminal) addColorPair](../catcurses.um#L237)

```go
fn (t: ^Terminal) addColorPair*(bg, fg: StandardColor): ColorPairID
```

Allocates a new color pair, with `bg` and `fg` as background/foreground colors, and returns its ID.


## [fn (^Terminal) setColor](../catcurses.um#L258)

```go
fn (t: ^Terminal) setColor*(color: StandardColor, r, g, b: real): bool
```

Changes the RGB values associated with `color` to `r`, `g` and `b`.
`r`, `g` and `b` should be normalized floats (0-1, they will be clamped if outside that range),
and will be quantized to a 0-1000 integer range for ncurses.

This function will affect every color pair using `color` as a palette entry.

It will do nothing (and return `false`) if:
- the terminal doesn't support colors (see `fn (^Terminal) supportsColors`),
- the colors are unable to be changed (see `fn (^Terminal) canChangeColors`), or
- `color` is outside the range of colors the terminal can support (see `type Terminal`)


## [fn (^Terminal) setColorBytes](../catcurses.um#L280)

```go
fn (t: ^Terminal) setColorBytes*(color: StandardColor, r, g, b: uint8): bool
```

Alternate version of `fn (^Terminal) setColor` that takes in byte values (0-255).


## [fn (^Terminal) setColorHex](../catcurses.um#L289)

```go
fn (t: ^Terminal) setColorHex*(color: StandardColor, rgb: uint32): bool
```

Alternate version of `fn (^Terminal) setColor` that takes in a full hex value (0xRRGGBB).


## [fn (^Terminal) destroy](../catcurses.um#L303)

```go
fn (t: ^Terminal) destroy*()
```

Destroys the underlying terminal. `t` is no longer valid for use after calling this method.
If `t` is the standard terminal, this method will finalize ncurses.


## [fn (^Terminal) raw](../catcurses.um#L327)

```go
fn (t: ^Terminal) raw*(b: bool): bool
```

Enables/disables raw mode for the terminal.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the terminal was set/unset to raw mode.


## [fn (^Terminal) cbreak](../catcurses.um#L343)

```go
fn (t: ^Terminal) cbreak*(b: bool): bool
```

Enables/disables cbreak mode (disables line buffering but still interprets signals) for the terminal.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the terminal was set/unset to cbreak mode.


## [fn (^Terminal) echo](../catcurses.um#L359)

```go
fn (t: ^Terminal) echo*(b: bool): bool
```

Enables/disables character echoing after each keypress.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the echo was able to be set/unset.


## [fn (^Terminal) nl](../catcurses.um#L376)

```go
fn (t: ^Terminal) nl*(b: bool): bool
```

Controls translation of the return key into NL (10, 0x0a, '\n').
This allows for detecting whether the return key was pressed, amongst other benefits (see `man 3x nonl`).
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the echo was able to be set/unset.


## [fn (^Terminal) halfDelay](../catcurses.um#L392)

```go
fn (t: ^Terminal) halfDelay*(n: int): bool
```

Sets the terminal's half-delay, in tenths of a second.
Must only be called on the standard terminal (see `fn stdTerminal`).
Reports whether the half delay was able to be changed.


## [type Visibility](../catcurses.um#L404)

```go
type Visibility* = enum { hidden; visible; veryVisible }
```

Cursor visibility specifiers. See `fn (^Terminal) cursorVisibility`.


## [fn (^Visibility) string](../catcurses.um#L410)

```go
fn (v: ^Visibility) string*(): str
```

Returns the string representation of this visibility specifier.
For debugging purposes.


## [fn (^Terminal) cursorVisibility](../catcurses.um#L427)

```go
fn (t: ^Terminal) cursorVisibility*(v: Visibility): (Visibility, bool)
```

Changes the cursor visibility to `vis` (see `type Visibility`).
Must only be called on the standard terminal (see `fn stdTerminal`).
Returns the previous visibility and whether it was able to change it at all.


## [type Key](../catcurses.um#L448)

```go
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

	// No key was pressed; applicable when
	// `halfDelay` or `noDelay` have been set
	// (technical detail: returned when errno = EAGAIN)
	none
}
```

The enum wrapping the value returned by getKey.
If getKey returns an ASCII keypress, the value wrapped under this enum will be that key.

> [!WARNING]
> The function keys (F0..F64 and arrow keys, amongst others)
> *do not match* the values used by ncurses, and don't currently intend to do so.


## [fn (^Key) string](../catcurses.um#L475)

```go
fn (k: ^Key) string*(): str
```

Returns the string representation of this key.
For debugging purposes.


## [fn keyF](../catcurses.um#L508)

```go
fn keyF*(i: int): Key { return Key(int(Key.f0)+i) }
```

Similar to ncurses' `KEY_F(n)` macro.


## [const ctrlMask](../catcurses.um#L513)

```go
const ctrlMask* = uint8(0x1f)
```

The bitmask pertaining to CTRL+char presses.


## [fn ctrlKey](../catcurses.um#L518)

```go
fn ctrlKey*(c: char): Key { return Key(uint8(c) & ctrlMask) }
```

Returns the CTRL representation of `c`.


## [fn (^Window) keypad](../catcurses.um#L524)

```go
fn (w: ^Window) keypad*(enable: bool): bool
```

Attempts to enable/disable function key detection in `w`.
Reports whether it was able to do so.


## [fn (^Window) print](../catcurses.um#L535)

```go
fn (w: ^Window) print*(fmt: str, a: ..any): int
```

Prints a formatted string on the current location of the cursor.
Returns how many characters were written.

The format string follows [fmt.um](https://umbox.tophat2d.dev/package/fmt/browse#:~:text=Syntax)'s syntax.


## [fn (^Window) printAt](../catcurses.um#L546)

```go
fn (w: ^Window) printAt*(x, y: int, fmt: str, a: ..any): int
```

Moves the cursor to (`x`,`y`) and prints a formatted string there.
Returns how many characters were written.

The format string follows [fmt.um](https://umbox.tophat2d.dev/package/fmt/browse#:~:text=Syntax)'s syntax.


## [fn (^Window) getKey](../catcurses.um#L555)

```go
fn (w: ^Window) getKey*(): (Key, std::Err)
```

Returns the current keystroke, if any.
Return values depend on the standard terminal's settings; check `man 3x getch` for information.


## [fn (^Window) refresh](../catcurses.um#L583)

```go
fn (w: ^Window) refresh*(): bool
```

Redraws this window, if applicable.


## [type Attribute](../catcurses.um#L591)

```go
type Attribute* = enum {
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

Color-independent character attributes.


## [fn (^Attribute) string](../catcurses.um#L611)

```go
fn (a: ^Attribute) string*(): str
```

Returns the string representation of this character attribute.
For debugging purposes.


## [fn (^Window) attrOn](../catcurses.um#L645)

```go
fn (w: ^Window) attrOn*(attrs: ..Attribute): (bool, Attribute)
```

Enables all the attributes listed in `attrs` (see `type Attribute`).
Reports whether any of the attributes could not be enabled, and which one.


## [fn (^Window) attrOff](../catcurses.um#L651)

```go
fn (w: ^Window) attrOff*(attrs: ..Attribute): (bool, Attribute)
```

Disables all the attributes listed in `attrs` (see `type Attribute`).
Reports whether any of the attributes could not be disabled, and which one.


## [fn (^Window) attrListOn](../catcurses.um#L656)

```go
fn (w: ^Window) attrListOn*(attrs: []Attribute): (bool, Attribute)
```

Alternate version of `fn (^Window) attrOn` that explicitly takes a list.


## [fn (^Window) attrListOff](../catcurses.um#L661)

```go
fn (w: ^Window) attrListOff*(attrs: []Attribute): (bool, Attribute)
```

Alternate version of `fn (^Window) attrOff` that explicitly takes a list.


## [fn (^Window) getAttributes](../catcurses.um#L680)

```go
fn (w: ^Window) getAttributes*(): (^map[Attribute]bool, ColorPairID)
```

Returns the set of attributes and color pair currently applied to this window.
Returns `null` for the attributes map on error.


## [fn (^Window) setAttributes](../catcurses.um#L685)

```go
fn (w: ^Window) setAttributes*(attrs: map[Attribute]bool, pair: ColorPairID): bool
```

Overwrites the set attributes and color pair currently set for this window.


## [fn (^Window) useColorPair](../catcurses.um#L713)

```go
fn (w: ^Window) useColorPair*(pair: ColorPairID): bool
```

Sets `pair` as the current color pair for this window's text output.
Reports whether it could not be set.


## [fn (^Window) detachColorPair](../catcurses.um#L719)

```go
fn (w: ^Window) detachColorPair*(pair: ColorPairID): bool
```

Stops using `pair` as the current color pair for this window's text output.
Reports whether it could not be disabled.


## [type WithFn](../catcurses.um#L727)

```go
type WithFn* = fn(win: ^Window)
```

Function type taken as argument by all `fn (^Window) with*` functions.


## [fn (^Window) withAttrs](../catcurses.um#L732)

```go
fn (w: ^Window) withAttrs*(attrs: []Attribute, f: WithFn)
```

Runs `f` with all attributes in `attrs` enabled, and disables them when the function ends.


## [fn (^Window) withColorPair](../catcurses.um#L745)

```go
fn (w: ^Window) withColorPair*(pair: ColorPairID, f: WithFn)
```

Runs `f` with `pair` set as the current color pair, and unsets it when the function ends.


## [fn (^Window) withAttrsAndColorPair](../catcurses.um#L761)

```go
fn (w: ^Window) withAttrsAndColorPair*(attrs: []Attribute, pair: ColorPairID, f: WithFn)
```

Shorthand for:
```go
win.withAttrs(attrs, { win.withColorPair(pair, f) })
```


## [fn (^Window) clear](../catcurses.um#L770)

```go
fn (w: ^Window) clear*(): bool
```

Clears this window.


## [fn (^Window) erase](../catcurses.um#L778)

```go
fn (w: ^Window) erase*(): bool
```

Fills this window with blank characters.


## [fn (^Window) getSize](../catcurses.um#L786)

```go
fn (w: ^Window) getSize*(): (int, int)
```

Returns the x and y sizes of the window, in that order.


