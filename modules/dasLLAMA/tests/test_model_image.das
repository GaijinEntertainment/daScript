options gen2
options persistent_heap   // + explicit deletes: each arm frees its model before the next loads
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _function_length = 0   // test bodies enumerate the fixture arms

require dastest/testing_boost public
require dasllama/dasllama            // the facade: load_model's image cache is the surface under test
require dasllama/dasllama_whisper
require dasllama/dasllama_gguf       // rd_u32/rd_u16 — the wav fixture reader
require dasllama/dasllama_image
require dasllama/dasllama_plane   // PlaneF — the served side of the round-trip comparisons
require dasllama/dasllama_canary  // CanaryStaging/stage_canary_encoder — the off-rail truth read
require dasllama/dasllama_gemma4a // Gemma4aStaging/stage_gemma4a_encoder — same, for the gemma4a arm
require dasllama/dasllama_gemma4uv // Gemma4uvStaging/stage_gemma4uv_embedder — same, for the vision arm
require dasllama/dasllama_parakeet // ParakeetStaging/stage_parakeet_model — same, for the parakeet arm
require dasllama/dasllama_config     // DlimConfiguration — the peek/round-trip mechanics arms
require dasllama/dasllama_math   // pin_kernel_backend — the metal-flavor arm's donor contract
require ?das_metal dasllama/dasllama_metal_tower   // set_metal_tower/metal_tower_stats — the gemma4uv-metal arm
require ?das_metal dasllama/dasllama_metal_prefill  // metal_prefill_shutdown — the mtower re-arm leg
require ?das_metal dasllama/dasllama_metal_asr_dec  // set_metal_wdec/metal_wdec_stats — the cross-KV gate + the tower cell's pin
require ?das_metal dasllama/dasllama_metal_decode   // metal_decode_shutdown — same
require dasllama/dasllama_layout // convert_model_to_metal_blob — the derive cell's in-RAM control
require daslib/jobque_boost
require daslib/archive
require daslib/apply
require daslib/fio
require strings
require math
require _model_tier

// The prepared-model image (.dlim) rail. The mechanics arm exercises save/load on a synthetic
// carrier (no models, runs everywhere incl. CI): value round trip, dotted nested-carrier
// planes, string-array meta rail, every decline shape, and the unmap-remap cycle. The model
// arms compare a gguf-backed load against a mapped image load of the same file — token-exact
// for text models, element-exact planes for towers, transcript-exact for whisper. The
// voxtral-mini arm is the designated >2 GiB-plane coverage (its q8 image is 5.4 GB), so it
// re-saves the image from cold on every run by design. Drive through modules/dasLLAMA/tests/run.das
// (--suite image); model arms follow the suite's presence/tier gates.

// element-exact plane compare: -1 = length mismatch, else the number of differing elements
// the staged (owned array) side against the served (borrowed plane) side — the only comparison
// that can prove an image round-trips, now that a served carrier has no array form
def private count_mismatch(a : array<float>; b : PlaneF) : int64 {
    if (long_length(a) != length(b)) {
        return -1l
    }
    var bad = 0l
    for (i in range64(long_length(a))) {
        if (a[i] != b[i]) {
            bad++
        }
    }
    return bad
}

def private count_mismatch_i8(a : array<int8>; b : PlaneI8) : int64 {
    if (long_length(a) != length(b)) {
        return -1l
    }
    var bad = 0l
    for (i in range64(long_length(a))) {
        if (a[i] != b[i]) {
            bad++
        }
    }
    return bad
}

def private count_mismatch_u16(a : array<uint16>; b : PlaneU16) : int64 {
    if (long_length(a) != length(b)) {
        return -1l
    }
    var bad = 0l
    for (i in range64(long_length(a))) {
        if (a[i] != b[i]) {
            bad++
        }
    }
    return bad
}

def private count_mismatch(a, b : array<auto(TT)>) : int64 {
    if (long_length(a) != long_length(b)) {
        return -1l
    }
    var bad = 0l
    for (x, y in a, b) {
        if (x != y) {
            bad++
        }
    }
    return bad
}

// ===== the synthetic carrier (mechanics arm) =====

struct ImgProbeInner {
    image_map : void?    // the nested-carrier marker: planes join the outer image as "nested.blob"
    image_bytes : uint64
    k : int64
    blob : array<float>
}

def finalize(var t : ImgProbeInner) {
    apply(t) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            if (lock_count(field) != 0) {
                unsafe {
                    _builtin_forget_temp_array(field)
                }
            } else {
                delete field
            }
        }
    }
}

struct ImgProbe {
    tagv : int
    plane_f : array<float>
    labels : array<string>   // string-array rail: must ride the meta blob, never a raw plane
    n : int64
    nested : ImgProbeInner = ImgProbeInner()
    plane_b : array<int8>
    name : string
    image_map : void?
    image_bytes : uint64
}

def finalize(var t : ImgProbe) {
    apply(t) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            if (lock_count(field) != 0) {
                unsafe {
                    _builtin_forget_temp_array(field)
                }
            } else {
                delete field
            }
        } static_elif (typeinfo is_struct(field)) {
            delete field
        }
    }
    if (t.image_map != null) {
        unsafe {
            fmap_close(t.image_map, t.image_bytes)
        }
        t.image_map = null
        t.image_bytes = 0ul
    }
}

def private serialize_image_meta(var arch : Archive; var t : ImgProbe) {
    arch |> serialize_raw(t.tagv)
    arch |> serialize_raw(t.n)
    arch |> serialize(t.name)
    serialize_strings(arch, t.labels)
    arch |> serialize_raw(t.nested.k)
}

[unused_argument(t)]
def private image_post_load(var t : ImgProbe) {}

def private make_probe() : ImgProbe {
    var p = ImgProbe(tagv = 7, n = 12345l, name = "probe")
    p.labels <- ["alpha", "", "omega"]
    p.plane_f |> resize(1000)
    for (i in range(1000)) {
        p.plane_f[i] = float(i) * 0.25
    }
    p.plane_b |> resize(4099)   // deliberately page-unaligned byte count
    for (i in range(4099)) {
        p.plane_b[i] = int8(i % 251 - 125)
    }
    p.nested.k = 99l
    p.nested.blob |> resize(2048)
    for (i in range(2048)) {
        p.nested.blob[i] = 1.0 / float(i + 1)
    }
    return <- p
}

def private read_u32_at(t : T?; path : string; at : int64) : uint {
    var v = 0u
    let f = fopen(path, "rb")
    if (f == null) {
        t |> failure("cannot open {path} for reading")
        return v
    }
    fseek(f, at, seek_set)
    var b : array<uint8>
    b |> resize(4)
    f |> fread(b)
    for (i in range(4)) {
        v |= uint(b[i]) << uint(i * 8)
    }
    delete b
    fclose(f)
    return v
}

def private patch_u32(t : T?; path : string; at : int64; v : uint) {
    let f = fopen(path, "r+b")
    if (f == null) {
        t |> failure("cannot reopen {path} for patching")
        return
    }
    fseek(f, at, seek_set)
    var b : array<uint8>
    b |> resize(4)
    for (i in range(4)) {
        b[i] = uint8((v >> uint(i * 8)) & 0xFFu)
    }
    f |> fwrite(b)
    delete b
    fclose(f)
}

[test]
def test_image_mechanics(t : T?) {
    t |> run("synthetic carrier: round trip, nested planes, declines, unmap-remap") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        var terr : string
        let tmp = temp_directory(terr)   // portable (no /tmp on Windows); cwd as a last resort
        let path = path_join(empty(tmp) ? "." : tmp, "dasllama_img_probe_{ref_time_ticks()}.dlim")
        var p <- make_probe()
        t |> success(save_image(p, path, "img-probe"), "save")
        t |> success(stat(path).is_valid, "file exists")
        t |> equal(stat(path).size % uint64(IMAGE_PAGE), 0ul, "file is a page multiple")

        var m = ImgProbe()
        let epoch0 = weights_epoch()
        t |> success(load_image(path, m, "img-probe"), "load")
        // the Metal drivers' weight caches key by HOST ADDRESS and flush on the weights epoch;
        // an image load that lands planes at a deleted model's recycled addresses without
        // moving the epoch makes them serve the dead model's buffers (arm10-kq, 2026-07-16)
        t |> success(weights_epoch() > epoch0, "an image load moves the weights epoch")
        t |> success(m.image_map != null, "image-backed")
        t |> equal(m.tagv, p.tagv)
        t |> equal(m.n, p.n)
        t |> equal(m.name, p.name)
        t |> equal(length(m.labels), 3)
        t |> equal(m.labels[0], "alpha")
        t |> equal(m.labels[1], "")
        t |> equal(m.labels[2], "omega")
        t |> equal(m.nested.k, 99l)
        t |> success(m.nested.image_map == null, "the OUTER struct owns the mapping")
        t |> equal(count_mismatch(m.plane_f, p.plane_f), 0l, "plane_f element-exact")
        t |> equal(count_mismatch(m.plane_b, p.plane_b), 0l, "plane_b element-exact")
        t |> equal(count_mismatch(m.nested.blob, p.nested.blob), 0l, "nested.blob element-exact")
        delete m   // forgets the borrowed planes + unmaps

        var m2 = ImgProbe()
        t |> success(load_image(path, m2, "img-probe"), "re-map after unmap")
        t |> equal(long_length(m2.nested.blob), 2048l)
        delete m2

        t |> success(save_image(p, path, "img-probe"), "re-save over an existing image")
        var m2b = ImgProbe()
        t |> success(load_image(path, m2b, "img-probe"), "re-saved image maps")
        delete m2b

        // the v7 meta head: peek reads version + identity + embedded config WITHOUT loading,
        // and the embedded config re-derives the exact identity the image was baked under
        let pk = image_peek(path)
        t |> success(pk.valid, "peek: header sane")
        t |> equal(pk.version, IMAGE_VERSION, "peek: version")
        t |> equal(pk.identity, image_identity("img-probe"), "peek: baked identity string")
        var pcfg = DlimConfiguration()
        t |> success(dlim_config_from_json(pk.config_json, pcfg), "peek: embedded config JSON parses")
        t |> equal(dlim_identity(pcfg, IMAGE_VERSION, "img-probe"), pk.identity, "peek: config re-derives the identity")

        var m3 = ImgProbe()
        t |> success(!load_image(path, m3, "wrong-tag"), "identity mismatch declines")
        patch_u32(t, path, 4l, 0xDEADu)
        var m4 = ImgProbe()
        t |> success(!load_image(path, m4, "img-probe"), "version mismatch declines")
        patch_u32(t, path, 4l, uint(IMAGE_VERSION))
        var m5 = ImgProbe()
        t |> success(load_image(path, m5, "img-probe"), "restored version maps again")
        delete m5
        let mb = read_u32_at(t, path, 24l)
        patch_u32(t, path, 24l, mb - 4u)   // truncate the meta tail: the archive under-reads its last field
        var m5c = ImgProbe()
        t |> success(!load_image(path, m5c, "img-probe"), "truncated meta blob declines")
        patch_u32(t, path, 24l, 0u)   // an empty meta blob is malformed, not an OOB addr
        var m5d = ImgProbe()
        t |> success(!load_image(path, m5d, "img-probe"), "zero-size meta blob declines")
        patch_u32(t, path, 24l, mb)
        patch_u32(t, path, 12l, 7u)   // header now lies about the section count
        var m5b = ImgProbe()
        t |> success(!load_image(path, m5b, "img-probe"), "section-count-lying header declines")
        patch_u32(t, path, 12l, 3u)   // the probe's true count: plane_f, plane_b, nested.blob
        patch_u32(t, path, 32l, 0x1u)   // header now lies about the file size
        var m6 = ImgProbe()
        t |> success(!load_image(path, m6, "img-probe"), "size-lying header declines")
        t |> success(remove(path), "probe image removed")   // also makes the absent-decline below honest
        var m7 = ImgProbe()
        t |> success(!load_image(path, m7, "img-probe"), "absent file declines")
        delete p

        // an OWNED array that merely holds a lock has the same lock/magic signature as a
        // borrowed view — forget must refuse it loudly (silently "forgetting" leaks storage)
        var owned : array<float>
        owned |> resize(4)
        var caught = false
        for (x in owned) {
            feint("{x}")
            try {
                unsafe {
                    _builtin_forget_temp_array(owned)
                }
            } recover {
                caught = true
            }
            break
        }
        t |> success(caught, "forget on an owned locked array is a loud error")
        delete owned
    }
    t |> run("synthetic carrier: the cold rail builds in memory, tees to disk, serves the chunk") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        var terr : string
        let tmp = temp_directory(terr)
        let path = path_join(empty(tmp) ? "." : tmp, "dasllama_img_chunk_{ref_time_ticks()}.dlim")
        var truth <- make_probe()   // the off-rail reference, untouched by the build
        var src <- make_probe()
        var cold = ImgProbe()
        let epoch0 = weights_epoch()
        t |> success(image_from_carrier(src, cold, path, "img-probe"), "cold build + adopt")
        t |> success(cold.image_map != null, "the cold load serves an image, not the carrier")
        t |> success(weights_epoch() > epoch0, "a chunk adopt moves the weights epoch too")
        t |> success(long_length(src.plane_f) == 0l, "the build consumed the source planes")
        delete src
        t |> equal(cold.tagv, truth.tagv)
        t |> equal(cold.name, truth.name)
        t |> equal(length(cold.labels), 3)
        t |> equal(cold.nested.k, 99l)
        t |> equal(count_mismatch(cold.plane_f, truth.plane_f), 0l, "chunk plane_f element-exact")
        t |> equal(count_mismatch(cold.nested.blob, truth.nested.blob), 0l, "chunk nested.blob element-exact")

        // the tee: what landed on disk must be the very image being served
        t |> success(stat(path).is_valid, "the cold build wrote the .dlim")
        t |> equal(stat(path).size, cold.image_bytes, "the file is exactly the chunk")
        var warm = ImgProbe()
        t |> success(load_image(path, warm, "img-probe"), "the tee'd file maps")
        t |> equal(count_mismatch(warm.plane_f, cold.plane_f), 0l, "mapped vs chunk plane_f")
        t |> equal(count_mismatch(warm.nested.blob, cold.nested.blob), 0l, "mapped vs chunk nested.blob")
        delete warm
        delete cold   // frees the chunk (image_chunk_free), not an unmap
        remove(path)

        // no path = no file: the image still gets built and served, nothing lands on disk
        var src2 <- make_probe()
        var mem_only = ImgProbe()
        t |> success(image_from_carrier(src2, mem_only, "", "img-probe"), "build with no save target")
        t |> success(mem_only.image_map != null, "still image-backed")
        t |> equal(count_mismatch(mem_only.plane_f, truth.plane_f), 0l, "memory-only plane_f element-exact")
        delete src2
        delete mem_only
        delete truth
    }
    t |> run("DlimConfiguration JSON round-trips with the identity intact") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let dc = dlim_config_current("q8")
        let js = dlim_config_json(dc)
        t |> success(!empty(js), "config serializes")
        var back = DlimConfiguration()
        t |> success(dlim_config_from_json(js, back), "JSON parses back")
        t |> equal(dlim_identity(back, IMAGE_VERSION), dlim_identity(dc, IMAGE_VERSION), "identity survives the round trip")
        var junk = DlimConfiguration()
        t |> success(!dlim_config_from_json("not a config", junk), "garbage declines")
    }
}

// ===== real carriers: gguf load vs mapped image load =====

// greedy continuation ids on a loaded model (no kernel pins: both sides of every compare run
// in this process with identical knobs, so determinism is per-run, not per-fixture)
def private gen_ids(tr : Model; prompt : array<int64>; n_gen : int) : array<int64> {
    var out : array<int64>
    with_job_que() {
        setup_dasllama_jobque()
        var s = make_run_state(tr.config)
        var ids <- generate(tr, s, prompt, length(prompt) + n_gen)
        out <- ids
        delete s
    }
    return <- out
}

// first config field whose value differs between two loads ("" = none) — catches an arch
// configure clobbering serialized gguf overrides even when short gens can't (sliding_window
// only bites past the window)
def private config_diff_field(a, b : Config) : string {
    var names : array<string>
    var vals : array<string>
    apply(a) $(name : string; field) {
        names |> push(name)
        vals |> push("{field}")
    }
    var bad = ""
    var i = 0
    apply(b) $ [unused_argument(name)] (name : string; field) {
        if (empty(bad) && "{field}" != vals[i]) {
            bad = "{names[i]}: gguf={vals[i]} image={field}"
        }
        i++
    }
    delete names
    delete vals
    return bad
}

def private text_image_roundtrip(t : T?; path : string; n_gen : int) {
    let img = image_path_for(path)
    remove(img)   // cold start: the first facade load must build + save
    // the truth is the OFF-RAIL gguf read: cold and warm are both image-backed now, so a
    // cold-vs-warm compare alone would only prove the file matches the chunk it came from
    var g <- load_model_(path, QuantMode.q8)
    var cfg_a := g.config   // kept past delete for the round-trip compare
    if (g.config.seq_len > 2048l) {
        g.config.seq_len = 2048l
    }
    var prompt <- encode(g, "Once upon a time")
    var ga <- gen_ids(g, prompt, n_gen)
    delete g
    var a <- load_model(path, QuantMode.q8)
    t |> success(a.image_map != null, "cold load serves the image it built")
    t |> success(stat(img).is_valid, "cold load saved an image")
    if (a.config.seq_len > 2048l) {
        a.config.seq_len = 2048l
    }
    var gcold <- gen_ids(a, prompt, n_gen)
    t |> success(!empty(ga) && count_mismatch(ga, gcold) == 0l, "gguf vs cold-built image tokens")
    delete gcold
    delete a
    var b <- load_model(path, QuantMode.q8)
    t |> success(b.image_map != null, "warm load is image-backed")
    let cfg_diff = config_diff_field(cfg_a, b.config)
    t |> success(empty(cfg_diff), "config round-trips exactly ({cfg_diff})")
    delete cfg_a
    // the image reader rebuilds the tokenizer's DERIVED tables (pair_merge / byte_id / bkey_id);
    // if either build_*_derived call were dropped, encode would silently emit fallback ids while
    // the forced-feed compares below stay green — so encode through the image-loaded model itself
    var pb <- encode(b, "Once upon a time")
    t |> success(long_length(pb) == long_length(prompt) && count_mismatch(prompt, pb) == 0l,
        "image-loaded tokenizer encodes byte-identically to the gguf-loaded one")
    delete pb
    if (b.config.seq_len > 2048l) {
        b.config.seq_len = 2048l
    }
    var gb <- gen_ids(b, prompt, n_gen)
    t |> success(!empty(ga) && count_mismatch(ga, gb) == 0l, "gguf vs image tokens")
    delete b   // unmaps
    var c <- load_model(path, QuantMode.q8)   // the file survives an unmap — map again
    t |> success(c.image_map != null, "re-mapped after delete")
    if (c.config.seq_len > 2048l) {
        c.config.seq_len = 2048l
    }
    var gc <- gen_ids(c, prompt, n_gen)
    t |> success(count_mismatch(ga, gc) == 0l, "re-mapped tokens")
    delete c
    delete ga
    delete gb
    delete gc
    delete prompt
}

[test]
def test_model_image_roundtrip(t : T?) {
    t |> run("SmolLM2-135M q8: gguf load vs mapped image, token-for-token") @(t : T?) {
        if (!family_on(t, "llama") || !arm_on(t, "smol")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        text_image_roundtrip(t, path, 24)
    }
}

// ===== the metal (blob-only) flavor =====

// counting prompt: confident continuations — the decode-parity suite's CPU-vs-GPU token bar
def private counting_ids(tr : Model) : array<int64> {
    return <- encode(tr, "The numbers from one to ten are: one, two, three,")
}

// Teacher-forced max-logits-diff: both models step through the SAME token sequence and the
// per-step logits compare — the valid cross-backend instrument for a 135M (greedy token
// equality is not: the model rides genuine near-ties that flip on ~0.02 gaps, while real
// breakage moves logits 15+; measured cross-backend noise here is ~0.75). With metal mode
// on, the planar model CPU-serves (the gates decline `planar`) and the blob model GPU-serves
// — no override juggling.
def private top_id(lg : array<float>) : int64 {
    var best = 0l
    for (v in range64(1l, long_length(lg))) {
        if (lg[v] > lg[best]) {
            best = v
        }
    }
    return best
}

def private forced_logits_maxd(tc, tg : Model; prompt, teach : array<int64>; tag : string) : float {
    var maxd = 0.0
    var picks : array<int64>   // the GPU's greedy would-be choice per teach position — eyeball rail
    with_job_que() {
        setup_dasllama_jobque()
        var sa = make_run_state(tc.config)
        var sb = make_run_state(tg.config)
        eval_(tc, sa, prompt)
        eval_(tg, sb, prompt)
        for (a, b in sa.logits, sb.logits) {
            maxd = max(maxd, abs(a - b))
        }
        for (tok in teach) {
            picks |> push(top_id(sb.logits))
            var one : array<int64>
            one |> push(tok)
            eval_(tc, sa, one)
            eval_(tg, sb, one)
            for (a, b in sa.logits, sb.logits) {
                maxd = max(maxd, abs(a - b))
            }
            delete one
        }
        delete sa
        delete sb
    }
    to_log(LOG_INFO, "{tag} eyeball - prompt: {eyeball_text(tc, prompt)}\n")
    to_log(LOG_INFO, "{tag} eyeball - CPU cont: {eyeball_text(tc, teach)} | GPU greedy-would-be: {eyeball_text(tc, picks)}\n")
    delete picks
    return maxd
}

[test]
def test_metal_flavor_image(tst : T?) {
    tst |> run("SmolLM2 q8: metal flavor - blob mint + map + GPU serve + CPU tripwire") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "llama") || !arm_on(t, "metal")) return
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            if (!has_decode_override("metal")) {
                t |> skip("metal overrides not registered")
                return
            }
            let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            // CPU control on the portable backend (the metal donor contract) — planar flavor
            let prior_backend = active_kernel_backend()
            pin_kernel_backend("portable")
            set_metal_mode(MetalMode.off)
            var tc <- load_model(path, QuantMode.q8)
            t |> success(!tc.metal_blob, "CPU-mode load stays planar")
            var prompt <- counting_ids(tc)
            var g_cpu <- gen_ids(tc, prompt, 16)
            delete tc
            // cold metal mint: the blob flavor regenerates + saves under its own identity tag
            let img_m = image_path_for(path, METAL_IMAGE_TAG)
            remove(img_m)
            set_metal_mode(MetalMode.when_available)
            var tm <- load_model(path, QuantMode.q8)
            t |> success(tm.metal_blob, "metal-mode load is blob-form")
            t |> success(tm.image_map != null, "cold metal load serves the image it built")
            t |> success(stat(img_m).is_valid, "cold metal load saved the metal-flavor image")
            t |> success(empty(tm.qblob), "planar q8 planes are gone")
            var g_mint <- gen_ids(tm, prompt, 16)   // a blob model CANNOT silently CPU-serve —
                                                    // completing at all proves the GPU served
            // teacher-forced parity vs a fresh planar CPU control (engine entry, no image
            // cache) — probe-measured noise ~0.75, breakage class 15+; 4.0 = ~4x SNR
            var tp <- load_model_(path, QuantMode.q8)
            // generate's echo is np-1 tokens (BOS dropped), so the continuation starts at
            // np-1 — slicing from np skipped the first generated token and denatured the feed
            let np = long_length(prompt)
            var teach <- [for (i in range64(np - 1l, long_length(g_cpu))); g_cpu[i]]
            let maxd = forced_logits_maxd(tp, tm, prompt, teach, "SmolLM2 metal forced")
            t |> success(maxd < 4.0, "teacher-forced GPU logits within 4.0 of the CPU control (maxd {maxd})")
            delete teach
            delete tp
            delete tm
            // warm map + the direct-.dlim path form (both flavors' identities probed)
            var tw <- load_model(path, QuantMode.q8)
            t |> success(tw.image_map != null && tw.metal_blob, "warm metal load maps the blob image")
            var g_map <- gen_ids(tw, prompt, 16)
            t |> success(count_mismatch(g_mint, g_map) == 0l, "mint vs mapped tokens")
            delete tw
            var td <- load_model(img_m, QuantMode.q8)
            t |> success(td.metal_blob, "direct .dlim load resolves the metal identity")
            var g_dir <- gen_ids(td, prompt, 16)
            t |> success(count_mismatch(g_mint, g_dir) == 0l, "direct-path tokens")
            // the CPU tripwire: with the overrides off, any inference on a blob model panics
            set_metal_mode(MetalMode.off)
            var tripped = false
            try {
                var g_bad <- gen_ids(td, prompt, 2)
                delete g_bad
            } recover {
                tripped = true
            }
            t |> success(tripped, "CPU inference on a blob model panics")
            delete td
            delete g_cpu
            delete g_mint
            delete g_map
            delete g_dir
            delete prompt
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
        }
    }
}

// The blob plane exists on two rails: the in-RAM transform builds it, the image writer derives it
// band by band from the planar planes so a cold metal mint never holds a second copy. They must
// agree byte for byte — this is the only thing keeping the cheap rail honest.
[test]
def test_metal_flavor_derive(tst : T?) {
    tst |> run("SmolLM2 q8: the derived blob plane equals the in-RAM transform's") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "llama") || !arm_on(t, "metal")) return
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            let prior_backend = active_kernel_backend()
            pin_kernel_backend("portable")
            // the control: the in-RAM transform, which is what the writer must reproduce
            var tr <- load_model_(path, QuantMode.q8)
            t |> success(convert_model_to_metal_blob(tr), "in-RAM metal-blob transform")
            // the derived image: a cold metal mint writes the plane straight out of the planar ones
            let img_m = image_path_for(path, METAL_IMAGE_TAG)
            remove(img_m)
            set_metal_mode(MetalMode.when_available)
            var tm <- load_model(path, QuantMode.q8)
            t |> success(tm.image_map != null && tm.metal_blob, "cold metal mint serves a derived blob image")
            t |> equal(long_length(tm.mblob), long_length(tr.mblob), "blob plane length")
            t |> equal(count_mismatch(tr.mblob, tm.mblob), 0l, "derived blob plane is byte-identical")
            // the kq scale strips take their device form on the same rail (empty on a pure q8)
            t |> equal(count_mismatch(tr.k4s, tm.k4s), 0l, "k4 scale strip")
            t |> equal(count_mismatch(tr.k5s, tm.k5s), 0l, "k5 scale strip")
            t |> equal(count_mismatch(tr.k6s, tm.k6s), 0l, "k6 scale strip")
            delete tm
            delete tr
            set_metal_mode(MetalMode.off)
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
        }
    }
}

[test]
def test_voxtral_mini_image(t : T?) {
    t |> run("voxtral-mini q8: the >2 GiB-plane image round trip") @(t : T?) {
        if (!family_on(t, "voxtral") || !arm_on(t, "voxtral")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "voxtral-mini-3b-q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        text_image_roundtrip(t, path, 12)
        // the arm exists for the 64-bit plane IO: the q8 image must actually cross 2 GiB
        t |> success(stat(image_path_for(path)).size > 0x80000000ul, "image crosses 2 GiB")
    }
}

[test]
def test_gemma4_image_roundtrip(t : T?) {
    t |> run("gemma-4-E2B q8: arch-configure fallbacks must not clobber mapped config") @(t : T?) {
        // gemma4's configure sets gguf-overridable fallbacks (sliding_window, swa_pattern);
        // re-running it on an image load stomped the serialized overrides (caught 2026-07-16:
        // token drift on the mapped E2B while every llama-arch carrier round-tripped clean)
        if (!family_on(t, "gemma") || !arm_on(t, "gemma")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        text_image_roundtrip(t, path, 16)
    }
}

[test]
def test_tower_image_roundtrip(t : T?) {
    t |> run("ultravox-1b tower: mapped image planes are element-exact vs gguf") @(t : T?) {
        if (!family_on(t, "ultravox") || !arm_on(t, "tower")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "mmproj-ultravox-1b-f32.gguf")
        if (!model_available(t, path)) {
            return
        }
        let img = image_path_for(path, "tower-f32")
        remove(img)
        // the truth is the OFF-RAIL staged read: cold and warm are both image-backed now, so a
        // cold-vs-warm compare alone would only prove the file matches the chunk it came from
        var inscope a <- stage_audio_tower(path)
        var inscope cold <- load_audio_tower(path)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> equal(count_mismatch(a.fblob, cold.fblob), 0l, "cold-built fblob element-exact")
        var inscope b <- load_audio_tower(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> equal(b.d_model, a.e.d_model)
        t |> equal(b.n_layer, a.e.n_layer)
        t |> equal(b.n_ff, a.e.n_ff)
        t |> equal(b.n_ctx, a.e.n_ctx)
        t |> equal(b.proj_dim, a.e.proj_dim)
        t |> equal(b.stack_factor, a.e.stack_factor)
        t |> equal(b.mm1_out, a.e.mm1_out)
        t |> success(b.proj_kind == a.e.proj_kind, "proj_kind")
        t |> equal(b.layers_off, a.e.layers_off)
        t |> equal(b.layer_stride, a.e.layer_stride)
        t |> equal(count_mismatch(a.fblob, b.fblob), 0l, "fblob element-exact")
    }
}

def private whisper_transcribe_text(path : string; samples : array<float>; cached : bool = true) : tuple<text : string; mapped : bool> {
    var inscope w = WhisperModel()
    if (cached) {
        w <- load_whisper_model(path)   // nolint:PERF030 — fresh default-constructed target, single move
    } else {
        // the control: a staged read minted straight into memory — no shared bytes, no shared file
        var inscope stg <- stage_whisper_model(path)
        mint_whisper_model(stg, w)
    }
    var text = ""
    with_job_que() {
        setup_dasllama_jobque()
        var s <- create_session(w)
        transcribe(w, s, samples) $(seg) {
            text += seg.text
        }
        delete s
    }
    let mapped = w.image_map != null
    return (text = text, mapped = mapped)
}

[test]
def test_whisper_image_roundtrip(t : T?) {
    t |> run("whisper tiny: gguf load vs mapped image, transcript-exact") @(t : T?) {
        if (!family_on(t, "whisper") || !arm_on(t, "whisper")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(whisper_dir(), "ggml-tiny.bin")
        if (!model_available(t, path)) {
            return
        }
        var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        let img = image_path_for(path, "whisper-f32")
        remove(img)
        // off-rail truth: both cached loads are image-backed, so they cannot check each other
        let g = whisper_transcribe_text(path, samples, cached = false)
        let a = whisper_transcribe_text(path, samples)
        t |> success(a.mapped, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        let b = whisper_transcribe_text(path, samples)
        t |> success(b.mapped, "warm load is image-backed")
        // every served model is image-backed now; the control differs by SINK, not by form —
        // it is minted into memory and never touches the canonical .dlim
        to_log(LOG_INFO, "whisper image-roundtrip jfk eyeball: '{g.text}'\n")
        t |> success(g.mapped && !empty(g.text), "non-empty transcript from the in-memory mint")
        t |> success(g.text == a.text, "in-memory mint vs cold-built image transcript")
        t |> success(g.text == b.text, "in-memory mint vs mapped image transcript")
        // streamed == eager, plane for plane: the cold mint above STREAMED the big planes from
        // the source mapping; the staged read materializes them. Same bytes or the stream lied.
        var inscope stg <- stage_whisper_model(path)
        var inscope w32 <- load_whisper_model(path)
        t |> equal(count_mismatch(stg.dblob, w32.dblob), 0l, "streamed dblob element-exact vs staged")
        t |> equal(count_mismatch(stg.enc.fblob, w32.enc.fblob), 0l, "streamed enc.fblob element-exact vs staged")
        t |> equal(count_mismatch(stg.filters, w32.filters), 0l, "filters element-exact")
        // and the q8 flavor — quantize+repack per streamed job must match the eager whole-blob pass
        let img8 = image_path_for(path, "whisper-q8")
        remove(img8)
        var inscope stg8 <- stage_whisper_model(path, true)
        var inscope w8 <- load_whisper_model(path, true)
        t |> success(w8.image_map != null, "q8 cold load serves the image it streamed")
        t |> equal(count_mismatch_i8(stg8.dqblob, w8.dqblob), 0l, "streamed dqblob element-exact vs staged")
        t |> equal(count_mismatch(stg8.dqscales, w8.dqscales), 0l, "streamed dqscales element-exact vs staged")
        t |> equal(count_mismatch_i8(stg8.enc.qblob, w8.enc.qblob), 0l, "streamed enc.qblob element-exact vs staged")
        t |> equal(count_mismatch(stg8.enc.qscales, w8.enc.qscales), 0l, "streamed enc.qscales element-exact vs staged")
        t |> equal(count_mismatch(stg8.dblob, w8.dblob), 0l, "q8 flavor's fp32 dblob element-exact too")
        // and the MIXED flavor (f32 enc / q8 dec — the GPU-tower serving mode): the decoder
        // q-planes match the eager pass, the encoder stays unquantized, and the facade's
        // .dlim route reads the tag back
        let imgm = image_path_for(path, "whisper-f32-q8")
        remove(imgm)
        var inscope stgm <- stage_whisper_model(path, false, true)
        var inscope wm <- load_whisper_model(path, false, true)
        t |> success(wm.image_map != null, "mixed cold load serves the image it streamed")
        t |> success(!wm.enc.q8 && wm.dq8, "mixed flavor: f32 encoder, q8 decoder")
        t |> equal(count_mismatch_i8(stgm.dqblob, wm.dqblob), 0l, "mixed streamed dqblob element-exact vs staged")
        t |> equal(count_mismatch(stgm.dqscales, wm.dqscales), 0l, "mixed streamed dqscales element-exact vs staged")
        t |> equal(count_mismatch(stgm.dblob, wm.dblob), 0l, "mixed streamed dblob element-exact vs staged")
        t |> equal(count_mismatch(stgm.enc.fblob, wm.enc.fblob), 0l, "mixed enc.fblob element-exact (full f32 layout)")
        var inscope am <- load_asr_model(imgm)
        t |> success(am.kind == AsrKind.whisper, "facade routes the mixed tag")
        t |> success(!am.whisper.enc.q8 && am.whisper.dq8, "facade .dlim route lands the mixed halves")
        delete samples
    }
}

// The whisper-class tower blocks on the GPU (arm `mtower`): CPU-vs-GPU transcript equality on
// the corpus clip + geometry-derived counter deltas, OFF leg in the same arm. Numeric bars live
// in the kernels suite; the transcript is the backend-portable oracle (battery: stated claims).
[test]
def test_whisper_metal_tower(tst : T?) {
    tst |> run("whisper tiny f32: the Metal tower blocks - transcript + counters + knob") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "whisper") || !arm_on(t, "mtower")) return
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(whisper_dir(), "ggml-tiny.bin")
            if (!model_available(t, path)) {
                return
            }
            var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present")
                return
            }
            // this cell is about the TOWER: pin the decoder-side driver off, or its f32-decoder
            // quant_mode decline panics the required-mode policy leg (test_whisper_metal_cross_kv
            // owns the wdec coverage)
            set_metal_wdec(false)
            set_metal_tower(false)
            let s0 = metal_tower_stats()
            let cpu = whisper_transcribe_text(path, samples, cached = false)
            set_metal_tower(true)
            let s1 = metal_tower_stats()
            t |> equal(s1.encodes, s0.encodes, "OFF leg never dispatches")
            t |> equal(s1.declines - s0.declines, 1l, "OFF leg records the knob decline")
            let gpu = whisper_transcribe_text(path, samples, cached = false)
            let s2 = metal_tower_stats()
            let chunks = s2.encodes - s1.encodes
            t |> equal(chunks, 1l, "jfk is one 30s chunk")
            t |> equal(s2.blocks - s1.blocks, 4l * chunks, "blocks delta = n_layer x chunks (tiny = 4)")
            t |> equal(s2.rows - s1.rows, 1500l * chunks, "rows delta = n_ctx x chunks")
            to_log(LOG_INFO, "whisper tiny mtower CPU eyeball: '{cpu.text}'\n")
            to_log(LOG_INFO, "whisper tiny mtower GPU eyeball: '{gpu.text}'\n")
            t |> success(!empty(gpu.text), "non-empty transcript")
            t |> success(cpu.text == gpu.text, "transcript-exact CPU vs GPU")
            // the deep instrument: large-v3-turbo's 32-layer encoder (f16 file, f32 mint)
            let lpath = path_join(whisper_dir(), "ggml-large-v3-turbo.bin")
            if (model_available(t, lpath)) {
                set_metal_tower(false)
                let lc = whisper_transcribe_text(lpath, samples, cached = false)
                set_metal_tower(true)
                let s3 = metal_tower_stats()
                let lg = whisper_transcribe_text(lpath, samples, cached = false)
                let s4 = metal_tower_stats()
                t |> equal(s4.encodes - s3.encodes, 1l, "large: one GPU chunk")
                t |> equal(s4.blocks - s3.blocks, 32l, "large: blocks delta = n_layer (32)")
                to_log(LOG_INFO, "whisper large mtower CPU eyeball: '{lc.text}'\n")
                to_log(LOG_INFO, "whisper large mtower GPU eyeball: '{lg.text}'\n")
                t |> success(!empty(lg.text), "large: non-empty transcript")
                t |> success(lc.text == lg.text, "large: transcript-exact CPU vs GPU")
            }
            // the q8 SERVING DEFAULT declines (quant_mode) and never dispatches — the shipped path stays CPU
            var inscope w8 = WhisperModel()
            var inscope stg8 <- stage_whisper_model(path, true)
            mint_whisper_model(stg8, w8)
            t |> success(w8.enc.q8, "the q8 flavor is on the q8 rail")
            let sq0 = metal_tower_stats()
            var q8_text = ""
            with_job_que() {
                setup_dasllama_jobque()
                var s8 <- create_session(w8)
                transcribe(w8, s8, samples) $(seg) {
                    q8_text += seg.text
                }
                delete s8
            }
            let sq1 = metal_tower_stats()
            t |> equal(sq1.encodes, sq0.encodes, "q8 default never dispatches the tower")
            t |> equal(sq1.declines - sq0.declines, 1l, "q8 default records the quant_mode decline")
            t |> success(key_exists(metal_tower_declines(), "quant_mode"), "the decline reason is quant_mode")
            t |> success(!empty(q8_text), "q8 default still transcribes (CPU serves)")
            // required-mode contract: a capability decline (quant_mode) under MetalMode.required
            // panics instead of silently CPU-serving; the knob decline is POLICY and never does
            set_metal_mode(MetalMode.required)
            var tripped = false
            try {
                with_job_que() {
                    setup_dasllama_jobque()
                    var s9 <- create_session(w8)
                    transcribe(w8, s9, samples) $(seg) {
                        pass
                    }
                    delete s9
                }
            } recover {
                tripped = true
            }
            set_metal_mode(MetalMode.when_available)
            t |> success(tripped, "required mode: the q8 tower decline panics")
            // the POLICY class: a knob decline never panics, even under required mode
            set_metal_mode(MetalMode.required)
            set_metal_tower(false)
            let sp0 = metal_tower_stats()
            let pol = whisper_transcribe_text(path, samples, cached = false)
            set_metal_tower(true)
            set_metal_mode(MetalMode.when_available)
            t |> success(!empty(pol.text), "required mode + knob off: the policy decline serves CPU, no panic")
            t |> equal(metal_tower_stats().declines - sp0.declines, 1l, "the knob decline is counted, not escalated")
            // re-arm after the shutdown seats: the registry PSOs and the tower's own objects
            // release, and the next encode brings everything back up
            metal_tower_shutdown()
            metal_prefill_shutdown()
            metal_decode_shutdown()
            let sr0 = metal_tower_stats()
            let rearm = whisper_transcribe_text(path, samples, cached = false)
            t |> equal(metal_tower_stats().encodes - sr0.encodes, 1l, "post-shutdown encode re-arms and serves")
            t |> success(rearm.text == cpu.text, "post-shutdown transcript unchanged")
            set_metal_wdec(true)
            delete samples
        }
    }
}

def private wdec_transcribe(w : WhisperModel; samples : array<float>) : string {
    var text = ""
    with_job_que() {
        setup_dasllama_jobque()
        var s <- create_session(w)
        transcribe(w, s, samples) $(seg) {
            text += seg.text
        }
        delete s
    }
    return text
}

// The whisper ASR-decoder driver (arm `mtower`, decoder half): GPU cross-KV on the q8 serving
// default — transcript equality vs the CPU chain, window-counter deltas, the knob/quant_mode
// decline classes, required-mode contract, and the shutdown re-arm.
[test]
def test_whisper_metal_cross_kv(tst : T?) {
    tst |> run("whisper tiny q8: GPU cross-KV - transcript + counters + knob + declines") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "whisper") || !arm_on(t, "mtower")) return
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(whisper_dir(), "ggml-tiny.bin")
            if (!model_available(t, path)) {
                return
            }
            var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present")
                return
            }
            var inscope w8 = WhisperModel()
            var inscope stg8 <- stage_whisper_model(path, true)
            mint_whisper_model(stg8, w8)
            // GPU leg: the q8 serving default's cross-KV lands on the driver, one window per clip
            set_metal_wdec(true)
            let s0 = metal_wdec_stats()
            let gpu_text = wdec_transcribe(w8, samples)
            let s1 = metal_wdec_stats()
            t |> equal(s1.windows - s0.windows, 1l, "jfk is one cross-KV window")
            // OFF leg: knob decline counted, CPU serves, transcript identical
            set_metal_wdec(false)
            let cpu_text = wdec_transcribe(w8, samples)
            let s2 = metal_wdec_stats()
            set_metal_wdec(true)
            t |> equal(s2.windows, s1.windows, "OFF leg never serves")
            t |> equal(s2.declines - s1.declines, 1l, "OFF leg records the knob decline")
            t |> success(key_exists(metal_wdec_declines(), "knob"), "the decline reason is knob")
            to_log(LOG_INFO, "whisper tiny cross-kv CPU eyeball: '{cpu_text}'\n")
            to_log(LOG_INFO, "whisper tiny cross-kv GPU eyeball: '{gpu_text}'\n")
            t |> success(!empty(gpu_text), "non-empty transcript")
            t |> success(cpu_text == gpu_text, "transcript-exact CPU vs GPU cross-KV")
            // the f32 ORACLE RAIL declines (quant_mode) — the driver is q8-native
            var inscope wf = WhisperModel()
            var inscope stgf <- stage_whisper_model(path)
            mint_whisper_model(stgf, wf)
            let sf0 = metal_wdec_stats()
            let f32_text = wdec_transcribe(wf, samples)
            let sf1 = metal_wdec_stats()
            t |> equal(sf1.windows, sf0.windows, "f32 decoder never serves")
            t |> success(sf1.declines - sf0.declines >= 1l, "f32 decoder records a decline")
            t |> success(key_exists(metal_wdec_declines(), "quant_mode"), "the decline reason is quant_mode")
            t |> success(!empty(f32_text), "f32 oracle rail still transcribes (CPU serves)")
            // required-mode contract: the capability decline panics, the knob decline never does
            set_metal_mode(MetalMode.required)
            var tripped = false
            try {
                with_job_que() {
                    setup_dasllama_jobque()
                    var s9 <- create_session(wf)
                    transcribe(wf, s9, samples) $(seg) {
                        pass
                    }
                    delete s9
                }
            } recover {
                tripped = true
            }
            t |> success(tripped, "required mode: the f32-decoder decline panics")
            // the POLICY leg rides the F32 model: its tower SERVES under required, so the only
            // decline is wdec's knob — policy, never escalated (the q8 model would panic here
            // on the TOWER's capability decline instead)
            set_metal_wdec(false)
            let sp0 = metal_wdec_stats()
            let pol_text = wdec_transcribe(wf, samples)
            set_metal_wdec(true)
            set_metal_mode(MetalMode.when_available)
            t |> success(!empty(pol_text), "required mode + knob off: the policy decline serves CPU, no panic")
            t |> equal(metal_wdec_stats().declines - sp0.declines, 1l, "the knob decline is counted, not escalated")
            // re-arm after the shutdown seat: the weight blob re-uploads on the next window
            metal_wdec_shutdown()
            let sr0 = metal_wdec_stats()
            let rearm_text = wdec_transcribe(w8, samples)
            t |> equal(metal_wdec_stats().windows - sr0.windows, 1l, "post-shutdown window re-arms and serves")
            t |> success(rearm_text == gpu_text, "post-shutdown transcript unchanged")
            delete samples
        }
    }
}

// Conformer families have NO seam - their hand-written encoders never reach encoder_blocks, so
// the tower counters must not move AT ALL (engagement absence, not a decline).
[test]
def test_parakeet_tower_absence(tst : T?) {
    tst |> run("parakeet: a Conformer transcribe never touches the tower rail") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "parakeet") || !arm_on(t, "mtower")) return
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
            if (!model_available(t, path)) {
                return
            }
            var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present")
                return
            }
            set_metal_tower(true)
            let s0 = metal_tower_stats()
            let g = parakeet_transcribe_text(path, samples, cached = false)
            let s1 = metal_tower_stats()
            t |> success(!empty(g.text), "transcript non-empty")
            t |> equal(s1.encodes, s0.encodes, "no tower encodes")
            t |> equal(s1.declines, s0.declines, "no tower declines either - the seam is never reached")
            delete samples
        }
    }
}

def private parakeet_transcribe_text(path : string; samples : array<float>; cached : bool = true) : tuple<text : string; mapped : bool> {
    var inscope m = AsrModel()
    if (cached) {
        m <- load_asr_model(path)   // nolint:PERF030 — fresh default-constructed target, single move
    } else {
        // the control: a staged read minted straight into memory, so it shares no bytes and no
        // file with the canonical image the other two arms load
        m.kind = AsrKind.parakeet
        var inscope stg <- stage_parakeet_model(path, true)
        mint_parakeet_model(stg, m.pk, true)
    }
    var text = ""
    with_job_que() {
        setup_dasllama_jobque()
        var s <- create_session(m, "en")
        text = transcribe(m, s, samples)
        delete s
    }
    let mapped = m.pk.image_map != null
    return (text = text, mapped = mapped)
}

[test]
def test_parakeet_image_roundtrip(t : T?) {
    t |> run("parakeet v3: bin load vs mapped image, transcript-exact") @(t : T?) {
        if (!family_on(t, "parakeet") || !arm_on(t, "parakeet")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        if (!model_available(t, path)) {
            return
        }
        var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        let img = image_path_for(path, "parakeet-q8")
        remove(img)
        // off-rail truth: both cached loads are image-backed, so they cannot check each other
        let g = parakeet_transcribe_text(path, samples, cached = false)
        let a = parakeet_transcribe_text(path, samples)
        t |> success(a.mapped, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        let b = parakeet_transcribe_text(path, samples)
        t |> success(b.mapped, "warm load is image-backed")
        // every served model is image-backed now; the control differs by SINK, not by form —
        // it is minted into memory and never touches the canonical .dlim
        to_log(LOG_INFO, "parakeet image-roundtrip jfk eyeball: '{g.text}'\n")
        t |> success(g.mapped && !empty(g.text), "non-empty transcript from the in-memory mint")
        t |> success(g.text == a.text, "in-memory mint vs cold-built image transcript")
        t |> success(g.text == b.text, "in-memory mint vs mapped image transcript")
        delete samples
    }
}

[test]
def test_qwen3a_tower_image_roundtrip(t : T?) {
    t |> run("qwen3a 0.6B tower: mapped image planes are element-exact vs gguf") @(t : T?) {
        if (!family_on(t, "qwen3a") || !arm_on(t, "qwen3a")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        if (!model_available(t, path)) {
            return
        }
        let img = image_path_for(path, "qwen3a-q8")
        remove(img)
        // off-rail truth: the staged read — both cached loads are image-backed and cannot
        // check each other
        var inscope a <- stage_qwen3a_tower(path)
        var inscope cold <- load_qwen3a_tower(path)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> success(cold.core.q8, "cold tower core is on the q8 rail")
        t |> equal(count_mismatch(a.cblob, cold.cblob), 0l, "cold-built cblob element-exact")
        var inscope b <- load_qwen3a_tower(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> success(b.core.q8, "warm tower core is on the q8 rail")
        t |> equal(b.core.d_model, a.e.core.d_model)
        t |> equal(b.core.n_layer, a.e.core.n_layer)
        t |> equal(b.core.n_ctx, a.e.core.n_ctx)
        t |> equal(b.proj_dim, a.e.proj_dim)
        t |> equal(b.n_ch, a.e.n_ch)
        t |> equal(count_mismatch(a.core.fblob, b.core.fblob), 0l, "core.fblob element-exact")
        t |> equal(count_mismatch_i8(a.core.qblob, b.core.qblob), 0l, "core.qblob element-exact")
        t |> equal(count_mismatch(a.core.qscales, b.core.qscales), 0l, "core.qscales element-exact")
        t |> equal(count_mismatch(a.cblob, b.cblob), 0l, "cblob element-exact")
        t |> success(b.cq8, "warm tower cblob trio is on the q8 rail")
        t |> equal(count_mismatch_i8(a.qcblob, b.qcblob), 0l, "qcblob element-exact")
        t |> equal(count_mismatch(a.qcscales, b.qcscales), 0l, "qcscales element-exact")
        // the f32 A/B rail still stages + serves (parity legs flip it; serving never does)
        set_qwen3a_q8(false)
        let img32 = image_path_for(path, "qwen3a-f32")
        remove(img32)
        var inscope a32 <- stage_qwen3a_tower(path)
        var inscope c32 <- load_qwen3a_tower(path)
        t |> success(!c32.core.q8, "f32 rail still serves")
        t |> equal(count_mismatch(a32.core.fblob, c32.core.fblob), 0l, "f32 core.fblob element-exact")
        set_qwen3a_q8(true)
    }
}

[test]
def test_gemma4uv_embedder_image_roundtrip(t : T?) {
    t |> run("gemma4uv embedder: mapped image planes are element-exact vs mmproj, bf16 lane") @(t : T?) {
        if (!family_on(t, "gemma") || !arm_on(t, "gemma4uv")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
        if (!model_available(t, path)) {
            return
        }
        let img = image_path_for(path, "gemma4uv")
        remove(img)
        // off-rail truth: the staged read — both cached loads are image-backed and cannot check
        // each other. A served embedder has no array-owning shape at all.
        var inscope a <- stage_gemma4uv_embedder(path)
        var inscope cold <- load_gemma4uv_embedder(path)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        // gemma-4's shipped "BF16" mmproj is MIXED: the patch embedder is F32 on disk, only the
        // projection is BF16. The plane choice follows each tensor, so exactly one rides wblob.
        t |> success(!cold.patch_bf16, "the F32 patch embedder stays on the fp32 plane")
        t |> success(cold.proj_bf16, "the BF16 projection rides the bf16 plane")
        t |> equal(count_mismatch(a.blob, cold.blob), 0l, "cold-built blob element-exact")
        var inscope b <- load_gemma4uv_embedder(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> success(b.patch_bf16 == cold.patch_bf16 && b.proj_bf16 == cold.proj_bf16,
                     "warm load keeps both plane choices")
        t |> equal(b.n_embd, a.e.n_embd)
        t |> equal(b.proj_dim, a.e.proj_dim)
        t |> equal(b.patch, a.e.patch)
        t |> equal(b.patch_dim, a.e.patch_dim)
        t |> equal(b.pos_entries, a.e.pos_entries)
        t |> equal(b.patch_w, a.e.patch_w)
        t |> equal(b.proj_w, a.e.proj_w)
        t |> equal(b.pos_w, a.e.pos_w)
        t |> equal(count_mismatch(a.blob, b.blob), 0l, "blob element-exact")
        t |> equal(count_mismatch_u16(a.wblob, b.wblob), 0l, "wblob (the bf16 GEMM) element-exact")
        // the bf16 plane holds the file's OWN halfwords: exactly the projection, nothing rounded
        t |> equal(long_length(a.wblob), a.e.n_embd * a.e.proj_dim,
                   "the bf16 plane is exactly the projection ({length(a.wblob)} halfwords)")
        // an F32 mmproj is a DIFFERENT lane and a different image — rounding it down to bf16
        // would be a real precision loss, so the flavor follows the file
        let f32path = path_join(models_dir(), "mmproj-gemma-4-12B-it-F32.gguf")
        if (stat(f32path).is_valid) {
            let img32 = image_path_for(f32path, "gemma4uv")
            remove(img32)
            var inscope a32 <- stage_gemma4uv_embedder(f32path)
            var inscope c32 <- load_gemma4uv_embedder(f32path)
            t |> success(!c32.patch_bf16 && !c32.proj_bf16, "an all-F32 mmproj mints no bf16 plane")
            t |> equal(long_length(a32.wblob), 0l, "the all-fp32 source leaves wblob empty")
            t |> equal(count_mismatch(a32.blob, c32.blob), 0l, "f32 blob element-exact")
            t |> success(img32 != img, "a different source keys a different image")
        }
    }
}

// The GPU tower vs the CPU chain on synthetic canvases — engage evidence is COUNTER DELTAS
// (a green that never dispatched is vacuous), and the OFF leg lives in the same arm so the
// knob is proven both ways every run.
[test]
def test_gemma4uv_metal_tower(tst : T?) {
    tst |> run("gemma4uv: the Metal tower serves the embedder - parity + counters + knob") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "gemma") || !arm_on(t, "gemma4uv-metal")) return
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
            if (!model_available(t, path)) {
                return
            }
            var inscope emb <- load_gemma4uv_embedder(path)
            var st = Gemma4uvState()
            // 672x336 is the non-square/orientation gate, 384x384 the no-pad shape (npos % 32 == 0)
            let shapes = [(w = 480l, h = 480l), (w = 336l, h = 336l), (w = 672l, h = 336l),
                          (w = 384l, h = 384l), (w = 96l, h = 96l)]
            for (sh in shapes) {
                let tag = "gemma4uv tower {sh.w}x{sh.h}"
                var planes : array<float>
                planes |> resize(sh.w * sh.h * 3l)
                for (i in range64(sh.w * sh.h * 3l)) {
                    planes[i] = sin(float(i % 977l) * 0.013) * 0.7
                }
                let s0 = metal_tower_stats()
                set_metal_tower(false)
                var cpu_out : array<float>
                let np_cpu = gemma4uv_encode(emb, st, planes, sh.w, sh.h, cpu_out)
                set_metal_tower(true)
                let s1 = metal_tower_stats()
                t |> equal(s1.encodes, s0.encodes, "{tag}: OFF leg never dispatches")
                t |> equal(s1.declines - s0.declines, 1l, "{tag}: OFF leg records the knob decline")
                var gpu_out : array<float>
                let np_gpu = gemma4uv_encode(emb, st, planes, sh.w, sh.h, gpu_out)
                let s2 = metal_tower_stats()
                t |> equal(np_cpu, (sh.w / emb.patch) * (sh.h / emb.patch), "{tag}: npos from geometry")
                t |> equal(np_gpu, np_cpu, "{tag}: both legs agree on npos")
                t |> equal(s2.encodes - s1.encodes, 1l, "{tag}: GPU leg served (encodes delta)")
                t |> equal(s2.rows - s1.rows, np_gpu, "{tag}: rows delta is npos")
                t |> equal(s2.declines, s1.declines, "{tag}: GPU leg declined nothing")
                tower_parity(t, tag, cpu_out, gpu_out)
                delete gpu_out
                delete cpu_out
                delete planes
            }
            delete st
            // the all-F32 mmproj leg: both GEMMs ride the f32 plane (the BF16 file's MIXED
            // layout never exercises the f32 PROJECTION arm) — skips honestly without the file
            let f32path = path_join(models_dir(), "mmproj-gemma-4-12B-it-F32.gguf")
            if (stat(f32path).is_valid) {
                var inscope emb32 <- load_gemma4uv_embedder(f32path)
                t |> success(!emb32.patch_bf16 && !emb32.proj_bf16, "the F32 file keeps both GEMMs on the f32 plane")
                var st32 = Gemma4uvState()
                var planes32 : array<float>
                planes32 |> resize(96l * 96l * 3l)
                for (i in range64(96l * 96l * 3l)) {
                    planes32[i] = sin(float(i) * 0.013) * 0.7
                }
                set_metal_tower(false)
                var cpu32 : array<float>
                gemma4uv_encode(emb32, st32, planes32, 96l, 96l, cpu32)
                set_metal_tower(true)
                let sf0 = metal_tower_stats()
                var gpu32 : array<float>
                gemma4uv_encode(emb32, st32, planes32, 96l, 96l, gpu32)
                t |> equal(metal_tower_stats().encodes - sf0.encodes, 1l, "f32-mmproj leg GPU-served")
                tower_parity(t, "gemma4uv tower f32-mmproj 96x96", cpu32, gpu32)
                delete gpu32
                delete cpu32
                delete planes32
                delete st32
            }
        }
    }
}

[test]
def test_canary_encoder_image_roundtrip(t : T?) {
    t |> run("canary encoder: mapped image planes are element-exact vs bin") @(t : T?) {
        if (!family_on(t, "canary") || !arm_on(t, "canary")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        if (!model_available(t, path)) {
            return
        }
        let img = image_path_for(path, "canary-f32")
        remove(img)
        // off-rail truth: both cached loads are image-backed, so they cannot check each other.
        // The staging carrier IS the off-rail read now — a served encoder has no array form.
        var inscope a <- stage_canary_encoder(path)
        var inscope cold <- load_canary_encoder(path)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> equal(count_mismatch(a.blob, cold.blob), 0l, "cold-built blob element-exact")
        var inscope b <- load_canary_encoder(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> equal(b.d_model, a.e.d_model)
        t |> equal(b.n_layer, a.e.n_layer)
        t |> equal(b.proj_dim, a.e.proj_dim)
        t |> equal(length(b.layers), long_length(a.layers))
        t |> equal(count_mismatch(a.blob, b.blob), 0l, "blob element-exact")
        t |> equal(count_mismatch(a.filters, b.filters), 0l, "filters element-exact")
        t |> equal(count_mismatch(a.window, b.window), 0l, "window element-exact")
        t |> equal(count_mismatch(a.twiddles, b.twiddles), 0l, "twiddles element-exact")
    }
}

[test]
def test_canary_encoder_q8_image_roundtrip(t : T?) {
    t |> run("canary encoder q8: mapped image planes are element-exact vs bin transcode") @(t : T?) {
        if (!family_on(t, "canary") || !arm_on(t, "canary")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        if (!model_available(t, path)) {
            return
        }
        let img = image_path_for(path, "canary-q8")
        remove(img)
        // off-rail truth: the staging carrier re-runs the read-time transcode + repack under
        // the same backend pin, so its planes are the served image's ground truth
        var inscope a <- stage_canary_encoder(path, true)
        var inscope cold <- load_canary_encoder(path, true)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> success(cold.q8, "served encoder is on the q8 rail")
        t |> equal(count_mismatch_i8(a.qblob, cold.qblob), 0l, "cold-built qblob element-exact")
        var inscope b <- load_canary_encoder(path, true)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> success(b.q8, "warm encoder is on the q8 rail")
        t |> equal(b.d_model, a.e.d_model)
        t |> equal(b.n_layer, a.e.n_layer)
        t |> equal(b.proj_dim, a.e.proj_dim)
        t |> equal(length(b.layers), long_length(a.layers))
        t |> equal(count_mismatch_i8(a.qblob, b.qblob), 0l, "qblob element-exact")
        t |> equal(count_mismatch(a.qscales, b.qscales), 0l, "qscales element-exact")
        t |> equal(count_mismatch(a.blob, b.blob), 0l, "compact fp32 blob element-exact")
        t |> equal(count_mismatch(a.filters, b.filters), 0l, "filters element-exact")
        // the off-rail control: a staged read minted straight into memory; the mint derives
        // its family tag from the staging's own precision
        var inscope stg <- stage_canary_encoder(path, true)
        var inscope minted = CanaryEncoder()
        mint_canary_encoder(stg, minted)
        t |> success(minted.q8, "minted encoder is on the staging's q8 rail")
        t |> equal(count_mismatch_i8(a.qblob, minted.qblob), 0l, "minted qblob element-exact vs staged truth")
    }
}

[test]
def test_gemma4a_encoder_image_roundtrip(t : T?) {
    t |> run("gemma4a encoder q8: mapped image planes are element-exact vs gguf transcode") @(t : T?) {
        if (!family_on(t, "gemma4a") || !arm_on(t, "gemma4a")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(t, path)) {
            return
        }
        let img = image_path_for(path, "gemma4a-q8")
        remove(img)
        // off-rail truth: the staging carrier re-runs the read-time transcode + repack under
        // the same backend pin, so its planes are the served image's ground truth
        var inscope a <- stage_gemma4a_encoder(path)
        var inscope cold <- load_gemma4a_encoder(path)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> success(cold.q8, "served encoder is on the q8 rail")
        t |> equal(count_mismatch_i8(a.qblob, cold.qblob), 0l, "cold-built qblob element-exact")
        var inscope b <- load_gemma4a_encoder(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> success(b.q8, "warm encoder is on the q8 rail")
        t |> equal(b.d_model, a.e.d_model)
        t |> equal(b.n_layer, a.e.n_layer)
        t |> equal(b.proj_dim, a.e.proj_dim)
        t |> equal(length(b.layers), long_length(a.layers))
        t |> equal(count_mismatch_i8(a.qblob, b.qblob), 0l, "qblob element-exact")
        t |> equal(count_mismatch(a.qscales, b.qscales), 0l, "qscales element-exact")
        t |> equal(count_mismatch(a.blob, b.blob), 0l, "compact fp32 blob element-exact")
        t |> equal(count_mismatch(a.twiddles, b.twiddles), 0l, "twiddles element-exact")
        t |> equal(count_mismatch(a.filters, b.filters), 0l, "filters element-exact")
        t |> equal(count_mismatch(a.pos_emb, b.pos_emb), 0l, "pos_emb element-exact")
        t |> equal(count_mismatch(a.hann, b.hann), 0l, "hann element-exact")
        t |> equal(count_mismatch(a.ones, b.ones), 0l, "ones element-exact")
        // the f32 A/B rail still stages + serves (parity legs flip it; serving never does)
        set_gemma4a_q8(false)
        let img32 = image_path_for(path, "gemma4a-f32")
        remove(img32)
        var inscope a32 <- stage_gemma4a_encoder(path)
        var inscope c32 <- load_gemma4a_encoder(path)
        t |> success(!c32.q8, "f32 rail still serves")
        t |> equal(count_mismatch(a32.blob, c32.blob), 0l, "f32 blob element-exact")
        set_gemma4a_q8(true)
    }
}

[test]
def test_gemma4a_transcribe_q8_ab(t : T?) {
    t |> run("gemma4a transcribe: the q8 tower vs the f32 rail on jfk, same text") @(t : T?) {
        if (!family_on(t, "gemma4a") || !arm_on(t, "gemma4a")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let dec_path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
        let mmp_path = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(t, dec_path) || !model_available(t, mmp_path)) return
        var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present beside the models")
            return
        }
        var q8_text = ""
        var f32_text = ""
        var lang = ""
        with_job_que() {
            setup_dasllama_jobque_()
            // decoder off-rail (REVIEW: suites load decoders with load_model_) — loaded ONCE; only the tower flips rails
            var dec <- load_model_(dec_path, QuantMode.q8)
            var inscope enc <- load_gemma4a_encoder(mmp_path)
            var inscope s <- create_gemma4a_asr_session(dec)
            t |> success(enc.q8, "leg A tower is on the q8 rail")
            gemma4a_transcribe(dec, enc, s, samples, $(seg : TranscribeSegment) {
                q8_text += seg.text
            }, lang)
            set_gemma4a_q8(false)
            var inscope enc32 <- load_gemma4a_encoder(mmp_path)
            var inscope s32 <- create_gemma4a_asr_session(dec)
            t |> success(!enc32.q8, "leg B tower is on the f32 rail")
            gemma4a_transcribe(dec, enc32, s32, samples, $(seg : TranscribeSegment) {
                f32_text += seg.text
            }, lang)
            set_gemma4a_q8(true)
            delete dec
        }
        to_log(LOG_INFO, "gemma4a q8 : {q8_text}\n")
        to_log(LOG_INFO, "gemma4a f32: {f32_text}\n")
        t |> equal(q8_text, f32_text, "q8 tower transcript matches the f32 rail")
        delete samples
    }
}

[test]
def test_qwen3a_tower_q8_ab(t : T?) {
    t |> run("qwen3a tower: the q8 core tracks the f32 rail on jfk window 0") @(t : T?) {
        if (!family_on(t, "qwen3a") || !arm_on(t, "qwen3a")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        if (!model_available(t, path)) return
        var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present beside the models")
            return
        }
        var inscope twq <- load_qwen3a_tower(path)
        t |> success(twq.core.q8, "tower core is on the q8 rail")
        set_qwen3a_q8(false)
        var inscope twf <- load_qwen3a_tower(path)
        set_qwen3a_q8(true)
        t |> success(!twf.core.q8, "reference tower is on the f32 rail")
        var inscope mel <- log_mel_qwen3a(samples, twq.core.n_mel)
        var inscope encq <- make_encoder_state(twq.core)
        var inscope encf <- make_encoder_state(twf.core)
        var inscope csq = Qwen3aConvState()
        var inscope csf = Qwen3aConvState()
        with_job_que() {
            setup_dasllama_jobque_()
            var inscope win <- qwen3a_mel_window(mel, 0l)
            qwen3a_encode(twq, encq, csq, win)
            var inscope winf <- qwen3a_mel_window(mel, 0l)
            qwen3a_encode(twf, encf, csf, winf)
        }
        var maxd = 0.0
        var maxa = 0.0
        var sd = 0.0lf
        var sa = 0.0lf
        let nv = long_length(encq.out)
        for (i in range64(nv)) {
            let df = encq.out[i] - encf.out[i]
            maxd = max(maxd, abs(df))
            maxa = max(maxa, abs(encf.out[i]))
            sd += double(df * df)
            sa += double(encf.out[i] * encf.out[i])
        }
        let rel_rms = float(sqrt(sd / max(sa, 1.0e-18lf)))
        to_log(LOG_INFO, "qwen3a q8-vs-f32 window 0: rel-rms {rel_rms}, maxdiff {maxd} (signal max {maxa}) over {nv} values\n")
        // element-level tripwire; the real gate is transcript-level (test_qwen3a_transcribe_q8_ab)
        t |> success(rel_rms < 0.05, "q8 core tracks the f32 rail (element-level bar)")
        delete samples
    }
}

[test]
def test_qwen3a_transcribe_q8_ab(t : T?) {
    t |> run("qwen3a transcribe: the q8 tower core vs the f32 rail on jfk, same text") @(t : T?) {
        if (!family_on(t, "qwen3a") || !arm_on(t, "qwen3a")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let dec_path = path_join(models_dir(), "Qwen3-ASR-0.6B-Q8_0.gguf")
        let mmp_path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        if (!model_available(t, dec_path) || !model_available(t, mmp_path)) return
        var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present beside the models")
            return
        }
        var q8_text = ""
        var f32_text = ""
        var lang = ""
        with_job_que() {
            setup_dasllama_jobque_()
            // decoder off-rail (REVIEW: suites load decoders with load_model_) — loaded ONCE; only the tower flips rails
            var dec <- load_model_(dec_path, QuantMode.q8)
            dec.config.seq_len = min(dec.config.seq_len, 8192l)
            var inscope tw <- load_qwen3a_tower(mmp_path)
            t |> success(tw.core.q8, "leg A tower core is on the q8 rail")
            var inscope s <- create_qwen3a_asr_session(dec, tw, "auto", false)
            qwen3a_transcribe(dec, tw, s, "", samples, $(seg : TranscribeSegment) {
                q8_text += seg.text
            }, lang)
            set_qwen3a_q8(false)
            var inscope tw32 <- load_qwen3a_tower(mmp_path)
            t |> success(!tw32.core.q8, "leg B tower core is on the f32 rail")
            var inscope s32 <- create_qwen3a_asr_session(dec, tw32, "auto", false)
            qwen3a_transcribe(dec, tw32, s32, "", samples, $(seg : TranscribeSegment) {
                f32_text += seg.text
            }, lang)
            set_qwen3a_q8(true)
            delete dec
        }
        to_log(LOG_INFO, "qwen3a q8 : {q8_text}\n")
        to_log(LOG_INFO, "qwen3a f32: {f32_text}\n")
        t |> equal(q8_text, f32_text, "q8 tower transcript matches the f32 rail")
        delete samples
    }
}

// The qwen3a f32-rail tower on the GPU (arm `mtower`): transcript equality with the knob off
// vs on, blocks-counter delta derived from the loaded tower's own n_layer. The q8 SERVING
// default declines by design (the whisper mtower cell above owns that assert).
[test]
def test_qwen3a_metal_tower(tst : T?) {
    tst |> run("qwen3a f32 rail: the Metal tower blocks - transcript + counters") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "qwen3a") || !arm_on(t, "mtower")) return
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let dec_path = path_join(models_dir(), "Qwen3-ASR-0.6B-Q8_0.gguf")
            let mmp_path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
            if (!model_available(t, dec_path) || !model_available(t, mmp_path)) return
            var samples <- read_wav_pcm16_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present beside the models")
                return
            }
            var cpu_text = ""
            var gpu_text = ""
            var lang = ""
            var nlayer = 0l
            var enc_delta = 0l
            var blk_delta = 0l
            with_job_que() {
                setup_dasllama_jobque_()
                var dec <- load_model_(dec_path, QuantMode.q8)
                dec.config.seq_len = min(dec.config.seq_len, 8192l)
                set_qwen3a_q8(false)
                var inscope tw <- load_qwen3a_tower(mmp_path)
                t |> success(!tw.core.q8, "tower core is on the f32 rail")
                nlayer = tw.core.n_layer
                set_metal_tower(false)
                var inscope s <- create_qwen3a_asr_session(dec, tw, "auto", false)
                qwen3a_transcribe(dec, tw, s, "", samples, $(seg : TranscribeSegment) {
                    cpu_text += seg.text
                }, lang)
                let s0 = metal_tower_stats()
                set_metal_tower(true)
                var inscope s2 <- create_qwen3a_asr_session(dec, tw, "auto", false)
                qwen3a_transcribe(dec, tw, s2, "", samples, $(seg : TranscribeSegment) {
                    gpu_text += seg.text
                }, lang)
                let s1 = metal_tower_stats()
                enc_delta = s1.encodes - s0.encodes
                blk_delta = s1.blocks - s0.blocks
                set_qwen3a_q8(true)
                delete dec
            }
            t |> success(enc_delta >= 1l, "GPU leg dispatched ({enc_delta} encodes)")
            t |> equal(blk_delta, nlayer * enc_delta, "blocks delta = n_layer x encodes")
            to_log(LOG_INFO, "qwen3a mtower CPU eyeball: {cpu_text}\n")
            to_log(LOG_INFO, "qwen3a mtower GPU eyeball: {gpu_text}\n")
            t |> success(!empty(gpu_text), "non-empty transcript")
            // a legitimate near-tie flip means converting this cell to forced-feed, not loosening the compare
            t |> equal(cpu_text, gpu_text, "transcript equality CPU vs GPU")
            delete samples
        }
    }
}

[test]
def test_fp32_decoder_image_roundtrip(t : T?) {
    t |> run("canary decoder fp32: gguf load vs mapped image, token-for-token") @(t : T?) {
        if (!family_on(t, "canary") || !arm_on(t, "canary-dec")) return
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "canary-qwen-2.5b-decoder-f16.gguf")
        if (!model_available(t, path)) {
            return
        }
        // the fp32 rail is opt-in (image_fp32) — the parity-pinned ASR decoders' lane
        let img = image_path_for(path, "", "fp32")
        remove(img)
        // off-rail truth: both cached loads are image-backed, so they cannot check each other
        var g <- load_model_(path, QuantMode.fp32)
        g.config.seq_len = min(g.config.seq_len, 2048l)
        var prompt <- encode(g, "Once upon a time")
        var ga <- gen_ids(g, prompt, 16)
        delete g
        var a <- load_model_cached(path, QuantMode.fp32, planar_only = true, image_fp32 = true)
        t |> success(a.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        a.config.seq_len = min(a.config.seq_len, 2048l)
        var gcold <- gen_ids(a, prompt, 16)
        t |> success(!empty(ga) && count_mismatch(ga, gcold) == 0l, "gguf vs cold-built image tokens")
        delete gcold
        delete a
        var b <- load_model_cached(path, QuantMode.fp32, planar_only = true, image_fp32 = true)
        t |> success(b.image_map != null, "warm load is image-backed")
        b.config.seq_len = min(b.config.seq_len, 2048l)
        var gb <- gen_ids(b, prompt, 16)
        t |> success(!empty(ga) && count_mismatch(ga, gb) == 0l, "gguf vs image tokens")
        delete b
        delete ga
        delete gb
        delete prompt
    }
}

// The lane-GC regression gate: a GPU-flavor bake runs in its own pin context, so its identity
// is un-recomputable here — the verdict must say FOREIGN (kept), never OTHER (reaped). The
// 2026-08-02 bring-up lost every two-flavor model to exactly this: the metal save reaped the
// planar sibling, then --clean reaped the metal images.
[test]
def test_dlim_flavor_lanes(t : T?) {
    t |> run("flavor lanes: another bake context's image is never judged from here") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let planar = image_identity("", "q8")
        t |> equal(dlim_identity_lane(planar), "q8#", "planar lane is quant-only")
        t |> equal(dlim_identity_lane("{planar}|metal"), "q8#metal", "metal tag folds into the lane")
        let vk_tag = "vulkan|m2n b8192 l4 s1 r110000110"
        t |> equal(dlim_identity_lane("{planar}|{vk_tag}"), "q8#{vk_tag}", "vulkan tags keep their inner pipes")
        var current <- dlim_current_identities()
        var p = DlimPeek(valid = true, version = IMAGE_VERSION, identity = planar)
        t |> equal(dlim_image_verdict(p, current), "CURRENT", "own planar identity")
        // the observed cross-context metal identity: blob bake selects portable + its own winners
        p.identity = "v{IMAGE_VERSION}|q8|portable|s16|q8 mr4 b0 g4|kq 4/4/4/4|q51 mr1|nat 1101|metal"
        t |> equal(dlim_image_verdict(p, current), "FOREIGN", "cross-context metal image is FOREIGN, not OTHER")
        p.identity = "v{IMAGE_VERSION}|q8|portable|s16|q8 mr4 b0 g4|kq 4/4/4/4|q51 mr1|nat 1101|{vk_tag}"
        t |> equal(dlim_image_verdict(p, current), "FOREIGN", "cross-context vulkan image is FOREIGN")
        // a family tag whose module is not loaded in the judging process: unrecomputable, so
        // FOREIGN — the 2026-08-02 board's step-zero GC reaped every whisper/parakeet image
        // this way before the rule generalized
        p.identity = "v{IMAGE_VERSION}|q8|arm64-gen|s16|q8 mr8 b0 g4|kq 8/8/4/8|q51 mr4|nat 1101|not-a-registered-family"
        t |> equal(dlim_image_verdict(p, current), "FOREIGN", "unregistered family tag is FOREIGN")
        // a REGISTERED family tag is judgeable — whisper's module is required by this suite
        p.identity = image_identity("whisper-q8")
        t |> equal(dlim_image_verdict(p, current), "CURRENT", "registered family tag, own identity")
        p.identity = replace(image_identity("whisper-q8"), "|s16|", "|s32|")
        t |> equal(dlim_image_verdict(p, current), "OTHER", "registered family tag, drifted knobs")
        // same LANE as this process, different knobs — provably stale here
        p.identity = replace(planar, "|s16|", "|s32|")
        t |> equal(dlim_image_verdict(p, current), "OTHER", "own-lane knob drift stays OTHER")
        p.version = IMAGE_VERSION - 1
        t |> equal(dlim_image_verdict(p, current), "STALE v{IMAGE_VERSION - 1}", "version staleness wins in any lane")
        p.valid = false
        t |> equal(dlim_image_verdict(p, current), "BROKEN", "invalid peek")
        delete current
    }
}
