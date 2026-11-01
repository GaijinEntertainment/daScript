"""Score asr_corpus_eval.das runs over FLEURS ru_ru test: per run, WER/CER against the FLEURS reference, the WER
change against the base run with a paired 95% bootstrap interval, WER against the base run's own transcripts, chrF of
the translated hypotheses against the en_us reference, and the speed.

  python -I score_asr_corpus.py F:/corpora/fleurs [base-run]

Reads <root>/ru_ru/test.tsv, <root>/en_us/test.tsv and every <root>/ru_ru/hyp/<run>.tsv; a run with fewer clips
than the base is left out. Needs jiwer==4.0.0 and sacrebleu==2.6.0.
"""
import csv
import glob
import os
import random
import re
import sys

import jiwer
from sacrebleu.metrics import CHRF


def norm(s):
    s = s.lower().replace("ё", "е")
    s = re.sub(r"[^\w\s]|_", " ", s)
    return " ".join(s.split())


def read_tsv(path):
    with open(path, encoding="utf-8", newline="") as f:
        return list(csv.reader(f, delimiter="\t", quoting=csv.QUOTE_NONE))


def utt_edits(hs, rs):
    out = []
    for h, r in zip(hs, rs):
        o = jiwer.process_words(r, h)
        out.append((o.substitutions + o.deletions + o.insertions, len(r.split())))
    return out


def wer_of(e):
    return 100.0 * sum(x[0] for x in e) / sum(x[1] for x in e)


def main():
    root = sys.argv[1] if len(sys.argv) > 1 else "F:/corpora/fleurs"
    base = sys.argv[2] if len(sys.argv) > 2 else "f16-exact"
    ru_ref = {r[1]: r[2] for r in read_tsv(os.path.join(root, "ru_ru", "test.tsv"))}
    en_ref = {}
    for r in read_tsv(os.path.join(root, "en_us", "test.tsv")):
        en_ref.setdefault(r[0], r[2])
    runs = {}
    for p in sorted(glob.glob(os.path.join(root, "ru_ru", "hyp", "*.tsv"))):
        runs[os.path.basename(p)[:-4]] = {r[1]: r for r in read_tsv(p)}
    wavs = sorted(runs[base].keys())
    runs = {k: v for k, v in runs.items() if len(v) == len(wavs)}
    refs = [norm(ru_ref[w]) for w in wavs]
    base_h = [norm(runs[base][w][4]) for w in wavs]
    base_e = utt_edits(base_h, refs)
    base_words = sum(len(b.split()) for b in base_h)
    chrf = CHRF()
    rng = random.Random(1)
    draws = [[rng.randrange(len(wavs)) for _ in wavs] for _ in range(1000)]
    print(f"{len(wavs)} clips, {sum(x[1] for x in base_e)} reference words")
    if len(runs[base][wavs[0]]) > 6:
        ceiling = chrf.corpus_score([runs[base][w][6] for w in wavs], [[en_ref[runs[base][w][0]] for w in wavs]]).score
        print(f"NMT ceiling (reference ru -> en): chrF {ceiling:.2f}")
    print(f"{'run':18} {'WER%':>6} {'CER%':>6} {'dWER':>7} {'95% CI':>15} {'vs base%':>9} {'chrF':>6} {'x RT':>6}")
    for name, rows in runs.items():
        hs = [norm(rows[w][4]) for w in wavs]
        e = utt_edits(hs, refs)
        deltas = sorted(100.0 * (sum(e[i][0] for i in d) - sum(base_e[i][0] for i in d)) / sum(base_e[i][1] for i in d)
                        for d in draws)
        vs = 100.0 * sum(x[0] for x in utt_edits(hs, base_h)) / max(1, base_words)
        ch = float("nan")
        if len(rows[wavs[0]]) > 5:
            ch = chrf.corpus_score([rows[w][5] for w in wavs], [[en_ref[rows[w][0]] for w in wavs]]).score
        aud = sum(float(rows[w][2]) for w in wavs)
        ms = sum(float(rows[w][3]) for w in wavs)
        print(f"{name:18} {wer_of(e):6.2f} {100.0 * jiwer.cer(refs, hs):6.2f} {wer_of(e) - wer_of(base_e):+7.2f} "
              f"[{deltas[25]:+6.2f},{deltas[974]:+6.2f}] {vs:9.2f} {ch:6.2f} {aud / (ms / 1000.0):6.1f}")


if __name__ == "__main__":
    main()
