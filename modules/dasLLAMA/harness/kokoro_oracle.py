"""The Kokoro oracle: the PyTorch reference (the `kokoro` package at the pinned snapshot) run
with its sine-source noise captured, per-stage activations dumped for the das port to match.

    python kokoro_oracle.py --out ~/Work/llama.cpp/models/tts_oracle [--voices af_heart,am_michael,bf_emma]
                            [--count 20] [--stage-cases 4]

Same corpus, container and record names as kitten_oracle.py (the das rig reads both): token ids
from the reference vocabulary over the fixture's misaki-inventory phonemes, the style row the
reference pipeline picks (phoneme count less one), the two noise tensors, the waveform, and on
the bring-up set the stage tensors caught by forward hooks. The model runs twice per case: once
to learn the duration (hence the noise shapes), once with the noise supplied.
"""
import argparse
import json
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import kitten_graph as kg  # noqa: E402
from kitten_oracle import FIXTURE, pick_sentences, write_dump  # noqa: E402

UPSAMPLE = 300
N_HARM = 9


class Hooks:
    """Forward hooks on the reference modules, collecting the stage tensors by the das names."""

    def __init__(self, model):
        self.out = {}
        m = model
        self.handles = [
            m.bert_encoder.register_forward_hook(lambda mod, i, o: self.put("bert", o[0])),
            m.text_encoder.register_forward_hook(lambda mod, i, o: self.put("text_enc", o[0])),
            m.predictor.duration_proj.register_forward_hook(lambda mod, i, o: self.put("dur_logits", o[0])),
            m.predictor.F0_proj.register_forward_hook(lambda mod, i, o: self.put("f0", o[0, 0])),
            m.predictor.N_proj.register_forward_hook(lambda mod, i, o: self.put("n", o[0, 0])),
            m.decoder.asr_res.register_forward_hook(lambda mod, i, o: self.put("asr_res", o[0])),
            m.decoder.decode[0].register_forward_pre_hook(lambda mod, i: self.put("dec_in", i[0][0])),
            m.decoder.generator.register_forward_pre_hook(lambda mod, i: self.put("gen_in", i[0][0])),
            m.decoder.generator.m_source.register_forward_hook(lambda mod, i, o: self.put("har", o[0][0, :, 0])),
            m.decoder.generator.ups[0].register_forward_hook(lambda mod, i, o: self.put("ups0", o[0])),
            m.decoder.generator.noise_convs[0].register_forward_hook(lambda mod, i, o: self.put("src0", o[0])),
            m.decoder.generator.noise_res[0].register_forward_hook(lambda mod, i, o: self.put("nres0", o[0])),
            m.decoder.generator.resblocks[0].register_forward_hook(lambda mod, i, o: self.put("rb0", o[0])),
            m.decoder.generator.resblocks[1].register_forward_hook(lambda mod, i, o: self.put("rb1", o[0])),
            m.decoder.generator.resblocks[2].register_forward_hook(lambda mod, i, o: self.put("rb2", o[0])),
            m.decoder.generator.ups[1].register_forward_hook(lambda mod, i, o: self.put("ups1", o[0])),
            m.decoder.generator.conv_post.register_forward_hook(lambda mod, i, o: self.put("conv_post", o[0])),
        ]

    def put(self, name, t):
        self.out[name] = t.detach().cpu().numpy().astype(np.float32).copy()

    def remove(self):
        for h in self.handles:
            h.remove()


class CapturedNoise:
    """torch.rand / torch.randn_like replaced for one forward: the sine source draws our arrays."""

    def __init__(self, torch, uniform, normal):
        self.torch, self.uniform, self.normal = torch, uniform, normal
        self.rand, self.randn_like = torch.rand, torch.randn_like

    def __enter__(self):
        t = self.torch
        u, n = t.from_numpy(self.uniform), t.from_numpy(self.normal)

        def rand(*shape, **kw):
            assert tuple(shape) == tuple(u.shape), (shape, u.shape)
            return u.clone()

        def randn_like(x, **kw):
            if tuple(x.shape) == tuple(n.shape):
                return n.clone()
            return t.zeros_like(x)   # the unused noise branch of SourceModuleHnNSF
        t.rand, t.randn_like = rand, randn_like
        return self

    def __exit__(self, *a):
        self.torch.rand, self.torch.randn_like = self.rand, self.randn_like


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True)
    ap.add_argument("--voices", default="af_heart,am_michael,bf_emma")
    ap.add_argument("--count", type=int, default=20)
    ap.add_argument("--seed-base", type=int, default=20260903)
    ap.add_argument("--stage-cases", type=int, default=4)
    a = ap.parse_args()
    import torch
    from kokoro.model import KModel
    torch.set_num_threads(8)
    snap = kg.KOKORO_SNAPSHOT
    cfg = json.load(open(snap + "config.json", encoding="utf8"))
    vocab = cfg["vocab"]
    model = KModel(repo_id="hexgrad/Kokoro-82M", config=snap + "config.json", model=snap + "kokoro-v1_0.pth").eval()
    rows = json.load(open(FIXTURE, encoding="utf8"))
    picked = pick_sentences(rows, a.count)
    out_dir = os.path.join(a.out, "kokoro")
    os.makedirs(out_dir, exist_ok=True)
    manifest = {"size": "82m", "revision": kg.KOKORO_REVISION, "seed_base": a.seed_base, "cases": []}
    voices = {v: torch.load(snap + f"voices/{v}.pt", map_location="cpu", weights_only=True) for v in a.voices.split(",")}
    for si, r in enumerate(picked):
        ps = r["ps_misaki"]
        ids = [0] + [vocab[c] for c in ps if c in vocab] + [0]
        input_ids = torch.LongTensor([ids])
        for vi, (voice, pack) in enumerate(voices.items()):
            row = min(len(ps) - 1, pack.shape[0] - 1)
            ref_s = pack[row]   # [1, 256]
            speed = 1.0
            with torch.no_grad():
                _, dur = model.forward_with_tokens(input_ids, ref_s, speed)
            n_samples = int(dur.sum()) * 2 * UPSAMPLE
            seed = a.seed_base + 1000 * si + vi
            rng = np.random.default_rng(seed)
            noise_u = rng.random((1, N_HARM), dtype=np.float32)
            noise_n = rng.standard_normal((1, n_samples, N_HARM), dtype=np.float32)
            hooks = Hooks(model)
            with torch.no_grad(), CapturedNoise(torch, noise_u, noise_n):
                wave, dur2 = model.forward_with_tokens(input_ids, ref_s, speed)
            hooks.remove()
            assert int(dur2.sum()) == int(dur.sum())
            st = hooks.out
            dur_raw = (1.0 / (1.0 + np.exp(-st.pop("dur_logits")))).sum(axis=-1) / speed
            har_t = torch.from_numpy(st["har"].copy())
            X = torch.stft(har_t, 20, 5, 20, window=torch.hann_window(20, periodic=True), return_complex=True)
            st["har_spec"] = torch.cat([torch.abs(X), torch.angle(X)], dim=0).numpy().astype(np.float32)
            records = {
                "input_ids": np.array(ids, dtype=np.int32),
                "style": ref_s[0].numpy().astype(np.float32),
                "style_row": np.array([row], dtype=np.int32),
                "speed": np.array([speed], dtype=np.float32),
                "noise_uniform": noise_u[0],
                "noise_normal": noise_n[0],
                "duration": dur2.numpy().astype(np.int32),
                "waveform": wave.numpy().astype(np.float32),
            }
            if si < a.stage_cases:
                records["dur_raw"] = dur_raw.astype(np.float32)
                records.update(st)
            fname = f"{r['id']}_{voice}.bin"
            write_dump(os.path.join(out_dir, fname), records)
            manifest["cases"].append({"file": fname, "id": r["id"], "text": r["text"], "norm": r["norm"],
                                      "ps_espeak": ps, "voice": voice, "speed": speed, "style_row": row, "seed": seed,
                                      "n_tokens": len(ids), "n_samples": int(wave.shape[-1]),
                                      "duration_sum": int(dur2.sum()), "stages": si < a.stage_cases})
            print(f"kokoro {r['id']} {voice}: {len(ids)} tokens, {int(dur2.sum())} frames, {wave.shape[-1]} samples")
    with open(os.path.join(out_dir, "manifest.json"), "w", encoding="utf8") as f:
        json.dump(manifest, f, indent=1, ensure_ascii=False)
    with open(os.path.join(out_dir, "LICENSE"), "w", encoding="utf8") as f:
        f.write(f"kokoro oracle dumps - derived from hexgrad/Kokoro-82M @ {kg.KOKORO_REVISION} (Apache License 2.0) "
                "run through PyTorch; see the dasLLAMA THIRD_PARTY_NOTICES.md\n")
    print(f"kokoro: {len(manifest['cases'])} cases -> {out_dir}")


if __name__ == "__main__":
    main()
