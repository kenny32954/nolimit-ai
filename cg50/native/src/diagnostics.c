#include "diagnostics.h"
#include "transport.h"

#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdbool.h>

static void draw_state(bool serial_open, bool linked, bool probing)
{
    color_t bg = C_RGB(3, 3, 4);
    color_t panel = C_RGB(6, 6, 8);
    color_t text = C_RGB(29, 29, 30);
    color_t muted = C_RGB(18, 18, 20);
    color_t good = C_RGB(8, 24, 13);
    color_t bad = C_RGB(27, 10, 10);
    color_t accent = C_RGB(28, 11, 8);

    dclear(bg);
    drect(0, 0, DWIDTH - 1, 31, panel);
    dtext(10, 8, text, "CONNECTION DIAGNOSTICS");

    dtext(14, 48, muted, "SERIAL INTERFACE");
    dtext(220, 48, serial_open ? good : bad, serial_open ? "OPEN" : "CLOSED");

    dtext(14, 72, muted, "DESKTOP BRIDGE");
    dtext(220, 72, probing ? accent : (linked ? good : bad),
          probing ? "PROBING..." : (linked ? "LINKED" : "NOT FOUND"));

    dtext(14, 96, muted, "PROTOCOL");
    dtext(220, 96, text, "QBAI v2");

    dtext(14, 120, muted, "SERIAL SPEED");
    dtext(220, 120, text, "115200 8N1");

    dtext(14, 146, muted, "API KEY");
    dtext(220, 146, good, "STAYS ON PC");

    dtext(14, 170, muted, "EXE = RETRY HANDSHAKE");
    dtext(14, 190, muted, "EXIT = BACK");
    dupdate();
}

void qb_diagnostics_show(bool *bridge_ready)
{
    bool linked = bridge_ready ? *bridge_ready : false;

    while(1) {
        key_event_t ev;

        draw_state(qb_transport_serial_open(), linked, false);
        ev = getkey();

        if(ev.key == KEY_EXIT) {
            if(bridge_ready) *bridge_ready = linked;
            return;
        }

        if(ev.key == KEY_EXE) {
            draw_state(qb_transport_serial_open(), linked, true);
            linked = qb_transport_probe();
            if(bridge_ready) *bridge_ready = linked;
        }
    }
}
