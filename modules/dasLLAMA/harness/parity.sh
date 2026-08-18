#!/usr/bin/env bash
# dasLLAMA token-for-token parity check: reference oracle (simple_ids) vs dasLLAMA (parity.das).
#
# Usage:   parity.sh <model.gguf> [N] [quant] [prompt] [kv] [ctk]
#   N       generated tokens to compare (default 40)
#   quant   dasLLAMA storage: fp32|q8|q4_0 — default q8 (K-quant/mxfp4/Q4_0 files serve their native
#           planes under q8; use fp32 for F16/F32; q4_0 = legacy requant tier)
#   prompt  raw continuation prompt (default "Once upon a time")
#   kv      dasLLAMA KV-cache dtype: f32|f16|q8_0 (default f32)
#   ctk     ORACLE KV-cache type: f16|q8_0 (default: llama.cpp's own default; pass q8_0 with kv=q8_0
#           for the both-sides-quantized run — the oracle then also turns flash attention on)
#
# Env overrides: LLAMA_CPP (default ~/Work/llama.cpp), DASLANG (default <repo>/bin/daslang)
# The oracle binary must be built once — see harness/oracle/simple_ids.cpp for the one-line recipe.
set -euo pipefail

MODEL="${1:?usage: parity.sh <model.gguf> [N] [quant] [prompt]}"
N="${2:-40}"
QUANT="${3:-q8}"
PROMPT="${4:-Once upon a time}"
KV="${5:-f32}"
CTK="${6:-}"

LLAMA_CPP="${LLAMA_CPP:-$HOME/Work/llama.cpp}"
ORACLE="$LLAMA_CPP/build/bin/simple_ids"
DAS_ROOT="$(cd "$(dirname "$0")/../../.." && pwd)"
DASLANG="${DASLANG:-$DAS_ROOT/bin/daslang}"
PARITY="$DAS_ROOT/modules/dasLLAMA/harness/parity.das"

[ -x "$ORACLE" ] || { echo "missing oracle: $ORACLE — build it (see harness/oracle/simple_ids.cpp)"; exit 2; }

# 1. reference: prompt ids (as llama.cpp tokenizes) + greedy generated ids
if [ -n "$CTK" ]; then
    REF="$("$ORACLE" -m "$MODEL" -n "$N" -p "$PROMPT" -ctk "$CTK" 2>/dev/null)"
else
    REF="$("$ORACLE" -m "$MODEL" -n "$N" -p "$PROMPT" 2>/dev/null)"
fi
PROMPT_IDS="$(printf '%s\n' "$REF" | sed -n 's/^PROMPT_IDS: //p')"
REF_GEN="$(printf '%s\n' "$REF" | sed -n 's/^GEN_IDS: //p')"
IDS_CSV="$(printf '%s' "$PROMPT_IDS" | tr ' ' ',')"

# 2. dasLLAMA: same prompt ids -> greedy generated ids. KQ_NATIVE=1/0 (env) A/Bs the native
# K-quant planes; unset keeps the engine default. DASLLAMA_CPU_PREFILL=1 declares the CPU
# prefill intent: without it a >512-token prompt trips the Metal-build tripwire, and with
# stderr dropped the whole script dies silently (set -e) — the 12B counting-prompt mint hit
# exactly that.
KQ_FLAG=""
[ -n "${KQ_NATIVE:-}" ] && KQ_FLAG="--kquant-native $KQ_NATIVE"
DAS="$(DASLLAMA_CPU_PREFILL=1 "$DASLANG" -jit "$PARITY" -- -m "$MODEL" -n "$N" --quant "$QUANT" --kv "$KV" $KQ_FLAG --ids "$IDS_CSV" 2>/dev/null)"
DAS_GEN="$(printf '%s\n' "$DAS" | sed -n 's/^GEN_IDS: //p')"

# 3. token-for-token diff
read -ra R <<< "$REF_GEN"
read -ra D <<< "$DAS_GEN"
matched=0
mismatch=-1
for i in "${!R[@]}"; do
    if [ "${D[$i]:-}" = "${R[$i]}" ]; then
        matched=$((matched + 1))
    elif [ "$mismatch" -lt 0 ]; then
        mismatch=$i
    fi
done

echo "model:  $MODEL"
echo "prompt: \"$PROMPT\"  (ids: $PROMPT_IDS)"
echo "ref:    $REF_GEN"
echo "das:    $DAS_GEN"
if [ "$mismatch" -lt 0 ] && [ "${#D[@]}" -eq "${#R[@]}" ]; then
    echo "PASS: $matched/${#R[@]} token-for-token"
    exit 0
else
    echo "FAIL: $matched/${#R[@]} matched; first mismatch at index $mismatch (ref=${R[$mismatch]:-} das=${D[$mismatch]:-})"
    exit 1
fi
