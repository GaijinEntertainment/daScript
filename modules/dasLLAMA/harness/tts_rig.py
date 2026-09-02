"""The TTS number-maker: synthesize the 200-sentence fixture with each das model, transcribe
with parakeet, score UTMOS, and print WER / UTMOS / RTF per model - the regression net for
every kernel and format change, and the end-to-end quality gate the parity seams cannot be.

    python tts_rig.py --root ~/Work/tts-ab/g2p --out /tmp/tts_rig \\
        --models kitten-nano:Bella kitten-mini:Bella kokoro-82m:af_heart [--limit 20]

`root` is the G2P fidelity experiment (its .venv-nemo carries parakeet, .venv-metrics carries
the UTMOS predictor, work/KittenTTS the normalizer WER scores through). The das binary and the
module cache are the repo's (`bin/daslang`, `build/module_cache.bin`).
"""
import argparse
import glob
import json
import os
import subprocess
import sys
import textwrap
import time

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
FIXTURE = os.path.join(HERE, "..", "tests", "_tts_fixtures", "g2p_corpus.json")
MODELS_DIR = os.path.expanduser("~/Work/llama.cpp/models")

ASR_SCRIPT = textwrap.dedent('''
    import glob, json, os, sys, logging
    logging.disable(logging.CRITICAL)
    import nemo.collections.asr as nemo_asr
    asr = nemo_asr.models.ASRModel.from_pretrained(model_name="nvidia/parakeet-tdt-0.6b-v2"); asr.eval()
    out = {}
    for d in sys.argv[1:]:
        wavs = sorted(glob.glob(os.path.join(d, "*.wav")))
        hyps = asr.transcribe(wavs, batch_size=8, verbose=False)
        out[d] = {os.path.splitext(os.path.basename(w))[0]: (h.text if hasattr(h, "text") else h) for w, h in zip(wavs, hyps)}
        json.dump(out[d], open(os.path.join(d, "asr.json"), "w"), indent=1)
        print("asr", d, len(wavs), flush=True)
''')

UTMOS_SCRIPT = textwrap.dedent('''
    import glob, json, os, sys
    import numpy as np, torch, soundfile as sf, librosa
    torch.set_num_threads(4)
    predictor = torch.hub.load("tarepan/SpeechMOS:v1.2.0", "utmos22_strong", trust_repo=True); predictor.eval()
    for d in sys.argv[1:]:
        rec = {}
        for w in sorted(glob.glob(os.path.join(d, "*.wav"))):
            x, sr = sf.read(w, dtype="float32")
            if x.ndim > 1: x = x.mean(axis=1)
            y = librosa.resample(np.nan_to_num(x), orig_sr=sr, target_sr=16000) if sr != 16000 else x
            with torch.no_grad():
                rec[os.path.splitext(os.path.basename(w))[0]] = float(predictor(torch.from_numpy(y).unsqueeze(0), 16000).item())
        json.dump(rec, open(os.path.join(d, "utmos.json"), "w"), indent=1)
        print("utmos", d, len(rec), flush=True)
''')


def run(cmd, env=None):
    print("+", " ".join(cmd), flush=True)
    t0 = time.time()
    subprocess.run(cmd, check=True, env=env)
    print(f"  done in {time.time() - t0:.0f} s", flush=True)


def wer_words(root):
    sys.path.insert(0, os.path.join(root, "scripts"))
    import wer as wer_mod   # the experiment's scorer: KittenTTS normalization on both sides
    return wer_mod.norm_words, wer_mod.edit_ops


def score_wer(norm_words, edit_ops, rows, hyps):
    errs = words = 0
    for r in rows:
        if r["id"] not in hyps:
            continue
        ref = norm_words(r["text"])
        hyp = norm_words(hyps[r["id"]])
        d = edit_ops(ref, hyp)
        errs += d[len(ref)][len(hyp)] if isinstance(d, list) else d
        words += len(ref)
    return 100.0 * errs / max(words, 1), words


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", default=os.path.expanduser("~/Work/tts-ab/g2p"))
    ap.add_argument("--out", required=True)
    ap.add_argument("--models", nargs="+", default=["kitten-nano:Bella", "kitten-mini:Bella", "kokoro-82m:af_heart"])
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--daslang", default=os.path.join(REPO, "bin", "daslang"))
    ap.add_argument("--skip-synth", action="store_true", help="score existing folders")
    ap.add_argument("--q8", action="store_true", help="serve the rows GEMMs as Q8_0 quants (folders get a _q8 suffix)")
    a = ap.parse_args()
    rows = json.load(open(FIXTURE, encoding="utf8"))
    if a.limit:
        rows = rows[: a.limit]
    dirs = {}
    for spec in a.models:
        model, voice = spec.split(":")
        d = os.path.join(a.out, f"{model}_{voice}" + ("_q8" if a.q8 else ""))
        dirs[spec] = d
        os.makedirs(d, exist_ok=True)
        if not a.skip_synth:
            cmd = [a.daslang, "-jit", "-module-cache", os.path.join(REPO, "build", "module_cache.bin"),
                   os.path.join(HERE, "tts_synth.das"), "--", "--model", os.path.join(MODELS_DIR, model + ".gguf"),
                   "--voice", voice, "--out", d]
            if a.limit:
                cmd += ["--limit", str(a.limit)]
            if a.q8:
                cmd += ["--q8"]
            run(cmd)
    env = dict(os.environ, HF_HOME=os.path.join(a.root, ".hf"), NEMO_CACHE_DIR=os.path.join(a.root, ".nemo"),
               TORCH_HOME=os.path.join(os.path.dirname(a.root), ".torchhub"))
    run([os.path.join(os.path.dirname(a.root), ".venv-nemo", "bin", "python"), "-c", ASR_SCRIPT] + list(dirs.values()), env)
    run([os.path.join(os.path.dirname(a.root), ".venv-metrics", "bin", "python"), "-c", UTMOS_SCRIPT] + list(dirs.values()), env)
    norm_words, edit_ops = wer_words(a.root)
    print()
    print(f"{'model:voice':24s} {'WER %':>7s} {'UTMOS':>7s} {'audio s':>8s} {'gen s':>7s} {'RTF':>6s}  sentences")
    for spec, d in dirs.items():
        hyps = json.load(open(os.path.join(d, "asr.json")))
        mos = json.load(open(os.path.join(d, "utmos.json")))
        tim = json.load(open(os.path.join(d, "timings.json")))
        wer, words = score_wer(norm_words, edit_ops, rows, hyps)
        audio = sum(t["audio_s"] for t in tim)
        gen = sum(t["gen_ms"] for t in tim) / 1000.0
        mos_mean = sum(mos.values()) / max(len(mos), 1)
        print(f"{spec:24s} {wer:7.2f} {mos_mean:7.3f} {audio:8.1f} {gen:7.1f} {gen / max(audio, 1e-9):6.3f}  {len(hyps)} ({words} words)")
    print("\nreference (python arm E, same sentences, same scorers): kitten-nano 4.22% / 4.035, kitten-mini 4.00% / 3.996, kokoro 3.18% / 4.499")


if __name__ == "__main__":
    main()
