import tempfile
import unittest
from pathlib import Path

from dialogue_trial import Trial, initial_state, unsupported_claims


class ReplyGuardTests(unittest.TestCase):
    def test_rejects_invented_missing_interlock(self):
        world = initial_state()['world']
        world['patch_repaired'] = True
        world['power'] = True
        self.assertEqual(['patch_repaired'], unsupported_claims(
            "Patch's inspection capability hasn't been confirmed.", world))
        self.assertEqual(['power'], unsupported_claims('Power is still off.', world))

    def test_checks_matching_outcome_not_any_success(self):
        world = initial_state()['world']
        world['brack_actuator_lent'] = True
        self.assertEqual(['archive_open'], unsupported_claims('Actuator transferred. The archive is open.', world))

    def test_normal_dialogue_and_refusal(self):
        world = initial_state()['world']
        for text in ['Hello, Teapot.', 'I cannot open the archive.', 'My leg is not repaired.',
                     'Your mother was a shopping cart.', "I'll follow you once it's fixed.",
                     "Install the actuator, then I'll follow you."]:
            self.assertEqual([], unsupported_claims(text, world))

    def test_retry_then_fallback_does_not_remember_rejected_claim(self):
        class Liar:
            native_tools = True
            calls = 0

            def chat(self, messages, npc, allow_tools=True):
                self.calls += 1
                return {'role': 'assistant', 'content': "I'll lend it."}, {}

        with tempfile.TemporaryDirectory(prefix='latchpoint-guard-') as directory:
            client = Liar()
            trial = Trial(Path(directory) / 'save.json', client)
            turn = trial.talk('brack', 'Lend me an actuator.')
            self.assertEqual(3, client.calls)
            self.assertEqual(3, len(turn['guard_events']))
            self.assertEqual([], trial.data['world']['inventory'])
            self.assertNotIn("I'll lend", str(trial.data['conversations']))
            self.assertIn('cannot confirm', turn['reply'])

    def test_retry_can_call_tool_and_release_confirmed_speech(self):
        class Corrects:
            native_tools = True
            calls = 0
            retry_messages = None

            def chat(self, messages, npc, allow_tools=True):
                self.calls += 1
                if self.calls == 1:
                    return {'role': 'assistant', 'content': 'Actuator transferred.'}, {}
                if self.calls == 2:
                    self.retry_messages = messages
                    return {'role': 'assistant', 'content': '', 'tool_calls': [{
                        'id': 'loan', 'function': {'name': 'lend_actuator', 'arguments': '{}'}}]}, {}
                return {'role': 'assistant', 'content': 'Actuator transferred.'}, {}

        with tempfile.TemporaryDirectory(prefix='latchpoint-guard-') as directory:
            trial = Trial(Path(directory) / 'save.json', Corrects())
            trial.data['conversations']['brack'].append([
                {'role': 'user', 'content': 'Old conversation.'},
                {'role': 'assistant', 'content': 'An obsolete state claim.'}])
            turn = trial.talk('brack', 'Lend me an actuator.')
            self.assertEqual(1, len(turn['guard_events']))
            self.assertEqual(['actuator'], trial.data['world']['inventory'])
            self.assertEqual('Actuator transferred.', turn['reply'])
            self.assertNotIn('An obsolete state claim.', str(trial.client.retry_messages))
            self.assertIn('An obsolete state claim.', str(trial.data['conversations']))


if __name__ == '__main__':
    unittest.main()
