#include "transport.h"

/*
 * UI-first transport stub.
 *
 * This keeps the native app buildable while the hardware serial implementation
 * is developed and tested. The public interface is already shaped so the real
 * serial transport can drop in without rewriting the UI.
 */

static bool ready = false;

void qb_transport_init(void)
{
    ready = false;
}

void qb_transport_close(void)
{
    ready = false;
}

bool qb_transport_ready(void)
{
    return ready;
}

bool qb_transport_probe(void)
{
    /*
     * TODO native milestone 2:
     * - configure CG50 3-pin serial link
     * - send a small HELLO frame
     * - wait for bridge acknowledgement
     * - set ready=true on success
     */
    return ready;
}
