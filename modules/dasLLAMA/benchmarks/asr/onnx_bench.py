#!/usr/bin/env python3
# ONNX-Runtime-CPU parakeet-tdt-0.6b-v3 bench — the third side_by_side column.
# Emits the same BENCH TSV lines as asr_bench.das (transcribe time only, load excluded),
# so side_by_side.sh aggregates all sides uniformly.
#
#   .venv/bin/python onnx_bench.py --variant int8 --threads 8 \
#       --wav corpus/jfk.wav --wav corpus/gb1.wav --reps 1
#   .venv/bin/python onnx_bench.py --variant whisper-tiny-int8 --threads 8 --wav corpus/jfk.wav
#
# Venv: the mil-ad server clone's (~/Work/parakeet-tdt-0.6b-v3-fastapi-openai/.venv —
# onnx-asr[hub] + onnxruntime). Models auto-download from HF on first use.
import argparse
import time
import wave

VARIANTS = {
    # parakeet v3 (mirror the mil-ad server's MODEL_CONFIGS)
    "int8": ("nemo-parakeet-tdt-0.6b-v3", "int8"),
    "fp32": ("istupakov/parakeet-tdt-0.6b-v3-onnx", None),
    "fp16": ("grikdotnet/parakeet-tdt-0.6b-fp16", "fp16"),
    # parakeet v2 (English-only)
    "v2-int8": ("nemo-parakeet-tdt-0.6b-v2", "int8"),
    "v2-fp32": ("nemo-parakeet-tdt-0.6b-v2", None),
    # whisper (onnx-community exports via onnx-asr's whisper adapter)
    "whisper-tiny-int8": ("onnx-community/whisper-tiny", "int8"),
    "whisper-tiny-fp32": ("onnx-community/whisper-tiny", None),
    "whisper-turbo-int8": ("onnx-community/whisper-large-v3-turbo", "int8"),
    "whisper-turbo-fp32": ("onnx-community/whisper-large-v3-turbo", None),
}


def audio_seconds(path):
    with wave.open(path, "rb") as w:
        return w.getnframes() / float(w.getframerate())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--variant", choices=sorted(VARIANTS), default="int8")
    ap.add_argument("--threads", type=int, default=4, help="ORT intra_op threads")
    ap.add_argument("--wav", action="append", required=True)
    ap.add_argument("--reps", type=int, default=1)
    ap.add_argument("--text", action="store_true", help="print last transcription")
    ap.add_argument("--out", default=None,
                    help="also write every row to this file — some loaders wreck the stdout "
                         "pipe on Windows (observed: parakeet variants deliver ~1 byte then "
                         "EOF to the parent), and a plain file cannot be wrecked")
    args = ap.parse_args()

    sink = open(args.out, "w", encoding="utf-8") if args.out else None

    def emit(line):
        print(line)
        if sink:
            sink.write(line + "\n")
            sink.flush()

    import onnx_asr
    import onnxruntime as ort

    so = ort.SessionOptions()
    so.intra_op_num_threads = args.threads
    so.inter_op_num_threads = 1
    so.execution_mode = ort.ExecutionMode.ORT_SEQUENTIAL
    so.graph_optimization_level = ort.GraphOptimizationLevel.ORT_ENABLE_ALL

    hf_id, quant = VARIANTS[args.variant]
    t0 = time.perf_counter()
    model = onnx_asr.load_model(hf_id, quantization=quant,
                                providers=["CPUExecutionProvider"], sess_options=so)
    emit(f"LOAD\t{hf_id}\t{(time.perf_counter() - t0) * 1000.0:.3f}")

    name = f"onnx-{args.variant}"
    for wav in args.wav:
        dur = audio_seconds(wav)
        base = wav.rsplit("/", 1)[-1]
        # onnx-asr's whisper adapter is a single 30 s window — no long-form chunking
        # (verified: hp0 returns empty text in 230 ms, jfk3 truncates at ~30 s). A longer
        # clip would bench a silent truncation, not a transcription.
        if args.variant.startswith("whisper") and dur > 30.0:
            emit(f"SKIP\t{name}\t{base}\tsingle-window adapter: {dur:.0f}s > 30s")
            continue
        text = ""
        for rep in range(1, args.reps + 1):
            t0 = time.perf_counter()
            try:
                text = model.recognize(wav)
            except Exception as e:
                # the exports bake a max input length (hp0x2 overflows the rel-pos
                # table); an over-long clip is a missing cell, not a dead sweep
                emit(f"SKIP\t{name}\t{base}\t{type(e).__name__}: {str(e).splitlines()[-1][:120]}")
                break
            ms = (time.perf_counter() - t0) * 1000.0
            emit(f"BENCH\t{name}\t{base}\t{dur:.3f}\t{rep}\t{ms:.3f}\t{dur * 1000.0 / ms:.3f}")
        if args.text:
            emit(f"TEXT\t{base}\t{text}")


if __name__ == "__main__":
    main()
