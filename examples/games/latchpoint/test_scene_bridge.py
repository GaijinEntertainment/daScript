from pathlib import Path
import json
import tempfile
import threading
import unittest

from dialogue_trial import Trial
from scene_bridge import SceneSession


class SceneBridgeTests(unittest.TestCase):
    def test_local_trace_records_events_without_becoming_npc_memory(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'save.json'
            trace = Path(directory) / 'playtest.jsonl'
            trial = Trial(path, None)
            session = SceneSession(trial, log_path=trace)
            job = session.submit({'kind': 'physical', 'text': 'salvage actuator'})
            session.pool.shutdown(wait=True)
            events = [json.loads(line) for line in trace.read_text(encoding='utf-8').splitlines()]
            self.assertEqual(['started', 'completed'], [e['event'] for e in events])
            self.assertEqual(job, events[0]['job'])
            self.assertEqual(['actuator'], events[1]['result']['world']['inventory'])
            self.assertEqual([], trial.data['conversations']['patch'])

    def test_failed_new_game_save_preserves_current_progress(self):
        with tempfile.TemporaryDirectory() as directory:
            trial = Trial(Path(directory) / 'save.json', None)
            trial.data['world']['power'] = True

            def fail_save():
                raise OSError('Disk unavailable')

            trial.save = fail_save
            session = SceneSession(trial)
            job = session.submit({'kind': 'new_game'})
            session.pool.shutdown(wait=True)
            self.assertTrue(trial.data['world']['power'])
            self.assertIn('error', session.job(job))
            self.assertNotIn('reset', session.job(job))

    def test_new_game_resets_world_and_conversations_on_disk(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'save.json'
            trial = Trial(path, None)
            trial.data['world']['power'] = True
            trial.data['world']['inventory'] = ['coil']
            trial.data['conversations']['patch'] = [[{'role': 'user', 'content': 'Remember this.'}]]
            trial.save()
            session = SceneSession(trial)
            job = session.submit({'kind': 'new_game'})
            session.pool.shutdown(wait=True)
            restored = Trial(path, None)
            self.assertFalse(restored.data['world']['power'])
            self.assertEqual([], restored.data['world']['inventory'])
            self.assertEqual([], restored.data['conversations']['patch'])
            self.assertTrue(session.job(job)['reset'])

    def test_rejects_unknown_and_malformed_actions(self):
        with tempfile.TemporaryDirectory() as directory:
            session = SceneSession(Trial(Path(directory) / 'save.json', None))
            try:
                for command in [{'kind': 'shell'}, {'kind': 'physical', 'text': 'give gold'},
                                {'kind': 'talk', 'npc': 'nobody', 'text': 'hi'},
                                {'kind': 'talk', 'npc': 'patch', 'text': ''}]:
                    with self.assertRaises(ValueError):
                        session.submit(command)
            finally:
                session.pool.shutdown()

    def test_physical_result_and_world_agree(self):
        with tempfile.TemporaryDirectory() as directory:
            session = SceneSession(Trial(Path(directory) / 'save.json', None))
            job = session.submit({'kind': 'physical', 'text': 'salvage actuator'})
            session.pool.shutdown(wait=True)
            self.assertEqual(['actuator'], session.job(job)['world']['inventory'])
            self.assertEqual(session.snapshot(), session.job(job)['world'])
            self.assertTrue(session.job(job)['done'])

    def test_world_poll_does_not_wait_for_dialogue(self):
        entered, release = threading.Event(), threading.Event()

        class SlowClient:
            native_tools = True

            def chat(self, *args, **kwargs):
                entered.set()
                release.wait(timeout=3)
                return {'role': 'assistant', 'content': 'Hello, Teapot.'}, {}

        with tempfile.TemporaryDirectory() as directory:
            session = SceneSession(Trial(Path(directory) / 'save.json', SlowClient()))
            try:
                job = session.submit({'kind': 'talk', 'npc': 'patch', 'text': 'Hello.'})
                self.assertTrue(entered.wait(timeout=1))
                self.assertFalse(session.job(job)['done'])
                self.assertEqual([], session.snapshot()['inventory'])
                with self.assertRaises(RuntimeError):
                    session.submit({'kind': 'physical', 'text': 'salvage actuator'})
            finally:
                release.set()
                session.pool.shutdown(wait=True)
            self.assertEqual('Hello, Teapot.', session.job(job)['reply'])


if __name__ == '__main__':
    unittest.main()
