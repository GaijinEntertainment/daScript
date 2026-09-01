options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _function_length = 0        // the race runner is one flat compile/oracle/time flow per tier
options _cyclomatic_complexity = 0  // per-format record building + the arm ladders follow the format axis
options _dasllama_internal = true

require daslib/clargs
require strings
require daslib/strings_boost
require daslib/strings_convert
require daslib/fio
require math
require metal/msl_shader
require ?das_metal metal/das_metal_boost
require ?das_metal dasllama/dasllama_metal_kernels
require ?das_metal dasllama/dasllama_metal_prefill   // the kq tensor mul_mm twins (Metal-4 boxes)
require ?das_metal ../../tests/_metal_kernel_common.das

// The per-format Metal KQ kernel RACE rig - rig 1 of the kernel catch-up procedure. Races the
// production decode GEMV and prefill mul_mm of every kq format against candidate spellings and
// against a pinned llama.cpp test-backend-ops reference row, on synthetic planes, no model, no
// tuner, seconds per cell. The dispatch-loop probe shape (HOW_TO_ADD_A_FORMAT.md QUIRK 22):
// `reps` back-to-back dispatches per timed encoder (the shared y buffer's hazard serializes
// them), best-of `rounds` encoders. The metric is weight-plane GB/s for GEMV (decode is
// weight-bound) and GMAC/s for mul_mm, plus us/dispatch for the reference ratio.
//
// Correctness before timing, per arm: one dispatch vs the CPU plane-dequant oracle
// (kq_row_ref + double dot) over EVERY output element on the dot-envelope bar
// |gpu-cpu| <= max(2e-4 * env, 1e-3) - the report line carries the relative factor.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/bench_metal_kq_race.das -- [--fmts iq3s,iq2]
//     [--tiers gemv,mm] [--shapes w13_1b] [--rounds 3] [--reps 50] [--ref _kq_race_ref.tsv]
//
// The --ref tsv pins the llama.cpp side (test-backend-ops -o MUL_MAT perf at these shapes):
// whitespace-separated rows `fmt tier shape us_per_dispatch`; the report prints theirs/ours.

[CommandLineArgs]
struct Args {
    @clarg_doc = "Substring filter over format names, comma list (empty = all)"
    fmts : string

    @clarg_doc = "Tiers to race: gemv (decode), mm (512-token prefill), mvb (2-column batch decode; grid formats only, no lcpp rows - their bs 2-8 runs the GEMV per column), comma list"
    tiers : string = "gemv,mm"

    @clarg_doc = "Substring filter over shape names, comma list (empty = all)"
    shapes : string

    @clarg_doc = "Best-of timing rounds per cell (round 0 is a discarded warmup)"
    rounds : int = 3

    @clarg_doc = "Dispatches per timed encoder"
    reps : int = 50

    @clarg_doc = "GPU burn before each cell's timing, in ms - ramps the idle-clock governor (a sparse run's encoders otherwise start at sunk clocks and read 2-3x slow; the dense full sweep never idles)"
    burn_ms : int = 150

    @clarg_doc = "Pinned llama.cpp reference table (tsv: fmt tier shape us_per_dispatch)"
    ref : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct KqRaceFmt {
    name : string
    fmt : int           //!< the kernel id (kq_fill_planes / kq_row_ref key)
    gemv_gdiv : int     //!< GEMV rows per threadgroup (dispatch groups = ceil(d / gdiv))
    split : bool        //!< split-scale form: the f16 d tail binds at nsb*16
}

struct KqRaceArm {
    name : string
    src : string
    entry : string
    fastmath : bool
    tgmem : uint64
    gdiv : int          //!< GEMV only; a candidate arm may change the row map
}

def race_formats : array<KqRaceFmt> {
    return <- [
        KqRaceFmt(name = "k4", fmt = 4, gemv_gdiv = 4, split = false),
        KqRaceFmt(name = "k5", fmt = 5, gemv_gdiv = 4, split = false),
        KqRaceFmt(name = "k6", fmt = 6, gemv_gdiv = 4, split = true),
        KqRaceFmt(name = "iq4xs", fmt = 44, gemv_gdiv = 4, split = true),
        KqRaceFmt(name = "iq4nl", fmt = 45, gemv_gdiv = 4, split = false),
        KqRaceFmt(name = "k3", fmt = 3, gemv_gdiv = 4, split = true),
        KqRaceFmt(name = "iq3s", fmt = 33, gemv_gdiv = 4, split = true),
        KqRaceFmt(name = "iq3xxs", fmt = 34, gemv_gdiv = 4, split = true),
        KqRaceFmt(name = "k2", fmt = 2, gemv_gdiv = 8, split = true),
        KqRaceFmt(name = "iq2s", fmt = 23, gemv_gdiv = 4, split = true),
        KqRaceFmt(name = "iq2xs", fmt = 24, gemv_gdiv = 4, split = true),
        KqRaceFmt(name = "iq2xxs", fmt = 25, gemv_gdiv = 4, split = true)
    ]
}

//! every shape keeps n % 256 (the kq superblock) and d % 64 (the mul_mm n-tile); the 1B set is
//! the vehicle GGUFs' geometry, the 3B pair the DRAM-honest ones
def race_shapes : array<tuple<name : string; n : int; d : int>> {
    return <- [(name = "kv1b", n = 2048, d = 512), (name = "q1b", n = 2048, d = 2048),
        (name = "w13_1b", n = 2048, d = 8192), (name = "w2_1b", n = 8192, d = 2048),
        (name = "q3b", n = 3072, d = 3072), (name = "w13_3b", n = 3072, d = 8192)]
}

def private want(filter, name : string) : bool {
    if (empty(filter)) {
        return true
    }
    for (tok in split(filter, ",")) {
        if (!empty(tok) && find(name, tok) >= 0) {
            return true
        }
    }
    return false
}

def private gemv_arms(f : KqRaceFmt) : array<KqRaceArm> {   // candidate GEMV spellings register here, after prod
    var arms : array<KqRaceArm>
    if (f.fmt == 4) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_k4_msl, entry = metal_kq_gemv_k4_msl_entry, fastmath = metal_kq_gemv_k4_msl_fastmath, tgmem = 0ul, gdiv = f.gemv_gdiv))
    } elif (f.fmt == 5) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_k5_msl, entry = metal_kq_gemv_k5_msl_entry, fastmath = metal_kq_gemv_k5_msl_fastmath, tgmem = 0ul, gdiv = f.gemv_gdiv))
    } elif (f.fmt == 6) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_k6_msl, entry = metal_kq_gemv_k6_msl_entry, fastmath = metal_kq_gemv_k6_msl_fastmath, tgmem = 0ul, gdiv = f.gemv_gdiv))
    } elif (f.fmt == 44) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_iq4xs_msl, entry = metal_kq_gemv_iq4xs_msl_entry, fastmath = metal_kq_gemv_iq4xs_msl_fastmath, tgmem = metal_kq_gemv_iq4xs_msl_tgmem, gdiv = f.gemv_gdiv))
    } elif (f.fmt == 45) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_iq4nl_msl, entry = metal_kq_gemv_iq4nl_msl_entry, fastmath = metal_kq_gemv_iq4nl_msl_fastmath, tgmem = metal_kq_gemv_iq4nl_msl_tgmem, gdiv = f.gemv_gdiv))
    } elif (f.fmt == 3) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_k3_msl, entry = metal_kq_gemv_k3_msl_entry, fastmath = metal_kq_gemv_k3_msl_fastmath, tgmem = 0ul, gdiv = f.gemv_gdiv))
    } elif (f.fmt == 33) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_iq3s_msl, entry = metal_kq_gemv_iq3s_msl_entry, fastmath = metal_kq_gemv_iq3s_msl_fastmath, tgmem = metal_kq_gemv_iq3s_msl_tgmem, gdiv = f.gemv_gdiv))
        arms |> push(KqRaceArm(name = "f4", src = metal_kq_gemv_iq3s_f4_msl, entry = metal_kq_gemv_iq3s_f4_msl_entry, fastmath = metal_kq_gemv_iq3s_f4_msl_fastmath, tgmem = metal_kq_gemv_iq3s_f4_msl_tgmem, gdiv = 8))
    } elif (f.fmt == 34) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_iq3xxs_msl, entry = metal_kq_gemv_iq3xxs_msl_entry, fastmath = metal_kq_gemv_iq3xxs_msl_fastmath, tgmem = metal_kq_gemv_iq3xxs_msl_tgmem, gdiv = f.gemv_gdiv))
        arms |> push(KqRaceArm(name = "f4", src = metal_kq_gemv_iq3xxs_f4_msl, entry = metal_kq_gemv_iq3xxs_f4_msl_entry, fastmath = metal_kq_gemv_iq3xxs_f4_msl_fastmath, tgmem = metal_kq_gemv_iq3xxs_f4_msl_tgmem, gdiv = 8))
    } elif (f.fmt == 2) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_k2_msl, entry = metal_kq_gemv_k2_msl_entry, fastmath = metal_kq_gemv_k2_msl_fastmath, tgmem = 0ul, gdiv = f.gemv_gdiv))
    } elif (f.fmt == 23) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_iq2s_msl, entry = metal_kq_gemv_iq2s_msl_entry, fastmath = metal_kq_gemv_iq2s_msl_fastmath, tgmem = 0ul, gdiv = f.gemv_gdiv))
    } elif (f.fmt == 24) {
        arms |> push(KqRaceArm(name = "prod", src = metal_kq_gemv_iq2xs_msl, entry = metal_kq_gemv_iq2xs_msl_entry, fastmath = metal_kq_gemv_iq2xs_msl_fastmath, tgmem = metal_kq_gemv_iq2xs_msl_tgmem, gdiv = f.gemv_gdiv))
    } else {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqGemvIq2xxs_metal_kq_gemv_iq2xxs_msl, entry = MetalKqGemvIq2xxs_metal_kq_gemv_iq2xxs_msl_entry, fastmath = MetalKqGemvIq2xxs_metal_kq_gemv_iq2xxs_msl_fastmath, tgmem = 0ul, gdiv = f.gemv_gdiv))
        arms |> push(KqRaceArm(name = "f4", src = MetalKqGemvIq2xxsF4_metal_kq_gemv_iq2xxs_msl, entry = MetalKqGemvIq2xxsF4_metal_kq_gemv_iq2xxs_msl_entry, fastmath = MetalKqGemvIq2xxsF4_metal_kq_gemv_iq2xxs_msl_fastmath, tgmem = MetalKqGemvIq2xxsF4_metal_kq_gemv_iq2xxs_msl_tgmem, gdiv = 4))
    }
    return <- arms
}

def private mvb_arms(f : KqRaceFmt) : array<KqRaceArm> {   // the B2 batch-decode arm per grid format; candidate spellings register here, after prod
    var arms : array<KqRaceArm>
    if (f.fmt == 33) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMvB2Iq3s_metal_kq_mv_iq3s_msl, entry = MetalKqMvB2Iq3s_metal_kq_mv_iq3s_msl_entry, fastmath = MetalKqMvB2Iq3s_metal_kq_mv_iq3s_msl_fastmath, tgmem = 0ul, gdiv = 8))
    } elif (f.fmt == 34) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMvB2Iq3xxs_metal_kq_mv_iq3xxs_msl, entry = MetalKqMvB2Iq3xxs_metal_kq_mv_iq3xxs_msl_entry, fastmath = MetalKqMvB2Iq3xxs_metal_kq_mv_iq3xxs_msl_fastmath, tgmem = 0ul, gdiv = 8))
    } elif (f.fmt == 23) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMvB2Iq2s_metal_kq_mv_iq2s_msl, entry = MetalKqMvB2Iq2s_metal_kq_mv_iq2s_msl_entry, fastmath = MetalKqMvB2Iq2s_metal_kq_mv_iq2s_msl_fastmath, tgmem = 0ul, gdiv = 8))
    } elif (f.fmt == 24) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMvB2Iq2xs_metal_kq_mv_iq2xs_msl, entry = MetalKqMvB2Iq2xs_metal_kq_mv_iq2xs_msl_entry, fastmath = MetalKqMvB2Iq2xs_metal_kq_mv_iq2xs_msl_fastmath, tgmem = 0ul, gdiv = 8))
    } elif (f.fmt == 25) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMvB2Iq2xxs_metal_kq_mv_iq2xxs_msl, entry = MetalKqMvB2Iq2xxs_metal_kq_mv_iq2xxs_msl_entry, fastmath = MetalKqMvB2Iq2xxs_metal_kq_mv_iq2xxs_msl_fastmath, tgmem = 0ul, gdiv = 8))
    }
    return <- arms
}

def private mm_arms(f : KqRaceFmt) : array<KqRaceArm> {   // candidate mul_mm spellings register here, after prod
    var arms : array<KqRaceArm>
    if (f.fmt == 4) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmK4_metal_kq_mulmm_msl, entry = MetalKqMulMmK4_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmK4_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmK4_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 5) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmK5_metal_kq_mulmm_msl, entry = MetalKqMulMmK5_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmK5_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmK5_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 6) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmK6_metal_kq_mulmm_msl, entry = MetalKqMulMmK6_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmK6_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmK6_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 44) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmIq4xs_metal_kq_mulmm_msl, entry = MetalKqMulMmIq4xs_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmIq4xs_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmIq4xs_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 45) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmIq4nl_metal_kq_mulmm_msl, entry = MetalKqMulMmIq4nl_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmIq4nl_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmIq4nl_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 3) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmK3_metal_kq_mulmm_msl, entry = MetalKqMulMmK3_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmK3_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmK3_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 33) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmIq3s_metal_kq_mulmm_msl, entry = MetalKqMulMmIq3s_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmIq3s_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmIq3s_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 34) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmIq3xxs_metal_kq_mulmm_msl, entry = MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmIq3xxs_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 2) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmK2_metal_kq_mulmm_msl, entry = MetalKqMulMmK2_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmK2_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmK2_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 23) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmIq2s_metal_kq_mulmm_msl, entry = MetalKqMulMmIq2s_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmIq2s_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmIq2s_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 24) {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmIq2xs_metal_kq_mulmm_msl, entry = MetalKqMulMmIq2xs_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmIq2xs_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmIq2xs_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    } else {
        arms |> push(KqRaceArm(name = "prod", src = MetalKqMulMmIq2xxs_metal_kq_mulmm_msl, entry = MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_entry, fastmath = MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_fastmath, tgmem = MetalKqMulMmIq2xxs_metal_kq_mulmm_msl_tgmem, gdiv = 0))
    }
    //! the Metal-4 tensor twins (f32-X form) are the ceiling the per-format twins chase; their pipeline build declines cleanly on a box with no tensor toolchain
    if (f.fmt == 4) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmK4T_metal_kq_mulmm_t_msl, entry = MetalKqMulMmK4T_metal_kq_mulmm_t_msl_entry, fastmath = MetalKqMulMmK4T_metal_kq_mulmm_t_msl_fastmath, tgmem = MetalKqMulMmK4T_metal_kq_mulmm_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 5) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmK5T_metal_kq_mulmm_t_msl, entry = MetalKqMulMmK5T_metal_kq_mulmm_t_msl_entry, fastmath = MetalKqMulMmK5T_metal_kq_mulmm_t_msl_fastmath, tgmem = MetalKqMulMmK5T_metal_kq_mulmm_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 6) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl, entry = MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_entry, fastmath = MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_fastmath, tgmem = MetalKqMulMmK6T_metal_kq_mulmm_k6_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 44) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl, entry = MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl_entry, fastmath = MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl_fastmath, tgmem = MetalKqMulMmIq4xsT_metal_kq_mulmm_iq4xs_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 45) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl, entry = MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl_entry, fastmath = MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl_fastmath, tgmem = MetalKqMulMmIq4nlT_metal_kq_mulmm_iq4xs_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 3) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl, entry = MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl_entry, fastmath = MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl_fastmath, tgmem = MetalKqMulMmK3T_metal_kq_mulmm_k3_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 2) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl, entry = MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl_entry, fastmath = MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl_fastmath, tgmem = MetalKqMulMmK2T_metal_kq_mulmm_k2_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 33) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl, entry = MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl_entry, fastmath = MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl_fastmath, tgmem = MetalKqMulMmIq3sT_metal_kq_mulmm_iq3s_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 34) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl, entry = MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl_entry, fastmath = MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl_fastmath, tgmem = MetalKqMulMmIq3xxsT_metal_kq_mulmm_iq3xxs_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 23) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl, entry = MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl_entry, fastmath = MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl_fastmath, tgmem = MetalKqMulMmIq2sT_metal_kq_mulmm_iq2s_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 24) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl, entry = MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl_entry, fastmath = MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl_fastmath, tgmem = MetalKqMulMmIq2xsT_metal_kq_mulmm_iq2xs_t_msl_tgmem, gdiv = 0))
    } elif (f.fmt == 25) {
        arms |> push(KqRaceArm(name = "tensor", src = MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl, entry = MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl_entry, fastmath = MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl_fastmath, tgmem = MetalKqMulMmIq2xxsT_metal_kq_mulmm_iq2xxs_t_msl_tgmem, gdiv = 0))
    }
    return <- arms
}

let PREFILL_M = 512

def private load_ref_table(path : string; var tab : table<string; double>) {
    return if (empty(path))
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("--ref {path}: cannot open")
        }
        let text = fread(f)
        for (ln in split(text, "\n")) {
            let cols <- split_by_chars(strip(ln), " \t")
            var vals <- [for (c in cols); c; where !empty(c)]
            if (length(vals) >= 4) {
                let us = try_to_double(vals[3]) ?? -1.0lf
                if (us > 0.0lf) {
                    tab["{vals[0]}/{vals[1]}/{vals[2]}"] = us
                }
            }
            delete vals
        }
    }
}

//! one timed encoder: bind once, `reps` back-to-back dispatches; per-dispatch GPU us, <0 on failure
def private time_arm_us(queue; pso; arm : KqRaceArm; bks, bkq, bx, by, b5, b6; sc_off : uint64; groups : uint3; tgsz, reps : int) : double {
    var err = ""
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (arm.tgmem != 0ul) {
            metal_set_threadgroup_memory_length(enc, arm.tgmem, 0)
        }
        metal_set_buffer(enc, bks, sc_off, 0)
        metal_set_buffer(enc, bks, 0ul, 1)
        metal_set_buffer(enc, bkq, 0ul, 2)
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_buffer(enc, b5, 0ul, 5)
        metal_set_buffer(enc, b6, 0ul, 6)
        for (_r in range(reps)) {
            metal_dispatch_threadgroups(enc, groups, uint3(uint(tgsz), 1u, 1u))
        }
    }
    if (!ok) {
        print("  {arm.name}: dispatch FAILED: {err}\n")
        return -1.0lf
    }
    return gpu_ms * 1000.0lf / double(reps)
}

//! the whole-output dot-envelope oracle the tests trust: want/env from kq_row_ref + double dot
def private build_oracle(fmt, n, d, nsb : int; kq, ks : array<uint8>; xf : array<float>; xstride, xrows : int;
                         var want : array<float>; var env : array<float>) {
    var wrow : array<float>
    wrow |> resize(n)
    let nb = n / 256
    for (r in range(d)) {
        kq_row_ref(fmt, r * nb, nb, nsb, kq, ks, wrow)
        for (xr in range(xrows)) {
            var acc = 0.0lf
            var ev = 0.0lf
            for (c in range(n)) {
                let p = double(wrow[c]) * double(xf[xr * xstride + c])
                acc += p
                ev += abs(p)
            }
            want[xr * d + r] = float(acc)
            env[xr * d + r] = float(ev)
        }
    }
    delete wrow
}

[arch(at="../../ARCHITECTURE_MEASUREMENT.md#kernel-race-fidelity")]
def private race_cell(queue, dev; f : KqRaceFmt; tier, shname : string; n, d : int;
                      arms : array<KqRaceArm>; args : Args; ref_tab : table<string; double>) : int {
    let is_mm = tier == "mm"
    let is_mvb = tier == "mvb"   //! 2-column batch decode: gemv's dispatch shape at gdiv 8, ka args at binding 5
    let nb = n / 256
    let nsb = d * nb
    var kq : array<uint8>
    var ks : array<uint8>
    kq_fill_planes(f.fmt, nsb, kq, ks)
    let xrows = is_mm ? PREFILL_M : (is_mvb ? 2 : 1)
    var xf : array<float>
    xf |> resize(xrows * n)
    for (i in range(xrows * n)) {
        xf[i] = 0.11 * float(i % 41) - 2.2
    }
    var want : array<float>
    var env : array<float>
    want |> resize(xrows * d)
    env |> resize(xrows * d)
    build_oracle(f.fmt, n, d, nsb, kq, ks, xf, n, xrows, want, env)
    var bks = buf_upload(dev, ks)
    var bkq = buf_upload(dev, kq)
    var bx = buf_upload(dev, xf)
    var kav <- [uint(n), uint(d), uint(d), 2u]   //! KqMvArgs{ndim, ddim, ys, nr} as its raw words
    var b5 = is_mvb ? buf_upload(dev, kav) : buf_u32(dev, uint(n))
    var b6 = buf_u32(dev, uint(d))
    let sc_off = f.split ? uint64(nsb * 16) : 0ul
    let wbytes = double(length(kq) + length(ks))
    var rows = 0
    print("--- {f.name} {tier} {shname} (n={n} d={d}, weights {wbytes / 1048576.0lf} MB)\n")
    //! compile + correctness gate, one y buffer per arm
    var psos : array<MetalComputePipeline?>
    var bys : array<MetalBuffer?>
    var live : array<bool>
    psos |> reserve(length(arms))
    bys |> reserve(length(arms))
    live |> reserve(length(arms))
    for (arm in arms) {
        var err = ""
        var pso = pipeline_from_source(dev, arm.src, arm.entry, arm.fastmath, err)
        if (pso == null) {
            print("  {arm.name}: pipeline FAILED: {err}\n")
        }
        psos |> push(pso)
        bys |> push(buf_fill(dev, xrows * d, -1000.0))
        live |> push(pso != null)
    }
    for (arm, lv, pso, by in arms, live, psos, bys) {
        continue if (!lv)
        let groups = is_mm ? uint3(uint(PREFILL_M / 32), uint(d / 64), 1u) : uint3(uint((d + arm.gdiv - 1) / arm.gdiv), 1u, 1u)
        let tgsz = is_mm ? 128 : 64
        let us0 = time_arm_us(queue, pso, arm, bks, bkq, bx, by, b5, b6, sc_off, groups, tgsz, 1)
        if (us0 < 0.0lf) {
            lv = false
            continue
        }
        let bad = buf_mismatch_env(by, want, env, 2e-4, "{f.name}/{tier}/{shname}/{arm.name}")
        if (bad != 0) {
            print("  {arm.name}: ORACLE MISMATCH ({bad} of {xrows * d}) - arm disqualified\n")
            lv = false
        }
    }
    var burned = 0.0lf
    var burn_rounds = 0   //! round cap: a device whose timestamps read 0.0 must not spin the budget loop forever
    while (burned < double(args.burn_ms) * 1000.0lf && burn_rounds < 2048) {
        burn_rounds++
        var did = false
        for (arm, lv, pso, by in arms, live, psos, bys) {
            continue if (!lv)
            let groups = is_mm ? uint3(uint(PREFILL_M / 32), uint(d / 64), 1u) : uint3(uint((d + arm.gdiv - 1) / arm.gdiv), 1u, 1u)
            let us = time_arm_us(queue, pso, arm, bks, bkq, bx, by, b5, b6, sc_off, groups, is_mm ? 128 : 64, args.reps)
            break if (us < 0.0lf)
            burned += us * double(args.reps)
            did = true
            break
        }
        break if (!did)
    }
    var best : array<double>
    best |> resize(length(arms))
    for (i in range(length(arms))) {
        best[i] = -1.0lf
    }
    for (round in range(args.rounds + 1)) {
        for (arm, lv, pso, by, bst in arms, live, psos, bys, best) {
            continue if (!lv)
            let groups = is_mm ? uint3(uint(PREFILL_M / 32), uint(d / 64), 1u) : uint3(uint((d + arm.gdiv - 1) / arm.gdiv), 1u, 1u)
            let tgsz = is_mm ? 128 : 64
            let us = time_arm_us(queue, pso, arm, bks, bkq, bx, by, b5, b6, sc_off, groups, tgsz, args.reps)
            if (us < 0.0lf) {
                lv = false
            } elif (round > 0 && (bst < 0.0lf || us < bst)) {
                bst = us
            }
        }
    }
    var base_us = -1.0lf
    for (i in range(length(arms))) {
        continue if (!live[i] || best[i] < 0.0lf)
        let arm = arms[i]
        let us = best[i]
        if (i == 0) {
            base_us = us
        }
        let gbs = wbytes / (us * 1000.0lf)   //! us * 1e3 = ns; bytes/ns = GB/s
        var line = "  {arm.name}: {us:.2f} us/dispatch  {gbs:.1f} wGB/s"
        if (is_mm) {
            let gmac = double(PREFILL_M) * double(n) * double(d) / (us * 1000.0lf)
            line = "{line}  {gmac:.0f} GMAC/s"
        }
        if (i > 0 && base_us > 0.0lf) {
            line = "{line}  vs prod {base_us / us:.3f}x"
        }
        let rk = "{f.name}/{tier}/{shname}"
        if (ref_tab |> key_exists(rk)) {
            line = "{line}  vs lcpp {(ref_tab?[rk] ?? 0.0lf) / us:.3f}x"
        }
        print("{line}  [env-bar 2e-4 ok]\n")
        rows++
    }
    for (p in psos) {
        if (p != null) {
            metal_release(p)
        }
    }
    for (b in bys) {
        metal_release(b)
    }
    metal_release(bks)
    metal_release(bkq)
    metal_release(bx)
    metal_release(b5)
    metal_release(b6)
    psos |> clear()   //! borrowed pointers - clear before delete (delete would free the pointees)
    bys |> clear()
    unsafe {
        delete psos
        delete bys
    }
    delete kq
    delete ks
    delete xf
    delete kav
    delete want
    delete env
    delete live
    delete best
    return rows
}

def private main_apple(args : Args) {
    var dev = metal_create_system_default_device()
    if (dev == null) {
        panic("no Metal device on this box")
    }
    print("device={metal_device_name(dev)} rounds={args.rounds} reps={args.reps}\n")
    var queue = metal_new_command_queue(dev)
    var ref_tab : table<string; double>
    load_ref_table(args.ref, ref_tab)
    var fmts <- race_formats()
    var shapes <- race_shapes()
    var rows = 0
    for (f in fmts) {
        continue if (!want(args.fmts, f.name))
        for (sh in shapes) {
            continue if (!want(args.shapes, sh.name))
            if (want(args.tiers, "gemv")) {
                var arms <- gemv_arms(f)
                rows += race_cell(queue, dev, f, "gemv", sh.name, sh.n, sh.d, arms, args, ref_tab)
                delete arms
            }
            if (want(args.tiers, "mm")) {
                var arms <- mm_arms(f)
                rows += race_cell(queue, dev, f, "mm", sh.name, sh.n, sh.d, arms, args, ref_tab)
                delete arms
            }
            if (want(args.tiers, "mvb")) {
                var arms <- mvb_arms(f)
                if (!empty(arms)) {
                    rows += race_cell(queue, dev, f, "mvb", sh.name, sh.n, sh.d, arms, args, ref_tab)
                }
                delete arms
            }
        }
    }
    metal_release(queue)
    metal_release(dev)
    delete fmts
    delete shapes
    delete ref_tab
    if (rows == 0) {
        panic("zero result rows - wrong filters, failed pipelines, or a declining device")
    }
    print("DONE: {rows} result rows\n")
}

[export]
def main {
    var args = Args()
    let rc = parse_args_with_help(args, "bench_metal_kq_race")
    return if (rc >= 0)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        main_apple(args)
    } else {
        panic("das_metal is not built on this platform")
    }
}
