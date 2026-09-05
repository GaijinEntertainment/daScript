"""Round-trip speech probe. WER flags intelligibility issues, not voice quality."""

import argparse
import json
from pathlib import Path
import re
import time
import urllib.request
import uuid

from dialogue_trial import CHARACTERS, Client


def words(text):
    return re.findall(r"\w+(?:'\w+)?", text.lower().replace("\u2019", "'"))


def word_error(reference, actual):
    expected, heard = words(reference), words(actual)
    row = list(range(len(heard) + 1))
    for i, token in enumerate(expected, 1):
        previous, row = row, [i]
        for j, other in enumerate(heard, 1):
            row.append(min(row[-1] + 1, previous[j] + 1,
                           previous[j - 1] + (token != other)))
    return {"errors": row[-1], "reference_words": len(expected),
            "wer": row[-1] / max(1, len(expected))}


def transcribe(url, path):
    boundary = uuid.uuid4().hex
    body = (f'--{boundary}\r\nContent-Disposition: form-data; name="file"; '
            'filename="speech.wav"\r\nContent-Type: audio/wav\r\n\r\n').encode()
    body += path.read_bytes()
    body += (f'\r\n--{boundary}\r\nContent-Disposition: form-data; '
             f'name="response_format"\r\n\r\njson\r\n--{boundary}--\r\n').encode()
    request = urllib.request.Request(url.rstrip('/') + '/audio/transcriptions', body,
        {'Content-Type': 'multipart/form-data; boundary=' + boundary})
    started = time.perf_counter()
    with urllib.request.urlopen(request, timeout=180) as response:
        result = json.load(response)
    return result['text'], time.perf_counter() - started


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--url', default='http://127.0.0.1:18082/v1')
    parser.add_argument('--output-dir', type=Path, required=True)
    args = parser.parse_args()
    if args.output_dir.exists():
        parser.error('Use a fresh output directory to preserve previous recordings.')
    args.output_dir.mkdir(parents=True)
    client = Client(args.url, 'qwen3-30b')
    lines = {
        'brack': 'Brack here. Take the spare actuator to Patch, then restore power to the workshop.',
        'patch': 'Call me Patch, Teapot. My leg is repaired. Shall we explore the mine together?',
        'vesper': 'Vesper speaking. The archive interlock is open. Your operator inhabits another simulation.'}
    report = []
    for npc, line in lines.items():
        path = args.output_dir / (npc + '.wav')
        speech = client.speech(line, CHARACTERS[npc]['voice'], path)
        transcript, elapsed = transcribe(args.url, path)
        item = {'npc': npc, 'voice': CHARACTERS[npc]['voice'], 'text': line,
                'transcript': transcript, 'speech': speech, 'asr_elapsed_s': elapsed,
                **word_error(line, transcript)}
        report.append(item)
        (args.output_dir / 'report.json').write_text(json.dumps(report, indent=2), encoding='utf-8')
        print(json.dumps(item), flush=True)


if __name__ == '__main__':
    main()
