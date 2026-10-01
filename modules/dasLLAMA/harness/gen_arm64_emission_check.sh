#!/bin/zsh
# gen_arm64_emission_check.sh — the slice J emission-only proof (gemm_generator_plan.md).
#
# Runs the generated GEMM family through a NATIVE aarch64 compile with the i8mm force rail
# (DAS_JIT_ARM64_FORCE_FEATURES=i8mm — M1 is dotprod-only, so the smmla rows emit but must
# never execute here): --jit-compile-only + --jit-dump, then llc + llvm-objdump, and gates
# per-variant instruction counts — the arm64 twin of gen_x64_emission_check.sh. Semantics
# gates for the smmla rows run on i8mm silicon (M3 Air) via DAS_TUNE_MODE=test
# (gen_tune_probe) and the shipped-DLL-cache recipe.
#
# Usage: gen_arm64_emission_check.sh [outdir]
#   LLC / LLVM_OBJDUMP env vars override tool paths (default: /opt/homebrew/opt/llvm/bin).
set -e
ROOT=${DAS_ROOT:-$(cd "$(dirname "$0")/../../.." && pwd)}
TOOLS=${LLVM_TOOLS:-/opt/homebrew/opt/llvm/bin}
LLC=${LLC:-$TOOLS/llc}
OBJDUMP=${LLVM_OBJDUMP:-$TOOLS/llvm-objdump}
OUT=${1:-/tmp/gen_arm64_emission}
mkdir -p "$OUT"

echo "== native compile-only dump (DAS_JIT_ARM64_FORCE_FEATURES=i8mm) =="
# gen_x64_emission_probe.das is architecture-NEUTRAL despite the name (it compiles the whole
# [tune] grid; per-ISA behavior comes from the force-features env) — both emission checks
# deliberately share it so the grids can't drift apart.
DAS_LOG_LEVEL=info DAS_TUNE_MODE=test DAS_JIT_ARM64_FORCE_FEATURES=i8mm \
    "$ROOT/bin/daslang" -jit "$ROOT/modules/dasLLAMA/harness/gen_x64_emission_probe.das" \
    -- --jit-compile-only --jit-dump > "$OUT/dump.txt" 2>&1
grep -q "compile-only - module built" "$OUT/dump.txt" || { echo "FAIL: compile-only did not complete"; exit 1; }

awk '/^\[I\] LLVM JIT: ; ModuleID/ { sub(/^\[I\] LLVM JIT: /, ""); on=1 } on && /^\[[IEW]\] / { exit } on { print }' \
    "$OUT/dump.txt" > "$OUT/module.ll"
echo "module.ll: $(wc -l < "$OUT/module.ll") lines"

echo "== llc + objdump =="
"$LLC" -mtriple=arm64-apple-macosx -mattr=+neon,+dotprod,+i8mm -filetype=obj \
    -o "$OUT/module.o" "$OUT/module.ll"
"$OBJDUMP" -d "$OUT/module.o" > "$OUT/disasm.txt"

# count_in <symbol-substring> <mnemonic> — instruction count inside the (single) matching
# function block; the das JIT inlines each variant's impl into its one exported wrapper
count_in() {
    awk -v pat="$1" -v mn="$2" '
        /^[0-9a-f]+ </ { infn = index($0, pat) > 0; next }
        infn && $0 ~ ("\t" mn) { n++ }
        END { print n + 0 }
    ' "$OUT/disasm.txt"
}

fails=0
gate() { # gate <desc> <sym> <mnemonic> <expected>
    local got=$(count_in "$2" "$3")
    if [[ "$got" == "$4" ]]; then
        echo "OK   $1: $3 == $4"
    else
        echo "FAIL $1: $3 == $got, want $4"
        fails=$((fails+1))
    fi
}

# smmla tile mr8/nrsplit2/kstep2: 2 slices x 3 block instances x (4 kslices x 1 tokpair x
# 4 rowpairs) = 96 MMAs, ZERO sdot (the whole lattice is MMA), B loads straight off the kg8
# plane (no in-body weight shuffles beyond the A zips + fold transposes)
T=q8q8_tile_gen__
gate "smmla tile mr8" "${T}dot_smmla_mr8_kstep2_nrsplit2 " smmla 96
gate "smmla tile mr8 no-sdot" "${T}dot_smmla_mr8_kstep2_nrsplit2 " sdot 0
# smmla tile mr4/nrsplit4/kstep2: 1 slice x 3 instances x (4 x 2 tokpairs x 2 rowpairs) = 48
gate "smmla tile mr4" "${T}dot_smmla_mr4_kstep2 " smmla 48
gate "smmla tile mr4 no-sdot" "${T}dot_smmla_mr4_kstep2 " sdot 0
# the sdot rows must stay MMA-free (family separation sanity)
gate "sdot tile no-smmla" "${T}kstep2 " smmla 0
# gemv under the smmla perm rides the sdot lattice over the kg8 plane (single token):
# gkstep1 = 1 block instance x (8 kg x rq2) = 16 sdot; the kg8 fixup (pair loads +
# uzp1/uzp2.4s) mostly folds into ld2.4s structured de-interleave loads — gate the
# INVARIANT: 16 kg4-shaped vectors materialize (2 per ld2 + 1 per uzp), spelling-agnostic
G=q8q8_gemv_gen__dot_smmla_mr8_kstep2_nrsplit2
gate "smmla gemv sdot" "$G " sdot 16
gate "smmla gemv no-mma" "$G " smmla 0
ld2=$(count_in "$G " "ld2.4s"); u1=$(count_in "$G " uzp1); u2=$(count_in "$G " uzp2)
dv=$((2*ld2 + u1 + u2))
if [[ $dv -eq 16 ]]; then
    echo "OK   smmla gemv kg8 fixup: 2*ld2($ld2) + uzp1($u1) + uzp2($u2) == 16"
else
    echo "FAIL smmla gemv kg8 fixup: 2*ld2($ld2) + uzp1($u1) + uzp2($u2) == $dv, want 16"
    fails=$((fails+1))
fi
# gkstep2 row: 3 block instances x 16 = 48 sdot
G2=q8q8_gemv_gen__dot_smmla_mr8_kstep2_nrsplit2_gkstep2
gate "smmla gemv gk2 sdot" "$G2 " sdot 48
gate "smmla gemv gk2 no-mma" "$G2 " smmla 0
# mx4 companion under the smmla perm: plain tbl-dequant sdot lattice (nibble plane is
# kgroup-independent): 1 block instance x 8rq lookups / 8rq dots at rq=2
M=mx4q8_gemv_gen__dot_smmla_mr8_kstep2_nrsplit2
gate "smmla mx4 tbl" "$M " tbl 16
gate "smmla mx4 sdot" "$M " sdot 16
gate "smmla mx4 no-mma" "$M " smmla 0

if [[ $fails -eq 0 ]]; then
    echo "GEN ARM64 EMISSION OK"
else
    echo "GEN ARM64 EMISSION: $fails gate(s) FAILED"
    exit 1
fi
