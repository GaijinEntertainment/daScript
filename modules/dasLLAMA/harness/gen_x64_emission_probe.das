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
    print("tile variants: {length(vs)}, gemv: {length(gvs)}, mx4: {length(mvs)}, witness: {length(wvs)}, tokstep: {length(tvs)}, cfg: {length(cvs)}\n")
    delete vs
    delete gvs
    delete mvs
    delete wvs
    delete tvs
    delete cvs
}
