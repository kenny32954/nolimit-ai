import base64
import unittest

import qb_bridge
from subject_router import normalize_level, resolve_subject


class FakeSerial:
    def __init__(self):
        self.lines = []

    def write(self, data):
        self.lines.append(data)
        return len(data)

    def flush(self):
        pass


class BridgeProtocolTests(unittest.TestCase):
    def test_v3_college_request(self):
        prompt = "Find the eigenvalues of this matrix"
        payload = base64.b64encode(prompt.encode()).decode()
        req = qb_bridge.parse_request(
            "Q:21:linear_algebra:derive:college:" + payload
        )
        self.assertEqual(req["id"], "21")
        self.assertEqual(req["subject"], "linear_algebra")
        self.assertEqual(req["mode"], "derive")
        self.assertEqual(req["level"], "college")
        self.assertEqual(req["prompt"], prompt)

    def test_v2_request_defaults_to_auto_level(self):
        prompt = "Solve 2x + 4 = 10"
        payload = base64.b64encode(prompt.encode()).decode()
        req = qb_bridge.parse_request("Q:12:algebra:steps:" + payload)
        self.assertEqual(req["subject"], "algebra")
        self.assertEqual(req["mode"], "steps")
        self.assertEqual(req["level"], "auto")
        self.assertEqual(req["prompt"], prompt)

    def test_legacy_request(self):
        prompt = "What is a cell?"
        payload = base64.b64encode(prompt.encode()).decode()
        req = qb_bridge.parse_request("Q:9:" + payload)
        self.assertEqual(req["subject"], "auto")
        self.assertEqual(req["mode"], "explain")
        self.assertEqual(req["level"], "auto")
        self.assertEqual(req["prompt"], prompt)

    def test_utf8_chunks_round_trip(self):
        text = "Spanish: ¿Cómo estás? Physics: Δv → acceleration."
        chunks = list(qb_bridge.utf8_chunks(text, 12))
        self.assertEqual(b"".join(chunks).decode("utf-8"), text)
        self.assertTrue(all(len(chunk) <= 12 for chunk in chunks))

    def test_answer_frames_round_trip(self):
        serial = FakeSerial()
        text = "A short answer with café and Δ."
        qb_bridge.send_answer(serial, "4", text)

        payload = bytearray()
        for index, raw in enumerate(serial.lines):
            line = raw.decode("ascii").strip()
            kind, req_id, seq, done, encoded = line.split(":", 4)
            self.assertEqual(kind, "A")
            self.assertEqual(req_id, "4")
            self.assertEqual(int(seq), index)
            payload.extend(base64.b64decode(encoded))
            if index == len(serial.lines) - 1:
                self.assertEqual(done, "1")

        self.assertEqual(payload.decode("utf-8"), text)

    def test_v3_handshake(self):
        serial = FakeSerial()
        qb_bridge.handle_line(serial, "H:QBAI:3")
        self.assertEqual(serial.lines, [b"K:QBAI:3\n"])

    def test_v2_handshake_compatibility(self):
        serial = FakeSerial()
        qb_bridge.handle_line(serial, "H:QBAI:2")
        self.assertEqual(serial.lines, [b"K:QBAI:2\n"])

    def test_configured_serial_port(self):
        self.assertEqual(qb_bridge.resolve_serial_port("COM42"), "COM42")

    def test_subject_detection(self):
        self.assertEqual(
            resolve_subject("auto", "Find the eigenvector and basis of this matrix"),
            "linear_algebra",
        )
        self.assertEqual(
            resolve_subject("auto", "Compare SN1 and SN2 stereochemistry"),
            "organic_chemistry",
        )
        self.assertEqual(
            resolve_subject("auto", "Explain a thesis paragraph"),
            "writing",
        )

    def test_academic_levels(self):
        self.assertEqual(normalize_level("college"), "college")
        self.assertEqual(normalize_level("advanced"), "advanced")
        self.assertEqual(normalize_level("not-a-level"), "auto")


if __name__ == "__main__":
    unittest.main()
