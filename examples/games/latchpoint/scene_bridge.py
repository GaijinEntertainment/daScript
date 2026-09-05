"""Local development adapter between the daslang scene and tested dialogue rules."""

import argparse
from concurrent.futures import ThreadPoolExecutor
import copy
from datetime import datetime, timezone
import hashlib
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
from pathlib import Path
import threading
import uuid

from dialogue_trial import CHARACTERS, ROOT, initial_state
from gameplay_dialogue import GameplayClient, GroundedTrial
from verified_dialogue import VerifiedClient, VerifiedTrial


class SceneSession:
    def __init__(self, trial, speech=False, log_path=None):
        self.trial = trial
        self.speech = speech
        self.lock = threading.Lock()
        self.pool = ThreadPoolExecutor(max_workers=1)
        self.jobs = {}
        self.busy = False
        self.world = copy.deepcopy(trial.data['world'])
        self.log_path = log_path

    def trace(self, event):
        if self.log_path is None:
            return
        try:
            self.log_path.parent.mkdir(parents=True, exist_ok=True)
            record = {'utc': datetime.now(timezone.utc).isoformat(), **event}
            with self.log_path.open('a', encoding='utf-8') as stream:
                stream.write(json.dumps(record, ensure_ascii=False) + '\n')
        except OSError as error:
            print(f'Playtest trace could not be written: {error}', flush=True)

    def snapshot(self):
        with self.lock:
            return copy.deepcopy(self.world)

    def submit(self, command):
        kind = command.get('kind')
        if kind not in ('talk', 'physical', 'new_game'):
            raise ValueError('Unknown scene command.')
        if kind == 'talk' and (command.get('npc') not in CHARACTERS or
                not isinstance(command.get('text'), str) or not 1 <= len(command['text'].strip()) <= 2000):
            raise ValueError('Invalid resident or text.')
        if kind == 'physical' and command.get('text') not in (
                'salvage actuator', 'install actuator', 'salvage coil', 'install coil'):
            raise ValueError('Unknown physical interaction.')
        with self.lock:
            if self.busy:
                raise RuntimeError('A resident is still responding.')
            self.busy = True
            job_id = uuid.uuid4().hex
            if len(self.jobs) >= 32:
                self.jobs.pop(next(iter(self.jobs)))
            self.jobs[job_id] = {'done': False}
        self.pool.submit(self.execute, job_id, dict(command))
        return job_id

    def execute(self, job_id, command):
        self.trace({'event': 'started', 'job': job_id, 'command': command, 'world': self.snapshot()})
        def report_stage(stage):
            with self.lock:
                self.jobs[job_id]['stage'] = stage

        self.trial.on_stage = report_stage
        try:
            if command['kind'] == 'new_game':
                previous = self.trial.data
                self.trial.data = initial_state()
                try:
                    self.trial.save()
                except Exception:
                    self.trial.data = previous
                    raise
                turn = {}
                reply = 'A quiet shift at Latchpoint. Approach a resident and press E to speak.'
            elif command['kind'] == 'talk':
                turn = self.trial.talk(command['npc'], command['text'])
                reply = turn['reply']
            else:
                turn = self.trial.interact(command['text'])
                reply = turn['detail']
            audio = None
            if self.speech and command['kind'] == 'talk' and reply:
                try:
                    report_stage('voice')
                    path = ROOT / 'saves' / 'scene-voice.wav'
                    turn['speech'] = self.trial.client.speech(reply, CHARACTERS[command['npc']]['voice'], path,
                                            CHARACTERS[command['npc']].get('speech_speed', 1.0))
                    turn['speech'].pop('path', None)
                    audio = path
                except Exception as error:
                    turn['speech_error'] = str(error)
            result = {'done': True, 'reply': reply, 'turn': turn, 'reset': command['kind'] == 'new_game'}
            if result['reset']:
                try:
                    import winsound
                    winsound.PlaySound(None, 0)
                except (ImportError, RuntimeError):
                    pass
            if audio and audio.exists():
                try:
                    import winsound
                    winsound.PlaySound(str(audio), winsound.SND_FILENAME | winsound.SND_ASYNC)
                except (ImportError, RuntimeError):
                    pass
        except Exception as error:
            result = {'done': True, 'error': str(error), 'reply': 'Connection interrupted. Check local telemetry.'}
        self.trial.on_stage = None
        with self.lock:
            self.world = copy.deepcopy(self.trial.data['world'])
            result['world'] = copy.deepcopy(self.world)
            result['has_save'] = self.trial.save_path.exists()
            self.jobs[job_id] = result
            self.busy = False
        self.trace({'event': 'completed', 'job': job_id, 'result': result})

    def job(self, job_id):
        with self.lock:
            return copy.deepcopy(self.jobs.get(job_id))


class Handler(BaseHTTPRequestHandler):
    def reply(self, code, value):
        data = json.dumps(value).encode('utf-8')
        self.send_response(code)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Content-Length', str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        if self.path == '/state':
            self.reply(200, {'world': self.server.session.snapshot(),
                             'has_save': self.server.session.trial.save_path.exists(),
                             'playtest_logging': self.server.session.log_path is not None})
        elif self.path.startswith('/job/'):
            result = self.server.session.job(self.path[5:])
            self.reply(200 if result else 404, result or {'error': 'Unknown job.'})
        else:
            self.reply(404, {'error': 'Unknown route.'})

    def do_POST(self):
        if self.headers.get('Origin') or self.path != '/command':
            self.reply(403, {'error': 'Only local game commands are accepted.'})
            return
        try:
            size = int(self.headers.get('Content-Length', '0'))
            if not 0 < size <= 8192:
                raise ValueError('Invalid body size.')
            command = json.loads(self.rfile.read(size))
            if not isinstance(command, dict):
                raise ValueError('Expected an object.')
            self.reply(202, {'job': self.server.session.submit(command)})
        except (ValueError, TypeError) as error:
            self.reply(400, {'error': str(error)})
        except RuntimeError as error:
            self.reply(409, {'error': str(error)})


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--save', type=Path, default=ROOT / 'saves' / 'scene.json')
    parser.add_argument('--speech', action='store_true')
    parser.add_argument('--verified', action='store_true', help='Opt into the experimental natural-dialogue verifier.')
    parser.add_argument('--no-playtest-log', action='store_true', help='Disable local diagnostic conversation traces.')
    args = parser.parse_args()
    server = ThreadingHTTPServer(('127.0.0.1', 18083), Handler)
    log_path = None if args.no_playtest_log else ROOT.parents[2] / 'logs' / 'latchpoint-playtests' / (
        datetime.now(timezone.utc).strftime('%Y%m%d-%H%M%S-') + uuid.uuid4().hex[:8] + '.jsonl')
    trial_type, client_type = (VerifiedTrial, VerifiedClient) if args.verified else (GroundedTrial, GameplayClient)
    server.session = SceneSession(trial_type(args.save, client_type('http://127.0.0.1:18082/v1',
                                    'qwen3-30b', native_tools=True)), args.speech, log_path)
    source_hash = hashlib.sha256(b''.join((ROOT / name).read_bytes() for name in (
        'verified_dialogue.py', 'gameplay_dialogue.py', 'dialogue_trial.py', 'world_knowledge.json', 'characters.json'))).hexdigest()
    server.session.trace({'event': 'session', 'protocol': 'verified-v1' if args.verified else 'grounded-v1', 'model': 'qwen3-30b',
                          'source_hash': source_hash, 'speech': args.speech})
    print('Latchpoint scene bridge: http://127.0.0.1:18083', flush=True)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
        server.session.pool.shutdown(wait=True)


if __name__ == '__main__':
    main()
