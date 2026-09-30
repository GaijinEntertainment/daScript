#!/bin/zsh
# gen_x64_emission_check.sh — the slice F emission-only proof (gemm_generator_plan.md).
#
# Runs the generated GEMM family through a CROSS x64 compile (arm64 host is fine): forced
# tier features + --jit-compile-only + --jit-dump, then llc (same LLVM major as the dasLLVM
# pin) + llvm-objdump, and gates per-variant instruction counts — the x64 twin of the M2
# "96 sdot / 0 dup" machine-code gate. Nothing x64 ever executes here; semantics gates run
# on x64 silicon via DAS_TUNE_MODE=test (gen_tune_probe).
#
# Usage: gen_x64_emission_check.sh [outdir]
#   LLC / LLVM_OBJDUMP env vars override tool paths (default: /opt/homebrew/opt/llvm/bin).
set -e
ROOT=${DAS_ROOT:-$(cd "$(dirname "$0")/../../.." && pwd)}
TOOLS=${LLVM_TOOLS:-/opt/homebrew/opt/llvm/bin}
LLC=${LLC:-$TOOLS/llc}
OBJDUMP=${LLVM_OBJDUMP:-$TOOLS/llvm-objdump}
OUT=${1:-/tmp/gen_x64_emission}
FEATURES=avx2,avxvnni,avx512f,avx512bw,avx512vl,avx512vnni,avxvnniint8,avx512vbmi,avx512bf16,amx-tile,amx-int8,amx-bf16
mkdir -p "$OUT"

echo "== cross compile-only dump (DAS_JIT_X64_FORCE_FEATURES=$FEATURES) =="
# info logging carries the IR lines; the module cache would serve a normal-mode compile of the grid
DAS_LOG_LEVEL=info DAS_TUNE_MODE=test DAS_JIT_X64_FORCE_FEATURES=$FEATURES \
    "$ROOT/bin/daslang" -no-module-cache -jit "$ROOT/modules/dasLLAMA/harness/gen_x64_emission_probe.das" \
    -- --jit-compile-only --jit-dump --jit-target=x86_64-unknown-linux-gnu > "$OUT/dump.txt" 2>&1
grep -q "compile-only - module built" "$OUT/dump.txt" || { echo "FAIL: compile-only did not complete"; exit 1; }
grep -q "keeps the tune grid" "$OUT/dump.txt" || { echo "FAIL: the cross compile-only dump did not announce the kept tune grid"; exit 1; }

awk '/^\[I\] LLVM JIT: ; ModuleID/ { sub(/^\[I\] LLVM JIT: /, ""); on=1 } on && /^\[[IEW]\] / { exit } on { print }' \
    "$OUT/dump.txt" > "$OUT/module.ll"
echo "module.ll: $(wc -l < "$OUT/module.ll") lines"

echo "== llc + objdump =="
"$LLC" -mtriple=x86_64-unknown-linux-gnu -mattr=+${FEATURES//,/,+} -filetype=obj \
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

# tile, vpdpbusd ymm (kstep2 nrsplit4: 3 block instances x 8 kg x 4 tokens = 96 dots;
# sign trick: one VPSIGNB per dot, one VPABSB per weight vector = 8 x 3)
T=q8q8_tile_gen__
gate "busd256 tile" "${T}dot_vpdpbusd_width256_mr8_kstep2 " vpdpbusd 96
gate "busd256 tile no-pair" "${T}dot_vpdpbusd_width256_mr8_kstep2 " vpmaddubsw 0
gate "busd256 tile sign" "${T}dot_vpdpbusd_width256_mr8_kstep2 " vpsignb 96
gate "busd256 tile abs" "${T}dot_vpdpbusd_width256_mr8_kstep2 " vpabsb 24
# tile, maddubs ymm: the VPMADDUBSW+VPMADDWD pair per dot, no VNNI op
gate "maddubs256 tile pair1" "${T}dot_maddubs_width256_mr8_kstep2 " vpmaddubsw 96
gate "maddubs256 tile pair2" "${T}dot_maddubs_width256_mr8_kstep2 " vpmaddwd 96
gate "maddubs256 tile no-vnni" "${T}dot_maddubs_width256_mr8_kstep2 " vpdpbusd 0
# tile, vpdpbssd ymm: native s8*s8 — no sign trick at all
gate "bssd256 tile" "${T}dot_vpdpbssd_width256_mr8_kstep2 " vpdpbssd 96
gate "bssd256 tile no-sign" "${T}dot_vpdpbssd_width256_mr8_kstep2 " vpsignb 0
gate "bssd256 tile no-abs" "${T}dot_vpdpbssd_width256_mr8_kstep2 " vpabsb 0
# tile, vpdpbusd zmm (mr16 = one zmm row-vector per dword group; select-based sign, no VPSIGNB)
gate "busd512 tile" "${T}dot_vpdpbusd_width512_mr16_kstep2 " vpdpbusd 96
gate "busd512 tile no-psign" "${T}dot_vpdpbusd_width512_mr16_kstep2 " vpsignb 0
# bias128 tiles (slice H): plain u8·s8 dots off the biased plane — the whole sign trick
# (VPABSB + VPSIGNB at 256 / VPMOVB2M + masked VPSUBB at 512) must vanish; the −128·Σx
# correction is the acc init (a broadcast load, not an ALU op). The grid carries the biased
# rows with gkstep2 only (the plain-gkstep twins and the maddubs zmm row were pruned).
B=${T}dot_vpdpbusd_width256_mr8_kstep2_gkstep2_bias128
gate "busd256-b128 tile" "$B " vpdpbusd 96
gate "busd256-b128 tile no-sign" "$B " vpsignb 0
gate "busd256-b128 tile no-abs" "$B " vpabsb 0
B=${T}dot_vpdpbusd_width512_mr16_kstep2_gkstep2_bias128
gate "busd512-b128 tile" "$B " vpdpbusd 96
gate "busd512-b128 tile no-mask" "$B " vpmovb2m 0
gate "busd512-b128 tile no-sub" "$B " vpsubb 0
# bias128 gemv: 24 dots + one inline-bsum dot per block instance (3) = 27; no sign ops
G=q8q8_gemv_gen__dot_vpdpbusd_width256_mr8_kstep2_gkstep2_bias128
gate "busd256-b128 gemv gk2" "$G " vpdpbusd 27
gate "busd256-b128 gemv no-sign" "$G " vpsignb 0
gate "busd256-b128 gemv no-abs" "$G " vpabsb 0
# mx4 gemv (gkstep1: 1 block instance; 4 nibble loads -> 8 VPSHUFB lookups, 8 dots)
gate "mx4 busd256 lut" "mx4q8_gemv_gen__dot_vpdpbusd_width256_mr8_kstep2 " vpshufb 8
gate "mx4 busd256 dot" "mx4q8_gemv_gen__dot_vpdpbusd_width256_mr8_kstep2 " vpdpbusd 8
gate "mx4 bssd256 lut" "mx4q8_gemv_gen__dot_vpdpbssd_width256_mr8_kstep2 " vpshufb 8
gate "mx4 bssd256 dot" "mx4q8_gemv_gen__dot_vpdpbssd_width256_mr8_kstep2 " vpdpbssd 8
gate "mx4 bssd256 no-abs" "mx4q8_gemv_gen__dot_vpdpbssd_width256_mr8_kstep2 " vpabsb 0

# amx tiles (slice I): the 2×2 TMUL macro per k-block = 2 B + 2 A tileloadd, 4 tilezero +
# tdpbssd + tilestored, 4×16 cvt fold rows; s8·s8 native so ZERO sign-trick ops anywhere;
# ldtilecfg is inserted by the backend's fast tile config (presence-gated below)
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit2
gate "amx 2x2 tile dots" "$A " tdpbssd 4
gate "amx 2x2 tile loads" "$A " tileloadd 4
gate "amx 2x2 tile zero" "$A " tilezero 4
gate "amx 2x2 tile spill" "$A " tilestored 4
gate "amx 2x2 tile fold" "$A " vcvtdq2ps 64
gate "amx 2x2 tile no-sign" "$A " vpsignb 0
gate "amx 2x2 tile no-abs" "$A " vpabsb 0
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit1
gate "amx 1x1 tile dots" "$A " tdpbssd 1
gate "amx 1x1 tile loads" "$A " tileloadd 2
gate "amx 1x1 tile fold" "$A " vcvtdq2ps 16
# pipelined amx tile (slice K I4): peeled block-0 macro + steady-state loop body = 2× every
# tile op; the 64 fold cvts appear twice (loop body folds block bi−1, exit folds the last spill)
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit2_pipe1
gate "amx 2x2 pipe dots" "$A " tdpbssd 8
gate "amx 2x2 pipe loads" "$A " tileloadd 8
gate "amx 2x2 pipe zero" "$A " tilezero 8
gate "amx 2x2 pipe spill" "$A " tilestored 8
gate "amx 2x2 pipe fold" "$A " vcvtdq2ps 128
gate "amx 2x2 pipe no-sign" "$A " vpsignb 0
gate "amx 2x2 pipe no-abs" "$A " vpabsb 0
# latch amx tile (slice K I1): raw immediate-tmm ops — same serial macro counts, but ZERO
# config traffic in the tile (no backend ldtilecfg at entry, no tilerelease at exit); the
# ONE ldtilecfg lives in the cfg companion the batch wrapper calls per chunk (zero-arg
# void companion — symbol anchor is '>', like the witness)
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit2_latch1
gate "amx 2x2 latch dots" "$A " tdpbssd 4
gate "amx 2x2 latch loads" "$A " tileloadd 4
gate "amx 2x2 latch zero" "$A " tilezero 4
gate "amx 2x2 latch spill" "$A " tilestored 4
gate "amx 2x2 latch fold" "$A " vcvtdq2ps 64
gate "amx 2x2 latch no-cfg" "$A " ldtilecfg 0
gate "amx 2x2 latch no-release" "$A " tilerelease 0
C='q8q8_amx_cfg_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2_latch1>'
gate "amx latch cfg companion" "$C" ldtilecfg 1
gate "amx latch cfg no-release" "$C" tilerelease 0
C='q8q8_amx_cfg_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2>'
gate "amx non-latch cfg empty" "$C" ldtilecfg 0
# biased amx tile (slice K I3): TDPBSUD (signed activations × unsigned w^0x80 plane) replaces
# TDPBSSD; the −128·Σx bsum correction is 64 embedded-broadcast vpaddd in the fold stage
# (tilezero stays — no acc init); its gemv companion rides the BIASED busd512 lattice
# (gkstep1: 8 kg dots + 1 inline bsum = 9, zero sign-trick ops — the fork's gemv penalty gone)
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit2_bias128
gate "amx 2x2 b128 dots" "$A " tdpbsud 4
gate "amx 2x2 b128 no-ssd" "$A " tdpbssd 0
gate "amx 2x2 b128 fold" "$A " vcvtdq2ps 64
gate "amx 2x2 b128 bsum" "$A " vpaddd 64
gate "amx 2x2 b128 zero" "$A " tilezero 4
G=q8q8_gemv_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2_bias128
gate "amx b128 gemv dots" "$G " vpdpbusd 9
gate "amx b128 gemv no-abs" "$G " vpabsb 0
gate "amx b128 gemv no-mask" "$G " vpmovb2m 0
# the cumulative row (pipe + latch + bias): pipelined structure on raw tmm ops with the
# biased dot — 8 tdpbsud, zero config traffic, 128 fold cvts + 128 bsum adds
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit2_pipe1_latch1_bias128
gate "amx cumul dots" "$A " tdpbsud 8
gate "amx cumul no-ssd" "$A " tdpbssd 0
gate "amx cumul no-cfg" "$A " ldtilecfg 0
gate "amx cumul no-release" "$A " tilerelease 0
gate "amx cumul fold" "$A " vcvtdq2ps 128
gate "amx cumul bsum" "$A " vpaddd 128
C='q8q8_amx_cfg_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2_pipe1_latch1_bias128>'
gate "amx cumul cfg companion" "$C" ldtilecfg 1
# pairwise rows (the full 2^3 pin lattice for the SPR sweep): each pair = the union of its
# arms' structural signatures
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit2_pipe1_latch1
gate "amx pipe+latch dots" "$A " tdpbssd 8
gate "amx pipe+latch no-cfg" "$A " ldtilecfg 0
gate "amx pipe+latch no-release" "$A " tilerelease 0
gate "amx pipe+latch fold" "$A " vcvtdq2ps 128
C='q8q8_amx_cfg_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2_pipe1_latch1>'
gate "amx pipe+latch cfg companion" "$C" ldtilecfg 1
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit2_pipe1_bias128
gate "amx pipe+b128 dots" "$A " tdpbsud 8
gate "amx pipe+b128 no-ssd" "$A " tdpbssd 0
gate "amx pipe+b128 fold" "$A " vcvtdq2ps 128
gate "amx pipe+b128 bsum" "$A " vpaddd 128
A=${T}dot_amx_int8_width512_mr16_kstep1_nrsplit2_latch1_bias128
gate "amx latch+b128 dots" "$A " tdpbsud 4
gate "amx latch+b128 no-ssd" "$A " tdpbssd 0
gate "amx latch+b128 no-cfg" "$A " ldtilecfg 0
gate "amx latch+b128 no-release" "$A " tilerelease 0
gate "amx latch+b128 fold" "$A " vcvtdq2ps 64
gate "amx latch+b128 bsum" "$A " vpaddd 64
C='q8q8_amx_cfg_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2_latch1_bias128>'
gate "amx latch+b128 cfg companion" "$C" ldtilecfg 1
# the vector companions under the amx stamp = the plain sign-trick busd512 lattice over the
# same unbiased plane (design fork (a)): gemv gkstep2 = 3 block instances x 8 kg = 24 dots +
# 24 |w|, select-based sign at 512 (no VPSIGNB); mx4 = 8 pshufb lookups + 8 dots; no tile ops
G=q8q8_gemv_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2_gkstep2
gate "amx gemv gk2 dots" "$G " vpdpbusd 24
gate "amx gemv gk2 abs" "$G " vpabsb 24
gate "amx gemv no-psign" "$G " vpsignb 0
gate "amx gemv no-tile" "$G " tdpbssd 0
M=mx4q8_gemv_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2
gate "amx mx4 lut" "$M " vpshufb 8
gate "amx mx4 dots" "$M " vpdpbusd 8
gate "amx mx4 no-tile" "$M " tdpbssd 0
# the amx witness IS the runtime enable: syscall(SYS_arch_prctl, ARCH_REQ_XCOMP_PERM,
# XFEATURE_XTILEDATA) == 0 as a JIT-resolved libc call. Zero-arg witnesses carry no
# mangled-signature suffix, so the symbol anchor is '>' (not the trailing space).
W='q8q8_family_live__dot_amx_int8_width512_mr16_kstep1_nrsplit2>'
gate "amx witness enable" "$W" callq 1

# ldtilecfg presence in the amx tile (fast tile config runs at codegen)
tc=$(awk -v pat="q8q8_tile_gen__dot_amx_int8_width512_mr16_kstep1_nrsplit2 " '
    /^[0-9a-f]+ </ { infn = index($0, pat) > 0; next }
    infn && /ldtilecfg/ { n++ } END { print n + 0 }' "$OUT/disasm.txt")
if [[ $tc -gt 0 ]]; then echo "OK   amx 2x2 tile configures tiles ($tc ldtilecfg)"; else echo "FAIL amx 2x2 tile: no ldtilecfg"; fails=$((fails+1)); fi

# zmm presence (register-width sanity for the 512 rows)
z=$(awk -v pat="q8q8_tile_gen__dot_vpdpbusd_width512_mr16_kstep2 " '
    /^[0-9a-f]+ </ { infn = index($0, pat) > 0; next }
    infn && /%zmm/ { n++ } END { print n + 0 }' "$OUT/disasm.txt")
if [[ $z -gt 0 ]]; then echo "OK   busd512 tile runs on zmm ($z zmm refs)"; else echo "FAIL busd512 tile: no zmm refs"; fails=$((fails+1)); fi

# the bf16 TMUL tile (the kq families' amx_bf16 leg): raw tmm ops, C in the tiles across the
# k loop — 2 A + nrsplit B tileloadd, 2·nrsplit tdpbf16ps in the loop body, 2·nrsplit tilezero
# before it and tilestored straight into y after it; no int32 spill, no fold cvt, no config traffic
K=k4q8_tile_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2
gate "bf16 2x2 tile dots" "$K " tdpbf16ps 4
gate "bf16 2x2 tile loads" "$K " tileloadd 4
gate "bf16 2x2 tile zero" "$K " tilezero 4
gate "bf16 2x2 tile store" "$K " tilestored 4
gate "bf16 2x2 tile no-fold" "$K " vcvtdq2ps 0
gate "bf16 2x2 tile no-cfg" "$K " ldtilecfg 0
gate "bf16 2x2 tile no-int8" "$K " tdpbssd 0
K=k4q8_tile_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit1
gate "bf16 2x1 tile dots" "$K " tdpbf16ps 2
gate "bf16 2x1 tile loads" "$K " tileloadd 3
gate "bf16 2x1 tile store" "$K " tilestored 2
# the bf16 cfg companion loads the full palette once; the witness is the XTILEDATA grant
C='k4q8_amx_cfg_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2>'
gate "bf16 cfg companion" "$C" ldtilecfg 1
W='k4q8_witness_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2>'
gate "bf16 witness enable" "$W" callq 1
# the emitted panels: the bf16 convert is the one instruction no daslang loop reaches; a
# per-superblock body converts 8 blocks x 8 quads x 2 pair-rows x 2 halves = 256 vectors of 16,
# whatever the format's code planes (high-bit planes, LUT lookups and grid gathers add no convert)
for f in k4 k5 k6 q40 iq4xs iq3s4 k3 iq3s iq3xxs iq4nl k2 iq2s iq2xs iq2xxs; do
    P="${f}q8_panel_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2 "
    gate "$f panel bf16 convert" "$P" vcvtneps2bf16 256
    gate "$f panel no-tile" "$P" tdpbf16ps 0
done
# the LUT formats look their codebook up per nibble vector: 8 blocks x 4 dwords x 2 nibbles VPSHUFB
for f in iq4xs iq3s4 iq4nl; do
    P="${f}q8_panel_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2 "
    gate "$f panel codebook" "$P" vpshufb 64
done
# off a bf16 perm the panel companion is a bare return
P='k4q8_panel_gen__dot_vpdpbusd_width512_mr16 '
gate "k4 panel off-perm empty" "$P" vcvtneps2bf16 0
# the q8 family's bf16 leg on both scale forms: the same tile, a per-32-block panel body (8
# quads x 2 pair-rows x 2 halves = 32 converts a block), the f16-scale panel widening its scales
for f in q8q8_tile_gen q8q8_tile_s16_gen; do
    K="${f}__dot_amx_bf16_width512_mr16_kstep1_nrsplit2 "
    gate "$f bf16 tile dots" "$K" tdpbf16ps 4
    gate "$f bf16 tile no-int8" "$K" tdpbssd 0
done
P='q8q8_panel_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2 '
gate "q8 panel bf16 convert" "$P" vcvtneps2bf16 32
gate "q8 panel no-tile" "$P" tdpbf16ps 0
P='q8q8_panel_s16_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2 '
gate "q8 s16 panel bf16 convert" "$P" vcvtneps2bf16 32
gate "q8 s16 panel f16 scales" "$P" vcvtph2ps 1
gate "q8 s16 panel no-tile" "$P" tdpbf16ps 0

# the tile4 companions: an amx perm's is the busd512 vector tile at kstep 4 with no tile op (7
# block instances - 4 in the loop body, a 3-block remainder chain - x 8 kg x 4 tokens = 224
# dots), its weight stream prefetched on the first token slice alone: 7 x 8 weight lines = 56 on
# the q8 plane, one a line of the 16 x 128-byte group on a packed superblock plane; a byte-panel
# format's reads the walk's fresh scratch and prefetches nothing; a vector perm's is its tile,
# with no prefetch
for f in q8q8_tile4_gen q8q8_tile4_s16_gen; do
    for p in amx_bf16 amx_int8; do
        K="${f}__dot_${p}_width512_mr16_kstep1_nrsplit2 "
        gate "$f $p dots" "$K" vpdpbusd 224
        gate "$f $p prefetch" "$K" prefetch 56
        gate "$f $p no-bf16" "$K" tdpbf16ps 0
        gate "$f $p no-int8" "$K" tdpbssd 0
        gate "$f $p no-tile-load" "$K" tileloadd 0
    done
done
K='q8q8_tile4_gen__dot_vpdpbusd_width512_mr16_kstep2 '
gate "q8q8_tile4_gen vector dots" "$K" vpdpbusd 96
gate "q8q8_tile4_gen vector no-prefetch" "$K" prefetch 0
for f in k4 q40 iq4xs; do
    K="${f}q8_tile4_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2 "
    gate "$f tile4 dots" "$K" vpdpbusd 256
    gate "$f tile4 prefetch" "$K" prefetch 32
    gate "$f tile4 no-tile" "$K" tdpbf16ps 0
done
for f in k6 iq3s; do
    K="${f}q8_tile4_gen__dot_amx_bf16_width512_mr16_kstep1_nrsplit2 "
    gate "$f tile4 dots" "$K" vpdpbusd 256
    gate "$f tile4 panel no-prefetch" "$K" prefetch 0
    gate "$f tile4 no-tile" "$K" tdpbf16ps 0
done
K='k4q8_tile4_gen__dot_vpdpbusd_width512_mr16 '
gate "k4 tile4 vector no-prefetch" "$K" prefetch 0

# off Linux the tile perms decline (the XTILEDATA grant is arch_prctl): the same features under a
# Windows triple emit the vector grid and no tile op
echo "== cross compile-only dump, x86_64-pc-windows-msvc =="
DAS_LOG_LEVEL=info DAS_TUNE_MODE=test DAS_JIT_X64_FORCE_FEATURES=$FEATURES \
    "$ROOT/bin/daslang" -no-module-cache -jit "$ROOT/modules/dasLLAMA/harness/gen_x64_emission_probe.das" \
    -- --jit-compile-only --jit-dump --jit-target=x86_64-pc-windows-msvc > "$OUT/dump_win.txt" 2>&1
grep -q "compile-only - module built" "$OUT/dump_win.txt" || { echo "FAIL: the windows compile-only did not complete"; exit 1; }
irgate() { # irgate <desc> <file> <pattern> <zero|some>
    local got=$(grep -c "$3" "$2")
    if [[ ( "$4" == zero && "$got" == 0 ) || ( "$4" == some && "$got" != 0 ) ]]; then
        echo "OK   $1: $3 x $got"
    else
        echo "FAIL $1: $3 x $got, want $4"
        fails=$((fails+1))
    fi
}
irgate "windows no bf16 tile" "$OUT/dump_win.txt" tdpbf16ps zero
irgate "windows no int8 tile" "$OUT/dump_win.txt" tdpbssd zero
irgate "windows vector grid live" "$OUT/dump_win.txt" vpdpbusd some

# the panel's bf16 convert is an AVX512_BF16 instruction: with the feature the object calls no
# soft-float convert, and without it the bf16 perms decline where the int8 tile stays
"$TOOLS/llvm-nm" -u "$OUT/module.o" > "$OUT/undefined.txt"
irgate "bf16 convert is an instruction" "$OUT/undefined.txt" __truncsfbf2 zero
echo "== cross compile-only dump, no avx512bf16 =="
DAS_LOG_LEVEL=info DAS_TUNE_MODE=test DAS_JIT_X64_FORCE_FEATURES=${FEATURES/avx512bf16,/} \
    "$ROOT/bin/daslang" -no-module-cache -jit "$ROOT/modules/dasLLAMA/harness/gen_x64_emission_probe.das" \
    -- --jit-compile-only --jit-dump --jit-target=x86_64-unknown-linux-gnu > "$OUT/dump_nobf16.txt" 2>&1
grep -q "compile-only - module built" "$OUT/dump_nobf16.txt" || { echo "FAIL: the no-avx512bf16 compile-only did not complete"; exit 1; }
irgate "no avx512bf16: no bf16 tile" "$OUT/dump_nobf16.txt" tdpbf16ps zero
irgate "no avx512bf16: no bf16 convert" "$OUT/dump_nobf16.txt" "to <16 x bfloat>" zero
irgate "no avx512bf16: int8 tile stays" "$OUT/dump_nobf16.txt" tdpbssd some

if [[ $fails -eq 0 ]]; then
    echo "GEN X64 EMISSION OK"
else
    echo "GEN X64 EMISSION: $fails gate(s) FAILED"
    exit 1
fi
