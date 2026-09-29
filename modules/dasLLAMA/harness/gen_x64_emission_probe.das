options gen2
options no_aot = true
options _dasllama_internal = true

// The x64 emission-only probe (gemm_generator_plan.md slice F): a minimal -jit program whose
// only job is to make every [tune] grid clone `used` (via the variants registries), so a
// cross-target compile-only run emits the whole generated family for inspection:
//
//   DAS_TUNE_MODE=test DAS_JIT_X64_FORCE_FEATURES=avx2,avxvnni,avx512f,avx512bw,avx512vl,avx512vnni,avxvnniint8 \
//   bin/daslang -jit modules/dasLLAMA/harness/gen_x64_emission_probe.das \
//       -- --jit-compile-only --jit-dump
//
// Nothing jitted ever executes (compile-only); main runs interpreted and just prints the
// variant counts. gen_x64_emission_check.sh drives this end to end (dump -> llc -> objdump ->
// per-variant instruction-count gates).

require dasllama/dasllama_math_gen
require llvm   // nolint:STYLE030 - the witness, unguarded on purpose: the `_variants()` registries below exist only with dasLLVM, so a host without it skips this program as missing its prerequisite

[export]
def main {
    var vs <- q8q8_tile_gen_variants()
    var gvs <- q8q8_gemv_gen_variants()
    var mvs <- mx4q8_gemv_gen_variants()
    // the witness + tokstep + cfg registries too, so their impls land in the dump (the amx
    // witness carries the syscall enable, the latch cfg companion the LDTILECFG — both
    // gated by the check script)
    var wvs <- q8q8_family_live_variants()
    var tvs <- q8q8_tokstep_gen_variants()
    var cvs <- q8q8_amx_cfg_gen_variants()
    var qfs <- q8q8_tileform_gen_variants()
    var qps <- q8q8_panel_gen_variants()
    var svs <- q8q8_tile_s16_gen_variants()
    var sps <- q8q8_panel_s16_gen_variants()
    var kvs <- k4q8_tile_gen_variants()
    var kfs <- k4q8_tileform_gen_variants()
    var kcs <- k4q8_amx_cfg_gen_variants()
    var kws <- k4q8_witness_gen_variants()
    var kps <- k4q8_panel_gen_variants()
    var npanels = 0
    npanels += length(k5q8_panel_gen_variants())
    npanels += length(k6q8_panel_gen_variants())
    npanels += length(q40q8_panel_gen_variants())
    npanels += length(iq4xsq8_panel_gen_variants())
    npanels += length(iq3s4q8_panel_gen_variants())
    npanels += length(k3q8_panel_gen_variants())
    npanels += length(iq3sq8_panel_gen_variants())
    npanels += length(iq3xxsq8_panel_gen_variants())
    npanels += length(iq4nlq8_panel_gen_variants())
    npanels += length(k2q8_panel_gen_variants())
    npanels += length(iq2sq8_panel_gen_variants())
    npanels += length(iq2xsq8_panel_gen_variants())
    npanels += length(iq2xxsq8_panel_gen_variants())
    print("tile variants: {length(vs)}, gemv: {length(gvs)}, mx4: {length(mvs)}, witness: {length(wvs)}, tokstep: {length(tvs)}, cfg: {length(cvs)}, tileform: {length(qfs)}, panel: {length(qps)}, k4 tile: {length(kvs)} (tileform {length(kfs)}, cfg {length(kcs)}, witness {length(kws)}, panel {length(kps)}), other panels {npanels}\n")
    delete kps
    delete qfs
    delete qps
    delete svs
    delete sps
    delete vs
    delete gvs
    delete mvs
    delete wvs
    delete tvs
    delete cvs
    delete kvs
    delete kfs
    delete kcs
    delete kws
}
