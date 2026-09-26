"""The reference TTS number-maker: time the kokoro and KittenTTS packages on the g2p corpus's
sentences, generation only - the phoneme string goes in as the fixture spells it, so the front
end is out of the compare - and print the mean generation wall per sentence and the real-time
factor, the alternate row every das TTS timing in PERF_LEDGER.md is held against.
    python tts_ref_bench.py --models kokoro-82m:af_heart kitten-nano:Bella kitten-mini:Bella \\
        [--device mps] [--threads 8] [--limit 20] [--passes 3]
Every sentence is timed `passes` times and the best kept; the mean over the sentences is the
figure. Needs the g2p experiment's venv (`~/Work/tts-ab/g2p/.venv-g2p`: kokoro, kittentts,
onnxruntime, torch). The kitten voices are the KittenTTS pack names (`Bella` = expr-voice-5-f).
"""
import argparse, json, os, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
FIXTURE = os.path.join(HERE, "..", "tests", "_tts_fixtures", "g2p_corpus.json")
KITTEN_REPOS = {"kitten-nano": "KittenML/kitten-tts-nano-0.8", "kitten-mini": "KittenML/kitten-tts-mini-0.8"}
KITTEN_VOICES = {"Bella": "expr-voice-5-f", "Jasper": "expr-voice-5-m"}
SR = 24000


def kokoro_synth(voice, device):
    import torch
    from kokoro import KModel, KPipeline
    model = KModel(repo_id="hexgrad/Kokoro-82M").to(device).eval()
    pipe = KPipeline(lang_code="a", repo_id="hexgrad/Kokoro-82M", model=False)
    pack = pipe.load_voice(voice)

    def run(ps):
        ref = pack[len(ps) - 1]
        with torch.no_grad():
            out = model(ps, ref, 1.0)
        audio = out.audio if hasattr(out, "audio") else out   # the package's newer builds return the tensor bare
        if device != "cpu":
            torch.mps.synchronize() if device == "mps" else torch.cuda.synchronize()
        return int(audio.shape[-1])

    return run


def kitten_synth(name, voice, threads, device):
    import numpy as np
    import onnxruntime as ort
    from huggingface_hub import hf_hub_download
    from kittentts.onnx_model import TextCleaner, basic_english_tokenize
    repo = KITTEN_REPOS[name]
    cfg = json.load(open(hf_hub_download(repo, "config.json")))
    so = ort.SessionOptions()
    so.intra_op_num_threads = threads
    # the CUDA provider when asked for it and present (onnxruntime-gpu), else the CPU one - the row names which ran
    providers = ["CUDAExecutionProvider", "CPUExecutionProvider"] if device == "cuda" else ["CPUExecutionProvider"]
    sess = ort.InferenceSession(hf_hub_download(repo, cfg["model_file"]), sess_options=so, providers=providers)
    print(f"  {name}: onnxruntime providers {sess.get_providers()}", flush=True)
    voices = np.load(hf_hub_download(repo, cfg["voices"]))
    vkey = KITTEN_VOICES.get(voice, voice)
    speed = cfg.get("speed_priors", {}).get(vkey, 1.0)
    cleaner = TextCleaner()

    def run(ps):
        toks = [0] + cleaner(" ".join(basic_english_tokenize(ps))) + [10, 0]
        ref_id = min(len(ps), voices[vkey].shape[0] - 1)
        audio = sess.run(None, {"input_ids": np.array([toks], dtype=np.int64),
                                "style": voices[vkey][ref_id:ref_id + 1],
                                "speed": np.array([speed], dtype=np.float32)})[0]
        return int(audio.shape[-1])

    return run


def _installed(package):
    import importlib.metadata as md
    try:
        md.version(package)
        return True
    except md.PackageNotFoundError:
        return False


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--models", nargs="+", default=["kokoro-82m:af_heart", "kitten-nano:Bella", "kitten-mini:Bella"])
    ap.add_argument("--device", default="cpu", help="the kokoro torch device (cpu, mps, cuda); cuda also asks onnxruntime for its CUDA provider on the kitten models")
    ap.add_argument("--threads", type=int, default=8, help="torch and onnxruntime intra-op threads")
    ap.add_argument("--limit", type=int, default=20)
    ap.add_argument("--passes", type=int, default=3)
    ap.add_argument("--fixture", default=FIXTURE)
    a = ap.parse_args()
    import torch
    torch.set_num_threads(a.threads)
    rows = json.load(open(a.fixture))
    if a.limit > 0:
        rows = rows[:a.limit]
    import importlib.metadata as md
    versions = " ".join(f"{p} {md.version(p)}" for p in ("torch", "kokoro", "kittentts", "onnxruntime", "onnxruntime-gpu") if _installed(p))
    print("ref bench:", " ".join(sys.argv[1:]), f"({len(rows)} sentences; {versions})", flush=True)
    for spec in a.models:
        name, voice = spec.split(":")
        run = kokoro_synth(voice, a.device) if name == "kokoro-82m" else kitten_synth(name, voice, a.threads, a.device)
        run(rows[0]["ps_espeak"])   # the first call pays the session's warm-up
        gen_s, audio_s = 0.0, 0.0
        for r in rows:
            best, n = None, 0
            for _ in range(a.passes):
                t0 = time.perf_counter()
                n = run(r["ps_espeak"])
                dt = time.perf_counter() - t0
                best = dt if best is None or dt < best else best
            gen_s += best
            audio_s += n / SR
        ms = 1000.0 * gen_s / len(rows)
        print(f"{name} {voice}: {len(rows)} sentences, {audio_s:.1f} s of audio, {ms:.0f} ms a sentence "
              f"(rtf {gen_s / audio_s:.4f}), best of {a.passes} passes each", flush=True)


if __name__ == "__main__":
    main()
