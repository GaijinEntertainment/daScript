#!/usr/bin/env python3
"""Per-stage GigaAM v3 oracle dump: the specification the das family is tested against.

Runs the upstream `gigaam` package on CPU in fp32 with every encoder weight rounded through
f16 (the GGUF carrier's precision), and writes one .npy per stage plus the transcript.

  .venv-gigaam/bin/python -I gigaam_oracle.py --variant e2e_rnnt --wav corpus/ru_short.wav \
      --out $DASLLAMA_MODELS_DIR/asr_oracle/gigaam/ru_short_e2e_rnnt

The tests read the dumps from `asr_oracle/gigaam/<clip>_<variant>/` under the model root (tests/_model_tier.das,
asr_oracle_dir), the clips from the corpus setup_asr_rig.das fetches.

Stages written (all float32, time-major where it applies):
  mel            [T, 64]        log-mel after the preprocessor (natural log, clamp 1e-9)
  front          [T/4, 768]     after the two conv1d subsampling stages (+ReLU)
  layer_NN       [T/4, 768]     after conformer layer NN (post norm_out)
  encoded        [T/4, 768]     encoder output
  ctc_logprobs   [T/4, C]       CTC only
  tokens.json    ids, frames, text
"""
import argparse
import json
import os
import warnings

import numpy as np
import torch

warnings.filterwarnings("ignore")


def round_f16(model):
    with torch.no_grad():
        for name, p in model.encoder.named_parameters():
            if name.endswith(".bias") or "norm" in name or name.startswith("pre_encode"):
                continue
            if p.dim() <= 1:
                continue
            p.copy_(p.half().float())
        if hasattr(model, "head"):
            for name, p in model.head.named_parameters():
                if name.endswith(".bias") or p.dim() <= 1:
                    continue
                p.copy_(p.half().float())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--variant", default="e2e_rnnt", help="ctc | rnnt | e2e_ctc | e2e_rnnt")
    ap.add_argument("--wav", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--no-f16", action="store_true", help="keep the fp32 weights (not the carrier's precision)")
    args = ap.parse_args()

    import gigaam

    torch.set_num_threads(max(1, os.cpu_count() // 2))
    model = gigaam.load_model(args.variant, device="cpu", fp16_encoder=False)
    model.eval()
    if not args.no_f16:
        round_f16(model)
    os.makedirs(args.out, exist_ok=True)

    wav, length = model.prepare_wav(args.wav)
    taps = {}

    def tap(name):
        def hook(_m, _i, out):
            t = out[0] if isinstance(out, tuple) else out
            taps[name] = t.detach().float().cpu().numpy()
        return hook

    enc = model.encoder
    enc.pre_encode.register_forward_hook(tap("front"))
    for i, layer in enumerate(enc.layers):
        layer.register_forward_hook(tap(f"layer_{i:02d}"))

    with torch.no_grad():
        mel, mel_len = model.preprocessor(wav, length)
        np.save(os.path.join(args.out, "mel.npy"), mel[0].T.contiguous().numpy())
        encoded, enc_len = enc(mel, mel_len)
        np.save(os.path.join(args.out, "encoded.npy"), encoded[0].T.contiguous().numpy())
        front = taps["front"]
        np.save(os.path.join(args.out, "front.npy"), front[0])  # pre_encode returns [B, T/4, D]
        for i in range(len(enc.layers)):
            np.save(os.path.join(args.out, f"layer_{i:02d}.npy"), taps[f"layer_{i:02d}"][0])
        if str(model.cfg.get("model_class", "")) == "ctc":
            lp = model.head(encoder_output=encoded)
            np.save(os.path.join(args.out, "ctc_logprobs.npy"), lp[0].numpy())
        res = model.decoding.decode(model.head, encoded, enc_len)[0]
        text, ids, frames = res[0], [int(x) for x in res[1]], [int(x) for x in res[2]]

    with open(os.path.join(args.out, "tokens.json"), "w", encoding="utf-8") as f:
        json.dump({"variant": args.variant, "wav": args.wav, "n_mel_frames": int(mel_len[0]),
                   "n_enc_frames": int(enc_len[0]), "text": text, "ids": ids, "frames": frames,
                   "f16_weights": not args.no_f16}, f, ensure_ascii=False, indent=1)
    print(text)


if __name__ == "__main__":
    main()
