#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    QB_TRANSPORT_NONE = 0,
    QB_TRANSPORT_CHUNK,
    QB_TRANSPORT_DONE,
    QB_TRANSPORT_ERROR
} qb_transport_event_t;

void qb_transport_init(void);
void qb_transport_close(void);
bool qb_transport_ready(void);
bool qb_transport_serial_open(void);
bool qb_transport_probe(void);

bool qb_transport_send_request(
    uint32_t request_id,
    char const *subject,
    char const *mode,
    char const *level,
    char const *prompt
);

/*
 * Poll serial input once. When a complete protocol frame is available:
 * - CHUNK: text contains decoded response text for this chunk.
 * - DONE:  text can contain the final decoded chunk.
 * - ERROR: text contains a decoded bridge error.
 * Returns QB_TRANSPORT_NONE when no complete frame is ready.
 */
qb_transport_event_t qb_transport_poll(
    uint32_t request_id,
    char *text,
    size_t text_size
);
