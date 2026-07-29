options gen2
options persistent_heap   // + explicit deletes: each arm frees its model before the next loads
options stack = 131072

require dastest/testing_boost public
require dasllama/dasllama            // the facade: load_model's image cache is the surface under test
require dasllama/dasllama_whisper
require dasllama/dasllama_gguf       // rd_u32/rd_u16 — the wav fixture reader
require dasllama/dasllama_image
require dasllama/dasllama_config     // DlimConfiguration — the peek/round-trip mechanics arms
require dasllama/dasllama_math   // pin_kernel_backend — the metal-flavor arm's donor contract
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

def private whisper_dir() : string {
    let env = get_env_variable("DASLLAMA_WHISPER_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/whisper.cpp/models/" : env
}

// element-exact plane compare: -1 = length mismatch, else the number of differing elements
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
    remove(img)   // cold start: the first facade load must regenerate + save
    var a <- load_model(path, QuantMode.q8)
    t |> success(a.image_map == null, "cold load is gguf-backed")
    t |> success(stat(img).is_valid, "cold load saved an image")
    var cfg_a := a.config   // the gguf truth, kept past delete for the round-trip compare
    if (a.config.seq_len > 2048l) {
        a.config.seq_len = 2048l
    }
    var prompt <- encode(a, "Once upon a time")
    var ga <- gen_ids(a, prompt, n_gen)
    delete a
    var b <- load_model(path, QuantMode.q8)
    t |> success(b.image_map != null, "warm load is image-backed")
    let cfg_diff = config_diff_field(cfg_a, b.config)
    t |> success(empty(cfg_diff), "config round-trips exactly ({cfg_diff})")
    delete cfg_a
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
        if (!family_on(t, "llama")) return
        if (!arm_on(t, "smol")) {
            return
        }
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
def private forced_logits_maxd(tc, tg : Model; prompt, teach : array<int64>) : float {
    var maxd = 0.0
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
    return maxd
}

[test]
def test_metal_flavor_image(tst : T?) {
    tst |> run("SmolLM2 q8: metal flavor — blob mint + map + GPU serve + CPU tripwire") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "llama")) return
            if (!arm_on(t, "metal")) {
                return
            }
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
            t |> success(tm.image_map == null, "cold metal load is gguf-backed")
            t |> success(stat(img_m).is_valid, "cold metal load saved the metal-flavor image")
            t |> success(length(tm.qblob) == 0, "planar q8 planes are gone")
            var g_mint <- gen_ids(tm, prompt, 16)   // a blob model CANNOT silently CPU-serve —
                                                    // completing at all proves the GPU served
            // teacher-forced parity vs a fresh planar CPU control (engine entry, no image
            // cache) — probe-measured noise ~0.75, breakage class 15+; 4.0 = ~4x SNR
            var tp <- load_model_(path, QuantMode.q8)
            let np = long_length(prompt)
            var teach <- [for (i in range64(np, long_length(g_cpu))); g_cpu[i]]
            let maxd = forced_logits_maxd(tp, tm, prompt, teach)
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

[test]
def test_voxtral_mini_image(t : T?) {
    t |> run("voxtral-mini q8: the >2 GiB-plane image round trip") @(t : T?) {
        if (!family_on(t, "voxtral")) return
        if (!arm_on(t, "voxtral")) {
            return
        }
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
        if (!family_on(t, "gemma")) return
        // gemma4's configure sets gguf-overridable fallbacks (sliding_window, swa_pattern);
        // re-running it on an image load stomped the serialized overrides (caught 2026-07-16:
        // token drift on the mapped E2B while every llama-arch carrier round-tripped clean)
        if (!arm_on(t, "gemma")) {
            return
        }
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
        if (!family_on(t, "ultravox")) return
        if (!arm_on(t, "tower")) {
            return
        }
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
        var a <- load_audio_tower(path)
        t |> success(a.image_map == null, "cold load is gguf-backed")
        t |> success(stat(img).is_valid, "cold load saved an image")
        var b <- load_audio_tower(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> equal(b.d_model, a.d_model)
        t |> equal(b.n_layer, a.n_layer)
        t |> equal(b.n_ff, a.n_ff)
        t |> equal(b.n_ctx, a.n_ctx)
        t |> equal(b.proj_dim, a.proj_dim)
        t |> equal(b.stack_factor, a.stack_factor)
        t |> equal(b.mm1_out, a.mm1_out)
        t |> success(b.proj_kind == a.proj_kind, "proj_kind")
        t |> equal(b.layers_off, a.layers_off)
        t |> equal(b.layer_stride, a.layer_stride)
        t |> equal(count_mismatch(a.fblob, b.fblob), 0l, "fblob element-exact")
        delete a
        delete b
    }
}

// minimal RIFF/WAVE reader for the 16-bit PCM mono fixture (jfk.wav) — test_whisper's twin
def private read_wav_pcm16_mono(path : string) : array<float> {
    var out : array<float>
    let f = fopen(path, "rb")
    if (f == null) {
        return <- out
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let n = long_length(bytes)
        var o = 12l
        while (o + 8l <= n) {
            let id = rd_u32(bytes, o)
            let sz = int64(rd_u32(bytes, o + 4l))
            if (id == 0x61746164u) {
                let cnt = min(sz, n - o - 8l) / 2l
                out |> resize(int(cnt))
                for (i in range64(cnt)) {
                    var v = int(rd_u16(bytes, o + 8l + i * 2l))
                    if (v >= 32768) {
                        v -= 65536
                    }
                    out[i] = float(v) / 32768.0
                }
                break
            }
            o += 8l + sz + (sz & 1l)
        }
    }
    fclose(f)
    return <- out
}

def private whisper_transcribe_text(path : string; samples : array<float>) : tuple<text : string; mapped : bool> {
    var w <- load_whisper_model(path)
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
    delete w
    return (text = text, mapped = mapped)
}

[test]
def test_whisper_image_roundtrip(t : T?) {
    t |> run("whisper tiny: gguf load vs mapped image, transcript-exact") @(t : T?) {
        if (!family_on(t, "whisper")) return
        if (!arm_on(t, "whisper")) {
            return
        }
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
        let a = whisper_transcribe_text(path, samples)
        t |> success(!a.mapped, "cold load is gguf-backed")
        t |> success(stat(img).is_valid, "cold load saved an image")
        let b = whisper_transcribe_text(path, samples)
        t |> success(b.mapped, "warm load is image-backed")
        t |> success(!empty(a.text), "non-empty transcript")
        t |> success(a.text == b.text, "transcripts match")
        delete samples
    }
}
