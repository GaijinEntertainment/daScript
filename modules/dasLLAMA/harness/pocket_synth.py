"""The reference arm for the Pocket TTS family: the 200-sentence fixture through Kyutai's own
package, one WAV per sentence and a timings.json in the shape harness/tts_synth.das writes, so
harness/tts_rig.py --skip-synth scores the folder like any das model folder.

    ~/Work/tts-ab/pocket/.venv-pocket/bin/python pocket_synth.py --language english_2026-04 \\
        --voice <name or wav path> --out ~/Work/tts-ab/results/pocket_ref/pocket-ref_<voice> [--limit N]

The package forces torch to one thread (pocket_tts/models/tts_model.py); the timings are what a
user of the package gets on this box, not a tuned figure.
"""
import argparse
import json
import os
import time

import numpy as np
import soundfile as sf
import torch

HERE = os.path.dirname(os.path.abspath(__file__))
FIXTURE = os.path.join(HERE, "..", "tests", "_tts_fixtures", "g2p_corpus.json")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--language", default="english_2026-04")
    ap.add_argument("--voice", required=True, help="a predefined voice name, or a path/URL to a wav")
    ap.add_argument("--out", required=True)
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--fixture", default=FIXTURE)
    ap.add_argument("--temp", type=float, default=None, help="sampling temperature (default: the config's)")
    a = ap.parse_args()

    from pocket_tts import TTSModel

    rows = json.load(open(a.fixture, encoding="utf8"))
    if a.limit:
        rows = rows[: a.limit]
    os.makedirs(a.out, exist_ok=True)

    t0 = time.monotonic()
    model = TTSModel.load_model(language=a.language, temp=a.temp)
    print(f"model {a.language} loaded in {time.monotonic() - t0:.1f} s, temp {model.temp}", flush=True)
    t0 = time.monotonic()
    voice_state = model.get_state_for_audio_prompt(a.voice)
    print(f"voice {a.voice} encoded in {time.monotonic() - t0:.1f} s", flush=True)

    timings = []
    audio_total = gen_total = 0.0
    for r in rows:
        torch.manual_seed(0)
        t0 = time.monotonic()
        audio = model.generate_audio(voice_state, r["text"])
        gen_ms = (time.monotonic() - t0) * 1000.0
        pcm = audio.numpy().astype(np.float32)
        sf.write(os.path.join(a.out, f'{r["id"]}.wav'), pcm, model.sample_rate, subtype="PCM_16")
        audio_s = len(pcm) / model.sample_rate
        timings.append(dict(id=r["id"], audio_s=audio_s, gen_ms=gen_ms, rtf=gen_ms / 1000.0 / max(audio_s, 1e-9)))
        audio_total += audio_s
        gen_total += gen_ms / 1000.0
        print(f'{r["id"]} {audio_s:6.2f} s audio in {gen_ms:7.0f} ms (rtf {gen_ms / 1000.0 / max(audio_s, 1e-9):.3f})', flush=True)
    json.dump(timings, open(os.path.join(a.out, "timings.json"), "w"), indent=1)
    print(f"pocket synth: {len(timings)} sentences, {audio_total:.1f} s of audio in {gen_total:.1f} s "
          f"(rtf {gen_total / max(audio_total, 1e-9):.3f}) -> {a.out}", flush=True)


if __name__ == "__main__":
    main()
