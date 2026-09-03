"""The KittenTTS oracle: onnxruntime on the f32 twin of each checkpoint, its sine-source noise
supplied as inputs, per-stage activations dumped for the das port to match to 1e-4.

    python kitten_oracle.py --out ~/Work/llama.cpp/models/tts_oracle [--sizes nano,mini]
                            [--voices expr-voice-2-f,expr-voice-5-m] [--count 20]

Per (size, sentence, voice) one `<sentence-id>_<voice>.bin` lands under `<out>/kitten_<size>/`
beside a `manifest.json`: the token ids the reference TextCleaner produced from the corpus's
espeak-inventory phonemes, the style row and speed the reference driver would feed
(`speed_priors` applied), the two noise tensors, the waveform, and - for the first
`--stage-cases` sentences - the stage tensors named in STAGES (the bring-up set). The
graph runs twice per case: once to learn the noise shapes (they follow the predicted
duration), once with the noise as inputs. Regenerating this store is the traceability of
every `tts_oracle/` file a test reads.
"""
import argparse
import json
import os
import struct
import sys

import numpy as np
import onnxruntime as ort

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import kitten_graph as kg  # noqa: E402

FIXTURE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "tests", "_tts_fixtures", "g2p_corpus.json")

from kittentts.onnx_model import TextCleaner, basic_english_tokenize  # noqa: E402  (the reference driver)

_CLEANER = TextCleaner()


def reference_token_ids(ps_espeak):
    """The reference driver: re-space punctuation, drop unknown symbols, wrap in pad / ellipsis /
    pad."""
    return [0] + _CLEANER(" ".join(basic_english_tokenize(ps_espeak))) + [10, 0]


# stage tensors, by the value name both graphs share (the quantizer kept the float output names)
STAGES = {
    "bert": "/bert_encoder/Add_output_0",
    "text_enc": "/text_encoder/Transpose_4_output_0",
    "dur_raw": "/Div_output_0",
    "align": "/ScatterND_output_0",
    "f0": "/F0_proj/Conv_output_0",
    "n": "/N_proj/Conv_output_0",
    "asr_res": "/decoder/asr_res/asr_res.0/Conv_output_0",
    "dec_in": "/decoder/Concat_1_output_0",
    "gen_in": "@/decoder/generator/LeakyRelu",   # "@node": that node's first input (the name a quantizer rewrites)
    "har": "/decoder/generator/m_source/l_tanh/Tanh_output_0",
    "stft_re": "/decoder/generator/Conv_output_0",
    "ups0": "/decoder/generator/ups.0/ConvTranspose_output_0",
    "ups1": "/decoder/generator/ups.1/ConvTranspose_output_0",
    "conv_post": "/decoder/generator/conv_post/Conv_output_0",
}

MAGIC = b"TTSO"
DTYPES = {np.dtype(np.float32): 0, np.dtype(np.int32): 1}


def write_dump(path, records):
    with open(path, "wb") as f:
        f.write(MAGIC + struct.pack("<II", 1, len(records)))
        for name, arr in records.items():
            arr = np.ascontiguousarray(arr)
            if arr.dtype == np.int64:
                arr = arr.astype(np.int32)
            nb = name.encode("utf8")
            f.write(struct.pack("<I", len(nb)) + nb)
            f.write(struct.pack("<II", DTYPES[arr.dtype], arr.ndim))
            f.write(struct.pack(f"<{arr.ndim}i", *arr.shape))
            f.write(arr.tobytes())


def pick_sentences(rows, count):
    """Deterministic spread over the corpus: rows whose normalized text is 20..110 characters,
    every k-th by corpus order."""
    ok = [r for r in rows if 20 <= len(r["norm"]) <= 110]
    step = max(1, len(ok) // count)
    return ok[::step][:count]


def resolve_stages(model):
    """STAGES with every "@node" entry replaced by that node's first input name."""
    by_name = {n.name: n for n in model.graph.node}
    out = {}
    for key, v in STAGES.items():
        out[key] = by_name[v[1:]].input[0] if v.startswith("@") else v
    return out


def sessions(size):
    m = kg.dequantize(kg.load(size))
    probe = m.__class__()
    probe.CopyFrom(m)
    kg.expose(probe, ["/decoder/generator/m_source/l_sin_gen/Mul_10_output_0"])
    first = ort.InferenceSession(probe.SerializeToString(), providers=["CPUExecutionProvider"])
    shapes = kg.noise_as_inputs(m)
    stages = resolve_stages(m)
    kg.expose(m, list(stages.values()))
    second = ort.InferenceSession(m.SerializeToString(), providers=["CPUExecutionProvider"])
    return first, second, shapes, stages


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True)
    ap.add_argument("--sizes", default="nano,mini")
    ap.add_argument("--voices", default="expr-voice-2-f,expr-voice-5-m")
    ap.add_argument("--count", type=int, default=20)
    ap.add_argument("--seed-base", type=int, default=20260902)
    ap.add_argument("--stage-cases", type=int, default=4, help="sentences (x voices) that also carry the per-stage tensors")
    a = ap.parse_args()
    rows = json.load(open(FIXTURE, encoding="utf8"))
    picked = pick_sentences(rows, a.count)
    for size in a.sizes.split(","):
        cfg = json.load(open(kg.SNAPSHOTS[size] + "config.json", encoding="utf8"))
        voices = np.load(kg.SNAPSHOTS[size] + "voices.npz")
        first, second, _, stages = sessions(size)
        out_dir = os.path.join(a.out, f"kitten_{size}")
        os.makedirs(out_dir, exist_ok=True)
        manifest = {"size": size, "revision": kg.REVISIONS[size], "seed_base": a.seed_base, "cases": []}
        for si, r in enumerate(picked):
            ids = np.array([reference_token_ids(r["ps_espeak"])], dtype=np.int64)
            for vi, voice in enumerate(a.voices.split(",")):
                speed = np.float32(1.0 * cfg.get("speed_priors", {}).get(voice, 1.0))
                row = min(len(r["norm"]), voices[voice].shape[0] - 1)
                style = voices[voice][row:row + 1].astype(np.float32)
                feed = {"input_ids": ids, "style": style, "speed": np.array([speed], dtype=np.float32)}
                _, _, mul10 = first.run(None, feed)
                seed = a.seed_base + 1000 * si + vi
                rng = np.random.default_rng(seed)
                noise_u = rng.random((1, mul10.shape[2]), dtype=np.float32)
                noise_n = rng.standard_normal(mul10.shape, dtype=np.float32)
                feed.update({"noise_uniform": noise_u, "noise_normal": noise_n})
                outs = second.run(None, feed)
                names = [o.name for o in second.get_outputs()]
                by = dict(zip(names, outs))
                records = {
                    "input_ids": ids[0].astype(np.int32),
                    "style": style[0],
                    "style_row": np.array([row], dtype=np.int32),
                    "speed": np.array([speed], dtype=np.float32),
                    "noise_uniform": noise_u[0],
                    "noise_normal": noise_n[0],
                    "duration": by["duration"].astype(np.int32),
                    "waveform": by["waveform"].astype(np.float32),
                }
                if si < a.stage_cases:
                    for key, vname in stages.items():
                        records[key] = np.squeeze(by[vname], axis=0) if by[vname].shape[0] == 1 and by[vname].ndim > 1 else by[vname]
                fname = f"{r['id']}_{voice}.bin"
                write_dump(os.path.join(out_dir, fname), records)
                manifest["cases"].append({"file": fname, "id": r["id"], "text": r["text"], "norm": r["norm"],
                                          "ps_espeak": r["ps_espeak"], "voice": voice, "speed": float(speed),
                                          "style_row": row, "seed": seed, "n_tokens": int(ids.shape[1]),
                                          "n_samples": int(by["waveform"].shape[-1]), "duration_sum": int(by["duration"].sum()),
                                          "stages": si < a.stage_cases})
                print(f"{size} {r['id']} {voice}: {ids.shape[1]} tokens, {int(by['duration'].sum())} frames, {by['waveform'].shape[-1]} samples")
        with open(os.path.join(out_dir, "manifest.json"), "w", encoding="utf8") as f:
            json.dump(manifest, f, indent=1, ensure_ascii=False)
        with open(os.path.join(out_dir, "LICENSE"), "w", encoding="utf8") as f:
            f.write(f"kitten_{size} oracle dumps - derived from KittenML/kitten-tts-{size}-0.8 @ {kg.REVISIONS[size]} "
                    "(Apache License 2.0) run through onnxruntime; see the dasLLAMA THIRD_PARTY_NOTICES.md\n")
        print(f"{size}: {len(manifest['cases'])} cases -> {out_dir}")


if __name__ == "__main__":
    main()
