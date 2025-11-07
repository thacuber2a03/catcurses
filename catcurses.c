#include <ncurses.h>

#include <umka_api.h>

#define ERR_RET(e) (umkaGetResult(params, result)->uintVal = (e) != ERR)

#define GET_WINDOW(p, i) ((WINDOW *)umkaGetParam(p, i)->ptrVal)
#define GET_SCREEN(p, i) ((SCREEN *)umkaGetParam(p, i)->ptrVal)
#define RET_WINDOW(p, r) (*(WINDOW **)umkaGetResult(p, r))
#define RET_SCREEN(p, r) (*(SCREEN **)umkaGetResult(p, r))

#define ARRLEN(a) (sizeof(a) / sizeof((a)[0]))
#define UNUSED(x) ((void)(x))

// fn umc__initscr(): RawWindow
UMKA_EXPORT void umc__initscr(UmkaStackSlot *params, UmkaStackSlot *result) { RET_WINDOW(params, result) = initscr(); }

// fn umc__curscr(): RawWindow
UMKA_EXPORT void umc__curscr(UmkaStackSlot *params, UmkaStackSlot *result) { RET_WINDOW(params, result) = curscr; }

// fn umc__endwin(): bool
UMKA_EXPORT void umc__endwin(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(endwin()); }

// fn umc__wprintw(win: RawWindow, s: str): int
UMKA_EXPORT void umc__wprintw(UmkaStackSlot *params, UmkaStackSlot *result)
{
    WINDOW *w     = GET_WINDOW(params, 0);
    const char *s = umkaGetParam(params, 1)->ptrVal;

    umkaGetResult(params, result)->intVal = wprintw(w, "%s", s);
}

// fn umc__mvwprintw(win: RawWindow, s: str): int
UMKA_EXPORT void umc__mvwprintw(UmkaStackSlot *params, UmkaStackSlot *result)
{
    WINDOW *w     = GET_WINDOW(params, 0);
    int x         = umkaGetParam(params, 1)->intVal;
    int y         = umkaGetParam(params, 2)->intVal;
    const char *s = umkaGetParam(params, 3)->ptrVal;

    umkaGetResult(params, result)->intVal = mvwprintw(w, y, x, "%s", s);
}

// fn umc__wrefresh(win: RawWindow): bool
UMKA_EXPORT void umc__wrefresh(UmkaStackSlot *params, UmkaStackSlot *result)
{
    ERR_RET(wrefresh(GET_WINDOW(params, 0)));
}

// fn umc__wgetch(): (char, bool)
UMKA_EXPORT void umc__wgetch(UmkaStackSlot *params, UmkaStackSlot *result)
{
    WINDOW *w   = GET_WINDOW(params, 0);
    int c       = wgetch(w);
    uint64_t *r = umkaGetResult(params, result)->ptrVal;
    r[0]        = c;
    r[1]        = c != ERR;
}

// fn umc__raw(): bool
UMKA_EXPORT void umc__raw(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(raw()); }

// fn umc__noraw(): bool
UMKA_EXPORT void umc__noraw(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(noraw()); }

// fn umc__cbreak(): bool
UMKA_EXPORT void umc__cbreak(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(cbreak()); }

// fn umc__nocbreak(): bool
UMKA_EXPORT void umc__nocbreak(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(nocbreak()); }

// fn umc__echo(): bool
UMKA_EXPORT void umc__echo(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(echo()); }

// fn umc__noecho(): bool
UMKA_EXPORT void umc__noecho(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(noecho()); }

// fn umc__keypad(win: RawWindow, enable: bool): bool
UMKA_EXPORT void umc__keypad(UmkaStackSlot *params, UmkaStackSlot *result)
{
    WINDOW *w   = GET_WINDOW(params, 0);
    bool enable = umkaGetParam(params, 1)->uintVal;
    ERR_RET(keypad(w, enable));
}

// fn umc__halfdelay(win: RawWindow, enable: bool): bool
UMKA_EXPORT void umc__halfdelay(UmkaStackSlot *params, UmkaStackSlot *result)
{
    ERR_RET(halfdelay(umkaGetParam(params, 0)->intVal));
}

// fn umc__set_term(term: RawTerminal): RawTerminal
UMKA_EXPORT void umc__set_term(UmkaStackSlot *params, UmkaStackSlot *result)
{
    SCREEN *new = GET_SCREEN(params, 0);

    RET_SCREEN(params, result) = set_term(new);
}

// fn umc__newterm(termType: str, outf, inf: std::File): RawTerminal
UMKA_EXPORT void umc__newterm(UmkaStackSlot *params, UmkaStackSlot *result)
{
    char *type = umkaGetParam(params, 0)->ptrVal;
    FILE *outf = umkaGetParam(params, 1)->ptrVal;
    FILE *inf  = umkaGetParam(params, 2)->ptrVal;

    RET_SCREEN(params, result) = newterm(type, outf, inf);
}

// fn umc__delscreen(term: RawTerminal)
UMKA_EXPORT void umc__delscreen(UmkaStackSlot *params, UmkaStackSlot *result)
{
    UNUSED(result);
    delscreen(GET_SCREEN(params, 0));
}

// fn umc__fnkeyoffset(key: int): int
UMKA_EXPORT void umc__fnkeyoffset(UmkaStackSlot *params, UmkaStackSlot *result)
{
    int key = umkaGetParam(params, 0)->intVal;

    umkaGetResult(params, result)->intVal = key - KEY_F0;
}

static int keys[] = {
    KEY_DOWN,
    KEY_UP,
    KEY_LEFT,
    KEY_RIGHT,
};

static int keysLen = ARRLEN(keys);

// fn umc__otherkeyoffset(key: int): int
UMKA_EXPORT void umc__otherkeyoffset(UmkaStackSlot *params, UmkaStackSlot *result)
{
    int key = umkaGetParam(params, 0)->intVal;

    int res = -1;
    for (int i = 0; i < keysLen; i++)
        if (key == keys[i]) res = i;

    umkaGetResult(params, result)->intVal = res;
}

// must match enum in catcurses.um
static const int attributes[] = {
    A_NORMAL,  A_STANDOUT, A_UNDERLINE,  A_REVERSE, A_BLINK,    A_DIM,   A_BOLD,
    A_PROTECT, A_INVIS,    A_ALTCHARSET, A_ITALIC,  A_CHARTEXT, A_COLOR,
};

// fn umc__wattron(win: RawWindow, a: int): int
UMKA_EXPORT void umc__wattron(UmkaStackSlot *params, UmkaStackSlot *result)
{
    WINDOW *win = GET_WINDOW(params, 0);
    int attr    = umkaGetParam(params, 1)->intVal;

    ERR_RET(wattron(win, attributes[attr]));
}

// fn umc__wattroff(win: RawWindow, a: int): int
UMKA_EXPORT void umc__wattroff(UmkaStackSlot *params, UmkaStackSlot *result)
{
    WINDOW *win = GET_WINDOW(params, 0);
    int attr    = umkaGetParam(params, 1)->intVal;

    ERR_RET(wattroff(win, attributes[attr]));
}

// fn umc__clear(win: RawWindow): bool
UMKA_EXPORT void umc__clear(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(wclear(GET_WINDOW(params, 0))); }

// fn umc__erase(win: RawWindow): bool
UMKA_EXPORT void umc__erase(UmkaStackSlot *params, UmkaStackSlot *result) { ERR_RET(werase(GET_WINDOW(params, 0))); }
