options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the umbrella's [init] registrations arm the llama arch and the CUDA home
require dasllama/dasllama
require daslib/jobque_boost
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_cuda_common
require _model_tier
require _resident_feed
require _compares

// The CUDA home's N-row token command (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command` on this home): sessions a
// mirror region each, their batched steps through one command - every GEMV an N-column dispatch, every row kernel a row
// a workgroup, each row at its own position and region through its token block - against each session stepped alone on
// the one-row command: every row's logits a step within the split bar of the session alone (the rows take the row
// kernels where the one-row command takes the GEMV epilogues), the row one step off the control, the N-row counter up by
// every step (the row-at-a-time loop moves it by nothing) and its row counter up by every row of every step, every call
// served. Two rows at two regions on the Q8_0 file (the q8 two-column stamp launched) and on the Q4_K_M file (the
// K-quant N-column leaves and the Q8_K feeds), and twelve rows at twelve regions on the Q8_0 file: one command of twelve
// rows a step, its GEMVs in a tile of eight columns and one of four (the q8 eight- and four-column stamps launched).
// Every cell skips off the JIT, without DASLLAMA_CUDA=1, without DASLLAMA_GPU=1 on a device the driver serves, without
// the file under models_dir(), or where the driver declines the model at the cell's region count.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // declared for the tier's guard: the alone arm prefills on device
}

let private FILE_1B = "Llama-3.2-1B-Instruct-Q8_0.gguf"
let private FILE_1B_K = "Llama-3.2-1B-Instruct-Q4_K_M.gguf"
let private STEPS = 6l
let private PROMPT_A = 40l
let private PROMPT_B = 57l
let private RIG_CTX = 2048l   //! the mirror's context a region: twelve regions of the 1B's f16 rows fit the card beside the planes

//! `rows` sessions over prompts of their own lengths (every prompt distinct), batched `STEPS` steps through the N-row command
//! against each alone, on `file` loaded at `rows` regions; every N-column stamp of `col_stamps` launched by the batched steps
def private batched_rows_cell(t : T?; file : string; rows : int64; col_stamps : array<string>) {
    let path = cuda_model_path(t, file)
    if (empty(path)) return
    let levers = resident_levers()
    set_gpu_ctx_max(RIG_CTX)
    var tr <- load_resident(path, rows)
    restore_resident_levers(levers)
    if (!moe_gpu_resident_active()) {
        t |> skip("the whole-model driver declined the model at {rows} regions (the load log names why)")
        delete tr
        return
    }
    t |> equal(rdec_token_n_rows(), RDEC_NROW_MAX, "the N-row command steps {RDEC_NROW_MAX} rows at once on the armed driver")
    with_dasllama_jobque_() {
        var prompts, feeds : array<array<int64>>
        prompts |> reserve(rows)
        feeds |> reserve(rows)
        for (i in range64(rows)) {
            prompts |> emplace <| resident_prompt(i % 2l == 0l ? PROMPT_A + i : PROMPT_B + i, i * 7l + 1l)
            feeds |> emplace <| resident_feed(STEPS)
        }
        let steps_alone0 = rdec_nrow_steps()
        var alone : array<array<array<float>>>
        alone |> reserve(rows)
        for (i in range64(rows)) {
            alone |> emplace <| feed_alone(tr, mirror_kv_dtype(), prompts[i], feeds[i])
        }
        t |> equal(rdec_nrow_steps() - steps_alone0, 0l, "the sessions alone stepped through the one-row command: the N-row counter's control")
        t |> success(logits_maxabs(alone[0][0]) < INFINITE_DIFF && logits_maxabs(alone[1][0]) < INFINITE_DIFF, "the first two prompts' logits are finite")
        t |> success(logits_maxdiff(alone[0][0], alone[1][0]) > 0.0, "the prompts read different logits, so a crossed region cannot pass")
        let rows0 = rdec_nrow_rows()
        let steps0 = rdec_nrow_steps()
        var stamps0 <- [for (name in col_stamps); cud_kernel_coverage_of(name)]
        var got <- batched_logits_rows(t, tr, prompts, feeds, mirror_kv_dtype(), STEPS) <| $ => rdec_nrow_steps()
        let commands = rdec_nrow_steps() - steps0
        t |> equal(rdec_nrow_rows() - rows0, rows * commands, "every N-row command carried all {rows} rows ({rdec_nrow_rows() - rows0} rows over {commands} commands)")
        for (name, n0 in col_stamps, stamps0) {
            t |> success(cud_kernel_coverage_of(name) > n0, "the batched steps launched the N-column stamp {name} ({cud_kernel_coverage_of(name) - n0} launches)")
        }
        var worst = 0.0
        var tight = 1.0e30
        for (st in range64(STEPS)) {
            for (i in range64(rows)) {
                let finite = (logits_maxabs(alone[i][st + 1l]) < INFINITE_DIFF && logits_maxabs(alone[i][st]) < INFINITE_DIFF
                    && logits_maxabs(got[st * rows + i]) < INFINITE_DIFF)
                t |> success(finite, "batched step {st}, row {i}: the batched row, the session's row and its previous step are finite")
                let bar = SPLIT_BAR_REL * logits_maxabs(alone[i][st + 1l])
                let diff = logits_maxdiff(got[st * rows + i], alone[i][st + 1l])
                let off = logits_maxdiff(got[st * rows + i], alone[i][st])
                to_log(LOG_INFO, "{file} batched step {st}, row {i}: maxdiff {diff}, bar {bar}, the previous step {off} off\n")
                t |> success(diff <= bar, "batched step {st}, row {i}: within the split bar of the session alone (maxdiff {diff}, bar {bar})")
                t |> success(off > bar, "batched step {st}, row {i}: the session's previous step lands outside the bar (the control, {off})")
                worst = max(worst, diff / bar)
                tight = min(tight, off / bar)
            }
        }
        for (i in range64(rows)) {
            var inscope got_i, want_i : array<array<float>>
            for (st in range64(STEPS)) {
                got_i |> push_clone(got[st * rows + i])
                want_i |> push_clone(alone[i][st + 1l])
            }
            log_argmax_rows(tr, "{file} batched row {i}: the session alone, then the batched rows", got_i, want_i)
        }
        to_log(LOG_INFO, "{file} at {rows} rows: the widest miss {worst} of the bar, the tightest control {tight} bars\n")
        delete stamps0
        served_every_call(t)
        delete got
        delete alone
        delete prompts
        delete feeds
    }
    delete tr
}

[test]
def test_gpu_resident_cuda_batch_two_rows(t : T?) {
    t |> run("{FILE_1B}: two sessions at two regions, six batched steps through the N-row command against each alone") @(t : T?) {
        batched_rows_cell(t, FILE_1B, 2l, ["q8_gemv_n2_cls_cuda"])
    }
}

[test]
def test_gpu_resident_cuda_batch_two_rows_k(t : T?) {
    t |> run("{FILE_1B_K}: the same on the K-quant planes (the kq N-column leaves, the Q8_K feeds)") @(t : T?) {
        var inscope no_stamps : array<string>
        batched_rows_cell(t, FILE_1B_K, 2l, no_stamps)
    }
}

[test]
def test_gpu_resident_cuda_batch_twelve_rows(t : T?) {
    t |> run("{FILE_1B}: twelve sessions at twelve regions, one command of twelve rows a step (a tile of eight columns and one of four)") @(t : T?) {
        batched_rows_cell(t, FILE_1B, 12l, ["q8_gemv_n8_cls_cuda", "q8_gemv_n4_cls_cuda"])
    }
}
