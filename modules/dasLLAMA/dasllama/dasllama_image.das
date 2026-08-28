options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_image shared public
options _cyclomatic_complexity = 0   // image walk/GC bodies mirror the archive schema
options _function_length = 0   // same

require dasllama/dasllama_lint public
require dasllama/dasllama_ple
require daslib/fio
require dasllama/dasllama_env
require daslib/apply
require strings
require daslib/strings_boost
require dasllama/dasllama_tokenizer
require daslib/archive
require daslib/array_boost
require dasllama/dasllama_plane
require dasllama/dasllama_common
require dasllama/dasllama_gguf
require dasllama/dasllama_layout
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_math
require dasllama/dasllama_config
require math
require dasllama/dasllama_load

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

let IMAGE_VERSION = 14   // 14: the baked dev-W f16 panel plane (devwf16 + site tables)

//! The metal (blob-only) flavor's identity tag: q8 planes ride the 34B block_q8_0 blob and the
//! kq scale planes their GPU forms (convert_model_to_metal_blob) — flavors are per-config and
//! self-sufficient, so the tag keeps the two flavors' files and identities apart.
let METAL_IMAGE_TAG = "metal"
let IMAGE_PAGE = 16384l
let IMAGE_HEADER_BYTES = 64l     // magic..layout fingerprint, inside page 0
let IMAGE_MAGIC = 0x314D4C44u    // 'DLM1'

// public only for the save_image/load_image generics — they instantiate from caller modules
struct ImgSection {
    name : string
    off : uint64      // absolute byte offset
    bytes : uint64
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-identity-backend-order")]
def image_identity(tag : string = ""; quant : string = "q8") : string {
    return dlim_identity(dlim_config_current(quant), IMAGE_VERSION, tag)
}

// a 64-bit identity hash per file: a collision would leave two identities re-saving over one path forever
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

[arch(at = "../ARCHITECTURE_IMAGE.md#dlim-meta-head")]
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
        head |> resize(int64(min(meta_bytes, 65536ul)))
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

//! A prepared image's family, from the baked identity's trailing tag ("" = no family tag — a
//! decoder image, which routes on its gguf instead). Panics on an unreadable image.
def image_family_tag(path : string) : string {
    let p = image_peek(path)
    if (!p.valid || empty(p.identity)) {
        panic("dasLLAMA: '{path}' is not a readable prepared image (see the log above)")
    }
    var parts <- split(p.identity, "|")
    let tag = parts[length(parts) - 1]
    delete parts
    return tag
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

//! The (quant, tag) LANE of an identity — the coexistence unit: flavors and quants coexist as
//! lanes, and a bake can prove staleness only inside its own lane.
def dlim_identity_lane(identity : string) : string {
    var parts <- split(identity, "|")
    if (length(parts) < 8) {
        delete parts
        return identity   // malformed/pre-v7 — its own lane, never cross-matched
    }
    let quant = parts[1]
    var tag = ""
    for (i in range(8, length(parts))) {
        tag = empty(tag) ? parts[i] : "{tag}|{parts[i]}"
    }
    delete parts
    return "{quant}#{tag}"
}

def private dlim_lane_foreign(lane : string) : bool {
    let tag = slice(lane, find(lane, "#") + 1, length(lane))
    if (empty(tag)) {
        return false
    }
    if (tag == METAL_IMAGE_TAG || (tag |> starts_with("vulkan"))) {
        return true
    }
    return !has_value(g_family_tags, tag)
}

def dlim_image_verdict(p : DlimPeek; current : table<string>) : string {
    if (!p.valid) {
        return "BROKEN"
    }
    if (p.version != IMAGE_VERSION) {
        return "STALE v{p.version}"
    }
    if (key_exists(current, p.identity)) {
        return "CURRENT"
    }
    return dlim_lane_foreign(dlim_identity_lane(p.identity)) ? "FOREIGN" : "OTHER"
}

//! Every .dlim beside `gguf_path` (or in a directory), with sizes and verdicts.
def dlim_inventory_(gguf_path : string) : array<DlimImageInfo> {
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

//! GC-after-save: a fresh bake proves dead the non-current siblings of ITS OWN LANE (the
//! (quant, tag) it just wrote), plus BROKEN and version-stale images in any lane. Other lanes'
//! identities fold other bake contexts' backend/winner state — this save proves nothing there.
def dlim_gc_stale(source_path : string; tag : string = ""; quant : string = "q8") {
    let lane = "{quant}#{tag}"
    var inv <- dlim_inventory_(source_path)
    var removed = 0
    var freed = 0l
    for (im in inv) {
        let stale_any = im.verdict == "BROKEN" || (im.verdict |> starts_with("STALE"))
        let lane_dead = ((im.verdict == "OTHER" || im.verdict == "FOREIGN")
            && dlim_identity_lane(im.identity) == lane)
        continue if (!(stale_any || lane_dead))
        if (remove(im.path)) {
            removed++
            freed += im.bytes
            to_log(LOG_INFO, "dasLLAMA image: GC removed {im.file} ({im.bytes >> 20l} MB, {im.verdict})\n")
        }
    }
    delete inv
    if (removed > 0) {
        to_log(LOG_INFO, "dasLLAMA image: GC freed {freed >> 20l} MB - {removed} dead sibling(s) of {base_name(source_path)}\n")
    }
}

//! The dlim GC beside `gguf_path`: everything non-CURRENT goes by default — BROKEN,
//! version-stale, AND valid other-identity siblings (old identities pile up across re-mints).
//! ``keep_other = true`` is the opt-out for a bake deliberately carried from another box.
def dlim_clean_(gguf_path : string; apply : bool; keep_other : bool = false) : tuple<total : int; stale : int; removed : int; freed : int64> {
    var r = (total = 0, stale = 0, removed = 0, freed = 0l)
    var inv <- dlim_inventory_(gguf_path)
    var foreign_kept = 0
    for (im in inv) {
        r.total++
        if (im.verdict == "FOREIGN") {
            foreign_kept++
            continue
        }
        continue if (im.verdict == "CURRENT" || (keep_other && im.verdict == "OTHER"))
        r.stale++
        if (apply && remove(im.path)) {
            r.removed++
            r.freed += im.bytes
            to_log(LOG_INFO, "dasLLAMA image: GC removed {im.file} ({im.bytes >> 20l} MB, {im.verdict})\n")
        }
    }
    delete inv
    if (foreign_kept > 0) {
        to_log(LOG_INFO, "dasLLAMA image: GC kept {foreign_kept} foreign-flavor image(s) - their bake contexts own them\n")
    }
    return r
}

//! The batch-lifecycle sink: delete EVERY image beside `gguf_path` (one model file, or a whole
//! directory), regardless of verdict or lane — for rigs that own the model dirs for a whole
//! batch. A direct dir scan on purpose: the wipe ignores identities, peeking would be waste.
def dlim_wipe(gguf_path : string) : tuple<removed : int; freed : int64> {
    var r = (removed = 0, freed = 0l)
    var st : FStat
    if (!stat(gguf_path, st)) {
        return r
    }
    let dirp = st.is_dir ? gguf_path : dir_name(gguf_path)
    // the trailing dot keeps the reach exact: wiping "foo.gguf" must not take "foo.gguf2"'s images
    let base = st.is_dir ? "" : "{base_name(gguf_path)}."
    dir(dirp) $(name : string) {
        return if (!(name |> ends_with(".dlim")) || (base != "" && !(name |> starts_with(base))))
        let full = path_join(dirp, name)
        let sz = int64(stat(full).size)
        if (remove(full)) {
            r.removed++
            r.freed += sz
        }
    }
    return r
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-page-alignment")]
def pad_to_page(cur : uint64) : uint64 {
    let page = uint64(IMAGE_PAGE)
    return (cur + page - 1ul) / page * page
}

let private DWRITE_STAGING_BAND = 16l * 1024l * 1024l
let private SECTION_TABLE_SLACK = 65536ul

//! The mint sink: append-only and strictly ascending (what keeps NTFS from charging
//! valid-data-length zero-fill on the preallocated file), one walk over two backings (memory
//! chunk / direct file writer). Public so family streamers append planes from their hooks.
struct ImgWriter {
    dwrite_h : void? = null
    chunk_base : void? = null
    chunk_cap : uint64 = 0ul
    cur : uint64 = 0ul
    ok : bool = true
}

def private w_open(path : string; total : uint64) : ImgWriter {
    let h = unsafe(dwrite_open(path, total, uint64(DWRITE_STAGING_BAND)))
    return ImgWriter(dwrite_h = h, ok = h != null)
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-sizing-exactness")]
def private w_open_mem(total : uint64) : ImgWriter {
    let base = image_chunk_alloc(total, uint64(IMAGE_PAGE))
    if (base == null) {
        to_log(LOG_WARNING, "dasLLAMA image: cannot allocate a {int64(total) >> 20l} MB image chunk\n")
    }
    return ImgWriter(chunk_base = base, chunk_cap = total, ok = base != null)
}

def private mem_at(base : void?; off : uint64) : void? {
    let at = unsafe(reinterpret<uint64>(base)) + off
    return unsafe(reinterpret<void?>(at))
}

def w_append(var w : ImgWriter; p : void?; nbytes : uint64) {
    if (!w.ok || nbytes == 0ul) {
        return
    }
    if (w.chunk_base != null) {
        if (nbytes > w.chunk_cap - w.cur) {
            to_log(LOG_ERROR, "dasLLAMA image: chunk overrun at {int64(w.cur)} + {int64(nbytes)} > {int64(w.chunk_cap)} - image sizing is wrong\n")
            w.ok = false
            return
        }
        unsafe {
            memcpy(mem_at(w.chunk_base, w.cur), p, nbytes)
        }
    } elif (!unsafe(dwrite_append(w.dwrite_h, p, nbytes))) {
        w.ok = false   // a dead save stays dead — don't keep pushing bytes at a full disk
        return
    }
    w.cur += nbytes
}

def w_zeros(var w : ImgWriter; n : uint64) {
    if (n == 0ul || !w.ok) {
        return
    }
    var z : array<uint8>
    z |> reserve(int64(n))
    z |> resize(int64(n))
    unsafe {
        w_append(w, addr<void?>(z[0]), n)
    }
    delete z
}

def private w_close(var w : ImgWriter) : bool {
    if (w.chunk_base != null) {
        return w.ok   // nothing to flush: the chunk IS the image
    }
    if (w.dwrite_h == null) {
        return false
    }
    let flushed = unsafe(dwrite_close(w.dwrite_h))
    w.dwrite_h = null
    return flushed && w.ok
}

// the direct writer is append-only, so page 0 is patched through an ordinary handle after close - hence the `path` parameter
def private w_header(var w : ImgWriter; path : string; var hdr : array<uint8>) : bool {
    if (w.chunk_base != null) {
        unsafe {
            memcpy(w.chunk_base, addr<void?>(hdr[0]), uint64(IMAGE_HEADER_BYTES))
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

[arch(at = "../ARCHITECTURE_IMAGE.md#image-sizing-exactness")]
def private image_total_bytes(var t; meta_bytes : uint64) : uint64 {
    var cur = uint64(IMAGE_PAGE)
    apply(t) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            static_if (!typeinfo is_string(field[0])) {
                cur = plane_end(cur, field)
            }
        } static_elif (typeinfo safe_has_field < image_planes > (field)) {
            // a staging plane-group (nested carrier's bytes) — sized exactly like nested planes
            apply(field) $ [unused_argument(iname)] (iname : string; var ifield) {
                static_if (typeinfo is_array(ifield)) {
                    cur = plane_end(cur, ifield)
                }
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



[arch(at = "../ARCHITECTURE_IMAGE.md#image-bulk-serialize")]
def private serialize_pod_array(var arch : Archive; var a : array<auto(TT)>) {
    var n = length(a)
    arch |> serialize_raw(n)
    if (arch.reading) {
        a |> reserve(n)
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
[arch(at = "../ARCHITECTURE_IMAGE.md#image-bulk-serialize")]
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
        blob |> reserve(total)
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
        for (n, s in lens, a) {
            if (n > 0) {
                unsafe {
                    s = string(temp_array(addr<uint8?>(blob[at]), n, type<uint8>))
                }
            } else {
                s = ""
            }
            at += n
        }
    }
    delete lens
    delete blob
}

// derived state (piece -> index, merge -> rank) is never stored: it rebuilds in milliseconds at load
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
        build_spm_derived(tk.spm)
        build_bpe_derived(tk.bpe)
    }
    delete merges
}

//! Model's half of the image contract: every non-array field, both directions, with a count
//! tripwire so the list grows with the struct (blocks re-binds in image_post_load instead).
//! Public only because build/parse resolve `_::` at the instantiation ROOT — not a call-me API.
def serialize_image_meta(var arch : Archive; var t : Model) {
    verify(count_meta_fields(t) == IMAGE_META_FIELDS)
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

// 66 serialized fields + 3 deliberate skips (blocks, image_map, image_bytes)
let IMAGE_META_FIELDS = 66 + 3

//! Count of meta-carried fields of any struct: non-arrays plus string arrays (those cannot
//! be raw planes — see serialize_strings). Pair with a per-type field-count constant to
//! tripwire an explicit serialize_image_meta list against struct growth.
def count_meta_fields(var t) : int {
    var n = 0
    apply(t) $ [unused_argument(name, field)] (name : string; var field) {
        static_if (typeinfo is_array(field)) {
            static_if (typeinfo is_string(field[0])) {
                n++
            }
        } static_elif (!typeinfo safe_has_field < borrowed_plane > (field)) {
            n++   // a borrowed plane is a section, not a meta field
        }
    }
    return n
}

//! The reflected-layout fingerprint the image header pins beside IMAGE_VERSION: every field's
//! name + full type name + byte size in order, recursing into struct fields and array-element
//! structs — any meta-closure shape change refuses by name; IMAGE_VERSION covers behavior.
def layout_fingerprint(var t) : uint64 {
    var d = ""
    layout_describe(t, d)
    return hash(d)
}

def private layout_describe(var t; var d : string&) {
    apply(t) $(name : string; var field) {
        d += "{name}:{typeinfo typename(field)}:{typeinfo sizeof(field)};"
        static_if (typeinfo is_struct(field)) {
            layout_describe(field, d)
            d += "^"   // close the nesting so sibling boundaries stay unambiguous
        } static_elif (typeinfo is_array(field)) {
            layout_describe_elem(field, d)
        }
    }
}

// an array field's ELEMENT struct also serializes element-wise — its shape is load-bearing
def private layout_describe_elem(var a : array<auto(TT)>; var d : string&) {
    static_if (typeinfo is_struct(type<TT>)) {
        var tmp : TT
        layout_describe(tmp, d)
        d += "^"
        delete tmp
    }
}

//! Model's post-read fix-up: blocks re-bind from the registry. Deliberately NOT resolve_arch —
//! its configure would clobber serialized gguf overrides with the arch's fallbacks. Public for
//! the same `_::` reason as serialize_image_meta.
def image_post_load(var t : Model) {
    rebind_arch_blocks(t, t.arch)
}

// long_ forms: a plane past 2 GiB (voxtral-mini qblob = 3.6 G elements) does not fit the int32 rail
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
    // >=256 MB only: a per-plane rate separates systemic cost from stalls, and small models stay quiet
    if (nbytes >= 256ul * 1024ul * 1024ul) {
        let ms = int64(get_time_usec(ts)) / 1000l
        let mb = int64(nbytes) >> 20l
        to_log(LOG_INFO, "dasLLAMA image:   plane {name} {mb} MB in {ms} ms = {ms > 0l ? mb * 1000l / ms : 0l} MB/s\n")
    }
    if (!w.ok) {
        to_log(LOG_ERROR, "dasLLAMA image: plane '{name}' failed writing {int64(nbytes)} bytes at offset {int64(at)} - disk full? image save aborted\n")
        return
    }
    sections |> push(ImgSection(name = name, off = at, bytes = nbytes))
}

//! THE image build — every prepared image in the process comes out of this walk, whichever sink it
//! lands in: planes page-aligned raw (borrowed back at load), everything else archived. `plane`
//! gets first refusal on each plane by name; `consume` frees each owned plane as it lands.
[arch(at = "../ARCHITECTURE_IMAGE.md#image-page-alignment"), arch(at = "../ARCHITECTURE_IMAGE.md#dlim-meta-head"),
    arch(at = "../ARCHITECTURE_IMAGE.md#image-sizing-exactness")]
def private build_image(var t; dst : string; in_mem : bool; extra : uint64;
                        tag : string; quant : string; consume : bool; layout : uint64;
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
            } static_elif (typeinfo safe_has_field < image_planes > (field)) {
                // a staging plane-group: the hook gets the same first refusal here as at the top level
                apply(field) $(iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        if (!invoke(plane, "{name}.{iname}", w, sections)) {
                            write_plane(w, sections, "{name}.{iname}", ifield)
                        }
                        if (consume) {
                            delete ifield
                        }
                    }
                }
            } static_elif (typeinfo safe_has_field < image_map > (field)) {
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
        if (w.dwrite_h != null) {
            // stage vs syscall vs unaccounted: what neither accounts for was spent outside the writer
            let copy_ms = int64(unsafe(dwrite_stat(w.dwrite_h, 0))) / 1000000l
            let write_ms = int64(unsafe(dwrite_stat(w.dwrite_h, 1))) / 1000000l
            let direct_mb = int64(unsafe(dwrite_stat(w.dwrite_h, 2))) >> 20l
            let bounce_mb = int64(unsafe(dwrite_stat(w.dwrite_h, 3))) >> 20l
            to_log(LOG_INFO, "dasLLAMA image:   writer split: stage {copy_ms}ms ({bounce_mb} MB bounced), syscall {write_ms}ms ({direct_mb} MB direct), unaccounted {planes_ms - copy_ms - write_ms}ms\n")
        }
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
        store_u64(hdr, 48, layout)
        w.ok = w_header(w, dst, hdr)
        if (!w.ok) {
            to_log(LOG_ERROR, "dasLLAMA image: '{dst}' header write failed - image build aborted\n")
        }
        delete hdr
    }
    w.cur = fend   // the image's true extent, for whoever maps or adopts it
    return w
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-publish")]
def private promote_tmp(tmp_path : string; path : string; built : bool; want_bytes : uint64) : bool {
    var ok = built
    if (ok) {
        // an ENOSPC that only surfaces at close still leaves a short file — check what landed
        var st : FStat
        if (!stat(tmp_path, st) || st.size != want_bytes) {
            to_log(LOG_ERROR, "dasLLAMA image: '{tmp_path}' is {int64(st.size)} bytes on disk, expected {int64(want_bytes)} - disk full? image save aborted\n")
            ok = false
        }
    }
    if (!ok) {
        // a failed OPEN never created the file, so only warn about a tmp that is actually there
        if (stat(tmp_path).is_valid && !remove(tmp_path)) {
            to_log(LOG_WARNING, "dasLLAMA image: orphaned tmp '{tmp_path}' could not be removed - clean it up by hand\n")
        }
        return false
    }
    if (rename(tmp_path, path)) {
        return true
    }
    remove(path)
    if (!rename(tmp_path, path)) {
        if (!remove(tmp_path)) {
            to_log(LOG_WARNING, "dasLLAMA image: orphaned tmp '{tmp_path}' could not be removed - clean it up by hand\n")
        }
        return false
    }
    return true
}

// per-writer name: concurrent savers must not clobber each other; last rename wins, same content
[arch(at = "../ARCHITECTURE_IMAGE.md#image-publish")]
def private tmp_name_for(path : string) : string {
    return "{path}.{ref_time_ticks()}.tmp"   // clock: control — unique temp-name suffix, not timing
}

def private save_image_to_file(var t; path : string; extra : uint64; tag : string; quant : string; layout : uint64;
                               plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : bool {
    let tmp_path = tmp_name_for(path)
    let w = build_image(t, tmp_path, false, extra, tag, quant, false, layout, plane)
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
    return save_image_to_file(t, path, 0ul, tag, quant, layout_fingerprint(t)) $(name : string; var w : ImgWriter; var sections : array<ImgSection>) {
        return false
    }
}

//! Model entry point (the AudioTower one is load_audio_tower's cache in dasllama_audio).
//! `tag` names the flavor (METAL_IMAGE_TAG for blob-only metal-flavor models, "" planar).
def save_model_image(var t : Model; path : string; tag : string = ""; quant : string = "q8") : bool {
    return save_image(t, path, tag, quant)
}

// streamed planes size as empty, so their bytes plus a page each are added here; over-reserve is free (dwrite_close truncates)
def private stream_extra_bytes(t : Model; jobs : array<FillJob>) : uint64 {
    var extra = 0ul
    for (fname in ["qblob", "mblob", "k4q", "k5q", "k6q", "q40q", "q51q"]) {
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
    //! the baked dev-W panels: tables fill BEFORE the build (they serialize as regular planes),
    //! the panel plane itself streams from the gguf mapping at its field's turn below
    let devw_bytes = t.metal_blob ? devw_bake_tables(t, jobs) : 0l
    let ok = save_image_to_file(t, path, stream_extra_bytes(t, jobs) + uint64(devw_bytes) + (devw_bytes > 0l ? uint64(IMAGE_PAGE) : 0ul), tag, quant, layout_fingerprint(t)) $(name : string;
                                                                                                                                                                               var w : ImgWriter; var sections : array<ImgSection>) {
        if (name == "devwf16" && devw_bytes > 0l) {
            w_zeros(w, pad_to_page(w.cur) - w.cur)
            let at = w.cur
            let ts_dw = ref_time_ticks()
            if (!fill_devwf16_plane(m, bytes, t, jobs) $(p : void?; nb : uint64) { w_append(w, p, nb) }) {
                to_log(LOG_ERROR, "dasLLAMA image: devwf16 bake failed - image save aborted\n")
                w.ok = false
            }
            if (w.ok && w.cur - at != uint64(devw_bytes)) {
                to_log(LOG_ERROR, "dasLLAMA image: devwf16 plane wrote {int64(w.cur - at)} bytes, tables say {devw_bytes} - image save aborted\n")
                w.ok = false
            }
            let dw_ms = int64(get_time_usec(ts_dw)) / 1000l
            to_log(LOG_INFO, "dasLLAMA image:   plane devwf16 {devw_bytes >> 20l} MB baked in {dw_ms} ms ({length(t.devwf16_key)} sites)\n")
            sections |> push(ImgSection(name = "devwf16", off = at, bytes = w.cur - at))
            return true
        }
        let want = stream_plane_bytes(t, jobs, name)
        if (want <= 0l) {
            // a blob build's kq scale strips take their device form as each one's turn comes, before its bytes are written
            if (t.metal_blob) {
                blob_stream_scale_plane(t, name)
            }
            return false   // not a streamed plane — the walk writes it from RAM
        }
        w_zeros(w, pad_to_page(w.cur) - w.cur)
        let at = w.cur
        if (!fill_stream_plane(m, bytes, t, name, jobs, regs) $(p : void?; nb : uint64) { w_append(w, p, nb) }) {
            // a failed transcode IS a failed write: kill the sink so the tmp never gets promoted
            to_log(LOG_ERROR, "dasLLAMA image: streamed fill of plane '{name}' failed - image save aborted\n")
            w.ok = false
        }
        // a plane short of its plan maps back as a silently-truncated borrow — that too is a failed write
        if (w.ok && w.cur - at != uint64(want)) {
            to_log(LOG_ERROR, "dasLLAMA image: streamed plane '{name}' wrote {int64(w.cur - at)} bytes, plan says {want} - image save aborted\n")
            w.ok = false
        }
        sections |> push(ImgSection(name = name, off = at, bytes = w.cur - at))
        return true
    }
    delete regs
    return ok
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-page-alignment")]
def private borrow_plane(bp : uint8 const?; msize : uint64; path : string;
                         by_name : table<string; ImgSection>; var ok : bool&;
                         name : string; var field : array<auto(TT)>) {
    if (!key_exists(by_name, name)) {
        return
    }
    let sec = by_name?[name] ?? ImgSection()
    let esz = uint64(typeinfo sizeof(field[0]))
    if (sec.off % uint64(IMAGE_PAGE) != 0ul || sec.bytes > msize || sec.off > msize - sec.bytes || sec.bytes % esz != 0ul) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - section '{name}' at {sec.off}+{sec.bytes} (esz {esz}) is malformed\n")
        ok = false
    } else {
        unsafe {
            _builtin_make_temp_array_i64(field, reinterpret<void?>(bp + int64(sec.off)), int64(sec.bytes / esz))
        }
    }
}

def private bind_plane_section(bp : uint8 const?; msize : uint64; path : string;
                               by_name : table<string; ImgSection>; var ok : bool&;
                               name : string; var pl) {
    if (!key_exists(by_name, name)) {
        return
    }
    let sec = by_name?[name] ?? ImgSection()
    if (sec.off % uint64(IMAGE_PAGE) != 0ul || sec.bytes > msize || sec.off > msize - sec.bytes) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - section '{name}' at {sec.off}+{sec.bytes} is malformed\n")
        ok = false
        return
    }
    if (!_::plane_bind(pl, bp, int64(sec.off), int64(sec.bytes))) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - section '{name}' is {sec.bytes} bytes, not a whole number of elements\n")
        ok = false
    }
}

//! THE image parse — the only way a prepared image becomes a struct, mapped .dlim or chunk this
//! process just built alike: plane fields borrow `base` in place, the archive blob restores
//! everything else. false = decline (the caller owns `base` and releases it either way).
[arch(at = "../ARCHITECTURE_IMAGE.md#dlim-meta-head"), arch(at = "../ARCHITECTURE_RUNTIME.md#weights-epoch-on-load"),
    arch(at = "../ARCHITECTURE_IMAGE.md#image-identity-backend-order")]
def private parse_image(base : void?; msize : uint64; path : string; var t; tag : string; quant : string) : bool {
    select_matmul_backend_for_load_()
    var bp = unsafe(reinterpret<uint8 const?>(base))
    if (read_u32(bp, 0l) != IMAGE_MAGIC || read_u32(bp, 4l) != uint(IMAGE_VERSION)
            || read_u32(bp, 8l) != uint(IMAGE_PAGE)) {
        // a version mismatch makes the meta layout unknowable, so this arm names raw fields, not the baked identity
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - size {msize} magic {read_u32(bp, 0l)} version {read_u32(bp, 4l)} (want {IMAGE_VERSION}) page {read_u32(bp, 8l)}\n")
        return false
    }
    let nsections = int(read_u32(bp, 12l))
    let meta_off = read_u64(bp, 16l)
    let meta_bytes = read_u64(bp, 24l)
    let file_bytes = read_u64(bp, 32l)
    // the INT_MAX cap bounds the int() casts below; addr(meta_blob[0]) needs a nonzero resize
    if (file_bytes != msize || meta_bytes == 0ul || meta_bytes > msize || meta_off > msize - meta_bytes || meta_bytes > uint64(INT_MAX)) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - header says {file_bytes} bytes / meta {meta_off}+{meta_bytes}, mapped {msize}\n")
        return false
    }
    if (read_u64(bp, 48l) != layout_fingerprint(t)) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - meta layout fingerprint {read_u64(bp, 48l)} vs this build's {layout_fingerprint(t)} (a struct in the meta closure changed shape) - regenerate from the source\n")
        return false
    }
    let want_ident = image_identity(tag, quant)
    if (read_u64(bp, 40l) != hash(want_ident)) {
        var head : array<uint8>
        head |> resize(int(min(meta_bytes, 65536ul)))
        unsafe {
            memcpy(addr(head[0]), reinterpret<void?>(bp + int64(meta_off)), uint64(long_length(head)))
        }
        var hmeta <- new MemSerializer(head)
        var harch = Archive(reading = true, stream = hmeta)
        var baked_ident = ""
        harch |> serialize(baked_ident)
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - baked for '{hmeta->OK() ? baked_ident : "(unreadable)"}', this run is '{want_ident}'\n")
        delete head
        unsafe {
            delete hmeta
        }
        return false
    }
    var meta_blob : array<uint8>
    meta_blob |> reserve(int(meta_bytes))
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
    // silently-absent sections would leave planes EMPTY, indistinguishable from legitimately-empty ones
    if (length(sections) != nsections) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - header says {nsections} sections, meta carries {length(sections)}\n")
        delete sections
        delete meta_blob
        unsafe {
            delete meta
        }
        return false
    }
    var by_name <- {for (sec in sections); sec.name => sec}
    // duplicate names would overwrite table entries and borrow the wrong plane
    if (length(by_name) != length(sections)) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - {length(sections)} sections carry duplicate names\n")
        delete by_name
        delete sections
        delete meta_blob
        unsafe {
            delete meta
        }
        return false
    }
    _::serialize_image_meta(rarch, t)
    // MemSerializer fails silently (lastError, no panic), so decline here - before post_load panics on garbage and before any plane borrows
    if (!meta->OK()) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - meta blob is truncated/corrupt ({meta->getLastError()})\n")
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
        } static_elif (typeinfo safe_has_field < borrowed_plane > (field)) {
            bind_plane_section(bp, msize, path, by_name, ok, name, field)
        } static_elif (typeinfo safe_has_field < image_map > (field)) {
            // the nested image_map stays null: the OUTER struct owns this mapping
            apply(field) $(iname : string; var ifield) {
                static_if (typeinfo is_array(ifield)) {
                    static_if (!typeinfo is_string(ifield[0])) {
                        borrow_plane(bp, msize, path, by_name, ok, "{name}.{iname}", ifield)
                    }
                } static_elif (typeinfo safe_has_field < borrowed_plane > (ifield)) {
                    bind_plane_section(bp, msize, path, by_name, ok, "{name}.{iname}", ifield)
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
        // image_map is not set yet, so the finalizer forgets the borrows and leaves the backing to the caller
        delete t
        return false
    }
    t.image_map = base
    t.image_bytes = msize
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
        // fmap_open declines with null (vanished file, mmap failure, >size_t on 32-bit)
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - cannot map ({msize} bytes)\n")
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

// on a decline the chunk is freed; on success the carrier's finalizer releases it like a mapping
def private adopt_image(base : void?; msize : uint64; src : string; var t; tag : string; quant : string) : bool {
    if (parse_image(base, msize, src, t, tag, quant)) {
        return true
    }
    image_chunk_free(base)
    return false
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-sizing-exactness")]
def private chunk_rail(var t; var out; save_to : string; extra : uint64; tag : string; quant : string;
                       plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : bool {
    let ts = ref_time_ticks()
    var w = build_image(t, "", true, extra, tag, quant, true, layout_fingerprint(out), plane)
    if (!w.ok) {
        if (w.chunk_base == null) {
            return false
        }
        image_chunk_free(w.chunk_base)
        panic("dasLLAMA: the image build failed after it began consuming the model - see the error above")
    }
    to_log(LOG_INFO, "dasLLAMA image: built in memory in {get_time_usec(ts) / 1000} ms ({int64(w.cur) >> 20l} MB)\n")
    if (save_to != "" && !write_chunk(w.chunk_base, w.cur, save_to)) {
        to_log(LOG_WARNING, "dasLLAMA image: could not write '{save_to}' - this run still serves the image it built\n")
    }
    if (!adopt_image(w.chunk_base, w.cur, save_to != "" ? save_to : "<in-memory image>", out, tag, quant)) {
        panic("dasLLAMA: the image this process just built would not parse back - see the decline above")
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
        let want = stream_plane_bytes(t, jobs, name)
        if (want <= 0l) {
            if (t.metal_blob) {   // the kq scale strips take their device form — see the file saver
                blob_stream_scale_plane(t, name)
            }
            return false   // not a streamed plane — the walk writes it from RAM
        }
        w_zeros(w, pad_to_page(w.cur) - w.cur)
        let at = w.cur
        if (!fill_stream_plane(m, bytes, t, name, jobs, regs) $(p : void?; nb : uint64) { w_append(w, p, nb) }) {
            to_log(LOG_ERROR, "dasLLAMA image: streamed fill of plane '{name}' failed - image build aborted\n")
            w.ok = false
        }
        // a plane short of its plan maps back as a silently-truncated borrow — that too is a failed write
        if (w.ok && w.cur - at != uint64(want)) {
            to_log(LOG_ERROR, "dasLLAMA image: streamed plane '{name}' wrote {int64(w.cur - at)} bytes, plan says {want} - image build aborted\n")
            w.ok = false
        }
        sections |> push(ImgSection(name = name, off = at, bytes = w.cur - at))
        return true
    }
    delete regs
    return ok
}

def private vulkan_bake_flavor(var t : Model; path : string; vk_tag : string) {
    if (vk_tag == "" || !vulkan_bake_take(t)) {
        return
    }
    let ts = ref_time_ticks()
    let img_v = image_path_for(path, vk_tag)
    if (save_model_image(t, img_v, vk_tag)) {
        to_log(LOG_INFO, "dasLLAMA: vulkan image baked in {get_time_usec(ts) / 1000} ms - {img_v}\n")
    }
    delete t.vkblob   // this run is already uploaded — the bake only serves the NEXT load
    delete t.vkplan
}

//! Whether a cold build persists its image. Off = build it in memory and serve that, leaving no
//! .dlim behind; the run pays a second copy of the model for it, so on by default.
def image_save_enabled : bool {
    return g_env_engine.image_save
}

// ~36 MB per band: the interleave amortizes and appends stay bulk, with a staging cost that never registers
let private METAL_BAND_BLOCKS = 1048576l

// band by band: the staging buffer holds one band, never the whole blob
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
    band |> reserve(min(sh.blocks, METAL_BAND_BLOCKS) * sh.stride)
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
        to_log(LOG_ERROR, "dasLLAMA image: plane 'mblob' failed writing {int64(nbytes)} bytes at offset {int64(at)} - disk full? image save aborted\n")
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

//! The cold half of a cached load, for any carrier: turn the freshly-loaded `m` into its prepared image and put the image-served struct in `out`.
//! With a target path the FILE is the handoff (`m` is dropped between write and map); without one the image is built in memory. `m` is spent.
//! True only when the FILE was written: only then may the caller reap its lane.
def cache_via_image(var m; var out; img : string; tag : string = ""; quant : string = "q8") : bool {
    return cache_via_image_(m, out, img, tag, quant) $(name : string; var w : ImgWriter; var sections : array<ImgSection>) {
        return false
    }
}

//! cache_via_image for a carrier whose MINT form differs from its SERVED form: `st` carries owned
//! plane arrays, `out` comes back borrowing the image. A staging struct is not servable, so a
//! build that cannot produce an image panics rather than handing anything back.
def cache_via_image_staged(var st; var out; img : string; tag : string = ""; quant : string = "q8") : bool {
    return cache_via_image_staged_hooked(st, out, img, 0ul, tag, quant) $ [unused_argument(name, w, sections)] (name : string;
                                                                                                                var w : ImgWriter; var sections : array<ImgSection>) {
        return false
    }
}

//! cache_via_image_staged with the walk's plane hook exposed plus a pre-sized `extra` for the
//! bytes the hook streams — the streamed mints serve their big planes straight from the source
//! mapping. True only when the FILE was written: only then may the caller reap its lane.
def cache_via_image_staged_hooked(var st; var out; img : string; extra : uint64; tag : string; quant : string;
                                  plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : bool {
    if (img != "" && image_save_enabled()) {
        let ts = ref_time_ticks()
        if (save_image_to_file(st, img, extra, tag, quant, layout_fingerprint(out), plane)) {
            to_log(LOG_INFO, "dasLLAMA: prepared image written in {get_time_usec(ts) / 1000} ms - {img}\n")
            delete st
            if (!load_image(img, out, tag, quant)) {
                panic("dasLLAMA: '{img}' was just written and verified, and will not map back - see the decline above")
            }
            return true
        }
        to_log(LOG_WARNING, "dasLLAMA: could not write '{img}' - building the image in memory instead\n")
    }
    if (!chunk_rail(st, out, "", extra, tag, quant, plane)) {
        panic("dasLLAMA: could not build the prepared image in memory - there is no other form to serve")
    }
    return false
}

def private cache_via_image_(var m; var out; img : string; tag : string; quant : string;
                             plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : bool {
    if (img == "" || !image_save_enabled()) {
        if (!chunk_rail(m, out, "", 0ul, tag, quant, plane)) {
            out <- m   // nolint:PERF030 — `out` is the caller's fresh destination (the cache_via_image contract); m is spent
        }
        return false
    }
    let ts = ref_time_ticks()
    if (!save_image_to_file(m, img, 0ul, tag, quant, layout_fingerprint(out), plane)) {
        to_log(LOG_WARNING, "dasLLAMA: could not write '{img}' - serving the carrier as loaded\n")
        out <- m   // nolint:PERF030 — `out` is the caller's fresh destination (the cache_via_image contract); m is spent
        return false
    }
    to_log(LOG_INFO, "dasLLAMA: prepared image written in {get_time_usec(ts) / 1000} ms - {img}\n")
    delete m
    if (!load_image(img, out, tag, quant)) {
        panic("dasLLAMA: '{img}' was just written and verified, and will not map back - see the decline above")
    }
    return true
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-interp-load")]
def private guard_interp_gguf_load(path : string) {
    if (jit_enabled() || is_standalone_exe()) {
        return
    }
    var st : FStat
    if (!stat(path, st) || int64(st.size) <= 4_294_967_296l) {
        return
    }
    if (g_env_engine.allow_interp_load) {
        to_log(LOG_WARNING, "dasLLAMA: interpreted gguf load of {int64(st.size) >> 20l} MB - expect minutes per GB (DASLLAMA_ALLOW_INTERP_LOAD)\n")
        return
    }
    panic("dasLLAMA: loading a {int64(st.size) >> 30l} GB gguf without -jit runs the load transforms interpreted (an hour on big models) - run with -jit, load a prepared .dlim, or set DASLLAMA_ALLOW_INTERP_LOAD=1")
}

//! Model entry point — see load_image (`tag` = the flavor, like save_model_image).
def load_model_image(path : string; var t : Model; tag : string = ""; quant : string = "q8") : bool {
    if (!load_image(path, t, tag, quant)) {
        return false
    }
    // after the plane borrows: the PLE tripwire and the MoE gather both need bound planes
    ple_check_table(t, "prepared image '{path}'")
    moe_gpu_upload_resident(t)
    return true
}

//! The image-cached model load — ``load_model``'s engine half, public for callers bypassing the facade. ``.dlim`` maps directly (wrong identity panics); q8-mode gguf maps/saves a prepared image (``DASLLAMA_IMAGE=0`` disables); an fp32-mode gguf joins the rail only with ``image_fp32`` (planar flavor — the parity-pinned decoders, e.g. canary's).
//! Under an active Metal mode the image is the BLOB-ONLY metal flavor (CPU inference against it panics) when servable, the planar CPU flavor otherwise; ``planar_only`` pins the planar CPU flavor regardless of armed GPU tiers (the ASR surface — its decode path reads CPU planes).
//! Call inside ``with_job_que()``.
[arch(at = "../ARCHITECTURE_IMAGE.md#image-interp-load"), arch(at = "../ARCHITECTURE_IMAGE.md#image-identity-backend-order")]
def load_model_cached(path : string; mode : QuantMode = QuantMode.fp32;
                      planar_only : bool = false; image_fp32 : bool = false) : Model {
    // under Metal the GPU serves through the override registries, so "no backend registered" would be a false alarm
    if (get_metal_mode() == MetalMode.off) {
        warn_gpu_backend_absent()
    }
    let qs = "{mode}"
    if (path |> ends_with(".dlim")) {
        // expected flavor first (metal > armed vulkan > planar): the normal case loads with no wrong-tag decline warning
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Model()
        let metal_first = !planar_only && get_metal_mode() != MetalMode.off && has_decode_override("metal")
        let vk_tag = (planar_only || metal_first) ? "" : moe_gpu_bake_tag()
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
            panic("dasLLAMA: '{path}' is not a prepared image for this box/knobs (identity {image_identity()} / {image_identity(METAL_IMAGE_TAG)}{vk_ident}) - regenerate it from the source gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared image mapped in {get_time_usec(ts_img) / 1000} ms - {path}\n")
        return <- t
    }
    let want_metal = !planar_only && mode == QuantMode.q8 && get_metal_mode() != MetalMode.off && has_decode_override("metal")
    if ((mode == QuantMode.q8 || (mode == QuantMode.fp32 && image_fp32)) && g_env_engine.image) {
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Model()
        if (want_metal) {
            let img_m = image_path_for(path, METAL_IMAGE_TAG)
            if (load_model_image(img_m, t, METAL_IMAGE_TAG)) {
                to_log(LOG_INFO, "dasLLAMA: prepared image mapped in {get_time_usec(ts_img) / 1000} ms - {img_m}\n")
                return <- t
            }
        }
        // the GPU walk slices baked device-layout planes off the mapping; any vulkan config switch changes the tag and rebakes
        let vk_tag = (planar_only || want_metal || mode != QuantMode.q8) ? "" : moe_gpu_bake_tag()
        if (vk_tag != "") {
            let img_v = image_path_for(path, vk_tag)
            vulkan_bake_slice_begin()
            if (load_model_image(img_v, t, vk_tag)) {
                vulkan_bake_slice_end(t)
                to_log(LOG_INFO, "dasLLAMA: prepared vulkan image mapped in {get_time_usec(ts_img) / 1000} ms - {img_v}\n")
                return <- t
            }
            vulkan_bake_disarm()
            if (g_image_frozen) {
                panic("dasLLAMA: no prepared vulkan image for '{path}' ({image_path_for(path, vk_tag)}) and artifacts are frozen - pre-mint it")
            }
            vulkan_bake_begin()   // whatever loads below runs the GPU walk — collect and bake
        }
        let img = image_path_for(path, "", qs)
        if (load_model_image(img, t, "", qs)) {
            vulkan_bake_flavor(t, path, vk_tag)
            if (!(want_metal && metal_model_servable(t))) {
                to_log(LOG_INFO, "dasLLAMA: prepared image mapped in {get_time_usec(ts_img) / 1000} ms - {img}\n")
                return <- t
            }
            // planar image, metal-servable config: drop it and rebake the blob flavor (the planar file stays for CPU loads)
            if (g_image_frozen) {
                panic("dasLLAMA: '{img}' is planar but this run needs the metal blob flavor, and artifacts are frozen - pre-mint {image_path_for(path, METAL_IMAGE_TAG)}")
            }
            delete t
        }
        if (g_image_frozen) {
            let want = want_metal ? image_path_for(path, METAL_IMAGE_TAG) : img
            panic("dasLLAMA: no prepared image for '{path}' ({want}) and artifacts are frozen - pre-mint it")
        }
        guard_interp_gguf_load(path)
        {
            let s_tag = want_metal ? METAL_IMAGE_TAG : ""
            let s_quant = want_metal ? "q8" : qs
            let s_img = want_metal ? image_path_for(path, METAL_IMAGE_TAG) : img
            var mt = Model()
            var jobs : array<FillJob>
            var streamed = Model()
            var served = false
            let ts_s = ref_time_ticks()
            let save = image_save_enabled()
            let ran = load_gguf_streaming(path, mode, mt, jobs, want_metal) $(gm : GGUFMeta; gbytes : array<uint8>) {
                // a metal run that got the planar answer must not write a planar image under the metal identity
                if (want_metal && !mt.metal_blob) {
                    return false
                }
                served = (save ? save_model_image_streaming(mt, gm, gbytes, jobs, s_img, s_tag, s_quant)
                    : image_from_model_streaming(mt, streamed, gm, gbytes, jobs, "", s_tag, s_quant))
                return served
            }
            delete jobs
            delete mt   // the streamed planes never became model planes; the rest goes now
            if (served && save) {
                to_log(LOG_INFO, "dasLLAMA: prepared image streamed in {get_time_usec(ts_s) / 1000} ms - {s_img}\n")
                if (!load_image(s_img, streamed, s_tag, s_quant)) {
                    panic("dasLLAMA: '{s_img}' was just streamed out and will not map back - see the decline above")
                }
                dlim_gc_stale(path, s_tag, s_quant)
            }
            if (served) {
                // the MoE gather over the PREPARED planes is also what collects the vulkan bake, so the flavor costs no reload
                moe_gpu_upload_resident(streamed)
                vulkan_bake_flavor(streamed, path, vk_tag)
                return <- streamed
            }
            delete streamed
            if (ran) {
                to_log(LOG_WARNING, "dasLLAMA: the streamed cold build declined for '{path}' - taking the eager rail\n")
            }
        }
        var m <- load_model_(path, mode)
        let metal_flavor = want_metal && metal_model_servable(m) && metal_blob_commit(m)
        if (!metal_flavor) {
            // the stream declined, so load_model_'s GPU walk did the collecting; the image it serves stays planar
            vulkan_bake_flavor(m, path, vk_tag)
        }
        var built = Model()
        let wrote = cache_via_image_(m, built, metal_flavor ? image_path_for(path, METAL_IMAGE_TAG) : img,
            metal_flavor ? METAL_IMAGE_TAG : "", metal_flavor ? "q8" : qs) $(name : string;
                                                                             var w : ImgWriter; var sections : array<ImgSection>) {
            return metal_flavor && metal_derive_plane(m, name, w, sections)
        }
        if (metal_flavor && built.image_map == null) {
            // the carrier came back committed but plane-less: finishing the flavor in RAM is the only state that still serves
            convert_model_to_metal_blob(built)
        }
        delete m
        if (wrote) {
            dlim_gc_stale(path, metal_flavor ? METAL_IMAGE_TAG : "", metal_flavor ? "q8" : qs)
        }
        // re-gather from the image: the build freed the planes the MoE tier gathered from (a no-op when it declined)
        moe_gpu_upload_resident(built)
        return <- built
    }
    if (g_image_frozen) {
        panic("dasLLAMA: the image rail is off/ineligible for '{path}' (mode {qs}) and artifacts are frozen - the oracle contract needs a prepared image")
    }
    guard_interp_gguf_load(path)
    var m <- load_model_(path, mode)
    // no image cache: the metal drivers are blob-only, so metal mode still takes the in-RAM form
    if (want_metal && metal_model_servable(m)) {
        convert_model_to_metal_blob(m)
    }
    return <- m
}
