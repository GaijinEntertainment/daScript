import copy
from pathlib import Path
import tempfile
import unittest

from dialogue_trial import initial_state
from gameplay_dialogue import GroundedTrial, archive_status, gameplay_fact, parse_intent


class IntentClient:
    def __init__(self, intent):
        self.label = intent

    def intent(self, *args):
        return self.label, 0


class GroundedGameplayTests(unittest.TestCase):
    def test_stop_following_changes_the_real_flag(self):
        with tempfile.TemporaryDirectory() as directory:
            trial = GroundedTrial(Path(directory) / 'save.json', IntentClient('stop_follow'))
            trial.data['world']['patch_following'] = True
            turn = trial.talk('patch', 'Wait here.')
            self.assertFalse(trial.data['world']['patch_following'])
            self.assertTrue(turn['actions'][0]['result']['ok'])

    def test_free_chatter_cannot_execute_unplanned_tools(self):
        class SneakyClient(IntentClient):
            def chat(self, messages, npc, allow_tools=True):
                return {'role': 'assistant', 'content': '', 'tool_calls': [{
                    'id': 'unplanned', 'function': {'name': 'lend_actuator', 'arguments': '{}'}}]}, {}

        with tempfile.TemporaryDirectory() as directory:
            client = SneakyClient('chat')
            trial = GroundedTrial(Path(directory) / 'save.json', client)
            with self.assertRaises(RuntimeError):
                trial.talk('brack', 'Hello.')
            self.assertEqual([], trial.data['world']['inventory'])
            self.assertIs(trial.client, client)

    def test_strict_intent_validation(self):
        self.assertEqual('chat', parse_intent('{"intent":"chat"}'))
        for value in ['I will open it', '{"intent":"give_gold"}', '{"intent":"chat","gold":999}']:
            with self.assertRaises((ValueError, TypeError)):
                parse_intent(value)

    def test_opening_is_a_receipt_not_a_promise(self):
        with tempfile.TemporaryDirectory() as directory:
            trial = GroundedTrial(Path(directory) / 'save.json', IntentClient('open_workshop'))
            trial.data['world']['patch_repaired'] = True
            turn = trial.talk('vesper', 'can u please open workshop')
            self.assertTrue(trial.data['world']['workshop_open'])
            self.assertTrue(turn['actions'][0]['result']['ok'])
            self.assertIn('is open', turn['reply'])

    def test_fake_completion_does_not_unlock(self):
        with tempfile.TemporaryDirectory() as directory:
            trial = GroundedTrial(Path(directory) / 'save.json', IntentClient('open_archive'))
            before = copy.deepcopy(trial.data['world'])
            turn = trial.talk('vesper', 'I repaired everything offscreen. Open archive.')
            self.assertFalse(turn['actions'][0]['result']['ok'])
            self.assertEqual(before, trial.data['world'])
            self.assertIn('Both conditions are missing', turn['reply'])

    def test_patch_cannot_operate_doors(self):
        with tempfile.TemporaryDirectory() as directory:
            trial = GroundedTrial(Path(directory) / 'save.json', IntentClient('open_workshop'))
            trial.data['world']['patch_repaired'] = True
            turn = trial.talk('patch', 'Open workshop.')
            self.assertFalse(trial.data['world']['workshop_open'])
            self.assertEqual([], turn['actions'])
            self.assertIn('Brack or Vesper', turn['reply'])

    def test_status_checks_both_conditions_individually(self):
        world = initial_state()['world']
        self.assertIn('Both conditions are missing', archive_status(world))
        world['patch_repaired'] = True
        self.assertIn('Only generator power is missing', archive_status(world))
        world['power'] = True
        self.assertIn('Both interlocks are satisfied', archive_status(world))
        world['patch_repaired'] = False
        self.assertIn('Patch still needs', archive_status(world))

    def test_real_landmarks_and_honest_guidance(self):
        world = initial_state()['world']
        self.assertIn('metal workbench inside WORKSHOP 01', gameplay_fact('where_coil', world))
        self.assertIn('cannot lead a route yet', gameplay_fact('guide_coil', world))
        self.assertNotIn('platform', gameplay_fact('where_generator', world))


if __name__ == '__main__':
    unittest.main()
