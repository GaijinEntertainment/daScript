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
struct private ImgWriter {
    h : void? = null
    cur : uint64 = 0ul
    ok : bool = true
}

def private w_open(path : string; total : uint64) : ImgWriter {
    let h = unsafe(dwrite_open(path, total, uint64(IMAGE_WRITE_BAND)))
    return ImgWriter(h = h, ok = h != null)
}

def private w_append(var w : ImgWriter; p : void?; nbytes : uint64) {
    if (!w.ok || nbytes == 0ul) {
        return
    }
    if (!unsafe(dwrite_append(w.h, p, nbytes))) {
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
    if (w.h == null) {
        return false
    }
    let flushed = unsafe(dwrite_close(w.h))
    w.h = null
    return flushed && w.ok
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

// every non-array Model field, explicitly, both directions (the archive is bidirectional).
// blocks is NOT serialized — image_post_load re-binds it from the registry after the read.
// The count tripwire forces this list to grow with the struct.
def private serialize_image_meta(var arch : Archive; var t : Model) {
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

// Model's post-read fix-up: blocks re-bind from the registry. Deliberately NOT resolve_arch —
// its configure would clobber serialized gguf overrides with the arch's fallbacks (see
// rebind_arch_blocks).
def private image_post_load(var t : Model) {
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

//! Serialize a loaded weight-carrying struct as a prepared image: array planes page-aligned raw
//! (borrowed back at load), everything else archived. Tmp+rename: atomic on POSIX; on Windows a
//! brief missing-file window lets a concurrent reader regenerate. Caller supplies `serialize_image_meta`/`image_post_load` (`_::` dispatch).
def save_image(var t; path : string; tag : string = ""; quant : string = "q8") : bool {
    // per-writer tmp name: concurrent savers (parallel test workers on one cache miss) must
    // not clobber each other's half-written file; last atomic rename wins with same content
    let tmp_path = "{path}.{ref_time_ticks()}.tmp"
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
    // the writer preallocate the exact file (see image_total_bytes)
    let ts_meta = ref_time_ticks()
    _::serialize_image_meta(warch, t)
    to_log(LOG_INFO, "dasLLAMA image: meta serialize {get_time_usec(ts_meta) / 1000}ms\n")
    var scalar_blob <- meta->extractData()
    unsafe {
        delete meta
    }
    var w = w_open(tmp_path, image_total_bytes(t, uint64(long_length(scalar_blob)) + SECTION_TABLE_SLACK))
    if (w.ok) {
        w_zeros(w, uint64(IMAGE_PAGE))   // page 0: header placeholder — patched after close
        let ts_planes = ref_time_ticks()
        apply(t) $(name : string; var field) {
            static_if (typeinfo is_array(field)) {
                static_if (!typeinfo is_string(field[0])) {   // string arrays ride the meta blob
                    write_plane(w, sections, name, field)
                }
            } static_elif (typeinfo safe_has_field < image_map > (field)) {
                // a nested weight-carrier on the image rail (marker: it declares image_map) —
                // its planes join this image under dotted names; scalars ride the meta blob
                // via the outer type's serialize_image_meta (e.g. WhisperModel.enc)
                apply(field) $(iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        static_if (!typeinfo is_string(ifield[0])) {
                            write_plane(w, sections, "{name}.{iname}", ifield)
                        }
                    }
                }
            }
        }
        let planes_ms = int64(get_time_usec(ts_planes)) / 1000l
        let planes_mb = int64(w.cur) >> 20l
        to_log(LOG_INFO, "dasLLAMA image: planes write {planes_ms}ms ({planes_mb} MB at {planes_ms > 0l ? planes_mb * 1000l / planes_ms : 0l} MB/s)\n")
        // the writer's own split — staging our bytes vs waiting on the device. Anything the two
        // do not account for is time spent OUTSIDE the writer (the walk itself).
        let copy_ms = int64(unsafe(dwrite_stat(w.h, 0))) / 1000000l
        let write_ms = int64(unsafe(dwrite_stat(w.h, 1))) / 1000000l
        let direct_mb = int64(unsafe(dwrite_stat(w.h, 2))) >> 20l
        let bounce_mb = int64(unsafe(dwrite_stat(w.h, 3))) >> 20l
        to_log(LOG_INFO, "dasLLAMA image:   writer split: stage {copy_ms}ms ({bounce_mb} MB bounced), syscall {write_ms}ms ({direct_mb} MB direct), unaccounted {planes_ms - copy_ms - write_ms}ms\n")
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
        // pad the file itself to a page multiple (the whole-file bytesNoCopy wrap needs it)
        fend = pad_to_page(w.cur)
        w_zeros(w, fend - w.cur)
        delete meta_blob
        unsafe {
            delete smeta
        }
        delete sections
    }
    delete scalar_blob
    var ok = w_close(w)
    if (ok) {
        // the header carries counts only the finished walk knows (section count, meta offset and
        // size), and the direct writer is append-only — so patch page 0 through an ordinary
        // handle now that the streaming one is closed
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
        ok = false
        fopen(tmp_path, "r+b") $(hf) {
            if (hf != null) {
                fseek(hf, 0l, seek_set)
                ok = int64(hf |> fwrite(hdr)) == IMAGE_HEADER_BYTES
            }
        }
        if (!ok) {
            to_log(LOG_ERROR, "dasLLAMA image: '{tmp_path}' header patch failed — image save aborted\n")
        }
        delete hdr
    }
    if (ok) {
        // last backstop: the writer reports its own failures, but an ENOSPC that only surfaces
        // at close would still leave a short file — verify what actually landed on disk
        var st : FStat
        if (!stat(tmp_path, st) || st.size != fend) {
            to_log(LOG_ERROR, "dasLLAMA image: '{tmp_path}' is {int64(st.size)} bytes on disk, expected {int64(fend)} — disk full? image save aborted\n")
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
    // rename-over-existing is not portable (Windows) — drop the stale image and retry; the
    // brief missing-file window only costs a concurrent reader a regenerate
    remove(path)
    if (!rename(tmp_path, path)) {
        if (!remove(tmp_path)) {
            to_log(LOG_WARNING, "dasLLAMA image: orphaned tmp '{tmp_path}' could not be removed — clean it up by hand\n")
        }
        return false
    }
    return true
}

//! Model entry point (the AudioTower one is load_audio_tower's cache in dasllama_audio).
//! `tag` names the flavor (METAL_IMAGE_TAG for blob-only metal-flavor models, "" planar).
def save_model_image(var t : Model; path : string; tag : string = ""; quant : string = "q8") : bool {
    return save_image(t, path, tag, quant)
}

//! save_image's STREAMING twin (Model-only): fields with recorded FillJobs stream through
//! fill_stream_plane; empty `jobs` degrades to the eager save exactly. Call from
//! load_gguf_streaming's block (m/bytes must be alive); keep the tails in step with save_image.
def save_model_image_streaming(var t : Model; m : GGUFMeta; bytes : array<uint8> | #;
                               var jobs : array<FillJob>; path : string; tag : string = ""; quant : string = "q8") : bool {
    let tmp_path = "{path}.{ref_time_ticks()}.tmp"
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
    serialize_image_meta(warch, t)
    to_log(LOG_INFO, "dasLLAMA image: meta serialize {get_time_usec(ts_meta) / 1000}ms\n")
    var scalar_blob <- meta->extractData()
    unsafe {
        delete meta
    }
    // preallocation: the generic total (streamed fields are empty, contributing 0) plus each
    // streamed plane's exact bytes + a page of padding, plus the pending-wscale qscales16
    // (absent until the epilogue). Over-reserve is free — dwrite_close truncates.
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
    var regs : array<StreamReg>
    stream_collect_regions(t, regs)
    var fill_ok = true
    var w = w_open(tmp_path, image_total_bytes(t, uint64(long_length(scalar_blob)) + SECTION_TABLE_SLACK) + extra)
    if (w.ok) {
        w_zeros(w, uint64(IMAGE_PAGE))   // page 0: header placeholder — patched after close
        let ts_planes = ref_time_ticks()
        apply(t) $(name : string; var field) {
            static_if (typeinfo is_array(field)) {
                static_if (!typeinfo is_string(field[0])) {
                    if (stream_plane_bytes(t, jobs, name) > 0l) {
                        let padded = pad_to_page(w.cur)
                        w_zeros(w, padded - w.cur)
                        let at = w.cur
                        if (!fill_stream_plane(m, bytes, t, name, jobs, regs) $(p : void?; nb : uint64) { w_append(w, p, nb) }) {
                            fill_ok = false
                        }
                        sections |> push(ImgSection(name = name, off = at, bytes = w.cur - at))
                    } else {
                        write_plane(w, sections, name, field)
                    }
                }
            }
        }
        let planes_ms = int64(get_time_usec(ts_planes)) / 1000l
        let planes_mb = int64(w.cur) >> 20l
        to_log(LOG_INFO, "dasLLAMA image: streamed planes write {planes_ms}ms ({planes_mb} MB at {planes_ms > 0l ? planes_mb * 1000l / planes_ms : 0l} MB/s)\n")
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
        if (meta_bytes > 0ul) {
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
    delete regs
    var ok = w_close(w) && fill_ok
    if (ok) {
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
        ok = false
        fopen(tmp_path, "r+b") $(hf) {
            if (hf != null) {
                fseek(hf, 0l, seek_set)
                ok = int64(hf |> fwrite(hdr)) == IMAGE_HEADER_BYTES
            }
        }
        if (!ok) {
            to_log(LOG_ERROR, "dasLLAMA image: '{tmp_path}' header patch failed — image save aborted\n")
        }
        delete hdr
    }
    if (ok) {
        var st : FStat
        if (!stat(tmp_path, st) || st.size != fend) {
            to_log(LOG_ERROR, "dasLLAMA image: '{tmp_path}' is {int64(st.size)} bytes on disk, expected {int64(fend)} — disk full? image save aborted\n")
            ok = false
        }
    }
    if (!ok) {
        if (stat(tmp_path).is_valid && !remove(tmp_path)) {
            to_log(LOG_WARNING, "dasLLAMA image: orphaned tmp '{tmp_path}' could not be removed — clean it up by hand\n")
        }
        return false
    }
    // POSIX rename-over is atomic; Windows needs remove-then-rename (a concurrent reader may
    // regenerate in the window)
    if (rename(tmp_path, path) || (remove(path) && rename(tmp_path, path))) {
        return true
    }
    to_log(LOG_ERROR, "dasLLAMA image: could not move '{tmp_path}' over '{path}'\n")
    return false
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

//! Map a prepared image and rebuild the struct with zero O(model) work: array plane fields borrow
//! the mapping (finalizer forgets + unmaps), archive blob restores everything else. false = absent /
//! wrong identity / wrong version — caller regenerates from gguf. Same `_::` contract as save_image.
def load_image(path : string; var t; tag : string = ""; quant : string = "q8") : bool {
    if (!stat(path).is_valid) {
        return false
    }
    // the mapped planes are packed for the load backend — activate it before any kernel runs
    // (same select the gguf loader does; the identity check below then names that backend)
    select_matmul_backend_for_load()
    var msize = 0ul
    var base : void?
    unsafe {
        base = fmap_open(path, addr(msize))
    }
    if (base == null || int64(msize) < IMAGE_HEADER_BYTES) {
        // fmap_open declines with null (vanished file, mmap failure, >size_t on 32-bit) —
        // treat it like any other decline; the header reads below need a real mapping
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — cannot map ({msize} bytes)\n")
        if (base != null) {
            unsafe(fmap_close(base, msize))
        }
        return false
    }
    var bp = unsafe(reinterpret<uint8 const?>(base))
    if (read_u32(bp, 0l) != IMAGE_MAGIC || read_u32(bp, 4l) != uint(IMAGE_VERSION)
            || read_u32(bp, 8l) != uint(IMAGE_PAGE)) {
        // every decline is loud (short of a missing file) — a silently regenerating cache is
        // undebuggable. A wrong version means the meta layout is unknowable, so this arm
        // cannot read the baked identity string the identity-mismatch arm below prints.
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — size {msize} magic {read_u32(bp, 0l)} version {read_u32(bp, 4l)} (want {IMAGE_VERSION}) page {read_u32(bp, 8l)}\n")
        unsafe(fmap_close(base, msize))
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
        unsafe(fmap_close(base, msize))
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
        unsafe(fmap_close(base, msize))
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
        unsafe(fmap_close(base, msize))
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
        unsafe(fmap_close(base, msize))
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
        unsafe(fmap_close(base, msize))
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
        // a malformed section table: forget whatever was borrowed (the finalizer), THEN unmap —
        // image_map was never set, so the finalizer alone would leak the mapping
        delete t
        unsafe(fmap_close(base, msize))
        return false
    }
    t.image_map = base
    t.image_bytes = msize
    // address-keyed weight caches (the Metal drivers' resident regions) flush on the weights
    // epoch; this mapping may sit at a deleted model's recycled addresses, so it counts as a
    // load exactly like load_gguf's own bump does
    bump_weights_epoch()
    return true
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
            delete t
        }
        guard_interp_gguf_load(path)
        var m <- load_model_(path, mode)
        let ts_save = ref_time_ticks()
        if (want_metal && metal_model_servable(m) && convert_model_to_metal_blob(m)) {
            let img_m = image_path_for(path, METAL_IMAGE_TAG)
            if (save_model_image(m, img_m, METAL_IMAGE_TAG)) {
                to_log(LOG_INFO, "dasLLAMA: prepared image saved in {get_time_usec(ts_save) / 1000} ms — {img_m}\n")
            }
        } else {
            // planar saves FIRST (vkblob still empty — the planar flavor must stay planar),
            // then the take moves the collected planes in and the vulkan flavor saves
            if (save_model_image(m, img, "", qs)) {
                to_log(LOG_INFO, "dasLLAMA: prepared image saved in {get_time_usec(ts_save) / 1000} ms — {img}\n")
            }
            if (vk_tag != "" && vulkan_bake_take(m)) {
                let ts_vk = ref_time_ticks()
                let img_v = image_path_for(path, vk_tag)
                if (save_model_image(m, img_v, vk_tag)) {
                    to_log(LOG_INFO, "dasLLAMA: vulkan image baked in {get_time_usec(ts_vk) / 1000} ms — {img_v}\n")
                }
                delete m.vkblob
                delete m.vkplan
            }
        }
        dlim_gc_stale(path)   // the fresh bake proves version-stale siblings dead
        return <- m
    }
    guard_interp_gguf_load(path)
    var m <- load_model_(path, mode)
    // no image cache: the metal drivers are blob-only, so metal mode still takes the in-RAM form
    if (want_metal && metal_model_servable(m)) {
        convert_model_to_metal_blob(m)
    }
    return <- m
}
