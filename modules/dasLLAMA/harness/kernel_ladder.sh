#!/usr/bin/env bash
# kernel_ladder.sh - the per-box kernel table: every format's stamped kernel (kq_kernel_bench at
# one thread, normal tune mode = the winner this box ships) against the reference exe's
# test-backend-ops perf at the same shape, decode (n=1) and prefill (n=512) rows, joined with
# the ratio (reference us / ours us; >= 1.0 = ours is faster). No model, no jobque: the kernel
# and nothing else. The bench's arena sits page-aligned (--base-align 4096) - the engine's image
# planes start on 16 KiB boundaries, and k6/k3 read 10-30% differently at the heap's random phase. The reference binary needs the GGML_BENCH_THREADS define (harness README).
#
# Usage: kernel_ladder.sh [fmt,fmt,...]          (default: every format)
#   KL_DASLANG the daslang binary (default: this tree's bin/daslang, or bin/Release/daslang.exe on
#              Windows - never the global DASLANG, which names another tree's binary on dev boxes)
#   LCPP_TBO   test-backend-ops (default: $HOME/Work/llama.cpp/build-clean-cpu/bin/Release/test-backend-ops.exe)
#   NTOK       prefill tokens (default 512; 0 = decode rows only)
#   ROUNDS     interleaved rounds per row (default 5)
#   TEAM       lanes for the decode rows: the bench dispatches the GEMV the engine's way (--team,
#              DAS_JOBQUE_THREADS=N) and the reference runs GGML_BENCH_THREADS=N. Default: the box's
#              hardware threads (nproc). SOLO=1 = one thread, one raw call - the kernel table the
#              tuner's races correspond to.
#   BIG        decode rows at d=32768 (a weight above any L3 - the many-lane comparison that
#              test-backend-ops perf's repeated 4096-row op does not give; the reference needs the
#              m=32768 perf case, a two-line edit beside the thread define - HOW_TO_GET_SIDECAR.md).
#              Default 1 with TEAM, 0 with SOLO=1.
# Output: a TSV table on stdout - fmt tier perm ours_us ours_med_us ref_us ratio - plus the box line.
set -e -o pipefail   # a failing daslang must not hide behind the join
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)   # the tree this script lives in, whatever the env says
DASLANG=${KL_DASLANG:-}
if [ -z "$DASLANG" ]; then
    if [ -x "$ROOT/bin/Release/daslang.exe" ]; then DASLANG="$ROOT/bin/Release/daslang.exe"; else DASLANG="$ROOT/bin/daslang"; fi
fi
TBO=${LCPP_TBO:-$HOME/Work/llama.cpp/build-clean-cpu/bin/Release/test-backend-ops.exe}
FMTS=${1:-all}
NTOK=${NTOK:-512}
ROUNDS=${ROUNDS:-5}
SOLO=${SOLO:-0}
if [ "$SOLO" -gt 0 ]; then TEAM=0; BIG=${BIG:-0}; else TEAM=${TEAM:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)}; BIG=${BIG:-1}; fi
ROWS=$(( BIG > 0 ? 32768 : 4096 ))
THREADS=$(( TEAM > 0 ? TEAM : 1 ))
TEAM_ARGS=""; [ "$TEAM" -gt 0 ] && TEAM_ARGS="--team"
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

# KqFmt name -> ggml type name (the reference's type_a)
ggml_type() {
    case "$1" in
        q8) echo q8_0;; k4) echo q4_K;; k5) echo q5_K;; k6) echo q6_K;; q40) echo q4_0;; q51) echo q5_1;;
        iq4xs) echo iq4_xs;; k3) echo q3_K;; iq3s) echo iq3_s;; iq3xxs) echo iq3_xxs;; iq4nl) echo iq4_nl;;
        k2) echo q2_K;; iq2s) echo iq2_s;; iq2xs) echo iq2_xs;; iq2xxs) echo iq2_xxs;; mx4) echo mxfp4;;
        *) echo "";;
    esac
}

echo "# kernel ladder  $(date +%F)  box=$(hostname)  daslang=$DASLANG  ref=$TBO  ntok=$NTOK rounds=$ROUNDS" >&2

# ---- ours: one process, every format, the stamped winner per format ----
[ -x "$TBO" ] || { echo "kernel_ladder: no test-backend-ops at '$TBO' (set LCPP_TBO)" >&2; exit 1; }
grep -q GGML_BENCH_THREADS "$TBO" || { echo "kernel_ladder: '$TBO' lacks the GGML_BENCH_THREADS define - it would run every core and the ratio column would lie (harness README)" >&2; exit 1; }
set +e
DAS_TUNE_MODE=normal DAS_JOBQUE_THREADS=$THREADS "$DASLANG" -jit "$ROOT/modules/dasLLAMA/benchmarks/matmul/kq_kernel_bench.das" \
    -- --fmt "$FMTS" --d "$ROWS" --ntok "$NTOK" --rounds "$ROUNDS" --tsv --base-align 4096 $TEAM_ARGS > "$WORK/ours.raw" 2> "$WORK/ours.err"
rc=$?
set -e
awk -F'\t' 'NF >= 11' "$WORK/ours.raw" > "$WORK/ours.tsv" || true   # the TSV rows; engine and tune notes fall away
if [ $rc -ne 0 ] || [ ! -s "$WORK/ours.tsv" ]; then
    echo "kernel_ladder: kq_kernel_bench failed (rc=$rc)" >&2
    tail -20 "$WORK/ours.err" >&2
    exit 1
fi

# ---- the reference: one perf run per type at one thread; n=1 and n=512 rows ----
: > "$WORK/ref.tsv"
for f in $(cut -f1 "$WORK/ours.tsv" | sort -u); do
    t=$(ggml_type "$f")
    [ -n "$t" ] || { echo "kernel_ladder: no ggml type for '$f'" >&2; continue; }
    GGML_BENCH_THREADS=$THREADS "$TBO" perf -b CPU -o MUL_MAT -p "type_a=$t,type_b=f32,m=$ROWS,n=(1|512)," 2>/dev/null \
        | sed 's/\x1b\[[0-9;]*m//g' | awk -v f="$f" '
            /MUL_MAT\(/ {
                n = $0; sub(/.*,n=/, "", n); sub(/,.*/, "", n)
                us = $0; sub(/.*runs - */, "", us); sub(/ us\/run.*/, "", us)
                print f "\t" (n == 1 ? "gemv" : "tile") "\t" us
            }' >> "$WORK/ref.tsv"
done

# ---- join: ours (fmt tier perm n d ntok best med ...) x ref (fmt tier us) ----
printf 'fmt\ttier\tperm\tours_us\tours_med_us\tref_us\tratio\n'
awk -F'\t' 'NR == FNR { ref[$1 "\t" $2] = $3; next }
    {
        key = $1 "\t" $2; r = (key in ref) ? ref[key] : ""
        ratio = (r != "" && $7 > 0) ? sprintf("%.2f", r / $7) : "-"
        printf "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", $1, $2, $3, $7, $8, (r != "" ? r : "-"), ratio
    }' "$WORK/ref.tsv" "$WORK/ours.tsv"
