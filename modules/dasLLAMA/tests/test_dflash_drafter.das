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
require dasllama/dasllama_scheduler   // the joint round's two streams
require _scheduler_rig   // make_req, drain, toks_of
require _resident_feed   // resident_trunk, load_resident_at: the discrete homes' trunk
require _cpu_gpu_model   // with_cpu_and_gpu_model, the forms: the 27B trunk's block
require _parity_cells   // the parity cells the block's parity knob reaches (`_::`): the 27B trunk's model_specs parity row on its block
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_verifies: the discrete walks' verifies on the resident driver
require ?vulkan dasllama/dasllama_vulkan_dflash   // nolint:STYLE030,LINT019 - the Vulkan arm registers at init
require ?cuda dasllama/dasllama_cuda_dflash   // nolint:STYLE030,LINT019 - the CUDA arm registers at init

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
// twin under the required mode; the snapshot trunk's and the E2B facade's Metal cells load their trunk off the image rail
// through `with_metal_trunk`. A cell skips
// without its files (the trunk, the drafters, the 27B snapshot), on a model file whose category the run does not name
// (`../TEST_CATEGORIES.md`), on the 27B snapshot without DASLLAMA_PARITY_FULL=1, and
// on the Metal device arms on a build without das_metal; the discrete-home cells skip on a build with no discrete GPU home,
// with no armed GPU tier (`gpu_tier_armed`), where the residency plan does not fit the card, and when interpreted.
// The NextN head's lossless gate on the large -MTP- carriers no block loads sits beside them: self-speculative greedy decode
// token for token against plain decode on the 64-trunk-layer Qwen3.6-27B-MTP and on the non-recurrent GLM-4.5-Air (forced
// rejects included), the first the `qwen35-27b` category's and the second the `glm45-air` category's, the invariance pair
// `_mtp_cpu_cells.das`'s.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU arm prefills on the CPU by design
}

let private SNAPSHOT_REPO = "mlx-community/Qwen3.8-27B-4bit"   //! the same model's affine 4-bit snapshot: mlx4b planes, no NextN head
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

//! the trunk at `path` loaded off the image rail - parsed into RAM, no `.dlim` minted or mapped beside it - and, for the Metal
//! home (`metal`), turned into the blob form the drivers serve, as the engine's own load does with the rail off
def private load_trunk(t : T?; path : string; metal : bool) : Model {
    var m <- load_model_(path, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, HOST_CTX)
    if (metal) {
        t |> success(convert_model_to_metal_blob(m), "the trunk takes the metal-blob form")
    }
    return <- m
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

//! the affine snapshot trunk's directory (large tier, as `test_mlx_snapshot.das`'s 27B cell); "" with the loud skip where it is
//! absent or the tier is off
def private snapshot_trunk(t : T?) : string {
    let snap = hf_snapshot_dir(SNAPSHOT_REPO)
    if (empty(snap)) {
        t |> skip("the 27B snapshot not present")
        return ""
    }
    if (!parity_full_mode()) {
        t |> skip("the 27B snapshot is large-tier - set DASLLAMA_PARITY_FULL=1 to run")
        return ""
    }
    return snap
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

//! `body` on the served `trunk` loaded for the Metal home - the mode required, the box's crowns applied as the server applies
//! them - then the mode put back
def private load_metal_trunk(t : T?; trunk : string; body : block<(var m : Model) : void>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior = get_metal_mode()
        set_metal_mode(MetalMode.required)
        apply_box_profile_runtime()
        with_dasllama_jobque() {
            var m <- load_trunk(t, trunk, true)
            invoke(body, m)
            delete m
        }
        set_metal_mode(prior)
    }
}

//! `body` on the served `trunk` (`load_metal_trunk`) under the Metal home's decode override (`on_metal_trunk`)
def private with_metal_trunk(t : T?; trunk : string; body : block<(var m : Model) : void>) {
    load_metal_trunk(t, trunk) $(var m) {
        on_metal_trunk() {
            invoke(body, m)
        }
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

//! the lossless legs (`metal_lossless_legs`) over the served `trunk` ("" where its skip is taken), the drafter file present
def private metal_lossless_cell(t : T?; trunk, drafter : string; fmts, trunk_fmts : array<KqFmt>;
                                tag : string; rows8 : bool = true) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (empty(trunk) || !model_available(t, path_join(_model_tier::models_dir(), drafter)))
        load_metal_trunk(t, trunk) $(var m) {
            metal_lossless_legs(t, m, drafter, fmts, trunk_fmts, tag, rows8)
        }
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

[test(description = "On the planar Qwen3.8-27B Q4_K_M trunk on the CPU arm, with the Q8_0 DFlash2 drafter the exact-accept sampled speculative walk spends one draw a judged row with drafts accepted and opens on the plain seeded sampled walk's first id, honors a stop id, and both rejection walks accept some drafts and reject some, and the greedy speculative walks of the Q8_0, Q4_K_M and NVFP4 drafters emit the plain greedy walk's ids token for token with drafts accepted, the K-quant and NVFP4 files' planes tagged with their disk types; on the serving discrete home's drafter form each drafter's greedy walk on the device arm is the plain walk's on the same device, every pass ending on the device's top-k and walk, the host's walk and selector drafting the same ids; on the Metal form, the file's blob twin under the required mode, the exact sampled walk holds the CPU arm's draws and equals the plain seeded sampled walk token for token, the rejection walks accept some drafts and reject some with drawn drafts on the device selector, each drafter file's greedy walk on the device arm is lossless with its formats' sites encoded by the pass, device and host accept and selector agreeing, replays unwaited on the tape and the CPU arm on the device's captures accepting the same count, depth pins hold, and two streams through the scheduler ride the verify seat's joint groups, each the plain scheduler's token for token; on the parity row's CPU form, on every box, the file's frozen greedy ids token for token with its chunked-prefill arm; each form loads once")]
def test_dflash_27b_trunk(t : T?) {
    return if (!family_on(t, "qwen35"))
    if (!dflash_27b_arms_on()) {
        t |> skip("the arm filter keeps none of the 27B trunk's cells (DASLLAMA_TEST_ARMS={g_env_test.test_arms})")
        return
    }
    var forms <- [cpu_cells_form()]
    static_if (typeinfo module_exists(dasllama_vulkan_dflash) || typeinfo module_exists(dasllama_cuda_dflash)) {
        forms |> push(discrete_drafter_form())
    } else {
        t |> skip("27B discrete: no discrete GPU home in this build")
    }
    if (any_arm_on(DFLASH_27B_METAL_ARMS)) {
        forms |> push(metal_form(required = true, ctx = HOST_CTX))
    }
    let parity = empty(arm_why("dflash-27b-cpu"))
    with_cpu_and_gpu_model(t, "llm", "Qwen3.8-27B-Q4_K_M.gguf", HOST_CTX, forms, parity = parity) $(var m, form) {
        if (form.on_metal) {
            dflash_27b_metal_cells(t, m)
        } elif (form.on_gpu) {
            dflash_27b_discrete_cells(t, m)
        } else {
            dflash_27b_cpu_cells(t, m)
        }
    }
    delete forms
}

//! the three acceptances on the Metal home's device arm over the served `trunk` with the Q8_0 drafter
[cold_path]
def private sampled_metal_cell(t : T?; trunk : string; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (empty(trunk) || !model_available(t, path_join(_model_tier::models_dir(), DRAFTER_FILE)))
        with_metal_trunk(t, trunk) $(var m) {
            sampled_modes_walk(t, m, DRAFTER_FILE, true, tag)
        }
    } else {
        t |> skip("{tag}: no das_metal in this build")
    }
}

[test(description = "The sampled DFlash walks on the Metal device arm hold over the 27B affine mlx 4-bit snapshot trunk: exact spends one draw a judged row with drafts accepted and opens on the plain sampled walk's first id, rejection walks accept and reject, and device and host draws give the same stream")]
def test_dflash_27b_mlx_sampled(t : T?) {
    if (!arm_on(t, "dflash-27b-mlx-sampled") || !family_on(t, "qwen35")) {
        return
    }
    sampled_metal_cell(t, snapshot_trunk(t), "mlx sampled metal")
}

[test(description = "The lossless greedy DFlash gate on the Metal device arm holds over the 27B mlx 4-bit snapshot trunk, with device and host accept and selector agreeing and mlx4b planes among the trunk's rows8 verify sites where rows8 runs")]
def test_dflash_27b_mlx_metal(t : T?) {
    if (!arm_on(t, "dflash-27b-mlx-metal") || !family_on(t, "qwen35")) {
        return
    }
    metal_lossless_cell(t, snapshot_trunk(t), DRAFTER_FILE, [KqFmt.q8], [KqFmt.mlx4b], "mlx metal")
}

let private CHAT_TURNS = fixed_array("Describe the TCP three-way handshake in three sentences.", "Now name the side that picks the first sequence number, in one sentence.")
let private CHAT_MAX_NEW = 96l

//! a two-turn greedy chat on `m`, thinking off: each turn's reply
def private chat_replies(m : Model) : array<string> {
    var replies : array<string>
    var chat <- create_chat(m, "", CHAT_MAX_NEW)
    set_thinking(chat, false)
    replies |> reserve(length(CHAT_TURNS))
    for (turn in CHAT_TURNS) {
        add_user(chat, turn)
        replies |> push(respond(m, chat, SamplingParams(temp = 0.0)) $(_piece) => true)
    }
    delete chat
    return <- replies
}

//! a scheduler's streams over `prompts`, `budget` ids each, speculating or plain: every stream's ids, the drafts the
//! scheduler accepted into `accepted`, the most streams one of its verifies carried into `joined`
def private sched_streams(m : Model; prompts : array<array<int64>>; budget : int64; spec : bool; var accepted, joined : int64&) : array<array<int64>> {
    var ev : array<SchedEvent>
    let n = long_length(prompts)
    var sch <- create_scheduler(m, n, n, 0l, 64l, 0l, KVDtype.f16, spec)
    for (i in range64(n)) {
        submit(m, sch, make_req(i + 1l, prompts[i], budget))
    }
    drain(m, sch, ev)
    var out <- [for (i in range64(n)); toks_of(ev, i + 1l)]
    accepted = sch.mtp_accepted
    joined = sch.mtp_joined_max
    delete sch
    delete ev
    return <- out
}

//! two streams through the scheduler with the drafter attached, at three drafts a round: a tick's rounds draft in one joint
//! pass and verify in one joint step, each stream the plain scheduler's token for token (the driver against itself) and the
//! two together accepting what each accepts alone within a fifth (the joint pass's drafts are each stream's own: a pass
//! reading another stream's rows drafts noise and accepts next to nothing); the rounds before it carried one stream each (the
//! joint count's control), and on a hybrid the joint verifies ride the seat's groups
def private joint_streams_leg(t : T?; m : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope prompts <- [for (turn in CHAT_TURNS); encode(m, turn, true)]
        var none, alone, together, joined, lone = 0l
        var inscope plain <- sched_streams(m, prompts, CHAT_MAX_NEW, false, none, joined)
        let depth_pin = mtp_depth_pin()
        set_mtp_depth(3l)
        for (p in prompts) {
            var inscope one : array<array<int64>>
            one |> push_clone(p)
            var acc = 0l
            var inscope solo <- sched_streams(m, one, CHAT_MAX_NEW, true, acc, lone)
            alone += acc
            t |> success(lone <= 1l, "a stream alone rides no joint verify ({lone}) - the joint count's control")
        }
        let j0 = metal_dflash_joint_rounds()
        let gv0 = metal_mtp_group_verifies()
        let held0 = metal_dflash_selected().held
        var inscope drafted <- sched_streams(m, prompts, CHAT_MAX_NEW, true, together, joined)
        restore_mtp_depth(depth_pin)
        let held = metal_dflash_selected().held - held0
        let j1 = metal_dflash_joint_rounds()
        if (metal_rows8_on()) {
            t |> success(alone > 0l, "each stream alone accepts drafts ({alone})")
            t |> success(together * 5l >= alone * 4l, "the two streams together accept what they accept alone within a fifth ({together} against {alone})")
            t |> equal(joined, 2l, "a tick's verify carried both streams' rows")
            t |> success(j1.rounds > j0.rounds, "the two streams ran joint rounds ({j1.rounds - j0.rounds})")
            t |> equal(held, 0l, "a joint round lands its drafts ahead of its verify, so no pass is held for the verify seat")
            if (m.config.recr_mask != 0ul) {
                t |> success(metal_mtp_group_verifies() > gv0, "a hybrid's joint verifies rode the seat's groups ({metal_mtp_group_verifies() - gv0})")
            }
        } else {
            t |> success(joined <= 1l, "off the matrix-unit form two streams stand down to the plain batched step")
        }
        for (i in range(length(prompts))) {
            t |> equal(length(drafted[i]), length(plain[i]), "stream {i}: the drafted stream runs the plain stream's length")
            t |> equal(first_mismatch(drafted[i], plain[i]), -1, "stream {i}: the drafted scheduler's ids are the plain scheduler's")
        }
    }
}

[test(description = "On gemma-4-E2B's Metal home with its DFlash2 drafter attached through the facade, a two-turn greedy chat's replies and a greedy completion's ids are the plain model's token for token (a driver-against-itself compare, the drafted chat's steps counted on the device), the drafted walks accepting drafts; two streams through the scheduler ride joint rounds with no pass held, each the plain scheduler's token for token; the stream with a stop id ends on that id's first occurrence; on the same load the exact sampled DFlash walk spends one draw a judged row with drafts accepted and opens on the plain sampled walk's first id, both rejection walks accept some drafts and reject some, drawn drafts end on the device selector, and verify-ahead off or the soft pick off reproduce the same stream, no sampled round holding its pass; over a prompt past one of the drafter's windows the ring that grew its dense layer drafts id for id what a ring reserved that wide drafts, and with every layer held to the window the drafts differ")]
def test_dflash_e2b_facade_streams(t : T?) {
    if (!arm_on(t, "dflash-e2b-facade") || !family_on(t, "gemma4e")) {
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let trunk = path_join(_model_tier::models_dir(), E2B_TRUNK_FILE)
        let drafter = path_join(_model_tier::models_dir(), E2B_DRAFTER_FILE)
        return if (!model_available(t, trunk) || !model_available(t, drafter))
        with_metal_trunk(t, trunk) $(var m) {
            let prompt <- encode(m, PROMPT, true)
            var inscope plain_replies <- chat_replies(m)
            var s1 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            var inscope plain_ids : array<int64>
            generate(m, s1, prompt, SamplingParams(temp = 0.0), COMPLETE_STEPS) $(id, _piece) {
                plain_ids |> push(id)
                return true
            }
            delete s1
            set_dflash_device(true)
            t |> equal(attach_block_drafter(m, drafter), "", "the drafter attaches through the facade")
            t |> success(block_drafter_attached(m), "the facade reports it attached")
            let st0 = metal_dflash_stats()
            let dev0 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
            var inscope drafted_replies <- chat_replies(m)
            let st1 = metal_dflash_stats()
            t |> success(st1.passes > st0.passes, "the chat's turns ran draft passes ({st1.passes - st0.passes})")
            let dev1 = metal_decode_stats().decodes + metal_batch_decode_stats().steps
            t |> success(dev1 > dev0, "the drafted chat's steps ran on the device ({dev1 - dev0})")
            t |> equal(length(drafted_replies), length(plain_replies))
            for (a, b, i in drafted_replies, plain_replies, count()) {
                t |> equal(a, b, "turn {i}: the drafted chat's reply is the plain chat's")
            }
            var s2 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            var inscope drafted_ids, no_stops : array<int64>
            let n = generate_drafted(m, s2, prompt, SamplingParams(temp = 0.0), COMPLETE_STEPS, no_stops) $(id, _piece) {
                drafted_ids |> push(id)
                return true
            }
            t |> equal(n, COMPLETE_STEPS, "the drafted completion runs its budget")
            t |> success(s2.mtp_accepted > 0l, "the completion accepted drafts ({s2.mtp_accepted} of {s2.mtp_drafted})")
            t |> equal(length(drafted_ids), length(plain_ids))
            t |> equal(first_mismatch(drafted_ids, plain_ids), -1, "the drafted completion's ids are the plain completion's")
            delete s2
            //! a stop id: the stream ends on its first occurrence, nothing past it emitted
            let stop_id = plain_ids[length(plain_ids) / 2]
            var first = 0
            while (plain_ids[first] != stop_id) {
                first++
            }
            var s3 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
            var inscope stops <- [stop_id]
            var inscope stopped_ids : array<int64>
            generate_drafted(m, s3, prompt, SamplingParams(temp = 0.0), COMPLETE_STEPS, stops) $(id, _piece) {
                return false if (id == stop_id)
                stopped_ids |> push(id)
                return true
            }
            t |> equal(length(stopped_ids), first, "the stopped stream emits the ids before the stop's first occurrence ({first})")
            t |> success(first < length(plain_ids) - 1, "the stop sits inside the budget (the control: the stopless stream ran past it)")
            delete s3
            tile_attention_leg(t, m, prompt, plain_ids)
            joint_streams_leg(t, m)
            detach_block_drafter()
            t |> success(!block_drafter_attached(m), "the facade reports it detached")
            e2b_metal_twins(t, m)
        }
    } else {
        t |> skip("E2B facade streams: no das_metal in this build")
    }
}

let private TILE_BAR = 5e-3   //! the tile form's logits against the fused form's, of a row's largest live logit: the two sum a row's keys in different orders

//! The tile form of a full-context layer's attention, pinned on at every depth and pinned off (the control: no dispatch):
//! the plain completion's ids fed a step at a time give the fused form's logits within `TILE_BAR`, each step's control the
//! fused row a step off; the drafted completion's verify and the scheduler's two-stream step take the form too. The
//! verify's and the batch's rows are held to the oracle by the kernel cells (`sq_tile_gate`): a drafted walk's ids can tie
[cold_path]
def private tile_attention_leg(t : T?; var m : Model; prompt, plain_ids : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope fed : array<array<array<float>>>
        var inscope walks : array<array<int64>>
        var inscope prompts <- [for (turn in CHAT_TURNS); encode(m, turn, true)]
        for (far in [0l, -1l]) {
            let on = far == 0l
            with_metal_attn_tile_far(far) {
                metal_kernel_coverage_reset()
                fed |> emplace <| feed_alone(m, KVDtype.f16, prompt, plain_ids)
                let steps = metal_kernel_count("metal_sq_attn_tile")
                metal_kernel_coverage_reset()
                var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
                var ids, no_stops : array<int64>
                generate_drafted(m, s, prompt, SamplingParams(temp = 0.0), COMPLETE_STEPS, no_stops) $(id, _piece) {
                    ids |> push(id)
                    return true
                }
                delete s
                delete no_stops
                let verifies = metal_kernel_count("metal_sq_attn_tile")
                walks |> emplace(ids)
                metal_kernel_coverage_reset()
                var none, joined = 0l
                var inscope streams <- sched_streams(m, prompts, CHAT_MAX_NEW, false, none, joined)
                let batched = metal_kernel_count("metal_sq_attn_tile")
                if (on) {
                    t |> success(steps > 0l, "the fed steps took the tile form on the full-context layers ({steps} dispatches)")
                    t |> success(verifies > 0l, "the drafted completion's steps took it ({verifies} dispatches)")
                    t |> success(batched > 0l, "the scheduler's two streams' steps took it ({batched} dispatches)")
                } else {
                    t |> equal(steps + verifies + batched, 0l, "control: the tile form pinned off dispatches none")
                }
            }
        }
        log_gen_texts(m, "E2B drafted completion, fused (as CPU) against tile (as GPU):", walks[1], walks[0])
        feed_steps_within(t, m, "E2B fed steps, tile against fused", long_length(prompt), fed[0], fed[1], plain_ids, TILE_BAR, "the tile form", "the fused form")
    }
}

//! the 27B's sampled Metal cell on the E2B pair, on the facade cell's load: the three sampled acceptances on the Q8_0 file
[cold_path]
def private e2b_metal_twins(t : T?; var m : Model) {
    unsafe {   //! the cell captures the loaded trunk by reference
        t |> run("gemma-4-E2B, the sampled DFlash walks on the Metal device arm: the exact walk spends one draw a judged row and opens on the plain sampled walk's first id, a stop id, the rejection walks accept some and reject some, the device selector, verify-ahead and soft pick give the same stream") @capture(& m)(t : T ?) {
            sampled_modes_walk(t, m, E2B_DRAFTER_FILE, device = true, tag = "E2B sampled metal", joint = true)
        }
        t |> run("gemma-4-E2B on the Metal device arm, over a prompt past one of the DFlash2 drafter's windows: the ring that grew its dense layer drafts id for id what a ring reserved that wide drafts, and with every layer held to the window the drafts differ") @capture(& m)(t : T ?) {
            dense_ring_leg(t, m, E2B_DRAFTER_FILE, "E2B Metal, the dense layer", METAL_DENSE_ITEMS, HOST_CTX, 1l, true)
        }
    }
}

// Qwen3.6-27B-MTP Q4_K_M — the 64-trunk-layer carrier: its draft block sits at layer index 64, the
// exact index where the pre-fix per-layer masks wrapped (mask >> 64 == mask >> 0 on x86) — this
// arm regresses that whole bug class (layer_is_recurrent / layer_is_sliding / wv_offs guards)
// besides being the model the feature is FOR (1.39x decode when frozen). The `qwen35-27b`
// category's (~16 GB): run.das --category qwen35-27b.
[test(description = "On Qwen3.6-27B-MTP Q4_K_M, whose draft block sits at layer index 64 where per-layer masks once wrapped, self-speculative greedy decode emits plain decode's counting tokens exactly")]
def test_mtp_invariance_27b(t : T?) {
    t |> run("MTP self-spec output-invariance (Qwen3.6-27B-MTP Q4_K_M, draft block at layer 64)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.6-27B-MTP-Q4_K_M.gguf")
        return if (!jit_model_available(t, path))
        var tr <- load_gguf(path, QuantMode.q8)
        t |> equal(tr.config.n_layer_nextn, 1l)
        t |> equal(tr.config.n_layers, 64l)
        tr.config.seq_len = 2048l
        with_pinned_kernels() {
            t |> invariance_ok(tr, MTP_PROMPT, 48, "counting-27b")
        }
        delete tr
    }
}

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
