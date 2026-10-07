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
require daslib/enum_trait   // the plane table's sections walk the KqFmt enum
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
require dasllama/dasllama_gemm_schema   // the q8 block geometry the mint's zero stand-in is sized by

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

let IMAGE_VERSION = 45   // Config.pooling and both tokenizers' add_eos flag ride the meta

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
def image_identity(tag : string = ""; quant : string = "q8") : string =>
    image_identity_of(image_config_for(tag, quant), tag)

//! The configuration an image under `tag` is keyed and stored by: the box's current one, or the
//! default one for a family registered config-free (no CPU knob shapes its bytes, so its image
//! is one file per box rather than one per tune state).
def image_config_for(tag : string; quant : string = "q8") : DlimConfiguration {
    if (tag != "" && has_value(g_config_free_tags, tag)) {
        return <- DlimConfiguration(quant = quant)
    }
    return <- dlim_config_current(quant)
}

//! The identity of an explicit configuration — the ONE spelling every identity consumer (the
//! loader, the peek verdicts, the converter's want/have compare, the tests) formats through, so
//! the schema version and the pack-code version can never be folded in two different ways.
def image_identity_of(c : DlimConfiguration; tag : string = "") : string =>
    dlim_identity(c, IMAGE_VERSION, tag, PACK_VERSION)

//! the CPU lane's spelling: the box's tune class where the tune framework is built, the architecture alone otherwise -
//! never the active backend, which is a pin the identity supersedes
def private image_cpu_class() : string {
    static_if (typeinfo builtin_module_exists(llvm)) {
        return tune_cpu_class()
    } else {
        return get_architecture_name()
    }
}

//! a lane keeps [A-Za-z0-9_-] - a tag spelling pipes or spaces (the vulkan plan) never reaches a file name whole
def private lane_sanitize(s : string) : string {
    return build_string() $(var w) {
        for (ch in s) {
            let ok = is_alpha(ch) || is_number(ch) || ch == '_' || ch == '-'
            write_char(w, ok ? ch : '-')
        }
    }
}

//! `with_head`: a split NextN head rides the load, so the trunk-alone and the headed image never share a path
[arch(at = "../ARCHITECTURE_IMAGE.md#image-lane-name")]
def image_lane_name(tag : string = ""; quant : string = "q8"; with_head : bool = false) : string {
    var lane = ""
    if (tag == "") {
        lane = "cpu-{image_cpu_class()}"
        let serve = g_env_engine.iq3s_serve
        if (serve == "lut" || serve == "grid") {
            lane = "{lane}-{serve}"
        }
    } elif (tag |> starts_with("vulkan")) {
        lane = "vulkan"
    } else {
        lane = tag
    }
    // a config-free family's identity carries no rope override, so its lane takes none either
    let rope = (tag != "" && has_value(g_config_free_tags, tag)) ? "" : rope_override_lane()
    if (!empty(rope)) {
        lane = "{lane}-{rope}"
    }
    if (quant != "q8") {
        lane = "{lane}-{quant}"
    }
    if (with_head) {
        lane = "{lane}-mtp"
    }
    return lane_sanitize(lane)
}

//! the file name an image would have carried before lanes named it - the identity hash, the head folded in
def private legacy_image_path(gguf_path : string; tag : string; quant : string) : string {
    let side = mtp_head_sidecar(gguf_path)
    let fold = empty(side) ? "" : "|mtp:{base_name(side)}:{stat(side).size}"
    return "{gguf_path}.{hash(image_identity(tag, quant) + fold)}.dlim"
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-lane-name"), arch(at = "../ARCHITECTURE_IMAGE.md#image-identity-head-fold")]
def image_path_for(gguf_path : string; tag : string = ""; quant : string = "q8") : string {
    let lane = image_lane_name(tag, quant, !empty(mtp_head_sidecar(gguf_path)))
    let path = "{gguf_path}.{lane}.dlim"
    if (!stat(path).is_valid) {
        let old = legacy_image_path(gguf_path, tag, quant)
        if (stat(old).is_valid && rename(old, path)) {
            to_log(LOG_INFO, "dasLLAMA image: adopted {base_name(old)} as {base_name(path)}\n")
        }
    }
    return path
}

//! the lane a `.dlim` file name spells - its last dot-segment - or "" for a hash-named legacy image
def dlim_file_lane(name : string) : string {
    if (!(name |> ends_with(".dlim"))) {
        return ""
    }
    let seg = extension(stem(name))   // ".<lane>"
    return (empty(seg) || (seg |> starts_with(".0x"))) ? "" : slice(seg, 1, length(seg))
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
    layout : uint64       //!< the header's meta-layout fingerprint - what parse_image refuses on
}

//! the leading 64 KiB of the meta blob at `meta_off`, read through `read`; answers whether the reads stayed inside it
[arch(at = "../ARCHITECTURE_IMAGE.md#dlim-meta-head")]
def private read_meta_head(bp : uint8 const?; meta_off, meta_bytes : uint64; read : block<(var arch : Archive) : void>) : bool {
    var head : array<uint8>
    head |> resize(int64(min(meta_bytes, 65536ul)))
    unsafe {
        memcpy(addr(head[0]), reinterpret<void?>(bp + int64(meta_off)), uint64(long_length(head)))
    }
    var hmeta <- new MemSerializer(head)
    var harch = Archive(reading = true, stream = hmeta)
    invoke(read, harch)
    let ok = hmeta->OK()
    delete head
    unsafe {
        delete hmeta
    }
    return ok
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
    p.layout = read_u64(bp, 48l)
    let meta_off = read_u64(bp, 16l)
    let meta_bytes = read_u64(bp, 24l)
    p.valid = (read_u32(bp, 0l) == IMAGE_MAGIC && read_u32(bp, 8l) == uint(IMAGE_PAGE)
        && read_u64(bp, 32l) == msize && meta_bytes != 0ul && meta_bytes <= msize
        && meta_off <= msize - meta_bytes && meta_bytes <= uint64(INT_MAX))
    if (p.valid && p.version >= 7) {
        let read = read_meta_head(bp, meta_off, meta_bytes) $(var harch : Archive) {
            harch |> serialize(p.identity)
            harch |> serialize(p.config_json)
        }
        if (!read) {
            p.identity = ""
            p.config_json = ""
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
    lane : string         //!< the lane the file name spells; "" for a hash-named legacy image
    bytes : int64
    version : int
    verdict : string      //!< CURRENT | OTHER | STALE vN | BROKEN
    identity : string
}

var private g_family_tags : array<string>
var private g_config_free_tags : array<string>
var private g_family_layouts : table<string; function<() : uint64>>

var private g_image_frozen = false

//! Artifact-frozen mode — the perf-oracle contract: ``load_model_cached`` loads ONLY existing
//! prepared images, and any load that would mint/bake a flavor panics instead of falling
//! through to the gguf. Arms via lcpp_bench ``--frozen``.
def set_image_artifacts_frozen(frozen : bool) {
    g_image_frozen = frozen
}

//! Register an image family/flavor tag (call from the family module's [init]) so
//! ``dlim_current_identities`` — and with it every list/GC verdict — covers its images.
def register_image_family_tag(tag : string; config_free : bool = false) {
    if (!has_value(g_family_tags, tag)) {
        g_family_tags |> push(tag)
    }
    if (config_free && !has_value(g_config_free_tags, tag)) {
        g_config_free_tags |> push(tag)
    }
}

//! The same registration with a function answering ``layout_fingerprint`` of the family's
//! carrier, which adds the ``STALE layout`` verdict. A function, not the value: the describe walks
//! the whole carrier, and at [init] that walk runs on every program's init stack.
def register_image_family_tag(tag : string; config_free : bool; layout_of : function<() : uint64>) {
    register_image_family_tag(tag, config_free)
    g_family_layouts[tag] = layout_of
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
    if (length(parts) < DLIM_IDENTITY_FIELDS) {
        delete parts
        return identity   // malformed/pre-v7 — its own lane, never cross-matched
    }
    let quant = parts[1]
    delete parts
    return "{quant}#{dlim_identity_tag(identity)}"
}

//! The flavor/family tag an identity was baked under ("" when it carries none); a tag spelling
//! its own pipes (the vulkan plan) comes back whole.
def dlim_identity_tag(identity : string) : string {
    var parts <- split(identity, "|")
    var tag = ""
    for (i in range(DLIM_IDENTITY_FIELDS, length(parts))) {
        tag = empty(tag) ? parts[i] : "{tag}|{parts[i]}"
    }
    delete parts
    return tag
}

// A registered family's carrier shape is knowable here, so an image whose header pins a
// different one is provably unloadable - parse_image refuses it by fingerprint.
def private dlim_layout_stale(p : DlimPeek) : bool {
    var stale = false
    g_family_layouts |> get(dlim_identity_tag(p.identity)) $(layout_of) {
        stale = p.layout != invoke(layout_of)
    }
    return stale
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
    if (dlim_layout_stale(p)) {
        return "STALE layout"
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
        out |> push(DlimImageInfo(file = name, path = full, lane = dlim_file_lane(name), bytes = p.bytes, version = p.version,
                                  verdict = dlim_image_verdict(p, current), identity = p.identity))
    }
    delete current
    return <- out
}

//! GC-after-save: a fresh bake proves dead the non-current siblings of ITS OWN LANE (the file name's
//! lane; a hash-named legacy image's is the identity's (quant, tag)) plus BROKEN and version-stale
//! images anywhere - other lanes fold other bake contexts' state, and this save proves nothing there
[arch(at = "../ARCHITECTURE_IMAGE.md#image-lane-name")]
def dlim_gc_stale(source_path : string; tag : string = ""; quant : string = "q8") {
    let keep = image_path_for(source_path, tag, quant)
    let lane = dlim_file_lane(base_name(keep))
    let legacy_lane = "{quant}#{tag}"
    var inv <- dlim_inventory_(source_path)
    var removed = 0
    var freed = 0l
    for (im in inv) {
        let stale_any = im.verdict == "BROKEN" || (im.verdict |> starts_with("STALE"))
        let other = im.verdict == "OTHER" || im.verdict == "FOREIGN"
        let same_lane = empty(im.lane) ? dlim_identity_lane(im.identity) == legacy_lane : im.lane == lane
        let lane_dead = other && im.path != keep && same_lane
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

//! The dlim GC beside `gguf_path`: BROKEN, version-stale and hash-named legacy images of another identity go;
//! a lane-named image of another identity is a lane someone switched away from and stays, like a FOREIGN one
//! (``keep_other = true`` spares the legacy others too - a bake deliberately carried from another box)
def dlim_clean_(gguf_path : string; apply : bool; keep_other : bool = false) : tuple<total : int; stale : int; removed : int; freed : int64> {
    var r = (total = 0, stale = 0, removed = 0, freed = 0l)
    var inv <- dlim_inventory_(gguf_path)
    var foreign_kept = 0
    for (im in inv) {
        r.total++
        if (im.verdict == "FOREIGN" || (im.verdict == "OTHER" && !empty(im.lane))) {
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
        to_log(LOG_INFO, "dasLLAMA image: GC kept {foreign_kept} image(s) of other lanes or bake contexts - their own loads judge them\n")
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
def pad_to_page(cur : uint64) : uint64 => round_up(cur, uint64(IMAGE_PAGE))

//! pad the writer to the next page and answer that offset, where the next section starts
def w_page(var w : ImgWriter) : uint64 {
    w_zeros(w, pad_to_page(w.cur) - w.cur)
    return w.cur
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

var private g_image_write_fails_after_bytes = 0ul
var private g_image_write_fault_hits = 0

//! Test seam: a file save that carries an image past `after_bytes` fails as a full disk does; 0 turns it off.
def set_image_write_fault(after_bytes : uint64) {
    g_image_write_fails_after_bytes = after_bytes
}

//! Current value of the [[set_image_write_fault]] seam (0 = off), so a fixture restores what it found.
def get_image_write_fault() : uint64 => g_image_write_fails_after_bytes

//! How many saves the [[set_image_write_fault]] seam has failed in this process.
def image_write_fault_hits() : int => g_image_write_fault_hits

def private image_write_faulted(bytes : uint64) : bool {
    return false if (g_image_write_fails_after_bytes == 0ul || bytes <= g_image_write_fails_after_bytes)
    g_image_write_fault_hits++
    return true
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
        unsafe(memcpy(mem_at(w.chunk_base, w.cur), p, nbytes))
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
    w_append(w, unsafe(addr<void?>(z[0])), n)
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
        } static_elif (typeinfo is_dim(field)) {
            for (el in field) {
                apply(el) $ [unused_argument(iname)] (iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        cur = plane_end(cur, ifield)
                    }
                }
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

def private store_at(var b : array<uint8>; at : int; v : auto(TT)) {
    unsafe {
        var p = addr<TT -const?>(b[at])
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
                s = string(unsafe(temp_array(unsafe(addr<uint8?>(blob[at])), n, type<uint8>)))
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
    verify(count_meta_fields(tk) == TOKENIZER_META_FIELDS)   // grew Tokenizer? extend this list
    verify(count_meta_fields(tk.spm) == SPM_META_FIELDS)   // grew SpmTokenizer? extend this list
    verify(count_meta_fields(tk.bpe) == BPE_META_FIELDS)   // grew BpeTokenizer? extend this list
    arch |> serialize_raw(tk.kind)
    serialize_strings(arch, tk.spm.vocab)
    serialize_pod_array(arch, tk.spm.scores)
    arch |> serialize_raw(tk.spm.max_token_length)
    arch |> serialize_raw(tk.spm.add_space_prefix)
    arch |> serialize_raw(tk.spm.bos_id)
    arch |> serialize_raw(tk.spm.eos_id)
    arch |> serialize_raw(tk.spm.add_eos)
    serialize_strings(arch, tk.bpe.vocab)
    serialize_pod_array(arch, tk.bpe.token_type)
    arch |> serialize(tk.bpe.pre)
    arch |> serialize_raw(tk.bpe.bos_id)
    arch |> serialize_raw(tk.bpe.eos_id)
    arch |> serialize_raw(tk.bpe.spm_space)
    arch |> serialize_raw(tk.bpe.add_bos)
    arch |> serialize_raw(tk.bpe.add_eos)
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
    for (f in type<KqFmt>) {
        if (kq_sb(f)) {
            arch |> serialize_raw(t.kq[int(f)].mr)
        }
    }
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
    arch |> serialize_raw(t.emb_q8)
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
    arch |> serialize_raw(t.mtp_hc_head_norm_off)
    arch |> serialize_raw(t.mtp_hc_head_down_off)
    arch |> serialize_raw(t.mtp_hc_head_up_off)
    arch |> serialize_raw(t.hc_norm_off)
    arch |> serialize_raw(t.hc_inject_off)
    arch |> serialize_raw(t.hc_head_norm_off)
    arch |> serialize_raw(t.hc_head_down_off)
    arch |> serialize_raw(t.hc_head_up_off)
    arch |> serialize_raw(t.ngram_type)
    arch |> serialize_raw(t.ngram_rows)
    arch |> serialize_raw(t.ngram_row_bytes)
    arch |> serialize_raw(t.ple_key_off)
    arch |> serialize_raw(t.ple_val_off)
    arch |> serialize_raw(t.ple_nkey_off)
    arch |> serialize_raw(t.ple_nquery_off)
    arch |> serialize_raw(t.ple_nconv_off)
    arch |> serialize_raw(t.ple_conv_off)
}

//! the 4 deliberate skips: blocks, blob_scaled, image_map, image_bytes; the plane table counts as ONE field.
let IMAGE_META_FIELDS = 81 + 4

// the tokenizer's tripwires: the SentencePiece backend's six serialized meta fields + its three
// derived tables (lookup, pair_merge, bkey_id); the BPE backend's eight (merge_rank rides as the
// merges array) + its three (lookup, pair_merge, cpt_id)
let private TOKENIZER_META_FIELDS = 3
let private SPM_META_FIELDS = 6 + 3
let private BPE_META_FIELDS = 8 + 3

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
        } static_elif (typeinfo is_dim(field)) {
            layout_describe(field[0], d)   // one element's shape stands for the table
            d += "^"
        } static_elif (typeinfo is_array(field)) {
            layout_describe_elem(field, d)
        }
    }
}

// an array field's ELEMENT struct also serializes element-wise — its shape is load-bearing
def private layout_describe_elem(var a : array<auto(TT)>; var d : string&) {
    static_if (typeinfo is_struct(type<TT>)) {
        unsafe {   //! the zeroed value: a nested carrier's initializer chain overflows the init script's stack, and only the type is read
            var tmp : TT
            layout_describe(tmp, d)
            d += "^"
            delete tmp
        }
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
    // >=256 MB only: a per-plane rate separates systemic cost from stalls, and small models stay quiet
    write_section(w, sections, name, nbytes, 256ul * 1024ul * 1024ul, "") $ {
        unsafe {
            w_append(w, addr<void?>(field[0]), nbytes)
        }
    }
}

//! One plane section of `nbytes`, page-aligned, the bytes `fill` appends: a failed write logged and left out of the table,
//! a written one logged with its rate from `rate_from` bytes up (`how` names what produced the bytes) and pushed to `sections`
def private write_section(var w : ImgWriter; var sections : array<ImgSection>; name : string; nbytes, rate_from : uint64; how : string;
                          fill : block<() : void>) {
    let at = w_page(w)
    let ts = ref_time_ticks()
    invoke(fill)
    if (!w.ok) {
        to_log(LOG_ERROR, "dasLLAMA image: plane '{name}' failed writing {int64(nbytes)} bytes at offset {int64(at)} - disk full? image save aborted\n")
        return
    }
    if (nbytes >= rate_from) {
        let ms = int64(get_time_usec(ts)) / 1000l
        let mb = int64(nbytes) >> 20l
        to_log(LOG_INFO, "dasLLAMA image:   plane {name} {mb} MB {how}in {ms} ms = {ms > 0l ? mb * 1000l / ms : 0l} MB/s\n")
    }
    sections |> push(ImgSection(name = name, off = at, bytes = nbytes))
}

//! THE image build — every prepared image in the process comes out of this walk, whichever sink it
//! lands in: planes page-aligned raw (borrowed back at load), everything else archived. `plane`
//! gets first refusal on each plane by name; `consume` frees each owned plane as it lands.
[arch(at = "../ARCHITECTURE_IMAGE.md#image-page-alignment"), arch(at = "../ARCHITECTURE_IMAGE.md#dlim-meta-head"),
    arch(at = "../ARCHITECTURE_IMAGE.md#image-sizing-exactness"),
    arch(at = "../ARCHITECTURE_IMAGE.md#image-kq-table-sb-only"),
    arch(at = "../ARCHITECTURE_ENGINE_FORMATS.md#model-plane-table-dim")]
def private build_image(var t; dst : string; in_mem : bool; extra : uint64;
                        tag : string; quant : string; consume : bool; layout : uint64;
                        plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : ImgWriter {
    // one config snapshot serves the meta embed AND the header hash — they must agree
    let dcfg = image_config_for(tag, quant)
    let ident = image_identity_of(dcfg, tag)
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
        // a failed write ends the walk: nothing after it is written, and nothing is consumed - the model stays whole for the fallback
        apply(t) $(name : string; var field) {
            if (!w.ok) {
                return
            }
            static_if (typeinfo is_array(field)) {
                static_if (!typeinfo is_string(field[0])) {   // string arrays ride the meta blob
                    if (!invoke(plane, name, w, sections) && w.ok) {
                        write_plane(w, sections, name, field)
                    }
                    // lock_count != 0 is a BORROWED view over some other image — never ours to free
                    if (w.ok && consume && lock_count(field) == 0) {
                        delete field
                    }
                }
            } static_elif (typeinfo is_dim(field)) {
                for (el, i in field, count()) {
                    let fmt = unsafe(reinterpret<KqFmt>(uint8(i)))
                    if (!kq_sb(fmt) || !w.ok) {
                        continue
                    }
                    apply(el) $(iname : string; var ifield) {
                        static_if (typeinfo is_array(ifield)) {
                            let pname = kq_plane_name(fmt, iname)
                            if (!invoke(plane, pname, w, sections) && w.ok) {
                                write_plane(w, sections, pname, ifield)
                            }
                            if (w.ok && consume && lock_count(ifield) == 0) {
                                delete ifield
                            }
                        }
                    }
                }
            } static_elif (typeinfo safe_has_field < image_planes > (field)) {
                // a staging plane-group: the hook gets the same first refusal here as at the top level
                apply(field) $(iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        if (!invoke(plane, "{name}.{iname}", w, sections) && w.ok) {
                            write_plane(w, sections, "{name}.{iname}", ifield)
                        }
                        if (w.ok && consume) {
                            delete ifield
                        }
                    }
                }
            } static_elif (typeinfo safe_has_field < image_map > (field)) {
                apply(field) $(iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        static_if (!typeinfo is_string(ifield[0])) {
                            if (w.ok) {
                                write_plane(w, sections, "{name}.{iname}", ifield)
                            }
                            if (w.ok && consume && lock_count(ifield) == 0) {
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
        fend = w_page(w)
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
        store_at(hdr, 0, IMAGE_MAGIC)
        store_at(hdr, 4, uint(IMAGE_VERSION))
        store_at(hdr, 8, uint(IMAGE_PAGE))
        store_at(hdr, 12, uint(nsections))
        store_at(hdr, 16, meta_off)
        store_at(hdr, 24, meta_bytes)
        store_at(hdr, 32, fend)
        store_at(hdr, 40, hash(ident))
        store_at(hdr, 48, layout)
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
    if (!ok) {
        to_log(LOG_ERROR, "dasLLAMA image: the save of '{path}' lost a write or its open - disk full or over quota? the save declines\n")
    } else {
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
def private tmp_name_for(path : string) : string =>
    "{path}.{ref_time_ticks()}{IMAGE_TMP_SUFFIX}"  // clock: control — unique temp-name suffix, not timing

let private IMAGE_TMP_SUFFIX = ".tmp"

let private TMP_STALE_S = 3600l   //! a temp file this long unwritten has no live writer

//! Removes the image temp files beside `stem` - names that start with its file name, carry `.dlim.` and end in the
//! temp suffix - that have gone `min_age_s` seconds unwritten; `stem` is an image path or the gguf every lane's image is named from.
[arch(at = "../ARCHITECTURE_IMAGE.md#image-publish")]
def sweep_image_tmps(stem : string; min_age_s : int64) : tuple<removed : int; freed : int64> {
    let folder = dir_name(stem)
    let base = base_name(stem)
    let now = int64(get_clock())   // clock: control — a temp file's age decides its removal
    var stale : array<string>
    dir(folder) $(name) {
        if (name |> starts_with(base) && name |> ends_with(IMAGE_TMP_SUFFIX) && find(name, ".dlim.") >= 0) {
            let full = path_join(folder, name)
            let st = stat(full)
            if (st.is_valid && !st.is_dir && now - int64(st.mtime) >= min_age_s) {
                stale |> push(full)
            }
        }
    }
    var swept : tuple<removed : int; freed : int64>
    for (full in stale) {
        let bytes = int64(stat(full).size)
        if (remove(full)) {
            swept.removed++
            swept.freed += bytes
            to_log(LOG_WARNING, "dasLLAMA image: removed '{full}' ({bytes >> 20l} MB) - a temp file an earlier save left behind\n")
        } else {
            to_log(LOG_WARNING, "dasLLAMA image: could not remove the stale temp file '{full}' - in use by a live writer?\n")
        }
    }
    return swept
}

def private save_image_to_file(var t; path : string; extra : uint64; tag : string; quant : string; layout : uint64;
                               plane : block<(name : string; var w : ImgWriter; var sections : array<ImgSection>) : bool>) : bool {
    sweep_image_tmps(path, TMP_STALE_S)
    let tmp_path = tmp_name_for(path)
    let w = build_image(t, tmp_path, false, extra, tag, quant, false, layout, plane)
    return promote_tmp(tmp_path, path, w.ok && !image_write_faulted(w.cur), w.cur)
}

// a finished chunk to disk, header and all — one append, no patch pass (page 0 is already stored)
def private write_chunk(base : void?; bytes : uint64; path : string) : bool {
    sweep_image_tmps(path, TMP_STALE_S)
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
def save_model_image(var t : Model; path : string; tag : string = ""; quant : string = "q8") : bool =>
    save_image(t, path, tag, quant)

//! the planes a streaming save sizes as empty and fills from the gguf mapping - the quant halves the loader streams
def private streamed_q_planes() : array<string> {
    var names <- [for (f in type<KqFmt>); kq_plane_name(f, "q"); where kq_sb(f)]
    names |> push("qblob")
    names |> push("mblob")
    names |> push("q51q")
    names |> push("iq4nl32q")
    return <- names
}

// streamed planes size as empty, so their bytes plus a page each are added here; over-reserve is free (dwrite_close truncates)
def private stream_extra_bytes(t : Model; jobs : array<FillJob>) : uint64 {
    var extra = 0ul
    for (fname in streamed_q_planes()) {
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

var private g_vk_mint_walk_writer : ImgWriter?           //! the writer the walk's entries stream into, for the length of one vkblob plane
var private g_vk_mint_meta : GGUFMeta const?
var private g_vk_mint_bytes : uint8 const?
var private g_vk_mint_nbytes = 0l
var private g_vk_mint_streamed_model : Model const?      //! the streamed model: its offsets and flags shape every job's view
var private g_vk_mint_jobs : array<FillJob>
var private g_vk_mint_regs : array<StreamReg>
var private g_vk_mint_job_view = Model()           //! the last span's view - an entry's pieces (a token table's rows) share one transcode
var private g_vk_mint_view_run : array<int>        //! the jobs the view spans, ascending; empty = no view
var private g_vk_mint_view_rebase = 0l
var private g_vk_mint_walked = false
var private g_vk_mint_declined = false             //! the walk declined this load's mint: the planar build serves from memory, no lane is written
var private g_vk_mint_decline_reason = ""          //! the first reason the gather hook could not feed the walk, read after it

def private vk_mint_release() {
    delete g_vk_mint_job_view
    g_vk_mint_view_run |> clear()
    delete g_vk_mint_jobs
    delete g_vk_mint_regs
    g_vk_mint_walk_writer = null
    g_vk_mint_meta = null
    g_vk_mint_bytes = null
    g_vk_mint_streamed_model = null
}

//! the mint's state before a cold load's saves: no walk ran, nothing declined
def private vk_mint_reset() {
    vk_mint_release()
    g_vk_mint_walked = false
    g_vk_mint_declined = false
}

//! whether this save mints through the walk: the tag is the armed tier's bake tag, and no earlier save of this load declined
def private vk_mint_arm_walk(tag : string) : bool {
    g_vk_mint_walked = false
    return !empty(tag) && !g_vk_mint_declined && tag == moe_gpu_bake_tag()
}

//! only an overrun panics - a dead memory-chunk writer; a file writer a lost write killed drops the rest of the walk and the save declines at the publish
def private vk_mint_sink(p : void?; nbytes : uint64) {
    if (!g_vk_mint_walk_writer.ok && g_vk_mint_walk_writer.chunk_base != null) {
        panic("dasLLAMA image: the vulkan lane's device twin overran the image's reserve mid-walk - the sizing is wrong, see the error above")
    }
    if (g_vk_mint_walk_writer.ok && g_vk_mint_walk_writer.chunk_base == null && image_write_faulted(g_vk_mint_walk_writer.cur + nbytes)) {
        g_vk_mint_walk_writer.ok = false
    }
    w_append(*g_vk_mint_walk_writer, p, nbytes)
}

def private same_run(a, b : array<int>) : bool
    => length(a) == length(b) && !empty(a) && a[0] == b[0] && a[length(a) - 1] == b[length(b) - 1]

//! the view over the run of streamed jobs holding `n` elements of `f`'s plane at `woff`, kept while the next entry sits in the
//! same run; false = no such run
def private vk_mint_view_at(f : KqFmt; woff, n : int64) : bool {
    var run <- stream_job_span(*g_vk_mint_streamed_model, g_vk_mint_jobs, f, woff, n)
    if (empty(run)) {
        return false
    }
    if (!same_run(run, g_vk_mint_view_run)) {
        delete g_vk_mint_job_view
        g_vk_mint_job_view = Model()
        g_vk_mint_view_rebase = stream_job_view(*g_vk_mint_meta, unsafe(temp_array(g_vk_mint_bytes, g_vk_mint_nbytes, type<uint8>)),
            *g_vk_mint_streamed_model, g_vk_mint_jobs, run, g_vk_mint_regs, g_vk_mint_job_view)
        delete g_vk_mint_view_run
        g_vk_mint_view_run <- run
    }
    delete run
    return true
}

[arch(at = "../ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint")]
def private vk_mint_gather_hook(fmt : int; woff, n, rows, slice_rows, mr, kgroup, wbias : int64;
                                var wq : array<uint8>; var ws : array<uint8>) : bool {
    let f = kq_fmt_of_slot(fmt)
    if (!vk_mint_view_at(f, woff, n * rows)) {
        if (empty(g_vk_mint_jobs)) {
            return false   // no streaming plan: the planes are whole in RAM, and the layout gather reads them
        }
        // an entry shape the mint does not know: the walk runs to its end over a zero stand-in and the mint declines after it
        if (empty(g_vk_mint_decline_reason)) {
            g_vk_mint_decline_reason = "the mint cannot gather {f} at element {woff} ({n}x{rows}) off the gguf - no run of streamed jobs holds it, an entry shape the mint does not know"
        }
        vk_mint_zero_view(f, woff, n * rows)
    }
    layout_gather_dispatch(g_vk_mint_job_view, f, woff - g_vk_mint_view_rebase, n, rows, slice_rows, mr, kgroup, wbias, wq, ws)
    return true
}

//! a view of zeroed planes standing in for `n` elements of `f`'s plane at `woff`, so a walk the mint cannot feed still runs to its end
def private vk_mint_zero_view(f : KqFmt; woff, n : int64) {
    delete g_vk_mint_job_view
    g_vk_mint_job_view = Model()
    g_vk_mint_view_run |> clear()
    g_vk_mint_view_rebase = woff
    g_vk_mint_job_view.quant = g_vk_mint_streamed_model.quant
    let units = n / kq_elems(f) + 1l
    let paired = kq_plane_pair(g_vk_mint_job_view, f) $(var q : array<uint8>; var s : array<uint8>) {
        q |> resize(units * kq_qsb(f))
        s |> resize(units * kq_ssb(f))
    }
    if (!paired) {
        g_vk_mint_job_view.qblob |> resize(n)
        g_vk_mint_job_view.qscales |> resize(n / Q8_BLOCK_ELEMS + 1l)
    }
}

//! the bound a streamed vulkan lane reserves for its twin, its plan and the packed emb region, since the walk sizes the twin
//! only as it runs: the quant planes' bytes, both halves - a device scale row is its disk row's f16 half or narrower, so the
//! twin never outgrows its planar source - an eighth over for the entry pads, and a window for the plan
[arch(at = "../ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint")]
def private vk_mint_extra_bytes(t : Model; jobs : array<FillJob>) : uint64 {
    var planar = 0l
    if (empty(jobs)) {   // no streaming plan: the quant halves are whole in RAM
        planar += long_length(t.qblob) + long_length(t.q51q) + long_length(t.iq4nl32q) + long_length(t.mxq)
        for (p in t.kq) {
            planar += long_length(p.q)
        }
    } else {
        for (fname in streamed_q_planes()) {
            planar += max(stream_plane_bytes(t, jobs, fname), 0l)
        }
    }
    planar += long_length(t.qscales) * 4l + long_length(t.qscales16) * 2l + long_length(t.q51s) + long_length(t.iq4nl32s) + long_length(t.mxs)
    for (p in t.kq) {
        planar += long_length(p.s)
    }
    let emb = t.planes_trimmed ? trim_emb_bytes(t) : 0l
    return uint64(planar + planar / 8l + emb) + 64ul * 1024ul * 1024ul
}

def private vk_mint_plane(m : GGUFMeta; bytes : array<uint8> | #; var t : Model; name : string; var w : ImgWriter;
                          var sections : array<ImgSection>; jobs : array<FillJob>; regs : array<StreamReg>) : bool {
    if (!g_vk_mint_walked) {   // at the first plane's turn, before any planar family: the plan decides what the families' turns write
        g_vk_mint_walked = true
        vk_mint_walk(m, bytes, t, w, sections, jobs, regs)
    }
    return name == "vkblob" || (t.planes_trimmed && trim_plane_dropped(name))   // written at the walk; dropped: no section, the field maps back empty
}

//! why the trimmed lane cannot be minted off this walk, "" where it can: the plan must sit whole in the driver's arena and the
//! emb region must pack off its jobs, or off the planes the load left whole
def private vk_mint_trim_decline(var t : Model; jobs : array<FillJob>) : string {
    let why = trim_plan_decline(t, moe_gpu_resident_active())
    if (!empty(why)) {
        return why
    }
    if (!(t.emb_q8 || cls_kq(t))) {
        return ""   // an fp32 token table rides fblob, kept: nothing to pack
    }
    let r = trim_emb_region(t)
    if (empty(jobs)) {
        return trim_pack_emb(t, t, 0l) ? "" : "the emb region would not pack"
    }
    if (!vk_mint_view_at(r.f, r.off, r.n)) {
        return "no run of streamed jobs holds the emb region"
    }
    return trim_pack_emb(g_vk_mint_job_view, t, g_vk_mint_view_rebase) ? "" : "the emb region would not pack"
}

[arch(at="../ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint")]
def private vk_mint_walk(m : GGUFMeta; bytes : array<uint8> | #; var t : Model; var w : ImgWriter;
                         var sections : array<ImgSection>; jobs : array<FillJob>; regs : array<StreamReg>) {
    let at = w_page(w)
    let ts = ref_time_ticks()
    unsafe {
        g_vk_mint_walk_writer = addr(w)
        g_vk_mint_meta = addr(m)
        g_vk_mint_bytes = addr(bytes[0])
        g_vk_mint_nbytes = long_length(bytes)
        g_vk_mint_streamed_model = addr(t)
    }
    g_vk_mint_jobs := jobs
    g_vk_mint_regs := regs
    g_vk_mint_view_run |> clear()
    g_vk_mint_decline_reason = ""
    vulkan_mint_begin(@@vk_mint_gather_hook, @@vk_mint_sink)
    moe_gpu_upload_resident(t)
    vulkan_mint_end()
    let took = vulkan_bake_take(t)
    var declined = g_vk_mint_decline_reason
    if (!empty(declined)) {
        // the gather hook's answer stands: the walk ran to its end over a zero stand-in
    } elif (!took) {
        if (t.planes_trimmed) {
            declined = "the GPU walk placed nothing, and a trimmed lane has nothing to serve from"
        } else {
            to_log(LOG_INFO, "dasLLAMA image:   the armed tier placed nothing - the vulkan lane carries no device twin\n")
        }
    } elif (t.planes_trimmed) {
        declined = vk_mint_trim_decline(t, jobs)
    }
    vk_mint_release()
    if (!empty(declined)) {
        to_log(LOG_ERROR, "dasLLAMA image: the vulkan mint declined - {declined}; this save is abandoned and the planar build serves from memory (DASLLAMA_TRIM=0 / set_vulkan_trim(false) mints a persistent lane with the planar families)\n")
        delete t.vkplan   // the plan's bytes went to the abandoned writer: a planar build must carry neither
        delete t.vkblob
        delete t.embq
        delete t.embs
        g_vk_mint_declined = true
        w.ok = false
        return
    }
    if (took) {
        let ms = int64(get_time_usec(ts)) / 1000l
        let mb = int64(w.cur - at) >> 20l
        to_log(LOG_INFO, "dasLLAMA image:   plane vkblob {mb} MB minted off the gguf in {ms} ms ({length(t.vkplan)} entries{t.planes_trimmed ? ", the planar families dropped under DASLLAMA_TRIM / set_vulkan_trim" : ""})\n")
        sections |> push(ImgSection(name = "vkblob", off = at, bytes = w.cur - at))
    }
}

//! Writes the page-aligned section `name` that `fill` streams and pushes it to `sections`. A fill that
//! fails, or writes other than `want` bytes (a short plane maps back as a silently truncated borrow),
//! kills the sink so the tmp never gets promoted; the abort lines name `fail`, `writer` and `plan`, and the sink's `what`
def private w_stream_section(var w : ImgWriter; var sections : array<ImgSection>; name, fail, writer, plan, what : string;
                             want : int64; fill : block<() : bool>) {
    let at = w_page(w)
    if (!invoke(fill)) {
        to_log(LOG_ERROR, "dasLLAMA image: {fail} failed - image {what} aborted\n")
        w.ok = false
    }
    if (w.ok && w.cur - at != uint64(want)) {
        to_log(LOG_ERROR, "dasLLAMA image: {writer} wrote {int64(w.cur - at)} bytes, {plan} {want} - image {what} aborted\n")
        w.ok = false
    }
    sections |> push(ImgSection(name = name, off = at, bytes = w.cur - at))
}

//! what a streaming mint adds to the eager walk: the dev-W panel bytes, the vulkan lane's walk armed, and the bytes past the eager planes
struct private StreamSave {
    devw_bytes : int64
    vk_mint : bool
    extra : uint64
}

//! The two streaming sinks' prologue. The baked dev-W panels' tables fill BEFORE the build (they serialize as
//! regular planes); the panel plane itself streams from the gguf mapping at its field's turn
def private stream_save_prologue(var t : Model; jobs : array<FillJob>; tag : string) : StreamSave {
    let devw_bytes = t.metal_blob ? devw_bake_tables(t, jobs) : 0l
    let vk_mint = vk_mint_arm_walk(tag)
    let vk_extra = vk_mint ? vk_mint_extra_bytes(t, jobs) : 0ul
    let planar_extra = vk_mint && t.planes_trimmed ? 0ul : stream_extra_bytes(t, jobs)   // a trimmed lane writes no streamed family
    return StreamSave(devw_bytes = devw_bytes, vk_mint = vk_mint,
        extra = planar_extra + uint64(devw_bytes) + (devw_bytes > 0l ? uint64(IMAGE_PAGE) : 0ul) + vk_extra)
}

//! One plane's turn in either streaming sink: the vulkan lane's planes, the dev-W panel plane and a streamed
//! plane write here and answer true; false hands the plane to the eager walk. `what` is the sink's verb in
//! its abort lines, `say_devw` logs the panel's bake time
def private stream_plane_turn(m : GGUFMeta; bytes : array<uint8> | #; var t : Model; name : string; var w : ImgWriter;
                              var sections : array<ImgSection>; jobs : array<FillJob>; regs : array<StreamReg>;
                              ss : StreamSave; what : string; say_devw : bool) : bool {
    if (ss.vk_mint && vk_mint_plane(m, bytes, t, name, w, sections, jobs, regs)) {
        return true
    }
    if (name == "devwf16" && ss.devw_bytes > 0l) {
        let ts_dw = ref_time_ticks()
        w_stream_section(w, sections, name, "devwf16 bake", "devwf16 plane", "tables say", what, ss.devw_bytes) $ {
            return fill_devwf16_plane(m, bytes, t, jobs) $(p : void?; nb : uint64) { w_append(w, p, nb) }
        }
        if (say_devw) {
            let dw_ms = int64(get_time_usec(ts_dw)) / 1000l
            to_log(LOG_INFO, "dasLLAMA image:   plane devwf16 {ss.devw_bytes >> 20l} MB baked in {dw_ms} ms ({length(t.devwf16_key)} sites)\n")
        }
        return true
    }
    let want = stream_plane_bytes(t, jobs, name)
    if (want <= 0l) {
        // a blob build's kq scale strips take their device form as each one's turn comes, before its bytes are written;
        // a dead sink leaves the carrier untouched - the in-memory fallback re-fills it with the disk stride
        if (w.ok && t.metal_blob) {
            blob_stream_scale_plane(t, name)
        }
        return false   // not a streamed plane — the walk writes it from RAM
    }
    w_stream_section(w, sections, name, "streamed fill of plane '{name}'", "streamed plane '{name}'", "plan says", what, want) $ {
        return fill_stream_plane(m, bytes, t, name, jobs, regs) $(p : void?; nb : uint64) { w_append(w, p, nb) }
    }
    return true
}

//! save_image's STREAMING form (Model-only): planes with recorded FillJobs are transcoded from the
//! gguf mapping straight into the image and never materialize in RAM; empty `jobs` degrades to the
//! eager walk exactly. Call from load_gguf_streaming's block — m/bytes must still be alive.
def save_model_image_streaming(var t : Model; m : GGUFMeta; bytes : array<uint8> | #;
                               var jobs : array<FillJob>; path : string; tag : string = ""; quant : string = "q8") : bool {
    var regs : array<StreamReg>
    stream_collect_regions(t, regs)
    let ss = stream_save_prologue(t, jobs, tag)
    let ok = save_image_to_file(t, path, ss.extra, tag, quant, layout_fingerprint(t)) $(name : string; var w : ImgWriter; var sections : array<ImgSection>) {
        return stream_plane_turn(m, bytes, t, name, w, sections, jobs, regs, ss, "save", true)
    }
    delete regs
    return ok
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-page-alignment")]
def private borrow_plane(bp : uint8 const?; msize : uint64; path : string;
                         by_name : table<string; ImgSection>; var ok : bool&;
                         name : string; var field : array<auto(TT)>) {
    let esz = uint64(typeinfo sizeof(field[0]))
    let s = image_section(by_name, msize, path, name, esz, " (esz {esz})", ok)
    if (s.found) {
        unsafe {
            _builtin_make_temp_array_i64(field, reinterpret<void?>(bp + int64(s.sec.off)), int64(s.sec.bytes / esz))
        }
    }
}

//! the table's section `name` when it is page-aligned inside the `msize` mapping in whole `esz`-byte elements; a malformed
//! one declines through `ok`, its line carrying `detail`; `found` false for a section the table does not name
def private image_section(by_name : table<string; ImgSection>; msize : uint64; path, name : string; esz : uint64; detail : string;
                          var ok : bool&) : tuple<found : bool; sec : ImgSection> {
    if (!key_exists(by_name, name)) {
        return (found = false, sec = ImgSection())
    }
    let sec = by_name?[name] ?? ImgSection()
    if (sec.off % uint64(IMAGE_PAGE) != 0ul || sec.bytes > msize || sec.off > msize - sec.bytes || sec.bytes % esz != 0ul) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - section '{name}' at {sec.off}+{sec.bytes}{detail} is malformed\n")
        ok = false
        return (found = false, sec = sec)
    }
    return (found = true, sec = sec)
}

def private bind_plane_section(bp : uint8 const?; msize : uint64; path : string;
                               by_name : table<string; ImgSection>; var ok : bool&;
                               name : string; var pl) {
    let s = image_section(by_name, msize, path, name, 1ul, "", ok)
    return if (!s.found)
    let sec = s.sec
    if (!_::plane_bind(pl, bp, int64(sec.off), int64(sec.bytes))) {
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - section '{name}' is {sec.bytes} bytes, not a whole number of elements\n")
        ok = false
    }
}

//! THE image parse — the only way a prepared image becomes a struct, mapped .dlim or chunk this
//! process just built alike: plane fields borrow `base` in place, the archive blob restores
//! everything else. false = decline (the caller owns `base` and releases it either way).
[arch(at = "../ARCHITECTURE_IMAGE.md#dlim-meta-head"), arch(at = "../ARCHITECTURE_RUNTIME.md#weights-epoch-on-load"),
    arch(at = "../ARCHITECTURE_IMAGE.md#image-identity-backend-order"),
    arch(at = "../ARCHITECTURE_IMAGE.md#image-kq-table-sb-only"),
    arch(at = "../ARCHITECTURE_ENGINE_FORMATS.md#model-plane-table-dim")]
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
        var baked_ident = ""
        let read = read_meta_head(bp, meta_off, meta_bytes) $(var harch : Archive) {
            harch |> serialize(baked_ident)
        }
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined - baked for '{read ? baked_ident : "(unreadable)"}', this run is '{want_ident}'\n")
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
        } static_elif (typeinfo is_dim(field)) {
            for (el, i in field, count()) {
                let fmt = unsafe(reinterpret<KqFmt>(uint8(i)))
                if (!kq_sb(fmt)) {
                    continue
                }
                apply(el) $(iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        borrow_plane(bp, msize, path, by_name, ok, kq_plane_name(fmt, iname), ifield)
                    }
                }
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
    if (g_env_engine.prefetch_image) {
        let ts_pf = ref_time_ticks()
        if (unsafe(prefetch_map(base, msize))) {
            to_log(LOG_INFO, "dasLLAMA image: prefetch read {int64(msize >> 20ul)} MB in {get_time_usec(ts_pf) / 1000} ms\n")
        }
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
        if (g_vk_mint_declined) {
            return false   // the mint declined at the first plane's turn, before any plane was consumed: the model is whole
        }
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
    let ss = stream_save_prologue(t, jobs, tag)
    let ok = chunk_rail(t, out, save_to, ss.extra, tag, quant) $(name : string; var w : ImgWriter; var sections : array<ImgSection>) {
        return stream_plane_turn(m, bytes, t, name, w, sections, jobs, regs, ss, "build", false)
    }
    delete regs
    return ok
}

def private vulkan_bake_flavor(var t : Model; path : string; vk_tag : string) {
    if (vk_tag == "" || !vulkan_bake_take(t)) {
        return
    }
    if (!image_save_enabled()) {
        to_log(LOG_INFO, "dasLLAMA: DASLLAMA_IMAGE_SAVE=0 - the vulkan lane this run collected is not written\n")
        delete t.vkblob
        delete t.vkplan
        return
    }
    if (vulkan_trim()) {
        //! the same rule as the streaming mint's: a lane this run cannot serve (the route, a prefill pin or override, the tier's
        //! seams, the card's room) is not written under the trimmed identity, or the next run would serve it untrimmed
        let trim_why = resident_trim_decline(t)
        if (!empty(trim_why) && resident_trim_decline_is_run_mode(trim_why)) {
            to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_TRIM / set_vulkan_trim asked for the trimmed vulkan lane, and this run cannot mint it ({trim_why}) - the lane this run collected from RAM is not written\n")
            delete t.vkblob
            delete t.vkplan
            return
        }
        to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_TRIM / set_vulkan_trim asked for the trimmed vulkan lane, and this arm bakes the lane from RAM with the planar families kept - the lane's identity still folds the trim\n")
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
def image_save_enabled : bool => g_env_engine.image_save

// ~36 MB per band: the interleave amortizes and appends stay bulk, with a staging cost that never registers
let private METAL_BAND_BLOCKS = 1048576l

// band by band: the staging buffer holds one band, never the whole blob
def private write_metal_blob_plane(var t : Model; var w : ImgWriter; var sections : array<ImgSection>) {
    let sh = metal_blob_shape(t)
    if (sh.blocks <= 0l) {
        return
    }
    write_section(w, sections, "mblob", uint64(sh.blocks * sh.stride), 0ul, "derived+written ") $ {
        var band : array<uint8>
        band |> reserve(min(sh.blocks, METAL_BAND_BLOCKS) * sh.stride)
        band |> resize_no_init(min(sh.blocks, METAL_BAND_BLOCKS) * sh.stride)
        var b0 = 0l
        while (b0 < sh.blocks && w.ok) {
            let n = min(METAL_BAND_BLOCKS, sh.blocks - b0)
            metal_blob_fill_blocks(t, b0, n, band)
            w_append(w, unsafe(addr<void?>(band[0])), uint64(n * sh.stride))
            b0 += n
        }
        delete band
    }
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
            map_written_image(st, out, img, tag, quant, ts)
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
    map_written_image(m, out, img, tag, quant, ts)
    return true
}

//! the image just written to `img` from `src`: the write time logged, `src` released, `out` mapped back from the file -
//! a written image that will not map back panics
def private map_written_image(var src; var out; img, tag, quant : string; ts : int64) {
    to_log(LOG_INFO, "dasLLAMA: prepared image written in {get_time_usec(ts) / 1000} ms - {img}\n")
    delete src
    if (!load_image(img, out, tag, quant)) {
        panic("dasLLAMA: '{img}' was just written and verified, and will not map back - see the decline above")
    }
}

[arch(at = "../ARCHITECTURE_IMAGE.md#image-interp-load"), arch(at = "../ARCHITECTURE_IMAGE_VULKAN.md#image-vulkan-mint")]
def private guard_interp_gguf_load(path : string) {
    if (jit_enabled() || is_standalone_exe() || aot_kernels_linked()) {
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
    if (t.planes_trimmed) {
        let why = resident_trim_decline(t)
        if (!empty(why)) {
            vulkan_bake_disarm()
            if (resident_trim_decline_is_run_mode(why)) {
                panic("dasLLAMA: '{path}' is a trimmed vulkan lane, and the whole-model driver would not serve it under this run's knobs ({why}) - a trimmed lane carries no planar families for the per-op rails; lift the knob, or load the source gguf, which serves planar from memory")
            }
            panic("dasLLAMA: '{path}' is a trimmed vulkan lane the whole-model driver no longer serves ({why}) - a trimmed lane carries no planar families for the per-op rails; load the source gguf, which mints the lane again with its planar families")
        }
    }
    // after the plane borrows: the PLE tripwire and the MoE gather both need bound planes
    ple_check_table(t, "prepared image '{path}'")
    moe_gpu_upload_resident(t)
    return true
}

//! The image-cached model load — ``load_model``'s engine half, public for callers bypassing the facade. ``.dlim`` maps directly (wrong identity panics); q8-mode gguf maps/saves a prepared image (``DASLLAMA_IMAGE=0`` disables); an fp32-mode gguf joins the rail only with ``image_fp32`` (planar flavor — the parity-pinned decoders, e.g. canary's).
//! Under an active Metal mode the image is the BLOB-ONLY metal flavor (CPU inference against it panics) when servable, the planar CPU flavor otherwise; ``planar_only`` pins the planar CPU flavor regardless of armed GPU tiers (the ASR surface — its decode path reads CPU planes).
//! Call inside ``with_job_que()``.
[arch(at = "../ARCHITECTURE_IMAGE.md#image-interp-load"), arch(at = "../ARCHITECTURE_IMAGE.md#image-identity-backend-order"), arch(at = "../ARCHITECTURE_IMAGE.md#dlim-meta-head")]
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
            let pk = image_peek(path)
            if (pk.valid && pk.version != IMAGE_VERSION) {
                panic("dasLLAMA: '{path}' was minted at IMAGE_VERSION {pk.version}, this build reads {IMAGE_VERSION} - re-mint it from the source gguf (dasllama-convert --config for the target build)")
            }
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
        var kept_why = ""   //! a trimmed lane this run cannot serve but keeps on disk: the decline is the run's, not the model's
        if (vk_tag != "") {
            let img_v = image_path_for(path, vk_tag)
            vulkan_bake_slice_begin()
            if (load_image(img_v, t, vk_tag)) {
                let trim_why = t.planes_trimmed ? resident_trim_decline(t) : ""
                if (empty(trim_why)) {
                    ple_check_table(t, "prepared image '{img_v}'")
                    moe_gpu_upload_resident(t)
                    vulkan_bake_slice_end(t)
                    to_log(LOG_INFO, "dasLLAMA: prepared vulkan image mapped in {get_time_usec(ts_img) / 1000} ms - {img_v}\n")
                    return <- t
                }
                vulkan_bake_disarm()
                delete t
                //! a run-mode decline (the route, a prefill pin or override, this card's room) keeps the lane for the next run and
                //! serves this one planar from memory; a model-shape decline means the trim rule moved, so the lane is minted again;
                //! frozen artifacts are never deleted
                if (resident_trim_decline_is_run_mode(trim_why)) {
                    kept_why = trim_why
                    to_log(LOG_WARNING, "dasLLAMA: the trimmed vulkan lane '{img_v}' maps, and this run cannot serve it ({trim_why}; DASLLAMA_GPU_CTX_MAX, DASLLAMA_VK_KV32, DASLLAMA_GPU_KV / set_gpu_kv_dtype, DASLLAMA_GPU_RESIDENT / set_gpu_resident_route and the prefill pin or override sit outside the lane's identity) - the lane stays on disk and the streamed build serves planar from memory\n")
                } elif (g_image_frozen) {
                    panic("dasLLAMA: the trimmed vulkan lane '{img_v}' maps, and the whole-model driver no longer serves it ({trim_why}) - artifacts are frozen, so it is kept; mint it again with the artifacts unfrozen")
                } else {
                    to_log(LOG_ERROR, "dasLLAMA: the trimmed vulkan lane '{img_v}' maps, and the whole-model driver no longer serves it ({trim_why}) - the lane is deleted and minted again with its planar families\n")
                    remove(img_v)
                }
            } else {
                vulkan_bake_disarm()
            }
            if (g_image_frozen) {
                panic(empty(kept_why) ? "dasLLAMA: no prepared vulkan image for '{path}' ({image_path_for(path, vk_tag)}) and artifacts are frozen - pre-mint it"
                    : "dasLLAMA: the trimmed vulkan lane '{img_v}' cannot serve this run ({kept_why}) and artifacts are frozen - lift the knob, or run with the artifacts unfrozen")
            }
            vulkan_bake_begin()   // whatever loads below runs the GPU walk — collect and bake
        }
        let img = image_path_for(path, "", qs)
        if (load_model_image(img, t, "", qs)) {
            vulkan_bake_flavor(t, path, vk_tag)
            //! the layout gate must agree, or the rebake below refuses and re-mints the SAME planar
            //! file every load (a repacking backend passes the mint-time shape gate); the mapping's
            //! borrowed planes are not the question - the rebake reads the gguf, never this mapping
            if (!(want_metal && metal_model_servable(t) && metal_blob_form_ok(t))) {
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
            let s_tag = want_metal ? METAL_IMAGE_TAG : vk_tag
            let s_quant = (want_metal || vk_tag != "") ? "q8" : qs
            let s_img = want_metal ? image_path_for(path, METAL_IMAGE_TAG) : (vk_tag != "" ? image_path_for(path, vk_tag) : img)
            var mt = Model()
            var jobs : array<FillJob>
            var streamed = Model()
            var served = false
            var saved = false
            var mint_held = false   //! a trim ask this run cannot mint (a run-mode decline): no lane is written, the streamed build serves from memory
            let ts_s = ref_time_ticks()
            let save = image_save_enabled()
            vk_mint_reset()
            let ran = load_gguf_streaming(path, mode, mt, jobs, want_metal) $(gm : GGUFMeta; gbytes : array<uint8>) {
                // a metal run that got the planar answer must not write a planar image under the metal identity
                if (want_metal && !mt.metal_blob) {
                    return false
                }
                if (vk_tag != "" && vulkan_trim()) {
                    let trim_why = resident_trim_decline(mt)
                    mt.planes_trimmed = empty(trim_why)
                    if (!mt.planes_trimmed) {
                        //! a run-mode decline writes NO lane: an untrimmed lane saved under the trimmed identity would keep serving
                        //! untrimmed once the knob lifts, and the load-time recheck reads trimmed lanes alone; the build stays on
                        //! this arm (the eager rail would cache a planar image, and the RAM bake never trims)
                        if (resident_trim_decline_is_run_mode(trim_why)) {
                            to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_TRIM / set_vulkan_trim asked for the trimmed vulkan lane, and this run cannot mint it ({trim_why}) - no lane is written; the streamed build serves planar from memory\n")
                            mint_held = true
                        } else {
                            to_log(LOG_WARNING, "dasLLAMA: DASLLAMA_TRIM / set_vulkan_trim asked for the trimmed vulkan lane, and the whole-model driver would not take '{path}' whole ({trim_why}) - the lane keeps its planar families\n")
                        }
                    }
                }
                if (save && !mint_held) {
                    saved = save_model_image_streaming(mt, gm, gbytes, jobs, s_img, s_tag, s_quant)
                    if (!saved) {
                        to_log(LOG_WARNING, "dasLLAMA: the image save for '{path}' declined - serving the streamed build from memory\n")
                    }
                }
                if (!saved && g_vk_mint_declined) {
                    mt.planes_trimmed = false   // the declined mint's build serves planar
                }
                served = saved || image_from_model_streaming(mt, streamed, gm, gbytes, jobs, "", s_tag, s_quant)
                if (!served && g_vk_mint_declined) {   // the in-memory mint declined before consuming a plane: build again without it
                    mt.planes_trimmed = false
                    served = image_from_model_streaming(mt, streamed, gm, gbytes, jobs, "", s_tag, s_quant)
                }
                return served
            }
            delete jobs
            if (vk_tag != "") {
                moe_gpu_drop_model_()   // the walk armed the device off `mt`, which goes next
            }
            delete mt   // the streamed planes never became model planes; the rest goes now
            if (saved) {
                to_log(LOG_INFO, "dasLLAMA: prepared image streamed in {get_time_usec(ts_s) / 1000} ms - {s_img}\n")
                if (!load_image(s_img, streamed, s_tag, s_quant)) {
                    panic("dasLLAMA: '{s_img}' was just streamed out and will not map back - see the decline above")
                }
                dlim_gc_stale(path, s_tag, s_quant)
            }
            if (served) {
                if (vk_tag != "" && !empty(streamed.vkplan)) {
                    vulkan_bake_slice_begin()
                    moe_gpu_upload_resident(streamed)
                    vulkan_bake_slice_end(streamed)
                } else {
                    moe_gpu_upload_resident(streamed)
                }
                return <- streamed
            }
            delete streamed
            if (vk_tag != "") {
                vulkan_bake_begin()
            }
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
