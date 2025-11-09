// TODO(thacuber2a03): order this file better

#include <ncurses.h>

#include <umka_api.h>

#define RET_CHECK_ERR(e) (umkaGetResult(params, result)->uintVal = (e) != ERR)

#define GET_WINDOW(p, i) ((WINDOW *)umkaGetParam(p, i)->ptrVal)
#define GET_SCREEN(p, i) ((SCREEN *)umkaGetParam(p, i)->ptrVal)
#define RET_WINDOW(p, r) (*(WINDOW **)umkaGetResult(p, r))
#define RET_SCREEN(p, r) (*(SCREEN **)umkaGetResult(p, r))

#define ARRLEN(a) (sizeof(a) / sizeof((a)[0]))
#define UNUSED(x) ((void)(x))

#define API_FN(name) UMKA_EXPORT void umc__##name(UmkaStackSlot *params, UmkaStackSlot *result)

// fn umc__initscr(): RawWindow
API_FN(initscr) { RET_WINDOW(params, result) = initscr(); }

// fn umc__curscr(): RawWindow
API_FN(curscr) { RET_WINDOW(params, result) = curscr; }

// fn umc__endwin(): bool
API_FN(endwin) { RET_CHECK_ERR(endwin()); }

// fn umc__wprintw(win: RawWindow, s: str): int
API_FN(wprintw)
{
    WINDOW *w     = GET_WINDOW(params, 0);
    const char *s = umkaGetParam(params, 1)->ptrVal;

    umkaGetResult(params, result)->intVal = wprintw(w, "%s", s);
}

// fn umc__mvwprintw(win: RawWindow, s: str): int
API_FN(mvwprintw)
{
    WINDOW *w     = GET_WINDOW(params, 0);
    int x         = umkaGetParam(params, 1)->intVal;
    int y         = umkaGetParam(params, 2)->intVal;
    const char *s = umkaGetParam(params, 3)->ptrVal;

    umkaGetResult(params, result)->intVal = mvwprintw(w, y, x, "%s", s);
}

// fn umc__wrefresh(win: RawWindow): bool
API_FN(wrefresh) { RET_CHECK_ERR(wrefresh(GET_WINDOW(params, 0))); }

// fn umc__wgetch(win: RawWindow, c: ^char): bool
API_FN(wgetch)
{
    WINDOW *w = GET_WINDOW(params, 0);
    int *c    = umkaGetParam(params, 1)->ptrVal;
    RET_CHECK_ERR(*c = wgetch(w));
}

// fn umc__raw(): bool
API_FN(raw) { RET_CHECK_ERR(raw()); }

// fn umc__noraw(): bool
API_FN(noraw) { RET_CHECK_ERR(noraw()); }

// fn umc__cbreak(): bool
API_FN(cbreak) { RET_CHECK_ERR(cbreak()); }

// fn umc__nocbreak(): bool
API_FN(nocbreak) { RET_CHECK_ERR(nocbreak()); }

// fn umc__echo(): bool
API_FN(echo) { RET_CHECK_ERR(echo()); }

// fn umc__noecho(): bool
API_FN(noecho) { RET_CHECK_ERR(noecho()); }

// fn umc__keypad(win: RawWindow, enable: bool): bool
API_FN(keypad)
{
    WINDOW *w   = GET_WINDOW(params, 0);
    bool enable = umkaGetParam(params, 1)->uintVal;
    RET_CHECK_ERR(keypad(w, enable));
}

// fn umc__halfdelay(win: RawWindow, enable: bool): bool
API_FN(halfdelay) { RET_CHECK_ERR(halfdelay(umkaGetParam(params, 0)->intVal)); }

// fn umc__set_term(term: RawTerminal): RawTerminal
API_FN(set_term)
{
    SCREEN *new = GET_SCREEN(params, 0);

    RET_SCREEN(params, result) = set_term(new);
}

// fn umc__newterm(termType: str, outf, inf: std::File): RawTerminal
API_FN(newterm)
{
    char *type = umkaGetParam(params, 0)->ptrVal;
    FILE *outf = umkaGetParam(params, 1)->ptrVal;
    FILE *inf  = umkaGetParam(params, 2)->ptrVal;

    RET_SCREEN(params, result) = newterm(type, outf, inf);
}

// fn umc__delscreen(term: RawTerminal)
API_FN(delscreen)
{
    UNUSED(result);
    delscreen(GET_SCREEN(params, 0));
}

// fn umc__fn_key_offset(key: int): int
API_FN(fn_key_offset)
{
    int key = umkaGetParam(params, 0)->intVal;

    umkaGetResult(params, result)->intVal = key - KEY_F0;
}

// must match the order of all keys listed
// after the one otherKeyStart refers to in catcurses.um
static int keys[] = {
    KEY_DOWN,
    KEY_UP,
    KEY_LEFT,
    KEY_RIGHT,
};

static int keysLen = ARRLEN(keys);

// fn umc__other_key_offset(key: int): int
API_FN(other_key_offset)
{
    int key = umkaGetParam(params, 0)->intVal;

    int res = -1;
    for (int i = 0; i < keysLen; i++)
        if (key == keys[i]) res = i;

    umkaGetResult(params, result)->intVal = res;
}

// must match `type Attribute` enum in catcurses.um
static const attr_t attributes[] = {
    A_NORMAL,  A_STANDOUT, A_UNDERLINE,  A_REVERSE, A_BLINK,    A_DIM,   A_BOLD,
    A_PROTECT, A_INVIS,    A_ALTCHARSET, A_ITALIC,  A_CHARTEXT, A_COLOR,
};

// fn umc__to_raw_attr(a: Attribute): RawAttribute
API_FN(to_raw_attr)
{
    int attr = umkaGetParam(params, 0)->intVal;

    umkaGetResult(params, result)->intVal = attributes[attr];
}

// fn umc__to_raw_color_pair(a: Attribute): RawAttribute
API_FN(to_raw_color_pair)
{
    int colorPair = umkaGetParam(params, 0)->intVal;

    umkaGetResult(params, result)->intVal = COLOR_PAIR(colorPair);
}

// fn umc__from_raw_attr(a: RawAttribute): Attribute
/*
API_FN(from_raw_attr)
{
    attr_t rawAttr = umkaGetParam(params, 0)->uintVal;

    for (size_t i = 0; i < ARRLEN(attributes); i++)
        if (rawAttr == attributes[i])
        {
            umkaGetResult(params, result)->intVal = i;
            return;
        }

    umkaGetResult(params, result)->intVal = -1;
}
*/

// fn umc__wattr_on(win: RawWindow, a: RawAttribute): bool
API_FN(wattr_on)
{
    WINDOW *win = GET_WINDOW(params, 0);
    attr_t attr = umkaGetParam(params, 1)->intVal;
    RET_CHECK_ERR(wattr_on(win, attr, NULL));
}

// fn umc__wattr_off(win: RawWindow, a: RawAttribute): bool
API_FN(wattr_off)
{
    WINDOW *win = GET_WINDOW(params, 0);
    int attr    = umkaGetParam(params, 1)->intVal;

    RET_CHECK_ERR(wattr_off(win, attr, NULL));
}

// fn umc__clear(win: RawWindow): bool
API_FN(clear) { RET_CHECK_ERR(wclear(GET_WINDOW(params, 0))); }

// fn umc__erase(win: RawWindow): bool
API_FN(erase) { RET_CHECK_ERR(werase(GET_WINDOW(params, 0))); }

// fn umc__getmaxxy(win: RawWindow, x, y: ^int)
API_FN(getmaxxy)
{
    UNUSED(result);
    WINDOW *w = GET_WINDOW(params, 0);
    int *x    = umkaGetParam(params, 1)->ptrVal;
    int *y    = umkaGetParam(params, 2)->ptrVal;
    getmaxyx(w, *y, *x);
}

// fn umc__curs_set(v: Visibility, prev: ^Visibility): bool
API_FN(curs_set)
{
    int *prev = umkaGetParam(params, 1)->ptrVal;
    int v     = umkaGetParam(params, 0)->intVal;
    RET_CHECK_ERR(*prev = curs_set(v));
}

// fn umc__wattr_get(win: RawWindow, attr: ^RawAttribute, pair: ^ColorPairID): bool
API_FN(wattr_get)
{
    WINDOW *w = GET_WINDOW(params, 0);
    attr_t *a = umkaGetParam(params, 1)->ptrVal;
    short *p  = umkaGetParam(params, 2)->ptrVal;
    RET_CHECK_ERR(wattr_get(w, a, p, NULL)); // that one Cardi B song
}

// fn umc__wattr_set(win: RawWindow, attr: RawAttribute, pair: ColorPairID): bool
API_FN(wattr_set)
{
    WINDOW *w = GET_WINDOW(params, 0);
    attr_t a  = umkaGetParam(params, 1)->uintVal;
    short p   = umkaGetParam(params, 2)->intVal;
    RET_CHECK_ERR(wattr_set(w, a, p, NULL)); // again
}

// fn umc__has_colors_sp(term: RawTerminal): bool
API_FN(has_colors_sp) { umkaGetResult(params, result)->intVal = has_colors_sp(GET_SCREEN(params, 0)); }

// fn umc__can_change_color_sp(term: RawTerminal): bool
API_FN(can_change_color_sp) { umkaGetResult(params, result)->intVal = can_change_color_sp(GET_SCREEN(params, 0)); }

// fn umc__start_color_sp(term: RawTerminal): bool
API_FN(start_color_sp) { RET_CHECK_ERR(start_color_sp(GET_SCREEN(params, 0))); }

// fn umc__get_current_max_colors(): int
API_FN(get_current_max_colors) { umkaGetResult(params, result)->intVal = COLORS; }

// must match the enum in catcurses.um
static int standardColors[] = {
    COLOR_BLACK,       //
    COLOR_RED,         //
    COLOR_GREEN,       //
    COLOR_YELLOW,      //
    COLOR_BLUE,        //
    COLOR_MAGENTA,     //
    COLOR_CYAN,        //
    COLOR_WHITE,       //
    COLOR_BLACK + 8,   //
    COLOR_RED + 8,     //
    COLOR_GREEN + 8,   //
    COLOR_YELLOW + 8,  //
    COLOR_BLUE + 8,    //
    COLOR_MAGENTA + 8, //
    COLOR_CYAN + 8,    //
    COLOR_WHITE + 8,   //
};

// fn umc__to_raw_standard_color(c: StandardColor): RawStandardColor
API_FN(to_raw_standard_color)
{
    int stdColor = umkaGetParam(params, 0)->intVal;

    umkaGetResult(params, result)->intVal = standardColors[stdColor];
}

// fn umc__init_pair_sp(term: RawTerminal, pair: ColorPairID, bg, fg: RawStandardColor): bool
API_FN(init_pair_sp)
{
    SCREEN *s = GET_SCREEN(params, 0);
    short p   = umkaGetParam(params, 1)->intVal;
    int bg    = umkaGetParam(params, 2)->intVal;
    int fg    = umkaGetParam(params, 3)->intVal;
    RET_CHECK_ERR(init_pair_sp(s, p, fg, bg));
}

// fn umc__init_color_sp(term: RawTerminal, color, r, g, b: int): bool
API_FN(init_color_sp)
{
    SCREEN *s = GET_SCREEN(params, 0);
    short c   = umkaGetParam(params, 1)->intVal;
    short r   = umkaGetParam(params, 2)->intVal;
    short g   = umkaGetParam(params, 3)->intVal;
    short b   = umkaGetParam(params, 4)->intVal;
    RET_CHECK_ERR(init_color_sp(s, c, r, g, b));
}

