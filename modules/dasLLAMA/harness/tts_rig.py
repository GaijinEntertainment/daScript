"""The TTS number-maker: synthesize the 200-sentence fixture with each das model, transcribe
with parakeet, score UTMOS, and print WER / UTMOS / RTF per model - the regression net for
every kernel and format change, and the end-to-end quality gate the parity seams cannot be.

    python tts_rig.py --root ~/Work/tts-ab/g2p --out /tmp/tts_rig \\
        --models kitten-nano:Bella kitten-mini:Bella kokoro-82m:af_heart [--limit 20]
    python tts_rig.py --root ~/Work/tts-ab/g2p --out /tmp/tts_rig_ru --lang ru

`--lang ru` speaks the 200-sentence Russian fixture with the kokoro-ru voices and transcribes with
the engine's own GigaAM (`asr_corpus_eval.das`); WER is on words lowercased with the punctuation
dropped and yo folded to ye.

`root` is the G2P fidelity experiment (its .venv-nemo carries parakeet, .venv-metrics carries
the UTMOS predictor, work/KittenTTS the normalizer WER scores through). The das binary and the
module cache are the repo's (`bin/daslang`, `build/module_cache.bin`).
"""
import argparse
import glob
import json
import os
import re
import subprocess
import sys
import textwrap
import time

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
FIXTURE = os.path.join(HERE, "..", "tests", "_tts_fixtures", "g2p_corpus.json")
FIXTURE_RU = os.path.join(HERE, "..", "tests", "_tts_fixtures", "g2p_corpus_ru.json")
MODELS_DIR = os.path.expanduser("~/Work/llama.cpp/models")
MODELS_RU = ["kokoro-ru:sveta", "kokoro-ru:masha", "kokoro-ru-dima:dima"]
ASR_RU = "gigaam-v3-e2e-rnnt-f16.gguf"

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


CATEGORIES = ["harvard", "ljspeech", "heteronym", "oov", "numeric"]   # the fixture's tags, in its order


def run(cmd, env=None):
    print("+", " ".join(cmd), flush=True)
    t0 = time.time()
    subprocess.run(cmd, check=True, env=env)
    print(f"  done in {time.time() - t0:.0f} s", flush=True)


def wer_words(root):
    sys.path.insert(0, os.path.join(root, "scripts"))
    import wer as wer_mod   # the experiment's scorer: KittenTTS normalization on both sides
    return wer_mod.norm_words, wer_mod.edit_ops


def spoken_words(norm_words, s):
    """the scorer's word list with the clock suffix as one word - the transcriber writes "am", the
    normalizers say "a m", and the two are the same speech"""
    words = norm_words(s)
    out = []
    for w in words:
        if out and w == "m" and out[-1] in ("a", "p"):
            out[-1] += "m"
        else:
            out.append(w)
    return out


def score_wer(norm_words, edit_ops, rows, hyps):
    """WER against the fixture's expected spoken form: `expected` where the upstream normalizer's
    reading was corrected by hand (the minter carries the table), else `norm`"""
    errs = words = 0
    for r in rows:
        ref = spoken_words(norm_words, r.get("expected") or r["norm"])
        words += len(ref)
        if r["id"] not in hyps:   # a sentence with no transcript is every word of it missed
            errs += len(ref)
            continue
        hyp = spoken_words(norm_words, hyps[r["id"]])
        d = edit_ops(ref, hyp)
        errs += d[len(ref)][len(hyp)] if isinstance(d, list) else d
    return 100.0 * errs / max(words, 1), words


def ru_words(s):
    """a Russian line as the scorer's words: lowercased, yo folded to ye, punctuation dropped"""
    return re.sub(r"[^\w\s]|_", " ", s.lower().replace("\u0451", "\u0435")).split()


def edit_distance(ref, hyp):
    row = list(range(len(hyp) + 1))
    for i, r in enumerate(ref, 1):
        prev, row[0] = row[0], i
        for j, h in enumerate(hyp, 1):
            prev, row[j] = row[j], min(row[j] + 1, row[j - 1] + 1, prev + (r != h))
    return row[len(hyp)]


def transcribe_ru(daslang, rows, d, asr_model):
    """GigaAM over a folder of the rig's WAVs through asr_corpus_eval.das, into the asr.json the English arm writes"""
    corpus = os.path.join(d, "asr")
    os.makedirs(corpus, exist_ok=True)
    split = os.path.join(corpus, "test")
    if os.path.islink(split):   # a link left by a run from another folder, dangling or not
        os.remove(split)
    if not os.path.exists(split):
        os.symlink(os.path.abspath(d), split)
    with open(os.path.join(corpus, "test.tsv"), "w", encoding="utf8") as f:
        for r in rows:
            f.write(f"{r['id']}\t{r['id']}.wav\t{r['text']}\n")
    hyp = os.path.join(corpus, "hyp.tsv")
    run([daslang, "-jit", "-dasroot", REPO, os.path.join(HERE, "asr_corpus_eval.das"), "--", "--model", asr_model,
         "--corpus", corpus, "--out", hyp])
    out = {}
    for line in open(hyp, encoding="utf8"):
        cols = line.rstrip("\n").split("\t")
        if len(cols) >= 5:
            out[cols[0]] = cols[4]
    json.dump(out, open(os.path.join(d, "asr.json"), "w", encoding="utf8"), indent=1, ensure_ascii=False)


def score_wer_ru(rows, hyps):
    errs = words = 0
    for r in rows:
        ref = ru_words(r["text"])
        words += len(ref)
        errs += edit_distance(ref, ru_words(hyps[r["id"]])) if r["id"] in hyps else len(ref)
    return 100.0 * errs / max(words, 1), words


def lane_suffix(a):
    """the run folder's lane part: a lane or recipe flag changes what is synthesized, so it changes the folder"""
    clip = "_clip" if a.clip else ""
    if a.q8:
        return "_q8" + clip
    if a.f32:
        return "_f32" + clip
    if a.recipe:
        return "_recipe-" + re.sub(r"[^a-z0-9]+", "-", a.recipe.lower()).strip("-") + clip
    return ("_small" if a.small else "") + clip


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", default=os.path.expanduser("~/Work/tts-ab/g2p"))
    ap.add_argument("--out", required=True)
    ap.add_argument("--models", nargs="+", default=None, help="model:voice pairs (default: the language's set)")
    ap.add_argument("--lang", default="en", choices=["en", "ru"], help="the sentence set and its scorer")
    ap.add_argument("--asr-model", default=os.path.join(MODELS_DIR, ASR_RU), help="the --lang ru transcriber")
    ap.add_argument("--tag", default="", help="a suffix for the output folders, to keep two runs of one lane apart")
    ap.add_argument("--small", action="store_true", help="a phoneme family's recipe lane, the small form (folders get a _small suffix)")
    ap.add_argument("--recipe", default="", help="group=format pairs pinned on a phoneme family's recipe lane (tts_synth.das --recipe; folders get a _recipe-<pairs> suffix)")
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--daslang", default=os.path.join(REPO, "bin", "daslang"))
    ap.add_argument("--skip-synth", action="store_true", help="score existing folders")
    ap.add_argument("--q8", action="store_true", help="pin the rows GEMMs to Q8_0 quants, the serving default (folders get a _q8 suffix)")
    ap.add_argument("--f32", action="store_true", help="pin the rows GEMMs to the file's f32 planes, the reference lane (folders get a _f32 suffix)")
    ap.add_argument("--clip", default="", help="clone each pair's voice from this audio clip (tts_synth.das --clip; a cloning Pocket file; folders get a _clip suffix)")
    a = ap.parse_args()
    if (a.small or a.recipe) and (a.q8 or a.f32):
        ap.error("--small / --recipe ask the recipe lane and --q8 / --f32 pin another: pass one lane")
    ru = a.lang == "ru"
    if a.models is None:
        a.models = MODELS_RU if ru else ["kitten-nano:Bella", "kitten-mini:Bella", "kokoro-82m:af_heart"]
    if (a.small or a.recipe) and any(spec.startswith("pocket") for spec in a.models):
        ap.error("--small / --recipe are a phoneme family's lane: a Pocket file serves the formats it stores")
    fixture = FIXTURE_RU if ru else FIXTURE
    rows = json.load(open(fixture, encoding="utf8"))
    if a.limit:
        rows = rows[: a.limit]
    dirs = {}
    for spec in a.models:
        model, voice = spec.split(":")
        d = os.path.join(a.out, f"{model}_{voice}" + lane_suffix(a) + (f"_{a.tag}" if a.tag else ""))
        dirs[spec] = d
        os.makedirs(d, exist_ok=True)
        if not a.skip_synth:
            cmd = [a.daslang, "-jit", "-dasroot", REPO, "-module-cache", os.path.join(REPO, "build", "module_cache.bin"),
                   os.path.join(HERE, "tts_synth.das"), "--", "--model", os.path.join(MODELS_DIR, model + ".gguf"),
                   "--voice", voice, "--out", d]
            if a.limit:
                cmd += ["--limit", str(a.limit)]
            if ru:
                cmd += ["--fixture", fixture]
            if a.recipe:
                cmd += ["--recipe", a.recipe]
            if a.small:
                cmd += ["--small"]
            if a.q8:
                cmd += ["--q8"]
            if a.f32:
                cmd += ["--f32"]
            if a.clip:
                cmd += ["--clip", a.clip]
            run(cmd)
    env = dict(os.environ, HF_HOME=os.path.join(a.root, ".hf"), NEMO_CACHE_DIR=os.path.join(a.root, ".nemo"),
               TORCH_HOME=os.path.join(os.path.dirname(a.root), ".torchhub"))
    if ru:
        for d in dirs.values():
            transcribe_ru(a.daslang, rows, d, a.asr_model)
    else:
        run([os.path.join(os.path.dirname(a.root), ".venv-nemo", "bin", "python"), "-c", ASR_SCRIPT] + list(dirs.values()), env)
    run([os.path.join(os.path.dirname(a.root), ".venv-metrics", "bin", "python"), "-c", UTMOS_SCRIPT] + list(dirs.values()), env)
    norm_words, edit_ops = (None, None) if ru else wer_words(a.root)
    print()
    print(f"{'model:voice':24s} {'WER %':>7s} {'UTMOS':>7s} {'audio s':>8s} {'gen s':>7s} {'RTF':>6s}  sentences")
    for spec, d in dirs.items():
        hyps = json.load(open(os.path.join(d, "asr.json"), encoding="utf8"))
        mos = json.load(open(os.path.join(d, "utmos.json"), encoding="utf8"))
        tim = json.load(open(os.path.join(d, "timings.json"), encoding="utf8"))
        if len(hyps) < len(rows):
            print(f"{spec}: {len(rows) - len(hyps)} of {len(rows)} sentences have no transcript - each scored as every word missed")
        wer, words = score_wer_ru(rows, hyps) if ru else score_wer(norm_words, edit_ops, rows, hyps)
        audio = sum(t["audio_s"] for t in tim)
        gen = sum(t["gen_ms"] for t in tim) / 1000.0
        mos_mean = sum(mos.values()) / max(len(mos), 1)
        print(f"{spec:24s} {wer:7.2f} {mos_mean:7.3f} {audio:8.1f} {gen:7.1f} {gen / max(audio, 1e-9):6.3f}  {len(hyps)} ({words} words)")
        if ru:
            continue
        cats = []
        for cat in CATEGORIES:
            cw, cn = score_wer(norm_words, edit_ops, [r for r in rows if r.get("category") == cat], hyps)
            cats.append(f"{cat} {cw:.2f} ({cn})")
        print(f"{'':24s} by category: " + ", ".join(cats))
    if ru:
        return
    print("\nreference (python arm E, its own WAVs re-scored on the same expected forms and scorer): kitten-nano 4.50% / 4.035,"
          " kitten-mini 4.09% / 3.996, kokoro 3.32% / 4.499; by category - oov: kitten-nano 9.34, kitten-mini 12.06, kokoro 12.06;"
          " numeric: kitten-nano 6.25, kitten-mini 8.63, kokoro 4.76; ljspeech: kitten-nano 6.60, kitten-mini 2.70, kokoro 2.43")


if __name__ == "__main__":
    main()
