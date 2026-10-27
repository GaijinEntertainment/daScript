#!/usr/bin/env python3
"""Pick the GigaAM fixture clips from FLEURS ru_ru: one short, one near the 25 s
window, and one long clip stitched from several so the chunked path has a subject.

  .venv-gigaam/bin/python fetch_fleurs_ru.py --out corpus

Writes ru_short.wav / ru_mid.wav / ru_long.wav (16 kHz mono s16) and ru_fleurs.json with the
FLEURS ids and reference transcriptions of every clip used.
"""
import argparse
import json
import os

import numpy as np
import soundfile as sf
from datasets import load_dataset


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True)
    ap.add_argument("--scan", type=int, default=120, help="how many test clips to look at")
    args = ap.parse_args()
    os.makedirs(args.out, exist_ok=True)

    ds = load_dataset("google/fleurs", "ru_ru", split="test", streaming=True)
    clips = []
    for i, row in enumerate(ds):
        if i >= args.scan:
            break
        a = row["audio"]
        sr = a["sampling_rate"]
        x = np.asarray(a["array"], dtype=np.float32)
        if sr != 16000:
            continue
        clips.append({"id": row["id"], "dur": len(x) / sr, "text": row["transcription"],
                      "raw": row["raw_transcription"], "x": x})

    def pick(lo, hi):
        c = [c for c in clips if lo <= c["dur"] <= hi]
        return c[0] if c else None

    short = pick(4.0, 8.0)
    mid = pick(18.0, 24.5)
    if short is None or mid is None:
        raise SystemExit("no clip in the wanted duration windows; raise --scan")
    used = {"short": [short], "mid": [mid]}
    acc, parts, total = [], [], 0.0
    for c in clips:
        if c is short or c is mid:
            continue
        parts.append(c)
        acc.append(c["x"])
        acc.append(np.zeros(8000, dtype=np.float32))
        total += c["dur"] + 0.5
        if total > 70.0:
            break
    used["long"] = parts

    def write(name, x):
        sf.write(os.path.join(args.out, name), x, 16000, subtype="PCM_16")

    write("ru_short.wav", short["x"])
    write("ru_mid.wav", mid["x"])
    write("ru_long.wav", np.concatenate(acc))
    meta = {k: [{"id": c["id"], "dur": round(c["dur"], 2), "text": c["text"], "raw": c["raw"]} for c in v]
            for k, v in used.items()}
    with open(os.path.join(args.out, "ru_fleurs.json"), "w", encoding="utf-8") as f:
        json.dump(meta, f, ensure_ascii=False, indent=1)
    for k, v in meta.items():
        print(k, sum(c["dur"] for c in v), "s", len(v), "clips")


if __name__ == "__main__":
    main()
