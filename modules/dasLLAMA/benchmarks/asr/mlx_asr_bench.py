#!/usr/bin/env python3
# MLX (mlx-audio, Apple GPU) speech-to-text bench - the MLX reference column of the ASR board.
# Emits the same BENCH TSV lines as asr_bench.das and onnx_bench.py (transcribe time only, model
# load excluded; the clip is passed by path, so its decode is inside the timed call as on the onnx
# side). One untimed warm-up transcription runs first, so the Metal kernels' first-call compile
# never lands in a timed rep.
#
#   .venv-mlx/bin/python mlx_asr_bench.py --variant parakeet-v3 --wav corpus/jfk.wav --reps 3
#
# Venv: benchmarks/asr/.venv-mlx from requirements-mlx.txt (setup_asr_rig --venvs, Apple only).
# Models download from HF on first use.
import argparse
import sys
import time
import wave

VARIANTS = {
    "parakeet-v2": "mlx-community/parakeet-tdt-0.6b-v2",
    "parakeet-v3": "mlx-community/parakeet-tdt-0.6b-v3",
    "whisper-turbo-fp16": "mlx-community/whisper-large-v3-turbo-asr-fp16",
    "whisper-turbo-8bit": "mlx-community/whisper-large-v3-turbo-asr-8bit",
    "qwen3-asr-0.6b-8bit": "mlx-community/Qwen3-ASR-0.6B-8bit",
    "qwen3-asr-1.7b-8bit": "mlx-community/Qwen3-ASR-1.7B-8bit",
}


def audio_seconds(path):
    with wave.open(path, "rb") as w:
        return w.getnframes() / float(w.getframerate())


def transcribe(model, wav):
    result = model.generate(wav)
    return getattr(result, "text", str(result)).strip()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--variant", choices=sorted(VARIANTS), default="parakeet-v3")
    ap.add_argument("--wav", action="append", required=True)
    ap.add_argument("--reps", type=int, default=1)
    ap.add_argument("--threads", type=int, default=0, help="accepted for the rig's uniform argv; MLX runs on the GPU")
    ap.add_argument("--text", action="store_true", help="print each clip's last transcription")
    ap.add_argument("--out", default=None, help="also write every row to this file")
    args = ap.parse_args()

    sink = open(args.out, "w", encoding="utf-8") if args.out else None

    def emit(line):
        print(line, flush=True)
        if sink:
            sink.write(line + "\n")
            sink.flush()

    from importlib.metadata import version
    from mlx_audio.stt.utils import load_model

    hf_id = VARIANTS[args.variant]
    emit(f"VERSIONS\tmlx={version('mlx')}\tmlx-audio={version('mlx-audio')}\t{hf_id}")
    t0 = time.perf_counter()
    model = load_model(hf_id)
    emit(f"LOAD\t{hf_id}\t{(time.perf_counter() - t0) * 1000.0:.3f}")
    transcribe(model, args.wav[0])

    name = f"mlx-{args.variant}"
    rows = 0
    for wav in args.wav:
        dur = audio_seconds(wav)
        base = wav.rsplit("/", 1)[-1]
        text = ""
        for rep in range(1, args.reps + 1):
            t0 = time.perf_counter()
            try:
                text = transcribe(model, wav)
            except Exception as e:
                emit(f"SKIP\t{name}\t{base}\t{type(e).__name__}: {str(e).splitlines()[-1][:120] if str(e) else ''}")
                break
            ms = (time.perf_counter() - t0) * 1000.0
            emit(f"BENCH\t{name}\t{base}\t{dur:.3f}\t{rep}\t{ms:.3f}\t{dur * 1000.0 / ms:.3f}")
            rows += 1
        if args.text:
            emit(f"TEXT\t{base}\t{text}")
    if rows == 0:
        # a run with zero BENCH rows is never a green harness - every clip was skipped
        emit("ERROR\tzero BENCH rows - every clip skipped")
        sys.exit(1)


if __name__ == "__main__":
    main()
