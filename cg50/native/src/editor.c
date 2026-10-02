#include "editor.h"

#include <gint/display.h>
#include <gint/keyboard.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int key;
    char normal;
    char alpha;
} key_map_t;

static key_map_t const map[] = {
    {KEY_7, '7', 'M'}, {KEY_8, '8', 'N'}, {KEY_9, '9', 'O'},
    {KEY_4, '4', 'P'}, {KEY_5, '5', 'Q'}, {KEY_6, '6', 'R'},
    {KEY_1, '1', 'U'}, {KEY_2, '2', 'V'}, {KEY_3, '3', 'W'},
    {KEY_0, '0', 'Z'}, {KEY_DOT, '.', ' '},
    {KEY_XOT, 0, 'A'}, {KEY_LOG, 0, 'B'}, {KEY_LN, 0, 'C'},
    {KEY_SIN, 0, 'D'}, {KEY_COS, 0, 'E'}, {KEY_TAN, 0, 'F'},
    {KEY_FRAC, 0, 'G'}, {KEY_FD, 0, 'H'}, {KEY_LEFTP, '(', 'I'},
    {KEY_RIGHTP, ')', 'J'}, {KEY_COMMA, ',', 'K'}, {KEY_ARROW, 0, 'L'},
    {KEY_MUL, '*', 'S'}, {KEY_DIV, '/', 'T'},
    {KEY_ADD, '+', 'X'}, {KEY_SUB, '-', 'Y'},
    {0, 0, 0}
};

static char map_key(int key, bool alpha, bool lower)
{
    int i;

    for(i = 0; map[i].key != 0; i++) {
        if(map[i].key == key) {
            char c = alpha ? map[i].alpha : map[i].normal;
            if(c && alpha && lower && c >= 'A' && c <= 'Z') {
                c = (char)tolower((unsigned char)c);
            }
            return c;
        }
    }

    return 0;
}

static void draw_editor(char const *buffer, bool alpha, bool lower)
{
    color_t bg = C_RGB(3, 3, 4);
    color_t panel = C_RGB(6, 6, 8);
    color_t line = C_RGB(10, 10, 13);
    color_t text = C_RGB(29, 29, 30);
    color_t muted = C_RGB(18, 18, 20);
    color_t accent = C_RGB(28, 11, 8);
    int len = (int)strlen(buffer);
    int start = len > 180 ? len - 180 : 0;

    dclear(bg);
    drect(0, 0, DWIDTH - 1, 31, panel);
    dtext(10, 8, text, "NEW QUESTION");
    dtext(DWIDTH - 104, 8, accent, alpha ? (lower ? "alpha abc" : "ALPHA ABC") : "123/SYM");

    drect(10, 47, DWIDTH - 11, DHEIGHT - 48, panel);
    drect(10, 47, DWIDTH - 11, 48, line);
    dtext(18, 58, text, buffer + start);

    drect(0, DHEIGHT - 31, DWIDTH - 1, DHEIGHT - 1, panel);
    dtext(7, DHEIGHT - 23, muted, "ALPHA LETTERS");
    dtext(126, DHEIGHT - 23, muted, "SHIFT CASE");
    dtext(224, DHEIGHT - 23, muted, "DEL");
    dtext(274, DHEIGHT - 23, muted, "EXE SEND");
    dupdate();
}

bool qb_editor_read(char *buffer, size_t buffer_size)
{
    bool alpha = true;
    bool lower = false;
    size_t len = strlen(buffer);

    while(true) {
        char c;
        key_event_t ev;

        draw_editor(buffer, alpha, lower);
        ev = getkey();

        if(ev.key == KEY_EXIT) {
            return false;
        }

        if(ev.key == KEY_EXE) {
            return len > 0;
        }

        if(ev.key == KEY_ALPHA) {
            alpha = !alpha;
            continue;
        }

        if(ev.key == KEY_SHIFT) {
            lower = !lower;
            continue;
        }

        if(ev.key == KEY_DEL) {
            if(len > 0) {
                buffer[--len] = '\0';
            }
            continue;
        }

        c = map_key(ev.key, alpha, lower);
        if(c && len + 1 < buffer_size) {
            buffer[len++] = c;
            buffer[len] = '\0';
        }
    }
}
