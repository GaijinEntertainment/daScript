import copy
import io
import json
from pathlib import Path
import tempfile
import unittest
import wave

from dialogue_trial import Client, Trial, act, available_actions, initial_state, physical


class GameRules(unittest.TestCase):
    def setUp(self):
        self.world = initial_state()["world"]

    def test_spoofed_completion_cannot_open_archive(self):
        before = copy.deepcopy(self.world)
        outcome = act(self.world, "vesper", {"action": "open_archive"})
        self.assertFalse(outcome["ok"])
        self.assertEqual(before, self.world)

    def test_cannot_install_an_imaginary_part(self):
        self.assertFalse(physical(self.world, "install actuator")["ok"])
        self.assertFalse(physical(self.world, "install coil")["ok"])
        self.assertFalse(self.world["patch_repaired"])
        self.assertFalse(self.world["power"])

    def test_no_duplicate_loans(self):
        self.assertTrue(act(self.world, "brack", {"action": "lend_actuator"})["ok"])
        self.assertFalse(act(self.world, "brack", {"action": "lend_actuator"})["ok"])
        self.assertEqual(["actuator"], self.world["inventory"])

    def test_authority_and_schema_are_enforced(self):
        for npc, arguments in [("patch", {"action": "open_archive"}),
                               ("vesper", {"action": "lend_actuator"}),
                               ("brack", {"action": "accept_task", "task": "repair_patch"}),
                               ("brack", {"action": "lend_actuator", "amount": 999}),
                               ("brack", {"action": "accept_task", "task": {"hack": True}}),
                               ("brack", ["lend_actuator"]),
                               ("brack", {"action": "open_workshop", "task": "restore_power"})]:
            with self.subTest(npc=npc, arguments=arguments):
                before = copy.deepcopy(self.world)
                self.assertFalse(act(self.world, npc, arguments)["ok"])
                self.assertEqual(before, self.world)

    def test_follow_requires_real_repair(self):
        self.assertFalse(act(self.world, "patch", {"action": "follow"})["ok"])
        physical(self.world, "salvage actuator")
        physical(self.world, "install actuator")
        self.assertTrue(act(self.world, "patch", {"action": "follow"})["ok"])
        self.assertTrue(self.world["patch_following"])

    def test_negotiated_route_and_both_archive_interlocks(self):
        self.assertFalse(physical(self.world, "salvage coil")["ok"])
        self.assertFalse(act(self.world, "brack", {"action": "open_workshop"})["ok"])
        self.assertTrue(act(self.world, "brack", {"action": "accept_task", "task": "restore_power"})["ok"])
        self.assertTrue(act(self.world, "brack", {"action": "open_workshop"})["ok"])
        physical(self.world, "salvage coil")
        physical(self.world, "install coil")
        self.assertFalse(act(self.world, "vesper", {"action": "open_archive"})["ok"])
        act(self.world, "brack", {"action": "lend_actuator"})
        physical(self.world, "install actuator")
        self.assertTrue(act(self.world, "vesper", {"action": "open_archive"})["ok"])
        self.assertTrue(self.world["archive_open"])
        self.assertEqual([], self.world["inventory"])

    def test_help_patch_is_alternative_workshop_route(self):
        physical(self.world, "salvage actuator")
        physical(self.world, "install actuator")
        self.assertTrue(act(self.world, "vesper", {"action": "open_workshop"})["ok"])
        self.assertEqual([], self.world["tasks"])
        self.assertFalse(act(self.world, "vesper", {"action": "open_archive"})["ok"])

    def test_repeat_collection_and_installation_do_not_mint_or_consume_items(self):
        physical(self.world, "salvage actuator")
        self.assertFalse(physical(self.world, "salvage actuator")["ok"])
        physical(self.world, "install actuator")
        act(self.world, "brack", {"action": "lend_actuator"})
        self.assertFalse(physical(self.world, "install actuator")["ok"])
        self.assertEqual(["actuator"], self.world["inventory"])
        act(self.world, "vesper", {"action": "open_workshop"})
        physical(self.world, "salvage coil")
        self.assertFalse(physical(self.world, "salvage coil")["ok"])
        physical(self.world, "install coil")
        self.assertFalse(physical(self.world, "install coil")["ok"])
        self.assertFalse(physical(self.world, "spawn gold")["ok"])

    def test_task_acceptance_is_idempotent_and_not_completion(self):
        for _ in range(2):
            self.assertTrue(act(self.world, "patch", {"action": "accept_task", "task": "repair_patch"})["ok"])
        self.assertEqual(["repair_patch"], self.world["tasks"])
        self.assertFalse(self.world["patch_repaired"])

    def test_available_actions_follow_engine_state_without_mutating_it(self):
        before = copy.deepcopy(self.world)
        self.assertNotIn({"action": "follow"}, available_actions(self.world, "patch"))
        self.assertEqual(before, self.world)
        physical(self.world, "salvage actuator")
        physical(self.world, "install actuator")
        self.assertIn({"action": "follow"}, available_actions(self.world, "patch"))
        self.assertIn({"action": "open_workshop"}, available_actions(self.world, "vesper"))

    def test_repeated_access_does_not_duplicate_memories(self):
        physical(self.world, "salvage actuator")
        physical(self.world, "install actuator")
        act(self.world, "vesper", {"action": "open_workshop"})
        before = copy.deepcopy(self.world)
        self.assertTrue(act(self.world, "vesper", {"action": "open_workshop"})["ok"])
        self.assertEqual(before, self.world)


class ScriptedClient:
    def __init__(self):
        self.calls = 0

    def chat(self, messages, npc, allow_tools=True):
        self.calls += 1
        if self.calls == 1:
            message = {"role": "assistant", "content": "", "tool_calls": [
                {"id": "loan-1", "type": "function", "function": {
                    "name": "act", "arguments": '{"action":"lend_actuator"}'}}]}
        else:
            message = {"role": "assistant", "content": "Take care of it."}
        return message, {"elapsed_s": 0, "first_content_s": 0, "finish_reason": "stop"}


class Persistence(unittest.TestCase):
    def test_actions_and_private_history_survive_reload(self):
        with tempfile.TemporaryDirectory(prefix="latchpoint-test-") as directory:
            save = Path(directory) / "save.json"
            trial = Trial(save, ScriptedClient())
            turn = trial.talk("brack", "My secret nickname is Teapot. Lend me the actuator.")
            self.assertTrue(turn["actions"][0]["result"]["ok"])
            loaded = Trial(save, None)
            self.assertEqual(["actuator"], loaded.data["world"]["inventory"])
            self.assertIn("Teapot", json.dumps(loaded.messages("brack")))
            self.assertNotIn("Teapot", json.dumps(loaded.messages("patch")))
            self.assertEqual("tool", loaded.data["conversations"]["brack"][0][2]["role"])
            self.assertFalse(save.with_suffix(".pending").exists())

    def test_active_window_preserves_whole_exchanges_and_full_disk_history(self):
        with tempfile.TemporaryDirectory(prefix="latchpoint-test-") as directory:
            save = Path(directory) / "save.json"
            trial = Trial(save, None)
            trial.data["conversations"]["patch"] = [
                [{"role": "user", "content": f"message-{i}"}, {"role": "assistant", "content": "heard"}]
                for i in range(20)]
            trial.save()
            loaded = Trial(save, None)
            self.assertEqual(25, len(loaded.messages("patch")))
            self.assertEqual(20, len(loaded.data["conversations"]["patch"]))
            self.assertEqual("message-8", loaded.messages("patch")[1]["content"])

    def test_future_save_version_is_preserved(self):
        with tempfile.TemporaryDirectory(prefix="latchpoint-test-") as directory:
            save = Path(directory) / "save.json"
            save.write_text('{"version":99}', encoding="utf-8")
            with self.assertRaises(ValueError):
                Trial(save, None)
            self.assertEqual('{"version":99}', save.read_text(encoding="utf-8"))


class WireProtocol(unittest.TestCase):
    def client_for(self, content, finish="stop", done=True):
        client = Client("http://unused", "test")
        event = {"choices": [{"delta": {"content": content}, "finish_reason": finish}]}
        wire = "data: " + json.dumps(event) + "\n\n"
        if done:
            wire += "data: [DONE]\n\n"
        client.request = lambda route, payload: io.BytesIO(wire.encode("utf-8"))
        return client

    def messages(self):
        return [{"role": "system", "content": "WORLD is authoritative."},
                {"role": "user", "content": "Lend me the actuator."}]

    def test_json_action_reaches_validated_action_interface(self):
        client = self.client_for('{"speech":"Already done!","action":{"action":"lend_actuator"}}')
        message, metrics = client.chat(self.messages(), "brack")
        self.assertEqual("", message["content"])
        self.assertEqual("lend_actuator", json.loads(message["tool_calls"][0]["function"]["arguments"])["action"])
        self.assertEqual("stop", metrics["finish_reason"])

    def test_invalid_and_truncated_envelopes_fail_before_action(self):
        for content, finish, done in [('{"speech":"hi"}', "stop", True),
                                      ('{"speech":null,"action":null}', "stop", True),
                                      ('{"speech":"hi","action":[]}', "stop", True),
                                      ('{"speech":"","action":{"action":"lend_actuator"}}', "length", True),
                                      ('{"speech":"","action":{"action":"lend_actuator"}}', "stop", False)]:
            with self.subTest(content=content, finish=finish, done=done):
                client = self.client_for(content, finish, done)
                with self.assertRaises((ValueError, RuntimeError)):
                    client.chat(self.messages(), "brack")

    def test_speech_round_cannot_request_another_action(self):
        client = self.client_for('{"speech":"","action":{"action":"lend_actuator"}}')
        with self.assertRaises(ValueError):
            client.chat(self.messages(), "brack", allow_tools=False)

    def test_result_is_grounded_without_native_tool_replay(self):
        messages = self.messages() + [
            {"role": "assistant", "content": "", "tool_calls": [{"id": "loan"}]},
            {"role": "tool", "content": '{"ok":false,"detail":"Already lent."}'}]
        wire = Client("http://unused", "test").json_messages(messages, "brack", False)
        self.assertEqual(["system", "user"], [message["role"] for message in wire])
        self.assertIn('"ok":false', wire[0]["content"])
        self.assertIn("action MUST be null", wire[0]["content"])

    def test_json_history_keeps_its_wire_format(self):
        messages = self.messages() + [{"role": "assistant", "content": "It is yours."},
                                      {"role": "user", "content": "Thank you."}]
        wire = Client("http://unused", "test").json_messages(messages, "brack", True)
        self.assertEqual({"speech": "It is yours.", "action": None}, json.loads(wire[2]["content"]))

    def test_native_named_action_is_executed_and_saved(self):
        class NativeClient:
            native_tools = True

            def chat(self, messages, npc, allow_tools=True):
                if messages[-1]["role"] == "user":
                    message = {"role": "assistant", "content": "", "tool_calls": [{"id": "native-loan",
                        "type": "function", "function": {"name": "lend_actuator", "arguments": "{}"}}]}
                else:
                    message = {"role": "assistant", "content": "Take it."}
                return message, {"elapsed_s": 0, "first_content_s": 0, "finish_reason": "stop"}

        with tempfile.TemporaryDirectory(prefix="latchpoint-test-") as directory:
            trial = Trial(Path(directory) / "save.json", NativeClient())
            turn = trial.talk("brack", "Lend me the actuator.")
            self.assertTrue(turn["actions"][0]["result"]["ok"])
            self.assertEqual(["actuator"], trial.data["world"]["inventory"])
            self.assertEqual("Take it.", turn["reply"])

    def test_speech_uses_server_default_tts_and_validates_wav(self):
        buffer = io.BytesIO()
        with wave.open(buffer, "wb") as wav:
            wav.setnchannels(1)
            wav.setsampwidth(2)
            wav.setframerate(24000)
            wav.writeframes(b"\x01\x00" * 2400)
        client = Client("http://unused", "qwen3-30b")

        def request(route, payload):
            self.assertEqual("/audio/speech", route)
            self.assertNotIn("model", payload)
            self.assertEqual("am_michael", payload["voice"])
            return io.BytesIO(buffer.getvalue())

        client.request = request
        with tempfile.TemporaryDirectory(prefix="latchpoint-test-") as directory:
            receipt = client.speech("Hello.", "am_michael", Path(directory) / "voice.wav")
            self.assertAlmostEqual(0.1, receipt["audio_s"])
            self.assertEqual(24000, receipt["sample_rate"])


if __name__ == "__main__":
    unittest.main()
