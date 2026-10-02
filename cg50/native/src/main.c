#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/clock.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "diagnostics.h"
#include "editor.h"
#include "picker.h"
#include "reference.h"
#include "transport.h"

static char const *subjects[] = {
    "auto",
    "pre_algebra", "algebra_1", "geometry", "algebra_2", "trigonometry",
    "precalculus", "statistics", "calculus", "consumer_math", "financial_math",
    "linear_algebra", "discrete_math", "differential_equations",
    "number_theory", "real_analysis", "abstract_algebra",
    "physical_science", "biology", "genetics", "microbiology",
    "anatomy_physiology", "chemistry", "organic_chemistry", "biochemistry",
    "physics", "thermodynamics", "circuits", "earth", "astronomy",
    "environmental_science", "forensic_science", "marine_science",
    "ela", "composition", "american_literature", "british_literature",
    "world_literature", "literature", "creative_writing", "journalism",
    "speech_debate", "media_literacy",
    "history", "world_history", "us_history", "european_history",
    "social_studies", "geography", "government", "civics", "political_science",
    "economics", "personal_finance", "finance", "psychology", "sociology",
    "anthropology", "research_methods", "philosophy_logic",
    "business", "marketing", "entrepreneurship", "accounting", "business_law",
    "computer_science", "web_development", "data_structures", "algorithms",
    "databases", "computer_architecture", "cybersecurity",
    "information_technology", "engineering", "robotics", "electronics",
    "cad_drafting", "statics_dynamics", "materials_science",
    "cte", "career_readiness", "agriculture", "animal_science", "plant_science",
    "construction_trades", "automotive_technology", "culinary_arts",
    "family_consumer_science", "child_development",
    "art", "drawing_painting", "graphic_design", "photography",
    "ceramics_sculpture", "art_history",
    "music", "music_theory", "band_orchestra", "choir", "theater", "dance",
    "media", "film_studies",
    "language", "spanish", "french", "german", "latin", "asl",
    "health", "nutrition", "physical_education", "sports_medicine",
    "exercise_science", "drivers_education", "jrotc_leadership",
    "yearbook", "study_skills"
};

static char const *subject_labels[] = {
    "AUTO",
    "PRE-ALGEBRA", "ALGEBRA I", "GEOMETRY", "ALGEBRA II", "TRIG",
    "PRECALCULUS", "STATISTICS", "CALCULUS", "CONSUMER MATH", "FINANCIAL MATH",
    "LINEAR ALG", "DISCRETE MATH", "DIFF EQ",
    "NUMBER THEORY", "REAL ANALYSIS", "ABSTRACT ALG",
    "PHYSICAL SCI", "BIOLOGY", "GENETICS", "MICROBIOLOGY",
    "ANATOMY/PHYS", "CHEMISTRY", "ORGANIC CHEM", "BIOCHEMISTRY",
    "PHYSICS", "THERMODYNAMICS", "CIRCUITS", "EARTH/SPACE", "ASTRONOMY",
    "ENV SCI", "FORENSICS", "MARINE SCI",
    "ELA", "COMPOSITION", "AMERICAN LIT", "BRITISH LIT",
    "WORLD LIT", "LITERATURE", "CREATIVE WRITE", "JOURNALISM",
    "SPEECH/DEBATE", "MEDIA LITERACY",
    "HISTORY", "WORLD HISTORY", "U.S. HISTORY", "EURO HISTORY",
    "SOCIAL STUDIES", "GEOGRAPHY", "GOV/CIVICS", "CIVICS", "POLI SCI",
    "ECONOMICS", "PERSONAL FIN", "FINANCE", "PSYCHOLOGY", "SOCIOLOGY",
    "ANTHROPOLOGY", "RESEARCH", "PHIL/LOGIC",
    "BUSINESS", "MARKETING", "ENTREPRENEUR", "ACCOUNTING", "BUSINESS LAW",
    "COMPUTER SCI", "WEB DEV", "DATA STRUCT", "ALGORITHMS",
    "DATABASES", "COMP ARCH", "CYBERSECURITY",
    "INFO TECH", "ENGINEERING", "ROBOTICS", "ELECTRONICS",
    "CAD/DRAFTING", "STATICS/DYN", "MATERIALS",
    "CTE", "CAREER READY", "AGRICULTURE", "ANIMAL SCI", "PLANT SCI",
    "CONSTRUCTION", "AUTO TECH", "CULINARY ARTS",
    "FAMILY/CONSUMER", "CHILD DEV",
    "ART", "DRAW/PAINT", "GRAPHIC DESIGN", "PHOTOGRAPHY",
    "CERAMICS/SCULPT", "ART HISTORY",
    "MUSIC", "MUSIC THEORY", "BAND/ORCH", "CHOIR", "THEATER", "DANCE",
    "MEDIA/A-V", "FILM STUDIES",
    "LANGUAGE", "SPANISH", "FRENCH", "GERMAN", "LATIN", "ASL",
    "HEALTH", "NUTRITION", "PE", "SPORTS MED",
    "EXERCISE SCI", "DRIVER ED", "JROTC/LEAD",
    "YEARBOOK", "STUDY SKILLS"
};

static char const *modes[] = {
    "answer", "explain", "steps", "check", "quiz",
    "summary", "flashcards", "derive", "proof", "research"
};

static char const *mode_labels[] = {
    "ANSWER", "EXPLAIN", "STEPS", "CHECK", "QUIZ",
    "SUMMARY", "FLASHCARDS", "DERIVE", "PROOF", "RESEARCH"
};

static char const *levels[] = {
    "auto", "school", "college", "advanced"
};

static char const *level_labels[] = {
    "AUTO", "SCHOOL", "COLLEGE", "ADVANCED"
};

#define SUBJECT_COUNT ((int)(sizeof(subjects) / sizeof(subjects[0])))
#define MODE_COUNT ((int)(sizeof(modes) / sizeof(modes[0])))
#define LEVEL_COUNT ((int)(sizeof(levels) / sizeof(levels[0])))
#define PROMPT_CAP 700
#define ANSWER_CAP 4096
#define RESPONSE_TIMEOUT_TICKS 12500
#define HISTORY_SLOTS 4
#define HISTORY_PROMPT_CAP 240
#define HISTORY_ANSWER_CAP 1600

typedef struct {
    int subject;
    int mode;
    int level;
    int scroll;
    bool bridge_ready;
    bool waiting;
    bool focus_answer;
    uint32_t active_request_id;
    uint32_t next_request_id;
    unsigned int wait_ticks;
    char prompt[PROMPT_CAP];
    char answer[ANSWER_CAP];
    size_t answer_len;
    char status[96];
} app_state_t;

typedef struct {
    char prompt[HISTORY_PROMPT_CAP];
    char answer[HISTORY_ANSWER_CAP];
    int subject;
    int mode;
    int level;
} history_turn_t;

static history_turn_t recent[HISTORY_SLOTS];
static int recent_count = 0;
static int recent_view = -1;

static void save_history_turn(app_state_t const *state)
{
    int i;

    if(!state->prompt[0] || !state->answer[0]) return;

    for(i = HISTORY_SLOTS - 1; i > 0; i--) {
        recent[i] = recent[i - 1];
    }

    strncpy(recent[0].prompt, state->prompt, HISTORY_PROMPT_CAP - 1);
    recent[0].prompt[HISTORY_PROMPT_CAP - 1] = '\0';
    strncpy(recent[0].answer, state->answer, HISTORY_ANSWER_CAP - 1);
    recent[0].answer[HISTORY_ANSWER_CAP - 1] = '\0';
    recent[0].subject = state->subject;
    recent[0].mode = state->mode;
    recent[0].level = state->level;

    if(recent_count < HISTORY_SLOTS) recent_count++;
    recent_view = 0;
}

static void load_history_turn(app_state_t *state, int index)
{
    if(index < 0 || index >= recent_count) return;

    strncpy(state->prompt, recent[index].prompt, sizeof(state->prompt) - 1);
    state->prompt[sizeof(state->prompt) - 1] = '\0';
    strncpy(state->answer, recent[index].answer, sizeof(state->answer) - 1);
    state->answer[sizeof(state->answer) - 1] = '\0';
    state->answer_len = strlen(state->answer);
    state->subject = recent[index].subject;
    state->mode = recent[index].mode;
    state->level = recent[index].level;
    state->scroll = 0;
    recent_view = index;
    snprintf(state->status, sizeof(state->status),
             "History %d/%d  LEFT/RIGHT browse",
             index + 1, recent_count);
}

static void draw_badge(int x, int y, int w, char const *text, color_t bg, color_t fg)
{
    drect(x, y, x + w, y + 18, bg);
    dtext(x + 6, y + 3, fg, text);
}

static int draw_wrapped(
    int x,
    int y,
    int width_px,
    char const *text,
    color_t color,
    int skip_lines,
    int max_draw_lines
)
{
    int max_chars = width_px / 7;
    char line[64];
    int len = (int)strlen(text);
    int start = 0;
    int logical_line = 0;
    int drawn = 0;

    while(start < len && y < DHEIGHT - 42) {
        int remaining = len - start;
        int take = remaining < max_chars ? remaining : max_chars;

        if(text[start] == '\n') {
            start++;
            logical_line++;
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

        memcpy(line, text + start, (size_t)take);
        line[take] = '\0';

        if(logical_line >= skip_lines) {
            if(max_draw_lines >= 0 && drawn >= max_draw_lines) break;
            dtext(x, y, color, line);
            y += 17;
            drawn++;
        }

        logical_line++;
        start += take;

        if(start < len && text[start] == '\n') start++;
        while(start < len && text[start] == ' ') start++;
    }

    return y;
}

static void draw_answer_focus(app_state_t const *state)
{
    color_t bg = C_RGB(3, 3, 4);
    color_t panel = C_RGB(6, 6, 8);
    color_t line = C_RGB(10, 10, 13);
    color_t text = C_RGB(29, 29, 30);
    color_t muted = C_RGB(18, 18, 20);
    color_t accent = C_RGB(28, 11, 8);
    color_t blue = C_RGB(12, 22, 31);

    dclear(bg);
    drect(0, 0, DWIDTH - 1, 31, panel);
    dtext(10, 8, blue, "QBAI ANSWER");
    dtext(106, 8, muted, subject_labels[state->subject]);
    dtext(238, 8, accent, mode_labels[state->mode]);
    dtext(322, 8, muted, level_labels[state->level]);

    draw_wrapped(
        12, 43, DWIDTH - 24,
        state->answer[0] ? state->answer : "Waiting for an answer...",
        text,
        state->scroll,
        -1
    );

    drect(0, DHEIGHT - 31, DWIDTH - 1, DHEIGHT - 1, panel);
    drect(0, DHEIGHT - 32, DWIDTH - 1, DHEIGHT - 31, line);
    dtext(8, DHEIGHT - 23, muted, "UP/DOWN SCROLL");
    dtext(151, DHEIGHT - 23, muted, "F3 CHAT");
    dtext(245, DHEIGHT - 23, muted, state->waiting ? "EXIT CANCEL" : "EXIT BACK");
    dupdate();
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

    if(state->focus_answer) {
        draw_answer_focus(state);
        return;
    }

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
    dtext(10, 62, muted, "LEVEL");
    dtext(63, 62, accent, level_labels[state->level]);
    dtext(155, 62, muted, "OPTN changes academic level");

    y = 84;

    if(state->prompt[0]) {
        dtext(12, y, accent, "YOU");
        y += 18;
        y = draw_wrapped(12, y, DWIDTH - 24, state->prompt, text, 0, 2);
        y += 7;
    }

    if(state->answer[0] && y < DHEIGHT - 55) {
        dtext(12, y, blue, "QBAI");
        y += 18;
        draw_wrapped(12, y, DWIDTH - 24, state->answer, text, state->scroll, -1);
    }
    else if(!state->prompt[0]) {
        dtext(12, 88, muted, "Press EXE to ask a question.");
        dtext(12, 107, muted, "F1 subject  F2 mode  OPTN level");
        dtext(12, 126, muted, "College + advanced disciplines supported.");
    }

    drect(0, DHEIGHT - 31, DWIDTH - 1, DHEIGHT - 1, panel);
    drect(0, DHEIGHT - 32, DWIDTH - 1, DHEIGHT - 31, line);
    dtext(7, DHEIGHT - 23, muted, "F1 SUBJ");
    dtext(69, DHEIGHT - 23, muted, "F2 MODE");
    dtext(137, DHEIGHT - 23, muted, "F3 ANS");
    dtext(195, DHEIGHT - 23, muted, "F4 REF");
    dtext(254, DHEIGHT - 23, muted, "F5 LINK");
    dtext(321, DHEIGHT - 23, muted, "EXE");

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

    if(space == add && state->answer_len == ANSWER_CAP - 1) {
        snprintf(state->status, sizeof(state->status), "Answer truncated to local buffer");
    }
}

static void poll_transport(app_state_t *state)
{
    char chunk[256];
    qb_transport_event_t event;

    if(!state->waiting) return;

    do {
        event = qb_transport_poll(state->active_request_id, chunk, sizeof(chunk));

        if(event == QB_TRANSPORT_CHUNK) {
            append_answer(state, chunk);
            state->wait_ticks = 0;
        }
        else if(event == QB_TRANSPORT_DONE) {
            append_answer(state, chunk);
            state->waiting = false;
            state->wait_ticks = 0;
            save_history_turn(state);
            snprintf(state->status, sizeof(state->status),
                     recent_count > 1
                         ? "Answer complete - LEFT/RIGHT history"
                         : "Answer complete");
        }
        else if(event == QB_TRANSPORT_ERROR) {
            state->waiting = false;
            state->wait_ticks = 0;
            snprintf(state->status, sizeof(state->status), "Bridge error: %.70s", chunk);
        }
    } while(event == QB_TRANSPORT_CHUNK);
}

static bool start_request(app_state_t *state)
{
    uint32_t id;

    if(!state->prompt[0]) return false;

    if(!state->bridge_ready) {
        state->bridge_ready = qb_transport_probe();
    }

    if(!state->bridge_ready) {
        snprintf(state->status, sizeof(state->status), "Bridge not found");
        return false;
    }

    id = state->next_request_id++;
    if(state->next_request_id == 0) state->next_request_id = 1;

    if(!qb_transport_send_request(
        id,
        subjects[state->subject],
        modes[state->mode],
        levels[state->level],
        state->prompt
    )) {
        snprintf(state->status, sizeof(state->status), "Could not send question");
        return false;
    }

    state->active_request_id = id;
    state->waiting = true;
    state->wait_ticks = 0;
    state->answer[0] = '\0';
    state->answer_len = 0;
    state->scroll = 0;
    recent_view = -1;
    snprintf(state->status, sizeof(state->status), "Question sent");
    return true;
}

int main(void)
{
    app_state_t state = {
        .subject = 0,
        .mode = 1,
        .level = 0,
        .scroll = 0,
        .bridge_ready = false,
        .waiting = false,
        .focus_answer = false,
        .active_request_id = 0,
        .next_request_id = 1,
        .wait_ticks = 0,
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

        ev = state.waiting ? pollevent() : getkey();

        if(state.waiting && ev.type == KEYEV_NONE) {
            state.wait_ticks++;
            if(state.wait_ticks >= RESPONSE_TIMEOUT_TICKS) {
                state.waiting = false;
                state.wait_ticks = 0;
                snprintf(state.status, sizeof(state.status), "Response timed out - F6 retry");
            }
            sleep_us_spin(8000);
            continue;
        }

        if(state.waiting && ev.type != KEYEV_DOWN) continue;

        if(ev.key == KEY_EXIT) {
            if(state.waiting) {
                state.waiting = false;
                state.wait_ticks = 0;
                snprintf(state.status, sizeof(state.status), "Local wait cancelled - F6 retry");
            }
            else if(state.focus_answer) {
                state.focus_answer = false;
            }
            else {
                break;
            }
        }
        else if(!state.waiting && ev.key == KEY_F1) {
            state.subject = qb_picker_select(
                "SELECT SUBJECT",
                subject_labels,
                SUBJECT_COUNT,
                state.subject
            );
        }
        else if(!state.waiting && ev.key == KEY_F2) {
            state.mode = qb_picker_select(
                "SELECT TUTOR MODE",
                mode_labels,
                MODE_COUNT,
                state.mode
            );
        }
        else if(!state.waiting && ev.key == KEY_OPTN) {
            state.level = qb_picker_select(
                "ACADEMIC LEVEL",
                level_labels,
                LEVEL_COUNT,
                state.level
            );
            snprintf(state.status, sizeof(state.status),
                     "Academic level: %s", level_labels[state.level]);
        }
        else if(ev.key == KEY_F3) {
            if(state.answer[0] || state.waiting) {
                state.focus_answer = !state.focus_answer;
                state.scroll = 0;
            }
        }
        else if(!state.waiting && ev.key == KEY_F4) {
            qb_reference_show(
                subjects[state.subject],
                subject_labels[state.subject]
            );
        }
        else if(!state.waiting && ev.key == KEY_F5) {
            qb_diagnostics_show(&state.bridge_ready);
            snprintf(state.status, sizeof(state.status),
                     state.bridge_ready ? "Bridge linked" : "Bridge not found");
        }
        else if(!state.waiting && ev.key == KEY_F6 && state.prompt[0]) {
            start_request(&state);
        }
        else if(!state.waiting && ev.key == KEY_EXE) {
            char draft[PROMPT_CAP];
            draft[0] = '\0';

            if(qb_editor_read(draft, sizeof(draft))) {
                strncpy(state.prompt, draft, sizeof(state.prompt) - 1);
                state.prompt[sizeof(state.prompt) - 1] = '\0';
                state.focus_answer = false;
                start_request(&state);
            }
        }
        else if(!state.waiting && ev.key == KEY_LEFT && recent_count > 0) {
            int next = recent_view < 0 ? 0 : recent_view + 1;
            if(next >= recent_count) next = recent_count - 1;
            load_history_turn(&state, next);
        }
        else if(!state.waiting && ev.key == KEY_RIGHT && recent_count > 0) {
            int next = recent_view < 0 ? 0 : recent_view - 1;
            if(next < 0) next = 0;
            load_history_turn(&state, next);
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
