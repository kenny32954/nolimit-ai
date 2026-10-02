#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/clock.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "editor.h"
#include "transport.h"

static char const *subjects[] = {
    "auto", "math", "algebra", "geometry", "statistics", "calculus",
    "biology", "chemistry", "physics", "earth", "environmental_science",
    "ela", "literature", "writing", "history", "social_studies",
    "geography", "government", "economics", "business", "accounting",
    "computer_science", "engineering", "cte", "agriculture", "psychology",
    "sociology", "art", "music", "media", "language", "health",
    "physical_education"
};

static char const *subject_labels[] = {
    "AUTO", "MATH", "ALGEBRA", "GEOMETRY", "STATISTICS", "CALCULUS",
    "BIOLOGY", "CHEMISTRY", "PHYSICS", "EARTH/SPACE", "ENV SCI",
    "ELA", "LITERATURE", "WRITING", "HISTORY", "SOCIAL STUDIES",
    "GEOGRAPHY", "GOV/CIVICS", "ECONOMICS", "BUSINESS", "ACCOUNTING",
    "COMPUTER SCI", "ENGINEERING", "CTE", "AGRICULTURE", "PSYCHOLOGY",
    "SOCIOLOGY", "ART", "MUSIC", "MEDIA/A-V", "LANGUAGE", "HEALTH", "PE"
};

static char const *modes[] = {
    "answer", "explain", "steps", "check", "quiz", "summary", "flashcards"
};

static char const *mode_labels[] = {
    "ANSWER", "EXPLAIN", "STEPS", "CHECK", "QUIZ", "SUMMARY", "FLASHCARDS"
};

#define SUBJECT_COUNT ((int)(sizeof(subjects) / sizeof(subjects[0])))
#define MODE_COUNT ((int)(sizeof(modes) / sizeof(modes[0])))
#define PROMPT_CAP 700
#define ANSWER_CAP 4096

typedef struct {
    int subject;
    int mode;
    int scroll;
    bool bridge_ready;
    bool waiting;
    uint32_t request_id;
    char prompt[PROMPT_CAP];
    char answer[ANSWER_CAP];
    size_t answer_len;
    char status[96];
} app_state_t;

static void draw_badge(int x, int y, int w, char const *text, color_t bg, color_t fg)
{
    drect(x, y, x + w, y + 18, bg);
    dtext(x + 6, y + 3, fg, text);
}

static int draw_wrapped(int x, int y, int width_px, char const *text,
                        color_t color, int skip_lines)
{
    int max_chars = width_px / 7;
    char line[64];
    int len = (int)strlen(text);
    int start = 0;
    int logical_line = 0;

    while(start < len && y < DHEIGHT - 42) {
        int remaining = len - start;
        int take = remaining < max_chars ? remaining : max_chars;

        if(text[start] == '\n') {
            start++;
            logical_line++;
            if(logical_line > skip_lines) y += 17;
            continue;
        }

        if(take < remaining) {
            int i;
            int newline_at = -1;

            for(i = 0; i < take; i++) {
                if(text[start + i] == '\n') {
                    newline_at = i;
                    break;
                }
            }

            if(newline_at >= 0) {
                take = newline_at;
            }
            else {
                for(i = take; i > 0; i--) {
                    if(text[start + i] == ' ') {
                        take = i;
                        break;
                    }
                }
            }
        }

        if(take <= 0) take = remaining < max_chars ? remaining : max_chars;
        if(take >= (int)sizeof(line)) take = (int)sizeof(line) - 1;

        memcpy(line, text + start, take);
        line[take] = '\0';

        if(logical_line >= skip_lines) {
            dtext(x, y, color, line);
            y += 17;
        }

        logical_line++;
        start += take;

        if(text[start] == '\n') start++;
        while(start < len && text[start] == ' ') start++;
    }

    return y;
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
    color_t blue = C_RGB(12, 22, 31);
    int y = 74;

    dclear(bg);

    drect(0, 0, DWIDTH - 1, 31, panel);
    drect(0, 31, DWIDTH - 1, 32, line);
    dtext(10, 8, text, "Q  QUANTUM BREAKS AI");

    if(state->bridge_ready) {
        draw_badge(DWIDTH - 78, 6, 66, state->waiting ? "THINKING" : "LINKED",
                   state->waiting ? accent : green, text);
    }
    else {
        draw_badge(DWIDTH - 78, 6, 66, "OFFLINE", line, muted);
    }

    dtext(10, 43, muted, "SUBJECT");
    draw_badge(76, 39, 116, subject_labels[state->subject], line, text);
    dtext(206, 43, muted, "MODE");
    draw_badge(252, 39, 124, mode_labels[state->mode], accent, text);

    if(state->prompt[0]) {
        dtext(12, y, accent, "YOU");
        y += 18;
        y = draw_wrapped(12, y, DWIDTH - 24, state->prompt, text, 0);
        y += 8;
    }

    if(state->answer[0] && y < DHEIGHT - 55) {
        dtext(12, y, blue, "QBAI");
        y += 18;
        draw_wrapped(12, y, DWIDTH - 24, state->answer, text, state->scroll);
    }
    else if(!state->prompt[0]) {
        dtext(12, 86, muted, "Press EXE to ask a question.");
        dtext(12, 105, muted, "F1 subject  F2 tutor mode");
        dtext(12, 124, muted, "F5 connects to the desktop bridge.");
    }

    drect(0, DHEIGHT - 31, DWIDTH - 1, DHEIGHT - 1, panel);
    drect(0, DHEIGHT - 32, DWIDTH - 1, DHEIGHT - 31, line);
    dtext(7, DHEIGHT - 23, muted, "F1 SUBJECT");
    dtext(105, DHEIGHT - 23, muted, "F2 MODE");
    dtext(190, DHEIGHT - 23, muted, "F5 LINK");
    dtext(268, DHEIGHT - 23, muted, "EXE ASK");

    if(state->status[0]) {
        dtext(12, DHEIGHT - 48, muted, state->status);
    }

    dupdate();
}

static void append_answer(app_state_t *state, char const *chunk)
{
    size_t add = strlen(chunk);
    size_t space = ANSWER_CAP - 1 - state->answer_len;

    if(add > space) add = space;
    if(add == 0) return;

    memcpy(state->answer + state->answer_len, chunk, add);
    state->answer_len += add;
    state->answer[state->answer_len] = '\0';
}

static void poll_transport(app_state_t *state)
{
    char chunk[256];
    qb_transport_event_t event;

    if(!state->waiting) return;

    do {
        event = qb_transport_poll(state->request_id, chunk, sizeof(chunk));

        if(event == QB_TRANSPORT_CHUNK) {
            append_answer(state, chunk);
        }
        else if(event == QB_TRANSPORT_DONE) {
            append_answer(state, chunk);
            state->waiting = false;
            snprintf(state->status, sizeof(state->status), "Answer complete");
        }
        else if(event == QB_TRANSPORT_ERROR) {
            state->waiting = false;
            snprintf(state->status, sizeof(state->status), "Bridge error: %.70s", chunk);
        }
    } while(event == QB_TRANSPORT_CHUNK);
}

int main(void)
{
    app_state_t state = {
        .subject = 0,
        .mode = 1,
        .scroll = 0,
        .bridge_ready = false,
        .waiting = false,
        .request_id = 1,
        .prompt = "",
        .answer = "",
        .answer_len = 0,
        .status = ""
    };

    qb_transport_init();
    state.bridge_ready = qb_transport_ready();

    while(true) {
        key_event_t ev;

        poll_transport(&state);
        draw_chat(&state);

        /*
         * Use pollevent while a response is arriving so serial polling and UI
         * refresh keep running. Otherwise getkey() can sleep until input.
         */
        ev = state.waiting ? pollevent() : getkey();

        if(state.waiting && ev.type == KEYEV_NONE) {
            sleep_us_spin(8000);
            continue;
        }

        if(ev.type != KEYEV_DOWN && state.waiting) continue;

        if(ev.key == KEY_EXIT) {
            if(state.waiting) {
                state.waiting = false;
                snprintf(state.status, sizeof(state.status), "Local wait cancelled");
            }
            else {
                break;
            }
        }
        else if(!state.waiting && ev.key == KEY_F1) {
            state.subject = (state.subject + 1) % SUBJECT_COUNT;
        }
        else if(!state.waiting && ev.key == KEY_F2) {
            state.mode = (state.mode + 1) % MODE_COUNT;
        }
        else if(!state.waiting && ev.key == KEY_F5) {
            snprintf(state.status, sizeof(state.status), "Connecting...");
            draw_chat(&state);
            state.bridge_ready = qb_transport_probe();
            snprintf(state.status, sizeof(state.status),
                     state.bridge_ready ? "Bridge linked" : "Bridge not found");
        }
        else if(!state.waiting && ev.key == KEY_EXE) {
            char draft[PROMPT_CAP];
            draft[0] = '\0';

            if(qb_editor_read(draft, sizeof(draft))) {
                strncpy(state.prompt, draft, sizeof(state.prompt) - 1);
                state.prompt[sizeof(state.prompt) - 1] = '\0';
                state.answer[0] = '\0';
                state.answer_len = 0;
                state.scroll = 0;

                if(!state.bridge_ready) {
                    state.bridge_ready = qb_transport_probe();
                }

                if(state.bridge_ready && qb_transport_send_request(
                    state.request_id,
                    subjects[state.subject],
                    modes[state.mode],
                    state.prompt
                )) {
                    state.waiting = true;
                    snprintf(state.status, sizeof(state.status), "Question sent");
                }
                else {
                    snprintf(state.status, sizeof(state.status), "No bridge connection");
                }

                state.request_id++;
                if(state.request_id == 0) state.request_id = 1;
            }
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
