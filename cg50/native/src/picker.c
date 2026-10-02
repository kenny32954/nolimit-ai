#include "picker.h"

#include <gint/display.h>
#include <gint/keyboard.h>

#define VISIBLE_ROWS 7

static void draw_picker(
    char const *title,
    char const *const *items,
    int count,
    int selected
)
{
    color_t bg = C_RGB(3, 3, 4);
    color_t panel = C_RGB(6, 6, 8);
    color_t line = C_RGB(10, 10, 13);
    color_t text = C_RGB(29, 29, 30);
    color_t muted = C_RGB(18, 18, 20);
    color_t accent = C_RGB(28, 11, 8);
    int start = selected - VISIBLE_ROWS / 2;
    int i;

    if(start < 0) start = 0;
    if(start > count - VISIBLE_ROWS) start = count - VISIBLE_ROWS;
    if(start < 0) start = 0;

    dclear(bg);
    drect(0, 0, DWIDTH - 1, 31, panel);
    dtext(10, 8, text, title);
    dtext(DWIDTH - 116, 8, muted, "EXE SELECT");

    for(i = 0; i < VISIBLE_ROWS && start + i < count; i++) {
        int index = start + i;
        int y = 42 + i * 23;

        if(index == selected) {
            drect(8, y - 3, DWIDTH - 9, y + 17, accent);
            dtext(17, y, text, items[index]);
        }
        else {
            dtext(17, y, muted, items[index]);
        }
    }

    drect(0, DHEIGHT - 31, DWIDTH - 1, DHEIGHT - 1, panel);
    drect(0, DHEIGHT - 32, DWIDTH - 1, DHEIGHT - 31, line);
    dtext(8, DHEIGHT - 23, muted, "UP/DOWN MOVE");
    dtext(150, DHEIGHT - 23, muted, "EXE SELECT");
    dtext(279, DHEIGHT - 23, muted, "EXIT BACK");

    dupdate();
}

int qb_picker_select(
    char const *title,
    char const *const *items,
    int count,
    int current
)
{
    int selected = current;

    if(count <= 0) return current;
    if(selected < 0 || selected >= count) selected = 0;

    while(1) {
        key_event_t ev;

        draw_picker(title, items, count, selected);
        ev = getkey();

        if(ev.key == KEY_EXIT) return current;
        if(ev.key == KEY_EXE) return selected;

        if(ev.key == KEY_UP) {
            selected--;
            if(selected < 0) selected = count - 1;
        }
        else if(ev.key == KEY_DOWN) {
            selected++;
            if(selected >= count) selected = 0;
        }
    }
}
