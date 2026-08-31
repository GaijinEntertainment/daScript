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


def rep_is_degenerate(n_ids):
    # a rep that emits almost nothing is invalid: the mutated-prompt and MPS+set_num_threads
    # failure modes both degenerate to a few-token stub that would silently win best-of
    return n_ids < 8


def build_prompts(prompt, tag, wav):
    # rebuilt PER REP: model.generate mutates the chat list in place, and a reused list
    # degenerates the next rep to a few tokens (fast on MPS, where it silently won best-of;
    # on CPU the encode cost hid it while the transcript was still garbage)
    return [[{"role": "user", "content": f"{prompt}{tag}", "audio": [wav]}]]


def self_test():
    # torch/nemo never imported here - the pure seams check on any stock python3
    assert rep_is_degenerate(0) and rep_is_degenerate(7)
    assert not rep_is_degenerate(8) and not rep_is_degenerate(256)
    p1 = build_prompts("T: ", "<a>", "x.wav")
    p1[0].append({"role": "assistant", "content": "mutated"})  # what generate() does in place
    p2 = build_prompts("T: ", "<a>", "x.wav")
    assert len(p2[0]) == 1, "prompts must come back fresh per rep"
    assert p2[0][0]["content"] == "T: <a>" and p2[0][0]["audio"] == ["x.wav"]
    print("SELFTEST\tok", flush=True)
    return 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", default="nvidia/canary-qwen-2.5b")
    ap.add_argument("--wav", action="append")
    ap.add_argument("--reps", type=int, default=3)
    ap.add_argument("--max-new-tokens", type=int, default=256)
    ap.add_argument("--prompt", default="Transcribe the following: ")
    ap.add_argument("--threads", type=int, default=0)
    ap.add_argument("--device", default="cpu")
    ap.add_argument("--self-test", action="store_true")
    args = ap.parse_args()

    if args.self_test:
        sys.exit(self_test())
    if not args.wav:
        ap.error("--wav is required")

    import torch
    if args.threads > 0 and args.device == "cpu":
        # CPU-arm only: set_num_threads alongside an MPS device degenerates SALM.generate
        # to a 3-token stub (reproduced 2026-08-30, torch in the pinned venv) - the threads
        # knob is meaningless off-CPU anyway
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
        for rep in range(args.reps):
            prompts = build_prompts(args.prompt, tag, wav)
            t0 = time.perf_counter()
            with torch.inference_mode():
                out = model.generate(prompts=prompts, generation_config=gen_cfg,
                                     max_new_tokens=args.max_new_tokens)
            ms = (time.perf_counter() - t0) * 1000.0
            n_ids = len(out[0]) if out is not None and len(out) else 0
            if rep_is_degenerate(n_ids):
                print(f"DEGENERATE\t{args.model}\t{base}\t{rep}\t{n_ids} ids - rep discarded", flush=True)
                continue
            speed = audio_s / (ms / 1000.0)
            print(f"BENCH\t{args.model}\t{base}\t{audio_s:.0f}\t{rep}\t{ms:.3f}\t{speed:.4f}", flush=True)


if __name__ == "__main__":
    main()
