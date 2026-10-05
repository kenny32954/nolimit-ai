#!/usr/bin/env python3
"""Generate fx-CG50 Quantum Breaks AI menu icons using stdlib only."""

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


def pixel(x, y, selected):
    if selected:
        bg = (38, 24, 22)
        panel = (60, 34, 29)
        accent = (255, 160, 130)
        glow = (190, 92, 70)
        white = (255, 249, 245)
    else:
        bg = (16, 16, 20)
        panel = (31, 31, 39)
        accent = (224, 123, 95)
        glow = (116, 74, 67)
        white = (245, 243, 239)

    base = panel if 4 <= x < W - 4 and 4 <= y < H - 4 else bg

    if inside_ring(x, y, 45, 31, 20, 14):
        return accent
    if inside_ring(x, y, 45, 31, 23, 20):
        return glow

    if 56 <= x <= 69 and 41 <= y <= 48 and (x - y) >= 12:
        return accent

    if 39 <= x <= 51 and 23 <= y <= 26:
        return white

    return base


def write_png(path, selected=False):
    rows = []
    for y in range(H):
        row = bytearray([0])
        for x in range(W):
            row.extend(pixel(x, y, selected))
        rows.append(bytes(row))

    raw = b"".join(rows)
    png = b"\x89PNG\r\n\x1a\n"
    png += chunk(b"IHDR", struct.pack(">IIBBBBB", W, H, 8, 2, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(raw, 9))
    png += chunk(b"IEND", b"")

    with open(path, "wb") as f:
        f.write(png)


if __name__ == "__main__":
    args = sys.argv[1:]
    selected = "--selected" in args
    args = [a for a in args if a != "--selected"]
    output = args[0] if args else "src/icon-uns.png"
    write_png(output, selected)
    print("Generated", output)
