// TODO(thacuber2a03): order this file better

#include <errno.h>
#include <ncurses.h>
#include <string.h>

#include <umka_api.h>

#define ARRLEN(a) (sizeof(a) / sizeof((a)[0]))
#define UNUSED(x) ((void)(x))

#define API_FN(name) UMKA_EXPORT void umc__##name(UmkaStackSlot *params, UmkaStackSlot *result)

#define API_HEADER                          \
    Umka *umka   = umkaGetInstance(result); \
    UmkaAPI *api = umkaGetAPI(umka)

#define RET_CHECK_ERR(e) (api->umkaGetResult(params, result)->uintVal = (e) != ERR)

#define GET_WINDOW(i) ((WINDOW *)api->umkaGetParam(params, i)->ptrVal)
#define GET_SCREEN(i) ((SCREEN *)api->umkaGetParam(params, i)->ptrVal)
#define RET_WINDOW()  (*(WINDOW **)api->umkaGetResult(params, result))
#define RET_SCREEN()  (*(SCREEN **)api->umkaGetResult(params, result))

// fn umc__initscr(): RawWindow
API_FN(initscr)
{
    API_HEADER;
    RET_WINDOW() = initscr();
}

// fn umc__curscr(): RawWindow
API_FN(curscr)
{
    API_HEADER;
    RET_WINDOW() = curscr;
}

// fn umc__endwin(): bool
API_FN(endwin)
{
    API_HEADER;
    RET_CHECK_ERR(endwin());
}

// fn umc__waddnstr(win: RawWindow, s: str, n: int): bool
API_FN(waddnstr)
{
    API_HEADER;
    WINDOW *win = GET_WINDOW(0);
    char *s     = api->umkaGetParam(params, 1)->ptrVal;
    int n       = api->umkaGetParam(params, 2)->intVal;
    RET_CHECK_ERR(waddnstr(win, s, n));
}

// fn umc__wrefresh(win: RawWindow): bool
API_FN(wrefresh)
{
    API_HEADER;
    RET_CHECK_ERR(wrefresh(GET_WINDOW(0)));
}

// fn umc__wgetch(win: RawWindow, c: ^char): bool
API_FN(wgetch)
{
    API_HEADER;
    WINDOW *w = GET_WINDOW(0);
    int *c    = api->umkaGetParam(params, 1)->ptrVal;
    RET_CHECK_ERR(*c = wgetch(w));
}

// fn umc__raw(): bool
API_FN(raw)
{
    API_HEADER;
    RET_CHECK_ERR(raw());
}

// fn umc__noraw(): bool
API_FN(noraw)
{
    API_HEADER;
    RET_CHECK_ERR(noraw());
}

// fn umc__cbreak(): bool
API_FN(cbreak)
{
    API_HEADER;
    RET_CHECK_ERR(cbreak());
}

// fn umc__nocbreak(): bool
API_FN(nocbreak)
{
    API_HEADER;
    RET_CHECK_ERR(nocbreak());
}

// fn umc__echo(): bool
API_FN(echo)
{
    API_HEADER;
    RET_CHECK_ERR(echo());
}

// fn umc__noecho(): bool
API_FN(noecho)
{
    API_HEADER;
    RET_CHECK_ERR(noecho());
}

// fn umc__nl(): bool
API_FN(nl)
{
    API_HEADER;
    RET_CHECK_ERR(nl());
}

// fn umc__nonl(): bool
API_FN(nonl)
{
    API_HEADER;
    RET_CHECK_ERR(nonl());
}

// fn umc__keypad(win: RawWindow, enable: bool): bool
API_FN(keypad)
{
    API_HEADER;
    WINDOW *w   = GET_WINDOW(0);
    bool enable = api->umkaGetParam(params, 1)->intVal;
    RET_CHECK_ERR(keypad(w, enable));
}

// fn umc__wtimeout(win: RawWindow, delay: int)
API_FN(wtimeout)
{
    API_HEADER;
    wtimeout(GET_WINDOW(0), api->umkaGetParam(params, 1)->intVal);
}

// fn umc__set_term(term: RawTerminal): RawTerminal
API_FN(set_term)
{
    API_HEADER;
    SCREEN *new = GET_SCREEN(0);

    RET_SCREEN() = set_term(new);
}

// fn umc__newterm(termType: str, outf, inf: std::File): RawTerminal
API_FN(newterm)
{
    API_HEADER;
    char *type = api->umkaGetParam(params, 0)->ptrVal;
    FILE *outf = api->umkaGetParam(params, 1)->ptrVal;
    FILE *inf  = api->umkaGetParam(params, 2)->ptrVal;

    RET_SCREEN() = newterm(type, outf, inf);
}

// fn umc__delscreen(term: RawTerminal)
API_FN(delscreen)
{
    API_HEADER;
    delscreen(GET_SCREEN(0));
}

// fn umc__fnKeyOffset(key: int): int
API_FN(fnKeyOffset)
{
    API_HEADER;
    int key = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->intVal = key - KEY_F0;
}

// must match the order of all keys listed
// after the one otherKeyStart refers to in catcurses.um
static int keys[] = {
    KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_PPAGE, KEY_NPAGE, KEY_HOME, KEY_END, KEY_IC, KEY_DC, KEY_BACKSPACE,
};

static int keysLen = ARRLEN(keys);

// fn umc__otherKeyOffset(key: int): int
API_FN(otherKeyOffset)
{
    API_HEADER;
    int key = api->umkaGetParam(params, 0)->intVal;

    int res = -1;
    for (int i = 0; i < keysLen; i++)
        if (key == keys[i]) res = i;

    api->umkaGetResult(params, result)->intVal = res;
}

// must match `type Attribute` enum in catcurses.um
static const attr_t attributes[] = {
    A_NORMAL,  A_STANDOUT, A_UNDERLINE,  A_REVERSE, A_BLINK,    A_DIM,   A_BOLD,
    A_PROTECT, A_INVIS,    A_ALTCHARSET, A_ITALIC,  A_CHARTEXT, A_COLOR,
};

// fn umc__toRawAttr(a: Attribute): RawAttribute
API_FN(toRawAttr)
{
    API_HEADER;
    int attr = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->intVal = attributes[attr];
}

// fn umc__toRawColorPair(a: ColorPairID): RawColorPair
API_FN(toRawColorPair)
{
    API_HEADER;
    int colorPair = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->intVal = COLOR_PAIR(colorPair);
}

// fn umc__wattr_on(win: RawWindow, a: RawAttribute): bool
API_FN(wattr_on)
{
    API_HEADER;
    WINDOW *win = GET_WINDOW(0);
    attr_t attr = api->umkaGetParam(params, 1)->intVal;
    RET_CHECK_ERR(wattr_on(win, attr, NULL));
}

// fn umc__wattr_off(win: RawWindow, a: RawAttribute): bool
API_FN(wattr_off)
{
    API_HEADER;
    WINDOW *win = GET_WINDOW(0);
    int attr    = api->umkaGetParam(params, 1)->intVal;

    RET_CHECK_ERR(wattr_off(win, attr, NULL));
}

// fn umc__clear(win: RawWindow): bool
API_FN(clear)
{
    API_HEADER;
    RET_CHECK_ERR(wclear(GET_WINDOW(0)));
}

// fn umc__erase(win: RawWindow): bool
API_FN(erase)
{
    API_HEADER;
    RET_CHECK_ERR(werase(GET_WINDOW(0)));
}

// fn umc__getmaxxy(win: RawWindow, x, y: ^int)
API_FN(getmaxxy)
{
    API_HEADER;
    WINDOW *w = GET_WINDOW(0);
    int *x    = api->umkaGetParam(params, 1)->ptrVal;
    int *y    = api->umkaGetParam(params, 2)->ptrVal;
    getmaxyx(w, *y, *x);
}

// fn umc__getxy(win: RawWindow, x, y: ^int)
API_FN(getxy)
{
    API_HEADER;
    WINDOW *w = GET_WINDOW(0);
    int *x    = api->umkaGetParam(params, 1)->ptrVal;
    int *y    = api->umkaGetParam(params, 2)->ptrVal;
    getyx(w, *y, *x);
}

// fn umc__wmove(win: RawWindow, x, y: int): bool
API_FN(wmove)
{
    API_HEADER;
    WINDOW *w = GET_WINDOW(0);
    int x     = api->umkaGetParam(params, 1)->intVal;
    int y     = api->umkaGetParam(params, 2)->intVal;
    RET_CHECK_ERR(wmove(w, y, x));
}

// fn umc__curs_set(v: Visibility, prev: ^Visibility): bool
API_FN(curs_set)
{
    API_HEADER;
    int v     = api->umkaGetParam(params, 0)->intVal;
    int *prev = api->umkaGetParam(params, 1)->ptrVal;
    RET_CHECK_ERR(*prev = curs_set(v));
}

// fn umc__wattr_get(win: RawWindow, attr: ^RawAttribute, pair: ^ColorPairID): bool
API_FN(wattr_get)
{
    API_HEADER;
    WINDOW *w = GET_WINDOW(0);
    attr_t *a = api->umkaGetParam(params, 1)->ptrVal;
    short *p  = api->umkaGetParam(params, 2)->ptrVal;
    RET_CHECK_ERR(wattr_get(w, a, p, NULL)); // that one Cardi B song
}

// fn umc__wattr_set(win: RawWindow, attr: RawAttribute, pair: ColorPairID): bool
API_FN(wattr_set)
{
    API_HEADER;
    WINDOW *w = GET_WINDOW(0);
    attr_t a  = api->umkaGetParam(params, 1)->uintVal;
    short p   = api->umkaGetParam(params, 2)->intVal;
    RET_CHECK_ERR(wattr_set(w, a, p, NULL)); // again
}

// fn umc__has_colors_sp(term: RawTerminal): bool
API_FN(has_colors_sp)
{
    API_HEADER;
    api->umkaGetResult(params, result)->intVal = has_colors_sp(GET_SCREEN(0));
}

// fn umc__can_change_color_sp(term: RawTerminal): bool
API_FN(can_change_color_sp)
{
    API_HEADER;
    api->umkaGetResult(params, result)->intVal = can_change_color_sp(GET_SCREEN(0));
}

// fn umc__start_color_sp(term: RawTerminal): bool
API_FN(start_color_sp)
{
    API_HEADER;
    RET_CHECK_ERR(start_color_sp(GET_SCREEN(0)));
}

// fn umc__getCurrentMaxColors(): int
API_FN(getCurrentMaxColors)
{
    API_HEADER;
    api->umkaGetResult(params, result)->intVal = COLORS;
}

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

// fn umc__toRawStandardColor(c: StandardColor): RawStandardColor
API_FN(toRawStandardColor)
{
    API_HEADER;
    int stdColor = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->intVal = standardColors[stdColor];
}

// fn umc__init_pair_sp(term: RawTerminal, pair: ColorPairID, bg, fg: RawStandardColor): bool
API_FN(init_pair_sp)
{
    API_HEADER;
    SCREEN *s = GET_SCREEN(0);
    short p   = api->umkaGetParam(params, 1)->intVal;
    int bg    = api->umkaGetParam(params, 2)->intVal;
    int fg    = api->umkaGetParam(params, 3)->intVal;
    RET_CHECK_ERR(init_pair_sp(s, p, fg, bg));
}

// fn umc__init_color_sp(term: RawTerminal, color, r, g, b: int): bool
API_FN(init_color_sp)
{
    API_HEADER;
    SCREEN *s = GET_SCREEN(0);
    short c   = api->umkaGetParam(params, 1)->intVal;
    short r   = api->umkaGetParam(params, 2)->intVal;
    short g   = api->umkaGetParam(params, 3)->intVal;
    short b   = api->umkaGetParam(params, 4)->intVal;
    RET_CHECK_ERR(init_color_sp(s, c, r, g, b));
}

// fn umc__errno(): int
API_FN(errno)
{
    API_HEADER;
    api->umkaGetResult(params, result)->intVal = errno;
}

// fn umc__strerror(errno: int): str
API_FN(strerror)
{
    API_HEADER;
    api->umkaGetResult(params, result)->ptrVal = strerror(api->umkaGetParam(params, 0)->intVal);
}

// fn umc__errnoIsAgain(): bool
API_FN(errnoIsAgain)
{
    API_HEADER;
    api->umkaGetResult(params, result)->intVal = (errno == EAGAIN);
}

// fn umc__nodelay(win: RawWindow, bf: bool): bool
API_FN(nodelay)
{
    API_HEADER;
    WINDOW *win = GET_WINDOW(0);
    bool bf     = api->umkaGetParam(params, 1)->intVal;
    RET_CHECK_ERR(nodelay(win, bf));
}

// fn umc__wclrtoeol(win: RawWindow): bool
API_FN(wclrtoeol)
{
    API_HEADER;
    WINDOW *win = GET_WINDOW(0);
    RET_CHECK_ERR(wclrtoeol(win));
}
