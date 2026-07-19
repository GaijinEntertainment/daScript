options gen2
options _comment_hygiene = true

module dasllama_image shared public

require daslib/fio
require daslib/apply
require strings
require dasllama/dasllama_tokenizer   // Tokenizer — the bulk tokenizer serializer below
require daslib/archive
require daslib/array_boost   // nolint:STYLE030 — temp_array is [template]; STYLE030 misses template-fn refs (same note as dasllama_common)
require dasllama/dasllama_common
require dasllama/dasllama_math   // active_kernel_backend — half the image identity

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

// bump on ANY layout/serialization change — a mismatched image regenerates from the gguf
let IMAGE_VERSION = 2
let IMAGE_PAGE = 16384l          // Apple-Silicon page: the bytesNoCopy alignment contract
let IMAGE_HEADER_BYTES = 64l     // magic..identity, inside the first page
let IMAGE_MAGIC = 0x314D4C44u    // 'DLM1'

// public only for the save_image/load_image generics — they instantiate from caller modules
struct ImgSection {
    name : string
    off : uint64      // absolute byte offset, page-aligned
    bytes : uint64
}

// the box + knob identity an image is valid for; folded into filename AND header. Must be computed
// AFTER the same backend select the gguf loader runs at repack time (entry-time default differs).
// `tag` distinguishes image kinds sharing one source path (e.g. "tower-f32" vs "tower-q8").
def image_identity(tag : string = "") : string {
    select_matmul_backend_for_load()
    return ("v{IMAGE_VERSION}|{active_kernel_backend()}|{get_wscale_f16() ? "s16" : "s32"}"
        + "|q8 mr{active_q8_layout_mr()} b{active_q8_wbias()} g{active_q8_kgroup()}"
        + "|kq {active_kq_layout_mr(4)}/{active_kq_layout_mr(5)}/{active_kq_layout_mr(6)}/{active_kq_layout_mr(40)}"
        + (tag != "" ? "|{tag}" : ""))
}

// the image path for (gguf, current identity) — knob combos coexist as separate files
// (the full 64-bit hash keeps that deterministic in practice; the header check would catch
// a collision loudly, but two identities fighting over one path re-save forever)
def image_path_for(gguf_path : string; tag : string = "") : string {
    return "{gguf_path}.{hash(image_identity(tag))}.dlim"
}

def private pad_to_page(cur : uint64) : uint64 {
    let page = uint64(IMAGE_PAGE)
    return (cur + page - 1ul) / page * page
}

def private write_zeros(f : file; n : uint64) {
    if (n == 0ul) {
        return
    }
    var z : array<uint8>
    z |> resize(int(n))
    f |> fwrite(z)
    delete z
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
            merges[int(v)] := k
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
    arch |> serialize_raw(t.experts_mx4)
    arch |> serialize_raw(t.ple_model_bf16)
    arch |> serialize_raw(t.kquant_native)
    arch |> serialize_raw(t.kq_repacked)
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

// the tripwire: serialize_image_meta(Model) covers 63 fields; blocks + image_map +
// image_bytes are deliberate skips. A new non-array Model field trips this until BOTH agree.
let IMAGE_META_FIELDS = 63 + 3

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

// one plane: pad to a page, bulk-write, account. long_ forms: a plane past 2 GiB
// (voxtral-mini qblob = 3.6 G elements) does not fit the int32 length/byte-count rail
def private write_plane(f : file; var sections : array<ImgSection>; var cur : uint64&;
                        var ok : bool&; name : string; var field : array<auto(TT)>) {
    if (empty(field)) {
        return
    }
    let nbytes = uint64(long_length(field)) * uint64(typeinfo sizeof(field[0]))
    let at = pad_to_page(cur)
    write_zeros(f, at - cur)
    f |> long_fwrite(field)
    sections |> push(ImgSection(name = name, off = at, bytes = nbytes))
    cur = at + nbytes
    // fwrite and the section table must agree byte-for-byte, or the mapped planes land at
    // the wrong offsets — fail the save loudly, not the load
    if (uint64(ftell(f)) != cur) {
        to_log(LOG_ERROR, "dasLLAMA image: plane '{name}' wrote {ftell(f)} != accounted {cur} (nbytes {nbytes}) — image save aborted\n")
        ok = false
    }
}

//! Serialize a loaded weight-carrying struct as a prepared image: array planes page-aligned raw
//! (borrowed back at load), everything else archived. Tmp+rename: atomic on POSIX; on Windows a
//! brief missing-file window lets a concurrent reader regenerate. Caller supplies `serialize_image_meta`/`image_post_load` (`_::` dispatch).
def save_image(var t; path : string; tag : string = "") : bool {
    // per-writer tmp name: concurrent savers (parallel test workers on one cache miss) must
    // not clobber each other's half-written file; last atomic rename wins with same content
    let tmp_path = "{path}.{ref_time_ticks()}.tmp"
    var ok = true
    var meta <- new MemSerializer()
    var warch = Archive(reading = false, stream = meta)
    var sections : array<ImgSection>
    fopen(tmp_path, "wb") $(f) {
        if (f == null) {
            ok = false
            return
        }
        // page 0: header placeholder — patched after the walk (fseek back)
        write_zeros(f, uint64(IMAGE_PAGE))
        var cur = uint64(IMAGE_PAGE)
        let ts_meta = ref_time_ticks()
        _::serialize_image_meta(warch, t)
        to_log(LOG_INFO, "dasLLAMA image: meta serialize {get_time_usec(ts_meta) / 1000}ms\n")
        let ts_planes = ref_time_ticks()
        apply(t) $(name : string; var field) {
            static_if (typeinfo is_array(field)) {
                static_if (!typeinfo is_string(field[0])) {   // string arrays ride the meta blob
                    write_plane(f, sections, cur, ok, name, field)
                }
            } static_elif (typeinfo safe_has_field < image_map > (field)) {
                // a nested weight-carrier on the image rail (marker: it declares image_map) —
                // its planes join this image under dotted names; scalars ride the meta blob
                // via the outer type's serialize_image_meta (e.g. WhisperModel.enc)
                apply(field) $(iname : string; var ifield) {
                    static_if (typeinfo is_array(ifield)) {
                        static_if (!typeinfo is_string(ifield[0])) {
                            write_plane(f, sections, cur, ok, "{name}.{iname}", ifield)
                        }
                    }
                }
            }
        }
        to_log(LOG_INFO, "dasLLAMA image: planes write {get_time_usec(ts_planes) / 1000}ms\n")
        // meta = [sections][the walk's scalar stream], one blob at the tail
        var smeta <- new MemSerializer()
        var sarch = Archive(reading = false, stream = smeta)
        sarch |> serialize(sections)
        var meta_blob <- smeta->extractData()
        var scalar_blob <- meta->extractData()
        meta_blob |> push_from(scalar_blob)
        let meta_off = cur
        let meta_bytes = uint64(length(meta_blob))
        f |> fwrite(meta_blob)
        cur += meta_bytes
        // pad the file itself to a page multiple (the whole-file bytesNoCopy wrap needs it)
        let fend = pad_to_page(cur)
        write_zeros(f, fend - cur)
        // patch the header
        var hdr : array<uint8>
        hdr |> resize(int(IMAGE_HEADER_BYTES))
        store_u32(hdr, 0, IMAGE_MAGIC)
        store_u32(hdr, 4, uint(IMAGE_VERSION))
        store_u32(hdr, 8, uint(IMAGE_PAGE))
        store_u32(hdr, 12, uint(length(sections)))
        store_u64(hdr, 16, meta_off)
        store_u64(hdr, 24, meta_bytes)
        store_u64(hdr, 32, fend)
        store_u64(hdr, 40, hash(image_identity(tag)))
        fseek(f, 0l, seek_set)
        f |> fwrite(hdr)
        delete hdr
        delete meta_blob
        delete scalar_blob
        unsafe {
            delete smeta
        }
        delete sections
    }
    unsafe {
        delete meta
    }
    if (!ok) {
        if (!remove(tmp_path)) {
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
def save_model_image(var t : Model; path : string) : bool {
    return save_image(t, path)
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
def load_image(path : string; var t; tag : string = "") : bool {
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
    if ((read_u32(bp, 0l) != IMAGE_MAGIC || read_u32(bp, 4l) != uint(IMAGE_VERSION)) ||
            (read_u32(bp, 8l) != uint(IMAGE_PAGE) || read_u64(bp, 40l) != hash(image_identity(tag)))) {
        // every decline is loud (short of the file simply not existing) — a silently
        // regenerating cache is undebuggable
        to_log(LOG_WARNING, "dasLLAMA image: '{path}' declined — size {msize} magic {read_u32(bp, 0l)} version {read_u32(bp, 4l)} (want {IMAGE_VERSION}) page {read_u32(bp, 8l)} identity {read_u64(bp, 40l)} (want {hash(image_identity(tag))} = '{image_identity(tag)}')\n")
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
    // the meta blob is the ONE copied+parsed piece (small); planes stay lazy in the mapping
    var meta_blob : array<uint8>
    meta_blob |> resize(int(meta_bytes))
    unsafe {
        memcpy(addr(meta_blob[0]), reinterpret<void?>(bp + int64(meta_off)), int(meta_bytes))
    }
    var meta <- new MemSerializer(meta_blob)
    var rarch = Archive(reading = true, stream = meta)
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

//! Model entry point — see load_image.
def load_model_image(path : string; var t : Model) : bool {
    if (!load_image(path, t)) {
        return false
    }
    // AFTER the plane borrows (image_post_load runs before them, when every array is still
    // empty): the GPU MoE tier gathers from the PREPARED planes, so the mapped rail serves it too
    moe_gpu_upload_resident(t)
    return true
}

//! The image-cached model load — ``load_model``'s engine half, public for callers bypassing the
//! full dasllama facade. ``.dlim`` maps directly (wrong identity panics); q8-mode gguf maps/saves
//! a prepared image (``DASLLAMA_IMAGE=0`` disables); else plain gguf. Call inside ``with_job_que()``.
def load_model_cached(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    if (path |> ends_with(".dlim")) {
        apply_box_profile_runtime()
        let ts_img = ref_time_ticks()
        var t = Model()
        if (!load_model_image(path, t)) {
            panic("dasLLAMA: '{path}' is not a prepared image for this box/knobs (identity {image_identity()}) — regenerate it from the source gguf")
        }
        to_log(LOG_INFO, "dasLLAMA: prepared image mapped in {get_time_usec(ts_img) / 1000} ms — {path}\n")
        return <- t
    }
    if (mode == QuantMode.q8 && get_env_variable("DASLLAMA_IMAGE") != "0") {
        apply_box_profile_runtime()   // a backend pin must precede the identity (it names the backend)
        let img = image_path_for(path)
        let ts_img = ref_time_ticks()
        var t = Model()
        if (load_model_image(img, t)) {
            to_log(LOG_INFO, "dasLLAMA: prepared image mapped in {get_time_usec(ts_img) / 1000} ms — {img}\n")
            return <- t
        }
        var m <- load_model_(path, mode)
        let ts_save = ref_time_ticks()
        if (save_model_image(m, img)) {
            to_log(LOG_INFO, "dasLLAMA: prepared image saved in {get_time_usec(ts_save) / 1000} ms — {img}\n")
        }
        return <- m
    }
    return <- load_model_(path, mode)
}
