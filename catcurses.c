// TODO(thacuber2a03): order this file better

#include <ncurses.h>

#include <umka_api.h>

#define RET_CHECK_ERR(e) (api->umkaGetResult(params, result)->uintVal = (e) != ERR)

#define GET_WINDOW(p, i) ((WINDOW *)api->umkaGetParam(p, i)->ptrVal)
#define GET_SCREEN(p, i) ((SCREEN *)api->umkaGetParam(p, i)->ptrVal)
#define RET_WINDOW(p, r) (*(WINDOW **)api->umkaGetResult(p, r))
#define RET_SCREEN(p, r) (*(SCREEN **)api->umkaGetResult(p, r))

#define ARRLEN(a) (sizeof(a) / sizeof((a)[0]))
#define UNUSED(x) ((void)(x))

#define API_FN(name) UMKA_EXPORT void umc__##name(UmkaStackSlot *params, UmkaStackSlot *result)

// fn umc__initscr(): RawWindow
API_FN(initscr)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    RET_WINDOW(params, result) = initscr();
}

// fn umc__curscr(): RawWindow
API_FN(curscr)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    RET_WINDOW(params, result) = curscr;
}

// fn umc__endwin(): bool
API_FN(endwin)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    RET_CHECK_ERR(endwin());
}

// fn umc__wprintw(win: RawWindow, s: str): int
API_FN(wprintw)
{
    Umka *umka    = umkaGetInstance(result);
    UmkaAPI *api  = umkaGetAPI(umka);
    WINDOW *w     = GET_WINDOW(params, 0);
    const char *s = api->umkaGetParam(params, 1)->ptrVal;

    api->umkaGetResult(params, result)->intVal = wprintw(w, "%s", s);
}

// fn umc__mvwprintw(win: RawWindow, s: str): int
API_FN(mvwprintw)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    WINDOW *w     = GET_WINDOW(params, 0);
    int x         = api->umkaGetParam(params, 1)->intVal;
    int y         = api->umkaGetParam(params, 2)->intVal;
    const char *s = api->umkaGetParam(params, 3)->ptrVal;

    api->umkaGetResult(params, result)->intVal = mvwprintw(w, y, x, "%s", s);
}

// fn umc__wrefresh(win: RawWindow): bool
API_FN(wrefresh)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(wrefresh(GET_WINDOW(params, 0)));
}

// fn umc__wgetch(win: RawWindow, c: ^char): bool
API_FN(wgetch)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    WINDOW *w = GET_WINDOW(params, 0);
    int *c    = api->umkaGetParam(params, 1)->ptrVal;
    RET_CHECK_ERR(*c = wgetch(w));
}

// fn umc__raw(): bool
API_FN(raw)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(raw());
}

// fn umc__noraw(): bool
API_FN(noraw)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(noraw());
}

// fn umc__cbreak(): bool
API_FN(cbreak)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(cbreak());
}

// fn umc__nocbreak(): bool
API_FN(nocbreak)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(nocbreak());
}

// fn umc__echo(): bool
API_FN(echo)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(echo());
}

// fn umc__noecho(): bool
API_FN(noecho)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(noecho());
}

// fn umc__keypad(win: RawWindow, enable: bool): bool
API_FN(keypad)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    WINDOW *w   = GET_WINDOW(params, 0);
    bool enable = api->umkaGetParam(params, 1)->uintVal;
    RET_CHECK_ERR(keypad(w, enable));
}

// fn umc__halfdelay(win: RawWindow, enable: bool): bool
API_FN(halfdelay)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(halfdelay(api->umkaGetParam(params, 0)->intVal));
}

// fn umc__set_term(term: RawTerminal): RawTerminal
API_FN(set_term)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    SCREEN *new = GET_SCREEN(params, 0);

    RET_SCREEN(params, result) = set_term(new);
}

// fn umc__newterm(termType: str, outf, inf: std::File): RawTerminal
API_FN(newterm)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    char *type = api->umkaGetParam(params, 0)->ptrVal;
    FILE *outf = api->umkaGetParam(params, 1)->ptrVal;
    FILE *inf  = api->umkaGetParam(params, 2)->ptrVal;

    RET_SCREEN(params, result) = newterm(type, outf, inf);
}

// fn umc__delscreen(term: RawTerminal)
API_FN(delscreen)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    delscreen(GET_SCREEN(params, 0));
}

// fn umc__fn_key_offset(key: int): int
API_FN(fn_key_offset)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    int key      = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->intVal = key - KEY_F0;
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
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    int key      = api->umkaGetParam(params, 0)->intVal;

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

// fn umc__to_raw_attr(a: Attribute): RawAttribute
API_FN(to_raw_attr)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    int attr     = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->intVal = attributes[attr];
}

// fn umc__to_raw_color_pair(a: Attribute): RawAttribute
API_FN(to_raw_color_pair)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    int colorPair = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->intVal = COLOR_PAIR(colorPair);
}

// fn umc__from_raw_attr(a: RawAttribute): Attribute
/*
API_FN(from_raw_attr)
{Umka *umka = umkaGetInstance(result); UmkaAPI *api = umkaGetAPI(umka);
    attr_t rawAttr = api->umkaGetParam(params, 0)->uintVal;

    for (size_t i = 0; i < ARRLEN(attributes); i++)
        if (rawAttr == attributes[i])
        {
            api->umkaGetResult(params, result)->intVal = i;
            return;
        }

    api->umkaGetResult(params, result)->intVal = -1;
}
*/

// fn umc__wattr_on(win: RawWindow, a: RawAttribute): bool
API_FN(wattr_on)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    WINDOW *win = GET_WINDOW(params, 0);
    attr_t attr = api->umkaGetParam(params, 1)->intVal;
    RET_CHECK_ERR(wattr_on(win, attr, NULL));
}

// fn umc__wattr_off(win: RawWindow, a: RawAttribute): bool
API_FN(wattr_off)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    WINDOW *win = GET_WINDOW(params, 0);
    int attr    = api->umkaGetParam(params, 1)->intVal;

    RET_CHECK_ERR(wattr_off(win, attr, NULL));
}

// fn umc__clear(win: RawWindow): bool
API_FN(clear)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(wclear(GET_WINDOW(params, 0)));
}

// fn umc__erase(win: RawWindow): bool
API_FN(erase)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(werase(GET_WINDOW(params, 0)));
}

// fn umc__getmaxxy(win: RawWindow, x, y: ^int)
API_FN(getmaxxy)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    WINDOW *w = GET_WINDOW(params, 0);
    int *x    = api->umkaGetParam(params, 1)->ptrVal;
    int *y    = api->umkaGetParam(params, 2)->ptrVal;
    getmaxyx(w, *y, *x);
}

// fn umc__curs_set(v: Visibility, prev: ^Visibility): bool
API_FN(curs_set)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    int *prev = api->umkaGetParam(params, 1)->ptrVal;
    int v     = api->umkaGetParam(params, 0)->intVal;
    RET_CHECK_ERR(*prev = curs_set(v));
}

// fn umc__wattr_get(win: RawWindow, attr: ^RawAttribute, pair: ^ColorPairID): bool
API_FN(wattr_get)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    WINDOW *w = GET_WINDOW(params, 0);
    attr_t *a = api->umkaGetParam(params, 1)->ptrVal;
    short *p  = api->umkaGetParam(params, 2)->ptrVal;
    RET_CHECK_ERR(wattr_get(w, a, p, NULL)); // that one Cardi B song
}

// fn umc__wattr_set(win: RawWindow, attr: RawAttribute, pair: ColorPairID): bool
API_FN(wattr_set)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    WINDOW *w = GET_WINDOW(params, 0);
    attr_t a  = api->umkaGetParam(params, 1)->uintVal;
    short p   = api->umkaGetParam(params, 2)->intVal;
    RET_CHECK_ERR(wattr_set(w, a, p, NULL)); // again
}

// fn umc__has_colors_sp(term: RawTerminal): bool
API_FN(has_colors_sp)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    api->umkaGetResult(params, result)->intVal = has_colors_sp(GET_SCREEN(params, 0));
}

// fn umc__can_change_color_sp(term: RawTerminal): bool
API_FN(can_change_color_sp)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    api->umkaGetResult(params, result)->intVal = can_change_color_sp(GET_SCREEN(params, 0));
}

// fn umc__start_color_sp(term: RawTerminal): bool
API_FN(start_color_sp)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);
    RET_CHECK_ERR(start_color_sp(GET_SCREEN(params, 0)));
}

// fn umc__get_current_max_colors(): int
API_FN(get_current_max_colors)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

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

// fn umc__to_raw_standard_color(c: StandardColor): RawStandardColor
API_FN(to_raw_standard_color)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    int stdColor = api->umkaGetParam(params, 0)->intVal;

    api->umkaGetResult(params, result)->intVal = standardColors[stdColor];
}

// fn umc__init_pair_sp(term: RawTerminal, pair: ColorPairID, bg, fg: RawStandardColor): bool
API_FN(init_pair_sp)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    SCREEN *s = GET_SCREEN(params, 0);
    short p   = api->umkaGetParam(params, 1)->intVal;
    int bg    = api->umkaGetParam(params, 2)->intVal;
    int fg    = api->umkaGetParam(params, 3)->intVal;
    RET_CHECK_ERR(init_pair_sp(s, p, fg, bg));
}

// fn umc__init_color_sp(term: RawTerminal, color, r, g, b: int): bool
API_FN(init_color_sp)
{
    Umka *umka   = umkaGetInstance(result);
    UmkaAPI *api = umkaGetAPI(umka);

    SCREEN *s = GET_SCREEN(params, 0);
    short c   = api->umkaGetParam(params, 1)->intVal;
    short r   = api->umkaGetParam(params, 2)->intVal;
    short g   = api->umkaGetParam(params, 3)->intVal;
    short b   = api->umkaGetParam(params, 4)->intVal;
    RET_CHECK_ERR(init_color_sp(s, c, r, g, b));
}

