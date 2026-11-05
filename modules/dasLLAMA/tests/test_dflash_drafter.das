options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade - engine + arch registrations
require dasllama/dasllama_common   // set_metal_mode, allow_cpu_prefill, generate, generate_mtp_greedy, the overrides
require dasllama/dasllama_dflash   // the subject: the DFlash2 block drafter's loader, seams and CPU arm
require dasllama/dasllama_sampling   // generate_, generate_mtp_sampled, set_seed_: the sampled walks
require dasllama/dasllama_gguf   // gguf_header_read, gguf_find_tensor: the drafter file's disk types against the loader's tags
require dasllama/dasllama_math   // with_dasllama_jobque
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - the Metal decode driver, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_dflash   // nolint:STYLE030,LINT019 - the drafter's device arm registers at init; its site counters
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - metal_decode_shutdown
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_prefill_shutdown, metal_rows8_on
require ?das_metal metal/das_metal_boost   // nolint:STYLE030,LINT019 - the system device the rows8 layout probe runs on
require ?das_metal _metal_kernel_common   // nolint:STYLE030,LINT019 - with_metal_cell: the probe's planes
require ?das_metal _rows8_probe   // nolint:STYLE030,LINT019 - rows8_probe_ok: the rows8 witnesses' layout fact
require dasllama/dasllama_layout   // convert_model_to_metal_blob - the device arm's trunk off the image rail
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 - g_env_metal.rows_mt, inside the das_metal half
require daslib/fio
require strings   // find: the refusal text's words
require _gguf_writer   // gguf_header, gguf_push_le, gguf_push_str: the synthesized drafter header
require _model_tier   // _model_tier::models_dir() + model_available() presence/size-tier gate
require _scratch_dir   // scratch_path / scratch_file: the refusal fixtures' files

// The DFlash2 block drafter (`dasllama/dasllama_dflash`): the loader's refusal contract, and the lossless gate every arm of
// the pass is held to - a greedy speculative walk's ids equal the plain walk's, with drafts accepted along the way - on the
// CPU arm over the planar Qwen3.8-27B (large tier, PARITY_FULL-gated) and on the Metal home's device arm over the served
// trunk, where the CPU arm fed by the device's captures accepts the same count. The drafter sidecars are the 27B's own
// DFlash2 Q8_0 file and its Q4_K_M file (Q4_K planes, Q6_K at some layers' v and down) under `drafters/`. A cell skips
// without its files (the trunk, the drafters, the 27B snapshot), above the large tier without DASLLAMA_PARITY_FULL=1, and
// on the device arms on a build without das_metal.

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the CPU arm prefills on the CPU by design
}

let private TRUNK_FILE = "Qwen3.8-27B-Q4_K_M.gguf"
let private SNAPSHOT_REPO = "mlx-community/Qwen3.8-27B-4bit"   //! the same model's affine 4-bit snapshot: mlx4b planes, no NextN head
let private DRAFTER_FILE = "drafters/Qwen3.8-27B-DFlash2-GGUF/Qwen3.8-27B-DFlash2-Q8_0.gguf"
let private DRAFTER_K4_FILE = "drafters/Qwen3.8-27B-DFlash2-GGUF/Qwen3.8-27B-DFlash2-Q4_K_M.gguf"
let private PROMPT = "Write a complete, well-commented Python module that implements a token-bucket rate limiter with per-key buckets, a sliding-window variant, a thread-safe wrapper, and a pytest test suite. Write at least 600 lines. Do not stop early."
let private STEPS = 24l

let private GGUF_U32 = 4ul   //! the GGUF value types the synthesized header writes
let private GGUF_I32 = 5ul
let private GGUF_F32 = 6ul
let private GGUF_STRING = 8ul
let private GGUF_ARRAY = 9ul

//! one GGUF key with a u32 value
def private kv_u32(var b : array<uint8>; key : string; v : uint64) {
    gguf_push_str(b, key)
    gguf_push_le(b, GGUF_U32, 4)
    gguf_push_le(b, v, 4)
}

//! one GGUF tensor record: its name, its dims, its type and its offset in the data
def private tensor_rec(var b : array<uint8>; name : string; dims : array<uint64>; gtype : int; off : uint64) {
    gguf_push_str(b, name)
    gguf_push_le(b, uint64(long_length(dims)), 4)
    for (dv in dims) {
        gguf_push_le(b, dv, 8)
    }
    gguf_push_le(b, uint64(gtype), 4)
    gguf_push_le(b, off, 8)
}

//! `b` zero-padded to the GGUF data alignment
def private pad32(var b : array<uint8>) {
    while (long_length(b) % 32l != 0l) {
        b |> push(uint8(0))
    }
}

//! a one-layer `dflash` file 256 wide whose fc.weight is of disk type `fc_type`, its selector codebook Q8_0, and no other
//! tensor: the loader reads the header, then fc.weight is the first plane it plans
def private synth_dflash(fc_type : int) : array<uint8> {
    let d = 256ul
    var b : array<uint8>
    gguf_header(b, 2ul, 17ul)
    gguf_push_str(b, "general.architecture")
    gguf_push_le(b, GGUF_STRING, 4)
    gguf_push_str(b, "dflash")
    for (kv in [("block_count", 1ul), ("embedding_length", d), ("feed_forward_length", d), ("attention.head_count", 2ul),
                ("attention.head_count_kv", 1ul), ("attention.key_length", 128ul), ("block_size", 8ul), ("conv_kernel_size", 2ul),
                ("conv_group_size", 64ul), ("selector_rank", d), ("selector_top_k", 4ul), ("attention.sliding_window", 2048ul)]) {
        kv_u32(b, "dflash.{kv._0}", kv._1)
    }
    for (kv in [("attention.layer_norm_rms_epsilon", 0x358637BDul), ("rope.freq_base", 0x49742400ul)]) {   //! 1e-6 and 1e6 as f32 bits
        gguf_push_str(b, "dflash.{kv._0}")
        gguf_push_le(b, GGUF_F32, 4)
        gguf_push_le(b, kv._1, 4)
    }
    gguf_push_str(b, "dflash.target_layers")
    gguf_push_le(b, GGUF_ARRAY, 4)
    gguf_push_le(b, GGUF_I32, 4)
    gguf_push_le(b, 1ul, 8)
    gguf_push_le(b, 2ul, 4)
    kv_u32(b, "tokenizer.ggml.mask_token_id", 3ul)
    let fc_bytes = ggml_type_bytes(fc_type, int64(d * d))
    let fc_span = (fc_bytes + 31l) & ~31l
    tensor_rec(b, "fc.weight", [d, d], fc_type, 0ul)
    tensor_rec(b, "selector_predecessor.weight", [d, 4ul], GGML_TYPE_Q8_0, uint64(fc_span))
    pad32(b)
    let data0 = long_length(b)
    b |> resize(data0 + fc_span + ggml_type_bytes(GGML_TYPE_Q8_0, int64(d) * 4l))
    return <- b
}

[test]
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
        var bytes <- synth_dflash(GGML_TYPE_IQ4_XS)
        let p = path_join(scratch_path("."), "iq4xs_dflash.gguf")
        fopen(p, "wb") $(f) {
            fwrite(f, bytes)
        }
        var dr <- load_dflash_drafter(p)
        t |> success(!dr.ok, "ok is false")
        t |> success(find(dr.why, "'fc.weight'") >= 0 && find(dr.why, "IQ4_XS") >= 0 && find(dr.why, "iq4xs") >= 0,
            "why names the site, the disk type and the member: '{dr.why}'")
        delete dr
        delete bytes
    }
    t |> run("the control: the same file with a Q4_K fc.weight loads that plane and refuses on the next, absent one") @(t : T?) {
        var bytes <- synth_dflash(GGML_TYPE_Q4_K)
        let p = path_join(scratch_path("."), "k4_dflash.gguf")
        fopen(p, "wb") $(f) {
            fwrite(f, bytes)
        }
        var dr <- load_dflash_drafter(p)
        t |> success(!dr.ok, "ok is false")
        t |> equal(dr.why, "missing tensor 'selector_hidden.weight'", "fc.weight passed the format check")
        t |> equal(dr.fc.fmt, KqFmt.k4, "fc.weight is tagged k4, its disk type")
        delete dr
        delete bytes
    }
}

//! every weight plane of the drafter at `path` tagged with the format of its disk type
def private planes_tag_disk_types(t : T?; path : string; tag : string) {
    var dr <- load_dflash_drafter(path)
    t |> equal(dr.why, "", "{tag}: the drafter loads")
    if (dr.ok) {
        var planes <- [("fc.weight", dr.fc), ("selector_hidden.weight", dr.sel_h), ("selector_predecessor.weight", dr.sel_prev),
                       ("selector_successor.weight", dr.sel_next)]
        planes |> reserve(4l + 9l * dr.n_layers)
        for (l in range64(dr.n_layers)) {
            planes |> push(("blk.{l}.attn_q.weight", dr.wq[l]))
            planes |> push(("blk.{l}.attn_k.weight", dr.wk[l]))
            planes |> push(("blk.{l}.attn_v.weight", dr.wv[l]))
            planes |> push(("blk.{l}.attn_output.weight", dr.wo[l]))
            planes |> push(("blk.{l}.ffn_gate.weight", dr.w1[l]))
            planes |> push(("blk.{l}.ffn_up.weight", dr.w3[l]))
            planes |> push(("blk.{l}.ffn_down.weight", dr.w2[l]))
            planes |> push(("blk.{l}.attn_conv_proj.weight", dr.aconv_proj[l]))
            planes |> push(("blk.{l}.ffn_conv_proj.weight", dr.fconv_proj[l]))
        }
        var off_tag = ""
        gguf_header_read(path) $(m; _bytes) {
            for (p in planes) {
                let gt = m.tensors[gguf_find_tensor(m, p._0)].ggml_type
                if (kq_ggml_type(p._1.fmt) != gt && empty(off_tag)) {
                    off_tag = "{p._0}: tagged {p._1.fmt} over disk type {ggml_type_name(gt)}"
                }
            }
        }
        t |> equal(off_tag, "", "{tag}: all {length(planes)} planes tagged with their disk type's format")
        delete planes
    }
    delete dr
}

//! the plain greedy walk against the speculative one on `m`, the drafter file `drafter` attached on the asked arm: the ids
//! equal, drafts accepted; the accepted count back for the arms' compare, the drafter left attached for the caller's witnesses
def private lossless_walk(t : T?; m : Model; drafter : string; device : bool; tag : string) : int64 {
    set_dflash_device(device)
    let why = attach_dflash_drafter(m, path_join(_model_tier::models_dir(), drafter))
    t |> equal(why, "", "{tag}: the drafter attaches")
    return 0l if (!empty(why))
    t |> equal(dflash_on_device(), device, "{tag}: the pass runs on the {device ? "device" : "CPU"}")
    let prompt <- encode(m, PROMPT, true)
    let total = long_length(prompt) - 1l + STEPS
    var s1 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    let plain <- generate(m, s1, prompt, int(total))
    var s2 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var nd = 0l
    var na = 0l
    let spec <- generate_mtp_greedy(m, s2, prompt, total, nd, na)
    t |> equal(length(spec), length(plain), "{tag}: the walks are the same length")
    var same = 0l
    while (same < long_length(plain) && same < long_length(spec) && plain[same] == spec[same]) {
        same++
    }
    t |> equal(same, long_length(plain), "{tag}: speculative == plain token for token (plain '{decode(m, plain)}', speculative '{decode(m, spec)}')")
    t |> success(na > 0l && nd >= na, "{tag}: drafts accepted {na} of {nd}")
    delete s1
    delete s2
    return na
}

//! the trunk at `path` loaded off the image rail - parsed into RAM, no `.dlim` minted or mapped beside it - and, for the Metal
//! home (`metal`), turned into the blob form the drivers serve, as the engine's own load does with the rail off
def private load_trunk(t : T?; path : string; metal : bool) : Model {
    var m <- load_model_(path, QuantMode.q8)
    if (metal) {
        t |> success(convert_model_to_metal_blob(m), "the trunk takes the metal-blob form")
    }
    return <- m
}

//! the lossless gate on the CPU arm over the planar trunk with the drafter file `drafter`
def private cpu_lossless_cell(t : T?; drafter : string; tag : string) {
    let trunk = path_join(_model_tier::models_dir(), TRUNK_FILE)
    return if (!model_available(t, trunk) || !model_available(t, path_join(_model_tier::models_dir(), drafter)))
    let prior = get_metal_mode()
    set_metal_mode(MetalMode.off)
    with_dasllama_jobque() {
        var m <- load_trunk(t, trunk, false)
        lossless_walk(t, m, drafter, false, tag)
        detach_dflash_drafter()
        delete m
    }
    set_metal_mode(prior)
}

//! `a` and `b` recorded drafts id for id, neither empty
def private same_drafts(t : T?; a, b : array<int64>; what : string) {
    var same = 0
    while (same < length(a) && same < length(b) && a[same] == b[same]) {
        same++
    }
    t |> success(!empty(a) && same == length(a) && same == length(b), "{what} id for id ({same} of {length(a)} / {length(b)} equal)")
}

let private SHALLOW_DEPTH = 3l   //! the asked round's drafts, under the block's seven

//! the device arm's greedy walk round by round at `depth` (0 = no depth set, the drafter's whole block): every drafting round
//! drafts `depth` (unset: past SHALLOW_DEPTH) and its verify runs one row more (`metal_verify_split`), the ids the plain walk's
def private depth_walk(t : T?; m : Model; drafter : string; depth : int64; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_dflash_device(true)
        let why = attach_dflash_drafter(m, path_join(_model_tier::models_dir(), drafter))
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

//! the planar Q4_K_M trunk's path; "" with the loud skip where it is absent or off the tier
def private gguf_trunk(t : T?) : string {
    let trunk = path_join(_model_tier::models_dir(), TRUNK_FILE)
    return model_available(t, trunk) ? trunk : ""
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
def private site_witnesses(t : T?; fmts, trunk_fmts : array<KqFmt>; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        for (f in fmts) {
            t |> success(metal_dflash_sites(f) > 0l, "{tag}: the device arm encoded {metal_dflash_sites(f)} sites over {f} planes")
        }
        let probe = rows8_layout_probe(t)
        let knob = g_env_metal.rows_mt
        t |> equal(metal_rows8_on(), probe && knob,
            "{tag}: the engine's rows8 gate is the layout probe's answer ({probe}) under DASLLAMA_METAL_ROWS_MT={knob}")
        return if (!probe || !knob)
        t |> success(metal_dflash_rows8_sites() > 0l, "{tag}: the rows8 form served {metal_dflash_rows8_sites()} of the device arm's sites")
        for (f in fmts) {
            t |> success(metal_dflash_rows8_sites_of(f) > 0l, "{tag}: the rows8 form served {metal_dflash_rows8_sites_of(f)} sites over {f} planes")
        }
        for (f in trunk_fmts) {
            t |> success(metal_rows8_sites_of(f) > 0l, "{tag}: the rows8 form served {metal_rows8_sites_of(f)} of the trunk's verify sites over {f} planes")
        }
    }
}

//! the lossless gate on the Metal home's device arm over the served `trunk` with the drafter file `drafter`, every format in
//! `fmts` counted among the pass's sites and the rows8 form serving where the box runs it - and every format in `trunk_fmts`
//! among the trunk's rows8 sites -, every pass's selector on the device and held for the verify, every held verify's accept on
//! the device, every reject's replay unwaited over the tape in place; the same walk with the host's accept over every landed
//! row and every replay waited, then with the host's selector over the device pass and the tape copied back,
//! each drafting the same ids and accepting the same count; then the CPU arm fed by the device's captures accepting the same count
def private metal_lossless_cell(t : T?; trunk, drafter : string; fmts, trunk_fmts : array<KqFmt>; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (empty(trunk) || !model_available(t, path_join(_model_tier::models_dir(), drafter)))
        let prior = get_metal_mode()
        set_metal_mode(MetalMode.required)
        apply_box_profile_runtime()   //! the box's crowns: the served trunk's forms, as the server applies them
        let override0 = active_decode_override()
        select_decode_override("metal")
        with_dasllama_jobque() {
            var m <- load_trunk(t, trunk, true)
            set_dflash_record(true)
            let rp0 = metal_verify_replays()
            let tape0 = metal_verify_replays_in_place()
            let dev = lossless_walk(t, m, drafter, true, "{tag} device arm")
            var inscope dev_drafts <- dflash_recorded_drafts()
            let rp1 = metal_verify_replays()
            let tape1 = metal_verify_replays_in_place()
            t |> success(rp1.total > rp0.total && rp1.unwaited - rp0.unwaited == rp1.total - rp0.total,
                "{tag}: every rejected round's recurrent replay committed unwaited ({rp1.unwaited - rp0.unwaited} of {rp1.total - rp0.total})")
            t |> equal(tape1 - tape0, rp1.total - rp0.total, "{tag}: every replay read the verify's tape in place")
            let sel = metal_dflash_selected()
            t |> success(sel.held > 0l && sel.held == sel.selected, "{tag}: the device selector ended {sel.selected} passes, {sel.held} of them held for the verify unwaited")
            t |> equal(metal_dflash_accepted(), sel.held, "{tag}: every held pass's verify landed its accept on the device")
            site_witnesses(t, fmts, trunk_fmts, tag)
            detach_dflash_drafter()
            set_metal_dflash_accept(false)
            set_metal_replay_unwaited(false)
            set_dflash_record(true)
            let host_acc = lossless_walk(t, m, drafter, true, "{tag} device arm, the host's accept, the replays waited")
            var inscope host_acc_drafts <- dflash_recorded_drafts()
            t |> success(metal_dflash_selected().held > 0l && metal_dflash_accepted() == 0l, "{tag}: the knob off, the held passes' verifies land every row for the host's accept")
            let rp2 = metal_verify_replays()
            t |> success(rp2.total > rp1.total && rp2.unwaited == rp1.unwaited, "{tag}: the replay knob off, every replay waited in the rollback seat ({rp2.total - rp1.total})")
            detach_dflash_drafter()
            set_metal_dflash_accept(true)
            set_metal_replay_unwaited(true)
            t |> equal(host_acc, dev, "{tag}: the host's accept over every landed row accepts the device accept's count")
            same_drafts(t, dev_drafts, host_acc_drafts, "{tag}: the device accept's rounds draft the host accept's")
            set_metal_dflash_select(false)
            set_metal_replay_tape(false)
            set_dflash_record(true)
            let tape2 = metal_verify_replays_in_place()
            let rp3 = metal_verify_replays()
            let host_sel = lossless_walk(t, m, drafter, true, "{tag} device arm, the host's selector, the tape copied back")
            var inscope host_drafts <- dflash_recorded_drafts()
            t |> equal(metal_dflash_selected().selected, 0l, "{tag}: the knob off, no pass ends on the device selector")
            t |> success(metal_verify_replays().total > rp3.total && metal_verify_replays_in_place() == tape2,
                "{tag}: the tape knob off, every replay copied its tape back into the stage planes")
            detach_dflash_drafter()
            set_metal_dflash_select(true)
            set_metal_replay_tape(true)
            set_dflash_record(false)
            t |> equal(host_sel, dev, "{tag}: the host's selector over the device pass accepts the device selector's count")
            same_drafts(t, dev_drafts, host_drafts, "{tag}: the device selector's drafts are the host selector's")
            let cpu = lossless_walk(t, m, drafter, false, "{tag}, the CPU arm on the device's captures")
            detach_dflash_drafter()
            t |> equal(dev, cpu, "{tag}: both arms accept the same count")
            depth_walk(t, m, drafter, SHALLOW_DEPTH, "{tag} device arm at depth {SHALLOW_DEPTH}")
            depth_walk(t, m, drafter, 0l, "{tag} device arm, no depth set")
            delete m
        }
        metal_decode_shutdown()
        metal_prefill_shutdown()
        select_decode_override(override0)
        set_metal_mode(prior)
    } else {
        t |> skip("{tag}: no das_metal in this build")
    }
}

let private SAMPLED_PARAMS = SamplingParams(temp = 0.7)
let private SAMPLED_SEED = 4242
let private SAMPLED_STEPS = 64l

//! the speculative sampled walk on `m` under `accept`, the drafter at `draft_temp`, seeded at SAMPLED_SEED
def private spec_sampled(m : Model; prompt : array<int64>; accept : MtpAccept; draft_temp : float) : MtpSampledWalk {
    set_mtp_accept(accept)
    set_dflash_draft_temp(draft_temp)
    var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_seed_(s, SAMPLED_SEED)
    var w <- generate_mtp_sampled(m, s, prompt, SAMPLED_PARAMS, SAMPLED_STEPS)
    delete s
    return <- w
}

def private accept_rate(w : MtpSampledWalk) : float => w.drafted > 0l ? float(w.accepted) / float(w.drafted) : 0.0

//! the device selector's ended passes so far (0 off the Metal build)
def private device_selections() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_dflash_selected().selected
    } else {
        return 0l
    }
}

//! the device selector's passes that drew a sampled round's drafts so far (0 off the Metal build)
def private device_soft_picks() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_dflash_soft_picked()
    } else {
        return 0l
    }
}

//! the sampled rounds whose pass held with the verify committed behind it so far (0 off the Metal build)
def private sampled_ahead_rounds() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_verify_ahead_rounds()
    } else {
        return 0l
    }
}

//! the verify-ahead level, the prior one back (0 off the Metal build)
def private set_verify_ahead(level : int) : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior = get_metal_verify_ahead()
        set_metal_verify_ahead(level)
        return prior
    } else {
        return 0
    }
}

//! the device selector's draw of a sampled round's drafts on or off (nothing off the Metal build)
def private set_device_soft_pick(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_dflash_soft_pick(on)
    }
}

//! the three acceptances over the drafter file `drafter` on `m`, the pass on the asked arm, against the plain sampled walk at the
//! same seed: exact equals it token for token with drafts accepted; each rejection walk (point-mass drafts, drafts drawn at the
//! stream's temperature) accepts some drafts and rejects some - its stream parts from the plain one at its first rejection, so
//! its rate is not the exact walk's to compare (the closed form is test_mtp_accept_walks.das's), the three rates logged; on the
//! device arm the drawn drafts drawn by the device selector, and with its soft pick off drawn on the host - the same stream
def private sampled_modes_walk(t : T?; m : Model; drafter : string; device : bool; tag : string) {
    set_dflash_device(device)
    let why = attach_dflash_drafter(m, path_join(_model_tier::models_dir(), drafter))
    t |> equal(why, "", "{tag}: the drafter attaches")
    return if (!empty(why))
    let prompt <- encode(m, PROMPT, true)
    var s1 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_seed_(s1, SAMPLED_SEED)
    var plain : array<int64>
    generate_(m, s1, prompt, SAMPLED_PARAMS, SAMPLED_STEPS) $(id : int64; _piece : string) : bool {
        plain |> push(id)
        return true
    }
    let knob = get_mtp_accept()
    let temp0 = get_dflash_draft_temp()
    let sel_before_exact = device_selections()
    var inscope exact <- spec_sampled(m, prompt, MtpAccept.exact, 0.0)
    let sel_exact = device_selections() - sel_before_exact
    var same = 0l
    while (same < long_length(plain) && same < long_length(exact.ids) && plain[same] == exact.ids[same]) {
        same++
    }
    t |> equal(length(exact.ids), length(plain), "{tag}: the exact walk is the plain walk's length")
    t |> equal(same, long_length(plain), "{tag}: exact == the plain sampled walk token for token (plain '{decode(m, plain)}', spec '{decode(m, exact.ids)}')")
    t |> success(exact.accepted > 0l, "{tag}: exact accepted {exact.accepted} of {exact.drafted}")
    var stops <- [plain[SAMPLED_STEPS / 2l]]
    let stop_at = find_index(plain, stops[0])
    set_mtp_accept(MtpAccept.exact)
    var s3 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_seed_(s3, SAMPLED_SEED)
    var inscope stopped <- generate_mtp_sampled(m, s3, prompt, SAMPLED_PARAMS, SAMPLED_STEPS, stops)
    t |> equal(length(stopped.ids), stop_at + 1, "{tag}: a stop id ends the exact walk on its first occurrence, kept as the last id (the stopless walk ran {length(exact.ids)})")
    t |> success(length(stopped.ids) == stop_at + 1 && stopped.ids[stop_at] == stops[0], "{tag}: ... the stop the stream's last id")
    delete s3
    delete stops
    var inscope point <- spec_sampled(m, prompt, MtpAccept.rejection, 0.0)
    t |> success(point.accepted > 0l && point.accepted < point.drafted,
        "{tag}: point-mass drafts accept some and reject some ({point.accepted} of {point.drafted}; exact {exact.accepted} of {exact.drafted})")
    let sel_before_soft = device_selections()
    let soft_before = device_soft_picks()
    var inscope soft <- spec_sampled(m, prompt, MtpAccept.rejection, -1.0)
    let sel_soft = device_selections() - sel_before_soft
    let soft_drawn = device_soft_picks() - soft_before
    t |> success(soft.accepted > 0l && soft.accepted < soft.drafted,
        "{tag}: drafts drawn at the stream's temperature (the knob's default) accept some and reject some ({soft.accepted} of {soft.drafted}; exact {exact.accepted} of {exact.drafted})")
    to_log(LOG_INFO, "{tag}: tokens a round exact {float(exact.accepted + exact.rounds) / float(max(exact.rounds, 1l))}, point {float(point.accepted + point.rounds) / float(max(point.rounds, 1l))}, drawn {float(soft.accepted + soft.rounds) / float(max(soft.rounds, 1l))}\n")
    to_log(LOG_INFO, "{tag}: drawn drafts' stream '{decode(m, soft.ids)}'\n")
    if (device && dflash_on_device()) {
        t |> success(sel_exact > 0l, "{tag}: the exact walk's greedy drafts end on the device selector ({sel_exact} passes)")
        t |> success(soft_drawn > 0l && soft_drawn == sel_soft, "{tag}: drawn drafts end on the device selector, which draws them ({soft_drawn} of {sel_soft} passes)")
        t |> success(sampled_ahead_rounds() > 0l, "{tag}: the sampled rounds' passes held with their verifies committed behind them ({sampled_ahead_rounds()})")
        let ahead0 = set_verify_ahead(0)
        let ahead_before = sampled_ahead_rounds()
        var inscope waited <- spec_sampled(m, prompt, MtpAccept.rejection, -1.0)
        t |> equal(sampled_ahead_rounds(), ahead_before, "{tag}: the verify-ahead knob off, no sampled round holds its pass")
        set_verify_ahead(ahead0)
        var same_waited = 0l
        while (same_waited < long_length(soft.ids) && same_waited < long_length(waited.ids) && soft.ids[same_waited] == waited.ids[same_waited]) {
            same_waited++
        }
        t |> success(same_waited == long_length(soft.ids) && length(soft.ids) == length(waited.ids) && soft.accepted == waited.accepted && soft.drafted == waited.drafted,
            "{tag}: the held sampled rounds draw the waited rounds' stream - equal for {same_waited} of {length(soft.ids)} ids, accepted {soft.accepted} / {waited.accepted} of {soft.drafted} / {waited.drafted}")
        set_device_soft_pick(false)
        let sel_before_host = device_selections()
        var inscope host_drawn <- spec_sampled(m, prompt, MtpAccept.rejection, -1.0)
        t |> equal(device_selections() - sel_before_host, 0l, "{tag}: the soft pick knob off, drawn drafts stand the device selector down")
        set_device_soft_pick(true)
        var same_soft = 0l
        while (same_soft < long_length(soft.ids) && same_soft < long_length(host_drawn.ids) && soft.ids[same_soft] == host_drawn.ids[same_soft]) {
            same_soft++
        }
        t |> success(same_soft == long_length(soft.ids) && length(soft.ids) == length(host_drawn.ids) && soft.accepted == host_drawn.accepted,
            "{tag}: the device's draws are the host's from the same rng - the streams equal for {same_soft} of {length(soft.ids)} ids, accepted {soft.accepted} / {host_drawn.accepted}")
    }
    set_mtp_accept(knob)
    set_dflash_draft_temp(temp0)
    detach_dflash_drafter()
    delete plain
    delete s1
}

[test]
def test_dflash_27b_sampled_cpu(t : T?) {
    if (!arm_on(t, "dflash-27b-sampled-cpu") || !family_on(t, "qwen35")) {
        return
    }
    let trunk = path_join(_model_tier::models_dir(), TRUNK_FILE)
    return if (!model_available(t, trunk) || !model_available(t, path_join(_model_tier::models_dir(), DRAFTER_FILE)))
    let prior = get_metal_mode()
    set_metal_mode(MetalMode.off)
    with_dasllama_jobque() {
        var m <- load_trunk(t, trunk, false)
        sampled_modes_walk(t, m, DRAFTER_FILE, false, "sampled cpu")
        delete m
    }
    set_metal_mode(prior)
}

//! the three acceptances on the Metal home's device arm over the served `trunk` with the Q8_0 drafter
def private sampled_metal_cell(t : T?; trunk : string; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (empty(trunk) || !model_available(t, path_join(_model_tier::models_dir(), DRAFTER_FILE)))
        let prior = get_metal_mode()
        set_metal_mode(MetalMode.required)
        apply_box_profile_runtime()
        let override0 = active_decode_override()
        select_decode_override("metal")
        with_dasllama_jobque() {
            var m <- load_trunk(t, trunk, true)
            sampled_modes_walk(t, m, DRAFTER_FILE, true, tag)
            delete m
        }
        metal_decode_shutdown()
        metal_prefill_shutdown()
        select_decode_override(override0)
        set_metal_mode(prior)
    } else {
        t |> skip("{tag}: no das_metal in this build")
    }
}

[test]
def test_dflash_27b_sampled_metal(t : T?) {
    if (!arm_on(t, "dflash-27b-sampled-metal") || !family_on(t, "qwen35")) {
        return
    }
    sampled_metal_cell(t, gguf_trunk(t), "sampled metal")
}

[test]
def test_dflash_27b_mlx_sampled(t : T?) {
    if (!arm_on(t, "dflash-27b-mlx-sampled") || !family_on(t, "qwen35")) {
        return
    }
    sampled_metal_cell(t, snapshot_trunk(t), "mlx sampled metal")
}

[test]
def test_dflash_27b_cpu(t : T?) {
    if (!arm_on(t, "dflash-27b-cpu") || !family_on(t, "qwen35")) {
        return
    }
    cpu_lossless_cell(t, DRAFTER_FILE, "cpu")
}

[test]
def test_dflash_27b_k4_cpu(t : T?) {
    if (!arm_on(t, "dflash-27b-k4-cpu") || !family_on(t, "qwen35")) {
        return
    }
    let drafter = path_join(_model_tier::models_dir(), DRAFTER_K4_FILE)
    return if (!model_available(t, drafter))
    planes_tag_disk_types(t, drafter, "k4 cpu")
    cpu_lossless_cell(t, DRAFTER_K4_FILE, "k4 cpu")
}

[test]
def test_dflash_27b_metal(t : T?) {
    if (!arm_on(t, "dflash-27b-metal") || !family_on(t, "qwen35")) {
        return
    }
    metal_lossless_cell(t, gguf_trunk(t), DRAFTER_FILE, [KqFmt.q8], array<KqFmt>(), "metal")
}

[test]
def test_dflash_27b_mlx_metal(t : T?) {
    if (!arm_on(t, "dflash-27b-mlx-metal") || !family_on(t, "qwen35")) {
        return
    }
    metal_lossless_cell(t, snapshot_trunk(t), DRAFTER_FILE, [KqFmt.q8], [KqFmt.mlx4b], "mlx metal")
}

[test]
def test_dflash_27b_k4_metal(t : T?) {
    if (!arm_on(t, "dflash-27b-k4-metal") || !family_on(t, "qwen35")) {
        return
    }
    let drafter = path_join(_model_tier::models_dir(), DRAFTER_K4_FILE)
    return if (!model_available(t, drafter))
    planes_tag_disk_types(t, drafter, "k4 metal")
    metal_lossless_cell(t, gguf_trunk(t), DRAFTER_K4_FILE, [KqFmt.k4, KqFmt.k6], array<KqFmt>(), "k4 metal")
}
