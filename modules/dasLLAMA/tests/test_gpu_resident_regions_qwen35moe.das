options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common   // Model, get_mtp_spec / set_mtp_spec
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active
require dasllama/dasllama_core   // set_gpu_ctx_max: the peel's mirror context cap
require daslib/fio
require _resident_regions
require _resident_feed   // greedy_chain_cell, recurrent_peel_cell
require _moe_resident   // MOE_LOGIT_BAR_REL: the routed chain's bar

// Qwen3.6-35B-A3B-MTP-IQ4_XS-pure (large tier, DASLLAMA_PARITY_FULL=1) on the whole-model driver: the deltanet hybrid MoE
// with a routed NextN head through the forms the rig holds on the small hybrids - a prefill from a position past zero on
// the window chain over the region's live recurrent state (the prefill counter, every call served), ten sessions a
// region each through batched steps the N-row command takes in groups past eight rows (the N-row counter) - on the
// N-column GEMVs within the wide bar, then on the CUDA home's rows tensor leaf nearer the session alone than the row one
// off (a weight arena past 2^32 words: the leaf's plane reads reach past a 32-bit word index) - the greedy
// chain forced on and adaptive against the plain walk bit for bit
// (the chain's hits and misses), and a short tail peeled through the
// token commands (the peel counter) with speculation off. The routed
// rows hold the routed chain's bar: the window's expert
// tiles schedule other row counts than the one-call form's, and the router's near-ties part the two. Skips off the JIT,
// without DASLLAMA_GPU=1 on a device the driver serves, or without the file under models_dir().
// Under DASLLAMA_DEVICE_POOL=0 the pool cell holds the off rail. The leaf cell skips on a home with no rows tensor leaf (every
// home but CUDA).
let private REGIONS_FILE = "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf"
let private WIDE_ROWS = 10l   //! past the N-row command's eight rows and its GEMVs' eight-column tile

[test(description = "On the Qwen3.6-35B MTP hybrid MoE a prefill continued past zero runs the window chain from the region's live recurrent state within the routed bar of the one-call prefill, every call served")]
def test_prefill_continues_qwen35moe(t : T?) {
    cell_prefill_continues_on_the_device(t, REGIONS_FILE, MOE_LOGIT_BAR_REL)
}

[test(description = "Ten Qwen3.6-35B hybrid MoE sessions, past the N-row command's eight rows, step in command groups with column-tiled GEMVs (the CUDA home's rows leaf off), every row within the wide bar of its session alone")]
def test_regions_batched_groups_qwen35moe(t : T?) {
    cell_regions_batched_groups(t, REGIONS_FILE, WIDE_ROWS, LogitsBar.wide_bar)
}

[test(description = "Ten Qwen3.6-35B hybrid MoE sessions step on the CUDA home's rows tensor leaf, a leaf stamp launched, every row nearer its session alone than its previous step")]
def test_regions_batched_leaf_qwen35moe(t : T?) {
    cell_regions_batched_leaf(t, REGIONS_FILE, WIDE_ROWS)
}

[test(description = "On the Qwen3.6-35B hybrid MoE a device-home scheduler's openings and checkpoints outlive their regions in the host pool, later requests take them up as the host prefix cache does")]
def test_device_pool_qwen35moe(t : T?) {
    cell_device_pool(t, REGIONS_FILE)
}

[test(description = "With speculation off, the greedy chain on the Qwen3.6-35B hybrid MoE, forced on and adaptive, reads the plain walk's tokens and logits bit for bit, every on-pick step chained")]
def test_greedy_chain_qwen35moe(t : T?) {
    t |> run("the greedy chain forced on reads the plain walk's tokens and logits bit for bit, every on-pick step chained") @(t : T?) {
        let spec0 = get_mtp_spec()
        set_mtp_spec(false)
        greedy_chain_cell(t, path_join(models_dir(), REGIONS_FILE))
        set_mtp_spec(spec0)
    }
}

[test(description = "With speculation off, a short tail on the Qwen3.6-35B hybrid MoE peels through the token commands, recurrent rows in order and the routed block every row, within the routed bar of one-row steps")]
def test_peel_qwen35moe(t : T?) {
    t |> run("a short tail peels through the token commands, the recurrent rows in order in the region's slot, the routed block every row, and reads what the one-row steps read") @(t : T?) {
        let path = path_join(models_dir(), REGIONS_FILE)
        if (!resident_ready(t, path)) return
        let spec0 = get_mtp_spec()
        set_mtp_spec(false)
        let levers = resident_levers()
        set_gpu_ctx_max(REGIONS_RIG_CTX)   //! the rig's mirror context for the load; the levers go back after
        var tr <- load_resident(path, 1l)
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the routed hybrid (the load log names any decline)")
        if (moe_gpu_resident_active()) {
            with_dasllama_jobque_() {
                recurrent_peel_cell(t, tr, MOE_LOGIT_BAR_REL)
            }
        }
        delete tr
        restore_resident_levers(levers)
        set_mtp_spec(spec0)
    }
}
