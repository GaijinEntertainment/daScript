options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade - engine + arch registrations
require dasllama/dasllama_common   // set_metal_mode, allow_cpu_prefill, generate, generate_mtp_greedy, the overrides
require dasllama/dasllama_dflash   // the subject: the DFlash2 block drafter's loader, seams and CPU arm
require dasllama/dasllama_sampling   // generate_, generate_mtp_sampled, set_seed_: the sampled walks
require dasllama/dasllama_gguf   // gguf_header_read, gguf_find_tensor: the drafter file's disk types against the loader's tags; gguf_put_le, gguf_put_str: the synthesized header's words
require dasllama/dasllama_math   // with_dasllama_jobque
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - the Metal decode driver, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_dflash   // nolint:STYLE030,LINT019 - the drafter's device arm registers at init; its site counters
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - the Metal decode driver registers at init
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_rows8_on
require ?das_metal metal/das_metal_boost   // nolint:STYLE030,LINT019 - the system device the rows8 layout probe runs on
require ?das_metal _metal_kernel_common   // nolint:STYLE030,LINT019 - with_metal_cell: the probe's planes
require ?das_metal _rows8_probe   // nolint:STYLE030,LINT019 - rows8_probe_ok: the rows8 witnesses' layout fact
require dasllama/dasllama_layout   // convert_model_to_metal_blob - the device arm's trunk off the image rail
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 - g_env_metal.rows_mt, inside the das_metal half
require daslib/fio
require strings   // find: the refusal text's words
require _gguf_writer   // gguf_header, gguf_tensor_info, gguf_pad_data: the synthesized drafter header
require _model_tier   // _model_tier::models_dir() + model_available() presence/size-tier gate
require _scratch_dir   // scratch_path / scratch_file: the refusal fixtures' files
require _dflash_cells   // the walks every trunk runs: lossless, sampled, the planes' tags, the discrete legs
require _mtp_cpu_cells   // invariance_ok, forced_invariance_ok, MTP_PROMPT: the NextN head's invariance pair on the large -MTP- carriers
require _compares   // first_mismatch: the facade streams' ids
require _metal_blob_twin   // metal_kernel_count: the tile attention's dispatches
require _scheduler_rig   // make_req, drain, toks_of
require _resident_feed   // resident_trunk, load_resident_at: the discrete homes' trunk
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms: the 27B trunk's block
require _parity_cells   // the parity cells the block's parity knob reaches (`_::`): the 27B trunk's model_specs parity row on its block
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_verifies: the discrete walks' verifies on the resident driver
require ?vulkan dasllama/dasllama_vulkan_dflash   // nolint:STYLE030,LINT019 - the Vulkan arm registers at init
require ?cuda dasllama/dasllama_cuda_dflash   // nolint:STYLE030,LINT019 - the CUDA arm registers at init
require _kernel_census   // nolint:STYLE030,LINT019 - kernel_census_reset_metal, inside the das_metal halves
require _metal_mtp_cells   // the Metal MTP rail's arms on the 27B trunks' served images
require _mlx_cells   // the 27B snapshot's own cells against mlx-lm, on its snapshot block

// The DFlash2 block drafter (`dasllama/dasllama_dflash`): the loader's refusal contract, and the lossless gate every arm of
// the pass is held to - a greedy speculative walk's ids equal the plain walk's, with drafts accepted along the way - on the
// CPU arm over the planar Qwen3.8-27B (the `qwen35-27b` category) and on the Metal home's device arm over the served
// trunk, where the CPU arm fed by the device's captures accepts the same count. The drafter sidecars are the 27B's own
// DFlash2 Q8_0 file and its Q4_K_M file (Q4_K planes, Q6_K at some layers' v and down) under `drafters/`; the regular DFlash
// drafter runs on the routed gemma-4-26B and, on the Metal home, on the dense Qwen3-4B (its Q8_0 file), whose device arm is a
// counted skip: its device pass does not reproduce the CPU arm's drafts. The 27B trunk's CPU cells and its discrete-home legs
// ride one load a form (`test_dflash_27b_trunk`); the routed 26B's CPU and discrete-home cells ride that trunk's block in
// `test_gpu_resident_gemma4_moe.das`; the regular DFlash drafter's Metal arms on the routed 26B and the dense Qwen3-4B ride
// the Metal form of those trunks' blocks (`metal_drafter_cell`, `_dflash_cells.das`), the 26B's prompt-capture rows its
// CPU and Metal forms (`test_mtp_gemma_drafter.das`); the 27B GGUF trunk's Metal cells ride its block's Metal form, the blob
// twin under the required mode, beside its served images for the batch rail's verify
// and, with the split NextN head, the MTP rail's arms (`_metal_mtp_cells.das`); the 27B
// snapshot trunk's Metal cells ride its snapshot block's blob twin (the same model's affine
// 4-bit snapshot: mlx4b planes, no NextN head); the E2B facade's Metal cells ride the E2B
// block's required blob twin (`dflash_e2b_facade_cell`, `_dflash_cells.das`). A cell skips
// without its files (the trunk, the drafters, the 27B snapshot), on a model file whose category the run does not name
// (`../TEST_CATEGORIES.md`), on the 27B snapshot without DASLLAMA_PARITY_FULL=1, and
// on the Metal device arms on a build without das_metal; the discrete-home cells skip on a build with no discrete GPU home,
// with no armed GPU tier (`gpu_tier_armed`), where the residency plan does not fit the card, and when interpreted.
// The NextN head's lossless gate on the large -MTP- carriers sits beside them:
// self-speculative greedy decode token for token against plain decode on the
// 64-trunk-layer Qwen3.6-27B-MTP, on its block's CPU form beside its served image's MTP
// rail arms, and on the non-recurrent GLM-4.5-Air (forced rejects included), the first
// the `qwen35-27b` category's and the second the `glm45-air` category's, the invariance
// pair `_mtp_cpu_cells.das`'s.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU arm prefills on the CPU by design
}

let private DRAFTER_FILE = "drafters/Qwen3.8-27B-DFlash2-GGUF/Qwen3.8-27B-DFlash2-Q8_0.gguf"
let private DRAFTER_K4_FILE = "drafters/Qwen3.8-27B-DFlash2-GGUF/Qwen3.8-27B-DFlash2-Q4_K_M.gguf"
let private DRAFTER_NV4_FILE = "drafters/Qwen3.8-27B-DFlash2-NVFP4-GGUF/Qwen3.8-27B-DFlash2-NVFP4.gguf"   //! every weight plane NVFP4, no `.scale` beside any

let private GGUF_I32 = 5ul   //! the GGUF value types the synthesized header writes beside `_gguf_writer.das`'s
let private GGUF_ARRAY = 9ul

//! a one-layer `dflash` file 256 wide whose fc.weight is of disk type `fc_type`, its selector codebook Q8_0, and no other
//! weight tensor: the loader reads the header, then fc.weight is the first plane it plans; `fc_scale` past 0 adds the
//! tensor's `.scale` companion (one f32)
def private synth_dflash(fc_type : int; without : string = ""; fc_scale : float = 0.0) : array<uint8> {
    let d = 256ul
    var b : array<uint8>
    gguf_header(b, fc_scale != 0.0 ? 3ul : 2ul, empty(without) ? 17ul : 16ul)
    gguf_kv_str(b, "general.architecture", "dflash")
    for (kv in [("block_count", 1ul), ("embedding_length", d), ("feed_forward_length", d), ("attention.head_count", 2ul),
                ("attention.head_count_kv", 1ul), ("attention.key_length", 128ul), ("block_size", 8ul), ("conv_kernel_size", 2ul),
                ("conv_group_size", 64ul), ("selector_rank", d), ("selector_top_k", 4ul), ("attention.sliding_window", 2048ul)]) {
        if (kv._0 != without) {
            gguf_kv_u32(b, "dflash.{kv._0}", kv._1)
        }
    }
    for (kv in [("attention.layer_norm_rms_epsilon", 0x358637BDul), ("rope.freq_base", 0x49742400ul)]) {   //! 1e-6 and 1e6 as f32 bits
        gguf_kv_f32_bits(b, "dflash.{kv._0}", kv._1)
    }
    gguf_put_str(b, "dflash.target_layers")
    gguf_put_le(b, GGUF_ARRAY, 4)
    gguf_put_le(b, GGUF_I32, 4)
    gguf_put_le(b, 1ul, 8)
    gguf_put_le(b, 2ul, 4)
    gguf_kv_u32(b, "tokenizer.ggml.mask_token_id", 3ul)
    let fc_bytes = ggml_type_bytes(fc_type, int64(d * d))
    let fc_span = (fc_bytes + 31l) & ~31l
    gguf_tensor_info(b, "fc.weight", [d, d], fc_type, 0ul)
    gguf_tensor_info(b, "selector_predecessor.weight", [d, 4ul], GGML_TYPE_Q8_0, uint64(fc_span))
    let sel_span = (ggml_type_bytes(GGML_TYPE_Q8_0, int64(d) * 4l) + 31l) & ~31l
    if (fc_scale != 0.0) {
        gguf_tensor_info(b, "fc.scale", [1ul], GGML_TYPE_F32, uint64(fc_span + sel_span))
    }
    gguf_pad_data(b)
    let data0 = long_length(b)
    b |> resize(data0 + fc_span + sel_span + 32l)
    if (fc_scale != 0.0) {
        *unsafe(addr<float?>(b[data0 + fc_span + sel_span])) = fc_scale
    }
    return <- b
}

//! a synthesized drafter's bytes written under `name` in the scratch folder and loaded
def private load_synth_dflash(bytes : array<uint8>; name : string) : DflashDrafter {
    let p = path_join(scratch_path("."), name)
    fopen(p, "wb") $(f) {
        fwrite(f, bytes)
    }
    return <- load_dflash_drafter(p)
}

[test(description = "Off the matrix-unit form the Metal verify seat takes a block drafter's round with no depth set at three drafts and their seed, and the round's widest rows on the form, under a set depth, and for a trunk with no block drafter")]
def test_dflash_metal_seat_rows(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> equal(metal_verify_rows_for(true, false, false), METAL_ROWS_DRAFTS + 1l, "off the form a block drafter's round verifies the row forms' drafts and their seed")
        t |> equal(METAL_ROWS_DRAFTS, 3l, "... three drafts")
        t |> equal(metal_verify_rows_for(true, true, false), MTP_MAX_ROWS, "control: on the matrix-unit form the round keeps its widest rows")
        t |> equal(metal_verify_rows_for(true, false, true), MTP_MAX_ROWS, "control: a set depth takes its own rows")
        t |> equal(metal_verify_rows_for(false, false, false), MTP_MAX_ROWS, "control: a round with no block drafter keeps its rows")
    } else {
        t |> skip("the Metal verify seat's rows: no Metal home in this build")
    }
}

[test(description = "`load_dflash_drafter` refuses a missing path, a non-GGUF file, a synthesized drafter with an IQ4_XS fc.weight (naming tensor, disk type and member) and one with a `.scale` beside a Q4_K fc.weight, each with a reason and no weights, while a Q4_K fc.weight passes tagged k4 with no scale (control) and an NVFP4 one passes tagged nv4 carrying the `.scale` beside it, or none where the file has none")]
def test_dflash_refusals(t : T?) {
    if (!arm_on(t, "dflash-refuse")) {
        return
    }
    t |> run("a missing path refuses with a reason instead of panicking") @(t : T?) {
        var dr <- load_dflash_drafter(path_join(scratch_path("."), "no-such-drafter.gguf"))
        t |> success(!dr.ok, "ok is false")
        t |> success(!empty(dr.why), "why names the refusal: '{dr.why}'")
        t |> success(empty(dr.blob) && empty(dr.fblob), "a refused load carries no weights")
        delete dr
    }
    t |> run("a file with no GGUF magic refuses with a reason") @(t : T?) {
        let p = scratch_file("junk_dflash.gguf", "not a gguf file at all, but long enough to pass the length gate")
        var dr <- load_dflash_drafter(p)
        t |> success(!dr.ok, "ok is false")
        t |> success(!empty(dr.why), "why names the refusal: '{dr.why}'")
        delete dr
    }
    t |> run("a plane of a format no arm serves refuses naming the tensor, its type and its member") @(t : T?) {
        var inscope bytes <- synth_dflash(GGML_TYPE_IQ4_XS)
        var dr <- load_synth_dflash(bytes, "iq4xs_dflash.gguf")
        t |> success(!dr.ok, "ok is false")
        t |> success(find(dr.why, "'fc.weight'") >= 0 && find(dr.why, "IQ4_XS") >= 0 && find(dr.why, "iq4xs") >= 0,
            "why names the site, the disk type and the member: '{dr.why}'")
        delete dr
    }
    t |> run("a file with three of the four DFlash2 keys refuses naming the count") @(t : T?) {
        var inscope bytes <- synth_dflash(GGML_TYPE_Q4_K, "selector_top_k")
        var dr <- load_synth_dflash(bytes, "three_key_dflash.gguf")
        t |> success(!dr.ok, "ok is false")
        t |> success(find(dr.why, "this one carries 3") >= 0, "why names the keys it carries: '{dr.why}'")
        delete dr
    }
    t |> run("the control: the same file with a Q4_K fc.weight loads that plane and refuses on the next, absent one") @(t : T?) {
        var inscope bytes <- synth_dflash(GGML_TYPE_Q4_K)
        var dr <- load_synth_dflash(bytes, "k4_dflash.gguf")
        t |> success(!dr.ok, "ok is false")
        t |> equal(dr.why, "missing tensor 'selector_hidden.weight'", "fc.weight passed the format check")
        t |> equal(dr.fc.fmt, KqFmt.k4, "fc.weight is tagged k4, its disk type")
        t |> equal(dr.fc.scale, 0.0, "a plane with no `.scale` beside it carries none")
        delete dr
    }
    t |> run("a `.scale` beside a plane of any format but NVFP4 refuses naming the tensor and its type") @(t : T?) {
        var inscope bytes <- synth_dflash(GGML_TYPE_Q4_K, "", 3.0)
        var dr <- load_synth_dflash(bytes, "k4_scaled_dflash.gguf")
        t |> success(!dr.ok, "ok is false")
        t |> success(find(dr.why, "'fc.weight'") >= 0 && find(dr.why, "per-tensor scale") >= 0 && find(dr.why, "Q4_K") >= 0,
            "why names the site, the scale and the disk type: '{dr.why}'")
        delete dr
    }
    t |> run("an NVFP4 fc.weight loads tagged nv4 with its `.scale` read, and with none beside it carries none") @(t : T?) {
        for (scale in fixed_array(3.0, 0.0)) {
            var inscope bytes <- synth_dflash(GGML_TYPE_NVFP4, "", scale)
            var dr <- load_synth_dflash(bytes, "nv4_dflash.gguf")
            t |> equal(dr.why, "missing tensor 'selector_hidden.weight'", "fc.weight passed the format check and the loader went on to the next plane")
            t |> equal(dr.fc.fmt, KqFmt.nv4, "fc.weight is tagged nv4, its disk type")
            t |> equal(dr.fc.scale, scale, "the plane carries the `.scale` the file holds ({scale})")
            delete dr
            }
    }
}

let private SHALLOW_DEPTH = 3l   //! the asked round's drafts, under the block's seven

//! the device arm's greedy walk round by round at `depth` (0 = no depth set, the drafter's whole block): every drafting round
//! drafts `depth` (unset: past SHALLOW_DEPTH) and its verify runs one row more (`metal_verify_split`), the ids the plain walk's
def private depth_walk(t : T?; m : Model; drafter : string; depth : int64; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let device0 = dflash_device_knob()
        set_dflash_device(true)
        let why = attach_dflash_drafter(m, path_join(_model_tier::models_dir(), drafter))
        set_dflash_device(device0)
        t |> equal(why, "", "{tag}: the drafter attaches")
        return if (!empty(why))
        let pin = mtp_depth_pin()
        if (depth > 0l) {
            set_mtp_depth(depth)
        } else {
            clear_mtp_depth()
        }
        let prompt <- encode(m, PROMPT, true)
        let np = long_length(prompt)
        let total = np - 1l + STEPS
        var s1 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        let plain <- generate(m, s1, prompt, int(total))
        var s2 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        let spec0 = get_mtp_spec()
        set_mtp_spec(true)
        var ids <- [for (i in range64(1l, np)); prompt[i]]
        ids |> reserve(total)
        forward_prefill(m, s2, prompt, np, 0l)
        s2.n_past = np
        let vocab = m.config.vocab_size
        var tok = parallel_argmax(s2.logits, vocab)
        var rounds = 0l
        var off = 0l
        while (long_length(ids) < total) {
            ids |> push(tok)
            break if (long_length(ids) >= total)
            let d0 = s2.mtp_drafted
            let a = mtp_spec_round(m, s2, tok)
            let drafted = s2.mtp_drafted - d0
            if (drafted > 0l) {
                rounds++
                let rows = metal_verify_split().rows
                let deep_ok = depth > 0l ? drafted == depth : drafted > SHALLOW_DEPTH
                if (!deep_ok || rows != drafted + 1l) {
                    off++
                    to_log(LOG_WARNING, "{tag}: a round drafted {drafted} and verified {rows} rows\n")
                }
            }
            for (j in range64(a)) {
                break if (long_length(ids) >= total)
                ids |> push(s2.mtp_vbatch[j + 1l])
            }
            break if (long_length(ids) >= total)
            tok = parallel_argmax(s2.logits, vocab)
        }
        set_mtp_spec(spec0)
        restore_mtp_depth(pin)
        detach_dflash_drafter()
        let asked = depth > 0l ? "{depth}" : "the whole block"
        t |> success(rounds > 0l && off == 0l, "{tag}: {rounds - off} of {rounds} drafting rounds drafted {asked} and verified one row more")
        var same = 0l
        while (same < long_length(plain) && same < long_length(ids) && plain[same] == ids[same]) {
            same++
        }
        t |> equal(same, long_length(plain), "{tag}: the walk == plain token for token (plain '{decode(m, plain)}', the walk '{decode(m, ids)}')")
        t |> equal(length(ids), length(plain), "{tag}: the walks are the same length")
        delete s1
        delete s2
    }
}

//! the rows8 form's layout probe, run by this test on the system device: false where the box's cooperative layout is another
//! generation's or no Metal-4 pipeline compiles (false off the Metal build)
def private rows8_layout_probe(t : T?) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        return false if (dev == null)
        var queue = metal_new_command_queue(dev)
        var ok = false
        with_metal_cell(dev, queue) $(var mc : MetalCell) {
            var err : string
            ok = rows8_probe_ok(t, mc, err)
        }
        metal_release(queue)
        metal_release(dev)
        return ok
    } else {
        return false
    }
}

//! the device arm's weight sites since the attach: every format in `fmts` among the pass's sites, and where the box runs the rows8
//! form - this test's layout probe passes and DASLLAMA_METAL_ROWS_MT is on, the engine's rows8 gate held to the same answer - among
//! its sites; and every format in `trunk_fmts` among the trunk's rows8 sites
def private site_witnesses(t : T?; fmts, trunk_fmts : array<KqFmt>; tag : string; rows8 : bool = true) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        for (f in fmts) {
            t |> success(metal_dflash_sites(f) > 0l, "{tag}: the device arm encoded {metal_dflash_sites(f)} sites over {f} planes")
        }
        let probe = rows8_layout_probe(t)
        let knob = g_env_metal.rows_mt
        t |> equal(metal_rows8_on(), probe && knob,
            "{tag}: the engine's rows8 gate is the layout probe's answer ({probe}) under DASLLAMA_METAL_ROWS_MT={knob}")
        return if (!probe || !knob || !rows8)   //! `rows8` off: the drafter's formats have no rows8 member
        t |> success(metal_dflash_rows8_sites() > 0l, "{tag}: the rows8 form served {metal_dflash_rows8_sites()} of the device arm's sites")
        for (f in fmts) {
            t |> success(metal_dflash_rows8_sites_of(f) > 0l, "{tag}: the rows8 form served {metal_dflash_rows8_sites_of(f)} sites over {f} planes")
        }
        for (f in trunk_fmts) {
            t |> success(metal_rows8_sites_of(f) > 0l, "{tag}: the rows8 form served {metal_rows8_sites_of(f)} of the trunk's verify sites over {f} planes")
        }
    }
}

//! the drafter's record knob and the Metal drafter's select, accept and replay knobs
struct private MetalDflashKnobs {
    record : bool
    select : bool
    accept : bool
    tape : bool
    unwaited : bool
}

//! every knob as its getter reads it (the Metal ones false off the Metal build)
def private metal_dflash_knobs() : MetalDflashKnobs {
    var k = MetalDflashKnobs(record = dflash_record_on())
    static_if (typeinfo builtin_module_exists(das_metal)) {
        k.select = get_metal_dflash_select()
        k.accept = get_metal_dflash_accept()
        k.tape = get_metal_replay_tape()
        k.unwaited = get_metal_replay_unwaited()
    }
    return k
}

//! every knob of `k` written through its setter
def private set_metal_dflash_knobs(k : MetalDflashKnobs) {
    set_dflash_record(k.record)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_dflash_select(k.select)
        set_metal_dflash_accept(k.accept)
        set_metal_replay_tape(k.tape)
        set_metal_replay_unwaited(k.unwaited)
    }
}

//! the lossless gate on the Metal home's device arm over the served `trunk` with the drafter file `drafter`, every format in
//! `fmts` counted among the pass's sites and the rows8 form serving where the box runs it - and every format in `trunk_fmts`
//! among the trunk's rows8 sites -, every pass's selector on the device and held for the verify, every held verify's accept on
//! the device, every reject's replay unwaited over the tape in place; the same walk with the host's accept over every landed
//! row and every replay waited, then with the host's selector over the device pass and the tape copied back,
//! each drafting the same ids and accepting the same count; then the CPU arm fed by the device's captures accepting the same count
def private metal_lossless_legs(t : T?; var m : Model; drafter : string; fmts, trunk_fmts : array<KqFmt>;
                                tag : string; rows8 : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let knobs0 = metal_dflash_knobs()
        set_metal_dflash_knobs(MetalDflashKnobs(record = true, select = true, accept = true, tape = true, unwaited = true))
        on_metal_trunk() {
            let rp0 = metal_verify_replays()
            let tape0 = metal_verify_replays_in_place()
            let dev = metal_lossless_walk(t, m, drafter, true, "{tag} device arm")
            var inscope dev_drafts <- dflash_recorded_drafts()
            let rp1 = metal_verify_replays()
            let tape1 = metal_verify_replays_in_place()
            t |> success(rp1.total > rp0.total && rp1.unwaited - rp0.unwaited == rp1.total - rp0.total,
                "{tag}: every rejected round's recurrent replay committed unwaited ({rp1.unwaited - rp0.unwaited} of {rp1.total - rp0.total})")
            t |> equal(tape1 - tape0, rp1.total - rp0.total, "{tag}: every replay read the verify's tape in place")
            let sel = metal_dflash_selected()
            t |> success(sel.held > 0l && sel.held == sel.selected, "{tag}: the device selector ended {sel.selected} passes, {sel.held} of them held for the verify unwaited")
            t |> equal(metal_dflash_accepted(), sel.held, "{tag}: every held pass's verify landed its accept on the device")
            site_witnesses(t, fmts, trunk_fmts, tag, rows8)
            detach_dflash_drafter()
            set_metal_dflash_accept(false)
            set_metal_replay_unwaited(false)
            set_dflash_record(true)
            let host_acc = metal_lossless_walk(t, m, drafter, true, "{tag} device arm, the host's accept, the replays waited")
            var inscope host_acc_drafts <- dflash_recorded_drafts()
            t |> success(metal_dflash_selected().held > 0l && metal_dflash_accepted() == 0l, "{tag}: the knob off, the held passes' verifies land every row for the host's accept")
            let rp2 = metal_verify_replays()
            t |> success(rp2.total > rp1.total && rp2.unwaited == rp1.unwaited, "{tag}: the replay knob off, every replay waited in the rollback seat ({rp2.total - rp1.total})")
            detach_dflash_drafter()
            set_metal_dflash_accept(true)
            set_metal_replay_unwaited(true)
            t |> equal(host_acc, dev, "{tag}: the host's accept over every landed row accepts the device accept's count")
            same_drafts(t, m, dev_drafts, host_acc_drafts, "{tag}: the device accept's rounds draft the host accept's")
            set_metal_dflash_select(false)
            set_metal_replay_tape(false)
            set_dflash_record(true)
            let tape2 = metal_verify_replays_in_place()
            let rp3 = metal_verify_replays()
            let host_sel = metal_lossless_walk(t, m, drafter, true, "{tag} device arm, the host's selector, the tape copied back")
            var inscope host_drafts <- dflash_recorded_drafts()
            t |> equal(metal_dflash_selected().selected, 0l, "{tag}: the knob off, no pass ends on the device selector")
            t |> success(metal_verify_replays().total > rp3.total && metal_verify_replays_in_place() == tape2,
                "{tag}: the tape knob off, every replay copied its tape back into the stage planes")
            detach_dflash_drafter()
            set_metal_dflash_select(true)
            set_metal_replay_tape(true)
            set_dflash_record(false)
            t |> equal(host_sel, dev, "{tag}: the host's selector over the device pass accepts the device selector's count")
            same_drafts(t, m, dev_drafts, host_drafts, "{tag}: the device selector's drafts are the host selector's")
            let cpu = metal_lossless_walk(t, m, drafter, false, "{tag}, the CPU arm on the device's captures")
            detach_dflash_drafter()
            t |> equal(dev, cpu, "{tag}: both arms accept the same count")
            depth_walk(t, m, drafter, SHALLOW_DEPTH, "{tag} device arm at depth {SHALLOW_DEPTH}")
            depth_walk(t, m, drafter, 0l, "{tag} device arm, no depth set")
        }
        set_metal_dflash_knobs(knobs0)
    }
}

//! the lossless legs (`metal_lossless_legs`) over the served trunk `m`, the drafter file present
def private lossless_metal_cell(t : T?; var m : Model; drafter : string;
                                fmts, trunk_fmts : array<KqFmt>; tag : string;
                                rows8 : bool = true) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!model_available(t, path_join(_model_tier::models_dir(), drafter)))
        metal_lossless_legs(t, m, drafter, fmts, trunk_fmts, tag, rows8)
    } else {
        t |> skip("{tag}: no das_metal in this build")
    }
}


//! the 27B trunk's CPU cells on the CPU form `m`, each under its arm, the Metal mode off (put back): the sampled acceptances
//! and the lossless walk of each drafter file, the K-quant and NVFP4 files' planes held to their disk types
[cold_path]
def private dflash_27b_cpu_cells(t : T?; var m : Model) {
    with_cpu_drafter_scope(m) {
        unsafe {   //! the cells capture the loaded trunk by reference
            t |> run("On the 27B trunk with the Q8_0 DFlash2 drafter on the CPU arm, the exact-accept sampled speculative walk spends one draw a judged row with drafts accepted and opens on the plain seeded sampled walk's first id, honors a stop id, and both rejection walks accept some drafts and reject some") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "dflash-27b-sampled-cpu") || !model_available(t, path_join(_model_tier::models_dir(), DRAFTER_FILE)))
                sampled_modes_walk(t, m, DRAFTER_FILE, false, "sampled cpu")
            }
            t |> run("On the planar 27B Q4_K_M trunk with the Q8_0 DFlash2 drafter on the CPU arm, the greedy speculative walk emits the plain greedy walk's ids token for token with drafts accepted") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "dflash-27b-cpu"))
                cpu_drafter_lossless(t, m, DRAFTER_FILE, "cpu", false)
            }
            t |> run("The Q4_K_M DFlash2 drafter tags every plane with its disk type's format, and on the CPU arm its greedy speculative walk emits the plain greedy walk's ids token for token with drafts accepted") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "dflash-27b-k4-cpu"))
                cpu_drafter_lossless(t, m, DRAFTER_K4_FILE, "k4 cpu", true)
            }
            t |> run("The NVFP4 DFlash2 drafter tags every plane nv4, and on the CPU arm its greedy speculative walk emits the plain greedy walk's ids token for token with drafts accepted") @capture(& m)(t : T ?) {
                return if (!arm_on(t, "dflash-27b-nv4-cpu"))
                cpu_drafter_lossless(t, m, DRAFTER_NV4_FILE, "nv4 cpu", true)
            }
        }
    }
}

//! the 27B trunk's discrete-home cell on the drafter form `m`: each drafter file's legs on the device arm
def private dflash_27b_discrete_cells(t : T?; m : Model) {
    discrete_drafter_cell(t, m, "On the 27B's resident trunk on the serving discrete home, the Q8_0, Q4_K_M and NVFP4 DFlash2 drafters' greedy walks on the device arm are the plain walk's on the same device (a driver-against-itself compare, every walk's trunk served on the driver and its verifies on the resident driver), every pass ending on the device's top-k and walk, and the host's walk and selector draft the same ids",
        "dflash-27b-discrete", [DRAFTER_FILE, DRAFTER_K4_FILE, DRAFTER_NV4_FILE], "27B discrete")
}

//! the 27B trunk's Metal cells on its Metal form `m` (the blob twin, required), each under its arm and with its drafter file:
//! the sampled acceptances, the lossless gate of each drafter file, the joint streams
[cold_path]
def private dflash_27b_metal_cells(t : T?; var m : Model) {
    unsafe {   //! the cells capture the loaded trunk by reference
        t |> run("On the Metal device arm over the 27B GGUF trunk, the exact-accept sampled walk spends one draw a judged row and equals the plain seeded sampled walk token for token with drafts accepted, honors a stop id, both rejection walks accept some drafts and reject some, drawn drafts end on the device selector, and verify-ahead off or the soft pick off reproduce the same stream") @capture(& m)(t : T ?) {
            let drafter = path_join(_model_tier::models_dir(), DRAFTER_FILE)
            return if (!arm_on(t, "dflash-27b-sampled-metal") || !model_available(t, drafter))
            on_metal_trunk() {
                sampled_modes_walk(t, m, DRAFTER_FILE, device = true, tag = "sampled metal", tokens_hold = true)
            }
        }
        t |> run("On the Metal device arm over the 27B GGUF trunk the greedy DFlash walk is lossless; device accept and selector match the host's drafts and accept count, replays commit unwaited on the tape, the CPU arm accepts the same count, and depth pins hold") @capture(& m)(t : T ?) {
            let drafter = path_join(_model_tier::models_dir(), DRAFTER_FILE)
            return if (!arm_on(t, "dflash-27b-metal") || !model_available(t, drafter))
            metal_lossless_legs(t, m, DRAFTER_FILE, [KqFmt.q8], array<KqFmt>(), "metal", true)
        }
        t |> run("The Q4_K_M DFlash2 drafter tags planes by disk type and its greedy walk on the Metal device arm is lossless, with k4 and k6 sites encoded by the pass and device and host accept and selector agreeing") @capture(& m)(t : T ?) {
            let drafter = path_join(_model_tier::models_dir(), DRAFTER_K4_FILE)
            return if (!arm_on(t, "dflash-27b-k4-metal") || !model_available(t, drafter))
            planes_tag_disk_types(t, drafter, "k4 metal")
            metal_lossless_legs(t, m, DRAFTER_K4_FILE, [KqFmt.k4, KqFmt.k6], array<KqFmt>(),
                "k4 metal", true)
        }
        t |> run("The NVFP4 DFlash2 drafter's greedy walk on the Metal device arm is the plain walk's on the same device (a driver-against-itself compare, every walk's trunk calls counted on the device), with nv4 sites encoded by the pass, the selector walking its NVFP4 codebooks on the device, and device and host accept and selector agreeing") @capture(& m)(t : T ?) {
            let drafter = path_join(_model_tier::models_dir(), DRAFTER_NV4_FILE)
            return if (!arm_on(t, "dflash-27b-nv4-metal") || !model_available(t, drafter))
            planes_tag_disk_types(t, drafter, "nv4 metal")
            metal_lossless_legs(t, m, DRAFTER_NV4_FILE, [KqFmt.nv4], array<KqFmt>(), "nv4 metal",
                false)
        }
        t |> run("On the 27B hybrid's Metal home with its DFlash2 drafter attached, two streams through the scheduler ride the verify seat's joint groups, each the plain scheduler's token for token, together accepting what each accepts alone within a fifth") @capture(& m)(t : T ?) {
            let drafter = path_join(_model_tier::models_dir(), DRAFTER_FILE)
            return if (!arm_on(t, "dflash-27b-joint") || !model_available(t, drafter))
            on_metal_trunk() {
                set_dflash_device(true)
                t |> equal(attach_block_drafter(m, drafter), "", "the drafter attaches")
                joint_streams_leg(t, m)
                detach_block_drafter()
            }
        }
    }
}

let private DFLASH_27B_METAL_ARMS = fixed_array("dflash-27b-sampled-metal", "dflash-27b-metal",
    "dflash-27b-k4-metal",     "dflash-27b-nv4-metal", "dflash-27b-joint")

//! whether a 27B cell's arm is on, no skip registered: a block none of whose arms the filter keeps loads nothing
def private dflash_27b_arms_on() : bool {
    return (any_arm_on(["dflash-27b-sampled-cpu", "dflash-27b-cpu", "dflash-27b-k4-cpu",
        "dflash-27b-nv4-cpu", "dflash-27b-discrete"]) || any_arm_on(DFLASH_27B_METAL_ARMS))
}

[test(description = "`arms_reached` keeps a list of arms when the arm filter is empty or one of its stripped tokens is a substring of one of the names, and drops it when no token reaches any name")]
def test_arms_reached(t : T?) {
    t |> success(arms_reached("", ["a-cpu"]), "an empty filter reaches every list")
    t |> success(arms_reached("x, -metal", fixed_array("a-cpu", "b-metal")), "a token, its comma space stripped, reaches the second name")
    t |> success(!arms_reached("gpu", ["a-cpu", "b-metal"]), "no token reaches any name (the control)")
    t |> success(!arms_reached("cpu", array<string>()), "an empty list under a filter")
}

[test(description = "`on_metal_dflash_trunk` (`on_metal_trunk` with the block drafter) runs its body under the Metal decode override, then puts back the override it found, the block drafter and the decode and prefill drivers down with no Metal object left; a skip on a build without das_metal")]
def test_on_metal_trunk(t : T?) {
    static_if (!typeinfo builtin_module_exists(das_metal)) {
        t |> skip("no das_metal in this build")
    } else {
        let found = active_decode_override()
        select_decode_override("")
        var inside = ""
        on_metal_dflash_trunk() {
            inside = active_decode_override()
        }
        t |> equal(inside, "metal", "the body runs under the Metal decode override")
        t |> equal(active_decode_override(), "", "the override found (none) is back")
        t |> equal(metal_log_leaks_if_any(), 0l, "no Metal object left after the drivers' shutdown")
        select_decode_override(found)
    }
}

[test(description = "On the planar Qwen3.8-27B Q4_K_M trunk on the CPU arm, with the Q8_0 DFlash2 drafter the exact-accept sampled speculative walk spends one draw a judged row with drafts accepted and opens on the plain seeded sampled walk's first id, honors a stop id, and both rejection walks accept some drafts and reject some, and the greedy speculative walks of the Q8_0, Q4_K_M and NVFP4 drafters emit the plain greedy walk's ids token for token with drafts accepted, the K-quant and NVFP4 files' planes tagged with their disk types; on the serving discrete home's drafter form each drafter's greedy walk on the device arm is the plain walk's on the same device, every pass ending on the device's top-k and walk, the host's walk and selector drafting the same ids; on the Metal form, the file's blob twin under the required mode, the exact sampled walk holds the CPU arm's draws and equals the plain seeded sampled walk token for token, the rejection walks accept some drafts and reject some with drawn drafts on the device selector, each drafter file's greedy walk on the device arm is lossless with its formats' sites encoded by the pass, device and host accept and selector agreeing, replays unwaited on the tape and the CPU arm on the device's captures accepting the same count, depth pins hold, and two streams through the scheduler ride the verify seat's joint groups, each the plain scheduler's token for token; on the Metal home's served image, one load on a das_metal build, the batch rail serves a same-slab verify's recurrent deltanet rows as one group, with no decline, the rows within 2.0 of the plain GPU step; on the served image with the Q8_0 NextN head loaded as a split shard beside the trunk, where the head file sits, the Metal MTP rail's verify rows within 0.5 of the plain GPU step and speculative free-run matching plain exactly; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token with its chunked-prefill arm; each form loads once")]
def test_dflash_27b_trunk(t : T?) {
    return if (!family_on(t, "qwen35"))
    var forms <- dflash_27b_forms(t)
    let parity = empty(arm_why("dflash-27b-cpu"))
    if (empty(forms) && !parity) {
        delete forms
        return
    }
    with_cpu_and_gpu_model(t, "llm", "Qwen3.8-27B-Q4_K_M.gguf", HOST_CTX, forms, parity = parity) $(var m, form) {
        if (form.metal_image) {
            dflash_27b_image_cells(t, m, form)
        } elif (form.on_metal) {
            dflash_27b_metal_cells(t, m)
        } elif (form.on_gpu) {
            dflash_27b_discrete_cells(t, m)
        } else {
            dflash_27b_cpu_cells(t, m)
        }
    }
    delete forms
}

let private DFLASH_27B_TRUNK = "Qwen3.8-27B-Q4_K_M.gguf"   //! the drafter cells' trunk
//! the 27B trunk's Q8_0 NextN head as a split shard
let private SPLIT_HEAD_FILE = "mtp-Qwen3.8-27B-Q8_0.gguf"

//! the 27B trunk's forms: the drafter's CPU, discrete and Metal twin
//! forms where the arm filter keeps a drafter cell; the served image
//! for the batch rail's verify where its arms reach it; the served
//! image with the split head pinned where the head file sits
def private dflash_27b_forms(t : T?) : array<ModelForm> {
    var forms : array<ModelForm>
    if (dflash_27b_arms_on()) {
        forms |> push(cpu_cells_form())
        static_if (typeinfo module_exists(dasllama_vulkan_dflash)
                || typeinfo module_exists(dasllama_cuda_dflash)) {
            forms |> push(discrete_drafter_form())
        } else {
            skip_cell(t, ("{DFLASH_27B_TRUNK}, {form_name(discrete_drafter_form())}: " +
                "27B discrete: no discrete GPU home in this build"))
        }
        if (any_arm_on(DFLASH_27B_METAL_ARMS)) {
            forms |> push(metal_form(required = true, ctx = HOST_CTX))
        }
    } else {
        skip_cell(t, ("the arm filter keeps none of the 27B trunk's drafter cells " +
            "(DASLLAMA_TEST_ARMS={g_env_test.test_arms})"))
    }
    var inscope verify_arms <- mtp_verify_arms(mtp_verify_qwen38_27b().tag)
    if (any_arm_on(verify_arms)) {
        forms |> push(metal_form(image = true, ctx = METAL_MTP_CELLS_CTX))
    }
    let split = metal_form(image = true, ctx = METAL_MTP_CELLS_CTX, mtp_head = SPLIT_HEAD_FILE)
    var split_on = false
    unsafe {   //! the head file's skip lands on a subtest naming the form
        t |> run("{DFLASH_27B_TRUNK}, {form_name(split)}") @capture(& split_on)(t : T ?) {
            split_on = model_available(t, path_join(_model_tier::models_dir(), SPLIT_HEAD_FILE))
        }
    }
    if (split_on) {
        forms |> push(split)
    }
    return <- forms
}

//! the 27B trunk's served-image cells: the batch rail's verify on the
//! plain image, the MTP rail's arms on the split-head image
def private dflash_27b_image_cells(t : T?; var m : Model; form : ModelForm) {
    if (empty(form.mtp_head)) {
        mtp_verify_metal(t, m, form, mtp_verify_qwen38_27b())
    } else {
        mtp_parity_metal(t, m, form, mtp_parity_qwen38_27b_split_head())
    }
}

//! the three acceptances on the Metal home's device arm over the
//! served trunk `m` with the Q8_0 drafter
[cold_path]
def private sampled_walks_cell(t : T?; var m : Model; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!model_available(t, path_join(_model_tier::models_dir(), DRAFTER_FILE)))
        on_metal_trunk() {
            sampled_modes_walk(t, m, DRAFTER_FILE, true, tag)
        }
    } else {
        t |> skip("{tag}: no das_metal in this build")
    }
}

//! the 27B snapshot's forms: its drafter cells' blob twin under the
//! required mode where the arm filter keeps one, beside the
//! snapshot's own forms against mlx-lm (`mlx_qwen38_27b_forms`)
def private dflash_27b_mlx_forms(t : T?; drafter : bool) : array<ModelForm> {
    var forms : array<ModelForm>
    if (drafter) {
        forms |> push(metal_form(required = true, ctx = HOST_CTX))
    } else {
        skip_cell(t, ("the arm filter keeps none of the 27B snapshot's drafter cells " +
            "(DASLLAMA_TEST_ARMS={g_env_test.test_arms})"))
    }
    var inscope mlx <- mlx_qwen38_27b_forms(t)
    forms |> push_from(mlx)
    return <- forms
}

//! the 27B snapshot's drafter cells on its required blob twin `m`: the
//! sampled walks and the lossless gate, each under its arm
def private dflash_27b_mlx_cells(t : T?; var m : Model; sampled, lossless : bool) {
    unsafe {   //! the cells capture the loaded trunk by reference
        if (sampled) {
            let title = ("the sampled DFlash walks on the Metal device arm over the 27B mlx " +
                "4-bit snapshot trunk")
            t |> run(title) @capture(& m)(t : T ?) {
                sampled_walks_cell(t, m, "mlx sampled metal")
            }
        }
        if (lossless) {
            let title = ("the lossless greedy DFlash gate on the Metal device arm over the 27B " +
                "mlx 4-bit snapshot trunk")
            t |> run(title) @capture(& m)(t : T ?) {
                lossless_metal_cell(t, m, DRAFTER_FILE, [KqFmt.q8], [KqFmt.mlx4b], "mlx metal")
            }
        }
    }
}

[test(description = "Over the 27B affine mlx 4-bit snapshot trunk on the Metal home's blob twin under the required mode, one load on a das_metal build: the sampled DFlash walks on the device arm hold - exact spends one draw a judged row with drafts accepted and opens on the plain sampled walk's first id, rejection walks accept and reject, and device and host draws give the same stream; the lossless greedy DFlash gate on the device arm holds, with device and host accept and selector agreeing and mlx4b planes among the trunk's rows8 verify sites where rows8 runs; on the snapshot's own forms (`mlx_qwen38_27b_forms`) its cells against mlx-lm - on the snapshot's Metal twin the teacher-forced logits and the served window and greedy steps, on its CPU form the mlx4b planes, the tokenizer's ids and the teacher-forced logits, the served cell's CPU side on the portable CPU form; each form loads once")]
def test_dflash_27b_mlx(t : T?) {
    return if (!family_on(t, "qwen35"))
    let sampled = empty(arm_why("dflash-27b-mlx-sampled"))
    let lossless = empty(arm_why("dflash-27b-mlx-metal"))
    var served : MlxServedKept
    var forms <- dflash_27b_mlx_forms(t, sampled || lossless)
    if (empty(forms)) {
        delete forms
        return
    }
    with_cpu_and_gpu_snapshot(t, "llm", "mlx-community/Qwen3.8-27B-4bit", HOST_CTX, forms,
            false) $(var m, form) {
        if (form.on_metal && form.metal_required) {
            dflash_27b_mlx_cells(t, m, sampled, lossless)
        } else {
            mlx_qwen38_27b_cells(t, m, form, served)
        }
    }
    mlx_served_verdict(t, served)
    delete forms
}

// Qwen3.6-27B-MTP Q4_K_M — the 64-trunk-layer carrier: its draft block sits at layer index 64, the
// exact index where the pre-fix per-layer masks wrapped (mask >> 64 == mask >> 0 on x86) — this
// arm regresses that whole bug class (layer_is_recurrent / layer_is_sliding / wv_offs guards)
// besides being the model the feature is FOR (1.39x decode when frozen). The `qwen35-27b`
// category's (~16 GB): run.das --category qwen35-27b. Its served image on the Metal home carries
// the MTP rail's arms.
[test(description = "On Qwen3.6-27B-MTP Q4_K_M, whose draft block sits at layer index 64 where per-layer masks once wrapped, on the q8 CPU form self-speculative greedy decode emits plain decode's counting tokens exactly; on the Metal home's served image, one load on a das_metal build, the Metal MTP rail's verify rows within 0.5 of the plain GPU step on forced feed and its replayed rounds, and speculative free-run emitting the plain run's tokens exactly; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token; each form loads once")]
def test_mtp_invariance_27b(t : T?) {
    var forms <- [cpu_cells_form(), metal_form(image = true, ctx = METAL_MTP_CELLS_CTX)]
    with_cpu_and_gpu_model(t, "llm", "Qwen3.6-27B-MTP-Q4_K_M.gguf", MTP_27B_CTX, forms,
            parity = true) $(var m, form) {
        if (form.on_metal) {
            mtp_parity_metal(t, m, form, mtp_parity_qwen36_27b())
        } elif (alone_pass_of(form, cpu_cells_form())) {
            let title = ("MTP self-spec output-invariance (Qwen3.6-27B-MTP Q4_K_M, draft block " +
                "at layer 64)")
            unsafe {   //! the cell reads the block's model by reference
                t |> run(title) @capture(& m)(t : T ?) {
                    t |> equal(m.config.n_layer_nextn, 1l)
                    t |> equal(m.config.n_layers, 64l)
                    with_pinned_kernels() {
                        t |> invariance_ok(m, MTP_PROMPT, 48, "counting-27b")
                    }
                }
            }
        }
    }
    delete forms
}

let private MTP_27B_CTX = 2048l   //! the invariance pair's sessions and the parity row's

// GLM-4.5-Air Q4_K_M — the only NON-RECURRENT MTP model we have, so this is the sole arm covering
// mtp_spec_eval's `recr_mask == 0` reject branch (a rejected draft consumes the verify's own row-0
// logits + h0 instead of paying a second full forward; every qwen35 MTP file is a deltanet hybrid
// and takes the restore-and-re-advance branch instead). The code fixture's own reject count rides
// the kernels' summation order (it has read zero), so the arm reaches the branch through the
// engine's seam - every third draft rejected whatever the verify said - and holds the stream to
// plain decode there too. It is also the first arch combining
// attn_qkv_bias with a NextN block — the shape where the qkv/out bias arrays were sized by the
// TRUNK layer count, so blk.<n_layers>.attn_{q,k,v}.bias never loaded and the draft read past the
// end of t.bq. And glm4moe ships nextn.embed_tokens/shared_head_head as copies of the trunk's own
// tables, which the loader shape-checks rather than disabling the head over.
// The `glm45-air` category's (~73 GB, 2 shards): run.das --category glm45-air.
[test(description = "On non-recurrent GLM-4.5-Air with qkv bias and a NextN head, self-speculative decode of a code prompt emits plain decode's tokens exactly, including with every third draft forced through the recr_mask-zero reject branch")]
def test_mtp_invariance_glm(t : T?) {
    t |> run("MTP self-spec output-invariance (GLM-4.5-Air Q4_K_M, non-recurrent reject path)") @(t : T?) {
        let path = path_join(models_dir(), "glm45air/Q4_K_M/GLM-4.5-Air-Q4_K_M-00001-of-00002.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_model_(path, QuantMode.q8)   // load_model: this arm encodes its prompt in-test
        t |> equal(tr.config.n_layer_nextn, 1l)    // head ENABLED, not disabled over the table copies
        t |> equal(tr.config.n_layers, 46l)        // block_count 47 - nextn 1
        // the two properties that make this arm load-bearing — asserted so a future file losing
        // either tells us the coverage moved instead of silently passing
        t |> equal(tr.config.recr_mask, 0ul)
        t |> success(tr.config.attn_qkv_bias, "glm4moe carries qkv bias (the layers_ld sizing case)")
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            // A CODE prompt on purpose. Strict token-identity is only meaningful on a NEAR-TIE-FREE
            // fixture: on K-quant planes the accept path reads BATCH-kernel logits (mm_gemm/mm_b_f)
            // while plain decode reads GEMV (mm_kq), and those differ in reduction order — a
            // legitimate FP difference that `with_pinned_kernels` does NOT cover (it pins attention
            // mode, activations and deltanet, not the matmul family). At Q4_K that delta is far
            // below the model's own quantization error, so on tie-dense PROSE the two paths pick
            // differently at ties and identity fails for no defect (measured: prose diverges every
            // run, this fixture is bit-identical at 95% accept). Code is strongly determined ⇒ no
            // ties to break ⇒ identity is a real assertion. Same reasoning as the 27B arm's
            // deliberately near-tie-free counting fixture.
            // FOLLOW-UP: a margin-gated arm can cover prose — assert that any divergence sits below
            // the quantization noise floor; mtp_verify_logits already computes the margin.
            let code <- encode(tr, "def fibonacci(n):", true, false)
            let ref_ids <- invariance_ok(t, tr, code, 40, "code-glm")
            forced_invariance_ok(t, tr, code, 40, ref_ids, "code-glm")
        }
        delete tr
    }
}
