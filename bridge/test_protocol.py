import base64
import os
import sys
import unittest

HERE = os.path.dirname(__file__)
sys.path.insert(0, HERE)

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


class ProtocolTests(unittest.TestCase):
    def test_v3_handshake(self):
        ser = FakeSerial()
        qb_bridge.handle_line(ser, "H:QBAI:3")
        self.assertEqual(ser.lines(), ["K:QBAI:3"])

    def test_v3_college_request(self):
        payload = base64.b64encode(
            "Prove the sequence is Cauchy".encode()
        ).decode()
        req = qb_bridge.parse_request(
            "Q:7:real_analysis:proof:advanced:" + payload
        )
        self.assertEqual(req["id"], "7")
        self.assertEqual(req["subject"], "real_analysis")
        self.assertEqual(req["mode"], "proof")
        self.assertEqual(req["level"], "advanced")

    def test_v2_request_compatibility(self):
        payload = base64.b64encode("solve 2x+4=10".encode()).decode()
        req = qb_bridge.parse_request("Q:7:algebra:steps:" + payload)
        self.assertEqual(req["level"], "auto")

    def test_parse_legacy_request(self):
        payload = base64.b64encode("hello".encode()).decode()
        req = qb_bridge.parse_request("Q:2:" + payload)
        self.assertEqual(req["subject"], "auto")
        self.assertEqual(req["mode"], "explain")
        self.assertEqual(req["level"], "auto")
        self.assertEqual(req["prompt"], "hello")

    def test_utf8_chunking_does_not_split_characters(self):
        text = "alpha π beta 日本語 gamma"
        chunks = list(qb_bridge.utf8_chunks(text, 8))
        rebuilt = b"".join(chunks).decode("utf-8")
        self.assertEqual(rebuilt, text)
        self.assertTrue(all(len(chunk) <= 8 for chunk in chunks))

    def test_answer_frames_round_trip(self):
        old = qb_bridge.CHUNK_BYTES
        qb_bridge.CHUNK_BYTES = 8
        try:
            ser = FakeSerial()
            text = "Force = 6 N. π"
            qb_bridge.send_answer(ser, "42", text)

            pieces = []
            lines = ser.lines()
            self.assertGreater(len(lines), 1)

            for index, line in enumerate(lines):
                kind, req_id, seq, done, payload = line.split(":", 4)
                self.assertEqual(kind, "A")
                self.assertEqual(req_id, "42")
                self.assertEqual(int(seq), index)
                self.assertEqual(int(done), 1 if index == len(lines) - 1 else 0)
                pieces.append(base64.b64decode(payload))

            self.assertEqual(b"".join(pieces).decode("utf-8"), text)
        finally:
            qb_bridge.CHUNK_BYTES = old

    def test_malformed_request_rejected(self):
        with self.assertRaises(RuntimeError):
            qb_bridge.parse_request("Q:1:math:steps")


if __name__ == "__main__":
    unittest.main()
