#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "transport.h"

static char const *subjects[] = {
    "AUTO", "MATH", "ALGEBRA", "GEOMETRY", "STATISTICS",
    "CALCULUS", "BIOLOGY", "CHEMISTRY", "PHYSICS", "EARTH/SPACE",
    "ELA", "LITERATURE", "WRITING", "HISTORY", "GOV/CIVICS",
    "ECONOMICS", "BUSINESS", "COMPUTER SCI", "LANGUAGE", "HEALTH"
};

static char const *modes[] = {
    "ANSWER", "EXPLAIN", "STEPS", "CHECK", "QUIZ", "SUMMARY", "FLASHCARDS"
};

#define SUBJECT_COUNT ((int)(sizeof(subjects) / sizeof(subjects[0])))
#define MODE_COUNT ((int)(sizeof(modes) / sizeof(modes[0])))

typedef struct {
    int subject;
    int mode;
    int scroll;
    bool bridge_ready;
} app_state_t;

static void draw_badge(int x, int y, int w, char const *text, color_t bg, color_t fg)
{
    drect(x, y, x + w, y + 18, bg);
    dtext(x + 6, y + 3, fg, text);
}

static void draw_wrapped(int x, int y, int width_px, char const *text, color_t color)
{
    /* Small predictable wrapper: about 7 px per glyph in the default font. */
    int max_chars = width_px / 7;
    char line[64];
    int len = (int)strlen(text);
    int start = 0;

    while(start < len && y < DHEIGHT - 42) {
        int remaining = len - start;
        int take = remaining < max_chars ? remaining : max_chars;

        if(take < remaining) {
            int i;
            for(i = take; i > 0; i--) {
                if(text[start + i] == ' ') {
                    take = i;
                    break;
                }
            }
        }

        if(take <= 0) take = remaining < max_chars ? remaining : max_chars;
        if(take >= (int)sizeof(line)) take = (int)sizeof(line) - 1;

        memcpy(line, text + start, take);
        line[take] = '\0';
        dtext(x, y, color, line);
        y += 17;

        start += take;
        while(start < len && text[start] == ' ') start++;
    }
}

static void draw_chat(app_state_t const *state)
{
    color_t bg = C_RGB(3, 3, 4);
    color_t panel = C_RGB(6, 6, 8);
    color_t line = C_RGB(10, 10, 13);
    color_t text = C_RGB(29, 29, 30);
    color_t muted = C_RGB(18, 18, 20);
    color_t accent = C_RGB(28, 11, 8);
    color_t green = C_RGB(8, 24, 13);

    dclear(bg);

    /* Top bar */
    drect(0, 0, DWIDTH - 1, 31, panel);
    drect(0, 31, DWIDTH - 1, 32, line);
    dtext(10, 8, text, "Q  QUANTUM BREAKS AI");

    if(state->bridge_ready) {
        draw_badge(DWIDTH - 78, 6, 66, "LINKED", green, text);
    }
    else {
        draw_badge(DWIDTH - 78, 6, 66, "OFFLINE", line, muted);
    }

    /* Subject/mode bar */
    dtext(10, 43, muted, "SUBJECT");
    draw_badge(76, 39, 116, subjects[state->subject], line, text);
    dtext(206, 43, muted, "MODE");
    draw_badge(252, 39, 124, modes[state->mode], accent, text);

    /* Conversation area */
    dtext(12, 76, accent, "YOU");
    draw_wrapped(
        12, 94, DWIDTH - 24,
        "Explain this school problem or topic using the selected subject and tutor mode.",
        text
    );

    dtext(12, 143, C_RGB(12, 22, 31), "QBAI");
    draw_wrapped(
        12, 161, DWIDTH - 24,
        "Native CG50 UI is running. Live model responses will arrive through the serial bridge.",
        text
    );

    /* Bottom controls */
    drect(0, DHEIGHT - 31, DWIDTH - 1, DHEIGHT - 1, panel);
    drect(0, DHEIGHT - 32, DWIDTH - 1, DHEIGHT - 31, line);
    dtext(7, DHEIGHT - 23, muted, "F1 SUBJECT");
    dtext(105, DHEIGHT - 23, muted, "F2 MODE");
    dtext(190, DHEIGHT - 23, muted, "F5 LINK");
    dtext(270, DHEIGHT - 23, muted, "EXIT QUIT");

    dupdate();
}

int main(void)
{
    app_state_t state = {
        .subject = 0,
        .mode = 1,
        .scroll = 0,
        .bridge_ready = false,
    };

    qb_transport_init();
    state.bridge_ready = qb_transport_ready();

    while(true) {
        draw_chat(&state);

        key_event_t ev = getkey();

        if(ev.key == KEY_EXIT) {
            break;
        }
        else if(ev.key == KEY_F1) {
            state.subject = (state.subject + 1) % SUBJECT_COUNT;
        }
        else if(ev.key == KEY_F2) {
            state.mode = (state.mode + 1) % MODE_COUNT;
        }
        else if(ev.key == KEY_F5) {
            state.bridge_ready = qb_transport_probe();
        }
        else if(ev.key == KEY_UP) {
            if(state.scroll > 0) state.scroll--;
        }
        else if(ev.key == KEY_DOWN) {
            state.scroll++;
        }
    }

    qb_transport_close();
    return 1;
}
