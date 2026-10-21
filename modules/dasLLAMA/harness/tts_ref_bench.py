"""The reference TTS number-maker: time each family's own package on the g2p corpus's sentences,
generation only, and print the mean generation wall per sentence and the real-time factor - the
reference row every das TTS cell (`lcpp_bench --tts`) is held against.
    python tts_ref_bench.py --models kokoro-82m:af_heart kitten-nano:Bella pocket-en:alba \\
        [--engine torch|mlx] [--device cpu|mps|cuda] [--threads 8] [--limit 20] [--passes 3]
The phoneme families take the fixture's `ps_espeak` string, so their front end is out of the
compare; Pocket reads the sentence text, as it does in its package. Every sentence is timed
`passes` times and the best kept, after one untimed warm-up call; the figure is the mean over the
sentences. Per model one machine line `BENCH\t<model>\tg2p<N>\t<audio_s>\t0\t<ms>\t<rtf>` carries
the summed audio seconds and best-of generation ms, and one `VERSIONS\t<line>` line names the
packages the run took. `--engine torch` (the default) runs the packages - kokoro's torch `KModel`,
KittenTTS's ONNX files under onnxruntime, Kyutai's `pocket-tts` - from the `tts` venv
(`setup_asr_rig --venvs`); `--engine mlx` runs mlx-audio's ports on the Apple GPU from the `mlx`
venv. The kitten voices are the KittenTTS pack names (`Bella` = expr-voice-5-f).
"""
import argparse, json, os, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
FIXTURE = os.path.join(HERE, "..", "tests", "_tts_fixtures", "g2p_corpus.json")
KOKORO_REPO, KOKORO_REV = "hexgrad/Kokoro-82M", "f3ff3571791e39611d31c381e3a41a3af07b4987"
KITTEN_REPOS = {"kitten-nano": ("KittenML/kitten-tts-nano-0.8", "7a1db645b1f3ab9420761d87428e042b9cec3f26"),
                "kitten-mini": ("KittenML/kitten-tts-mini-0.8", "c02725660cea441db4c383af69f1f26f5cd00947")}
KITTEN_VOICES = {"Bella": "expr-voice-5-f", "Jasper": "expr-voice-5-m"}
POCKET_LANGUAGES = {"pocket-en": "english_2026-04"}
MLX_REPOS = {"kokoro-82m": ("mlx-community/Kokoro-82M-bf16", "a71e4d38b236d968966a2002c4c895dbd12b1c3c"),
             "kitten-nano": ("mlx-community/kitten-tts-nano-0.8", "f57e91b190ca3323aa94c7bbdde4565343d79588"),
             "kitten-mini": ("mlx-community/kitten-tts-mini-0.8", "a22cce835c4af60bf9a50779124cf8cc34b0c36e"),
             "pocket-en": ("mlx-community/pocket-tts-8bit", "ee0d088c10729a33640bac050b8796993f468a53")}
SR = 24000


def torch_sync(device):
    import torch
    if device == "mps":
        torch.mps.synchronize()
    elif device == "cuda":
        torch.cuda.synchronize()


def kokoro_synth(voice, device):
    import torch
    from huggingface_hub import hf_hub_download
    from kokoro import KModel
    config = hf_hub_download(KOKORO_REPO, "config.json", revision=KOKORO_REV)
    weights = hf_hub_download(KOKORO_REPO, "kokoro-v1_0.pth", revision=KOKORO_REV)
    model = KModel(repo_id=KOKORO_REPO, config=config, model=weights).to(device).eval()
    pack = torch.load(hf_hub_download(KOKORO_REPO, f"voices/{voice}.pt", revision=KOKORO_REV), weights_only=True).to(device)

    def run(row):
        ps = row["ps_espeak"]
        with torch.no_grad():
            out = model(ps, pack[len(ps) - 1], 1.0)
        audio = out.audio if hasattr(out, "audio") else out   # the package's newer builds return the tensor bare
        torch_sync(device)
        return int(audio.shape[-1])

    return run


def kitten_tokens(ps, cleaner, tokenize):
    """the KittenTTS driver's ids for a phoneme string - both engines feed the same list"""
    return [0] + cleaner(" ".join(tokenize(ps))) + [10, 0]


def kitten_synth(name, voice, threads, device):
    import numpy as np
    import onnxruntime as ort
    from huggingface_hub import hf_hub_download
    from kittentts.onnx_model import TextCleaner, basic_english_tokenize
    cleaner = TextCleaner()
    repo, rev = KITTEN_REPOS[name]
    cfg = json.load(open(hf_hub_download(repo, "config.json", revision=rev)))
    so = ort.SessionOptions()
    so.intra_op_num_threads = threads
    # the CUDA provider when asked for it and present (onnxruntime-gpu), else the CPU one - the row names which ran
    providers = ["CUDAExecutionProvider", "CPUExecutionProvider"] if device == "cuda" else ["CPUExecutionProvider"]
    sess = ort.InferenceSession(hf_hub_download(repo, cfg["model_file"], revision=rev), sess_options=so, providers=providers)
    print(f"  {name}: onnxruntime providers {sess.get_providers()}", flush=True)
    voices = np.load(hf_hub_download(repo, cfg["voices"], revision=rev))
    vkey = KITTEN_VOICES.get(voice, voice)
    speed = cfg.get("speed_priors", {}).get(vkey, 1.0)

    def run(row):
        ps = row["ps_espeak"]
        ref_id = min(len(ps), voices[vkey].shape[0] - 1)
        audio = sess.run(None, {"input_ids": np.array([kitten_tokens(ps, cleaner, basic_english_tokenize)], dtype=np.int64),
                                "style": voices[vkey][ref_id:ref_id + 1],
                                "speed": np.array([speed], dtype=np.float32)})[0]
        return int(audio.shape[-1])

    return run


def pocket_synth(name, voice, device):
    import torch
    from pocket_tts import TTSModel
    model = TTSModel.load_model(language=POCKET_LANGUAGES[name])
    state = model.get_state_for_audio_prompt(voice)

    def run(row):
        torch.manual_seed(0)
        audio = model.generate_audio(state, row["text"])
        torch_sync(device)
        return int(audio.shape[-1])

    return run


def mlx_synth(name, voice):
    import mlx.core as mx
    from huggingface_hub import snapshot_download
    from mlx_audio.tts.utils import load_model
    from pathlib import Path
    repo, rev = MLX_REPOS[name]
    root = Path(snapshot_download(repo, revision=rev))
    model = load_model(root)
    if name == "kokoro-82m":
        pack = mx.load(str(root / "voices" / f"{voice}.safetensors"))["voice"]   # the pack alone: the text pipeline is out of the compare

        def run(row):
            ps = row["ps_espeak"]
            return int(model(ps, pack[len(ps) - 1], 1.0).shape[-1])

        return run
    if name.startswith("kitten"):
        from mlx_audio.tts.models.kitten_tts.kitten_tts import basic_english_tokenize
        vkey = KITTEN_VOICES.get(voice, voice)
        speed = model.speed_priors.get(vkey, 1.0)

        def run(row):
            ps = row["ps_espeak"]
            ref_id = min(len(ps), model.voices[vkey].shape[0] - 1)
            ids = mx.array([kitten_tokens(ps, model._text_cleaner, basic_english_tokenize)], dtype=mx.int32)
            return int(model(ids, mx.array(model.voices[vkey][ref_id:ref_id + 1]), speed).shape[-1])

        return run
    state = model.get_state_for_audio_prompt(voice)

    def run(row):
        mx.random.seed(0)
        audio = model.generate_audio(state, row["text"])
        mx.eval(audio)
        return int(audio.shape[-1])

    return run


def package_versions(engine):
    import importlib.metadata as md
    names = (("mlx", "mlx-audio", "misaki") if engine == "mlx"
             else ("torch", "kokoro", "kittentts", "pocket-tts", "onnxruntime", "onnxruntime-gpu"))
    found = []
    for p in names:
        try:
            found.append(f"{p} {md.version(p)}")
        except md.PackageNotFoundError:
            pass
    return " / ".join(found)


def make_synth(name, voice, a):
    if a.engine == "mlx":
        return mlx_synth(name, voice)
    if name == "kokoro-82m":
        return kokoro_synth(voice, a.device)
    if name in POCKET_LANGUAGES:
        return pocket_synth(name, voice, a.device)
    return kitten_synth(name, voice, a.threads, a.device)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--models", nargs="+", default=["kokoro-82m:af_heart", "kitten-nano:Bella", "kitten-mini:Bella"])
    ap.add_argument("--engine", default="torch", choices=["torch", "mlx"], help="the packages (torch, onnxruntime) or mlx-audio's ports")
    ap.add_argument("--device", default="cpu", help="the torch device (cpu, mps, cuda); cuda also asks onnxruntime for its CUDA provider on the kitten models")
    ap.add_argument("--threads", type=int, default=8, help="torch and onnxruntime intra-op threads")
    ap.add_argument("--limit", type=int, default=20)
    ap.add_argument("--passes", type=int, default=3)
    ap.add_argument("--fixture", default=FIXTURE)
    a = ap.parse_args()
    if a.engine == "torch":
        import torch
        torch.set_num_threads(a.threads)
    rows = json.load(open(a.fixture, encoding="utf8"))
    if a.limit > 0:
        rows = rows[:a.limit]
    versions = package_versions(a.engine)
    print("ref bench:", " ".join(sys.argv[1:]), f"({len(rows)} sentences; {versions})", flush=True)
    where = "--engine mlx" if a.engine == "mlx" else f"--device {a.device}"
    print(f"VERSIONS\t{versions} (tts_ref_bench.py {where}, requirements-{'mlx' if a.engine == 'mlx' else 'tts'}.txt)", flush=True)
    for spec in a.models:
        name, voice = spec.split(":")
        run = make_synth(name, voice, a)
        run(rows[0])   # the first call pays the session's warm-up
        gen_s, audio_s = 0.0, 0.0
        for r in rows:
            best, n = None, 0
            for _ in range(a.passes):
                t0 = time.perf_counter()
                n = run(r)
                dt = time.perf_counter() - t0
                best = dt if best is None or dt < best else best
            gen_s += best
            audio_s += n / SR
            print(f"  {r['id']} {n / SR:6.2f} s audio in {best * 1000.0:7.1f} ms", flush=True)
        ms = 1000.0 * gen_s / len(rows)
        print(f"{name} {voice}: {len(rows)} sentences, {audio_s:.1f} s of audio, {ms:.0f} ms a sentence "
              f"(rtf {gen_s / audio_s:.4f}), best of {a.passes} passes each", flush=True)
        print(f"BENCH\t{name}\tg2p{len(rows)}\t{audio_s:.4f}\t0\t{gen_s * 1000.0:.2f}\t{gen_s / audio_s:.5f}", flush=True)


if __name__ == "__main__":
    main()
