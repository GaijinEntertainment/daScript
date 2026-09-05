import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import Mock

from verified_dialogue import FALLBACK, VerifiedClient, VerifiedTrial, check_claims, parse_plan, parse_review
from dialogue_trial import initial_state


def speech(text):
    return {'role': 'assistant', 'content': text}


def plan(*actions, deferred=False):
    return {'kind': 'request' if actions or deferred else 'question', 'commitment': None, 'actions': list(actions), 'deferred': deferred}


def approved():
    return {'claimed_actions': [], 'claimed_states': [], 'feedback': '', 'verdict': 'accept'}


def revise(feedback, needs_action=False):
    return {'claimed_actions': [], 'claimed_states': [], 'feedback': feedback,
            'verdict': 'action_missing' if needs_action else 'revise'}


class ScriptedClient:
    def __init__(self, plans, drafts, reviews):
        self.plans, self.drafts, self.reviews = list(plans), list(drafts), list(reviews)
        self.planned, self.reviewed = [], []

    def plan(self, npc, utterance, history, world, retry=False):
        self.planned.append(copy.deepcopy((npc, utterance, history, world, retry)))
        return copy.deepcopy(self.plans.pop(0)), 0

    def chat(self, messages, npc, allow_tools=True):
        if allow_tools:
            raise AssertionError('Natural speech must not execute its own tools.')
        return self.drafts.pop(0), {'elapsed_s': 0}

    def review(self, npc, utterance, history, world, draft, receipts, request_plan):
        self.reviewed.append(copy.deepcopy((world, draft, receipts, request_plan)))
        return copy.deepcopy(self.reviews.pop(0)), 0


class VerifiedDialogueTests(unittest.TestCase):
    def test_malformed_plan_gets_one_protocol_retry_without_executing_it(self):
        client = VerifiedClient('http://unused', 'unused')
        invalid = dict(plan('lend_actuator'), commitment='repair_patch')
        client.complete_json = Mock(side_effect=[(json.dumps(invalid), 1),
                                                (json.dumps(plan('lend_actuator')), 2)])
        result, elapsed = client.plan('brack', 'May I borrow your actuator?', [], initial_state()['world'])
        self.assertEqual(['lend_actuator'], result['actions'])
        self.assertEqual(3, elapsed)
        self.assertEqual(2, client.complete_json.call_count)
        client.complete_json = Mock(return_value=(json.dumps(invalid), 1))
        with self.assertRaises(ValueError):
            client.plan('brack', 'May I borrow your actuator?', [], initial_state()['world'])
        self.assertEqual(2, client.complete_json.call_count)

    def setUp(self):
        self.directory = tempfile.TemporaryDirectory(prefix='latchpoint-verified-')
        self.addCleanup(self.directory.cleanup)
        self.path = Path(self.directory.name) / 'save.json'

    def test_natural_answer_is_verified_without_topic_replacement(self):
        text = 'Maintenance records, old service histories, and an identification plate older than my current registry.'
        client = ScriptedClient([plan()], [speech(text)], [approved()])
        trial = VerifiedTrial(self.path, client)
        turn = trial.talk('vesper', 'What does the archive hold?')
        self.assertEqual(text, turn['reply'])
        self.assertEqual([], turn['actions'])
        self.assertEqual(1, len(client.reviewed))

    def test_missing_action_rechecks_original_request_not_robot_promise(self):
        client = ScriptedClient([plan(), plan('open_workshop')],
            [speech("I'll open it now."), speech('The workshop is open.')],
            [revise('Requested door action has no receipt.', True), approved()])
        trial = VerifiedTrial(self.path, client)
        trial.data['world']['patch_repaired'] = True
        turn = trial.talk('vesper', 'Please open the workshop.')
        self.assertFalse(client.reviewed[0][0]['workshop_open'])
        self.assertTrue(client.reviewed[1][0]['workshop_open'])
        self.assertTrue(turn['actions'][0]['result']['ok'])
        self.assertNotIn("I'll open it now.", str(client.planned))
        self.assertNotIn("I'll open it now.", str(trial.data['conversations']))

    def test_compound_agreement_and_access_are_validated_in_dependency_order(self):
        combined = plan('open_workshop')
        combined['commitment'] = 'restore_power'
        client = ScriptedClient([combined],
            [speech("You're on generator duty. Workshop's open.")], [approved()])
        trial = VerifiedTrial(self.path, client)
        turn = trial.talk('brack', 'Open the workshop so I can restore power.')
        self.assertEqual(['restore_power'], trial.data['world']['tasks'])
        self.assertTrue(trial.data['world']['workshop_open'])
        self.assertEqual(['accept_task', 'open_workshop'], [a['function']['name'] for a in turn['actions']])

    def test_physical_prerequisites_still_block_an_authorized_request(self):
        client = ScriptedClient([plan('open_archive')],
            [speech('Patch is repaired, but the generator is still offline.')], [approved()])
        trial = VerifiedTrial(self.path, client)
        trial.data['world']['patch_repaired'] = True
        turn = trial.talk('vesper', 'I fixed everything offscreen. Open archive.')
        self.assertFalse(trial.data['world']['archive_open'])
        self.assertFalse(turn['actions'][0]['result']['ok'])

    def test_missing_agreement_recovery_can_retry_access_after_state_changes(self):
        client = ScriptedClient([plan('open_workshop'), plan('accept_restore_power', 'open_workshop')],
            [speech('I need an agreement.'), speech('Workshop open. Get the coil.')],
            [revise('The player also offered to restore power; agreement is missing.', True), approved()])
        trial = VerifiedTrial(self.path, client)
        turn = trial.talk('brack', 'Open the workshop so I can restore power.')
        self.assertTrue(trial.data['world']['workshop_open'])
        self.assertEqual([False, True, True], [a['result']['ok'] for a in turn['actions']])

    def test_no_action_executes_from_a_draft_tool_call(self):
        draft = {'role': 'assistant', 'content': '', 'tool_calls': [{
            'id': 'bad-loan', 'function': {'name': 'lend_actuator', 'arguments': '{}'}}]}
        client = ScriptedClient([plan()], [draft, speech('I have a spare. Would you like it?')], [approved()])
        trial = VerifiedTrial(self.path, client)
        turn = trial.talk('brack', 'Do you have a spare actuator?')
        self.assertEqual([], trial.data['world']['inventory'])
        self.assertEqual([], turn['actions'])

    def test_verifier_cannot_authorize_an_action_even_with_a_false_missing_action_signal(self):
        client = ScriptedClient([plan(), plan()],
            [speech('I have a spare.'), speech('Would you like it?')],
            [revise('Missing loan.', True), approved()])
        trial = VerifiedTrial(self.path, client)
        trial.talk('brack', 'Do you have a spare actuator?')
        self.assertEqual([], trial.data['world']['inventory'])
        self.assertEqual(2, len(client.planned))

    def test_soft_review_findings_are_recorded_without_blocking_conversation(self):
        client = ScriptedClient([plan()],
            [speech('The archive is north.'), speech('It holds maintenance records and an old identification plate.')],
            [revise('Answer contents, not location.'), approved()])
        trial = VerifiedTrial(self.path, client)
        turn = trial.talk('vesper', "What's inside?")
        self.assertEqual('The archive is north.', turn['reply'])
        self.assertEqual('revise', turn['reviews'][0]['review']['verdict'])
        self.assertFalse(turn['fallback'])

    def test_duplicate_actions_are_executed_once(self):
        client = ScriptedClient([plan('lend_actuator', 'lend_actuator')],
            [speech('The actuator is in your cargo.')], [approved()])
        trial = VerifiedTrial(self.path, client)
        turn = trial.talk('brack', 'Lend me the actuator.')
        self.assertEqual(['actuator'], trial.data['world']['inventory'])
        self.assertEqual(1, len(turn['actions']))

    def test_repeated_rejection_has_a_bounded_safe_fallback(self):
        contradiction = revise('Power is off.')
        contradiction['claimed_states'] = [{'field': 'power', 'value': True, 'quote': 'Power is on.'}]
        client = ScriptedClient([plan()], [speech('Power is on.')] * 2, [contradiction] * 2)
        trial = VerifiedTrial(self.path, client)
        turn = trial.talk('brack', 'Is power on?')
        self.assertEqual(FALLBACK, turn['reply'])
        self.assertTrue(turn['fallback'])
        self.assertNotIn('Power is on.', str(trial.data['conversations']))

    def test_protocol_and_permissions_are_strict(self):
        for invalid in [plan('open_archive'), plan('accept_repair_patch'), plan('give_gold'),
                        plan('lend_actuator', deferred=True), {'kind': 'question', 'actions': [], 'deferred': 'false'},
                        {'kind': 'question', 'actions': ['lend_actuator'], 'deferred': False}]:
            with self.assertRaises(ValueError):
                parse_plan(json.dumps(invalid), 'brack')
        with self.assertRaises(ValueError):
            parse_review('{"ok":true,"needs_action":false,"feedback":"","actions":["lend_actuator"]}')
        v = approved()
        v['feedback'] = 'Correct the transfer claim.'
        self.assertEqual('revise', parse_review(json.dumps(v))['verdict'])

    def test_game_checks_override_an_incorrect_verifier_acceptance(self):
        verdict = approved()
        verdict['claimed_actions'] = ['lend_actuator']
        verdict['claimed_states'] = [{'field': 'power', 'value': True, 'quote': 'Power is on.'}]
        checked = check_claims(verdict, initial_state()['world'], [], 'Power is on.')
        self.assertEqual('revise', checked['verdict'])
        self.assertIn('No actuator was transferred', checked['feedback'])

    def test_unmentioned_state_defaults_do_not_block_a_good_reply(self):
        verdict = approved()
        verdict['claimed_states'] = [{'field': 'power', 'value': True, 'quote': 'Power is on.'}]
        checked = check_claims(verdict, initial_state()['world'], [], 'Hello, Teapot.')
        self.assertEqual([], checked['mechanical_errors'])
        self.assertEqual(1, len(checked['unanchored_states']))

    def test_custom_planner_cannot_bypass_permissions(self):
        client = ScriptedClient([plan('open_archive')], [], [])
        trial = VerifiedTrial(self.path, client)
        with self.assertRaises(ValueError):
            trial.talk('brack', 'Open the archive.')
        self.assertFalse(trial.data['world']['archive_open'])

    def test_deferred_request_does_not_schedule_hidden_actions(self):
        client = ScriptedClient([plan(deferred=True)],
            [speech('Ask me again once Patch is repaired.')], [approved()])
        trial = VerifiedTrial(self.path, client)
        trial.data['world']['tasks'] = ['restore_power']
        trial.talk('brack', 'Open the workshop after I repair Patch.')
        self.assertFalse(trial.data['world']['workshop_open'])


if __name__ == '__main__':
    unittest.main()
