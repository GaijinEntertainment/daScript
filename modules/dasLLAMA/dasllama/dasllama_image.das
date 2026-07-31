options gen2
options _comment_hygiene = true

module dasllama_image shared public
options _cyclomatic_complexity = 0   // image walk/GC bodies mirror the archive schema
options _function_length = 0   // same

require daslib/fio
require dasllama/dasllama_env  // the shared typed env readers + the knob registry
require daslib/apply
require strings
require dasllama/dasllama_tokenizer   // Tokenizer — the bulk tokenizer serializer below
require daslib/archive
require daslib/array_boost   // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs (same note as dasllama_common)
require dasllama/dasllama_common
require dasllama/dasllama_gguf   // GGUFMeta — the streamed cold build's block signature names it
require dasllama/dasllama_layout   // convert_model_to_metal_blob — the blob-flavor image transform
require dasllama/dasllama_math   // active_kernel_backend — half the image identity
require dasllama/dasllama_config   // DlimConfiguration + dlim_identity — the identity formatter
require math

// The prepared-model image (spec: fmap-model-image plan, locked 2026-07-16): the post-load
// Model dumped once — planes POST-repack, page-aligned — then mapped back with zero O(model)
// allocation or copying. Plane fields become borrowed views over the mapping (the Model
// finalizer forgets them and unmaps); everything else (config, scalars, tokenizer, arch
// blocks) rides a daslib/archive blob and is PARSED at load (the small-structure carve-out).
// Images are BOX + KNOB specific (identity below) and versioned — a mismatch regenerates,
// never migrates. The load path touches only the header + meta blob: bigger-than-RAM images
// stay lazy (full verification belongs to the offline tool, not load).
// Nested weight-carriers (a struct field that itself declares image_map, e.g.
// WhisperModel.enc) contribute their planes under dotted names ("enc.fblob"); string-array
// fields ride the meta blob via serialize_strings — raw string pointers can't be planes.

// Bump on ANY change to the image's layout/serialization OR to WHAT THE LOADER PUTS IN IT — the
// identity below is computed before the gguf is parsed, so it can carry box/knob state but nothing
// config-derived. A loader change that alters which tensors load leaves a structurally VALID stale
// image that silently represents a different model; only this version catches it. Mismatch
// regenerates from the gguf, never migrates.
let IMAGE_VERSION = 7   // v7: identity string + DlimConfiguration JSON ride the meta head (readable declines; the .dlim-arc format changes batch into this one bump)

//! The metal (blob-only) flavor's identity tag: q8 planes ride the 34B block_q8_0 blob and the
//! kq scale planes their GPU forms (convert_model_to_metal_blob) — flavors are per-config and
//! self-sufficient, so the tag keeps the two flavors' files and identities apart.
let METAL_IMAGE_TAG = "metal"
let IMAGE_PAGE = 16384l          // Apple-Silicon page: the bytesNoCopy alignment contract
let IMAGE_HEADER_BYTES = 64l     // magic..identity, inside the first page
let IMAGE_MAGIC = 0x314D4C44u    // 'DLM1'

// public only for the save_image/load_image generics — they instantiate from caller modules
struct ImgSection {
    name : string
    off : uint64      // absolute byte offset, page-aligned
    bytes : uint64
}

// the box + knob identity an image is valid for; folded into filename AND header. A PURE
// formatter of DlimConfiguration — the backend-select side effect lives in the config's cpu
// source, so callers need no ordering ritual. `tag` distinguishes kinds sharing one source path.
def image_identity(tag : string = ""; quant : string = "q8") : string {
    return dlim_identity(dlim_config_current(quant), IMAGE_VERSION, tag)
}

// the image path for (gguf, current identity) — knob combos coexist as separate files
// (the full 64-bit hash keeps that deterministic in practice; the header check would catch
// a collision loudly, but two identities fighting over one path re-save forever)
def image_path_for(gguf_path : string; tag : string = ""; quant : string = "q8") : string {
    return "{gguf_path}.{hash(image_identity(tag, quant))}.dlim"
}

//! What list/GC tooling reads from an image WITHOUT loading it: the header fields plus the
//! meta head's baked identity and embedded config JSON (empty when the version's layout is
//! unknown — pre-v7 images carry no readable head).
struct DlimPeek {
    valid : bool          //!< magic/page/bounds all sane (any version)
    version : int
    bytes : int64
    identity : string
    config_json : string
}

def image_peek(path : string) : DlimPeek {
    var p = DlimPeek()
    var msize = 0ul
    var base : void?
    unsafe {
        base = fmap_open(path, addr(msize))
    }
    if (base == null || int64(msize) < IMAGE_HEADER_BYTES) {
        if (base != null) {
            unsafe(fmap_close(base, msize))
        }
        return p
    }
    var bp = unsafe(reinterpret<uint8 const?>(base))
    p.bytes = int64(msize)
    p.version = int(read_u32(bp, 4l))
    let meta_off = read_u64(bp, 16l)
    let meta_bytes = read_u64(bp, 24l)
    p.valid = (read_u32(bp, 0l) == IMAGE_MAGIC && read_u32(bp, 8l) == uint(IMAGE_PAGE)
        && read_u64(bp, 32l) == msize && meta_bytes != 0ul && meta_bytes <= msize
        && meta_off <= msize - meta_bytes && meta_bytes <= uint64(INT_MAX))
    if (p.valid && p.version >= 7) {
        var head : array<uint8>
        head |> resize(int(min(meta_bytes, 65536ul)))
        unsafe {
            memcpy(addr(head[0]), reinterpret<void?>(bp + int64(meta_off)), uint64(long_length(head)))
        }
        var hmeta <- new MemSerializer(head)
        var harch = Archive(reading = true, stream = hmeta)
        harch |> serialize(p.identity)
        harch |> serialize(p.config_json)
        if (!hmeta->OK()) {
            p.identity = ""
            p.config_json = ""
        }
        delete head
        unsafe {
            delete hmeta
        }
    }
    unsafe(fmap_close(base, msize))
    return p
}

//! One .dlim sibling of a GGUF, as list/GC tooling sees it (converter --list, the server page).
struct DlimImageInfo {
    file : string         //!< basename
    path : string
    bytes : int64
    version : int
    verdict : string      //!< CURRENT | OTHER | STALE vN | BROKEN
    identity : string
}

// family flavor tags (whisper-q8, tower-f32, parakeet-q8, …) — each family module's [init]
// registers its own, so inventory/GC verdicts recognize those images as CURRENT
var private g_family_tags : array<string>

var private g_image_frozen = false

//! Artifact-frozen mode — the perf-oracle contract: ``load_model_cached`` loads ONLY existing
//! prepared images, and any load that would mint/bake a flavor panics instead of falling
//! through to the gguf. Arms via lcpp_bench ``--frozen``.
def set_image_artifacts_frozen(frozen : bool) {
    g_image_frozen = frozen
}

//! Register an image family/flavor tag (call from the family module's [init]) so
//! ``dlim_current_identities`` — and with it every list/GC verdict — covers its images.
def register_image_family_tag(tag : string) {
    if (!has_value(g_family_tags, tag)) {
        g_family_tags |> push(tag)
    }
}

//! The identities THIS box would produce right now, across flavors and quants — the CURRENT
//! verdict's reference set. Callers pin the box profile first (same ordering as the load path).
def dlim_current_identities : table<string> {
    var idents : table<string>
    for (q in ["q8", "fp32", "q4_0"]) {
        idents |> insert(image_identity("", q))
        idents |> insert(image_identity(METAL_IMAGE_TAG, q))
        let vk = moe_gpu_bake_tag()
        if (vk != "") {
            idents |> insert(image_identity(vk, q))
        }
    }
    for (t in g_family_tags) {
        idents |> insert(image_identity(t))   // family tags fold their flavor into the tag itself
    }
    return <- idents
}

def dlim_image_verdict(p : DlimPeek; current : table<string>) : string {
    if (!p.valid) {
        return "BROKEN"
    }
    if (p.version != IMAGE_VERSION) {
        return "STALE v{p.version}"
    }
    return key_exists(current, p.identity) ? "CURRENT" : "OTHER"
}

//! Every .dlim beside `gguf_path` (or in a directory), with sizes and verdicts.
def dlim_inventory(gguf_path : string) : array<DlimImageInfo> {
    var out : array<DlimImageInfo>
    var st : FStat
    if (!stat(gguf_path, st)) {
        return <- out
    }
    let dirp = st.is_dir ? gguf_path : dir_name(gguf_path)
    let base = st.is_dir ? "" : base_name(gguf_path)
    var current <- dlim_current_identities()
    dir(dirp) $(name : string) {
        if (!(name |> ends_with(".dlim")) || (base != "" && !(name |> starts_with(base)))) {
            return
        }
        let full = path_join(dirp, name)
        let p = image_peek(full)
        out |> push(DlimImageInfo(file = name, path = full, bytes = p.bytes, version = p.version,
                                  verdict = dlim_image_verdict(p, current), identity = p.identity))
    }
    delete current
    return <- out
}

//! GC-after-save: a fresh bake proves every non-CURRENT sibling of this source dead — BROKEN,
//! version-stale, AND other-identity images all go (old identities piled up 0.5 TB per box).
//! CURRENT spans all flavors/quants/families, so a planar save never touches the metal twin.
def dlim_gc_stale(source_path : string) {
    let r = dlim_clean(source_path, true)
    if (r.removed > 0) {
        to_log(LOG_INFO, "dasLLAMA image: GC freed {r.freed >> 20l} MB — {r.removed} dead sibling(s) of {base_name(source_path)}\n")
    }
}

//! The dlim GC beside `gguf_path`: everything non-CURRENT goes by default — BROKEN,
//! version-stale, AND valid other-identity siblings (old identities pile up across re-mints).
//! ``keep_other = true`` is the opt-out for a bake deliberately carried from another box.
def dlim_clean(gguf_path : string; apply : bool; keep_other : bool = false) : tuple<total : int; stale : int; removed : int; freed : int64> {
    var r = (total = 0, stale = 0, removed = 0, freed = 0l)
    var inv <- dlim_inventory(gguf_path)
    for (im in inv) {
        r.total++
        continue if (im.verdict == "CURRENT" || (keep_other && im.verdict == "OTHER"))
        r.stale++
        if (apply && remove(im.path)) {
            r.removed++
            r.freed += im.bytes
            to_log(LOG_INFO, "dasLLAMA image: GC removed {im.file} ({im.bytes >> 20l} MB, {im.verdict})\n")
        }
    }
    delete inv
    return r
}

def private pad_to_page(cur : uint64) : uint64 {
    let page = uint64(IMAGE_PAGE)
    return (cur + page - 1ul) / page * page
}

let private IMAGE_WRITE_BAND = 16l * 1024l * 1024l   // the direct writer's staging band
let private SECTION_TABLE_SLACK = 65536ul            // preallocation headroom for the section blob

// The image's byte sink: append-only and strictly ascending, which is what keeps NTFS from
// charging valid-data-length zero-fill on the preallocated file (measured: 3.4 s per 8 GB gap).
// Two backings, one walk: a memory chunk or the direct file writer.
struct private ImgWriter {
    h : void? = null       // the direct file writer; null on the memory rail
    mem : void? = null     // the page-aligned chunk; null on the file rail
    cap : uint64 = 0ul     // the chunk's usable extent — an append past it is a sizing bug
    cur : uint64 = 0ul
    ok : bool = true
}

def private w_open(path : string; total : uint64) : ImgWriter {
    let h = unsafe(dwrite_open(path, total, uint64(IMAGE_WRITE_BAND)))
    return ImgWriter(h = h, ok = h != null)
}

// the memory sink. `total` must be image_total_bytes' answer — the chunk is exact, so an overrun
// means the sizing pass and the walk disagree, which is a bug and not a disk condition
def private w_open_mem(total : uint64) : ImgWriter {
    let base = image_chunk_alloc(total, uint64(IMAGE_PAGE))
    if (base == null) {
        to_log(LOG_WARNING, "dasLLAMA image: cannot allocate a {int64(total) >> 20l} MB image chunk\n")
    }
    return ImgWriter(mem = base, cap = total, ok = base != null)
}

def private mem_at(base : void?; off : uint64) : void? {
    let at = unsafe(reinterpret<uint64>(base)) + off
    return unsafe(reinterpret<void?>(at))
}

def private w_append(var w : ImgWriter; p : void?; nbytes : uint64) {
    if (!w.ok || nbytes == 0ul) {
        return
    }
    if (w.mem != null) {
        if (nbytes > w.cap - w.cur) {
            to_log(LOG_ERROR, "dasLLAMA image: chunk overrun at {int64(w.cur)} + {int64(nbytes)} > {int64(w.cap)} — image sizing is wrong\n")
            w.ok = false
            return
        }
        unsafe {
            memcpy(mem_at(w.mem, w.cur), p, nbytes)
        }
    } elif (!unsafe(dwrite_append(w.h, p, nbytes))) {
        w.ok = false   // a dead save stays dead — don't keep pushing bytes at a full disk
        return
    }
    w.cur += nbytes
}

def private w_zeros(var w : ImgWriter; n : uint64) {
    if (n == 0ul || !w.ok) {
        return
    }
    var z : array<uint8>
    z |> resize(int(n))
    unsafe {
        w_append(w, addr<void?>(z[0]), n)
    }
    delete z
}

def private w_close(var w : ImgWriter) : bool {
    if (w.mem != null) {
        return w.ok   // nothing to flush: the chunk IS the image
    }
    if (w.h == null) {
        return false
    }
    let flushed = unsafe(dwrite_close(w.h))
    w.h = null
    return flushed && w.ok
}

// Page 0, which only the finished walk knows (section count, meta offset and size). The memory
// rail stores it in place; the file rail's direct writer is append-only, so it patches through an
// ordinary handle after close — which is why `path` is a parameter and not w's business.
def private w_header(var w : ImgWriter; path : string; var hdr : array<uint8>) : bool {
    if (w.mem != null) {
        unsafe {
            memcpy(w.mem, addr<void?>(hdr[0]), uint64(IMAGE_HEADER_BYTES))
        }
        return true
    }
    var ok = false
    fopen(path, "r+b") $(hf) {
        if (hf != null) {
            fseek(hf, 0l, seek_set)
            ok = (hf |> long_fwrite(hdr)) == IMAGE_HEADER_BYTES
        }
    }
    return ok
}

def private plane_end(cur : uint64; field : array<auto(TT)>) : uint64 {
    if (empty(field)) {
        return cur
    }
    return pad_to_page(cur) + uint64(long_length(field)) * uint64(typeinfo sizeof(field[0]))
}

// The image's final size, known before a byte goes out: planes contribute their sizes, the meta
// blob is already serialized. The writer preallocates from this, so the file lands as one
// contiguous run.
def private image_total_bytes(var t; meta_bytes : uint64) : uint64 {
    var cur = uint64(IMAGE_PAGE)
    apply(t) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            static_if (!typeinfo is_string(field[0])) {
                cur = plane_end(cur, field)
            }
        } static_elif (typeinfo safe_has_field < image_map > (field)) {
            apply(field) $ [unused_argument(iname)] (iname : string; var ifield) {
                static_if (typeinfo is_array(ifield)) {
                    static_if (!typeinfo is_string(ifield[0])) {
                        cur = plane_end(cur, ifield)
                    }
                }
            }
        }
    }
    return pad_to_page(cur + meta_bytes)
}

def private store_u32(var b : array<uint8>; at : int; v : uint) {
    unsafe {
        var p = unsafe(addr < uint? >(b[at]))
        p[0] = v
    }
}

def private store_u64(var b : array<uint8>; at : int; v : uint64) {
    unsafe {
        var p = unsafe(addr < uint64? >(b[at]))
        p[0] = v
    }
}

def private read_u32(p : uint8 const?; at : int64) : uint {
    unsafe {
        var q = reinterpret<uint const?>(p + at)
        return q[0]
    }
}

def private read_u64(p : uint8 const?; at : int64) : uint64 {
    unsafe {
        var q = reinterpret<uint64 const?>(p + at)
        return q[0]
    }
}



// bulk POD-array serialize: ONE stream call for the payload. daslib/archive's per-element
// generic dispatch measured ~340us/element — a 128k-vocab tokenizer took 238s through it.
def private serialize_pod_array(var arch : Archive; var a : array<auto(TT)>) {
    var n = length(a)
    arch |> serialize_raw(n)
    if (arch.reading) {
        a |> resize(n)
    }
    if (n > 0) {
        let nbytes = n * typeinfo sizeof(type<TT>)
        unsafe {
            if (arch.reading) {
                arch.stream->read(addr(a[0]), nbytes)
            } else {
                arch.stream->write(addr(a[0]), nbytes)
            }
        }
    }
}

//! String arrays as [lengths][one blob] — two bulk payloads instead of per-string dispatch.
//! Public because string-array fields cannot be raw planes (they hold pointers) — a type's
//! serialize_image_meta must route them through this instead.
def serialize_strings(var arch : Archive; var a : array<string>) {
    var lens : array<int>
    var blob : array<uint8>
    if (!arch.reading) {
        lens |> reserve(length(a))
        var total = 0
        for (v in a) {
            let n = length(v)
            lens |> push(n)
            total += n
        }
        blob |> resize(total)
        var at = 0
        for (v in a) {
            let n = length(v)
            if (n > 0) {
                peek_data(v) $(bytes : array<uint8> const#) {
                    unsafe {
                        memcpy(addr(blob[at]), addr(bytes[0]), n)
                    }
                }
            }
            at += n
        }
    }
    serialize_pod_array(arch, lens)
    serialize_pod_array(arch, blob)
    if (arch.reading) {
        a |> resize(length(lens))
        var at = 0
        for (i in range(length(lens))) {
            let n = lens[i]
            if (n > 0) {
                unsafe {
                    a[i] = string(temp_array(addr < uint8? >(blob[at]), n, type<uint8>))
                }
            } else {
                a[i] = ""
            }
            at += n
        }
    }
    delete lens
    delete blob
}

// the tokenizer, bulk: vocab/scores/token_type/merges as flat payloads; the lookup tables are
// DERIVED (piece -> index, merge -> rank-order index) and rebuild in milliseconds at load —
// exactly the "parse at load" carve-out the format spec allows
def private serialize_tokenizer(var arch : Archive; var tk : Tokenizer) {
    arch |> serialize_raw(tk.kind)
    serialize_strings(arch, tk.spm.vocab)
    serialize_pod_array(arch, tk.spm.scores)
    arch |> serialize_raw(tk.spm.max_token_length)
    arch |> serialize_raw(tk.spm.add_space_prefix)
    arch |> serialize_raw(tk.spm.bos_id)
    arch |> serialize_raw(tk.spm.eos_id)
    serialize_strings(arch, tk.bpe.vocab)
    serialize_pod_array(arch, tk.bpe.token_type)
    arch |> serialize(tk.bpe.pre)
    arch |> serialize_raw(tk.bpe.bos_id)
    arch |> serialize_raw(tk.bpe.eos_id)
    arch |> serialize_raw(tk.bpe.spm_space)
    arch |> serialize_raw(tk.bpe.add_bos)
    var merges : array<string>
    if (!arch.reading) {
        merges |> resize(length(tk.bpe.merge_rank))
        for (k, v in keys(tk.bpe.merge_rank), values(tk.bpe.merge_rank)) {
            merges[int(v)] = k
        }
    }
    serialize_strings(arch, merges)
    if (arch.reading) {
        for (i, v in count(), tk.spm.vocab) {
            tk.spm.lookup[v] = int64(i)
        }
        for (i, v in count(), tk.bpe.vocab) {
            tk.bpe.lookup[v] = int64(i)
        }
        for (i, v in count(), merges) {
            tk.bpe.merge_rank[v] = int64(i)
        }
    }
    delete merges
}

//! Model's half of the image contract: every non-array field, both directions, with a count
//! tripwire so the list grows with the struct (blocks re-binds in image_post_load instead).
//! Public only because build/parse resolve `_::` at the instantiation ROOT — not a call-me API.
def serialize_image_meta(var arch : Archive; var t : Model) {
    verify(count_meta_fields(t) == IMAGE_META_FIELDS)   // grew Model? extend this list
    arch |> serialize(t.config)
    arch |> serialize(t.arch)
    arch |> serialize(t.chat_template_str)
    serialize_tokenizer(arch, t.tok)
    arch |> serialize_raw(t.tok_emb_off)
    arch |> serialize_raw(t.rms_att_off)
    arch |> serialize_raw(t.rms_ffn_off)
    arch |> serialize_raw(t.rms_final_off)
    arch |> serialize_raw(t.rms_post_att_off)
    arch |> serialize_raw(t.rms_post_ffn_off)
    arch |> serialize_raw(t.rms_ones_off)
    arch |> serialize_raw(t.out_scale_off)
    arch |> serialize_raw(t.ple_emb_off)
    arch |> serialize_raw(t.ple_model_off)
    arch |> serialize_raw(t.ple_pnorm_off)
    arch |> serialize_raw(t.ple_gate_off)
    arch |> serialize_raw(t.ple_proj_off)
    arch |> serialize_raw(t.ple_post_off)
    arch |> serialize_raw(t.wscale_f16)
    arch |> serialize_raw(t.metal_blob)
    arch |> serialize_raw(t.planes_trimmed)
    arch |> serialize_raw(t.experts_mx4)
    arch |> serialize_raw(t.ple_model_bf16)
    arch |> serialize_raw(t.kquant_native)
    arch |> serialize_raw(t.kq_repacked)
    arch |> serialize_raw(t.mx4_repacked)
    arch |> serialize_raw(t.kq_repack_mr4)
    arch |> serialize_raw(t.kq_repack_mr5)
    arch |> serialize_raw(t.kq_repack_mr6)
    arch |> serialize_raw(t.kq_repack_mr40)
    arch |> serialize_raw(t.wcls_fmt)
    arch |> serialize_raw(t.emb_fmt)
    arch |> serialize_raw(t.ple_emb_fmt)
    arch |> serialize_raw(t.router_off)
    arch |> serialize_raw(t.shexp_gate_off)
    arch |> serialize_raw(t.moe_router_scale_off)
    arch |> serialize_raw(t.moe_down_scale_off)
    arch |> serialize_raw(t.rms_pre_ffn2_off)
    arch |> serialize_raw(t.rms_post_ffn1_off)
    arch |> serialize_raw(t.rms_post_ffn2_off)
    arch |> serialize_raw(t.router_b_off)
    arch |> serialize_raw(t.exp_probs_off)
    arch |> serialize_raw(t.web1_off)
    arch |> serialize_raw(t.web2_off)
    arch |> serialize_raw(t.web3_off)
    arch |> serialize_raw(t.sinks_off)
    arch |> serialize_raw(t.wsh1_off)
    arch |> serialize_raw(t.wsh2_off)
    arch |> serialize_raw(t.wsh3_off)
    arch |> serialize_raw(t.wcls_off)
    arch |> serialize_raw(t.cls_q8)
    arch |> serialize_raw(t.emb_q8_off)
    arch |> serialize_raw(t.quant)
    arch |> serialize_raw(t.dn_conv_off)
    arch |> serialize_raw(t.dn_dt_off)
    arch |> serialize_raw(t.dn_a_off)
    arch |> serialize_raw(t.dn_norm_off)
    arch |> serialize_raw(t.dn_ba_f32)
    arch |> serialize_raw(t.dn_betaf_off)
    arch |> serialize_raw(t.dn_alphaf_off)
    arch |> serialize_raw(t.dn_grouped)
    arch |> serialize_raw(t.mtp_ehproj_off)
    arch |> serialize_raw(t.mtp_ehproj_fmt)
    arch |> serialize_raw(t.mtp_enorm_off)
    arch |> serialize_raw(t.mtp_hnorm_off)
    arch |> serialize_raw(t.mtp_headnorm_off)
}

// the tripwire: serialize_image_meta(Model) covers 66 fields; blocks + image_map +
// image_bytes are deliberate skips. A new non-array Model field trips this until BOTH agree.
let IMAGE_META_FIELDS = 66 + 3

//! Count of meta-carried fields of any struct: non-arrays plus string arrays (those cannot
//! be raw planes — see serialize_strings). Pair with a per-type field-count constant to
//! tripwire an explicit serialize_image_meta list against struct growth.
def count_meta_fields(var t) : int {
    var n = 0
    apply(t) $ [unused_argument(name, field)] (name : string; var field) {
        static_if (!typeinfo is_array(field)) {
            n++
        } static_elif (typeinfo is_string(field[0])) {
            n++
        }
    }
    return n
}

//! Model's post-read fix-up: blocks re-bind from the registry. Deliberately NOT resolve_arch —
//! its configure would clobber serialized gguf overrides with the arch's fallbacks. Public for
//! the same `_::` reason as serialize_image_meta.
def image_post_load(var t : Model) {
    rebind_arch_blocks(t, t.arch)
}

// ===== save =====

// one plane: pad to a page, bulk-append, account. The writer advances w.cur by exactly what it
// accepted, so the section table cannot drift from the file. long_ forms: a plane past 2 GiB
// (voxtral-mini qblob = 3.6 G elements) does not fit the int32 rail.
def private write_plane(var w : ImgWriter; var sections : array<ImgSection>;
                        name : string; var field : array<auto(TT)>) {
    if (empty(field) || !w.ok) {
        return
    }
    let nbytes = uint64(long_length(field)) * uint64(typeinfo sizeof(field[0]))
    let at = pad_to_page(w.cur)
    w_zeros(w, at - w.cur)
    let ts = ref_time_ticks()
    unsafe {
        w_append(w, addr<void?>(field[0]), nbytes)
    }
    // big planes get their own line: a uniform rate means the cost is systemic, a few slow ones
    // mean stalls. Only >=256 MB, so small models stay quiet and this needs no env knob.
    if (nbytes >= 256ul * 1024ul * 1024ul) {
        let ms = int64(get_time_usec(ts)) / 1000l
        let mb = int64(nbytes) >> 20l
        to_log(LOG_INFO, "dasLLAMA image:   plane {name} {mb} MB in {ms} ms = {ms > 0l ? mb * 1000l / ms : 0l} MB/s\n")
    }
    if (!w.ok) {
        to_log(LOG_ERROR, "dasLLAMA image: plane '{name}' failed writing {int64(nbytes)} bytes at offset {int64(at)} — disk full? image save aborted\n")
        return
    }
    sections |> push(ImgSection(name = name, off = at, bytes = nbytes))
}

//! THE image build — every prepared image in the process comes out of this walk, whichever sink it
//! lands in: planes page-aligned raw (borrowed back at load), everything else archived. `plane`
//! gets first refusal on each plane by name; `consume` frees each owned plane as it lands.
def private build_image(var t; dst : string; in_mem : bool; extra : uint64;   // nolint:STYLE038 — one linear pass; the phases share w/sections and split into nothing reusable
                        tag : string; quant : string; consume : bool;
                        plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : ImgWriter {
    // one config snapshot serves the meta embed AND the header hash — they must agree
    let dcfg = dlim_config_current(quant)
    let ident = dlim_identity(dcfg, IMAGE_VERSION, tag)
    var fend = 0ul
    var meta_off = 0ul
    var meta_bytes = 0ul
    var nsections = 0
    var meta <- new MemSerializer()
    var warch = Archive(reading = false, stream = meta)
    var sections : array<ImgSection>
    // meta first: it needs nothing from the planes, and having it sized up front is what lets
    // the sink preallocate exactly (see image_total_bytes)
    let ts_meta = ref_time_ticks()
    _::serialize_image_meta(warch, t)
    to_log(LOG_INFO, "dasLLAMA image: meta serialize {get_time_usec(ts_meta) / 1000}ms\n")
    var scalar_blob <- meta->extractData()
    unsafe {
        delete meta
    }
    let total = image_total_bytes(t, uint64(long_length(scalar_blob)) + SECTION_TABLE_SLACK) + extra
    var w = in_mem ? w_open_mem(total) : w_open(dst, total)
    if (w.ok) {
        w_zeros(w, uint64(IMAGE_PAGE))   // page 0: header placeholder — stored once the walk knows it
        let ts_planes = ref_time_ticks()
        apply(t) $(name : string; var field) {
            static_if (typeinfo is_array(field)) {
                static_if (!typeinfo is_string(field[0])) {   // string arrays ride the meta blob
                    if (!invoke(plane, name, w, sections)) {
                        write_plane(w, sections, name, field)
                    }
                    // lock_count != 0 is a BORROWED view over some other image — never ours to free
                    if (consume && lock_count(field) == 0) {
                        delete field
                    }
                }
            } static_elif (typeinfo safe_has_field < image_map > (field)) {
                // a nested weight-carrier on the image rail (marker: it declares image_map) —
                // its planes join this image under dotted names; scalars ride the meta blob
                // via the outer type's serialize_image_meta (e.g. WhisperModel.enc)
                apply(field) $(iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        static_if (!typeinfo is_string(ifield[0])) {
                            write_plane(w, sections, "{name}.{iname}", ifield)
                            if (consume && lock_count(ifield) == 0) {
                                delete ifield
                            }
                        }
                    }
                }
            }
        }
        let planes_ms = int64(get_time_usec(ts_planes)) / 1000l
        let planes_mb = int64(w.cur) >> 20l
        to_log(LOG_INFO, "dasLLAMA image: planes write {planes_ms}ms ({planes_mb} MB at {planes_ms > 0l ? planes_mb * 1000l / planes_ms : 0l} MB/s)\n")
        if (w.h != null) {
            // the file writer's own split — staging our bytes vs waiting on the device. Anything
            // the two do not account for is time spent OUTSIDE the writer (the walk itself).
            let copy_ms = int64(unsafe(dwrite_stat(w.h, 0))) / 1000000l
            let write_ms = int64(unsafe(dwrite_stat(w.h, 1))) / 1000000l
            let direct_mb = int64(unsafe(dwrite_stat(w.h, 2))) >> 20l
            let bounce_mb = int64(unsafe(dwrite_stat(w.h, 3))) >> 20l
            to_log(LOG_INFO, "dasLLAMA image:   writer split: stage {copy_ms}ms ({bounce_mb} MB bounced), syscall {write_ms}ms ({direct_mb} MB direct), unaccounted {planes_ms - copy_ms - write_ms}ms\n")
        }
        // meta = [identity string][config JSON][sections][the walk's scalar stream], one blob at
        // the tail — the two strings lead so a mismatched load can print WHAT the image was
        // baked for without parsing anything else
        var smeta <- new MemSerializer()
        var sarch = Archive(reading = false, stream = smeta)
        var m_ident = ident
        var m_cfg = dlim_config_json(dcfg)
        sarch |> serialize(m_ident)
        sarch |> serialize(m_cfg)
        sarch |> serialize(sections)
        var meta_blob <- smeta->extractData()
        meta_blob |> push_from(scalar_blob)
        nsections = length(sections)
        meta_off = w.cur
        meta_bytes = uint64(long_length(meta_blob))
        if (meta_bytes > 0ul) {   // never empty in practice — the section table always rides here
            unsafe {
                w_append(w, addr<void?>(meta_blob[0]), meta_bytes)
            }
        }
        // pad the image itself to a page multiple (the whole-image bytesNoCopy wrap needs it)
        fend = pad_to_page(w.cur)
        w_zeros(w, fend - w.cur)
        delete meta_blob
        unsafe {
            delete smeta
        }
        delete sections
    }
    delete scalar_blob
    w.ok = w_close(w)
    if (w.ok) {
        var hdr : array<uint8>
        hdr |> resize(int(IMAGE_HEADER_BYTES))
        store_u32(hdr, 0, IMAGE_MAGIC)
        store_u32(hdr, 4, uint(IMAGE_VERSION))
        store_u32(hdr, 8, uint(IMAGE_PAGE))
        store_u32(hdr, 12, uint(nsections))
        store_u64(hdr, 16, meta_off)
        store_u64(hdr, 24, meta_bytes)
        store_u64(hdr, 32, fend)
        store_u64(hdr, 40, hash(ident))
        w.ok = w_header(w, dst, hdr)
        if (!w.ok) {
            to_log(LOG_ERROR, "dasLLAMA image: '{dst}' header write failed — image build aborted\n")
        }
        delete hdr
    }
    w.cur = fend   // the image's true extent, for whoever maps or adopts it
    return w
}

// tmp+rename: atomic on POSIX; on Windows a brief missing-file window costs a reader a regenerate
def private promote_tmp(tmp_path : string; path : string; built : bool; want_bytes : uint64) : bool {
    var ok = built
    if (ok) {
        // an ENOSPC that only surfaces at close still leaves a short file — check what landed
        var st : FStat
        if (!stat(tmp_path, st) || st.size != want_bytes) {
            to_log(LOG_ERROR, "dasLLAMA image: '{tmp_path}' is {int64(st.size)} bytes on disk, expected {int64(want_bytes)} — disk full? image save aborted\n")
            ok = false
        }
    }
    if (!ok) {
        // a failed OPEN never created the file, so only warn about a tmp that is actually there
        if (stat(tmp_path).is_valid && !remove(tmp_path)) {
            to_log(LOG_WARNING, "dasLLAMA image: orphaned tmp '{tmp_path}' could not be removed — clean it up by hand\n")
        }
        return false
    }
    if (rename(tmp_path, path)) {
        return true   // the atomic replace (POSIX rename-over)
    }
    // rename-over-existing is not portable (Windows) — drop the stale image and retry
    remove(path)
    if (!rename(tmp_path, path)) {
        if (!remove(tmp_path)) {
            to_log(LOG_WARNING, "dasLLAMA image: orphaned tmp '{tmp_path}' could not be removed — clean it up by hand\n")
        }
        return false
    }
    return true
}

// per-writer name: concurrent savers must not clobber each other; last rename wins, same content
def private tmp_name_for(path : string) : string {
    return "{path}.{ref_time_ticks()}.tmp"
}

def private save_image_to_file(var t; path : string; extra : uint64; tag : string; quant : string;
                               plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : bool {
    let tmp_path = tmp_name_for(path)
    let w = build_image(t, tmp_path, false, extra, tag, quant, false, plane)
    return promote_tmp(tmp_path, path, w.ok, w.cur)
}

// a finished chunk to disk, header and all — one append, no patch pass (page 0 is already stored)
def private write_chunk(base : void?; bytes : uint64; path : string) : bool {
    let tmp_path = tmp_name_for(path)
    var w = w_open(tmp_path, bytes)
    let ts = ref_time_ticks()
    w_append(w, base, bytes)
    let ok = w_close(w)
    if (ok) {
        let mb = int64(bytes) >> 20l
        let ms = int64(get_time_usec(ts)) / 1000l
        to_log(LOG_INFO, "dasLLAMA image: chunk written in {ms} ms ({mb} MB at {ms > 0l ? mb * 1000l / ms : 0l} MB/s)\n")
    }
    return promote_tmp(tmp_path, path, ok, bytes)
}

//! Serialize a loaded weight-carrying struct as a prepared image on disk (see build_image).
def save_image(var t; path : string; tag : string = ""; quant : string = "q8") : bool {
    // no plane wants special handling — the walk writes them all from RAM
    return save_image_to_file(t, path, 0ul, tag, quant) $(name : string; var w : ImgWriter; var sections : array<ImgSection>) {
        return false
    }
}

//! Model entry point (the AudioTower one is load_audio_tower's cache in dasllama_audio).
//! `tag` names the flavor (METAL_IMAGE_TAG for blob-only metal-flavor models, "" planar).
def save_model_image(var t : Model; path : string; tag : string = ""; quant : string = "q8") : bool {
    return save_image(t, path, tag, quant)
}

// preallocation for a streamed build: the generic total counts streamed fields as empty (they
// are), so add each streamed plane's exact bytes + a page of padding, plus the pending-wscale
// qscales16 (absent until the epilogue). Over-reserve is free — dwrite_close truncates.
def private stream_extra_bytes(t : Model; jobs : array<FillJob>) : uint64 {
    var extra = 0ul
    for (fname in ["qblob", "k4q", "k5q", "k6q", "q40q", "q51q"]) {
        let sb = stream_plane_bytes(t, jobs, fname)
        if (sb > 0l) {
            extra += uint64(sb) + uint64(IMAGE_PAGE)
        }
    }
    if (t.wscale_f16 && empty(t.qscales16) && !empty(t.qscales)) {
        extra += uint64(long_length(t.qscales) * 2l) + uint64(IMAGE_PAGE)
    }
    return extra
}

//! save_image's STREAMING form (Model-only): planes with recorded FillJobs are transcoded from the
//! gguf mapping straight into the image and never materialize in RAM; empty `jobs` degrades to the
//! eager walk exactly. Call from load_gguf_streaming's block — m/bytes must still be alive.
def save_model_image_streaming(var t : Model; m : GGUFMeta; bytes : array<uint8> | #;
                               var jobs : array<FillJob>; path : string; tag : string = ""; quant : string = "q8") : bool {
    var regs : array<StreamReg>
    stream_collect_regions(t, regs)
    let ok = save_image_to_file(t, path, stream_extra_bytes(t, jobs), tag, quant) $(name : string;
            var w : ImgWriter; var sections : array<ImgSection>) {
        if (stream_plane_bytes(t, jobs, name) <= 0l) {
            return false   // not a streamed plane — the walk writes it from RAM
        }
        w_zeros(w, pad_to_page(w.cur) - w.cur)
        let at = w.cur
        if (!fill_stream_plane(m, bytes, t, name, jobs, regs) $(p : void?; nb : uint64) { w_append(w, p, nb) }) {
            // a failed transcode IS a failed write: kill the sink so the tmp never gets promoted
            to_log(LOG_ERROR, "dasLLAMA image: streamed fill of plane '{name}' failed — image save aborted\n")
            w.ok = false
        }
        sections |> push(ImgSection(name = name, off = at, bytes = w.cur - at))
        return true
    }
    delete regs
    return ok
}

// ===== load =====

// one plane: validate the section against the mapping, then borrow it in place
def private borrow_plane(bp : uint8 const?; msize : uint64; path : string;
                         by_name : table<string; ImgSection>; var ok : bool&;
                         name : string; var field : array<auto(TT)>) {
    if (!key_exists(by_name, name)) {
        return
    }
    let sec = by_name?[name] ?? ImgSection()
    let esz = uint64(typeinfo sizeof(field[0]))
    // bytes-then-offset ordering keeps the bounds math wrap-free (off + bytes could overflow)
    if (sec.off % uint64(IMAGE_PAGE) != 0ul || sec.bytes > msize || sec.off > msize - sec.bytes || sec.bytes % esz != 0ul) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — section '{name}' at {sec.off}+{sec.bytes} (esz {esz}) is malformed\n")
        ok = false
    } else {
        unsafe {
            _builtin_make_temp_array_i64(field, reinterpret<void?>(bp + int64(sec.off)), int64(sec.bytes / esz))
        }
    }
}

//! THE image parse — the only way a prepared image becomes a struct, mapped .dlim or chunk this
//! process just built alike: plane fields borrow `base` in place, the archive blob restores
//! everything else. false = decline (the caller owns `base` and releases it either way).
def private parse_image(base : void?; msize : uint64; path : string; var t; tag : string; quant : string) : bool {
    // the image's planes are packed for the load backend — activate it before any kernel runs
    // (same select the gguf loader does; the identity check below then names that backend)
    select_matmul_backend_for_load()
    var bp = unsafe(reinterpret<uint8 const?>(base))
    if (read_u32(bp, 0l) != IMAGE_MAGIC || read_u32(bp, 4l) != uint(IMAGE_VERSION)
            || read_u32(bp, 8l) != uint(IMAGE_PAGE)) {
        // every decline is loud (short of a missing file) — a silently regenerating cache is
        // undebuggable. A wrong version means the meta layout is unknowable, so this arm
        // cannot read the baked identity string the identity-mismatch arm below prints.
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — size {msize} magic {read_u32(bp, 0l)} version {read_u32(bp, 4l)} (want {IMAGE_VERSION}) page {read_u32(bp, 8l)}\n")
        return false
    }
    let nsections = int(read_u32(bp, 12l))
    let meta_off = read_u64(bp, 16l)
    let meta_bytes = read_u64(bp, 24l)
    let file_bytes = read_u64(bp, 32l)
    // bytes-first ordering keeps meta_off + meta_bytes wrap-free; the INT_MAX cap bounds the
    // int() casts the copy below takes (real meta blobs are a few MB, never empty — the
    // section table always rides there, and addr(meta_blob[0]) needs a nonzero resize)
    if (file_bytes != msize || meta_bytes == 0ul || meta_bytes > msize || meta_off > msize - meta_bytes || meta_bytes > uint64(INT_MAX)) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — header says {file_bytes} bytes / meta {meta_off}+{meta_bytes}, mapped {msize}\n")
        return false
    }
    let want_ident = image_identity(tag, quant)
    if (read_u64(bp, 40l) != hash(want_ident)) {
        // same version ⇒ the meta head is readable: say WHAT the image was baked for, not
        // just that a 64-bit hash missed (bounds were validated above)
        var head : array<uint8>
        head |> resize(int(min(meta_bytes, 65536ul)))
        unsafe {
            memcpy(addr(head[0]), reinterpret<void?>(bp + int64(meta_off)), uint64(long_length(head)))
        }
        var hmeta <- new MemSerializer(head)
        var harch = Archive(reading = true, stream = hmeta)
        var baked_ident = ""
        harch |> serialize(baked_ident)
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — baked for '{hmeta->OK() ? baked_ident : "(unreadable)"}', this run is '{want_ident}'\n")
        delete head
        unsafe {
            delete hmeta
        }
        return false
    }
    // the meta blob is the ONE copied+parsed piece (small); planes stay lazy in the mapping
    var meta_blob : array<uint8>
    meta_blob |> resize(int(meta_bytes))
    unsafe {
        memcpy(addr(meta_blob[0]), reinterpret<void?>(bp + int64(meta_off)), meta_bytes)
    }
    var meta <- new MemSerializer(meta_blob)
    var rarch = Archive(reading = true, stream = meta)
    var baked_ident = ""
    var baked_cfg = ""
    rarch |> serialize(baked_ident)
    rarch |> serialize(baked_cfg)
    var sections : array<ImgSection>
    rarch |> serialize(sections)
    // the header count must agree with the meta blob's own section list — a disagreement
    // means header/meta corruption, and silently-absent sections would leave planes EMPTY
    // (indistinguishable from legitimately-empty ones)
    if (length(sections) != nsections) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — header says {nsections} sections, meta carries {length(sections)}\n")
        delete sections
        delete meta_blob
        unsafe {
            delete meta
        }
        return false
    }
    var by_name <- {for (sec in sections); sec.name => sec}
    // duplicate section names would silently overwrite table entries and borrow the wrong
    // plane — a valid save never produces them, so a collision is corruption
    if (length(by_name) != length(sections)) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — {length(sections)} sections carry duplicate names\n")
        delete by_name
        delete sections
        delete meta_blob
        unsafe {
            delete meta
        }
        return false
    }
    _::serialize_image_meta(rarch, t)
    // a truncated/corrupt meta blob reads back as silent zero-progress (MemSerializer sets
    // lastError, no panic) — decline BEFORE post_load, which would panic on garbage (e.g. an
    // unreadable arch name), and before any plane borrows
    if (!meta->OK()) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — meta blob is truncated/corrupt ({meta->getLastError()})\n")
        delete t   // half-deserialized, all owned — nothing is borrowed yet
        delete meta_blob
        delete sections
        delete by_name
        unsafe {
            delete meta
        }
        return false
    }
    _::image_post_load(t)
    var ok = true
    apply(t) $(name : string; var field) {
        static_if (typeinfo is_array(field)) {
            static_if (!typeinfo is_string(field[0])) {   // string arrays ride the meta blob
                borrow_plane(bp, msize, path, by_name, ok, name, field)
            }
        } static_elif (typeinfo safe_has_field < image_map > (field)) {
            // nested weight-carrier: borrow its planes back from the dotted sections (the
            // nested image_map stays null — the OUTER struct owns this mapping)
            apply(field) $(iname : string; var ifield) {
                static_if (typeinfo is_array(ifield)) {
                    static_if (!typeinfo is_string(ifield[0])) {
                        borrow_plane(bp, msize, path, by_name, ok, "{name}.{iname}", ifield)
                    }
                }
            }
        }
    }
    delete meta_blob
    delete sections
    delete by_name
    unsafe {
        delete meta
    }
    if (!ok) {
        // a malformed section table: forget whatever was borrowed (the finalizer) and leave the
        // backing to the caller — image_map is not set, so the finalizer would not release it
        delete t
        return false
    }
    t.image_map = base
    t.image_bytes = msize
    // address-keyed weight caches (the Metal drivers' resident regions) flush on the weights
    // epoch; this image may sit at a deleted model's recycled addresses, so it counts as a
    // load exactly like load_gguf's own bump does
    bump_weights_epoch()
    return true
}

//! Map a prepared image off disk and parse it: plane fields become borrowed views over the
//! mapping (the finalizer forgets them and unmaps). false = absent / wrong identity / wrong
//! version — the caller regenerates from gguf. Same `_::` contract as save_image.
def load_image(path : string; var t; tag : string = ""; quant : string = "q8") : bool {
    if (!stat(path).is_valid) {
        return false
    }
    var msize = 0ul
    var base : void?
    unsafe {
        base = fmap_open(path, addr(msize))
    }
    if (base == null || int64(msize) < IMAGE_HEADER_BYTES) {
        // fmap_open declines with null (vanished file, mmap failure, >size_t on 32-bit) —
        // treat it like any other decline; the header reads need a real mapping
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — cannot map ({msize} bytes)\n")
        if (base != null) {
            unsafe(fmap_close(base, msize))
        }
        return false
    }
    if (parse_image(base, msize, path, t, tag, quant)) {
        return true
    }
    unsafe(fmap_close(base, msize))
    return false
}

// Parse an image chunk this process just built. On a decline the chunk is freed; on success the
// carrier owns it and its finalizer releases it exactly like a mapping.
def private adopt_image(base : void?; msize : uint64; src : string; var t; tag : string; quant : string) : bool {
    if (parse_image(base, msize, src, t, tag, quant)) {
        return true
    }
    image_chunk_free(base)
    return false
}

// The chunk rail's body: build into a chunk, tee it to `save_to`, adopt it into `out`. Both cold
// entries below are this with a different plane hook.
def private chunk_rail(var t; var out; save_to : string; extra : uint64; tag : string; quant : string;
                       plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : bool {
    let ts = ref_time_ticks()
    var w = build_image(t, "", true, extra, tag, quant, true, plane)
    if (!w.ok) {
        // failing to get the chunk is the only decline that precedes the walk — `t` is whole and
        // the caller can still serve it. Failing after it means the sizing pass and the walk
        // disagree, which is a bug in this file and not a condition to limp on from.
        if (w.mem == null) {
            return false
        }
        image_chunk_free(w.mem)
        panic("dasLLAMA: the image build failed after it began consuming the model — see the error above")
    }
    to_log(LOG_INFO, "dasLLAMA image: built in memory in {get_time_usec(ts) / 1000} ms ({int64(w.cur) >> 20l} MB)\n")
    // the save is a courtesy to the NEXT run — a full disk must not cost this one its model
    if (save_to != "" && !write_chunk(w.mem, w.cur, save_to)) {
        to_log(LOG_WARNING, "dasLLAMA image: could not write '{save_to}' — this run still serves the image it built\n")
    }
    if (!adopt_image(w.mem, w.cur, save_to != "" ? save_to : "<in-memory image>", out, tag, quant)) {
        panic("dasLLAMA: the image this process just built would not parse back — see the decline above")
    }
    return true
}

//! THE cold load: build `t`'s image once into one page-aligned chunk, optionally write it to
//! `save_to`, then serve `out` from that chunk — the same borrowed-plane struct the mapped rail
//! yields. `t`'s planes free as they land, so this never holds two copies; `t` ends up spent.
def image_from_carrier(var t; var out; save_to : string; tag : string = ""; quant : string = "q8") : bool {
    return chunk_rail(t, out, save_to, 0ul, tag, quant) $(name : string; var w : ImgWriter; var sections : array<ImgSection>) {
        return false
    }
}

//! image_from_carrier's STREAMING form: a plane with a recorded FillJob is transcoded from the
//! gguf mapping DIRECTLY into the image and never becomes a model plane — the difference between
//! peaking at one copy of the model and two. Call inside load_gguf_streaming's block.
def image_from_model_streaming(var t : Model; var out : Model; m : GGUFMeta; bytes : array<uint8> | #;
                               var jobs : array<FillJob>; save_to : string;
                               tag : string = ""; quant : string = "q8") : bool {
    var regs : array<StreamReg>
    stream_collect_regions(t, regs)
    let ok = chunk_rail(t, out, save_to, stream_extra_bytes(t, jobs), tag, quant) $(name : string;
            var w : ImgWriter; var sections : array<ImgSection>) {
        if (stream_plane_bytes(t, jobs, name) <= 0l) {
            return false   // not a streamed plane — the walk writes it from RAM
        }
        w_zeros(w, pad_to_page(w.cur) - w.cur)
        let at = w.cur
        if (!fill_stream_plane(m, bytes, t, name, jobs, regs) $(p : void?; nb : uint64) { w_append(w, p, nb) }) {
            to_log(LOG_ERROR, "dasLLAMA image: streamed fill of plane '{name}' failed — image build aborted\n")
            w.ok = false
        }
        sections |> push(ImgSection(name = name, off = at, bytes = w.cur - at))
        return true
    }
    delete regs
    return ok
}

//! Whether a cold build persists its image. Off = build it in memory and serve that, leaving no
//! .dlim behind; the run pays a second copy of the model for it, so on by default.
def image_save_enabled : bool {
    return env_flag("DASLLAMA_IMAGE_SAVE", true)
}

// ===== the metal flavor, derived in the writer =====

// ~36 MB of blob per band: big enough that the interleave amortizes and the writer sees bulk
// appends, small enough that the staging cost does not register against the model.
let private METAL_BAND_BLOCKS = 1048576l

// Write mblob without it ever being a plane: interleave the planar planes band by band into a
// staging buffer the writer consumes before the next band refills it. Runs at QBLOB's position in
// the walk, not mblob's — on the consuming (chunk) rail qblob is freed the moment its field lands.
def private write_metal_blob_plane(var t : Model; var w : ImgWriter; var sections : array<ImgSection>) {
    let sh = metal_blob_shape(t)
    if (sh.blocks <= 0l) {
        return
    }
    let nbytes = uint64(sh.blocks * sh.stride)
    let at = pad_to_page(w.cur)
    w_zeros(w, at - w.cur)
    let ts = ref_time_ticks()
    var band : array<uint8>
    band |> resize_no_init(min(sh.blocks, METAL_BAND_BLOCKS) * sh.stride)
    var b0 = 0l
    while (b0 < sh.blocks && w.ok) {
        let n = min(METAL_BAND_BLOCKS, sh.blocks - b0)
        metal_blob_fill_blocks(t, b0, n, band)
        unsafe {
            w_append(w, addr<void?>(band[0]), uint64(n * sh.stride))
        }
        b0 += n
    }
    delete band
    if (!w.ok) {
        to_log(LOG_ERROR, "dasLLAMA image: plane 'mblob' failed writing {int64(nbytes)} bytes at offset {int64(at)} — disk full? image save aborted\n")
        return
    }
    let ms = int64(get_time_usec(ts)) / 1000l
    let mb = int64(nbytes) >> 20l
    to_log(LOG_INFO, "dasLLAMA image:   plane mblob {mb} MB derived+written in {ms} ms = {ms > 0l ? mb * 1000l / ms : 0l} MB/s\n")
    sections |> push(ImgSection(name = "mblob", off = at, bytes = nbytes))
}

//! The metal flavor's plane hook: the blob layout IS qblob+qscales interleaved, so those three
//! planar planes are not planes of a metal image — mblob stands in for all of them, derived at
//! the first of the three so both sources are still live on the consuming rail.
def private metal_derive_plane(var t : Model; name : string; var w : ImgWriter;
                               var sections : array<ImgSection>) : bool {
    if (name == "qblob") {
        write_metal_blob_plane(t, w, sections)
        return true
    }
    return name == "qscales" || name == "qscales16" || name == "mblob"
}

//! The cold half of a cached load, for any carrier: turn the freshly-loaded `m` into its prepared
//! image and put the image-served struct in `out`. With a target path the FILE is the handoff (`m`
//! is dropped between write and map); without one the image is built in memory. `m` is spent.
def cache_via_image(var m; var out; img : string; tag : string = ""; quant : string = "q8") {
    // no plane wants special handling — the walk writes them all from RAM
    cache_via_image_(m, out, img, tag, quant) $(name : string; var w : ImgWriter; var sections : array<ImgSection>) {
        return false
    }
}

// cache_via_image with the walk's plane hook exposed — the metal flavor derives its blob plane
// from the planar ones here instead of materializing it first (see metal_derive_plane).
def private cache_via_image_(var m; var out; img : string; tag : string; quant : string;
                             plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) {
    if (img == "" || !image_save_enabled()) {
        if (!chunk_rail(m, out, "", 0ul, tag, quant, plane)) {
            out <- m
        }
        return
    }
    let ts = ref_time_ticks()
    if (!save_image_to_file(m, img, 0ul, tag, quant, plane)) {
        to_log(LOG_WARNING, "dasLLAMA: could not write '{img}' — serving the carrier as loaded\n")
        out <- m
        return
    }
    to_log(LOG_INFO, "dasLLAMA: prepared image written in {get_time_usec(ts) / 1000} ms — {img}\n")
    delete m   // before the map, not after: holding both is what makes a cold load peak twice
    if (!load_image(img, out, tag, quant)) {
        panic("dasLLAMA: '{img}' was just written and verified, and will not map back — see the decline above")
    }
}

// interp-load guard: the gguf path's O(model) das loops run ~10x slower interpreted (tinyllama
// load 53s vs 5.5s jitted; repack itself is native tune kernels, 0ms either way) — a 69GB
// hybrid extrapolates to an hour. Prepared images (the slice path) stay interp-friendly.
def private guard_interp_gguf_load(path : string) {
    if (jit_enabled() || is_standalone_exe()) {
        return
    }
    var st : FStat
    if (!stat(path, st) || int64(st.size) <= 4_294_967_296l) {
        return
    }
    if (env_flag("DASLLAMA_ALLOW_INTERP_LOAD", false)) {
        to_log(LOG_WARNING, "dasLLAMA: interpreted gguf load of {int64(st.size) >> 20l} MB — expect minutes per GB (DASLLAMA_ALLOW_INTERP_LOAD)\n")
        return
    }
    panic("dasLLAMA: loading a {int64(st.size) >> 30l} GB gguf without -jit runs the load transforms interpreted (an hour on big models) — run with -jit, load a prepared .dlim, or set DASLLAMA_ALLOW_INTERP_LOAD=1")
}

//! Model entry point — see load_image (`tag` = the flavor, like save_model_image).
def load_model_image(path : string; var t : Model; tag : string = ""; quant : string = "q8") : bool {
    if (!load_image(path, t, tag, quant)) {
        return false
    }
    // AFTER the plane borrows (image_post_load runs before them, when every array is still
    // empty): the GPU MoE tier gathers from the PREPARED planes, so the mapped rail serves it too
    moe_gpu_upload_resident(t)
    return true
}

//! The image-cached model load — ``load_model``'s engine half, public for callers bypassing the facade. ``.dlim`` maps directly (wrong identity panics); q8-mode gguf maps/saves a prepared image (``DASLLAMA_IMAGE=0`` disables); an fp32-mode gguf joins the rail only with ``image_fp32`` (planar flavor — the parity-pinned decoders, e.g. canary's).
//! Under an active Metal mode the image is the BLOB-ONLY metal flavor (CPU inference against it panics) when servable, the planar CPU flavor otherwise; ``planar_only`` pins the planar CPU flavor regardless of armed GPU tiers (the ASR surface — its decode path reads CPU planes).
//! Call inside ``with_job_que()``.
def load_model_cached(path : string; mode : QuantMode = QuantMode.fp32;
                      planar_only : bool = false; image_fp32 : bool = false) : Model {
    let qs = "{mode}"
    if (path |> ends_with(".dlim")) {
        // a prepared image named directly — first-class once the source gguf is gone. EXPECTED
        // flavor first (metal mode > armed vulkan tier > planar) so the normal case loads
        // without the wrong-tag decline warning; a full mismatch panics
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Model()
        let metal_first = !planar_only && get_metal_mode() != MetalMode.off && has_decode_override("metal")
        let vk_cfg = (planar_only || metal_first) ? "" : moe_gpu_bake_tag()
        let vk_tag = vk_cfg   // dlim_vulkan_tag already carries the "vulkan|" prefix ("" = tier absent)
        var loaded = false
        if (vk_tag != "") {
            vulkan_bake_slice_begin()
            loaded = load_model_image(path, t, vk_tag)
            if (loaded) {
                vulkan_bake_slice_end(t)
            } else {
                vulkan_bake_disarm()
            }
        }
        if (!loaded && mode != QuantMode.q8) {
            loaded = load_model_image(path, t, "", qs)   // the named image may be this mode's non-q8 planar bake
        }
        if (!loaded) {
            loaded = (metal_first
                ? load_model_image(path, t, METAL_IMAGE_TAG) || load_model_image(path, t)
                : load_model_image(path, t) || load_model_image(path, t, METAL_IMAGE_TAG))
        }
        if (!loaded) {
            let vk_ident = vk_tag != "" ? " / {image_identity(vk_tag)}" : ""
            panic("dasLLAMA: '{path}' is not a prepared image for this box/knobs (identity {image_identity()} / {image_identity(METAL_IMAGE_TAG)}{vk_ident}) — regenerate it from the source gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared image mapped in {get_time_usec(ts_img) / 1000} ms — {path}\n")
        return <- t
    }
    // load-time flavor pick: blob-only metal when its drivers serve the config; the vulkan
    // flavor (planar + baked GPU planes) when the vulkan tier arms; planar CPU otherwise
    let want_metal = !planar_only && mode == QuantMode.q8 && get_metal_mode() != MetalMode.off && has_decode_override("metal")
    if ((mode == QuantMode.q8 || (mode == QuantMode.fp32 && image_fp32)) && env_flag("DASLLAMA_IMAGE", true)) {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let ts_img = ref_time_ticks()
        var t = Model()
        if (want_metal) {
            let img_m = image_path_for(path, METAL_IMAGE_TAG)
            if (load_model_image(img_m, t, METAL_IMAGE_TAG)) {
                to_log(LOG_INFO, "dasLLAMA: prepared image mapped in {get_time_usec(ts_img) / 1000} ms — {img_m}\n")
                return <- t
            }
        }
        // the vulkan flavor: the GPU walk slices the baked device-layout planes off the mapping
        // instead of re-gathering; ANY vulkan config switch changes the tag and rebakes below
        let vk_cfg = (planar_only || want_metal || mode != QuantMode.q8) ? "" : moe_gpu_bake_tag()
        let vk_tag = vk_cfg   // dlim_vulkan_tag already carries the "vulkan|" prefix ("" = tier absent)
        if (vk_tag != "") {
            let img_v = image_path_for(path, vk_tag)
            vulkan_bake_slice_begin()
            if (load_model_image(img_v, t, vk_tag)) {
                vulkan_bake_slice_end(t)
                to_log(LOG_INFO, "dasLLAMA: prepared vulkan image mapped in {get_time_usec(ts_img) / 1000} ms — {img_v}\n")
                return <- t
            }
            vulkan_bake_disarm()
            if (g_image_frozen) {
                panic("dasLLAMA: no prepared vulkan image for '{path}' ({image_path_for(path, vk_tag)}) and artifacts are frozen — pre-mint it")
            }
            vulkan_bake_begin()   // whatever loads below runs the GPU walk — collect and bake
        }
        let img = image_path_for(path, "", qs)
        if (load_model_image(img, t, "", qs)) {
            if (vk_tag != "" && vulkan_bake_take(t)) {
                // the planar image served this run's uploads; bake the flavor for the next one
                let ts_save = ref_time_ticks()
                let img_v = image_path_for(path, vk_tag)
                if (save_model_image(t, img_v, vk_tag)) {
                    to_log(LOG_INFO, "dasLLAMA: vulkan image baked in {get_time_usec(ts_save) / 1000} ms — {img_v}\n")
                }
                delete t.vkblob   // this run is already uploaded — the bake only serves the NEXT load
                delete t.vkplan
            }
            if (!(want_metal && metal_model_servable(t))) {
                to_log(LOG_INFO, "dasLLAMA: prepared image mapped in {get_time_usec(ts_img) / 1000} ms — {img}\n")
                return <- t
            }
            // a planar image of a metal-servable config under metal mode — drop the mapping
            // and regenerate the blob flavor (the planar file stays for CPU-mode loads)
            if (g_image_frozen) {
                panic("dasLLAMA: '{img}' is planar but this run needs the metal blob flavor, and artifacts are frozen — pre-mint {image_path_for(path, METAL_IMAGE_TAG)}")
            }
            delete t
        }
        if (g_image_frozen) {
            let want = want_metal ? image_path_for(path, METAL_IMAGE_TAG) : img
            panic("dasLLAMA: no prepared image for '{path}' ({want}) and artifacts are frozen — pre-mint it")
        }
        guard_interp_gguf_load(path)
        // the planar flavor streams, so its cold build peaks at ONE copy of the model; the blob
        // flavors cannot (their transforms need the whole plane in RAM) and take the eager rail
        // below, as does any model that will not stream
        if (!want_metal && vk_tag == "") {
            var mt = Model()
            var jobs : array<FillJob>
            var streamed = Model()
            var served = false
            let ts_s = ref_time_ticks()
            let save = image_save_enabled()
            let ran = load_gguf_streaming(path, mode, mt, jobs) $(gm : GGUFMeta; gbytes : array<uint8>) {
                served = (save ? save_model_image_streaming(mt, gm, gbytes, jobs, img, "", qs)
                    : image_from_model_streaming(mt, streamed, gm, gbytes, jobs, "", "", qs))
                return served
            }
            delete jobs
            delete mt   // the streamed planes never became model planes; the rest goes now
            if (served && save) {
                to_log(LOG_INFO, "dasLLAMA: prepared image streamed in {get_time_usec(ts_s) / 1000} ms — {img}\n")
                if (!load_image(img, streamed, "", qs)) {
                    panic("dasLLAMA: '{img}' was just streamed out and will not map back — see the decline above")
                }
                dlim_gc_stale(path)   // the fresh bake proves version-stale siblings dead
            }
            if (served) {
                // the GPU MoE tier gathers from the PREPARED planes, same as the mapped rail
                moe_gpu_upload_resident(streamed)
                return <- streamed
            }
            delete streamed
            if (ran) {
                to_log(LOG_WARNING, "dasLLAMA: the streamed cold build declined for '{path}' — taking the eager rail\n")
            }
        }
        var m <- load_model_(path, mode)
        // COMMIT to the metal flavor, do not build it: the blob plane is the planar planes
        // interleaved, and the writer derives it band by band — materializing it here is what
        // used to make a metal mint peak at two copies of the weights
        let metal_flavor = want_metal && metal_model_servable(m) && metal_blob_commit(m)
        if (!metal_flavor && vk_tag != "" && vulkan_bake_take(m)) {
            // a side artifact for the NEXT run: this one is already uploaded, and the image it
            // goes on to serve must stay planar — so the collected planes go out, then go away
            let ts_vk = ref_time_ticks()
            let img_v = image_path_for(path, vk_tag)
            if (save_model_image(m, img_v, vk_tag)) {
                to_log(LOG_INFO, "dasLLAMA: vulkan image baked in {get_time_usec(ts_vk) / 1000} ms — {img_v}\n")
            }
            delete m.vkblob
            delete m.vkplan
        }
        // and now the load's own flavor, on the one rail: built once in memory, written out for
        // the next run, and served from the very bytes that were written
        var built = Model()
        cache_via_image_(m, built, metal_flavor ? image_path_for(path, METAL_IMAGE_TAG) : img,
            metal_flavor ? METAL_IMAGE_TAG : "", metal_flavor ? "q8" : qs) $(name : string;
                var w : ImgWriter; var sections : array<ImgSection>) {
            return metal_flavor && metal_derive_plane(m, name, w, sections)
        }
        if (metal_flavor && built.image_map == null) {
            // the image declined and handed the carrier back committed but plane-less — finish
            // the flavor in RAM, which is the one state that can still serve this run
            convert_model_to_metal_blob(built)
        }
        delete m
        dlim_gc_stale(path)   // the fresh bake proves version-stale siblings dead
        // the GPU MoE tier gathered from planes the build just freed — re-gather from the image,
        // exactly as the mapped rail does (a no-op when the build declined and `built` IS m)
        moe_gpu_upload_resident(built)
        return <- built
    }
    if (g_image_frozen) {
        panic("dasLLAMA: the image rail is off/ineligible for '{path}' (mode {qs}) and artifacts are frozen — the oracle contract needs a prepared image")
    }
    guard_interp_gguf_load(path)
    var m <- load_model_(path, mode)
    // no image cache: the metal drivers are blob-only, so metal mode still takes the in-RAM form
    if (want_metal && metal_model_servable(m)) {
        convert_model_to_metal_blob(m)
    }
    return <- m
}
