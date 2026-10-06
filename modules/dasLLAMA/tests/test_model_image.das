options gen2
options persistent_heap   // + explicit deletes: each arm frees its model before the next loads
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _function_length = 0   // test bodies enumerate the fixture arms
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama            // the facade: load_model's image cache is the surface under test
require dasllama/dasllama_whisper
require dasllama/dasllama_gguf       // rd_u32/rd_u16 — the wav fixture reader
require dasllama/dasllama_image
require dasllama/dasllama_plane   // PlaneF — the served side of the round-trip comparisons
require dasllama/dasllama_canary  // CanaryStaging/stage_canary_encoder — the off-rail truth read
require dasllama/dasllama_gemma4a // Gemma4aStaging/stage_gemma4a_encoder — same, for the gemma4a arm
require dasllama/dasllama_gemma4uv // Gemma4uvStaging/stage_gemma4uv_embedder — same, for the vision arm
require dasllama/dasllama_gemma4v // Gemma4vStaging/stage_gemma4v_tower — the gemma4v (E-series ViT) arm
require dasllama/dasllama_gemma3v // Gemma3vStaging/stage_gemma3v_tower — the gemma3v (SigLIP) arm
require dasllama/dasllama_vision_embedder // the carrier's .dlim route
require dasllama/dasllama_parakeet // ParakeetStaging/stage_parakeet_model — same, for the parakeet arm
require dasllama/dasllama_config     // DlimConfiguration — the peek/round-trip mechanics arms
require dasllama/dasllama_styletts2  // stage_styletts2/load_styletts2/mint_styletts2 + the two lane tags — the kitten arm's carrier, its family data riding the image meta
require dasllama/dasllama_tts        // load_tts_model/caps/synthesize — the kitten arm's facade leg
require dasllama/dasllama_tts_blocks // TtsSpan — the staged blob's span table
require daslib/defer
require daslib/rtti   // this_context().last_exception: the facade's decline is read as text
require dasllama/dasllama_math   // pin_kernel_backend — the metal-flavor arm's donor contract
require ?das_metal dasllama/dasllama_metal_tower   // set_metal_tower/metal_tower_stats — the gemma4uv-metal arm
require ?das_metal dasllama/dasllama_metal_prefill  // metal_prefill_shutdown — the mtower re-arm leg
require ?das_metal dasllama/dasllama_metal_asr_dec  // set_metal_wdec/metal_wdec_stats — the cross-KV gate + the tower cell's pin
require ?das_metal dasllama/dasllama_metal_decode   // metal_decode_shutdown — same
require dasllama/dasllama_gemm_schema // Q8_BLOCK_ELEMS/Q8_QPB/Q8_SPB — the dev-W bake key arithmetic
require dasllama/dasllama_gpu_tier    // moe_gpu_bake_tag — the lane a cold load under an armed tier mints
require dasllama/dasllama_layout // convert_model_to_metal_blob — the derive cell's in-RAM control
require daslib/jobque_boost
require daslib/archive
require daslib/apply
require daslib/fio
require strings
require math
require dasllama/dasllama_audio_io
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
def private count_mismatch(a : array<auto(TT)>; b : auto(PP)) : int64 {
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
                unsafe(_builtin_forget_temp_array(field))
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
                unsafe(_builtin_forget_temp_array(field))
            } else {
                delete field
            }
        } static_elif (typeinfo is_struct(field)) {
            delete field
        }
    }
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
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

//! the text of the panic the facade raises declining `path` (empty when it loads)
def private facade_decline_of(path : string) : string {
    var why = ""
    try {
        var m <- load_model_cached(path, QuantMode.q8)
        delete m
    } recover {
        why = this_context().last_exception |> rtrim
    }
    return why
}

// the per-format plane table beside a 32-block format's top-level pair - the Model's shape
struct ImgTableProbe {
    @safe_when_uninitialized kq : KqPlanes[KQ_FMT_COUNT]
    q51q : array<uint8>
    q51s : array<uint8>
    iq4nl32q : array<uint8>
    iq4nl32s : array<uint8>
    image_map : void?
    image_bytes : uint64
}

def finalize(var t : ImgTableProbe) {
    apply(t) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            if (lock_count(field) != 0) {
                unsafe(_builtin_forget_temp_array(field))
            } else {
                delete field
            }
        } static_elif (typeinfo is_dim(field)) {
            for (el in field) {
                apply(el) $ [unused_argument(iname)] (iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        if (lock_count(ifield) != 0) {
                            unsafe(_builtin_forget_temp_array(ifield))
                        } else {
                            delete ifield
                        }
                    }
                }
            }
        }
    }
    if (t.image_map != null) {
        image_backing_release(t.image_map, t.image_bytes)
        t.image_map = null
        t.image_bytes = 0ul
    }
}

[unused_argument(arch, t)]
def private serialize_image_meta(var arch : Archive; var t : ImgTableProbe) {}

[unused_argument(t)]
def private image_post_load(var t : ImgTableProbe) {}

def private fill_bytes(var a : array<uint8>; n : int; seed : int) {
    a |> resize(n)
    for (i in range(n)) {
        a[i] = uint8((i * 31 + seed) % 251)
    }
}

[test]
def test_image_table_slot_names(t : T?) {
    t |> run("synthetic carrier: a 32-block format's table slot never writes - its planes are the top-level pair of the same section name") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let path = path_join(test_temp_dir(), "dasllama_img_table.dlim")
        var p = ImgTableProbe()
        fill_bytes(p.kq[int(KqFmt.k4)].q, 4096, 1)
        fill_bytes(p.kq[int(KqFmt.k4)].s, 512, 2)
        fill_bytes(p.q51q, 2048, 3)
        fill_bytes(p.q51s, 256, 4)
        fill_bytes(p.kq[int(KqFmt.q51)].q, 777, 5)
        fill_bytes(p.iq4nl32q, 1024, 6)
        fill_bytes(p.iq4nl32s, 128, 7)
        fill_bytes(p.kq[int(KqFmt.iq4nl32)].q, 555, 8)
        t |> success(save_image(p, path, "img-table"), "save")
        var m = ImgTableProbe()
        t |> success(load_image(path, m, "img-table"), "the image maps back (no two sections share a name)")
        t |> equal(count_mismatch(m.q51q, p.q51q), 0l, "q51q element-exact from the top-level pair")
        t |> equal(count_mismatch(m.q51s, p.q51s), 0l, "q51s element-exact")
        t |> equal(count_mismatch(m.iq4nl32q, p.iq4nl32q), 0l, "iq4nl32q element-exact from the top-level pair")
        t |> equal(count_mismatch(m.iq4nl32s, p.iq4nl32s), 0l, "iq4nl32s element-exact")
        t |> equal(long_length(m.kq[int(KqFmt.iq4nl32)].q), 0l, "the iq4nl32 slot rode no section")
        t |> equal(count_mismatch(m.kq[int(KqFmt.k4)].q, p.kq[int(KqFmt.k4)].q), 0l, "the k4 slot's q plane element-exact")
        t |> equal(count_mismatch(m.kq[int(KqFmt.k4)].s, p.kq[int(KqFmt.k4)].s), 0l, "the k4 slot's s plane element-exact")
        t |> equal(long_length(m.kq[int(KqFmt.q51)].q), 0l, "the q51 slot rode no section")
        delete m
        t |> success(remove(path), "probe image removed")
        delete p
    }
}

[test]
def test_image_tmp_does_not_outlive_a_save(t : T?) {
    t |> run("a failed write removes the save's temp file; a temp file a killed writer left goes at the next save, a fresh one stays") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let path = path_join(test_temp_dir(), "dasllama_img_tmp_probe.dlim")
        remove(path)
        var p <- make_probe()
        let fault_found = get_image_write_fault()
        let hits_found = image_write_fault_hits()
        set_image_write_fault(1ul)
        let saved = save_image(p, path, "img-probe")
        set_image_write_fault(0ul)
        t |> equal(image_write_fault_hits(), hits_found + 1, "the seam failed this save, once")
        t |> success(!saved && !stat(path).is_valid, "a save whose write fails declines and publishes nothing")
        t |> equal(image_tmp_count(path), 0, "the failed save's temp file is gone")
        let stale = "{path}.123.tmp"
        let fresh = "{path}.456.tmp"
        for (planted in fixed_array(stale, fresh)) {
            fopen(planted, "wb") $(f) {
                fwrite(f, "x")
            }
        }
        var mtime_err : string
        t |> success(set_mtime(stale, mktime(2020, 1, 1, 0, 0, 0), mtime_err), "the stale temp file's mtime is set back")
        t |> success(save_image(p, path, "img-probe"), "control: with the seam off the same save lands")
        t |> success(!stat(stale).is_valid, "the next save removes the temp file a killed writer left")
        t |> success(stat(fresh).is_valid, "control: a temp file written a moment ago stays - it may have a live writer")
        remove(fresh)
        remove(path)
        set_image_write_fault(fault_found)
        delete p
    }
}

[test]
def test_image_mechanics(t : T?) {
    t |> run("synthetic carrier: round trip, nested planes, declines, unmap-remap") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let path = path_join(test_temp_dir(), "dasllama_img_probe.dlim")
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
        t |> equal(image_identity_of(pcfg, "img-probe"), pk.identity, "peek: config re-derives the identity")

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
                unsafe(_builtin_forget_temp_array(owned))
            } recover {
                caught = true
            }
            break
        }
        t |> success(caught, "forget on an owned locked array is a loud error")
        delete owned
    }
    t |> run("synthetic carrier: the facade's decline names the minted version when that is the reason") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let path = path_join(test_temp_dir(), "dasllama_img_stale.dlim")
        var p <- make_probe()
        t |> success(save_image(p, path, "img-probe"), "save")
        delete p
        let foreign = facade_decline_of(path)
        t |> success(find(foreign, "is not a prepared image for this box/knobs") >= 0, "an image of this version under a foreign identity declines as a wrong-box image:\n{foreign}")
        t |> success(find(foreign, "was minted at IMAGE_VERSION") < 0, "and says nothing about a version that matches:\n{foreign}")
        patch_u32(t, path, 4l, 0xDEADu)
        let stale = facade_decline_of(path)
        t |> success(find(stale, "was minted at IMAGE_VERSION {int(0xDEADu)}") >= 0, "a stale image's decline names the version it was minted at:\n{stale}")
        t |> success(find(stale, "this build reads {IMAGE_VERSION}") >= 0, "and the version this build reads:\n{stale}")
        t |> success(remove(path), "stale image removed")
    }
    t |> run("synthetic carrier: the cold rail builds in memory, tees to disk, serves the chunk") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let path = path_join(test_temp_dir(), "dasllama_img_chunk.dlim")
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
        t |> equal(image_identity_of(back), image_identity_of(dc), "identity survives the round trip")
        var junk = DlimConfiguration()
        t |> success(!dlim_config_from_json("not a config", junk), "garbage declines")
    }
    t |> run("lanes: the file name spells the lane, a legacy image is adopted, a save reaps its own lane only, a stale lane re-bakes in place") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let fake = path_join(test_temp_dir(), "dasllama_lane.gguf")
        let fk = fopen(fake, "wb")   // the inventory scans the siblings of an existing source file
        t |> success(fk != null, "the fake source exists")
        fclose(fk)
        t |> equal(dlim_file_lane("m.gguf.metal.dlim"), "metal", "the last dot-segment is the lane")
        t |> equal(dlim_file_lane("m.Q4_K_M.gguf.cpu-arm-i8mm-fp32.dlim"), "cpu-arm-i8mm-fp32", "the source's own dots stay out of the lane")
        t |> equal(dlim_file_lane("m.gguf.0xc09d7ce0fafeac5c.dlim"), "", "a hash-named legacy image has no lane")
        t |> equal(dlim_file_lane("m.gguf"), "", "a file that is no image has no lane")
        t |> equal(dlim_file_lane("m.dlim"), "", "an image with no lane segment has none")
        let prev_rope = rope_scaling_override()
        set_rope_scaling_override(RopeScalingOverride())   // the cells below spell lanes with the override off; the env may have seeded one
        t |> equal(image_lane_name("whisper-f32"), "whisper-f32", "a family tag is its own lane")
        t |> equal(image_lane_name(METAL_IMAGE_TAG), "metal", "the blob flavor's lane")
        t |> equal(image_lane_name("vulkan|m2n b8192 l4"), "vulkan", "the vulkan plan folds to one lane")
        t |> equal(image_lane_name("a b|c/d"), "a-b-c-d", "a lane keeps letters, digits, _ and - alone")
        t |> equal(image_lane_name("", "fp32"), "{image_lane_name()}-fp32", "an off-q8 quant suffixes the lane")
        t |> equal(image_lane_name(METAL_IMAGE_TAG, "q8", true), "metal-mtp", "a split head riding the load suffixes the lane")
        static_if (typeinfo builtin_module_exists(llvm)) {
            t |> equal(image_lane_name(), "cpu-{tune_cpu_class()}", "the planar lane names the box's tune class")
        } else {
            t |> equal(image_lane_name(), "cpu-{get_architecture_name()}", "the planar lane names the architecture where no tune framework is built")
        }
        let eight = "v{IMAGE_VERSION}|q8|arm64-gen|s16|q8 mr8 b0 g4|kq 8/8/4/8|q51 mr4|nat 1101"
        t |> equal(dlim_identity_lane(eight), eight, "an identity short of DLIM_IDENTITY_FIELDS is its own lane, never cross-matched")
        // two images baked under the box-bound identity, then the tags registered config-free: the
        // same lanes, another identity - OTHER, never FOREIGN
        var p <- make_probe()
        let stray = "{fake}.0xfeedfacecafebeef.dlim"
        t |> success(save_image(p, stray, "img-lane"), "a legacy-named image of the lane's (quant, tag) off the hash path")
        let lane2 = "{fake}.img-lane-2.dlim"
        t |> success(save_image(p, lane2, "img-lane-2"), "a second lane's image")
        register_image_family_tag("img-lane", true)
        register_image_family_tag("img-lane-2", true)
        t |> equal(dlim_identity_tag(image_identity("img-lane")), "img-lane", "the identity writes DLIM_IDENTITY_FIELDS fields before the tag")
        // the image a pre-lane build minted: the identity hash names it, the lane adopts it
        let legacy = "{fake}.{hash(image_identity("img-lane", "q8"))}.dlim"
        t |> success(save_image(p, legacy, "img-lane"), "a legacy-named image at the identity-hash path")
        let path = image_path_for(fake, "img-lane")
        t |> equal(base_name(path), "{base_name(fake)}.img-lane.dlim", "the lane path")
        t |> success(!stat(legacy).is_valid && stat(path).is_valid, "the image at the identity-hash path is adopted into the lane")
        var inv <- dlim_inventory_(fake)
        t |> equal(length(inv), 3, "three images beside the source")
        for (im in inv) {
            t |> equal(im.verdict, im.path == path ? "CURRENT" : "OTHER", "{im.file}: the box-bound identity against the config-free one")
            t |> equal(im.lane, im.path == stray ? "" : (im.path == path ? "img-lane" : "img-lane-2"), "{im.file}: the lane the name spells")
        }
        delete inv
        // the lane's own save: the image at the lane path is rewritten; its GC reaps the legacy-named other of
        // its own (quant, tag) and leaves the other lane's OTHER alone
        t |> success(save_image(p, path, "img-lane"), "re-bake in place")
        dlim_gc_stale(fake, "img-lane")
        t |> success(!stat(stray).is_valid, "a legacy-named image of this lane's (quant, tag) under another identity goes with the save")
        t |> success(stat(lane2).is_valid, "another lane's OTHER image survives this lane's save")
        var m = ImgProbe()
        t |> success(load_image(path, m, "img-lane"), "the re-baked lane image maps")
        delete m
        // the explicit clean: a lane-named OTHER stays
        let r = dlim_clean_(fake, true)
        t |> equal(r.removed, 0, "the clean removes no lane-named image ({r.removed})")
        t |> success(stat(path).is_valid && stat(lane2).is_valid, "both lanes stay")
        // version-stale goes in any lane
        patch_u32(t, lane2, 4l, 0xDEADu)
        dlim_gc_stale(fake, "img-lane")
        t |> success(!stat(lane2).is_valid, "a version-stale image of another lane goes")
        delete p
        remove(path)
        remove(fake)
        set_rope_scaling_override(prev_rope)
    }
    t |> run("lanes: the rope-scaling override is a lane suffix and an identity element") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let prev = rope_scaling_override()
        set_rope_scaling_override(RopeScalingOverride())   // the plain spellings, whatever DASLLAMA_ROPE_SCALING seeded ({prev.kind})
        let plain_lane = image_lane_name()
        let plain_id = image_identity()
        set_rope_scaling_override(RopeScalingOverride(kind = "YARN", scale = 4.0, orig_ctx = 32768l))
        t |> equal(rope_scaling_override().kind, "yarn", "the kind reads lower-case")
        t |> equal(image_lane_name(), "{plain_lane}-yarn4", "the planar lane takes the yarn suffix")
        t |> equal(image_lane_name(METAL_IMAGE_TAG), "metal-yarn4", "so does the blob lane")
        register_image_family_tag("img-lane-free", true)
        t |> equal(image_lane_name("img-lane-free"), "img-lane-free", "a config-free family's lane takes none - its identity carries no override")
        t |> success(find(image_identity(), " rope yarn4/32768") >= 0, "the identity spells the factor and the original context ({image_identity()})")
        t |> equal(dlim_identity_tag(image_identity("img-lane")), "img-lane", "the element rides inside the last fixed field - the tag still parses")
        set_rope_scaling_override(RopeScalingOverride(kind = "yarn", scale = 4.0, orig_ctx = 32768l, beta_fast = 16.0, beta_slow = 2.0))
        t |> success(find(image_identity(), " rope yarn4/32768/16/2") >= 0, "set betas ride the identity ({image_identity()})")
        t |> equal(image_lane_name(), "{plain_lane}-yarn4", "and not the lane")
        set_rope_scaling_override(RopeScalingOverride(kind = "linear", scale = 2.5))
        t |> success(find(image_identity(), " rope linear2.5") >= 0, "a linear factor spells as it reads ({image_identity()})")
        t |> equal(image_lane_name(), "{plain_lane}-linear2-5", "the linear lane - the dot yields to the file name's segment reader")
        set_rope_scaling_override(RopeScalingOverride(kind = "none"))
        t |> equal(image_lane_name(), "{plain_lane}-norope", "dropping a file's scaling is its own lane")
        t |> success(find(image_identity(), " rope none") >= 0, "and its own identity ({image_identity()})")
        set_rope_scaling_override(RopeScalingOverride())
        t |> equal(image_identity(), plain_id, "the override off leaves the identity as it was")
        set_rope_scaling_override(prev)
    }
}

// ===== real carriers: gguf load vs mapped image load =====

// first config field whose value differs between two loads ("" = none) — catches an arch
// configure clobbering serialized gguf overrides even when short gens can't (sliding_window
// only bites past the window)
def private config_diff_field(a, b : Config) : string {
    var names, vals : array<string>
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
    let img = image_path_for(path, moe_gpu_bake_tag())   // each lane is its mode's: an armed tier's cold load mints the vulkan lane
    remove(img)   // cold start: the first facade load must build + save
    // the truth is the OFF-RAIL gguf read: cold and warm are both image-backed now, so a
    // cold-vs-warm compare alone would only prove the file matches the chunk it came from
    var g <- load_model_(path, QuantMode.q8)
    var cfg_a := g.config   // kept past delete for the round-trip compare
    if (g.config.seq_len > 2048l) {
        g.config.seq_len = 2048l
    }
    var prompt <- encode(g, "Once upon a time")
    var ga <- greedy_gen(g, prompt, n_gen)
    delete g
    var a <- load_model(path, QuantMode.q8)
    t |> success(a.image_map != null, "cold load serves the image it built")
    t |> success(stat(img).is_valid, "cold load saved an image")
    if (a.config.seq_len > 2048l) {
        a.config.seq_len = 2048l
    }
    var gcold <- greedy_gen(a, prompt, n_gen)
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
    var gb <- greedy_gen(b, prompt, n_gen)
    t |> success(!empty(ga) && count_mismatch(ga, gb) == 0l, "gguf vs image tokens")
    delete b   // unmaps
    var c <- load_model(path, QuantMode.q8)   // the file survives an unmap — map again
    t |> success(c.image_map != null, "re-mapped after delete")
    if (c.config.seq_len > 2048l) {
        c.config.seq_len = 2048l
    }
    var gc <- greedy_gen(c, prompt, n_gen)
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
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        text_image_roundtrip(t, path, 24)
    }
    // the untied twin: a separate output.weight, so the image carries the classifier plane AND
    // the embedding's own linear q8 copy (emb_q8) with no fp32 table - the smol arm never sees that layout
    t |> run("stories15M q8 (untied): gguf load vs mapped image, token-for-token") @(t : T?) {
        if (!family_on(t, "llama") || !arm_on(t, "untied")) return
        let path = path_join(models_dir(), "stories15M-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        text_image_roundtrip(t, path, 24)
        var m <- load_model(path, QuantMode.q8)
        t |> success(m.emb_q8 && !m.cls_q8 && m.tok_emb_off < 0l, "the mapped image serves the untied q8 table with no fp32 copy")
        // the whole image less its device twin is smaller than the f32 table alone (27 MB against 37 MB on stories15M): an
        // image that still carried the table could not be, whatever else it packs
        let img_bytes = int64(stat(image_path_for(path, moe_gpu_bake_tag())).size) - long_length(m.vkblob)
        let table_f32 = m.config.vocab_size * m.config.dim * 4l
        t |> success(img_bytes < table_f32, "the image ({img_bytes >> 20l} MB past its twin) is smaller than the f32 table it no longer carries ({table_f32 >> 20l} MB)")
        delete m
    }
}

// ===== the metal (blob-only) flavor =====

// counting prompt: confident continuations — the decode-parity suite's CPU-vs-GPU token bar
def private counting_ids(tr : Model) : array<int64> =>
    <- encode(tr, "The numbers from one to ten are: one, two, three,")

// Teacher-forced max-logits-diff: both models step through the SAME token sequence and the
// per-step logits compare — the valid cross-backend instrument for a 135M (greedy token
// equality is not: the model rides genuine near-ties that flip on ~0.02 gaps, while real
// breakage moves logits 15+; measured cross-backend noise here is ~0.75). With metal mode
// on, the planar model CPU-serves (the gates decline `planar`) and the blob model GPU-serves
// — no override juggling.

def private forced_logits_maxd(tc, tg : Model; prompt, teach : array<int64>; tag : string) : float {
    var maxd = 0.0
    var picks : array<int64>   // the GPU's greedy would-be choice per teach position — eyeball rail
    with_dasllama_jobque() {
        var sa = make_run_state(tc.config)
        var sb = make_run_state(tg.config)
        eval_(tc, sa, prompt)
        eval_(tg, sb, prompt)
        for (a, b in sa.logits, sb.logits) {
            maxd = max(maxd, abs(a - b))
        }
        for (tok in teach) {
            picks |> push(argmax_of(sb.logits))
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
            return if (!family_on(t, "llama") || !arm_on(t, "metal") || !jit_only(t))
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
            var g_cpu <- greedy_gen(tc, prompt, 16)
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
            var g_mint <- greedy_gen(tm, prompt, 16)   // a blob model CANNOT silently CPU-serve —
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
            var g_map <- greedy_gen(tw, prompt, 16)
            t |> success(count_mismatch(g_mint, g_map) == 0l, "mint vs mapped tokens")
            delete tw
            var td <- load_model(img_m, QuantMode.q8)
            t |> success(td.metal_blob, "direct .dlim load resolves the metal identity")
            var g_dir <- greedy_gen(td, prompt, 16)
            t |> success(count_mismatch(g_mint, g_dir) == 0l, "direct-path tokens")
            // the CPU tripwire: with the overrides off, any inference on a blob model panics
            set_metal_mode(MetalMode.off)
            var tripped = false
            try {
                var g_bad <- greedy_gen(td, prompt, 2)
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
    // the untied twin on the blob flavor: TinyLlama carries its own output.weight, so the blob plane
    // holds the classifier AND the embedding's linear q8 copy (emb_q8 without cls_q8) - the layout
    // the SmolLM2 cell never reaches: the blob gate admits the copy, the transform carries it into
    // mblob, and embed_row reads it there
    tst |> run("TinyLlama-1.1B q8 (untied): metal flavor - blob mint + map + GPU serve on the emb_q8 table") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!family_on(t, "llama") || !arm_on(t, "metal-untied") || !jit_only(t))
            if (!has_decode_override("metal")) {
                t |> skip("metal overrides not registered")
                return
            }
            let path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            let prior_backend = active_kernel_backend()
            pin_kernel_backend("portable")
            set_metal_mode(MetalMode.off)
            var tc <- load_model(path, QuantMode.q8)
            t |> success(!tc.metal_blob && tc.emb_q8 && !tc.cls_q8, "CPU-mode load stays planar and serves the untied q8 table")
            let table_floats = tc.config.vocab_size * tc.config.dim
            var prompt <- counting_ids(tc)
            var g_cpu <- greedy_gen(tc, prompt, 16)
            delete tc
            let img_m = image_path_for(path, METAL_IMAGE_TAG)
            remove(img_m)
            set_metal_mode(MetalMode.when_available)
            var tm <- load_model(path, QuantMode.q8)
            t |> success(tm.metal_blob, "metal-mode load is blob-form - the layout gate admits the untied emb_q8 copy")
            t |> success(tm.image_map != null && stat(img_m).is_valid, "cold metal load saved and serves the metal-flavor image")
            t |> success(tm.emb_q8 && !tm.cls_q8 && tm.tok_emb_off < 0l && long_length(tm.fblob) < table_floats,
                "the blob flavor keeps the untied q8 table and no fp32 copy (fblob {long_length(tm.fblob)} floats against a {table_floats}-float table)")
            var g_mint <- greedy_gen(tm, prompt, 16)   // every token's embedding row read from the blob copy
            var tp <- load_model_(path, QuantMode.q8)
            let np = long_length(prompt)
            var teach <- [for (i in range64(np - 1l, long_length(g_cpu))); g_cpu[i]]
            let maxd = forced_logits_maxd(tp, tm, prompt, teach, "TinyLlama metal forced")
            t |> success(maxd < 4.0, "teacher-forced GPU logits within 4.0 of the CPU control (maxd {maxd})")
            delete teach
            delete tp
            delete tm
            var tw <- load_model(path, QuantMode.q8)
            t |> success(tw.image_map != null && tw.metal_blob && tw.emb_q8 && !tw.cls_q8, "warm metal load maps the blob image with the untied q8 table")
            var g_map <- greedy_gen(tw, prompt, 16)
            t |> success(count_mismatch(g_mint, g_map) == 0l, "mint vs mapped tokens")
            delete tw
            delete g_cpu
            delete g_mint
            delete g_map
            delete prompt
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
        }
    }
}

[test]
def test_gemma4e_flavor_image(tst : T?) {
    tst |> run("E2B q8: the metal-flavor image serves the PLE table (mint + map, the go-live tripwire seam)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!family_on(t, "gemma4e") || !arm_on(t, "gemma4e") || !jit_only(t))
            if (!has_decode_override("metal")) {
                t |> skip("metal overrides not registered")
                return
            }
            let path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            // cold metal mint: the freshly baked blob image must clear the PLE go-live tripwire
            // (ple_check_table once ran BEFORE the plane borrows and false-panicked on every
            // fresh mint) and then serve — a blob model cannot silently CPU-serve
            let prior_backend = active_kernel_backend()
            pin_kernel_backend("portable")   // BEFORE image_path_for — the identity carries the pin
            set_metal_mode(MetalMode.when_available)
            let img_m = image_path_for(path, METAL_IMAGE_TAG)
            remove(img_m)
            var tm <- load_model(path, QuantMode.q8)
            tm.config.seq_len = min(tm.config.seq_len, 512l)
            t |> success(tm.metal_blob, "metal-mode load is blob-form")
            t |> success(stat(img_m).is_valid, "cold metal load saved the metal-flavor image")
            var prompt <- counting_ids(tm)
            var g_mint <- greedy_gen(tm, prompt, 8)
            t |> success(length(g_mint) > length(prompt), "mint-backed blob model serves a continuation")
            delete tm
            // warm map: the tripwire's after-borrows seam again, now off the mapped planes
            var tw <- load_model(path, QuantMode.q8)
            tw.config.seq_len = min(tw.config.seq_len, 512l)
            t |> success(tw.image_map != null && tw.metal_blob, "warm metal load maps the blob image")
            var g_map <- greedy_gen(tw, prompt, 8)
            log_gen_texts(tw, "E2B mint-vs-mapped", g_mint, g_map)
            t |> success(count_mismatch(g_mint, g_map) == 0l,
                "mapped-image tokens match the mint's ({count_mismatch(g_mint, g_map)} differ)")
            delete tw
            set_metal_mode(MetalMode.off)
            delete g_mint
            delete g_map
            delete prompt
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
        } else {
            feint("das_metal is not built on this platform - the metal-flavor image is Apple-only\n")
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
            let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
            return if (!jit_model_available(t, path))
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
            t |> equal(count_mismatch(tr.kq[int(KqFmt.k4)].s, tm.kq[int(KqFmt.k4)].s), 0l, "k4 scale strip")
            t |> equal(count_mismatch(tr.kq[int(KqFmt.k5)].s, tm.kq[int(KqFmt.k5)].s), 0l, "k5 scale strip")
            t |> equal(count_mismatch(tr.kq[int(KqFmt.k6)].s, tm.kq[int(KqFmt.k6)].s), 0l, "k6 scale strip")
            delete tm
            delete tr
            set_metal_mode(MetalMode.off)
            clear_kernel_backend_pin()
            select_kernel_backend(prior_backend)
        }
    }
}

// elements of the served view that differ from the staging blob's span (the off-rail truth)
def private tts_span_mismatch(blob : array<float>; span : TtsSpan; view : array<float>) : int {
    if (long_length(view) != span.n) {
        return 1000000
    }
    var bad = 0
    for (i in range64(span.n)) {
        if (view[i] != blob[span.off + i]) {
            bad++
        }
    }
    return bad
}

// the same, for a weight the q8 lane carries as int8 quants
def private tts_span_mismatch_q8(qblob : array<int8>; span : TtsSpan; view : array<int8>) : int {
    if (long_length(view) != span.n) {
        return 1000000
    }
    var bad = 0
    for (i in range64(span.n)) {
        if (view[i] != qblob[span.off + i]) {
            bad++
        }
    }
    return bad
}

struct KittenRoundtrip {
    cold_backed : bool
    warm_backed : bool
    minted_backed : bool
    lane_q8 : bool
    quantized : bool
    mismatches : int
    voices : int
}

// the image rail on the kitten-nano GGUF, once per lane: a fresh staged read on that lane is
// the off-rail truth; the cold load (a bake or a map, both image-backed) must serve its bytes;
// the warm load maps; the in-memory mint serves without a file
def private kitten_image_roundtrip(path : string; q8 : bool) : KittenRoundtrip {
    set_styletts2_q8(q8)
    defer() {
        reset_styletts2_q8()
    }
    var r = KittenRoundtrip()
    var inscope st <- stage_styletts2(path, q8)
    var inscope m <- load_styletts2(path)
    r.cold_backed = m.image_map != null
    r.lane_q8 = m.q8
    if (q8) {
        // the rows GEMMs' weights ride the int8 plane, their Q8_0 scales the float one
        r.quantized = !empty(m.albert.q.wq)
        r.mismatches += tts_span_mismatch_q8(st.qblob, m.albert.q.wq_span, m.albert.q.wq)
        r.mismatches += tts_span_mismatch(st.blob, m.albert.q.wqs_span, m.albert.q.wqs)
    } else {
        r.mismatches += tts_span_mismatch(st.blob, m.albert.q.wt_span, m.albert.q.wt)
        r.mismatches += tts_span_mismatch(st.blob, m.decoder.resblocks[0].convs1[0].wk_span, m.decoder.resblocks[0].convs1[0].wk)
    }
    r.mismatches += tts_span_mismatch(st.blob, m.decoder.ups[0].wtr_span, m.decoder.ups[0].wtr)
    r.mismatches += tts_span_mismatch(st.blob, m.decoder.window.span, m.decoder.window.a)
    r.mismatches += tts_span_mismatch(st.blob, m.text_encoder.cnn[0].w_span, m.text_encoder.cnn[0].w)
    for (name in m.voice_names) {
        r.mismatches += tts_span_mismatch(st.blob, m.voice_spans?[name] ?? TtsSpan(), m.voices[name])
        r.voices++
    }
    var inscope warm <- load_styletts2(path)
    r.warm_backed = warm.image_map != null
    var inscope st2 <- stage_styletts2(path, q8)
    var inscope minted = StyleTts2Model()
    mint_styletts2(st2, minted)
    r.minted_backed = minted.image_map != null
    r.mismatches += tts_span_mismatch(st.blob, minted.text_encoder.cnn[0].w_span, minted.text_encoder.cnn[0].w)
    return r
}

// the TTS area's image arm: the StyleTTS2 carrier through its own loader (load_styletts2),
// on both lanes, and the image loaded by its own path with the kitten family data riding its meta
[test, arch(at = "../ARCHITECTURE_TTS.md#tts-image-rail")]
def test_kitten_image(t : T?) {
    let path = path_join(models_dir(), "kitten-nano.gguf")
    t |> run("kitten-nano: the staged read, the mapped image and the in-memory mint agree on both lanes") @(t : T?) {
        return if (!family_on(t, "kitten") || !arm_on(t, "kitten") || !jit_model_available(t, path))
        with_dasllama_jobque_() {
            for (q8 in [false, true]) {
                let lane = q8 ? "q8" : "f32"
                let r = kitten_image_roundtrip(path, q8)
                t |> success(r.cold_backed, "{lane}: the cold load serves from an image")
                t |> success(r.warm_backed, "{lane}: the warm load maps the image")
                t |> success(r.minted_backed, "{lane}: the in-memory mint serves from a chunk")
                t |> equal(r.lane_q8, q8, "{lane}: the served model carries the lane it was minted on")
                t |> equal(r.voices, 8)
                t |> equal(r.mismatches, 0, "{lane}: served elements differing from the staged blob")
            }
            let r8 = kitten_image_roundtrip(path, true)
            t |> success(r8.quantized, "q8: the rows GEMMs' weights are Q8_0 quants")
        }
    }
    t |> run("kitten-nano: the two lanes are two files") @(t : T?) {
        if (!family_on(t, "kitten") || !arm_on(t, "kitten")) return
        let f32_img = image_path_for(path, ST2_IMAGE_TAG)
        let q8_img = image_path_for(path, ST2_IMAGE_TAG_Q8)
        t |> success(f32_img != q8_img, "each lane is its own prepared image:\n  {f32_img}\n  {q8_img}")
    }
    t |> run("kitten-nano: the image loads by its own path, the family data riding its meta") @(t : T?) {
        return if (!family_on(t, "kitten") || !arm_on(t, "kitten") || !jit_model_available(t, path))
        with_dasllama_jobque_() {
            var inscope from_gguf <- load_styletts2(path)   // bakes the q8 sibling when it is not there yet
            let img = image_path_for(path, ST2_IMAGE_TAG_Q8)
            t |> success(stat(img).is_valid, "the q8 lane's image is on disk: {img}")
            set_styletts2_q8(false)
            defer() {
                reset_styletts2_q8()
            }
            var inscope m <- load_styletts2(img)
            t |> success(m.q8, "the file's lane is the served lane")
            t |> equal(length(m.voice_names), 8)
            t |> equal(length(m.kitten.aliases), length(from_gguf.kitten.aliases), "the aliases rode the image ({length(m.kitten.aliases)})")
            t |> equal(length(m.kitten.speed_priors), length(from_gguf.kitten.speed_priors), "the speed priors rode the image ({length(m.kitten.speed_priors)})")
            t |> success(!empty(m.kitten.aliases) && !empty(m.kitten.speed_priors), "the gguf carries family data to compare")
            for (an, av in keys(m.kitten.aliases), values(m.kitten.aliases)) {
                t |> equal(from_gguf.kitten.aliases?[an] ?? "", av, "alias {an}")
            }
            if (stat(path_join(models_dir(), "tts_g2p.bin")).is_valid && stat(path_join(models_dir(), "tts_postag.bin")).is_valid) {
                var inscope tm <- load_tts_model(img)
                var inscope c <- caps(tm)
                var inscope a <- synthesize(tm, "The image spoke.", c.voices[length(c.voices) - 1], 1.0)
                t |> success(long_length(a.pcm) > 8000l, "the facade synthesized {long_length(a.pcm)} samples from the image")
            }
        }
    }
}

[test]
def test_voxtral_mini_image(t : T?) {
    t |> run("voxtral-mini q8: the >2 GiB-plane image round trip") @(t : T?) {
        if (!family_on(t, "voxtral") || !arm_on(t, "voxtral")) return
        let path = path_join(models_dir(), "voxtral-mini-3b-q8_0.gguf")
        return if (!jit_model_available(t, path))
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
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        text_image_roundtrip(t, path, 16)
    }
}

[test]
def test_tower_image_roundtrip(t : T?) {
    t |> run("ultravox-1b tower: mapped image planes are element-exact vs gguf") @(t : T?) {
        if (!family_on(t, "ultravox") || !arm_on(t, "tower")) return
        let path = path_join(models_dir(), "mmproj-ultravox-1b-f32.gguf")
        return if (!jit_model_available(t, path))
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
    with_dasllama_jobque() {
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
        let path = path_join(whisper_dir(), "ggml-tiny.bin")
        return if (!jit_model_available(t, path))
        var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
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
        t |> equal(count_mismatch(stg8.dqblob, w8.dqblob), 0l, "streamed dqblob element-exact vs staged")
        t |> equal(count_mismatch(stg8.dqscales, w8.dqscales), 0l, "streamed dqscales element-exact vs staged")
        t |> equal(count_mismatch(stg8.enc.qblob, w8.enc.qblob), 0l, "streamed enc.qblob element-exact vs staged")
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
        t |> equal(count_mismatch(stgm.dqblob, wm.dqblob), 0l, "mixed streamed dqblob element-exact vs staged")
        t |> equal(count_mismatch(stgm.dqscales, wm.dqscales), 0l, "mixed streamed dqscales element-exact vs staged")
        t |> equal(count_mismatch(stgm.dblob, wm.dblob), 0l, "mixed streamed dblob element-exact vs staged")
        t |> equal(count_mismatch(stgm.enc.fblob, wm.enc.fblob), 0l, "mixed enc.fblob element-exact (full f32 layout)")
        var inscope am <- load_asr_model(imgm)
        t |> success(am.kind == AsrKind.whisper, "facade routes the mixed tag")
        t |> success(!am.whisper.enc.q8 && am.whisper.dq8, "facade .dlim route lands the mixed halves")
        // the REJECTED half pair: enc-q8/dec-f32 stages but has no serving mode - the mint panics
        var inscope stgb <- stage_whisper_model(path, true, false)
        var inscope wb = WhisperModel()
        var tag_tripped = false
        try {
            mint_whisper_model(stgb, wb, true, false)
        } recover {
            tag_tripped = true
        }
        t |> success(tag_tripped, "enc-q8/dec-f32 mint panics (not a served combination)")
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
            let path = path_join(whisper_dir(), "ggml-tiny.bin")
            return if (!jit_model_available(t, path))
            var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
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
            t |> equal(s2.convs - s1.convs, chunks, "tiny's 3 x 80 conv columns pad to 256: the conv front served on the device")
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
                t |> equal(s4.convs - s3.convs, 1l, "large: the conv frontend served on the GPU")
                to_log(LOG_INFO, "whisper large mtower CPU eyeball: '{lc.text}'\n")
                to_log(LOG_INFO, "whisper large mtower GPU eyeball: '{lg.text}'\n")
                t |> success(!empty(lg.text), "large: non-empty transcript")
                t |> success(lc.text == lg.text, "large: transcript-exact CPU vs GPU")
            }
            set_metal_tensor_crowns("mulmm_q8")
            metal_prefill_shutdown()
            let tw0 = metal_tower_f16_encodes()
            let twg = whisper_transcribe_text(path, samples, cached = false)
            t |> success(metal_tower_f16_encodes() - tw0 >= 1l, "the f16-sourced tower rode the twin route")
            t |> success(twg.text == cpu.text, "twin route: transcript-exact vs CPU")
            set_metal_tower_f16(false)
            let tw1 = metal_tower_f16_encodes()
            let twf = whisper_transcribe_text(path, samples, cached = false)
            set_metal_tower_f16(true)
            t |> equal(metal_tower_f16_encodes(), tw1, "twin knob off: the route never engaged")
            t |> success(twf.text == cpu.text, "twin knob off: the f32 route stays transcript-exact")
            var inscope wpz = WhisperModel()
            var inscope stgz <- stage_whisper_model(path, false)
            t |> success(long_length(stgz.enc.wblob) > 0l, "the f16 bin baked a twin at stage")
            let zbase = stgz.e.enc.layers_off + 2l * stgz.e.enc.layer_stride
            for (i in range64(long_length(stgz.enc.wblob) > 0l ? stgz.e.enc.layer_stride : 0l)) {
                stgz.enc.wblob[zbase + i] = uint16(0)
            }
            mint_whisper_model(stgz, wpz)
            var z_text = ""
            with_dasllama_jobque() {
                var sz <- create_session(wpz)
                transcribe(wpz, sz, samples) $(seg) {
                    z_text += seg.text
                }
                delete sz
            }
            t |> success(z_text != cpu.text, "a zeroed twin layer CHANGES the GPU transcript (the twin is read)")
            set_metal_tensor_crowns("")
            metal_prefill_shutdown()
            // the q8 SERVING DEFAULT serves on the tower: its block GEMMs read the q8 planes as a device blob
            var inscope w8 = WhisperModel()
            var inscope stg8 <- stage_whisper_model(path, true)
            mint_whisper_model(stg8, w8)
            t |> success(w8.enc.q8, "the q8 flavor is on the q8 rail")
            set_metal_tower(false)
            let q8_cpu = wdec_transcribe(w8, samples)
            set_metal_tower(true)
            let sq0 = metal_tower_stats()
            let tq0 = metal_tower_f16_encodes()
            let dq0 = metal_tower_declines()?["quant_mode"] ?? 0l
            let q8_gpu = wdec_transcribe(w8, samples)
            let sq1 = metal_tower_stats()
            to_log(LOG_INFO, "whisper tiny q8 mtower CPU eyeball: '{q8_cpu}'\n")
            to_log(LOG_INFO, "whisper tiny q8 mtower GPU eyeball: '{q8_gpu}'\n")
            t |> equal(sq1.encodes - sq0.encodes, 1l, "q8: one GPU chunk")
            t |> equal(sq1.blocks - sq0.blocks, 4l, "q8: blocks delta = n_layer (tiny = 4)")
            t |> equal((metal_tower_declines()?["quant_mode"] ?? 0l) - dq0, 0l, "q8: no quant_mode decline")
            t |> equal(metal_tower_f16_encodes(), tq0, "q8: the halfword twin route stays out of it")
            t |> success(!empty(q8_gpu) && q8_gpu == q8_cpu, "q8: transcript-exact CPU vs GPU")
            // the control: a q8 tower with one layer's GEMM planes zeroed, through the device chain, reads another text
            var inscope wz8 = WhisperModel()
            var inscope stgz8 <- stage_whisper_model(path, true)
            let zq = stgz8.e.enc.qlayers_off + 2l * stgz8.e.enc.qlayer_stride
            for (i in range64(stgz8.e.enc.qlayer_stride)) {
                stgz8.enc.qblob[zq + i] = int8(0)
            }
            mint_whisper_model(stgz8, wz8)
            let sz0 = metal_tower_stats()
            let zq_text = wdec_transcribe(wz8, samples)
            t |> equal(metal_tower_stats().encodes - sz0.encodes, 1l, "q8 control: the zeroed tower served on the device")
            t |> success(zq_text != q8_gpu, "q8 control: a zeroed layer CHANGES the GPU transcript (the q8 blob is read)")
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
    with_dasllama_jobque() {
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
            let path = path_join(whisper_dir(), "ggml-tiny.bin")
            return if (!jit_model_available(t, path))
            var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present")
                return
            }
            var inscope w8 = WhisperModel()
            var inscope stg8 <- stage_whisper_model(path, true)
            mint_whisper_model(stg8, w8)
            // GPU leg: the q8 serving default's cross-KV AND decode steps land on the driver
            // (the step floor drops to 0 — tiny sits under the serving floor by design)
            set_metal_wdec(true)
            set_metal_wdec_step(true)
            set_metal_wdec_step_min_d(0l)
            let s0 = metal_wdec_stats()
            let gpu_text = wdec_transcribe(w8, samples)
            let s1 = metal_wdec_stats()
            t |> equal(s1.windows - s0.windows, 1l, "jfk is one cross-KV window")
            t |> success(s1.steps - s0.steps >= 20l, "the decode batches served on the GPU")
            t |> success(s1.tokens - s0.tokens >= s1.steps - s0.steps, "token rows cover the steps")
            // OFF leg: knob decline counted, CPU serves, transcript identical
            set_metal_wdec(false)
            let cpu_text = wdec_transcribe(w8, samples)
            let s2 = metal_wdec_stats()
            set_metal_wdec(true)
            t |> equal(s2.windows, s1.windows, "OFF leg never serves")
            t |> equal(s2.steps, s1.steps, "OFF leg never steps")
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
                with_dasllama_jobque() {
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
            t |> success(metal_wdec_stats().steps - sr0.steps >= 20l, "post-shutdown decode re-arms too")
            to_log(LOG_INFO, "whisper tiny cross-kv rearm eyeball: '{rearm_text}'\n")
            t |> success(rearm_text == gpu_text, "post-shutdown transcript unchanged")
            // the serving FLOOR: at the default min_d, tiny cross-KV-serves but never steps,
            // and the small decline is recorded as policy
            set_metal_wdec_step_min_d(METAL_WDEC_STEP_MIN_D)
            let sm0 = metal_wdec_stats()
            let small_text = wdec_transcribe(w8, samples)
            let sm1 = metal_wdec_stats()
            t |> equal(sm1.windows - sm0.windows, 1l, "under the floor: cross-KV still serves")
            t |> equal(sm1.steps, sm0.steps, "under the floor: the step never serves")
            t |> success(key_exists(metal_wdec_declines(), "small"), "the floor decline is recorded")
            t |> equal(sm1.declines - sm0.declines, 0l, "a policy note is not a decline")
            to_log(LOG_INFO, "whisper tiny cross-kv floor eyeball: '{small_text}'\n")
            t |> success(small_text == cpu_text, "under the floor: the CPU decode transcript")
            // the STEP KNOB: cross-KV serves, the step stays CPU, and the reason is readable
            set_metal_wdec_step_min_d(0l)
            set_metal_wdec_step(false)
            let sk0 = metal_wdec_stats()
            let sk_text = wdec_transcribe(w8, samples)
            let sk1 = metal_wdec_stats()
            set_metal_wdec_step(true)
            t |> equal(sk1.windows - sk0.windows, 1l, "step knob off: cross-KV still serves")
            t |> equal(sk1.steps, sk0.steps, "step knob off: the step never serves")
            t |> success(key_exists(metal_wdec_declines(), "step_knob"), "the step-knob note is recorded")
            t |> equal(sk1.declines - sk0.declines, 0l, "the step-knob note is policy, not a decline")
            to_log(LOG_INFO, "whisper tiny cross-kv step-knob eyeball: '{sk_text}'\n")
            t |> success(sk_text == cpu_text, "step knob off: the CPU decode transcript")
            // the grouped-arm carrier: on an Apple box with a fresh manifest the natural backend
            // is grp-mr, so the un-repack's GROUPED arm ran above (negative-controlled: it can go
            // dead silently when this reads 1)
            t |> success(active_q8_layout_mr() > 1l, "grp-mr backend active - the grouped blob arm was exercised")
            delete samples
        } else {
            feint("das_metal is not built on this platform; the wdec gates need it\n")
        }
    }
}

[test]
def test_parakeet_tower_metal(tst : T?) {
    tst |> run("parakeet tower: the Metal FastConformer block loop matches the CPU f32 rail on jfk") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "parakeet") || !arm_on(t, "mtower")) return
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
            return if (!jit_model_available(t, path))
            var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present")
                return
            }
            var cpu_toks : array<int>
            var gpu_toks : array<int>
            var cpu_text = ""
            var gpu_text = ""
            var nlayer = 0l
            var enc_delta = 0l
            var blk_delta = 0l
            var off_encodes = 0l
            var off_declines = 0l
            var enc_rel_l2 = 0.0
            var poison_rel_l2 = 0.0
            var rows_gpu = 0l
            var twin_delta = 0l
            var tile_delta = 0l
            var twin_rel_l2 = 0.0
            var tile_rel_l2 = 0.0
            var front_delta = 0l
            var cpu_front_convs = 0l
            var cpu_front_encodes = 0l
            var cpu_front_rel_l2 = 0.0
            var dev_delta = 0l
            var dev_off_delta = 0l
            var dev_rel_l2 = 0.0
            var dev_off_rel_l2 = 0.0
            var dev_text = ""
            with_dasllama_jobque_() {
                var inscope m = AsrModel(kind = AsrKind.parakeet)
                var inscope stg <- stage_parakeet_model(path, false)
                mint_parakeet_model(stg, m.pk, false)
                t |> success(!m.pk.q8, "model is on the f32 rail")
                nlayer = m.pk.n_layer
                // every lever a leg below depends on, set here and put back at the cell's end
                let crowns_prev = get_metal_tensor_crowns()
                let front_prev = set_metal_parakeet_front(true)
                let dev_prev = set_metal_fc_attn_dev(true)
                let twin_prev = get_metal_tower_f16()
                set_metal_tower_f16(true)
                set_metal_tower(false)
                let d0 = metal_tower_stats()
                var inscope s <- create_session(m, "en")
                transcribe(m, s, samples) $(seg) {
                    cpu_toks |> push_from(seg.tokens)
                    cpu_text += seg.text
                }
                let d1 = metal_tower_stats()
                off_encodes = d1.encodes - d0.encodes
                off_declines = d1.declines - d0.declines
                let s0 = metal_tower_stats()
                set_metal_tower(true)
                var inscope s2 <- create_session(m, "en")
                transcribe(m, s2, samples) $(seg) {
                    gpu_toks |> push_from(seg.tokens)
                    gpu_text += seg.text
                }
                let s1 = metal_tower_stats()
                enc_delta = s1.encodes - s0.encodes
                blk_delta = s1.blocks - s0.blocks
                front_delta = s1.convs - s0.convs
                var inscope stc = ParakeetState()
                var inscope stg2 = ParakeetState()
                set_metal_tower(false)
                parakeet_log_mel(samples, m.pk, stc)
                parakeet_encode(m.pk, stc)
                set_metal_tower(true)
                parakeet_log_mel(samples, m.pk, stg2)
                let e0 = metal_tower_stats().encodes
                parakeet_encode(m.pk, stg2)
                rows_gpu = metal_tower_stats().encodes - e0
                enc_rel_l2 = rel_l2(stg2.x, stc.x)
                // the control: the same clip with its mel scaled x3 through the GPU chain must red the bar
                for (i in range(length(stg2.mel.data))) {
                    stg2.mel.data[i] *= 3.0
                }
                parakeet_encode(m.pk, stg2)
                poison_rel_l2 = rel_l2(stg2.x, stc.x)
                // the front left to the CPU: the block seat serves behind it
                parakeet_log_mel(samples, m.pk, stg2)
                set_metal_parakeet_front(false)
                let f0 = metal_tower_stats()
                parakeet_encode(m.pk, stg2)
                let f1 = metal_tower_stats()
                set_metal_parakeet_front(true)
                cpu_front_convs = f1.convs - f0.convs
                cpu_front_encodes = f1.encodes - f0.encodes
                cpu_front_rel_l2 = rel_l2(stg2.x, stc.x)
                // the two GEMM routes under a pinned tensor crown: the device twin, then the knob's f32 tiles
                set_metal_tensor_crowns("mulmm_q8")
                metal_prefill_shutdown()
                parakeet_log_mel(samples, m.pk, stg2)
                let tw0 = metal_tower_f16_encodes()
                parakeet_encode(m.pk, stg2)
                twin_delta = metal_tower_f16_encodes() - tw0
                twin_rel_l2 = rel_l2(stg2.x, stc.x)
                set_metal_tower_f16(false)
                let tw1 = metal_tower_f16_encodes()
                parakeet_encode(m.pk, stg2)
                tile_delta = metal_tower_f16_encodes() - tw1
                tile_rel_l2 = rel_l2(stg2.x, stc.x)
                set_metal_tower_f16(true)
                // the all-heads attention under the device pair's crown, then its lever off: the per-head loop serves
                set_metal_tensor_crowns("mulmm_q8,attn_dev")
                metal_prefill_shutdown()
                let dv0 = metal_tower_fc_dev_encodes()
                parakeet_encode(m.pk, stg2)
                dev_delta = metal_tower_fc_dev_encodes() - dv0
                dev_rel_l2 = rel_l2(stg2.x, stc.x)
                var inscope s3 <- create_session(m, "en")
                transcribe(m, s3, samples) $(seg) {
                    dev_text += seg.text
                }
                set_metal_fc_attn_dev(false)
                let dv1 = metal_tower_fc_dev_encodes()
                parakeet_encode(m.pk, stg2)
                dev_off_delta = metal_tower_fc_dev_encodes() - dv1
                dev_off_rel_l2 = rel_l2(stg2.x, stc.x)
                set_metal_fc_attn_dev(dev_prev)
                set_metal_parakeet_front(front_prev)
                set_metal_tower_f16(twin_prev)
                set_metal_tensor_crowns(crowns_prev)
                metal_prefill_shutdown()
            }
            t |> equal(dev_delta, 1l, "under the device pair's crown the attention ran over every head at once")
            t |> success(dev_rel_l2 < 5.0e-2, "the all-heads attention tracks the CPU rail (rel-l2 {dev_rel_l2})")
            t |> equal(dev_text, cpu_text, "the all-heads attention's transcript is the CPU's")
            t |> equal(dev_off_delta, 0l, "all-heads lever off: the route never engaged")
            t |> success(dev_off_rel_l2 < 5.0e-2, "all-heads lever off: the per-head loop tracks the CPU rail (rel-l2 {dev_off_rel_l2})")
            t |> success(dev_rel_l2 != dev_off_rel_l2, "the two attention forms round apart - the lever switched the route")
            to_log(LOG_INFO, "parakeet mtower encode rel-l2 by attention form: all heads {dev_rel_l2}, per head {dev_off_rel_l2}\n")
            t |> equal(front_delta, enc_delta, "every GPU encode ran the subsample front on the device ({front_delta} of {enc_delta})")
            t |> equal(cpu_front_convs, 0l, "front lever off: the front never dispatched")
            t |> equal(cpu_front_encodes, 1l, "front lever off: the block seat still served the encode")
            t |> success(cpu_front_rel_l2 < 5.0e-2, "front lever off: the CPU front under the device blocks tracks the CPU rail (rel-l2 {cpu_front_rel_l2})")
            to_log(LOG_INFO, "parakeet mtower encode rel-l2 with the CPU front: {cpu_front_rel_l2}\n")
            t |> equal(twin_delta, 1l, "under a tensor crown the encode rode the device twin")
            t |> success(twin_rel_l2 < 5.0e-2, "the twin route tracks the CPU rail (rel-l2 {twin_rel_l2})")
            t |> equal(tile_delta, 0l, "twin knob off: the route never engaged")
            t |> success(tile_rel_l2 < 5.0e-2, "twin knob off: the f32 tile route tracks the CPU rail (rel-l2 {tile_rel_l2})")
            to_log(LOG_INFO, "parakeet mtower encode rel-l2 by route: twin {twin_rel_l2}, f32 tiles {tile_rel_l2}\n")
            t |> success(parakeet_gpu_serves(), "the driver reports serving with the tower on")
            t |> success(!parakeet_serves_q8(), "the un-pinned lane is the f32 planes while the driver serves")
            t |> equal(off_encodes, 0l, "the tower-off leg never dispatched")
            t |> success(off_declines >= 1l, "the tower-off leg recorded the knob decline ({off_declines})")
            t |> equal(rows_gpu, 1l, "the rows compare's GPU encode dispatched")
            t |> success(enc_delta >= 1l, "GPU leg dispatched ({enc_delta} encodes)")
            t |> equal(blk_delta, nlayer * enc_delta, "blocks delta = n_layer x encodes")
            // the canary cell's bar (f32 GEMMs both sides; the device GEMM tiles re-order accumulation), logged either way
            t |> success(enc_rel_l2 < 5.0e-2, "GPU encode tracks the CPU rail element-level (rel-l2 {enc_rel_l2})")
            t |> success(poison_rel_l2 >= 5.0e-2, "a poisoned GPU element reds the bar (rel-l2 {poison_rel_l2})")
            to_log(LOG_INFO, "parakeet mtower encode rel-l2: {enc_rel_l2}\n")
            to_log(LOG_INFO, "parakeet mtower CPU eyeball: {cpu_text}\n")
            to_log(LOG_INFO, "parakeet mtower GPU eyeball: {gpu_text}\n")
            t |> success(!empty(gpu_text), "non-empty transcript")
            // a legitimate near-tie flip means converting this cell to forced-feed, not loosening the compare
            t |> equal(length(gpu_toks), length(cpu_toks), "token count CPU vs GPU")
            for (a, b in cpu_toks, gpu_toks) {
                t |> equal(a, b, "token CPU vs GPU")
            }
            t |> equal(cpu_text, gpu_text, "transcript equality CPU vs GPU")
            delete samples
        } else {
            t |> skip("no Metal build - the parakeet FastConformer driver has no device to serve")
        }
    }
}

// A q8 parakeet stays on the CPU chain: the driver declines it (quant_mode) and never dispatches.
[test]
def test_parakeet_tower_q8_decline(tst : T?) {
    tst |> run("parakeet tower: the q8 lane records the quant_mode decline and never dispatches") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (!family_on(t, "parakeet") || !arm_on(t, "mtower")) return
            let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
            return if (!jit_model_available(t, path))
            var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present")
                return
            }
            var text = ""
            var enc_delta = 0l
            var dec_delta = 0l
            with_dasllama_jobque_() {
                var inscope m = AsrModel(kind = AsrKind.parakeet)
                var inscope stg <- stage_parakeet_model(path, true)
                mint_parakeet_model(stg, m.pk, true)
                t |> success(m.pk.q8, "model is on the q8 lane")
                set_metal_tower(true)
                let s0 = metal_tower_stats()
                var inscope s <- create_session(m, "en")
                text = transcribe(m, s, samples)
                let s1 = metal_tower_stats()
                enc_delta = s1.encodes - s0.encodes
                dec_delta = s1.declines - s0.declines
            }
            t |> success(!empty(text), "transcript non-empty")
            t |> equal(enc_delta, 0l, "no tower encodes on the q8 lane")
            t |> success(dec_delta >= 1l, "the quant_mode decline is counted ({dec_delta})")
            // required-mode contract: a capability decline (quant_mode) under MetalMode.required
            // panics instead of silently CPU-serving
            var tripped = false
            let mode_was = get_metal_mode()
            set_metal_mode(MetalMode.required)
            try {
                with_dasllama_jobque_() {
                    var inscope m = AsrModel(kind = AsrKind.parakeet)
                    var inscope stg <- stage_parakeet_model(path, true)
                    mint_parakeet_model(stg, m.pk, true)
                    var inscope s <- create_session(m, "en")
                    transcribe(m, s, samples)
                }
            } recover {
                tripped = true
            }
            set_metal_mode(mode_was)
            t |> success(tripped, "required mode: the q8 tower decline panics")
            delete samples
        } else {
            t |> skip("no Metal build - the parakeet FastConformer driver has no device to decline")
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
        let cached_lane_q8 = parakeet_serves_q8()   // the lane the cached loads take on this box
        var inscope stg <- stage_parakeet_model(path, cached_lane_q8)
        mint_parakeet_model(stg, m.pk, cached_lane_q8)
    }
    var text = ""
    with_dasllama_jobque() {
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
        let path = path_join(whisper_dir(), "ggml-parakeet-tdt-0.6b-v3-f32.bin")
        return if (!jit_model_available(t, path))
        var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present")
            return
        }
        let img = image_path_for(path, parakeet_serves_q8() ? "parakeet-q8" : "parakeet-f32")
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
        let path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        return if (!jit_model_available(t, path))
        set_qwen3a_q8(true)
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
        t |> equal(count_mismatch(a.core.qblob, b.core.qblob), 0l, "core.qblob element-exact")
        t |> equal(count_mismatch(a.core.qscales, b.core.qscales), 0l, "core.qscales element-exact")
        t |> equal(count_mismatch(a.cblob, b.cblob), 0l, "cblob element-exact")
        t |> success(b.cq8, "warm tower cblob trio is on the q8 rail")
        t |> equal(count_mismatch(a.qcblob, b.qcblob), 0l, "qcblob element-exact")
        t |> equal(count_mismatch(a.qcscales, b.qcscales), 0l, "qcscales element-exact")
        // the f32 A/B rail still stages + serves (parity legs flip it; serving never does)
        set_qwen3a_q8(false)
        let img32 = image_path_for(path, "qwen3a-f32")
        remove(img32)
        var inscope a32 <- stage_qwen3a_tower(path)
        var inscope c32 <- load_qwen3a_tower(path)
        t |> success(!c32.core.q8, "f32 rail still serves")
        t |> equal(count_mismatch(a32.core.fblob, c32.core.fblob), 0l, "f32 core.fblob element-exact")
        t |> success(long_length(a32.core.wblob) > 0l, "the bf16 mmproj bakes the halfword twin on the exact lane")
        reset_qwen3a_q8()
    }
}

[test]
def test_gemma4uv_embedder_image_roundtrip(t : T?) {
    t |> run("gemma4uv embedder: mapped image planes are element-exact vs mmproj, bf16 lane") @(t : T?) {
        if (!family_on(t, "gemma") || !arm_on(t, "gemma4uv")) return
        let path = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
        return if (!jit_model_available(t, path))
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
        t |> equal(count_mismatch(a.wblob, b.wblob), 0l, "wblob (the bf16 GEMM) element-exact")
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

[test]
def test_gemma4v_tower_image_roundtrip(t : T?) {
    t |> run("gemma4v tower: mapped image planes are element-exact vs mmproj, bf16 lane") @(t : T?) {
        if (!family_on(t, "gemma") || !arm_on(t, "gemma4v")) return
        let path = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        return if (!jit_model_available(t, path))
        let img = image_path_for(path, "gemma4v")
        remove(img)
        set_gemma4v_q8(false)   // the exact-plane lane first; the q8 serving lane is the second half
        var inscope a <- stage_gemma4v_tower(path)
        var inscope cold <- load_gemma4v_tower(path)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> success(!cold.patch_bf16 && cold.blk_bf16 && cold.proj_bf16, "plane split follows the file: f32 conv, bf16 GEMMs")
        t |> equal(count_mismatch(a.blob, cold.blob), 0l, "cold-built blob element-exact")
        var inscope b <- load_gemma4v_tower(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> equal(b.n_embd, a.t.n_embd)
        t |> equal(b.n_layer, a.t.n_layer)
        t |> equal(b.proj_dim, a.t.proj_dim)
        t |> equal(b.pos_entries, a.t.pos_entries)
        t |> equal(b.layer_f, a.t.layer_f)
        t |> equal(b.layer_f_stride, a.t.layer_f_stride)
        t |> equal(b.layer_w, a.t.layer_w)
        t |> equal(b.layer_w_stride, a.t.layer_w_stride)
        t |> equal(b.proj_clamp, a.t.proj_clamp)
        t |> equal(count_mismatch(a.blob, b.blob), 0l, "blob (f32: conv, pos tables, norms, clamp table) element-exact")
        t |> equal(count_mismatch(a.wblob, b.wblob), 0l, "wblob (the bf16 GEMMs) element-exact")
        t |> equal(long_length(a.wblob), a.t.n_layer * a.t.layer_w_stride + a.t.n_embd * a.t.proj_dim,
                   "the bf16 plane is exactly the GEMM set ({length(a.wblob)} halfwords)")
        set_gemma4v_q8(true)
        let imgq = image_path_for(path, "gemma4v-q8")
        remove(imgq)
        var inscope aq <- stage_gemma4v_tower(path)
        var inscope cq <- load_gemma4v_tower(path)
        t |> success(cq.q8 && cq.image_map != null && stat(imgq).is_valid, "q8 lane: cold load built and serves its own image")
        t |> success(imgq != img, "the q8 lane keys a different image")
        t |> equal(count_mismatch(aq.blob, cq.blob), 0l, "q8 lane: blob element-exact")
        t |> equal(count_mismatch(aq.wblob, cq.wblob), 0l, "q8 lane: wblob (the projection) element-exact")
        t |> equal(count_mismatch(aq.qblob, cq.qblob), 0l, "q8 lane: qblob element-exact")
        t |> equal(count_mismatch(aq.qscales, cq.qscales), 0l, "q8 lane: qscales element-exact")
        t |> equal(long_length(aq.qblob), aq.t.n_layer * aq.t.layer_w_stride, "qblob is exactly the block GEMM set")
        t |> equal(long_length(aq.wblob), aq.t.n_embd * aq.t.proj_dim, "q8 lane: the bf16 plane is exactly the projection")
        reset_gemma4v_q8()
        // a .dlim named directly loads on its BAKED lane, whatever the pin, and the carrier sniffs it
        set_gemma4v_q8(false)
        var inscope dq <- load_gemma4v_tower(imgq)
        t |> success(dq.q8 && dq.image_map != null, "the q8 .dlim serves its q8 lane under an exact pin")
        reset_gemma4v_q8()
        var inscope dcarrier <- load_vision_embedder(imgq)
        t |> equal(vision_family(dcarrier), "gemma4v", "the carrier routes the q8-tagged .dlim to gemma4v")
        t |> equal(vision_exec_fmt(dcarrier), "q8", "...and stamps its lane")
        var inscope dx <- load_gemma4v_tower(img)
        t |> success(!dx.q8, "the exact-plane .dlim serves its lane")
    }
}

[test]
def test_gemma3v_tower_image_roundtrip(t : T?) {
    t |> run("gemma3v tower: mapped image planes are element-exact vs mmproj, both lanes") @(t : T?) {
        if (!family_on(t, "gemma") || !arm_on(t, "gemma3v")) {
            return
        }
        let path = path_join(models_dir(), "mmproj-gemma-3-4b-it-F16.gguf")
        return if (!jit_model_available(t, path))
        let img = image_path_for(path, "gemma3v")
        remove(img)
        set_gemma3v_q8(false)   // the exact-plane (f32-widened) lane first; the q8 serving lane follows
        var inscope a <- stage_gemma3v_tower(path)
        var inscope cold <- load_gemma3v_tower(path)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> equal(count_mismatch(a.blob, cold.blob), 0l, "cold-built blob element-exact")
        var inscope b <- load_gemma3v_tower(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> equal(b.n_embd, a.t.n_embd)
        t |> equal(b.n_layer, a.t.n_layer)
        t |> equal(b.proj_dim, a.t.proj_dim)
        t |> equal(b.ff_pad, a.t.ff_pad)
        t |> equal(b.image_px, a.t.image_px)
        t |> equal(b.layer_f, a.t.layer_f)
        t |> equal(b.layer_f_stride, a.t.layer_f_stride)
        t |> equal(b.layer_w, a.t.layer_w)
        t |> equal(b.layer_w_stride, a.t.layer_w_stride)
        t |> equal(count_mismatch(a.blob, b.blob), 0l, "blob (all f32, incl. the ff_pad-padded GEMMs) element-exact")
        set_gemma3v_q8(true)
        let imgq = image_path_for(path, "gemma3v-q8")
        remove(imgq)
        var inscope aq <- stage_gemma3v_tower(path)
        var inscope cq <- load_gemma3v_tower(path)
        t |> success(cq.q8 && cq.image_map != null && stat(imgq).is_valid, "q8 lane: cold load built and serves its own image")
        t |> success(imgq != img, "the q8 lane keys a different image")
        t |> equal(count_mismatch(aq.blob, cq.blob), 0l, "q8 lane: blob element-exact")
        t |> equal(count_mismatch(aq.qblob, cq.qblob), 0l, "q8 lane: qblob element-exact")
        t |> equal(count_mismatch(aq.qscales, cq.qscales), 0l, "q8 lane: qscales element-exact")
        t |> equal(long_length(aq.qblob), aq.t.n_layer * aq.t.layer_w_stride, "qblob is exactly the padded block GEMM set")
        reset_gemma3v_q8()
        // a .dlim named directly loads on its BAKED lane, whatever the pin, and the carrier sniffs it
        set_gemma3v_q8(false)
        var inscope dq <- load_gemma3v_tower(imgq)
        t |> success(dq.q8 && dq.image_map != null, "the q8 .dlim serves its q8 lane under an exact pin")
        reset_gemma3v_q8()
        var inscope dcarrier <- load_vision_embedder(imgq)
        t |> equal(vision_family(dcarrier), "gemma3v", "the carrier routes the q8-tagged .dlim to gemma3v")
        t |> equal(vision_exec_fmt(dcarrier), "q8", "...and stamps its lane")
        var inscope dx <- load_gemma3v_tower(img)
        t |> success(!dx.q8, "the exact-plane .dlim serves its lane")
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
            let path = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
            return if (!jit_model_available(t, path))
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
        let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        return if (!jit_model_available(t, path))
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
        let path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
        return if (!jit_model_available(t, path))
        let img = image_path_for(path, "canary-q8")
        remove(img)
        // off-rail truth: the staging carrier re-runs the read-time transcode + repack under
        // the same backend pin, so its planes are the served image's ground truth
        var inscope a <- stage_canary_encoder(path, true)
        var inscope cold <- load_canary_encoder(path, true)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> success(cold.q8, "served encoder is on the q8 rail")
        t |> equal(count_mismatch(a.qblob, cold.qblob), 0l, "cold-built qblob element-exact")
        var inscope b <- load_canary_encoder(path, true)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> success(b.q8, "warm encoder is on the q8 rail")
        t |> equal(b.d_model, a.e.d_model)
        t |> equal(b.n_layer, a.e.n_layer)
        t |> equal(b.proj_dim, a.e.proj_dim)
        t |> equal(length(b.layers), long_length(a.layers))
        t |> equal(count_mismatch(a.qblob, b.qblob), 0l, "qblob element-exact")
        t |> equal(count_mismatch(a.qscales, b.qscales), 0l, "qscales element-exact")
        t |> equal(count_mismatch(a.blob, b.blob), 0l, "compact fp32 blob element-exact")
        t |> equal(count_mismatch(a.filters, b.filters), 0l, "filters element-exact")
        // the off-rail control: a staged read minted straight into memory; the mint derives
        // its family tag from the staging's own precision
        var inscope stg <- stage_canary_encoder(path, true)
        var inscope minted = CanaryEncoder()
        mint_canary_encoder(stg, minted)
        t |> success(minted.q8, "minted encoder is on the staging's q8 rail")
        t |> equal(count_mismatch(a.qblob, minted.qblob), 0l, "minted qblob element-exact vs staged truth")
    }
}

[test]
def test_gemma4a_encoder_image_roundtrip(t : T?) {
    t |> run("gemma4a encoder q8: mapped image planes are element-exact vs gguf transcode") @(t : T?) {
        if (!family_on(t, "gemma4a") || !arm_on(t, "gemma4a")) return
        let path = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        return if (!jit_model_available(t, path))
        set_gemma4a_q8(true)
        let img = image_path_for(path, "gemma4a-q8")
        remove(img)
        // off-rail truth: the staging carrier re-runs the read-time transcode + repack under
        // the same backend pin, so its planes are the served image's ground truth
        var inscope a <- stage_gemma4a_encoder(path)
        var inscope cold <- load_gemma4a_encoder(path)
        t |> success(cold.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        t |> success(cold.q8, "served encoder is on the q8 rail")
        t |> equal(count_mismatch(a.qblob, cold.qblob), 0l, "cold-built qblob element-exact")
        var inscope b <- load_gemma4a_encoder(path)
        t |> success(b.image_map != null, "warm load is image-backed")
        t |> success(b.q8, "warm encoder is on the q8 rail")
        t |> equal(b.d_model, a.e.d_model)
        t |> equal(b.n_layer, a.e.n_layer)
        t |> equal(b.proj_dim, a.e.proj_dim)
        t |> equal(length(b.layers), long_length(a.layers))
        t |> equal(count_mismatch(a.qblob, b.qblob), 0l, "qblob element-exact")
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
        reset_gemma4a_q8()
    }
}

[test]
def test_gemma4a_transcribe_q8_ab(t : T?) {
    t |> run("gemma4a transcribe: the q8 tower vs the f32 rail on jfk, same text") @(t : T?) {
        return if (!family_on(t, "gemma4a") || !arm_on(t, "gemma4a") || !jit_only(t))
        let dec_path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
        let mmp_path = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
        if (!model_available(t, dec_path) || !model_available(t, mmp_path)) return
        var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present beside the models")
            return
        }
        var q8_text = ""
        var f32_text = ""
        var lang = ""
        with_dasllama_jobque_() {
            // decoder off-rail (REVIEW: suites load decoders with load_model_) — loaded ONCE; only the tower flips rails
            var dec <- load_model_(dec_path, QuantMode.q8)
            set_gemma4a_q8(true)
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
            reset_gemma4a_q8()
            delete dec
        }
        to_log(LOG_INFO, "gemma4a q8 : {q8_text}\n")
        to_log(LOG_INFO, "gemma4a f32: {f32_text}\n")
        t |> equal(q8_text, f32_text, "q8 tower transcript matches the f32 rail")
        delete samples
    }
}

[test]
def test_gemma4a_tower_metal(t : T?) {
    g4a_tower_metal_cell(t, "E2B", "gemma-4-E2B-it-Q8_0.gguf", "mmproj-gemma-4-E2B-it-bf16.gguf", 1536l)
}

//! E4B's audio embedder widens the encoder's 1536 to the decoder's 2560, where E2B's is square
[test]
def test_gemma4a_tower_metal_e4b(t : T?) {
    g4a_tower_metal_cell(t, "E4B", "gemma-4-E4B-it-Q8_0.gguf", "mmproj-gemma-4-E4B-it-BF16.gguf", 2560l)
}

def private g4a_tower_metal_cell(tst : T?; tag, dec_file, mmp_file : string; want_proj : int64) {
    tst |> run("gemma4a tower {tag}: the Metal Conformer chain, the projector tail on the device, matches the CPU f32 rail on jfk") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!family_on(t, "gemma4a") || !arm_on(t, "mtower") || !jit_only(t))
            let dec_path = path_join(models_dir(), dec_file)
            let mmp_path = path_join(models_dir(), mmp_file)
            if (!model_available(t, dec_path) || !model_available(t, mmp_path)) return
            var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
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
            var enc_rel_l2 = 0.0
            with_dasllama_jobque_() {
                var dec <- load_model_(dec_path, QuantMode.q8)
                dec.config.seq_len = min(dec.config.seq_len, 8192l)
                set_gemma4a_q8(false)
                var inscope enc <- load_gemma4a_encoder(mmp_path)
                t |> success(!enc.q8, "tower is on the f32 rail")
                t |> equal(enc.proj_dim, want_proj, "the embedder projects to the decoder's width")
                t |> equal(enc.proj_dim, dec.config.dim)
                t |> equal(gemma4a_mid_dim(enc), 1536l, "the encoder's own output width")
                nlayer = enc.n_layer
                set_metal_tower(false)
                var inscope s <- create_gemma4a_asr_session(dec)
                gemma4a_transcribe(dec, enc, s, samples, $(seg : TranscribeSegment) {
                    cpu_text += seg.text
                }, lang)
                let s0 = metal_tower_stats()
                set_metal_tower(true)
                var inscope s2 <- create_gemma4a_asr_session(dec)
                gemma4a_transcribe(dec, enc, s2, samples, $(seg : TranscribeSegment) {
                    gpu_text += seg.text
                }, lang)
                let s1 = metal_tower_stats()
                enc_delta = s1.encodes - s0.encodes
                blk_delta = s1.blocks - s0.blocks
                var inscope stc = Gemma4aState()
                var inscope stg = Gemma4aState()
                set_metal_tower(false)
                gemma4a_log_mel(enc, samples, 0l, long_length(samples), stc)
                gemma4a_encode(enc, stc)
                set_metal_tower(true)
                gemma4a_log_mel(enc, samples, 0l, long_length(samples), stg)
                gemma4a_encode(enc, stg)
                enc_rel_l2 = rel_l2(stg.out, stc.out)
                t |> success(stg.out_ready && !stc.out_ready, "the device leg ran the projector tail, the CPU leg its own")
                reset_gemma4a_q8()
                delete dec
            }
            t |> success(enc_delta >= 1l, "GPU leg dispatched ({enc_delta} encodes)")
            t |> equal(blk_delta, nlayer * enc_delta, "blocks delta = n_layer x encodes")
            //! measured 3.9e-4 on the jfk fixture (12 blocks, window-12 attention caps the mixing)
            t |> success(enc_rel_l2 < 5.0e-3, "GPU encode tracks the CPU rail element-level (rel-l2 {enc_rel_l2})")
            to_log(LOG_INFO, "gemma4a mtower CPU eyeball: {cpu_text}\n")
            to_log(LOG_INFO, "gemma4a mtower GPU eyeball: {gpu_text}\n")
            t |> success(!empty(gpu_text), "non-empty transcript")
            t |> equal(cpu_text, gpu_text, "transcript equality CPU vs GPU")
            delete samples
        } else {
            t |> skip("no Metal build - the gemma4a Conformer driver has no device to serve")
        }
    }
}

[test, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def test_canary_tower_metal(t : T?) {
    t |> run("canary tower: the Metal FastConformer block loop matches the CPU f32 rail on jfk") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!family_on(t, "canary") || !arm_on(t, "mtower") || !jit_only(t))
            let dec_path = path_join(models_dir(), "canary-qwen-2.5b-decoder-q8_0.gguf")
            let mmp_path = path_join(models_dir(), "canary-qwen-2.5b-encoder-f32.bin")
            if (!model_available(t, dec_path) || !model_available(t, mmp_path)) return
            var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
            if (empty(samples)) {
                t |> skip("jfk.wav not present beside the models")
                return
            }
            var cpu_text = ""
            var gpu_text = ""
            var nlayer = 0l
            var enc_delta = 0l
            var blk_delta = 0l
            var enc_rel_l2 = 0.0
            var poison_rel_l2 = 0.0
            var rows_gpu = 0l
            with_dasllama_jobque_() {
                var dec <- load_model_(dec_path, QuantMode.q8)
                dec.config.seq_len = min(dec.config.seq_len, 8192l)
                set_canary_enc_q8(false)
                var inscope enc <- load_canary_encoder(mmp_path, false)
                t |> success(!enc.q8, "encoder is on the f32 rail")
                nlayer = enc.n_layer
                set_metal_tower(false)
                var inscope s <- create_canary_asr_session(dec, "en")
                var lang = ""
                canary_transcribe(dec, enc, s, samples, $(seg : TranscribeSegment) {
                    cpu_text += seg.text
                }, lang)
                let s0 = metal_tower_stats()
                set_metal_tower(true)
                var inscope s2 <- create_canary_asr_session(dec, "en")
                canary_transcribe(dec, enc, s2, samples, $(seg : TranscribeSegment) {
                    gpu_text += seg.text
                }, lang)
                let s1 = metal_tower_stats()
                enc_delta = s1.encodes - s0.encodes
                blk_delta = s1.blocks - s0.blocks
                var inscope stc = CanaryState()
                var inscope stg = CanaryState()
                set_metal_tower(false)
                canary_log_mel(enc, samples, stc)
                canary_encode(enc, stc)
                set_metal_tower(true)
                canary_log_mel(enc, samples, stg)
                let e0 = metal_tower_stats()
                canary_encode(enc, stg)
                rows_gpu = metal_tower_stats().encodes - e0.encodes
                t |> equal(metal_tower_stats().convs - e0.convs, 1l, "the mel and the subsample front served on the device")
                enc_rel_l2 = rel_l2(stg.out, stc.out)
                // the control: the device front reads the windowed frames, so the clip with the first half of them
                // silenced, through the GPU chain, must red the bar (a scaled clip would not: the mel normalizes per feature)
                for (i in range(length(stg.xw) / 2)) {
                    stg.xw[i] = 0.0
                }
                canary_encode(enc, stg)
                poison_rel_l2 = rel_l2(stg.out, stc.out)
                reset_canary_enc_q8()
                delete dec
            }
            t |> success(enc_delta >= 1l, "GPU leg dispatched ({enc_delta} encodes)")
            t |> equal(blk_delta, nlayer * enc_delta, "blocks delta = n_layer x encodes")
            //! measured 3.1e-4 on the jfk fixture (f32 GEMMs both sides; the online-softmax
            //! attention re-orders accumulation) - the bar leaves deep-GPU headroom
            t |> equal(rows_gpu, 1l, "the rows compare's GPU encode dispatched")
            t |> success(enc_rel_l2 < 5.0e-2, "GPU encode tracks the CPU rail element-level (rel-l2 {enc_rel_l2})")
            t |> success(poison_rel_l2 >= 5.0e-2, "a poisoned GPU element reds the bar (rel-l2 {poison_rel_l2})")
            to_log(LOG_INFO, "canary mtower encode rel-l2: {enc_rel_l2}\n")
            to_log(LOG_INFO, "canary mtower CPU eyeball: {cpu_text}\n")
            to_log(LOG_INFO, "canary mtower GPU eyeball: {gpu_text}\n")
            t |> success(!empty(gpu_text), "non-empty transcript")
            t |> equal(cpu_text, gpu_text, "transcript equality CPU vs GPU")
            delete samples
        } else {
            t |> skip("no Metal build - the canary FastConformer driver has no device to serve")
        }
    }
}

[test]
def test_qwen3a_tower_q8_ab(t : T?) {
    t |> run("qwen3a tower: the q8 core tracks the f32 rail on jfk window 0") @(t : T?) {
        if (!family_on(t, "qwen3a") || !arm_on(t, "qwen3a")) return
        let path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        return if (!jit_model_available(t, path))
        var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present beside the models")
            return
        }
        set_qwen3a_q8(true)
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
        with_dasllama_jobque_() {
            var inscope win <- qwen3a_mel_window(mel, 0l)
            qwen3a_encode(twq, encq, csq, win)
            var inscope winf <- qwen3a_mel_window(mel, 0l)
            qwen3a_encode(twf, encf, csf, winf)
        }
        let nv = long_length(encq.out)
        t |> equal(nv, long_length(encf.out), "the q8 and f32 cores encode one window's rows")
        if (nv == long_length(encf.out)) {
            let rel = rel_l2(encq.out, encf.out)
            to_log(LOG_INFO, "qwen3a q8-vs-f32 window 0: rel-l2 {rel}, maxdiff {logits_maxdiff(encq.out, encf.out)} (signal max {logits_maxabs(encf.out)}) over {nv} values\n")
            // element-level tripwire; the real gate is transcript-level (test_qwen3a_transcribe_q8_ab)
            t |> success(rel < 0.05, "q8 core tracks the f32 rail (element-level bar)")
        }
        reset_qwen3a_q8()
        delete samples
    }
}

[test]
def test_qwen3a_transcribe_q8_ab(t : T?) {
    t |> run("qwen3a transcribe: the q8 tower core vs the f32 rail on jfk, same text") @(t : T?) {
        return if (!family_on(t, "qwen3a") || !arm_on(t, "qwen3a") || !jit_only(t))
        let dec_path = path_join(models_dir(), "Qwen3-ASR-0.6B-Q8_0.gguf")
        let mmp_path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
        if (!model_available(t, dec_path) || !model_available(t, mmp_path)) return
        var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
        if (empty(samples)) {
            t |> skip("jfk.wav not present beside the models")
            return
        }
        var q8_text = ""
        var f32_text = ""
        var lang = ""
        with_dasllama_jobque_() {
            // decoder off-rail (REVIEW: suites load decoders with load_model_) — loaded ONCE; only the tower flips rails
            var dec <- load_model_(dec_path, QuantMode.q8)
            dec.config.seq_len = min(dec.config.seq_len, 8192l)
            set_qwen3a_q8(true)
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
            reset_qwen3a_q8()
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
[test, arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-encode-chains")]
def test_qwen3a_metal_tower(tst : T?) {
    tst |> run("qwen3a f32 rail: the Metal tower blocks - transcript + counters") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!family_on(t, "qwen3a") || !arm_on(t, "mtower") || !jit_only(t))
            let dec_path = path_join(models_dir(), "Qwen3-ASR-0.6B-Q8_0.gguf")
            let mmp_path = path_join(models_dir(), "mmproj-Qwen3-ASR-0.6B-bf16.gguf")
            if (!model_available(t, dec_path) || !model_available(t, mmp_path)) return
            var samples <- load_audio_16k_mono(path_join(models_dir(), "jfk.wav"))
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
            var conv_delta = 0l
            var enc_rel_l2 = 0.0
            with_dasllama_jobque_() {
                var dec <- load_model_(dec_path, QuantMode.q8)
                dec.config.seq_len = min(dec.config.seq_len, 8192l)
                set_qwen3a_q8(false)
                set_metal_tower_f16(false)   //! this cell's element bar claims the f32 GPU chain; the twin leg below re-arms
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
                conv_delta = s1.convs - s0.convs
                // window-0 encode pair, same f32 core, tower off/on: the element instrument
                // that SEES the conv bias (transcripts do not - negative-controlled)
                var inscope mel <- log_mel_qwen3a(samples, tw.core.n_mel)
                var inscope ecpu <- make_encoder_state(tw.core)
                var inscope egpu <- make_encoder_state(tw.core)
                var inscope c1 = Qwen3aConvState()
                var inscope c2 = Qwen3aConvState()
                set_metal_tower(false)
                var inscope w1 <- qwen3a_mel_window(mel, 0l)
                qwen3a_encode(tw, ecpu, c1, w1)
                set_metal_tower(true)
                var inscope w2 <- qwen3a_mel_window(mel, 0l)
                qwen3a_encode(tw, egpu, c2, w2)
                enc_rel_l2 = rel_l2(egpu.out, ecpu.out)
                set_metal_tower_f16(true)
                let tw0 = metal_tower_f16_encodes()
                var twin_text = ""
                var inscope s3 <- create_qwen3a_asr_session(dec, tw, "auto", false)
                qwen3a_transcribe(dec, tw, s3, "", samples, $(seg : TranscribeSegment) {
                    twin_text += seg.text
                }, lang)
                t |> success(metal_tower_f16_encodes() - tw0 >= 1l, "the bf16-sourced tower rode the twin route")
                t |> equal(gpu_text, twin_text, "twin route: transcript matches the f32 GPU turn")
                reset_qwen3a_q8()
                delete dec
            }
            t |> success(enc_delta >= 1l, "GPU leg dispatched ({enc_delta} encodes)")
            t |> equal(blk_delta, nlayer * enc_delta, "blocks delta = n_layer x encodes")
            t |> success(conv_delta >= 1l, "the conv2d frontend served on the GPU ({conv_delta} convs)")
            // 5e-3, not 1e-3: the tower's attention score slab is f16 (written half once,
            // read half twice) - measured 2.5e-3 on the jfk fixture; transcripts stay exact
            t |> success(enc_rel_l2 < 5.0e-3, "GPU encode tracks the CPU rail element-level (rel-l2 {enc_rel_l2})")
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
        let path = path_join(models_dir(), "canary-qwen-2.5b-decoder-f16.gguf")
        return if (!jit_model_available(t, path))
        // the fp32 rail is opt-in (image_fp32) — the parity-pinned ASR decoders' lane
        let img = image_path_for(path, "", "fp32")
        remove(img)
        // off-rail truth: both cached loads are image-backed, so they cannot check each other
        var g <- load_model_(path, QuantMode.fp32)
        g.config.seq_len = min(g.config.seq_len, 2048l)
        var prompt <- encode(g, "Once upon a time")
        var ga <- greedy_gen(g, prompt, 16)
        delete g
        var a <- load_model_cached(path, QuantMode.fp32, planar_only = true, image_fp32 = true)
        t |> success(a.image_map != null, "cold load serves the image it built")
        t |> success(stat(img).is_valid, "cold load saved an image")
        a.config.seq_len = min(a.config.seq_len, 2048l)
        var gcold <- greedy_gen(a, prompt, 16)
        t |> success(!empty(ga) && count_mismatch(ga, gcold) == 0l, "gguf vs cold-built image tokens")
        delete gcold
        delete a
        var b <- load_model_cached(path, QuantMode.fp32, planar_only = true, image_fp32 = true)
        t |> success(b.image_map != null, "warm load is image-backed")
        b.config.seq_len = min(b.config.seq_len, 2048l)
        var gb <- greedy_gen(b, prompt, 16)
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
def private probe_layout() : uint64 => 0xf00dcafeul

[test]
def test_dlim_flavor_lanes(t : T?) {
    t |> run("flavor lanes: another bake context's image is never judged from here") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let planar = image_identity("", "q8")
        t |> equal(dlim_identity_lane(planar), "q8#", "planar lane is quant-only")
        t |> equal(dlim_identity_lane("{planar}|metal"), "q8#metal", "metal tag folds into the lane")
        let vk_tag = "vulkan|m2n b8192 l4 s1 r1100001101"
        t |> equal(dlim_identity_lane("{planar}|{vk_tag}"), "q8#{vk_tag}", "vulkan tags keep their inner pipes")
        var current <- dlim_current_identities()
        var p = DlimPeek(valid = true, version = IMAGE_VERSION, identity = planar)
        t |> equal(dlim_image_verdict(p, current), "CURRENT", "own planar identity")
        // a cross-context GPU identity: winners impossible on any box (mr9/kq9), so it can never
        // coincide with THIS process's own identity the way a realistic fixture once did
        p.identity = "v{IMAGE_VERSION}|q8|foreignbox|s16|q8 mr9 b7 g5|kq 9/9/9/9|q51 mr9|iq4nl32 mr9|nat 00000|metal"
        t |> equal(dlim_image_verdict(p, current), "FOREIGN", "cross-context metal image is FOREIGN, not OTHER")
        p.identity = "v{IMAGE_VERSION}|q8|foreignbox|s16|q8 mr9 b7 g5|kq 9/9/9/9|q51 mr9|iq4nl32 mr9|nat 00000|{vk_tag}"
        t |> equal(dlim_image_verdict(p, current), "FOREIGN", "cross-context vulkan image is FOREIGN")
        // a family tag whose module is not loaded in the judging process: unrecomputable, so
        // FOREIGN — the 2026-08-02 board's step-zero GC reaped every whisper/parakeet image
        // this way before the rule generalized
        p.identity = "v{IMAGE_VERSION}|q8|arm64-gen|s16|q8 mr8 b0 g4|kq 8/8/4/8|q51 mr4|iq4nl32 mr4|nat 11010|not-a-registered-family"
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
    t |> run("a registered layout fingerprint judges an image parse_image would refuse") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        // the family half of the header: a served-layout change that keeps the version bumps the
        // meta layout fingerprint, and a peek that cannot read it calls a dead image CURRENT
        register_image_family_tag("img-probe-layout", false, @@probe_layout)
        var current <- dlim_current_identities()
        var p = DlimPeek(valid = true, version = IMAGE_VERSION, identity = image_identity("img-probe-layout"),
            layout = 0xf00dcafeul)
        t |> equal(dlim_image_verdict(p, current), "CURRENT", "this build's layout fingerprint")
        p.layout = 0xdeadbeeful
        t |> equal(dlim_image_verdict(p, current), "STALE layout", "a drifted layout outranks the identity match")
        p.version = IMAGE_VERSION - 1
        t |> equal(dlim_image_verdict(p, current), "STALE v{IMAGE_VERSION - 1}", "the version still wins")
        // a family that registered no fingerprint is judged exactly as before
        let q = DlimPeek(valid = true, version = IMAGE_VERSION, identity = image_identity("whisper-q8"))
        t |> equal(dlim_image_verdict(q, current), "CURRENT", "an unregistered layout never reads STALE layout")
        delete current
    }
    t |> run("a config-free tag's identity is the default configuration's, a config-bound one the box's") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        register_image_family_tag("img-probe-free", true)
        register_image_family_tag("img-probe-bound")
        let default_cfg = DlimConfiguration(quant = "q8")
        let box_cfg = dlim_config_current("q8")
        t |> success(image_identity_of(default_cfg, "img-probe-free") != image_identity_of(box_cfg, "img-probe-free"),
            "this box's configuration differs from the default - the claim below is not vacuous")
        t |> equal(image_identity("img-probe-free"), image_identity_of(default_cfg, "img-probe-free"),
            "a config-free tag ignores dlim_config_current: one file per box, not one per tune state")
        t |> equal(image_identity("img-probe-bound"), image_identity_of(box_cfg, "img-probe-bound"),
            "a config-bound tag moves with dlim_config_current")
        t |> equal(dlim_identity_tag(image_identity("img-probe-free")), "img-probe-free", "the tag reads back off the identity")
    }
}

// Model-free: the k6 scale-plane GPU split at 27B scale — a ~1.8B-weight q6 region is a strip
// past the 64 MB max_unreserved_size guard, so the staging copy must reserve, not bare-resize.
// Split layout owner: metal_blob_scale_plane in dasllama_layout.das.
[test]
def test_k6_scale_split_sizing(t : T?) {
    t |> run("a scale-plane name is a lattice format's member spelling plus s, and nothing else") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        var m = Model()
        t |> success(metal_blob_scale_plane(m, "k4s") && metal_blob_scale_plane(m, "iq2xxss"), "k4s and iq2xxss name lattice scale planes")
        t |> success(!metal_blob_scale_plane(m, "k6"), "no s suffix")
        t |> success(!metal_blob_scale_plane(m, "zzs"), "no such format")
        t |> success(!metal_blob_scale_plane(m, "q8s"), "q8 is off the lattice")
        t |> success(!metal_blob_scale_plane(m, "q40s") && !metal_blob_scale_plane(m, "iq4nls"), "q40 and iq4nl carry no split scale plane")
    }
    t |> run("k6 scale split at 27B-scale strip clears max_unreserved_size") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let nsb = 4000000l   // x K6_SSB = 72 MB, past the 67,108,864 B guard
        var m = Model()
        let k6 = int(KqFmt.k6)
        m.kq[k6].s |> reserve_resize(nsb * K6_SSB)
        for (i in range64(K6_SSB)) {   // K6_SSB bytes into sb0 and the last sb — distinct values trace the split
            m.kq[k6].s[i] = uint8(100l + i)
            m.kq[k6].s[(nsb - 1l) * K6_SSB + i] = uint8(200l + i)
        }
        t |> success(metal_blob_scale_plane(m, "k6s"), "k6s is a kq scale plane")
        t |> equal(long_length(m.kq[k6].s), nsb * K6_SSB, "split form keeps the byte count")
        t |> equal(int(m.kq[k6].s[0]), 100, "sb0 sub-scales lead the strip")
        t |> equal(int(m.kq[k6].s[15l]), 115, "sb0 sub-scale 15 in place")
        t |> equal(int(m.kq[k6].s[(nsb - 1l) * 16l]), 200, "last sb's subs end the sub-strip")
        t |> equal(int(m.kq[k6].s[nsb * 16l]), 116, "sb0 d low byte opens the d strip")
        t |> equal(int(m.kq[k6].s[nsb * 16l + 1l]), 117, "sb0 d high byte")
        t |> equal(int(m.kq[k6].s[nsb * 16l + (nsb - 1l) * 2l]), 216, "last sb's d low byte closes the strip")
        delete m
    }
    t |> run("the split-scale arms: k3/k6 lead with the 16-byte strip, every other split format leads with the d") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let nsb = 3l
        for (f in [KqFmt.k6, KqFmt.k3, KqFmt.iq4xs, KqFmt.k2]) {
            let ssb = kq_ssb(f)
            let strip_first = f == KqFmt.k6 || f == KqFmt.k3
            let tail = f == KqFmt.k2 ? 4l : 2l              // k2 carries d AND dmin; every other split format d alone
            let strip_at = strip_first ? 0l : 4l            // the source offset the 16 sub-scale bytes start at
            let tail_at = strip_first ? 16l : 0l            // and the one the d bytes start at
            var src <- split_scale_src(ssb, nsb)
            var dst : array<uint8>
            kq_split_scale_plane(f, unsafe(addr<uint8 const?>(src[0])), nsb, dst)
            t |> equal(long_length(dst), nsb * (16l + tail), "{f}: the split plane is nsb x (16 + {tail})")
            for (sb in range64(nsb)) {
                t |> equal(int(dst[sb * 16l]), int(sb * 32l + strip_at), "{f}: sb{sb}'s strip opens at source byte {strip_at}")
                t |> equal(int(dst[sb * 16l + 15l]), int(sb * 32l + strip_at + 15l), "{f}: sb{sb}'s strip closes 16 bytes on")
                for (h in range64(tail)) {
                    t |> equal(int(dst[nsb * 16l + sb * tail + h]), int(sb * 32l + tail_at + h),
                        "{f}: sb{sb}'s tail byte {h} comes from source byte {tail_at + h}")
                }
            }
            // the poison is a value ADDED to sb1's FIRST tail byte - a byte this arm reads only
            // into the tail, and the other arm would read into the strip; it must move exactly
            // one tail byte and nothing in the strip
            var poisoned <- split_scale_src(ssb, nsb)
            poisoned[ssb + tail_at] = uint8(int(poisoned[ssb + tail_at]) + 7)
            var pdst : array<uint8>
            kq_split_scale_plane(f, unsafe(addr<uint8 const?>(poisoned[0])), nsb, pdst)
            t |> equal(int(pdst[nsb * 16l + tail]), int(dst[nsb * 16l + tail]) + 7, "{f}: the poison lands in sb1's tail")
            var strip_moved = 0l
            for (i in range64(nsb * 16l)) {
                if (pdst[i] != dst[i]) {
                    strip_moved++
                }
            }
            t |> equal(strip_moved, 0l, "{f}: and nowhere in the strip")
            delete src
            delete dst
            delete poisoned
            delete pdst
        }
    }
}

// Model-free: the streamed save transforms each kq scale plane in place as its turn comes, and a
// writer that dies mid-walk hands the carrier - planes already converted - to the in-memory
// streamer, which walks it again. The per-carrier memo is what makes that second walk correct.
[test]
def test_blob_stream_scale_plane_once(t : T?) {
    t |> run("the streamed blob build converts a named scale plane at most once per carrier") @(tt : T?) {
        if (!arm_on(tt, "mechanics")) {
            return
        }
        let nsb = 3l
        var src <- split_scale_src(K6_SSB, nsb)
        var once : array<uint8>
        kq_split_scale_plane(KqFmt.k6, unsafe(addr<uint8 const?>(src[0])), nsb, once)
        var m = Model()
        let k6 = int(KqFmt.k6)
        m.kq[k6].s <- split_scale_src(K6_SSB, nsb)
        tt |> success(blob_stream_scale_plane(m, "k6s"), "the first walk converts the plane")
        tt |> success(blob_stream_scale_plane(m, "k6s"), "the second walk answers for a plane already converted")
        tt |> equal(long_length(m.kq[k6].s), long_length(once), "the plane keeps its device-form size")
        var moved = 0l
        for (i in range64(min(long_length(m.kq[k6].s), long_length(once)))) {
            if (m.kq[k6].s[i] != once[i]) {
                moved++
            }
        }
        tt |> equal(moved, 0l, "and every byte is the once-transformed form")
        // the must-red control: the split is NOT a no-op on its own output, so the equality above
        // is the memo's doing and not an accident of the transform
        var twice : array<uint8>
        kq_split_scale_plane(KqFmt.k6, unsafe(addr<uint8 const?>(once[0])), long_length(once) / K6_SSB, twice)
        var differs = 0l
        for (i in range64(min(long_length(twice), long_length(once)))) {
            if (twice[i] != once[i]) {
                differs++
            }
        }
        tt |> success(differs > 0l, "a hand-doubled transform shreds the plane - the memo is load-bearing")
        delete src
        delete once
        delete twice
        delete m
    }
}

//! one scale plane's source superblocks, every byte naming its own (superblock, offset) pair
def private split_scale_src(ssb, nsb : int64) : array<uint8> {
    var @exact_size s : array<uint8>
    s |> reserve_resize(nsb * ssb)
    for (sb in range64(nsb)) {
        for (b in range64(ssb)) {
            s[sb * ssb + b] = uint8(sb * 32l + b)
        }
    }
    return <- s
}

// ===== the meta-layout fingerprint (header byte 48) =====

struct FpInner {
    x : int64
}

struct FpPair {
    a : int
    b : float
}

struct FpPairSwapped {
    b : float
    a : int
}

struct FpPairWide {
    a : int64
    b : float
}

struct FpPairPlus {
    a : int
    b : float
    c : int
}

struct FpNested {
    a : int
    inner : FpInner
}

struct FpRows {
    rows : array<FpInner>
}

struct FpRowsPlus {
    rows : array<FpPairPlus>
}

// the dev-W bake tables: devw_bake_tables fills the four site tables from the job list, and it
// is the WHOLE bake set each time - a second call on the same carrier must not append to the
// first. Model-free: the tables are pure taxonomy over a job list, no plane bytes are read.
[test, arch(at="../ARCHITECTURE_IMAGE.md#image-devw-plane")]
def test_devw_bake_tables(t : T?) {
    t |> run("the bake tables are rebuilt, not appended, and key by format") @(tt : T?) {
        if (!arm_on(tt, "mechanics")) {
            return
        }
        var m = Model()
        m.quant = QuantMode.q8
        m.metal_blob = true   // q8 then streams as the one mblob plane, which the bake covers
        var jobs <- [ FillJob(name = "blk.0.attn_q.weight", woff = 64l, n = 256l, src_off = 0l, fmt = KqFmt.q8),
                      FillJob(name = "blk.0.ffn_up.weight", woff = 512l, n = 512l, src_off = 0l, fmt = KqFmt.k4) ]
        let bytes = devw_bake_tables(m, jobs)
        tt |> equal(bytes, (256l + 512l) * 2l, "panel bytes = the f16 element total")
        tt |> equal(long_length(m.devwf16_key), 2l, "both sites landed")
        // the key is what the prefill's resident lookup recomputes: q8 = the site's mblob BYTE
        // offset, kq = the site's ELEM woff
        tt |> equal(m.devwf16_key[0], (64l / Q8_BLOCK_ELEMS) * (Q8_QPB + Q8_SPB), "q8 key is a byte offset")
        tt |> equal(m.devwf16_key[1], 512l, "kq key is the elem woff")
        tt |> equal(m.devwf16_fmt[0], 0, "q8 sites stamp fmt 0")
        tt |> equal(m.devwf16_fmt[1], kq_stream_code(KqFmt.k4), "kq sites stamp their stream code")
        tt |> equal(m.devwf16_off[0], 0l, "the first panel opens the plane")
        tt |> equal(m.devwf16_off[1], 256l, "the second follows it")
        let again = devw_bake_tables(m, jobs)
        tt |> equal(again, bytes, "a second call reports the same panel bytes")
        tt |> equal(long_length(m.devwf16_key), 2l, "and does not double the key table")
        tt |> equal(long_length(m.devwf16_fmt), 2l, "nor the fmt table")
        tt |> equal(long_length(m.devwf16_off), 2l, "nor the offset table")
        tt |> equal(long_length(m.devwf16_n), 2l, "nor the length table")
        // an expert stack's slice (the per-expert fused gate/up split, or a whole down stack) has no
        // dev-W reader: the routed block reads the quantized planes, so the bake leaves it out. The
        // routed block's SHARED expert reaches dev-W through enc_gemm_mm like any dense site: the
        // exclusion matches "_exps.weight" and not "_shexp.weight", so the shared expert still bakes
        var routed <- [ FillJob(name = "blk.0.attn_q.weight", woff = 64l, n = 256l, src_off = 0l, fmt = KqFmt.q8),
                        FillJob(name = "blk.0.ffn_gate_shexp.weight", woff = 512l, n = 128l, src_off = 0l, fmt = KqFmt.q8),
                        FillJob(name = "blk.0.ffn_gate_up_exps.weight", woff = 1024l, n = 256l, src_off = 0l, fmt = KqFmt.q8),
                        FillJob(name = "blk.0.ffn_down_exps.weight", woff = 2048l, n = 512l, src_off = 0l, fmt = KqFmt.k4) ]
        tt |> equal(devw_bake_tables(m, routed), (256l + 128l) * 2l, "the expert slices bake nothing - the dense site and the shared expert do")
        tt |> equal(long_length(m.devwf16_key), 2l, "two sites in the key table")
        // and the control that the suffix is doing the work: mis-spell it and the same row bakes
        var misspelt <- [ FillJob(name = "blk.0.ffn_gate_up_exps.weights", woff = 1024l, n = 256l, src_off = 0l, fmt = KqFmt.q8) ]
        tt |> equal(devw_bake_tables(m, misspelt), 256l * 2l, "a mis-spelt suffix bakes - the refusal reads the suffix, not the word 'exps'")
        tt |> equal(long_length(m.devwf16_key), 1l, "the mis-spelt row is the one site")
        delete misspelt
        delete routed
        delete jobs
        delete m
    }
    t |> run("a split-scale panel bakes only at a whole-superblock row width and a row count on the 64 lattice") @(tt : T?) {
        if (!arm_on(tt, "mechanics")) {
            return
        }
        var m = Model()
        m.quant = QuantMode.q8
        m.metal_blob = true
        // iq4xs streams as its own split scale pair, so its bake runs through the registered
        // split-scale mirror - and that mirror refuses a panel it cannot walk. One refusal aborts
        // the whole image save, so a site the mirror would refuse is never covered.
        var whole <- [ FillJob(name = "blk.0.ffn_down.weight", woff = 0l, n = 256l * 64l, src_off = 0l, fmt = KqFmt.iq4xs, kdim = 256l) ]
        var odd_width <- [ FillJob(name = "blk.0.ffn_down.weight", woff = 0l, n = 384l * 64l, src_off = 0l, fmt = KqFmt.iq4xs, kdim = 384l) ]
        var odd_rows <- [ FillJob(name = "blk.0.ffn_down.weight", woff = 0l, n = 256l * 32l, src_off = 0l, fmt = KqFmt.iq4xs, kdim = 256l) ]
        static_if (typeinfo builtin_module_exists(das_metal)) {
            tt |> equal(devw_bake_tables(m, whole), 256l * 64l * 2l, "256 wide x 64 rows: whole superblocks on the 64 lattice, baked")
            tt |> equal(long_length(m.devwf16_key), 1l, "the admitted panel is the one site")
            tt |> equal(devw_bake_tables(m, odd_width), 0l, "384 wide: not a whole superblock, refused")
            tt |> equal(long_length(m.devwf16_key), 0l, "and the tables rebuild empty - never left carrying the admitted panel")
            tt |> equal(devw_bake_tables(m, odd_rows), 0l, "32 rows: off the 64 lattice, refused")
        } else {
            // the mirror is the Metal prefill module's [init]; with no module there is no mirror,
            // and the cover predicate refuses every split-scale panel whatever its shape
            tt |> equal(devw_bake_tables(m, whole), 0l, "no split-scale mirror registered: a whole-superblock panel is refused too")
            tt |> equal(devw_bake_tables(m, odd_width), 0l, "and so is an odd width")
            tt |> equal(devw_bake_tables(m, odd_rows), 0l, "and an odd row count")
        }
        delete whole
        delete odd_width
        delete odd_rows
        delete m
    }
}

[test]
def test_layout_fingerprint(t : T?) {
    t |> run("the fingerprint moves on any field-shape change and holds on none") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        var p1 = FpPair()
        var p2 = FpPair()
        t |> equal(layout_fingerprint(p1), layout_fingerprint(p2), "deterministic per type")
        var sw = FpPairSwapped()
        t |> success(layout_fingerprint(p1) != layout_fingerprint(sw), "field REORDER moves it")
        var wd = FpPairWide()
        t |> success(layout_fingerprint(p1) != layout_fingerprint(wd), "field RETYPE moves it")
        var pl = FpPairPlus()
        t |> success(layout_fingerprint(p1) != layout_fingerprint(pl), "field ADD moves it")
        var ns = FpNested()
        var iv = FpInner()
        t |> success(layout_fingerprint(ns) != layout_fingerprint(iv), "nesting reaches the print")
        var r1 = FpRows()
        var r2 = FpRowsPlus()
        t |> success(layout_fingerprint(r1) != layout_fingerprint(r2), "array ELEMENT shape reaches the print")
        var m = Model()
        let mfp = layout_fingerprint(m)
        to_log(LOG_INFO, "image fingerprint: Model meta closure = {mfp}\n")
        t |> success(mfp != 0ul, "the real Model closure fingerprints (no recursion trap)")
        delete m
    }
    t |> run("a corrupted fingerprint byte refuses the image at parse") @(t : T?) {
        if (!arm_on(t, "mechanics")) {
            return
        }
        let path = path_join(test_temp_dir(), "dasllama_img_fp.dlim")
        var p <- make_probe()
        t |> success(save_image(p, path, "img-probe"), "save")
        var f = fopen(path, "rb+")
        t |> success(f != null, "reopen for the poke")
        fseek(f, 48l, seek_set)
        var one : array<uint8>
        one |> push(uint8(0xA5))
        fwrite(f, one)
        fclose(f)
        var m = ImgProbe()
        t |> success(!load_image(path, m, "img-probe"), "a flipped fingerprint byte declines the load")
        delete m
        delete p
        remove(path)
    }
}

[test]
def test_dlim_identity_pack_version(t : T?) {
    t |> run("the interleave segment has one slot per lattice format, in enum order") <| @(t : T?) {
        var lattice = 0
        for (i in range(KQ_FMT_COUNT)) {
            if (kq_sb(i)) {
                lattice++
            }
        }
        var c = DlimConfiguration()
        let base = dlim_identity(c, IMAGE_VERSION, "", 0)
        let seg = base |> find("|kq ")
        t |> success(seg >= 0, "the identity carries its kq segment: {base}")
        let rest = slice(base, seg + 4)
        let segment = slice(rest, 0, rest |> find("|"))
        var slots = 1
        for (ch in segment) {
            if (ch == '/') {
                slots++
            }
        }
        t |> equal(slots, lattice, "one slot per lattice format: {segment}")
        c.cpu.kq_mr |> resize(KQ_FMT_COUNT)
        c.cpu.kq_mr[int(KqFmt.k4)] = 8l
        let k4 = dlim_identity(c, IMAGE_VERSION, "", 0)
        c.cpu.kq_mr[int(KqFmt.k4)] = 0l
        c.cpu.kq_mr[int(KqFmt.k6)] = 8l
        let k6 = dlim_identity(c, IMAGE_VERSION, "", 0)
        t |> success(k4 != base && k6 != base && k4 != k6, "the slot a format's interleave lands in is its own: {k4} vs {k6}")
    }
    t |> run("the pack-code version folds into the identity and nothing else moves") <| @(t : T?) {
        let c = DlimConfiguration()
        let base = dlim_identity(c, IMAGE_VERSION, "", 0)
        let packed = dlim_identity(c, IMAGE_VERSION, "", 7)
        t |> success(base != packed, "pack_version 7 changes the identity")
        t |> success(packed |> starts_with("v{IMAGE_VERSION}p7|"), "the pack version rides the schema version: {packed}")
        t |> success(base |> starts_with("v{IMAGE_VERSION}|"), "pack_version 0 is the pre-versioned spelling: {base}")
        t |> equal(image_identity_of(c), dlim_identity(c, IMAGE_VERSION, "", PACK_VERSION), "image_identity_of folds the layout's PACK_VERSION")
    }
}
