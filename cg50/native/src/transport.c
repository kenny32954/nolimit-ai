#include "transport.h"
#include "base64.h"

#include <gint/clock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Serial_Open(unsigned char *mode);
int Serial_IsOpen(void);
int Serial_Close(int mode);
int Serial_ReadSingle(unsigned char *out);
int Serial_Write(unsigned char const *buf, int count);
int Serial_PollRX(void);
int Serial_PollTX(void);
int Serial_ClearRX(void);
int Serial_ClearTX(void);

#define RX_LINE_CAP 1024
#define TX_FRAME_CAP 1400
#define B64_CAP 1100

static bool ready = false;
static char rx_line[RX_LINE_CAP];
static size_t rx_len = 0;
static uint32_t active_request_id = 0;
static unsigned long expected_seq = 0;

static bool serial_send(char const *text)
{
    size_t len = strlen(text);
    size_t offset = 0;

    while(offset < len) {
        int free_bytes = Serial_PollTX();
        if(free_bytes <= 0) {
            sleep_us_spin(1000);
            continue;
        }

        size_t remaining = len - offset;
        int chunk = (int)(remaining < (size_t)free_bytes ? remaining : (size_t)free_bytes);
        if(chunk <= 0) chunk = 1;

        if(Serial_Write((unsigned char const *)(text + offset), chunk) < 0) {
            return false;
        }

        offset += (size_t)chunk;
    }

    return true;
}

static bool read_line(char *out, size_t out_size)
{
    while(Serial_PollRX() > 0) {
        unsigned char c = 0;

        if(Serial_ReadSingle(&c) != 0) {
            break;
        }

        if(c == '\r') continue;

        if(c == '\n') {
            if(rx_len >= out_size) rx_len = out_size - 1;
            memcpy(out, rx_line, rx_len);
            out[rx_len] = '\0';
            rx_len = 0;
            return true;
        }

        if(rx_len + 1 < RX_LINE_CAP) {
            rx_line[rx_len++] = (char)c;
        }
        else {
            /* Drop an oversized frame cleanly. */
            rx_len = 0;
        }
    }

    return false;
}

static char *next_field(char **cursor)
{
    char *start;
    char *sep;

    if(!cursor || !*cursor) return NULL;

    start = *cursor;
    sep = strchr(start, ':');

    if(sep) {
        *sep = '\0';
        *cursor = sep + 1;
    }
    else {
        *cursor = NULL;
    }

    return start;
}

void qb_transport_init(void)
{
    unsigned char mode[6] = {0, 9, 0, 0, 0, 0}; /* 115200 baud, 8N1 */

    rx_len = 0;
    ready = false;
    active_request_id = 0;
    expected_seq = 0;

    if(Serial_Open(mode) == 0 || Serial_IsOpen()) {
        Serial_ClearRX();
        Serial_ClearTX();
    }
}

void qb_transport_close(void)
{
    if(Serial_IsOpen()) {
        Serial_Close(1);
    }

    rx_len = 0;
    ready = false;
    active_request_id = 0;
    expected_seq = 0;
}

bool qb_transport_ready(void)
{
    return ready && Serial_IsOpen();
}

bool qb_transport_probe(void)
{
    char line[128];
    int i;

    if(!Serial_IsOpen()) {
        qb_transport_init();
    }

    if(!Serial_IsOpen()) {
        ready = false;
        return false;
    }

    Serial_ClearRX();
    rx_len = 0;

    if(!serial_send("H:QBAI:2\n")) {
        ready = false;
        return false;
    }

    for(i = 0; i < 500; i++) {
        if(read_line(line, sizeof(line))) {
            if(strcmp(line, "K:QBAI:2") == 0) {
                ready = true;
                return true;
            }
        }

        sleep_us_spin(2000);
    }

    ready = false;
    return false;
}

bool qb_transport_send_request(
    uint32_t request_id,
    char const *subject,
    char const *mode,
    char const *prompt
)
{
    char encoded[B64_CAP];
    char frame[TX_FRAME_CAP];
    size_t prompt_len = strlen(prompt);

    if(!qb_transport_ready()) return false;

    if(qb_base64_encode(
        (unsigned char const *)prompt,
        prompt_len,
        encoded,
        sizeof(encoded)
    ) == 0) {
        return false;
    }

    if(snprintf(
        frame,
        sizeof(frame),
        "Q:%lu:%s:%s:%s\n",
        (unsigned long)request_id,
        subject,
        mode,
        encoded
    ) >= (int)sizeof(frame)) {
        return false;
    }

    if(!serial_send(frame)) return false;

    active_request_id = request_id;
    expected_seq = 0;
    return true;
}

qb_transport_event_t qb_transport_poll(
    uint32_t request_id,
    char *text,
    size_t text_size
)
{
    char line[RX_LINE_CAP];
    char *cursor;
    char *kind;
    char *id_text;
    char *seq_text;
    char *done_text;
    char *payload;
    unsigned long frame_id;
    unsigned long seq;
    size_t decoded;

    if(text_size == 0) return QB_TRANSPORT_NONE;
    text[0] = '\0';

    if(!read_line(line, sizeof(line))) {
        return QB_TRANSPORT_NONE;
    }

    cursor = line;
    kind = next_field(&cursor);
    id_text = next_field(&cursor);

    if(!kind || !id_text) return QB_TRANSPORT_NONE;

    frame_id = strtoul(id_text, NULL, 10);
    if(frame_id != (unsigned long)request_id ||
       frame_id != (unsigned long)active_request_id) {
        return QB_TRANSPORT_NONE;
    }

    if(strcmp(kind, "E") == 0) {
        payload = cursor;
        if(!payload || !*payload) {
            snprintf(text, text_size, "Malformed bridge error frame");
            return QB_TRANSPORT_ERROR;
        }

        decoded = qb_base64_decode(payload, (unsigned char *)text, text_size - 1);
        if(decoded == 0 && payload[0]) {
            snprintf(text, text_size, "Could not decode bridge error");
            return QB_TRANSPORT_ERROR;
        }

        text[decoded] = '\0';
        active_request_id = 0;
        expected_seq = 0;
        return QB_TRANSPORT_ERROR;
    }

    if(strcmp(kind, "A") != 0) {
        return QB_TRANSPORT_NONE;
    }

    seq_text = next_field(&cursor);
    done_text = next_field(&cursor);
    payload = cursor;

    if(!seq_text || !done_text || !payload) {
        snprintf(text, text_size, "Malformed answer frame");
        active_request_id = 0;
        expected_seq = 0;
        return QB_TRANSPORT_ERROR;
    }

    seq = strtoul(seq_text, NULL, 10);
    if(seq != expected_seq) {
        snprintf(text, text_size, "Serial sequence mismatch");
        active_request_id = 0;
        expected_seq = 0;
        return QB_TRANSPORT_ERROR;
    }

    decoded = qb_base64_decode(payload, (unsigned char *)text, text_size - 1);
    if(decoded == 0 && payload[0]) {
        snprintf(text, text_size, "Could not decode answer frame");
        active_request_id = 0;
        expected_seq = 0;
        return QB_TRANSPORT_ERROR;
    }

    text[decoded] = '\0';
    expected_seq++;

    if(done_text[0] == '1') {
        active_request_id = 0;
        expected_seq = 0;
        return QB_TRANSPORT_DONE;
    }

    return QB_TRANSPORT_CHUNK;
}
