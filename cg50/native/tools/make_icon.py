#!/usr/bin/env python3
"""Generate a tiny fx-CG50 Quantum Breaks AI PNG icon using stdlib only."""

import binascii
import struct
import sys
import zlib

W, H = 92, 64


def chunk(kind, data):
    return (
        struct.pack(">I", len(data))
        + kind
        + data
        + struct.pack(">I", binascii.crc32(kind + data) & 0xFFFFFFFF)
    )


def inside_ring(x, y, cx, cy, outer, inner):
    d2 = (x - cx) * (x - cx) + (y - cy) * (y - cy)
    return inner * inner <= d2 <= outer * outer


def pixel(x, y):
    # Dark Quantum Breaks palette.
    bg = (16, 16, 20)
    panel = (31, 31, 39)
    accent = (224, 123, 95)
    glow = (116, 74, 67)
    white = (245, 243, 239)

    # Soft panel.
    if 4 <= x < W - 4 and 4 <= y < H - 4:
        base = panel
    else:
        base = bg

    # Q glow/ring.
    if inside_ring(x, y, 45, 31, 20, 14):
        return accent
    if inside_ring(x, y, 45, 31, 23, 20):
        return glow

    # Q tail.
    if 56 <= x <= 69 and 41 <= y <= 48 and (x - y) >= 12:
        return accent

    # Tiny highlight.
    if 39 <= x <= 51 and 23 <= y <= 26:
        return white

    return base


def write_png(path):
    rows = []
    for y in range(H):
        row = bytearray([0])
        for x in range(W):
            row.extend(pixel(x, y))
        rows.append(bytes(row))

    raw = b"".join(rows)
    png = b"\x89PNG\r\n\x1a\n"
    png += chunk(b"IHDR", struct.pack(">IIBBBBB", W, H, 8, 2, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(raw, 9))
    png += chunk(b"IEND", b"")

    with open(path, "wb") as f:
        f.write(png)


if __name__ == "__main__":
    output = sys.argv[1] if len(sys.argv) > 1 else "src/icon.png"
    write_png(output)
    print("Generated", output)
