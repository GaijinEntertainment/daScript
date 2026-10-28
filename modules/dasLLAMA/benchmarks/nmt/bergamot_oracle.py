#!/usr/bin/env python3
"""Per-stage Bergamot oracle dump: the specification the das translation family is tested against.

Runs the patched upstream marian-decoder (setup_asr_rig.das --refs, benchmarks/nmt/patches) greedy on
one CPU thread over each line of a fixture, and writes one .npy per stage plus the tokens.

  .venv-nmt/bin/python -I bergamot_oracle.py --marian $MARIAN/inference/build/marian-decoder \
      --model $DASLLAMA_MODELS_DIR/bergamot/ru-en-base-memory --fixture corpus/ru.txt \
      --out $DASLLAMA_MODELS_DIR/nmt_oracle/ru-en-base-memory/ru_f32

Variants:
  f32     marian's f32 graph over the int8 weights dequantized (weight = int8 / quantMult): the weights
          the das image serves, f32 activations. Stage dumps + tokens.
  int8    the shipped file under Firefox's `int8shiftAlphaAll` (static activation scales; ARM drops the
          shift): what the browser translates. Tokens only - a quality reference, not a stage bar.

Written per line under <out>/line_NN/ (float32, time-major):
  enc_emb        [S, d]     scaled source embedding + sinusoid
  enc_lN         [S, d]     after encoder layer N (post FFN LayerNorm)
  dec_emb        [T, d]     per step: target embedding + sinusoid
  dec_lN_rnn     [T, d]     per step: after layer N's SSRU block
  dec_lN_ctx     [T, d]     per step: after layer N's cross-attention
  dec_lN_ffn     [T, d]     per step: after layer N's FFN
  logits         [T, V]     per step: tied output projection + bias
  tokens.json    src, src_ids (eos-terminated), ids (greedy, eos-terminated), text

--spm-edge <fixture> also writes <out>/spm_edge.json: the source vocabulary's ids for each line of a
normalizer fixture (corpus/spm_edge.txt - whitespace runs, NFKC folds, emoji byte fallback), no marian run.
"""
import argparse
import glob
import json
import os
import struct
import subprocess
import tempfile

import numpy as np
import sentencepiece as spm

INTGEMM8 = 0x4101
FLOAT32 = 0x0404


def read_marian(path):
    f = open(path, "rb").read()
    _, n = struct.unpack_from("<QQ", f, 0)
    o = 16
    heads = [struct.unpack_from("<QQQQ", f, o + 32 * i) for i in range(n)]
    o += 32 * n
    names = []
    for nl, _, _, _ in heads:
        names.append(f[o:o + nl])
        o += nl
    shapes = []
    for _, _, r, _ in heads:
        shapes.append(struct.unpack_from("<%di" % r, f, o))
        o += 4 * r
    pad, = struct.unpack_from("<Q", f, o)
    o += 8 + pad
    items = []
    for (_, t, _, b), name, shape in zip(heads, names, shapes):
        items.append((name, t, shape, f[o:o + b]))
        o += b
    return items


def write_marian(path, items):
    heads = b"".join(struct.pack("<QQQQ", len(nm), t, len(sh), len(blob)) for nm, t, sh, blob in items)
    names = b"".join(nm for nm, _, _, _ in items)
    shapes = b"".join(struct.pack("<%di" % len(sh), *sh) for _, _, sh, _ in items)
    head = struct.pack("<QQ", 1, len(items)) + heads + names + shapes
    pad = (256 - (len(head) + 8) % 256) % 256
    with open(path, "wb") as out:
        out.write(head + struct.pack("<Q", pad) + b"\0" * pad)
        for _, _, _, blob in items:
            out.write(blob)


def dequantize(src, dst):
    """The f32 twin of an intgemm8 file: every matrix int8 / quantMult, stored [in][out] - intgemm8 keeps
    each one transposed, `Wemb` alone untransposed; the `_QuantMultA` activation scales are dropped."""
    items = []
    for name, t, shape, blob in read_marian(src):
        if name.rstrip(b"\0").endswith(b"_QuantMultA"):
            continue
        if t == INTGEMM8:
            count = int(np.prod(shape))
            w = np.frombuffer(blob[:count], dtype=np.int8).astype(np.float32) / struct.unpack_from("<f", blob, count)[0]
            if b"Wemb" not in name:
                w = w.reshape(shape[1], shape[0]).T
            blob = np.ascontiguousarray(w, dtype=np.float32).tobytes()
            t = FLOAT32
        items.append((name, t, shape, blob))
    write_marian(dst, items)


def stack_stage(dump, stage):
    steps = sorted(glob.glob(os.path.join(dump, stage + "_*.npy")))
    rows = [np.load(p).reshape(-1, np.load(p).shape[-1]) for p in steps]
    return np.concatenate(rows, axis=0)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--marian", required=True, help="the patched marian-decoder")
    ap.add_argument("--model", required=True, help="a bergamot/<pair>-<tier> directory: model.*.bin + vocab*.spm")
    ap.add_argument("--fixture", required=True, help="one source line per line")
    ap.add_argument("--out", required=True)
    ap.add_argument("--variant", default="f32", help="f32 | int8")
    ap.add_argument("--spm-edge", help="a normalizer fixture: write <out>/spm_edge.json and stop")
    args = ap.parse_args()

    model = glob.glob(os.path.join(args.model, "model.*.intgemm.alphas.bin"))[0]
    vocabs = sorted(glob.glob(os.path.join(args.model, "*vocab.*.spm")))
    src_vocab = next((v for v in vocabs if os.path.basename(v).startswith("srcvocab")), vocabs[0])
    trg_vocab = next((v for v in vocabs if os.path.basename(v).startswith("trgvocab")), vocabs[0])
    os.makedirs(args.out, exist_ok=True)
    if args.spm_edge:
        sp = spm.SentencePieceProcessor(model_file=src_vocab)
        edge = [ln.rstrip("\n") for ln in open(args.spm_edge, encoding="utf-8") if ln.strip()]
        with open(os.path.join(args.out, "spm_edge.json"), "w", encoding="utf-8") as f:
            json.dump({"lines": [{"text": ln, "ids": sp.encode(ln)} for ln in edge]}, f, ensure_ascii=False, indent=1)
        return
    if args.variant == "f32":
        served = os.path.join(args.out, "model.f32.bin")
        if not os.path.exists(served):
            dequantize(model, served)
        precision = []
    else:
        served = model
        precision = ["--gemm-precision", "int8shiftAlphaAll"]

    sp_src = spm.SentencePieceProcessor(model_file=src_vocab)
    sp_trg = spm.SentencePieceProcessor(model_file=trg_vocab)
    cmd = [args.marian, "-m", served, "-v", src_vocab, trg_vocab, "--beam-size", "1", "--cpu-threads", "1",
           "--max-length-factor", "2.0", "--mini-batch", "1", "--maxi-batch", "1", "--skip-cost",
           "--quiet-translation"] + precision
    lines = [ln.rstrip("\n") for ln in open(args.fixture, encoding="utf-8") if ln.strip()]
    for i, line in enumerate(lines):
        line_dir = os.path.join(args.out, f"line_{i:02d}")
        os.makedirs(line_dir, exist_ok=True)
        with tempfile.TemporaryDirectory() as dump:
            env = dict(os.environ, MARIAN_DUMP_DIR=dump) if args.variant == "f32" else dict(os.environ)
            text = subprocess.run(cmd, input=line + "\n", capture_output=True, text=True, env=env,
                                  check=True).stdout.strip()
            ids = []
            if args.variant == "f32":
                stages = sorted({os.path.basename(p).rsplit("_", 1)[0] for p in glob.glob(os.path.join(dump, "*.npy"))})
                for stage in stages:
                    np.save(os.path.join(line_dir, stage + ".npy"), stack_stage(dump, stage))
                ids = [int(x) for x in np.argmax(np.load(os.path.join(line_dir, "logits.npy")), axis=1)]
        src_ids = sp_src.encode(line) + [0]
        with open(os.path.join(line_dir, "tokens.json"), "w", encoding="utf-8") as f:
            json.dump({"variant": args.variant, "src": line, "src_ids": src_ids, "ids": ids, "text": text,
                       "ids_text": sp_trg.decode([x for x in ids if x != 0])}, f, ensure_ascii=False, indent=1)
        print(f"{i:02d}\t{text}")


if __name__ == "__main__":
    main()
