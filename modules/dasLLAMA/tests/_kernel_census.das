options gen2
options _dasllama_internal = true

module _kernel_census shared public

//! The kernel dispatch census across a run's processes: each test
//! process keeps a running sum of its census tables, one a home,
//! writes it into the folder DASLLAMA_KERNEL_CENSUS_DIR names after
//! every GPU form a block runs, and the census verdict here reads
//! every process's file of a home merged: the floor, the declared
//! blind spots and the conditional entries. It loads no model

require dastest/testing_boost public
require dasllama/dasllama_env   // g_env_test.kernel_census_dir
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the Metal census
require ?vulkan dasllama/dasllama_vulkan_common   // nolint:STYLE030,LINT019 - the Vulkan census
require daslib/fio
require strings
require daslib/strings_boost   // join
require dasllama/dasllama_gpu_tier   // kernel_coverage_report: the verdicts' report

//! A census table's note keys start with this: a value a cell writes
//! beside the dispatch counts. A merge of several processes' tables
//! sums every key; a note is read as a count or as a flag (> 0)
let CENSUS_NOTE = "@"

let CENSUS_METAL = "metal"   //! the Metal home's census files
let CENSUS_VULKAN = "vulkan"   //! the Vulkan home's census files

let NOTE_VK_ROUTED = "@vk_routed"   //! sum: routed rows that ran
//! flag: the served set put a column of the routed block on the int8
//! tiles (`vk_mmq_any`) as a Vulkan cell read it: the served set
//! reads the box's stored set, which a model's load applies, and the
//! form's coopmat mode, and the verdict's process loads no model
let NOTE_VK_INT8_COLUMNS = "@vk_int8_columns"
//! flag prefix: one key a tensor crown the box profile put on the
//! Metal home, as a zoo cell read it after its form's load
let NOTE_METAL_CROWN = "@metal_crown."

var private g_census_notes : table<string; int64>
var private g_census_sums : table<string; table<string; int64>>
var private g_census_stamp = 0l

//! `n` added to the note `key` of this process's census
def kernel_census_note_add(key : string; n : int64) {
    g_census_notes[key] += n
}

//! the note `key` of this process's census set as a flag
def kernel_census_note_flag(key : string) {
    g_census_notes[key] = 1l
}

//! the notes this process's cells wrote (a copy)
def kernel_census_notes : table<string; int64> => <- clone_to_move(g_census_notes)

//! this process's notes dropped, beside a reset of the census
def kernel_census_notes_reset {
    g_census_notes |> clear()
}

//! the engine's dispatch table of `home` since its last reset; empty
//! on a build without the home's module
def private engine_census(home : string) : table<string; int64> {
    var out : table<string; int64>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (home == CENSUS_METAL) {
            out <- metal_kernel_coverage()
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (home == CENSUS_VULKAN) {
            out <- vk_kernel_coverage()
        }
    }
    return <- out
}

//! `from`'s every key added into `into`
def private census_add(var into : table<string; int64>; from : table<string; int64>) {
    for (k, n in keys(from), values(from)) {
        into[k] += n
    }
}

//! this process's census table of `home` since the last reset: the
//! engine's dispatches, the cells' notes beside them
def kernel_census_table(home : string) : table<string; int64> {
    var out <- engine_census(home)
    census_add(out, g_census_notes)
    return <- out
}

//! this process's Metal census table (`kernel_census_table`)
def kernel_census_table_metal : table<string; int64> => <- kernel_census_table(CENSUS_METAL)

//! this process's Vulkan census table (`kernel_census_table`)
def kernel_census_table_vulkan : table<string; int64> => <- kernel_census_table(CENSUS_VULKAN)

//! the engine's dispatches of `home` folded into this process's sum
//! where `folder`, the run's census folder, is named
def private census_fold(home, folder : string) {
    return if (empty(folder))
    var inscope now <- engine_census(home)
    census_add(g_census_sums[home], now)
}

//! the Metal census reset a cell asks for, its dispatches so far kept
//! in the process's census file
def kernel_census_reset_metal() {
    kernel_census_reset_metal_in(g_env_test.kernel_census_dir)
}

//! `kernel_census_reset_metal` where `folder` names the run's census
//! folder. Non-private: a test names a folder of its own
def kernel_census_reset_metal_in(folder : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        census_fold(CENSUS_METAL, folder)
        metal_kernel_coverage_reset()
    }
}

//! the Vulkan census reset a cell asks for, its dispatches so far
//! kept in the process's census file
def kernel_census_reset_vulkan() {
    kernel_census_reset_vulkan_in(g_env_test.kernel_census_dir)
}

//! `kernel_census_reset_vulkan` where `folder` names the run's census
//! folder. Non-private: a test names a folder of its own
def kernel_census_reset_vulkan_in(folder : string) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        census_fold(CENSUS_VULKAN, folder)
        vk_kernel_coverage_reset()
    }
}

//! the census file of `home` this process writes in `folder`
def private census_file(folder, home : string) : string {
    if (g_census_stamp == 0l) {
        g_census_stamp = ref_time_ticks()
    }
    return path_join(folder, "{home}_{g_census_stamp}.json")
}

//! where DASLLAMA_KERNEL_CENSUS_DIR names a folder, this process's
//! census of every home the build carries - its sum and the table
//! since the last reset - written over its file there
def kernel_census_write() {
    kernel_census_write_in(g_env_test.kernel_census_dir)
}

//! `kernel_census_write` into `folder` ("" writes nothing).
//! Non-private: a test names a folder of its own
def kernel_census_write_in(folder : string) {
    return if (empty(folder))
    mkdir_rec(folder)
    for (home in fixed_array(CENSUS_METAL, CENSUS_VULKAN)) {
        var inscope cov <- engine_census(home)
        continue if (empty(cov))
        census_add(cov, g_census_notes)
        g_census_sums |> get(home) $(sum) {
            census_add(cov, sum)
        }
        fopen(census_file(folder, home), "wb") $(f) {
            if (f != null) {
                fwrite(f, sprint_json(cov, false))
            }
        }
    }
}

//! a merge of a home's census files: how many parsed, every key
//! summed over them (the keys' union), and the files that did not
//! parse
struct CensusMerge {
    files : int
    cov : table<string; int64>
    corrupt : array<string>
}

//! every census file of `home` in `folder` merged (`CensusMerge`)
def kernel_census_merged(folder, home : string) : CensusMerge {
    var out : CensusMerge
    return <- out if (empty(folder) || !stat(folder).is_dir)
    let lead = "{home}_"
    var inscope names : array<string>
    dir(folder) $(name) {
        if (name |> starts_with(lead) && name |> ends_with(".json")) {
            names |> push(name)
        }
    }
    for (name in names) {
        var part : table<string; int64>
        if (sscan_json(fread(path_join(folder, name)), part)) {
            census_add(out.cov, part)
            out.files++
        } else {
            out.corrupt |> push(name)
        }
        delete part
    }
    return <- out
}

//! what the census verdict does with a run's census folder
enum CensusRead {
    skip_unset   //! no folder named: a bare run of the verdict file
    skip_empty   //! a narrowed run's folder with no file of the home
    fail_empty   //! a whole run's folder with no file of the home
    read         //! census files to merge and judge
}

//! the census verdict's read of `folder` holding `files` census files
//! of a home, on a run that was `whole` or narrowed (`CensusRead`)
def census_read_of(folder : string; files : int; whole : bool) : CensusRead {
    return CensusRead.skip_unset if (empty(folder))
    return CensusRead.read if (files > 0)
    return whole ? CensusRead.fail_empty : CensusRead.skip_empty
}

//! `verdict` on the merged census of `home` in `folder` and on
//! `whole`, whether the run covered every standing area whole
//! (DASLLAMA_KERNEL_CENSUS_WHOLE); a loud skip where `folder` is ""
//! or a narrowed run wrote no file of `home`, a failure where a whole
//! run wrote none or a file does not parse. False without a verdict
def with_kernel_census(t : T?; folder, home : string; whole : bool;
                       verdict : block<(cov : table<string; int64>; whole : bool) : void>
                       ) : bool {
    var inscope merged <- kernel_census_merged(folder, home)
    if (!empty(merged.corrupt)) {
        t |> failure(("{home} census file(s) in {folder} do not parse, so the merged census " +
            "undercounts the run: {join(merged.corrupt, ", ")}"))
        return false
    }
    let read = census_read_of(folder, merged.files, whole)
    if (read == CensusRead.skip_unset) {
        t |> skip("DASLLAMA_KERNEL_CENSUS_DIR is unset: no run's census to read")
        return false
    }
    if (read == CensusRead.skip_empty) {
        t |> skip("no {home} census file in {folder}: no cell of this narrowed run reached the home")
        return false
    }
    if (read == CensusRead.fail_empty) {
        t |> failure(("no {home} census file in {folder}: the whole run's processes wrote " +
            "no census of the home, so nothing was checked"))
        return false
    }
    to_log(LOG_INFO, "{home} kernel census: {merged.files} process file(s), whole run {whole}\n")
    invoke(verdict, merged.cov, whole)
    return true
}

// The census's DECLARED blind spots: kernel classes no cell of the
// run dispatches, each with the reason and the gate that covers it.
// A new [metal_kernel] belongs in a cell, not here; a class lands
// here only when no cell can reach it, and it leaves the moment one
// does. Reconcile the list against the report's NEVER DISPATCHED
// warnings - a name here that the report shows dispatched is stale,
// and a warning with no entry here is a coverage gap to close with
// a cell.
let private CENSUS_NEVER_DISPATCHED <- [
    // the MoE expert mul_mm tall M-tile pair (128-row floor +
    // remainder) in q5_K: no cell of the run loads a Q5_K expert
    // plane. Its coverage is the tall-pair arm of
    // moe_mulmm_split_gate (fmt 5) in test_metal_gemm_kernels.das
    "MetalMoeMulMmK5TH128", "MetalMoeMulMmK5THR",
    // the q5_1 MoE tensor twins' tall pair: the run's Q5_1 expert
    // planes reach the half-X twin, and the pair engages at
    // npos * n_expert_used >= 128 * n_expert rows, past every cell's
    // window. Its coverage is the bit-exact twin + tall-pair gate in
    // test_metal_prefill_kernels.das (moe_mulmm_b32_gate)
    "MetalMoeMulMmQ51TH128", "MetalMoeMulMmQ51THR",
    // the iq4nl32 MoE tensor twins' tall pair: the same row floor,
    // past every cell's window. Their coverage is the same gate's
    // iq4nl32 arm
    "MetalMoeMulMmIq4nl32TH128", "MetalMoeMulMmIq4nl32THR",
    // the dense tall in-kernel-dequant kq stamps: they engage only
    // when a site's f16 panel would reach TALLKQ_MIN_PANEL (96 MiB),
    // and every cell's sites top out far below it. Their coverage is
    // the tall kernel-unit leg in test_metal_gemm_kernels.das
    // (kq_mulmm_gate)
    "MetalKqMulMmK4TH128", "MetalKqMulMmK5TH128", "MetalKqMulMmK6TH128",
    "MetalKqMulMmIq3sTH128", "MetalKqMulMmIq3xxsTH128",
    // the nine split-scale and grid formats (Q2_K, Q3_K, IQ4_XS,
    // IQ4_NL, IQ3_S, IQ3_XXS, IQ2_S, IQ2_XS, IQ2_XXS): no cell of the
    // run loads a Q3_K, IQ2_S or IQ2_XS plane, its IQ3_S planes serve
    // the window's GEMM alone, and no cell steps a batch on the
    // others at the rows these small-batch tiles take (IQ4_XS and
    // IQ4_NL reach four rows, not eight). Their coverage is the
    // kernel-unit gates in test_metal_gemv_kernels.das (kq_mvb_gate)
    // / test_metal_gemm_kernels.das (kq_mulmm_gate) and the
    // per-format e2e parity rows recorded in HOW_TO_ADD_A_FORMAT.md.
    // A class naming its own kernel records the bare shader entry
    // name, so those stamps sit here under it, not under the class
    "MetalKqMvB2K2", "MetalKqMvB4K2", "MetalKqMvB8K2",
    "metal_kq_gemv_k3", "MetalKqMvB2K3", "MetalKqMvB4K3", "MetalKqMvB8K3", "MetalKqMulMmK3",
    "MetalKqMvB8Iq4xs",
    "MetalKqMvB8Iq4nl",
    "metal_kq_gemv_iq3s", "MetalKqMvB2Iq3s", "MetalKqMvB4Iq3s", "MetalKqMvB8Iq3s",
    "MetalKqMvB2Iq3xxs", "MetalKqMvB4Iq3xxs", "MetalKqMvB8Iq3xxs",
    "metal_kq_gemv_iq2s", "MetalKqMvB2Iq2s", "MetalKqMvB4Iq2s", "MetalKqMvB8Iq2s",
    "MetalKqMulMmIq2s",
    "metal_kq_gemv_iq2xs", "MetalKqMvB2Iq2xs", "MetalKqMvB4Iq2xs", "MetalKqMvB8Iq2xs",
    "MetalKqMulMmIq2xs",
    "MetalKqMvB2Iq2xxs", "MetalKqMvB4Iq2xxs", "MetalKqMvB8Iq2xxs",
    // the rows' expert union - its table, the gate/up and down over
    // each entry's rows and the combine over partial rows - serves
    // only under DASLLAMA_METAL_MOE_UNION (off by default), which no
    // cell of the run sets; its coverage is moe_union_gate in
    // test_metal_gemv_kernels.das
    "MetalMoeUnion", "metal_moe_gu_union_iq4xs", "metal_moe_dn_union_iq4xs",
    "metal_moe_down2p_iq4xs",
    // the fused routed pair's Flash-Next stamps (the iq3s grid
    // gate/up, the iq4nl32 down with the q8 or iq4nl32 shared down,
    // the row and the hyper-connection landings): their one carrier
    // is Qwen3.8-Flash-Next (large tier), which no cell of the run
    // loads; their coverage is moe_fused_fn_gate in
    // test_metal_gemv_kernels.das
    "metal_moe_gemv_gu_iq3s_f4", "metal_moe_down2_iq4nl32", "metal_moe_down2_iq4nl32_hc",
    "metal_moe_down2_iq4nl32_shq8",
    "metal_moe_down2_iq4nl32_shq8_hc",
    // the same nine formats' crown-gated forms: the f4-slab GEMV
    // twins and the tensor mul_mm T/TH stamps compile only behind a
    // box crown, and no cell of the run serves them - it loads no
    // Q3_K, IQ2_S or IQ2_XS plane, and its IQ3_S and IQ3_XXS sites
    // take their other stamps. Their coverage is the same
    // kernel-unit gates plus the tensor-twins/iq2xxs matrix row and
    // the mint's METAL_TWIN race rows
    "metal_kq_gemv_iq3s_f4", "metal_kq_gemv_iq3xxs_f4",
    "metal_moe_gemv_iq3s_f4",
    "MetalKqMulMmK3T", "MetalKqMulMmK3TH",
    "MetalKqMulMmIq3sT", "MetalKqMulMmIq3sTH",
    "MetalKqMulMmIq2sT", "MetalKqMulMmIq2sTH", "MetalKqMulMmIq2xsT", "MetalKqMulMmIq2xsTH",
    // the k4 two-row tile: it serves only under the kq_mvb2_k4_r2
    // crown. Its coverage is the r2 arm of kq_mvb_gate and the
    // pair-walk gate in test_metal_gemv_kernels.das
    "MetalKqMvB2K4R2",
    // k5's half-x small-batch stamps: every half stamp compiles at
    // decode init, and k5's dispatch only where the box crowned
    // `kq_rows_half_k5` - the run's batched k5 steps pin no crown,
    // and the one cell that pins it (the MTP half-x leg) runs on the
    // 9B, a large-tier file. k4's wide forms and k6's half stamps
    // are cells of the run under the batchB8-kq half leg's own pins
    // (`_metal_decode_cells.das`). Their coverage is
    // test_metal_kq_half_forms in test_metal_gemv_kernels.das. The
    // nine formats above cover their own half stamps by prefix (the
    // stamp is the f32 class's name plus H)
    "MetalKqMvB2K5H", "MetalKqMvB4K5H", "MetalKqMvB8K5H",
    // the twin producers' rows8 table mode: a step of five to eight
    // rows that carries the half-x twins (a box-crowned
    // `kq_rows_half_<fmt>`) on a box whose rows8 layout probe passed.
    // Their coverage is rows8_table_gate and rows8_k4_fed_gate in
    // test_metal_gemm_kernels.das
    "MetalRmsNormHXT", "metal_add_rms_hxt", "metal_pre_add_rms_hxt", "MetalSwigluHXT",
    "MetalGegluHXT",
    // the Q4_0 small-batch twins at four and eight rows: the run's
    // Q4_0 batched steps take two rows. The simdgroup dense mul_mm
    // loses the box race to its own tensor stamp, so it never serves
    // either - its entry carries the method suffix, since the bare
    // class name prefix-matches the T/TH keys the run DOES dispatch.
    // Their coverage is kq_mvb_gate(fmt 40) in
    // test_metal_gemv_kernels.das and kq_mulmm_gate(fmt 40) in
    // test_metal_gemm_kernels.das
    "MetalKqMvB4Q40", "MetalKqMvB8Q40", "MetalKqMulMmQ40_metal_kq_mulmm",
    // the nv4 half-x small-batch twins: the NVFP4 llama row's batched
    // steps dispatch the f32 forms, and the half forms wait on a
    // `kq_rows_half_nv4` crown no box has raced. Their coverage is
    // test_metal_kq_half_forms in test_metal_gemv_kernels.das
    "MetalKqMvB2Nv4H", "MetalKqMvB4Nv4H", "MetalKqMvB8Nv4H",
    // the nv4 dense tensor twins the NVFP4 llama row dispatches only
    // under the `kq_mulmm_nv4` crown (full keys: the T stem is a
    // prefix of every other twin's); their coverage is
    // kq_mulmm_gate(fmt 49) in test_metal_gemm_kernels.das
    "MetalKqMulMmNv4T_metal_kq_mulmm_iq4xs_t", "MetalKqMulMmNv4TH_metal_kq_mulmm_iq4xs_t",
    // the dev-W dequant kernels of the split-scale and iquant
    // formats: the image mint's dev-W bake runs one per format, and
    // no cell of the run bakes a dev-W image in these. Their coverage
    // is kq_devw_gate in test_metal_gemm_kernels.das, whose
    // kq_dq_kernel picks the stamp per format
    "MetalKqDequantIq4nl", "MetalKqDequantK3",
    "MetalKqDequantIq3s", "MetalKqDequantIq3xxs", "MetalKqDequantK2", "MetalKqDequantIq2s",
    "MetalKqDequantIq2xs", "MetalKqDequantIq2xxs", "MetalKqDequantIq3s4",
    // the tall and double-buffered dense twins of every split-scale
    // and per-32 format, and the per-32 formats' dense tensor twins
    // and dev-W passes: no cell of the run holds a dense plane in
    // these formats at a DRAM-class panel (the gemma4moe row reaches
    // q40's under its crown; the kq-scratch/llama-q51 cell's crowned
    // window takes a q51 twin, so the bare `MetalKqMulMmQ51T` prefix
    // is no entry). Their coverage is kq_mulmm_gate (tall)
    // and kq_dbuf_gate (the 128- and 32-row pairs) in
    // test_metal_gemm_kernels.das, and b32_dense_twins in
    // test_metal_prefill_kernels.das
    "MetalKqMulMmIq4xsTH128", "MetalKqMulMmIq3s4TH128", "MetalKqMulMmIq4nlTH128",
    "MetalKqMulMmQ40TH128", "MetalKqMulMmNv4TH128", "MetalKqMulMmK3TH128", "MetalKqMulMmK2TH128",
    "MetalKqMulMmIq2sTH128", "MetalKqMulMmIq2xsTH128", "MetalKqMulMmIq2xxsTH128",
    "MetalKqMulMmIq4xsTHDb", "MetalKqMulMmIq3s4THDb", "MetalKqMulMmIq4nlTHDb",
    "MetalKqMulMmQ40THDb", "MetalKqMulMmNv4THDb", "MetalKqMulMmK3THDb", "MetalKqMulMmK2THDb",
    "MetalKqMulMmIq2sTHDb", "MetalKqMulMmIq2xsTHDb", "MetalKqMulMmIq2xxsTHDb",
    "MetalKqMulMmIq3sTHDb", "MetalKqMulMmIq3xxsTHDb",
    "MetalKqMulMmIq4xsTHDb128", "MetalKqMulMmIq3s4THDb128", "MetalKqMulMmIq4nlTHDb128",
    "MetalKqMulMmQ40THDb128", "MetalKqMulMmNv4THDb128", "MetalKqMulMmK3THDb128",
    "MetalKqMulMmK2THDb128", "MetalKqMulMmIq2sTHDb128", "MetalKqMulMmIq2xsTHDb128",
    "MetalKqMulMmIq2xxsTHDb128", "MetalKqMulMmIq3sTHDb128", "MetalKqMulMmIq3xxsTHDb128",
    "MetalKqMulMmQ51TH", "MetalKqMulMmQ51TH128", "MetalKqMulMmQ51THDb",
    "MetalKqMulMmQ51THDb128",
    "MetalKqMulMmIq4nl32T", "MetalKqMulMmIq4nl32TH", "MetalKqMulMmIq4nl32TH128",
    "MetalKqMulMmIq4nl32THDb", "MetalKqMulMmIq4nl32THDb128", "MetalKqDequantIq4nl32",
    "MetalKqMulMmQ4032T", "MetalKqMulMmQ4032TH", "MetalKqMulMmQ4032TH128", "MetalKqMulMmQ4032THDb",
    "MetalKqMulMmQ4032THDb128",
    "MetalKqMulMmQ5032T", "MetalKqMulMmQ5032TH", "MetalKqMulMmQ5032TH128", "MetalKqMulMmQ5032THDb",
    "MetalKqMulMmQ5032THDb128", "MetalKqDequantQ5032",
    // the short window's split-K pair
    // (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k) of the
    // split-scale and grid formats, and the f32-X stamp of k4 and k6:
    // no window of the run takes a split-K site on a dense plane in
    // these formats, and its Q4_K_M rows reach the f32-X split sites
    // on k5 planes alone (the attention output and the down sites of
    // the k4/k6 layers carry the half twin). The per-32 formats'
    // pairs fall under their T / TH prefixes above. The ladder files
    // (fmt-ladder/Qwen3-Embedding-0.6B-<T>-pure-local), which no cell
    // of the run loads, dispatch every one; their coverage is
    // kq_split_gate in test_metal_gemm_kernels.das and
    // b32_mm_split_gate in test_metal_prefill_kernels.das
    "MetalKqMulMmIq4xsTHSk_metal_kq_mulmm_db", "MetalKqMulMmIq4xsTSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq3s4THSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq3s4TSk_metal_kq_mulmm_db", "MetalKqMulMmIq4nlTHSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq4nlTSk_metal_kq_mulmm_db",
    "MetalKqMulMmQ40THSk_metal_kq_mulmm_db", "MetalKqMulMmQ40TSk_metal_kq_mulmm_db",
    "MetalKqMulMmK3THSk_metal_kq_mulmm_db",
    "MetalKqMulMmK3TSk_metal_kq_mulmm_db", "MetalKqMulMmK2THSk_metal_kq_mulmm_db",
    "MetalKqMulMmK2TSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq3sTHSk_metal_kq_mulmm_db", "MetalKqMulMmIq3sTSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq3xxsTHSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq3xxsTSk_metal_kq_mulmm_db", "MetalKqMulMmIq2sTHSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq2sTSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xsTHSk_metal_kq_mulmm_db", "MetalKqMulMmIq2xsTSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xxsTHSk_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xxsTSk_metal_kq_mulmm_db", "MetalKqMulMmK4TSk_metal_kq_mulmm_db",
    "MetalKqMulMmK6TSk_metal_kq_mulmm_db",
    // the short window's 16-row split stamps
    // (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k), every
    // format and the q8 blob: they serve a window of 5 to 16 rows,
    // and the run's short windows take no split stamp: the kq-tail
    // cells (`_metal_prefill_cells.das`) run theirs under the
    // mm-tail peel (`set_metal_prefill_mm_tail`), and every other
    // cell prefills past 16 rows. Their coverage is q8_split_gate
    // and kq_split_gate at 16 rows in
    // test_metal_gemm_kernels.das and b32_mm_split_gate in
    // test_metal_prefill_kernels.das
    "MetalKqMulMmK4THSk16_metal_kq_mulmm_db", "MetalKqMulMmK4TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmK5THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmK5TSk16_metal_kq_mulmm_db", "MetalKqMulMmK6THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmK6TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq4xsTHSk16_metal_kq_mulmm_db", "MetalKqMulMmIq4xsTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq3s4THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq3s4TSk16_metal_kq_mulmm_db", "MetalKqMulMmIq4nlTHSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq4nlTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmQ40THSk16_metal_kq_mulmm_db", "MetalKqMulMmQ40TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmNv4THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmNv4TSk16_metal_kq_mulmm_db", "MetalKqMulMmK3THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmK3TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmK2THSk16_metal_kq_mulmm_db", "MetalKqMulMmK2TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq3sTHSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq3sTSk16_metal_kq_mulmm_db", "MetalKqMulMmIq3xxsTHSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq3xxsTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq2sTHSk16_metal_kq_mulmm_db", "MetalKqMulMmIq2sTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xsTHSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xsTSk16_metal_kq_mulmm_db", "MetalKqMulMmIq2xxsTHSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq2xxsTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmQ51THSk16_metal_kq_mulmm_db", "MetalKqMulMmQ51TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq4nl32THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmIq4nl32TSk16_metal_kq_mulmm_db", "MetalKqMulMmQ4032THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmQ4032TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmQ5032THSk16_metal_kq_mulmm_db", "MetalKqMulMmQ5032TSk16_metal_kq_mulmm_db",
    "MetalQ8MulMmTHSk16_metal_q8_mulmm_t",
    "MetalQ8MulMmTSk16_metal_q8_mulmm_t",
    // the simdgroup base tiles' split-K stamps
    // (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k), the q8 blob
    // and every format: they serve a site whose format the box's
    // dense race crowned no tensor twin for (every format on the M1),
    // and no cell of the run reaches such a site in these formats;
    // their coverage is base_split_gate in
    // test_metal_gemm_kernels.das and b32_mm_base_split_gate in
    // test_metal_prefill_kernels.das
    "MetalKqMulMmIq3s4Sk_metal_kq_mulmm",
    "MetalKqMulMmK3Sk_metal_kq_mulmm", "MetalKqMulMmIq3sSk_metal_kq_mulmm",
    "MetalKqMulMmIq3xxsSk_metal_kq_mulmm", "MetalKqMulMmK2Sk_metal_kq_mulmm",
    "MetalKqMulMmIq2sSk_metal_kq_mulmm", "MetalKqMulMmIq2xsSk_metal_kq_mulmm",
    "MetalKqMulMmIq2xxsSk_metal_kq_mulmm", "MetalKqMulMmIq4nlSk_metal_kq_mulmm",
    "MetalKqMulMmQ40Sk_metal_kq_mulmm", "MetalKqMulMmNv4Sk_metal_kq_mulmm",
    "MetalKqMulMmIq4nl32Sk_metal_kq_mulmm",
    "MetalKqMulMmQ4032Sk_metal_kq_mulmm", "MetalKqMulMmQ5032Sk_metal_kq_mulmm",
    // the split-format MoE expert mul_mm stamps (f32-X base, half-X
    // twin, and the tall pair): the run's IQ3_S expert planes take
    // no window GEMM and no cell loads an IQ2_XS one, so all four
    // forms of those two sit here; the other formats' windows reach
    // the half-X twin but not the tall pair's row floor (the q40
    // entry below). The half-X twin and the tall pair are covered by
    // moe_mulmm_split_gate in test_metal_gemm_kernels.das; the f32-X
    // base of every one of these formats is NOT - moe_split_twin has
    // no MoeSplitForm.t arm, and the only path to that stamp is a
    // served routed model whose prefill minted no half panel
    // (pf_enc_moe_mm's last arm). followup_metal.md sec.14 owes that
    // arm
    "MetalMoeMulMmK3TH128", "MetalMoeMulMmK3THR",
    "MetalMoeMulMmIq4xsTH128",
    "MetalMoeMulMmIq4xsTHR",
    "MetalMoeMulMmNv4TH128", "MetalMoeMulMmNv4THR",
    "MetalMoeMulMmIq3sT", "MetalMoeMulMmIq3sTH", "MetalMoeMulMmIq3sTH128", "MetalMoeMulMmIq3sTHR",
    "MetalMoeMulMmIq3xxsTH128",
    "MetalMoeMulMmIq3xxsTHR",
    "MetalMoeMulMmIq2sTH128", "MetalMoeMulMmIq2sTHR",
    "MetalMoeMulMmIq2xsT", "MetalMoeMulMmIq2xsTH", "MetalMoeMulMmIq2xsTH128",
    "MetalMoeMulMmIq2xsTHR",
    "MetalMoeMulMmIq2xxsTH128",
    "MetalMoeMulMmIq2xxsTHR",
    // the q40 expert twins the gemma4moe row cannot reach: the tall
    // pair engages at npos * n_expert_used >= 128 * n_expert = 16384
    // rows, and its coverage is moe_mulmm_split_gate(fmt 40) in
    // test_metal_gemm_kernels.das. The f32-X base serves only where
    // the prefill minted no half panel (pf_enc_moe_mm's last arm),
    // which no served routed model on this box takes, and
    // moe_split_twin has no MoeSplitForm.t arm for any format - so
    // the T stamp has NO model-less cell, in q40 or in the seven
    // formats above. followup_metal.md sec.14 owes that arm
    "MetalMoeMulMmQ40T_metal_moe_mulmm_iq4xs_t",
    // the expert-indexed GEMVs of the same formats: the run's
    // Q3_K, IQ3_XXS and IQ2_XXS expert planes reach the window's GEMM
    // alone, and no cell loads an IQ2_XS one. Their coverage is
    // moe_kq_gemv_gate in test_metal_gemv_kernels.das
    "metal_moe_gemv_k3", "metal_moe_gemv_iq3xxs",
    "metal_moe_gemv_iq2xs", "metal_moe_gemv_iq2xxs",
    // the Q2_K and superblock IQ4_NL routed forms - the Q2_K
    // gathered GEMV and the four tensor twins of each (the T prefix
    // takes all four): no cell of the run loads a Q2_K expert plane,
    // and its IQ4_NL expert planes take the gathered GEMV alone.
    // Their coverage is moe_kq_gemv_gate (fmt 2 / 45) in
    // test_metal_gemv_kernels.das and moe_mulmm_kq_gate /
    // moe_mulmm_split_gate in test_metal_gemm_kernels.das
    "metal_moe_gemv_k2", "MetalMoeMulMmK2T", "MetalMoeMulMmIq4nlT",
    // the simdgroup base mul_mm of the split formats - the routed
    // site's form on a box without the tensor toolchain, or where the
    // window took no gather panel: no window of the run takes it on
    // these formats' expert planes. Their coverage is
    // moe_mulmm_kq_gate in test_metal_gemm_kernels.das
    "metal_moe_mulmm_k2", "metal_moe_mulmm_iq4nl",
    "metal_moe_mulmm_q40", "metal_moe_mulmm_nv4",
    "metal_moe_mulmm_iq3s",
    "metal_moe_mulmm_iq2xs",
    // the affine members' (mlx4 / mlx8 / mlx4b / mlx8b) forms no
    // cell of the run dispatches: the bare `metal_kq_gemv_mlx`
    // entry, the B4/B8 small-batch stamps, the dev-W pass, the dense
    // tensor twins under their kq_mulmm_<member> crowns and their
    // 16-row split, and the routed forms (the expert-indexed GEMV,
    // the routed base tile and its twins). The run's dense affine
    // cells (the metal-mlx8 / metal-mlx4 arms of
    // test_mlx_snapshot.das, the batch-per32 arm of
    // `_metal_decode_cells.das`) step no batch past two rows and bake
    // no dev-W image; their prefill arms pin kq_mulmm_mlx4 / _mlx8
    // and serve a twin of the member (`_mlx_cells.das`), so the bare
    // `MetalKqMulMmMlx4T` / `Mlx8T` prefixes are no entries, and a
    // 40-row window reaches none of the 128-row twins. The routed
    // forms' carrier is the 35B-A3B snapshot (the metal-35b arm,
    // large tier). The model-free coverage is the affine rows of the
    // per-32 gates in test_metal_prefill_kernels.das, embed_b32_gate
    // in test_metal_misc_kernels.das and rows8_aff_gate in
    // test_metal_gemm_kernels.das
    "metal_kq_gemv_mlx", "metal_moe_gemv_mlx",
    "MetalB32MvB4Mlx", "MetalB32MvB8Mlx",
    "MetalKqDequantMlx", "metal_moe_mulmm_mlx", "MetalMoeMulMmMlx", "MetalMoeGemvMlx",
    "MetalKqMulMmMlx4TH", "MetalKqMulMmMlx4TH128", "MetalKqMulMmMlx4THDb",
    "MetalKqMulMmMlx4THDb128",
    "MetalKqMulMmMlx8TH", "MetalKqMulMmMlx8TH128", "MetalKqMulMmMlx8THDb",
    "MetalKqMulMmMlx8THDb128",
    "MetalKqMulMmMlx4bT", "MetalKqMulMmMlx4bTH", "MetalKqMulMmMlx4bTH128", "MetalKqMulMmMlx4bTHDb",
    "MetalKqMulMmMlx4bTHDb128",
    "MetalKqMulMmMlx8bT", "MetalKqMulMmMlx8bTH", "MetalKqMulMmMlx8bTH128", "MetalKqMulMmMlx8bTHDb",
    "MetalKqMulMmMlx8bTHDb128",
    "MetalKqMulMmMlx4THSk16_metal_kq_mulmm_db", "MetalKqMulMmMlx4TSk16_metal_kq_mulmm_db",
    "MetalKqMulMmMlx8THSk16_metal_kq_mulmm_db",
    "MetalKqMulMmMlx8TSk16_metal_kq_mulmm_db", "MetalKqMulMmMlx4bTHSk16_metal_kq_mulmm_db",
    "MetalKqMulMmMlx4bTSk16_metal_kq_mulmm_db",
    "MetalKqMulMmMlx8bTHSk16_metal_kq_mulmm_db", "MetalKqMulMmMlx8bTSk16_metal_kq_mulmm_db",
    // the 6-bit member mlx6b's dense forms (the prefixes above take
    // its GEMV, small-batch, gather, base tile, dev-W and rows8
    // stamps): no cell of the run loads a 6-bit plane - the
    // Qwen3.8-Flash-Next pack's attention planes carry one; the
    // model-free coverage is the mlx6b rows of the same three files'
    // gates
    "MetalKqMulMmMlx6bT", "MetalKqMulMmMlx6bTH", "MetalKqMulMmMlx6bTH128", "MetalKqMulMmMlx6bTHDb",
    "MetalKqMulMmMlx6bTHDb128",
    "MetalKqMulMmMlx6bTHSk16_metal_kq_mulmm_db", "MetalKqMulMmMlx6bTSk16_metal_kq_mulmm_db",
    // the fused mixer pair's affine arms and the routed pair's affine
    // stamps: no cell of the run serves an affine mixer or an affine
    // routed stack on the fused pair; the coverage is
    // gemv_hcn_aff_gate / gemv_sg_lo_aff_gate in
    // test_metal_misc_kernels.das and
    // moe_fused_aff_gate in test_metal_gemv_kernels.das
    "MetalGemvHcNMlx", "MetalGemvSgLoMlx", "metal_moe_down2_mlx", "MetalMoeGuMlx",
    // the 16-deep router select stamp: it serves past 256 experts,
    // and no MoE model the run loads routes more than 256. Its
    // coverage is moe_select_gate's ne=512 and ne=300 cases in
    // test_metal_prefill_kernels.das
    "MetalMoeSelect512",
    // its down-scale twin: a gemma-4 MoE past 256 experts, which no
    // file carries (the 26B routes 128). Its coverage is
    // moe_select_gate's ne=300 down-scale case in
    // test_metal_prefill_kernels.das
    "MetalMoeSelect512Ds",
    // the unconverted geglu arm of the bound-total MoE activation:
    // the gemma4moe row picks the geglu family on moe_dense_shexp,
    // and its panel always clears CVT_MIN_ROWS, so the HX twin serves
    // and this one never does. Its coverage is ew2_hx_gate(geglu =
    // true, bounded = true) in test_metal_misc_kernels.das
    "MetalGegluB",
    // the hyper-connection mixer, its scatter, the n-gram side input
    // and the σ(z) deltanet gate (qwen4exp): the one carrier is
    // Qwen3.8-Flash-Next (94 GB, the large tier), which no cell of
    // the run loads. Their coverage is the hc_*/ple_* gates in
    // test_metal_misc_kernels.das and the sigmoid leg of
    // dn_chain_gate in test_metal_prefill_kernels.das
    "metal_hc_norm", "MetalHcLo", "metal_hc_init", "metal_hc_mix", "metal_hc_combine",
    "metal_hc_stream_mean", "metal_ple_gate",
    "metal_ple_conv", "MetalDnGateSig",
    // ... and the single row's fused seams (the mixer's opening and
    // up, the out projections' scatter, the routed combine's
    // scatter): the gemv_hcn / gemv_sg_lo / gemv_hc / combine_sh_hc
    // gates in test_metal_misc_kernels.das
    "MetalQ8GemvHcN", "MetalQ8GemvSgLo", "MetalQ8GemvHc", "metal_moe_combine_sh_hc",
    // ... and the five-to-eight-row mixer's four (the prep, the
    // stream-split down, the finish, the up with the mix):
    // hc_mix8_gate in test_metal_misc_kernels.das
    "MetalHcPrep8", "MetalHcFin8", "MetalRows8Q8Hc", "MetalRows8Q8HcMix",
    // the f32 rows' element-walk GEMV pair: it serves an f32 row set
    // whose dim, base or column stride sits off the float4 lattice
    // (an E-series pre-step projection or per-layer plane in f32 rows
    // at such a width), and every model the run loads steps its
    // widths in whole float4s, so no cell reaches it. Its coverage is
    // f32_rows_gate's off-lattice arms in
    // test_metal_prefill_kernels.das
    "metal_f32_rows_one", "metal_f32_rows_b",
    // the E-series projection's bf16 halfword walk: it serves bf16
    // projection rows whose model dim or base is odd, and every
    // E-series model the run loads has an even dim on an even base.
    // Its coverage is ple_proj_bf16_gate's odd arms in
    // test_metal_prefill_kernels.das
    "metal_ple_proj_bf16_hw_one", "metal_ple_proj_bf16_hw",
    // gigaam's depthwise conv over f32 taps: it serves a carrier
    // whose depthwise taps hold no f16, and the run's gigaam
    // carrier (gigaam-v3-e2e-rnnt-f16.gguf) holds them in f16, which
    // `metal_ga_hdw` serves. Its coverage is ga_dw_gate in
    // test_metal_prefill_kernels.das
    "metal_ga_dw"
]

//! Blind entries whose stamp compiles only behind a box crown, and
//! the crown that reaches them: where the box crowns it a cell of
//! the run may dispatch the stamp, and where it does not the stamp
//! never compiled and the entry matches no census key - the drain
//! assert would red on a correct box either way. A stamp a cell of
//! the run dispatches under a crown the cell pins itself is no
//! entry: it dispatches on every box.
//! `CENSUS_NEVER_DISPATCHED` has no vocabulary for "blind only off
//! this crown", so the pair is named here.
let private CENSUS_CROWN_CONDITIONAL : table<string; string> <- {
    // the dense tensor twins every format stamps behind its own
    // kq_mulmm crown: the tall and double-buffered forms of the
    // split-scale formats, every form of the per-32 formats
    "MetalKqMulMmIq4xsTH128" => "kq_mulmm_iq4xs", "MetalKqMulMmIq4xsTHDb" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq4xsTHDb128" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq3s4TH128" => "kq_mulmm_iq3s4", "MetalKqMulMmIq3s4THDb" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq3s4THDb128" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq4nlTH128" => "kq_mulmm_iq4nl", "MetalKqMulMmIq4nlTHDb" => "kq_mulmm_iq4nl",
    "MetalKqMulMmIq4nlTHDb128" => "kq_mulmm_iq4nl",
    "MetalKqMulMmQ40TH128" => "kq_mulmm_q40", "MetalKqMulMmQ40THDb" => "kq_mulmm_q40",
    "MetalKqMulMmQ40THDb128" => "kq_mulmm_q40",
    "MetalKqMulMmNv4TH128" => "kq_mulmm_nv4", "MetalKqMulMmNv4THDb" => "kq_mulmm_nv4",
    "MetalKqMulMmNv4THDb128" => "kq_mulmm_nv4",
    "MetalKqMulMmNv4T_metal_kq_mulmm_iq4xs_t" => "kq_mulmm_nv4",
    "MetalKqMulMmNv4TH_metal_kq_mulmm_iq4xs_t" => "kq_mulmm_nv4",
    "MetalKqMvB2Nv4H" => "kq_rows_half_nv4", "MetalKqMvB4Nv4H" => "kq_rows_half_nv4",
    "MetalKqMvB8Nv4H" => "kq_rows_half_nv4",
    "MetalKqMulMmK3TH128" => "kq_mulmm_k3", "MetalKqMulMmK3THDb" => "kq_mulmm_k3",
    "MetalKqMulMmK3THDb128" => "kq_mulmm_k3",
    "MetalKqMulMmK2TH128" => "kq_mulmm_k2", "MetalKqMulMmK2THDb" => "kq_mulmm_k2",
    "MetalKqMulMmK2THDb128" => "kq_mulmm_k2",
    "MetalKqMulMmIq2sTH128" => "kq_mulmm_iq2s", "MetalKqMulMmIq2sTHDb" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2sTHDb128" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2xsTH128" => "kq_mulmm_iq2xs", "MetalKqMulMmIq2xsTHDb" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xsTHDb128" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xxsTH128" => "kq_mulmm_iq2xxs", "MetalKqMulMmIq2xxsTHDb" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmIq2xxsTHDb128" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmIq3sTHDb" => "kq_mulmm_iq3s", "MetalKqMulMmIq3sTHDb128" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3xxsTHDb" => "kq_mulmm_iq3xxs", "MetalKqMulMmIq3xxsTHDb128" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmQ51TH" => "kq_mulmm_q51",
    "MetalKqMulMmQ51TH128" => "kq_mulmm_q51", "MetalKqMulMmQ51THDb" => "kq_mulmm_q51",
    "MetalKqMulMmQ51THDb128" => "kq_mulmm_q51",
    "MetalKqMulMmIq4nl32T" => "kq_mulmm_iq4nl32", "MetalKqMulMmIq4nl32TH" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmIq4nl32TH128" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmIq4nl32THDb" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmIq4nl32THDb128" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmQ4032T" => "kq_mulmm_q4032", "MetalKqMulMmQ4032TH" => "kq_mulmm_q4032",
    "MetalKqMulMmQ4032TH128" => "kq_mulmm_q4032", "MetalKqMulMmQ4032THDb" => "kq_mulmm_q4032",
    "MetalKqMulMmQ4032THDb128" => "kq_mulmm_q4032",
    "MetalKqMulMmMlx4TH" => "kq_mulmm_mlx4",
    "MetalKqMulMmMlx4TH128" => "kq_mulmm_mlx4", "MetalKqMulMmMlx4THDb" => "kq_mulmm_mlx4",
    "MetalKqMulMmMlx4THDb128" => "kq_mulmm_mlx4",
    "MetalKqMulMmMlx8TH" => "kq_mulmm_mlx8",
    "MetalKqMulMmMlx8TH128" => "kq_mulmm_mlx8", "MetalKqMulMmMlx8THDb" => "kq_mulmm_mlx8",
    "MetalKqMulMmMlx8THDb128" => "kq_mulmm_mlx8",
    "MetalKqMulMmMlx4bT" => "kq_mulmm_mlx4b", "MetalKqMulMmMlx4bTH" => "kq_mulmm_mlx4b",
    "MetalKqMulMmMlx4bTH128" => "kq_mulmm_mlx4b", "MetalKqMulMmMlx4bTHDb" => "kq_mulmm_mlx4b",
    "MetalKqMulMmMlx4bTHDb128" => "kq_mulmm_mlx4b",
    "MetalKqMulMmMlx8bT" => "kq_mulmm_mlx8b", "MetalKqMulMmMlx8bTH" => "kq_mulmm_mlx8b",
    "MetalKqMulMmMlx8bTH128" => "kq_mulmm_mlx8b", "MetalKqMulMmMlx8bTHDb" => "kq_mulmm_mlx8b",
    "MetalKqMulMmMlx8bTHDb128" => "kq_mulmm_mlx8b",
    "MetalKqMulMmMlx6bT" => "kq_mulmm_mlx6b", "MetalKqMulMmMlx6bTH" => "kq_mulmm_mlx6b",
    "MetalKqMulMmMlx6bTH128" => "kq_mulmm_mlx6b", "MetalKqMulMmMlx6bTHDb" => "kq_mulmm_mlx6b",
    "MetalKqMulMmMlx6bTHDb128" => "kq_mulmm_mlx6b",
    "MetalKqMulMmQ5032T" => "kq_mulmm_q5032", "MetalKqMulMmQ5032TH" => "kq_mulmm_q5032",
    "MetalKqMulMmQ5032TH128" => "kq_mulmm_q5032", "MetalKqMulMmQ5032THDb" => "kq_mulmm_q5032",
    "MetalKqMulMmQ5032THDb128" => "kq_mulmm_q5032",
    // the split-K pair behind each format's own kq_mulmm crown ...
    // and the 16-row stamps behind the same crowns (the q8 blob's
    // behind mulmm_q8)
    "MetalKqMulMmK4THSk16_metal_kq_mulmm_db" => "kq_mulmm_k4",
    "MetalKqMulMmK4TSk16_metal_kq_mulmm_db" => "kq_mulmm_k4",
    "MetalKqMulMmK5THSk16_metal_kq_mulmm_db" => "kq_mulmm_k5",
    "MetalKqMulMmK5TSk16_metal_kq_mulmm_db" => "kq_mulmm_k5",
    "MetalKqMulMmK6THSk16_metal_kq_mulmm_db" => "kq_mulmm_k6",
    "MetalKqMulMmK6TSk16_metal_kq_mulmm_db" => "kq_mulmm_k6",
    "MetalKqMulMmIq4xsTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq4xsTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq3s4THSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq3s4TSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq4nlTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4nl",
    "MetalKqMulMmIq4nlTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4nl",
    "MetalKqMulMmQ40THSk16_metal_kq_mulmm_db" => "kq_mulmm_q40",
    "MetalKqMulMmQ40TSk16_metal_kq_mulmm_db" => "kq_mulmm_q40",
    "MetalKqMulMmNv4THSk16_metal_kq_mulmm_db" => "kq_mulmm_nv4",
    "MetalKqMulMmNv4TSk16_metal_kq_mulmm_db" => "kq_mulmm_nv4",
    "MetalKqMulMmK3THSk16_metal_kq_mulmm_db" => "kq_mulmm_k3",
    "MetalKqMulMmK3TSk16_metal_kq_mulmm_db" => "kq_mulmm_k3",
    "MetalKqMulMmK2THSk16_metal_kq_mulmm_db" => "kq_mulmm_k2",
    "MetalKqMulMmK2TSk16_metal_kq_mulmm_db" => "kq_mulmm_k2",
    "MetalKqMulMmIq3sTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3sTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3xxsTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmIq3xxsTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmIq2sTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2sTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2xsTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xsTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xxsTHSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmIq2xxsTSk16_metal_kq_mulmm_db" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmQ51THSk16_metal_kq_mulmm_db" => "kq_mulmm_q51",
    "MetalKqMulMmQ51TSk16_metal_kq_mulmm_db" => "kq_mulmm_q51",
    "MetalKqMulMmIq4nl32THSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmIq4nl32TSk16_metal_kq_mulmm_db" => "kq_mulmm_iq4nl32",
    "MetalKqMulMmQ4032THSk16_metal_kq_mulmm_db" => "kq_mulmm_q4032",
    "MetalKqMulMmQ4032TSk16_metal_kq_mulmm_db" => "kq_mulmm_q4032",
    "MetalKqMulMmQ5032THSk16_metal_kq_mulmm_db" => "kq_mulmm_q5032",
    "MetalKqMulMmQ5032TSk16_metal_kq_mulmm_db" => "kq_mulmm_q5032",
    "MetalKqMulMmMlx4THSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx4",
    "MetalKqMulMmMlx4TSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx4",
    "MetalKqMulMmMlx8THSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx8",
    "MetalKqMulMmMlx8TSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx8",
    "MetalKqMulMmMlx4bTHSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx4b",
    "MetalKqMulMmMlx4bTSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx4b",
    "MetalKqMulMmMlx8bTHSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx8b",
    "MetalKqMulMmMlx8bTSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx8b",
    "MetalKqMulMmMlx6bTHSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx6b",
    "MetalKqMulMmMlx6bTSk16_metal_kq_mulmm_db" => "kq_mulmm_mlx6b",
    "MetalQ8MulMmTHSk16_metal_q8_mulmm_t" => "mulmm_q8",
    "MetalQ8MulMmTSk16_metal_q8_mulmm_t" => "mulmm_q8",
    "MetalKqMulMmIq4xsTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq4xsTSk_metal_kq_mulmm_db" => "kq_mulmm_iq4xs",
    "MetalKqMulMmIq3s4THSk_metal_kq_mulmm_db" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq3s4TSk_metal_kq_mulmm_db" => "kq_mulmm_iq3s4",
    "MetalKqMulMmIq4nlTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq4nl",
    "MetalKqMulMmIq4nlTSk_metal_kq_mulmm_db" => "kq_mulmm_iq4nl",
    "MetalKqMulMmQ40THSk_metal_kq_mulmm_db" => "kq_mulmm_q40",
    "MetalKqMulMmQ40TSk_metal_kq_mulmm_db" => "kq_mulmm_q40",
    "MetalKqMulMmK3THSk_metal_kq_mulmm_db" => "kq_mulmm_k3",
    "MetalKqMulMmK3TSk_metal_kq_mulmm_db" => "kq_mulmm_k3",
    "MetalKqMulMmK2THSk_metal_kq_mulmm_db" => "kq_mulmm_k2",
    "MetalKqMulMmK2TSk_metal_kq_mulmm_db" => "kq_mulmm_k2",
    "MetalKqMulMmIq3sTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3sTSk_metal_kq_mulmm_db" => "kq_mulmm_iq3s",
    "MetalKqMulMmIq3xxsTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmIq3xxsTSk_metal_kq_mulmm_db" => "kq_mulmm_iq3xxs",
    "MetalKqMulMmIq2sTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2sTSk_metal_kq_mulmm_db" => "kq_mulmm_iq2s",
    "MetalKqMulMmIq2xsTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xsTSk_metal_kq_mulmm_db" => "kq_mulmm_iq2xs",
    "MetalKqMulMmIq2xxsTHSk_metal_kq_mulmm_db" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmIq2xxsTSk_metal_kq_mulmm_db" => "kq_mulmm_iq2xxs",
    "MetalKqMulMmK4TSk_metal_kq_mulmm_db" => "kq_mulmm_k4",
    "MetalKqMulMmK6TSk_metal_kq_mulmm_db" => "kq_mulmm_k6",
    "MetalKqMvB2K4R2" => "kq_mvb2_k4_r2",
    "MetalRmsNormHXT" => "kq_rows_half_k4",
    "metal_add_rms_hxt" => "kq_rows_half_k4",
    "metal_pre_add_rms_hxt" => "kq_rows_half_k4",
    "MetalSwigluHXT" => "kq_rows_half_k4",
    "MetalGegluHXT" => "kq_rows_half_k4",
    "MetalKqMvB2K5H" => "kq_rows_half_k5",
    "MetalKqMvB4K5H" => "kq_rows_half_k5",
    "MetalKqMvB8K5H" => "kq_rows_half_k5",
    "metal_kq_gemv_iq3s_f4" => "kq_gemv_iq3s_f4",
    "metal_moe_gemv_iq3s_f4" => "kq_gemv_iq3s_f4",
    "metal_kq_gemv_iq3xxs_f4" => "kq_gemv_iq3xxs_f4",
    "MetalKqGemvIq2xxsF4" => "kq_gemv_iq2xxs_f4"
}

// The Vulkan census's DECLARED blind spots, by census name
// (`<vk_dispatch name>_spv`): kernel classes no cell of a whole run
// dispatches, each with the reason and the kernel-unit cell that does.
// An entry some rows dispatch on one kind of device and no row
// reaches on another has a line in `VK_CENSUS_PICK_CONDITIONAL` too.
let private VK_CENSUS_NEVER_DISPATCHED <- [
    // the iq4nl32 rail's cm2 l stamp (the 32-block IQ4_NL down planes
    // off the 256 lattice: the run's carriers dispatch its m and s
    // stamps and its decode GEMV, none its l stamp); its coverage is
    // test_vkd_iq4nl32_cm2_batch
    "iq4nl32_batch_cm2l_cls_spv",
    // the nv4 rail's per-op batch tile and its KHR tile (NVFP4
    // planes: the run's carriers dispatch the decode GEMV, the
    // token-table gather and the cm2 stamps); their coverage is
    // test_vkd_kq_batch_family
    "kq_batch_nv4_cls_spv", "kq_batch_nv4_khr_cls_spv",
    // the hyper-connection chain (Qwen3.8-Flash-Next, the one
    // qwen4exp carrier, above the zoo's tier): the mixer seams and
    // the n-gram side input; their coverage is test_vkd_hc_seams,
    // test_vkd_hc_rows and test_vkd_ple_side, the serving run
    // test_gpu_resident_hc.das
    "hc_init_cls_spv", "hc_norm_cls_spv", "hc_lo_rq_cls_spv", "hc_mix_cls_spv", "hc_comb_cls_spv",
    "ple_gate_cls_spv", "ple_conv_cls_spv", "ple_commit_cls_spv",
    // ... and the deltanet out-gate's sigmoid stamps the same
    // carrier's recurrent layers take; their coverage is the sigmoid
    // arms of test_vkd_dn_family
    "dn_step_sig_cls_spv", "dn_scan_p3_sig_cls_spv",
    // the per-32 expert rails' KHR arms (the expert schedule on a
    // KHR-mode card: the 26B-A4B Q4_K_M's q51 down stacks, gpt-oss's
    // mx4 stacks, Flash-Next's iq4nl32 stacks - carriers above the
    // zoo's tier); their coverage is the KHR arm of
    // test_vkd_q51_cm2_batch, test_vkd_mx4_cm2_batch and
    // test_vkd_iq4nl32_cm2_batch
    "q51_batch_khr_cls_spv", "mx4_batch_khr_cls_spv", "iq4nl32_batch_khr_cls_spv",
    // the int8 tiles of the routed block, their feeds, row map and
    // device check: a column of the routed block rides them where the
    // served set names it (`vk_mmq_any` - an AMD device's own rule
    // `vk_alt_mmq_by_vendor`, no other device's). There the census's
    // four routed rows (the Qwen1.5-MoE and Qwen3-30B cuts) dispatch
    // the k4, k6, q5032 and q8 stamps their planes and that rule's
    // columns reach, with the feeds, the row map and the check: those
    // ten are conditional (`VK_CENSUS_PICK_CONDITIONAL`). The other
    // six no row reaches on any device - no routed row holds a k5 or
    // q51 expert plane, and the rule leaves the down's m pieces on
    // the f16 tile. Their coverage is test_vkd_mmq_tiles,
    // test_vkd_mmq_feed, test_vkd_mmq_inv and
    // test_vkd_mmq_device_check in test_vulkan_kernels_gemm.das
    "kq_batch_k4_mmq_cls_spv", "kq_batch_k4_mmqs_cls_spv", "kq_batch_k5_mmq_cls_spv",
    "kq_batch_k5_mmqs_cls_spv", "kq_batch_k6_mmq_cls_spv", "kq_batch_k6_mmqs_cls_spv",
    "q51_batch_mmq_cls_spv", "q51_batch_mmqs_cls_spv", "q5032_batch_mmq_cls_spv",
    "q5032_batch_mmqs_cls_spv", "q8_batch_mmq_cls_spv", "q8_batch_mmqs_cls_spv", "mmq_feed_cls_spv",
    "mmq_feed16_cls_spv", "mmq_inv_cls_spv", "mmq_probe_cls_spv",
    // the IQ4_XS and iq3s4 experts' fused gate+up+act+requant GEMVs
    // (the gemma-4 26B's Unsloth dynamic files' gate_up stacks, above
    // the run's tier); their coverage is test_vkd_kq_gemv_k4_gu in
    // test_vulkan_kernels.das
    "kq_gemv_iq4xs_gu_cls_spv", "kq_gemv_iq3s4_gu_cls_spv",
    // the attention-sink twins a whole run leaves idle (gpt-oss's
    // per-head sink logits: its carrier dispatches the f16-out cm2
    // tile and the f16 token pass): the f32-out h64 cm2 tile, the
    // KHR tiles and the f32 token pass; their coverage is the sink
    // arms of test_vkd_fa_cm2, test_vkd_fa_khr and test_vkd_da_attn
    // in test_vulkan_kernels.das
    "fa_cm2_h64_sink_cls_spv", "fa_khr_h64_sink_cls_spv", "fa_khr_h64_sink_f16_cls_spv",
    "da_attn_sink_cls_spv",
    // the token command's f32-mirror attention stamps (the four-head
    // slab and the two-head one): the census steps every session on
    // the f16 mirror the tier arms, the f32 twin serves under
    // DASLLAMA_VK_KV32=1 (gemma-2-2b Q8_0's f32 parity run dispatches
    // both); their coverage is test_vkd_da_attn and
    // test_vkd_da_attn_rqk in test_vulkan_kernels.das
    "da_attn_cls_spv", "da_attn_g2_cls_spv",
    // the KHR flash-attention tile's 256 and 512 heads: the mode
    // sweep's mm row reaches the 64 and 128 heads through the zoo's
    // qwens; no zoo row carries a 512 head or an ungated or capped
    // 256 head (the gemmas sit above the tier); the gated 256 keys
    // are the zoo's Qwen3.5-9B's, which the census runs at the box's
    // coopmat mode - cm2 on the pod, so its row takes the cm2 stamp;
    // a KHR-only device runs that file and the Qwen3.6-35B-A3B files
    // on the KHR tile, an AMD one on the tile-first alternate of the
    // f16-out key (below); their coverage is test_vkd_fa_khr,
    // test_vkd_fa_khr_h256_softcap and test_vkd_fa_khr_h256_gated in
    // test_vulkan_kernels.das
    "fa_khr_h256_cls_spv", "fa_khr_h256_f16_cls_spv", "fa_khr_h256_cap_cls_spv",
    "fa_khr_h256_cap_f16_cls_spv", "fa_khr_h256_g_cls_spv", "fa_khr_h256_g_f16_cls_spv",
    "fa_khr_h512_cls_spv", "fa_khr_h512_f16_cls_spv",
    // the bidirectional flash tiles' KHR arms (the vision towers'
    // encoder attention: the towers' carriers dispatch the cm2 arms);
    // their kernel coverage is test_vkt_fa_bidir in
    // test_vulkan_tower_kernels.das
    "fa_khr_h64_bidir_cls_spv", "fa_khr_h128_bidir_cls_spv",
    // the whisper-class projector tails' swapped SwiGLU; its coverage
    // is test_vkt_tower_tail_rows
    "tower_swiglu_swapped_cls_spv",
    // gigaam's biased tap-major depthwise conv: no census row reaches
    // it because the Vulkan census loads no gigaam carrier
    // (gigaam-v3-e2e-rnnt-f16.gguf, run by
    // test_gigaam_discrete_twin); its coverage is
    // test_vkt_tower_fastconformer
    "tower_ga_dw_cls_spv",
    // canary's per-feature mel normalization; its coverage is
    // test_vkt_tower_front_mel
    "tower_mel_norm_cls_spv",
    // canary's front depthwise conv; its coverage is
    // test_vkt_tower_front_mel
    "tower_dw_conv_cls_spv",
    // canary's rel-plane attention at the 64-wide stamp (the canary
    // twin dispatches the 128-wide stamp, canary's head width): it
    // serves no stocked carrier; its kernel coverage is
    // test_vkt_tower_cn_attn_rel_plane in
    // test_vulkan_tower_kernels.das
    "tower_cn_attn_r64_cls_spv",
    // the whisper decoder's f32 K/V store and its chunked attention's
    // f32 combine (the stocked whisper files dispatch the f16 store
    // and the requant combine); their kernel coverage is
    // test_vkt_wdec_classes in test_vulkan_tower_kernels.das
    "tower_wdec_kv_cls_spv", "tower_wdec_attn_comb_cls_spv",
    // the audio towers' classes no stocked carrier dispatches: the
    // 64-wide windowed RPE attention stamp and the FastConformer's
    // centered depthwise conv; their kernel coverage is
    // test_vkt_tower_conformer and test_vkt_tower_fastconformer
    "tower_g4a_attn64_cls_spv", "tower_cn_dw_cls_spv",
    // the KHR f16 GEMM (the E-series projection off cm2): no zoo row
    // carries a per-layer embedding; its coverage is
    // test_vkd_f16_gemm_khr
    "f16_gemm_khr_cls_spv",
    // the batched windowed decode attention and its f16 twin (a
    // sliding window over the batch rows: the gemma carriers, no zoo
    // row); its coverage is test_vkd_da_attn_bw in
    // test_vulkan_kernels.das
    "da_attn_bw_cls_spv", "da_attn_bw_f16_cls_spv",
    // the per-32 routed down GEMVs' summing twins with no stocked
    // Vulkan carrier (they serve only the gemma-4 MoE's device
    // experts). A key a line:
    // q4032: gemma-4-26B-A4B-6L-Q4_0-local.gguf (Q4_0 down), no
    //        stocked Vulkan file; test_vkd_q4032_gemv
    // q5032: no stocked file carries a Q5_0 down stack;
    //        test_vkd_q5032_gemv
    "q4032_gemv_sum_cls_spv", "q5032_gemv_sum_cls_spv",
    // the E-series BF16 projection's halfword stamp, which serves an
    // odd model dim or base - every stocked E-series carrier's dim is
    // even on an even base, so no file reaches it;
    // test_vkd_router_gemv_cols's halfword arms
    "router_gemv_bf16_hw_cls_spv",
    // the N-column decode GEMV leaves no carrier's N-row token
    // command reaches on a whole run; the q8 row-pair stamps serve
    // only past `g_q8_n2_min_n`, off until a box's token command
    // proves the pair. Their coverage is test_vkd_gemv_ncol in
    // test_vulkan_kernels.das
    "q8_gemv_n2p_cls_spv", "q8_gemv_n4p_cls_spv", "q8_gemv_n8p_cls_spv", "kq_gemv_k5_n_cls_spv",
    "kq_gemv_q40_n_cls_spv", "kq_gemv_iq3s4_n_cls_spv", "kq_gemv_k3_n_cls_spv",
    "kq_gemv_k2_n_cls_spv", "kq_gemv_iq3s_n_cls_spv", "kq_gemv_iq3xxs_n_cls_spv",
    "kq_gemv_iq4nl_n_cls_spv", "kq_gemv_iq2s_n_cls_spv", "kq_gemv_iq2xs_n_cls_spv",
    "kq_gemv_iq2xxs_n_cls_spv", "kq_gemv_q40_n4_cls_spv", "kq_gemv_iq3s_n4_cls_spv",
    "kq_gemv_iq2xxs_n4_cls_spv",
    // the sink stamp over a tq4 K/V mirror (gpt-oss with the mirror
    // asked in tq4: no cell of a whole run loads it so); its coverage
    // is the sink arm of test_vkc_tq4_da_attn in
    // test_vulkan_kv_codec_kernels.das
    "da_attn_tq4_sink_cls_spv",
    // the stand-ins of the KHR flash slots listed above (the 256 and
    // 512 heads, the capped 256, the sink 64, the bidirectional 64
    // and 128): a slot's tile-first alternate and its small-tile twin
    // serve at that slot's sites alone, which no census row reaches.
    // The alternate serves under `VK_ALT_FA_TFIRST`, an AMD device's
    // own rule: there every stocked file that reaches a slot
    // dispatches its alternate in the slot's place -
    // gemma-4-E2B-it-Q8_0.gguf in test_gpu_resident_gemma4_e2b.das
    // the 256 and 512 heads', the other slots' the files their
    // entries name. The twin serves under a shared-memory cap below
    // the tile's footprint (`FA_KHR_WG_BYTES`) with the bit out of
    // the served set, which no stocked box gives. Their coverage is
    // test_vkd_fa_khr_tfirst and test_vkd_fa_khr_small_twins in
    // test_vulkan_kernels_attn.das, the bidirectional slots' the
    // second cell of test_vkt_fa_bidir in
    // test_vulkan_tower_kernels.das. Of the gated 256 slot's two
    // alternates the f32-out one is an entry and the f16-out one is
    // not: on an AMD device the Qwen3.5-9B row (`run.das --category
    // qwen35-9b`) dispatches the f16-out alternate in its slot's
    // place - that row's windows take the f16-out form - and no row
    // takes the f32-out one
    "fa_khr_h256_tf_cls_spv", "fa_khr_h256_f16_tf_cls_spv", "fa_khr_h256_cap_tf_cls_spv",
    "fa_khr_h256_cap_f16_tf_cls_spv", "fa_khr_h256_g_tf_cls_spv", "fa_khr_h512_tf_cls_spv",
    "fa_khr_h512_f16_tf_cls_spv", "fa_khr_h64_sink_tf_cls_spv", "fa_khr_h64_sink_f16_tf_cls_spv",
    "fa_khr_h64_bidir_tf_cls_spv", "fa_khr_h128_bidir_tf_cls_spv", "fa_khr_h256_cls_s_spv",
    "fa_khr_h256_f16_cls_s_spv", "fa_khr_h256_cap_cls_s_spv", "fa_khr_h256_cap_f16_cls_s_spv",
    "fa_khr_h256_g_cls_s_spv", "fa_khr_h256_g_f16_cls_s_spv", "fa_khr_h512_cls_s_spv",
    "fa_khr_h512_f16_cls_s_spv", "fa_khr_h64_sink_cls_s_spv", "fa_khr_h64_sink_f16_cls_s_spv",
    "fa_khr_h64_bidir_cls_s_spv", "fa_khr_h128_bidir_cls_s_spv",
    // the gemma-4 MoE combine's small-slab twins: the three combine
    // slots listed above serve their twin on a device whose
    // shared-memory cap is under the row slab's
    // (`ROW_SLAB_WG_BYTES`), and the slots' carrier, the 26B-A4B,
    // sits above the zoo's tier. On such a device the 26B cuts of
    // test_gpu_resident_gemma4_moe.das dispatch them (the gemma4-moe
    // category's files among them); their coverage is
    // test_vkd_moe_routing's gemma-4 combine arm in
    // test_vulkan_kernels.das, which runs the twin where the device's
    // cap picks it
    "cls_ar_comb_g4_s_spv", "cls_ar_comb_g4_f16_b_s_spv", "cls_ar_comb_g4_rq_s_spv",
    // the rotated-walk alternates of the KHR tiles listed above (the
    // staged f32 tile, the nv4 tile, the per-32 expert rails' q51,
    // mx4 and iq4nl32 tiles): an alternate serves at its slot's sites
    // alone, under `VK_ALT_KHR_ROT` - a bit only a box's race or a
    // `DASLLAMA_VK_ALT_STAMPS` pin sets, no device's own rule - so no
    // stocked file dispatches one on a box whose stored set lacks the
    // bit; their coverage is test_vkd_khr_rot_tiles in
    // test_vulkan_kernels_gemm.das
    "f32_batch_khr_rot_cls_spv", "kq_batch_nv4_khr_rot_cls_spv", "q51_batch_khr_rot_cls_spv",
    "mx4_batch_khr_rot_cls_spv", "iq4nl32_batch_khr_rot_cls_spv",
    // the N-column decode GEMV leaves' rotated twins: they serve the
    // N-row token command, as the N-column leaves listed above do,
    // and only over a plane whose quant rows collide
    // (`gemv_n_rot_wanted`, `q8_gemv_n_rot_wanted`) under
    // `VK_ALT_N_ROT`, an AMD device's own rule. On an AMD device the
    // regions files those leaves' entry names dispatch a format's
    // twin over such a plane - gemma-4-E2B-it-Q8_0.gguf in
    // test_gpu_resident_gemma4_e2b.das takes the q8 `n2r`, `n4r` and
    // `n8r` twins, the Qwen1.5-MoE-A2.7B-4L cuts of
    // test_gpu_resident_qwen_moe.das `q8_gemv_n2r_cls`; their
    // coverage is the rotated leg of test_vkd_gemv_ncol in
    // test_vulkan_kernels.das
    "q8_gemv_n2r_cls_spv", "q8_gemv_n4r_cls_spv", "q8_gemv_n8r_cls_spv", "q8_gemv_n2o_cls_spv",
    "q8_gemv_n4o_cls_spv", "q8_gemv_n8o_cls_spv", "kq_gemv_k4_nr_cls_spv", "kq_gemv_k4_n4r_cls_spv",
    "kq_gemv_q40_nr_cls_spv", "kq_gemv_q40_n4r_cls_spv", "kq_gemv_iq4xs_nr_cls_spv",
    "kq_gemv_iq4xs_n4r_cls_spv", "kq_gemv_iq3s4_nr_cls_spv", "kq_gemv_iq3s4_n4r_cls_spv",
    "kq_gemv_iq4nl_nr_cls_spv", "kq_gemv_iq4nl_n4r_cls_spv", "kq_gemv_nv4_nr_cls_spv",
    "kq_gemv_nv4_n4r_cls_spv", "kq_gemv_k2_nr_cls_spv", "kq_gemv_k2_n4r_cls_spv",
    "kq_gemv_iq2xs_nr_cls_spv", "kq_gemv_iq2xs_n4r_cls_spv", "kq_gemv_iq2xxs_nr_cls_spv",
    "kq_gemv_iq2xxs_n4r_cls_spv",
    // the small-tile twins of the KHR flash tile's decode form, which
    // a carrier dispatches on a device under the tile's footprint
    // (`FA_KHR_WG_BYTES`); their coverage is test_vkd_da_attn_tile in
    // test_vulkan_kernels_attn.das
    "fa_khr_dec_h512_cls_s_spv", "fa_khr_dec_h256_cls_s_spv", "fa_khr_dec_h128_cls_s_spv"]


//! the Metal census's declared blind entries
//! (`CENSUS_NEVER_DISPATCHED`), a copy. Non-private:
//! the verdict's branch tests plant a table naming them
def metal_census_blind_entries : array<string> => <- clone_to_move(CENSUS_NEVER_DISPATCHED)

//! the Metal blind entries a crown makes conditional, with their
//! crowns (`CENSUS_CROWN_CONDITIONAL`), a copy. Non-private:
//! the verdict's branch tests plant a table naming them
def metal_census_crown_entries : table<string; string> => <- clone_to_move(CENSUS_CROWN_CONDITIONAL)

//! the Vulkan census's declared blind entries
//! (`VK_CENSUS_NEVER_DISPATCHED`), a copy. Non-private:
//! the verdict's branch tests plant a table naming them
def vulkan_census_blind_entries : array<string> => <- clone_to_move(VK_CENSUS_NEVER_DISPATCHED)

//! the Vulkan blind entries a driver pick makes conditional, with
//! their picks (`VK_CENSUS_PICK_CONDITIONAL`), a copy. Non-private:
//! the verdict's branch tests plant a table naming them
def vulkan_census_pick_entries : table<string; string> => <- clone_to_move(VK_CENSUS_PICK_CONDITIONAL)

//! the census table's dispatch counts alone, its notes left out
def private census_dispatches(cov : table<string; int64>) : table<string; int64> {
    var out <- {for (k, n in keys(cov), values(cov)); k => n; where !starts_with(k, CENSUS_NOTE)}
    return <- out
}

//! the dispatched kernels a census table counts: its keys above zero
def private census_served(cov : table<string; int64>) : int {
    var served = 0
    for (n in values(cov)) {
        if (n > 0l) {
            served++
        }
    }
    return served
}

//! whether a Metal zoo cell noted any tensor crown on the box
def private crowns_noted(cov : table<string; int64>) : bool {
    for (k, n in keys(cov), values(cov)) {
        return true if (n > 0l && starts_with(k, NOTE_METAL_CROWN))
    }
    return false
}

//! whether a Metal zoo cell noted the tensor crown `crown` on the box
def private crown_noted(cov : table<string; int64>; crown : string) : bool
    => (cov?["{NOTE_METAL_CROWN}{crown}"] ?? 0l) > 0l

//! Did this run compile the GATED stamp families the blind list
//! names? Two gates decide it, both read from the run itself rather
//! than re-probed: the Metal-4 tensor toolchain - the prefill's PSO
//! init puts every tensor stamp behind one probe, so the dense q8
//! tensor family's presence in the census answers for all of them -
//! and the box's tensor crowns, since every crown-gated twin compiles
//! only behind `metal_tensor_crowned` and an empty crown list means
//! none of them exist.
def private census_gated_stamps_compiled(cov : table<string; int64>) : bool {
    if (!crowns_noted(cov)) {
        return false
    }
    for (k in keys(cov)) {
        if (starts_with(k, "MetalQ8MulMmT")) {
            return true
        }
    }
    return false
}

//! The blind-spot list self-drains: census keys are the compiled
//! kernels' MSL entry names, so a declared entry whose key counted
//! dispatches is a STALE entry, and one matching no key at all names
//! nothing and would sit green forever. A box that compiled none of
//! the gated stamp families cannot tell that second case from a
//! misspelt key, so it SKIPS the entry loudly, never passes it.
def private metal_census_blind_drain(t : T?; cov : table<string; int64>) {
    let judge_unmatched = census_gated_stamps_compiled(cov)
    for (blind in CENSUS_NEVER_DISPATCHED) {
        var hits = 0l
        var named = false
        for (k, n in keys(cov), values(cov)) {
            if (starts_with(k, blind)) {
                hits += n
                named = true
            }
        }
        if (!named && !judge_unmatched) {
            t |> skip(("{blind}: this box compiled no tensor or crowned stamp, so an entry " +
                "matching no census key cannot be told from a misspelt one here"))
            continue
        }
        let crown = CENSUS_CROWN_CONDITIONAL?[blind] ?? ""
        if (!named && !empty(crown) && !crown_noted(cov, crown)) {
            t |> skip(("{blind}: the {crown} crown is not pinned on this box, so its stamp " +
                "never compiled and the entry matches no census key here"))
            continue
        }
        if (hits > 0l && !empty(crown) && crown_noted(cov, crown)) {
            t |> skip(("{blind}: the {crown} crown is pinned on this box, so a cell of the run " +
                "reaches it ({hits}) - the entry is blind only off that crown"))
            continue
        }
        t |> success(named,
            ("{blind} names no compiled kernel - spell the census key (the class's own shader " +
            "entry name, or <Class>_<kernel method>)"))
        t |> success(hits == 0l,
            ("{blind} is a DECLARED blind spot - a cell of the run dispatches it ({hits}): " +
            "drop the entry"))
    }
}

//! The Metal census verdict over `cov`, every test process's census
//! table merged (`kernel_census_table_metal`): the report prints the
//! counts and LOUD WARNINGS for the zeros, the blind list drains, and
//! on a `whole` run (`DASLLAMA_KERNEL_CENSUS_WHOLE`) at least 40
//! distinct kernels dispatched - the floor pins only that
//! the RAIL counts; coverage gaps are human calls
def metal_census_verdict(t : T?; cov : table<string; int64>; whole : bool) {
    var inscope disp <- census_dispatches(cov)
    kernel_coverage_report("metal", "compiled", disp)
    metal_census_blind_drain(t, cov)
    let served = census_served(disp)
    if (whole) {
        t |> success(served >= 40,
            "the census counted {served} distinct dispatched kernels (rail floor 40)")
    } else {
        to_log(LOG_INFO, "census floor not asserted (the run was not whole): {served} served\n")
    }
}

//! `vk_mmq_any`: the served set puts a column of the routed block on
//! the int8 tiles
let private VK_PICK_INT8_COLUMNS = "the int8 columns"


//! Blind entries some census rows dispatch on one kind of device and
//! no row reaches on another, and the driver pick that tells the two
//! apart (`vk_census_pick`): where the pick holds its carrier rows
//! dispatch the entry, and where it does not the entry is blind.
//! `VK_CENSUS_NEVER_DISPATCHED` has no vocabulary for "blind only off
//! this pick", so the pair is named here.
let private VK_CENSUS_PICK_CONDITIONAL : table<string; string> <- {
    "kq_batch_k4_mmq_cls_spv" => VK_PICK_INT8_COLUMNS,
    "kq_batch_k4_mmqs_cls_spv" => VK_PICK_INT8_COLUMNS,
    "kq_batch_k6_mmqs_cls_spv" => VK_PICK_INT8_COLUMNS,
    "q5032_batch_mmqs_cls_spv" => VK_PICK_INT8_COLUMNS,
    "q8_batch_mmq_cls_spv" => VK_PICK_INT8_COLUMNS, "q8_batch_mmqs_cls_spv" => VK_PICK_INT8_COLUMNS,
    "mmq_feed_cls_spv" => VK_PICK_INT8_COLUMNS, "mmq_feed16_cls_spv" => VK_PICK_INT8_COLUMNS,
    "mmq_inv_cls_spv" => VK_PICK_INT8_COLUMNS, "mmq_probe_cls_spv" => VK_PICK_INT8_COLUMNS
}

//! the census's routed rows (`coverage_vk_routed`): the carriers of
//! the int8 columns' entries
let private VK_ROUTED_ROWS = 4

//! what a run's carrier rows read: the routed rows that ran, and
//! whether the served set put a column of the routed block on the
//! int8 tiles
struct private VkCarrierRows {
    routed : int
    int8_columns : bool
}

//! the carrier rows' notes in a census table
def private vk_carrier_rows(cov : table<string; int64>) : VkCarrierRows {
    return VkCarrierRows(routed = int(cov?[NOTE_VK_ROUTED] ?? 0l),
        int8_columns = (cov?[NOTE_VK_INT8_COLUMNS] ?? 0l) > 0l)
}

//! a driver pick as one run reads it: whether it holds, and how many
//! of the rows that dispatch its entries ran
struct private VkPickState {
    holds : bool
    ran : int
    rows : int
    carriers : string
}

//! the driver pick `pick` on this run, by the answer the driver
//! itself acts on: the served set's int8 columns as the device stood
def private vk_census_pick(pick : string; rows : VkCarrierRows) : VkPickState {
    if (pick == VK_PICK_INT8_COLUMNS) {
        return VkPickState(holds = rows.int8_columns, ran = rows.routed, rows = VK_ROUTED_ROWS,
            carriers = "routed rows")
    }
    panic("the Vulkan census knows no pick '{pick}'")
}

//! one kernel's dispatches under its census name; a name no kernel
//! seeded panics, so a misspelt key cannot read as a zero count
def private vk_census_count(cov : table<string; int64>; kernel : string) : int64 {
    if (!key_exists(cov, kernel)) {
        panic("dasLLAMA vulkan tier: no seeded kernel named '{kernel}' in the coverage census")
    }
    return cov?[kernel] ?? 0l
}

//! The Vulkan blind list drains itself: an entry names a seeded
//! kernel - `cov` carries every seeded kernel at zero, so an entry
//! matching none names nothing and would sit green forever - and
//! counts no dispatch. A conditional entry whose pick holds is the
//! other case: its carrier rows dispatch it here, and once every one
//! of them ran it is held to a counted dispatch; with a carrier row
//! filtered out or its file absent it skips, since its carrier may be
//! the row that did not run.
def private vk_census_blind_drain(t : T?; cov : table<string; int64>; rows : VkCarrierRows) {
    var conditional = 0
    for (blind in VK_CENSUS_NEVER_DISPATCHED) {
        var named = false
        for (k in keys(cov)) {
            if (starts_with(k, blind)) {
                named = true
            }
        }
        t |> success(named,
            "{blind} names no seeded kernel - spell the census key (<vk_dispatch name>_spv)")
        // the count panics on an unseeded name; the assert above is
        // the legible red
        let hits = named ? vk_census_count(cov, blind) : 0l
        let pick = VK_CENSUS_PICK_CONDITIONAL?[blind] ?? ""
        conditional += empty(pick) ? 0 : 1
        let state = empty(pick) ? VkPickState() : vk_census_pick(pick, rows)
        if (!state.holds) {
            t |> success(hits == 0l,
                ("{blind} is a DECLARED blind spot - a stocked carrier now dispatches it " +
                "({hits}): drop the entry, its census row exists"))
        } elif (state.ran == state.rows) {
            t |> success(hits > 0l,
                ("{blind} is conditional on {pick}, which holds on this device, and with every " +
                "one of the {state.carriers} run no row dispatched it: the entry is blind here " +
                "too - drop its conditional line"))
            to_log(LOG_INFO, ("vulkan kernel coverage: {blind} is blind off {pick} alone - the " +
                "{state.carriers} dispatch it on this device ({hits})\n"))
        } else {
            t |> skip(("{blind}: {pick} holds on this device, where the {state.carriers} " +
                "dispatch it ({hits} from the {state.ran} of {state.rows} that ran) - held to a " +
                "dispatch only once every one of them runs"))
        }
    }
    t |> equal(conditional, length(VK_CENSUS_PICK_CONDITIONAL),
        "every conditional line names a blind entry by its exact text")
}

//! The Vulkan serving census verdict over `cov`, every test process's
//! census table merged (`kernel_census_table_vulkan`): the report,
//! the blind list's drain under the carrier rows' notes, and on a
//! `whole` run at least 24 distinct kernels dispatched
def vulkan_census_verdict(t : T?; cov : table<string; int64>; whole : bool) {
    var inscope disp <- census_dispatches(cov)
    kernel_coverage_report("vulkan", "seeded kernels", disp)
    let served = census_served(disp)
    vk_census_blind_drain(t, disp, vk_carrier_rows(cov))
    if (whole) {
        // floor calibration: 25 measured on the default 5-model zoo
        // (MoE rows skip without PARITY_FULL; coopmat-mode variants
        // and mode-gated twins legitimately idle here); 24 catches a
        // gross wiring regression without tripping on one
        // near-boundary row
        t |> success(served >= 24,
            "the serving census dispatched {served} distinct kernels (rail floor 24)")
    } else {
        to_log(LOG_INFO, "census floor not asserted (the run was not whole): {served} served\n")
    }
}
