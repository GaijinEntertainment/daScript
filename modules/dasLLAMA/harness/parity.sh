#!/usr/bin/env bash
# dasLLAMA token-for-token parity check: reference oracle (simple_ids) vs dasLLAMA (parity.das).
#
# Usage:   parity.sh <model.gguf> [N] [quant] [prompt]
#   N       generated tokens to compare (default 40)
#   quant   dasLLAMA storage: fp32|q8|q4 — pick the file's native type (default q8; use fp32 for F16/F32)
#   prompt  raw continuation prompt (default "Once upon a time")
#
# Env overrides: LLAMA_CPP (default ~/Work/llama.cpp), DASLANG (default <repo>/bin/daslang)
# The oracle binary must be built once — see harness/oracle/simple_ids.cpp for the one-line recipe.
set -euo pipefail

MODEL="${1:?usage: parity.sh <model.gguf> [N] [quant] [prompt]}"
N="${2:-40}"
QUANT="${3:-q8}"
PROMPT="${4:-Once upon a time}"

LLAMA_CPP="${LLAMA_CPP:-$HOME/Work/llama.cpp}"
ORACLE="$LLAMA_CPP/build/bin/simple_ids"
DAS_ROOT="$(cd "$(dirname "$0")/../../.." && pwd)"
DASLANG="${DASLANG:-$DAS_ROOT/bin/daslang}"
PARITY="$DAS_ROOT/modules/dasLLAMA/harness/parity.das"

[ -x "$ORACLE" ] || { echo "missing oracle: $ORACLE — build it (see harness/oracle/simple_ids.cpp)"; exit 2; }

# 1. reference: prompt ids (as llama.cpp tokenizes) + greedy generated ids
REF="$("$ORACLE" -m "$MODEL" -n "$N" -p "$PROMPT" 2>/dev/null)"
PROMPT_IDS="$(printf '%s\n' "$REF" | sed -n 's/^PROMPT_IDS: //p')"
REF_GEN="$(printf '%s\n' "$REF" | sed -n 's/^GEN_IDS: //p')"
IDS_CSV="$(printf '%s' "$PROMPT_IDS" | tr ' ' ',')"

# 2. dasLLAMA: same prompt ids -> greedy generated ids
DAS="$("$DASLANG" -jit "$PARITY" -- -m "$MODEL" -n "$N" --quant "$QUANT" --ids "$IDS_CSV" 2>/dev/null)"
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
