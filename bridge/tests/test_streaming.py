import base64
import unittest

import qb_bridge


class FakeSerial:
    def __init__(self):
        self.data = bytearray()

    def write(self, data):
        self.data.extend(data)
        return len(data)

    def flush(self):
        pass

    def lines(self):
        return bytes(self.data).decode("ascii").splitlines()


def sample_stream():
    yield "Linear "
    yield "algebra "
    yield "answer with π and vectors."
    return (
        "linear_algebra",
        "explain",
        "college",
        ("linear_algebra", "college"),
        [],
        "Linear algebra answer with π and vectors.",
    )


class StreamingBridgeTests(unittest.TestCase):
    def test_streaming_answer_frames_are_ordered_and_finalized(self):
        old = qb_bridge.CHUNK_BYTES
        qb_bridge.CHUNK_BYTES = 12

        try:
            ser = FakeSerial()
            metadata = qb_bridge.send_streaming_answer(
                ser, "77", sample_stream()
            )

            lines = ser.lines()
            self.assertGreater(len(lines), 1)

            rebuilt = bytearray()
            for index, line in enumerate(lines):
                kind, req_id, seq, done, payload = line.split(":", 4)
                self.assertEqual(kind, "A")
                self.assertEqual(req_id, "77")
                self.assertEqual(int(seq), index)
                self.assertEqual(done, "1" if index == len(lines) - 1 else "0")
                rebuilt.extend(base64.b64decode(payload))

            self.assertEqual(
                rebuilt.decode("utf-8"),
                "Linear algebra answer with π and vectors.",
            )
            self.assertEqual(metadata[0], "linear_algebra")
            self.assertEqual(metadata[2], "college")
        finally:
            qb_bridge.CHUNK_BYTES = old

    def test_empty_final_frame_is_valid(self):
        def exact_stream():
            yield "123456789012"
            return ("math", "answer", "school", ("math", "school"), [], "123456789012")

        old = qb_bridge.CHUNK_BYTES
        qb_bridge.CHUNK_BYTES = 12
        try:
            ser = FakeSerial()
            qb_bridge.send_streaming_answer(ser, "5", exact_stream())
            lines = ser.lines()
            self.assertEqual(lines[-1].split(":", 4)[3], "1")
        finally:
            qb_bridge.CHUNK_BYTES = old


if __name__ == "__main__":
    unittest.main()
