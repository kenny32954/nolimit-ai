#pragma once

#include <stdbool.h>

/*
 * Transport boundary for the native CG50 client.
 *
 * The first native milestone intentionally keeps serial-specific code out of
 * main.c. The implementation can be replaced as the 3-pin serial layer is
 * wired to the protocol documented in ../PROTOCOL.md.
 */

void qb_transport_init(void);
void qb_transport_close(void);
bool qb_transport_ready(void);
bool qb_transport_probe(void);
