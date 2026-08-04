#!/bin/zsh
# ASR serve/mint peak-memory census: /usr/bin/time -l around one asr_bench process per cell.
# Warm = .dlim present; cold = images removed first (the mint). One clip per invocation so the
# peak attributes to that clip length. macOS only (-l); sample AFTER the process exits — the
# footprint_peak trap does not apply to time(1)'s accounting.
#
#   ./mem_census.sh                # the standard ladder (canary + parakeet v3)
set -e
DAS=${DAS:-$HOME/Work/daScript/bin/daslang}
ASR=${ASR:-$HOME/Work/daScript/modules/dasLLAMA/benchmarks/asr}
LM=${LM:-$HOME/Work/llama.cpp/models}
WM=${WM:-$HOME/Work/whisper.cpp/models}

cell() { # label model mmproj wav
    local label=$1 model=$2 mmproj=$3 wav=$4 out
    local margs=(--model "$model")
    [[ -n "$mmproj" ]] && margs+=(--mmproj "$mmproj")
    out=$(cd "$ASR" && DASLLAMA_CPU_PREFILL=1 /usr/bin/time -l "$DAS" -jit asr_bench.das -- \
          "${margs[@]}" --wav "corpus/$wav" --reps 1 2>&1)
    local rss=$(echo "$out" | awk '/maximum resident/ {print $1}')
    local fp=$(echo "$out" | awk '/peak memory footprint/ {print $1}')
    local ms=$(echo "$out" | awk -F'\t' '$1=="BENCH" {print $6}')
    if [[ -z "$rss" || -z "$fp" ]]; then
        # a panicked cell leaves no time(1) line; dying under set -e mid-ladder would strand
        # the models dir with its images already removed and no census printed
        printf "%-28s %-12s FAILED — no time(1) accounting in output:\n%s\n" "$label" "$wav" "$out"
        return 0   # a failed cell is a visible row; set -e must not strand the ladder mid-census
    fi
    printf "%-28s %-12s rss %6.2f GB  footprint %6.2f GB  %10s ms\n" \
        "$label" "$wav" $((rss/1073741824.0)) $((fp/1073741824.0)) "$ms"
}

echo "== canary (q8/q8), warm =="
cell canary "$LM/canary-qwen-2.5b-decoder-f16.gguf" "$LM/canary-qwen-2.5b-encoder-f32.bin" jfk.wav
cell canary "$LM/canary-qwen-2.5b-decoder-f16.gguf" "$LM/canary-qwen-2.5b-encoder-f32.bin" gb1.wav
cell canary "$LM/canary-qwen-2.5b-decoder-f16.gguf" "$LM/canary-qwen-2.5b-encoder-f32.bin" hp0x2.wav
echo "== canary cold (mint) =="
rm -f "$LM"/canary-qwen-2.5b-decoder-f16.gguf.*.dlim "$LM"/canary-qwen-2.5b-encoder-f32.bin.*.dlim
cell canary-mint "$LM/canary-qwen-2.5b-decoder-f16.gguf" "$LM/canary-qwen-2.5b-encoder-f32.bin" gb1.wav
echo "== parakeet v3 (q8), warm =="
cell parakeet-v3 "$WM/ggml-parakeet-tdt-0.6b-v3-f32.bin" "" hp0x2.wav
echo "CENSUS-DONE"
