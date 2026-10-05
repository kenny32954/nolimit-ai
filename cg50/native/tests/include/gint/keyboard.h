#pragma once

typedef struct {
    int key;
    int type;
} key_event_t;

#define KEYEV_NONE 0
#define KEYEV_DOWN 1

enum {
    KEY_EXIT=1, KEY_EXE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_OPTN, KEY_ALPHA, KEY_SHIFT, KEY_DEL,
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
    KEY_DOT, KEY_XOT, KEY_LOG, KEY_LN, KEY_SIN, KEY_COS, KEY_TAN,
    KEY_FRAC, KEY_FD, KEY_LEFTP, KEY_RIGHTP, KEY_COMMA, KEY_ARROW,
    KEY_MUL, KEY_DIV, KEY_ADD, KEY_SUB, KEY_POWER
};

key_event_t getkey(void);
key_event_t pollevent(void);
