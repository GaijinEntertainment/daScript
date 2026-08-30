#!/usr/bin/env python3
# NeMo SALM (canary-qwen-2.5b) transcribe bench — the reference-engine side of the
# Canary-Qwen ASR A/B (llama.cpp has NO canary/SALM support, and onnx-asr ships no canary
# export, so NeMo greedy IS the only reference). Engine-level (no HTTP), greedy; --device
# picks the torch device (cpu default, mps = the Apple-GPU reference arm); the
# timed region is model.generate() only (model + audio load excluded), mirroring
# asr_bench.das's per-transcribe BENCH line. Same greedy config as canary_qwen_oracle.py.
#
# Venv: the committed oracle venv (~/Work/canary-nemo-oracle/.venv).
#
#   ~/Work/canary-nemo-oracle/.venv/bin/python canary_qwen_bench.py \
#       --wav corpus/jfk.wav --wav corpus/jfk3.wav --reps 3
import argparse
import time
import sys


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", default="nvidia/canary-qwen-2.5b")
    ap.add_argument("--wav", action="append", required=True)
    ap.add_argument("--reps", type=int, default=3)
    ap.add_argument("--max-new-tokens", type=int, default=256)
    ap.add_argument("--prompt", default="Transcribe the following: ")
    ap.add_argument("--threads", type=int, default=0)
    ap.add_argument("--device", default="cpu")
    args = ap.parse_args()

    import torch
    if args.threads > 0:
        torch.set_num_threads(args.threads)
    import soundfile as sf
    from transformers import GenerationConfig
    from nemo.collections.speechlm2.models import SALM

    torch.manual_seed(0)
    t_load = time.perf_counter()
    model = SALM.from_pretrained(args.model)
    model.eval()
    if args.device != "cpu":
        model = model.to(args.device)
    load_ms = (time.perf_counter() - t_load) * 1000.0
    print(f"LOAD\t{args.model}\t{load_ms:.3f}", flush=True)

    gen_cfg = GenerationConfig(
        do_sample=False, num_beams=1,
        bos_token_id=model.text_bos_id,
        eos_token_id=model.text_eos_id,
        pad_token_id=model.text_pad_id,
    )
    tag = model.audio_locator_tag

    for wav in args.wav:
        info = sf.info(wav)
        audio_s = info.frames / info.samplerate
        base = wav.rsplit("/", 1)[-1]
        prompts = [[{"role": "user", "content": f"{args.prompt}{tag}", "audio": [wav]}]]
        for rep in range(args.reps):
            t0 = time.perf_counter()
            with torch.inference_mode():
                model.generate(prompts=prompts, generation_config=gen_cfg,
                               max_new_tokens=args.max_new_tokens)
            ms = (time.perf_counter() - t0) * 1000.0
            speed = audio_s / (ms / 1000.0)
            print(f"BENCH\t{args.model}\t{base}\t{audio_s:.0f}\t{rep}\t{ms:.3f}\t{speed:.4f}", flush=True)


if __name__ == "__main__":
    main()
