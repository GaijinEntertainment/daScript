options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)

// dasllama-convert — turn a GGUF into a prepared .dlim image, offline.
//
//   bin/daslang -jit utils/dasllama-convert/main.das -- -m <model.gguf> [-o <out.dlim>] [-q q8]
//
// The image is BOX- AND KNOB-specific (see image_identity in dasllama_image.das), so the default
// output name carries the identity hash and is exactly what the normal load path looks for — run
// this ahead of time and the first real load maps instead of converting. `--flavor` picks which
// image a config with several flavors emits (planar CPU, metal blob, vulkan-baked).
//
// The config is EXPLICIT (DlimConfiguration): `--dump-config` prints/writes this box's config as
// JSON; `--config x.json` bakes against a supplied one — applied knob-by-knob, then verified
// fail-closed against the resulting identity, so a knob that did not take is an error, never a
// silently different image. `-f vulkan` bakes OFFLINE: the GPU walk's accept/refuse arithmetic
// runs against the config's numbers (dry tier), gathers collect into vkblob — no VRAM touched.
//
// Conversion is I/O bound at both ends: the transcode is ~5% of wall time, the rest is reading the
// GGUF and writing the image. Both numbers are reported per stage so a slow box is diagnosable
// without a profiler.

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration
require dasllama/dasllama_math          // apply_box_profile_runtime + select_matmul_backend_for_load
require dasllama/dasllama_image         // the .dlim rail itself
require dasllama/dasllama_layout        // convert_model_to_metal_blob — the metal flavor's transform
require dasllama/dasllama_config        // DlimConfiguration — --config / --dump-config
require dasllama/dasllama_gguf          // GGUFMeta — the streaming block's mapping handle
require dasllama/dasllama_asr           // the ASR family loaders — their wraps bake the family images
require dasllama/dasllama_audio         // load_audio_tower — the qwen2a/ultravox/voxtral mmproj family
require daslib/clargs
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost
require strings


enum private Flavor {
    planar      // the CPU image every box can load
    metal       // blob-only Apple flavor (METAL_IMAGE_TAG)
    vulkan      // planar + baked GPU-layout planes
}

[CommandLineArgs]
struct ConvertArgs {
    // deliberately NOT @clarg_required: that check fires before the help branch, so
    // `--show-help` alone would exit 2 with an error line. Validated by hand below instead.
    @clarg_short = "m"
    @clarg_doc = "Source GGUF path (the FIRST shard for split models)"
    model : string

    @clarg_short = "o"
    @clarg_doc = "Output .dlim path (default: the identity-hashed name beside the GGUF, which is what the load path looks for)"
    out : string

    @clarg_short = "q"
    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_short = "f"
    @clarg_doc = "Which image flavor to emit (default: planar)"
    flavor : Flavor = Flavor.planar

    @clarg_doc = "Path to a DlimConfiguration JSON (from --dump-config on the target box) to bake against"
    config : string

    @clarg_doc = "Print this box's DlimConfiguration JSON (or write it to -o) and exit"
    dump_config : bool

    @clarg_doc = "P3 trim (with -f vulkan): drop the big CPU weight families from the image — pure resident-driver dense models only; serve with DASLLAMA_TRIM=1"
    trim : bool

    @clarg_doc = "Streaming conversion (planar q8): big planes go source->transcode->image without materializing in RAM; unstreamable models fall back to eager automatically"
    stream : bool

    @clarg_doc = "List every .dlim beside -m (a GGUF, or a directory): size, version, verdict, baked identity"
    list : bool

    @clarg_doc = "GC beside -m: old-version and unreadable .dlim files (never a loadable image — another box's bake reads as OTHER and stays). DRY-RUN unless --apply"
    clean : bool

    @clarg_doc = "With --clean: also delete valid other-identity images — only CURRENT survives (old tune identities pile up across re-mints)"
    clean_other : bool

    @clarg_doc = "With --clean: actually delete (the default is a dry-run listing)"
    apply : bool

    @clarg_doc = "Convert even if a valid image is already there (default: report it and exit 0)"
    force : bool

    @clarg_doc = "Do NOT remove stale '*.dlim.<ticks>.tmp' siblings left behind by killed runs"
    keep_stale_tmp : bool

    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// A killed converter leaves its tmp behind — save_image only removes its own on a CLEAN failure.
// They are full-size (95 GB each was found on this box), so sweep them before writing another.
def private sweep_stale_tmp(gguf : string) : int {
    let dir_path = dir_name(gguf)
    let base = base_name(gguf)
    var freed = 0l
    var n = 0
    dir(dir_path) $(name : string) {
        // only OUR tmp shape (<gguf>….dlim.<ticks>.tmp) — never an unrelated .tmp
        if (!(name |> starts_with(base)) || !(name |> ends_with(".tmp")) || find(name, ".dlim.") < 0) {
            return
        }
        let full = path_join(dir_path, name)
        var st : FStat
        if (!stat(full, st) || st.is_dir) {
            return
        }
        let bytes = int64(st.size)
        if (remove(full)) {
            freed += bytes
            n++
            to_log(LOG_INFO, "dasllama-convert: removed stale tmp {name} ({bytes >> 20l} MB)\n")
        } else {
            to_log(LOG_WARNING, "dasllama-convert: could not remove stale tmp {name} — in use by a live converter?\n")
        }
    }
    if (n > 0) {
        to_log(LOG_INFO, "dasllama-convert: reclaimed {freed >> 30l} GB from {n} stale tmp file(s)\n")
    }
    return n
}

// q is a REFERENCE — a plain `var q : QuantMode` is a workhorse-type copy and the caller
// would never see the write (LINT003 caught exactly that)
def private quant_from_string(s : string; var q : QuantMode&) : bool {
    if (s == "fp32") {
        q = QuantMode.fp32
    } elif (s == "q8") {
        q = QuantMode.q8
    } elif (s == "q4_0") {
        q = QuantMode.q4_0
    } else {
        return false
    }
    return true
}

// list = the inventory; clean = GC of what can never load again (both via the dasllama_image
// library — the server's /v1/images + startup GC ride the same functions)
def private list_images(cfg : ConvertArgs) : int {
    var st : FStat
    if (!stat(cfg.model, st)) {
        to_log(LOG_ERROR, "dasllama-convert: no such path '{cfg.model}'\n")
        return 2
    }
    if (!cfg.clean) {
        var inv <- dlim_inventory(cfg.model)
        for (im in inv) {
            let idnote = empty(im.identity) ? "" : "  {im.identity}"
            to_log(LOG_INFO, "dasllama-convert: {im.file}\n")
            to_log(LOG_INFO, "dasllama-convert:   {im.bytes >> 20l} MB  v{im.version}  {im.verdict}{idnote}\n")
        }
        to_log(LOG_INFO, "dasllama-convert: {length(inv)} image(s)\n")
        delete inv
        return 0
    }
    if (!cfg.apply) {
        var inv <- dlim_inventory(cfg.model)
        for (im in inv) {
            if (im.verdict != "CURRENT" && (cfg.clean_other || im.verdict != "OTHER")) {
                to_log(LOG_INFO, "dasllama-convert: would remove {im.file} ({im.bytes >> 20l} MB, {im.verdict})\n")
            }
        }
        delete inv
    }
    let r = dlim_clean(cfg.model, cfg.apply, keep_other = !cfg.clean_other)
    if (cfg.apply) {
        to_log(LOG_INFO, "dasllama-convert: removed {r.removed} of {r.stale} stale image(s), reclaimed {r.freed >> 20l} MB\n")
        if (!st.is_dir) {
            sweep_stale_tmp(cfg.model)
        }
    } else {
        to_log(LOG_INFO, "dasllama-convert: {r.stale} stale of {r.total} image(s) — rerun with --apply to remove\n")
    }
    return 0
}

// "" = a regular decoder GGUF (the Model flow); else the audio family the source carries:
// whisper | parakeet | canary | qwen3a | gemma4a | tower (qwen2a/ultravox/voxtral mmproj) |
// vision (a vision mmproj — declined loudly in convert_asr)
def private sniff_asr_kind(path : string) : string {
    var st : FStat
    if (!stat(path, st) || st.is_dir || int64(st.size) < 8l) {
        return ""
    }
    var kind = ""
    let f = fopen(path, "rb")
    if (f == null) {
        return ""
    }
    fmap(f) $(var bytes : array<uint8>#) {
        let magic = rd_u32(bytes, 0l)
        if (magic == 0x67676d6cu) {   // ggml bin: whisper vs parakeet on vocab size
            kind = int(rd_u32(bytes, 4l)) < 32768 ? "parakeet" : "whisper"
        } elif (magic == CANARY_MAGIC) {
            kind = "canary"
        } elif (magic == 0x46554747u) {   // GGUF: an audio mmproj or a plain decoder
            var inscope m <- parse_gguf_meta(bytes)
            var proj = ""
            if (gguf_has(m, "clip.audio.projector_type")) {
                proj = gguf_str(m, bytes, "clip.audio.projector_type")
            } elif (gguf_has(m, "clip.projector_type")) {
                proj = gguf_str(m, bytes, "clip.projector_type")
            }
            if (proj == "qwen3a" || proj == "gemma4a") {
                kind = proj
            } elif (proj == "qwen2a" || proj == "qwen2.5o" || proj == "ultravox" || proj == "voxtral") {
                kind = "tower"
            } elif (proj != "") {
                kind = "vision"
            }
        }
    }
    fclose(f)
    return kind
}

// ASR carriers (whisper/parakeet ggml bins, the CNRY encoder, audio mmproj GGUFs) bake through
// their family loaders' own image cache: load once eagerly, the wrap saves the identity-named
// image beside the source. -o is unsupported (family images are identity-named siblings).
def private convert_asr(cfg : ConvertArgs; kind : string; qm : QuantMode) : int {
    if (kind == "vision") {
        to_log(LOG_ERROR, "dasllama-convert: '{cfg.model}' is a VISION mmproj — no das image family bakes those\n")
        return 2
    }
    if (cfg.flavor != Flavor.planar) {
        to_log(LOG_ERROR, "dasllama-convert: ASR sources have a single (planar) image flavor\n")
        return 2
    }
    if (cfg.trim || cfg.stream) {
        to_log(LOG_ERROR, "dasllama-convert: --trim/--stream apply to decoder GGUFs, not ASR sources\n")
        return 2
    }
    if (cfg.out != "") {
        to_log(LOG_ERROR, "dasllama-convert: -o is unsupported for ASR sources — the image lands beside the source under its identity name\n")
        return 2
    }
    let q8 = qm == QuantMode.q8
    if (!q8 && qm != QuantMode.fp32) {
        to_log(LOG_ERROR, "dasllama-convert: ASR sources bake q8 or fp32\n")
        return 2
    }
    var tag = ""
    if (kind == "whisper") {
        tag = q8 ? "whisper-q8" : "whisper-f32"
    } elif (kind == "parakeet") {
        tag = q8 ? "parakeet-q8" : "parakeet-f32"
    } elif (kind == "canary") {
        tag = "canary-f32"       // the CNRY encoder is fp32-only
    } elif (kind == "qwen3a") {
        tag = "qwen3a-f32"
    } elif (kind == "gemma4a") {
        tag = "gemma4a-f32"
    } else {
        tag = q8 ? "tower-q8" : "tower-f32"
    }
    let out = image_path_for(cfg.model, tag)
    to_log(LOG_INFO, "dasllama-convert: {cfg.model} ({kind})\n")
    to_log(LOG_INFO, "dasllama-convert:   -> {out}\n")
    to_log(LOG_INFO, "dasllama-convert:   identity {image_identity(tag)}\n")
    if (!cfg.keep_stale_tmp) {
        sweep_stale_tmp(cfg.model)
    }
    if (stat(out).is_valid) {
        if (!cfg.force) {
            to_log(LOG_INFO, "dasllama-convert: image already present — nothing to do (use --force to rebuild)\n")
            return 0
        }
        if (!remove(out)) {
            to_log(LOG_ERROR, "dasllama-convert: cannot remove '{out}' for the --force rebuild\n")
            return 1
        }
    }
    let ts = ref_time_ticks()
    with_job_que() {
        // the family wrap loads eagerly on the miss and saves the image itself
        if (kind == "whisper") {
            var inscope m <- load_whisper_model(cfg.model, q8)
        } elif (kind == "parakeet") {
            var inscope m <- load_parakeet_model(cfg.model, q8)
        } elif (kind == "canary") {
            var inscope m <- load_canary_encoder(cfg.model)
        } elif (kind == "qwen3a") {
            var inscope m <- load_qwen3a_tower(cfg.model)
        } elif (kind == "gemma4a") {
            var inscope m <- load_gemma4a_encoder(cfg.model)
        } else {
            var inscope m <- load_audio_tower(cfg.model, q8)
        }
    }
    var st : FStat
    if (!stat(out, st)) {
        to_log(LOG_ERROR, "dasllama-convert: '{out}' did not land — see the log above (DASLLAMA_IMAGE=0 set?)\n")
        return 1
    }
    let total_ms = int64(get_time_usec(ts)) / 1000l
    to_log(LOG_INFO, "dasllama-convert: wrote {int64(st.size) >> 20l} MB in {total_ms} ms total\n")
    return 0
}

def private flavor_tag(f : Flavor) : string {
    if (f == Flavor.metal) {
        return METAL_IMAGE_TAG
    }
    if (f == Flavor.vulkan) {
        let cfg = moe_gpu_bake_tag()
        if (cfg == "") {
            panic("dasllama-convert: --flavor vulkan needs the vulkan tier armed (see DASLLAMA_GPU_* in modules/dasLLAMA/ENVIRONMENT.md)")
        }
        return cfg   // dlim_vulkan_tag already carries the "vulkan|" prefix
    }
    return ""
}

[export]
def main() : int {
    var args_r <- parse_args(type<ConvertArgs>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "error: {args_r |> unwrap_err}\n\n")
        print_help(get_command_info(type<ConvertArgs>), "dasllama-convert")
        return 2
    }
    let cfg <- args_r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<ConvertArgs>), "dasllama-convert")
        return 0
    }
    if (cfg.dump_config) {
        // same ordering the load path uses: the sidecar's backend pin precedes the identity
        apply_box_profile_runtime()
        let dc = dlim_config_current("{cfg.quant}")
        let js = dlim_config_json(dc)
        var rc0 = 0
        if (cfg.out != "") {
            var wrote = false
            fopen(cfg.out, "wb") $(f) {
                if (f != null) {
                    f |> fwrite(js)
                    wrote = true
                }
            }
            if (wrote) {
                to_log(LOG_INFO, "dasllama-convert: config -> {cfg.out}\n")
            } else {
                to_log(LOG_ERROR, "dasllama-convert: cannot write '{cfg.out}'\n")
                rc0 = 1
            }
        } else {
            print("{js}\n")
        }
        to_log(LOG_INFO, "dasllama-convert: identity {dlim_identity(dc, IMAGE_VERSION)}\n")
        return rc0
    }
    if (cfg.model == "") {
        to_log(LOG_ERROR, "dasllama-convert: --model is required\n\n")
        print_help(get_command_info(type<ConvertArgs>), "dasllama-convert")
        return 2
    }
    if (cfg.list || cfg.clean) {
        // verdicts compare against this box's CURRENT identities — same pin the load path takes
        apply_box_profile_runtime()
        return list_images(cfg)
    }
    if (!stat(cfg.model).is_valid) {
        to_log(LOG_ERROR, "dasllama-convert: no such GGUF '{cfg.model}'\n")
        return 2
    }
    if (cfg.model |> ends_with(".dlim")) {
        to_log(LOG_ERROR, "dasllama-convert: '{cfg.model}' is already a prepared image — this tool converts GGUF -> .dlim\n")
        return 2
    }
    // the backend pin must precede the identity: image_identity names the active kernel backend
    apply_box_profile_runtime()
    var qm = cfg.quant
    var want = DlimConfiguration()
    var have_want = false
    if (cfg.config != "") {
        var text = ""
        fopen(cfg.config, "rb") $(f) {
            if (f != null) {
                text = fread(f)
            }
        }
        if (text == "") {
            to_log(LOG_ERROR, "dasllama-convert: cannot read --config '{cfg.config}'\n")
            return 2
        }
        if (!dlim_config_from_json(text, want)) {
            to_log(LOG_ERROR, "dasllama-convert: --config '{cfg.config}' is not a DlimConfiguration JSON\n")
            return 2
        }
        if (!quant_from_string(want.quant, qm)) {
            to_log(LOG_ERROR, "dasllama-convert: --config quant '{want.quant}' is not a QuantMode\n")
            return 2
        }
        // apply what a host can take, then verify FAIL-CLOSED: any knob that did not take
        // (missing s16 twins, no repack family for a foreign backend) shows up as an identity
        // mismatch below. A FOREIGN backend name arms repack-by-numbers: the local generic
        // grp<mr> primitives produce the target's exact plane bytes from the config's numbers.
        set_wscale_f16(want.cpu.wscale_f16)
        set_kquant_native(want.cpu.kquant_native)
        set_kq_q40_native(want.cpu.kq_q40_native)
        set_kq_q50_native(want.cpu.kq_q50_native)
        set_kq_q51_native(want.cpu.kq_q51_native)
        if (!pin_backend_for_bake(want.cpu)) {
            to_log(LOG_ERROR, "dasllama-convert: --config targets '{want.cpu.backend}' and this host has no repack family to bake it with\n")
            return 2
        }
        let want_ident = dlim_identity(want, IMAGE_VERSION)
        let have_ident = dlim_identity(dlim_config_current(want.quant), IMAGE_VERSION)
        if (have_ident != want_ident) {
            to_log(LOG_ERROR, "dasllama-convert: --config could not be fully applied on this host:\n  want {want_ident}\n  have {have_ident}\n")
            return 2
        }
        have_want = true
    }
    let asr_kind = sniff_asr_kind(cfg.model)
    if (asr_kind != "") {
        return convert_asr(cfg, asr_kind, qm)
    }
    if (cfg.trim && cfg.flavor != Flavor.vulkan) {
        to_log(LOG_ERROR, "dasllama-convert: --trim is a vulkan-flavor property (add -f vulkan)\n")
        return 2
    }
    if (cfg.flavor == Flavor.metal) {
        // the metal donor contract: the blob transform reads ROW-MAJOR planes, so the mint pins
        // the portable backend (repack interleaves are unreadable by the drivers). Portable's
        // layout numbers are box-independent — the metal identity converges across boxes
        pin_kernel_backend("portable")
    }
    if (cfg.flavor == Flavor.vulkan) {
        // OFFLINE bake: the walk's accept/refuse arithmetic runs against the config's numbers,
        // the gathers collect, and no Vulkan call is made — no instance, no device, no VRAM
        var vkc = DlimVulkanConfig()
        if (have_want && want.vulkan_on) {
            vkc = want.vulkan
        } else {
            let probed = dlim_config_current("{qm}")
            if (!probed.vulkan_on) {
                to_log(LOG_ERROR, "dasllama-convert: --flavor vulkan needs the vulkan tier armed (DASLLAMA_GPU=1, see modules/dasLLAMA/ENVIRONMENT.md) or a --config carrying a vulkan section\n")
                return 2
            }
            vkc = probed.vulkan
        }
        vkc.trim = cfg.trim
        if (!moe_gpu_dry_bake_arm(vkc)) {
            to_log(LOG_ERROR, "dasllama-convert: offline vulkan bake could not arm — see the log above\n")
            return 2
        }
    }
    let tag = flavor_tag(cfg.flavor)
    let qs = "{qm}"
    let out = cfg.out != "" ? cfg.out : image_path_for(cfg.model, tag, qs)
    to_log(LOG_INFO, "dasllama-convert: {cfg.model}\n")
    to_log(LOG_INFO, "dasllama-convert:   -> {out}\n")
    to_log(LOG_INFO, "dasllama-convert:   identity {image_identity(tag, qs)}\n")

    if (!cfg.keep_stale_tmp) {
        sweep_stale_tmp(cfg.model)
    }
    if (!cfg.force && stat(out).is_valid) {
        to_log(LOG_INFO, "dasllama-convert: image already present — nothing to do (use --force to rebuild)\n")
        return 0
    }

    if (cfg.stream && cfg.flavor != Flavor.planar) {
        to_log(LOG_ERROR, "dasllama-convert: --stream covers the planar flavor (metal/vulkan bakes convert eagerly)\n")
        return 2
    }
    var rc = 0
    var did_stream = false
    let ts = ref_time_ticks()
    // the job que carries the transcode/repack passes; the load spins its own if we do not
    with_job_que() {
        if (cfg.stream && cfg.flavor == Flavor.planar) {
            // NO apply_box_profile_runtime() here: it already ran before the --config override,
            // and re-reading the sidecar would revert the config's pinned knobs AFTER the
            // fail-closed identity verify (silently un-pinning a cross-box streamed bake)
            var mt = Model()
            var jobs : array<FillJob>
            var blk_ran = false
            let sok = load_gguf_streaming(cfg.model, qm, mt, jobs) $(gm : GGUFMeta; gbytes : array<uint8>) {
                blk_ran = true
                if (empty(jobs)) {
                    to_log(LOG_INFO, "dasllama-convert: streaming unavailable for this model — writing eagerly from RAM\n")
                }
                return save_model_image_streaming(mt, gm, gbytes, jobs, out, tag, qs)
            }
            if (sok || blk_ran) {
                did_stream = true
                rc = sok ? 0 : 1
                if (rc != 0) {
                    to_log(LOG_ERROR, "dasllama-convert: streamed write of '{out}' failed — see the error above\n")
                }
            } else {
                to_log(LOG_INFO, "dasllama-convert: streaming declined (split shards?) — converting eagerly\n")
            }
            delete jobs
            delete mt
        }
        if (!did_stream) {
            if (cfg.flavor == Flavor.vulkan) {
                vulkan_bake_begin()   // the load's GPU walk collects into the bake blob (dry — no uploads)
            }
            stream_eager_convert(cfg, qm, qs, out, tag, rc)
        }
    }
    if (rc == 0 && did_stream) {
        var st : FStat
        let bytes = stat(out, st) ? int64(st.size) : 0l
        let total_ms = int64(get_time_usec(ts)) / 1000l
        to_log(LOG_INFO, "dasllama-convert: wrote {bytes >> 20l} MB (streamed) in {total_ms} ms total\n")
    }
    if (rc == 0) {
        dlim_gc_stale(cfg.model)   // the fresh bake proves version-stale siblings dead
    }
    return rc
}

// the pre-streaming eager conversion body, verbatim (load whole -> flavor transform -> save)
def private stream_eager_convert(cfg : ConvertArgs; qm : QuantMode; qs, out, tag : string; var rc : int&) {
    let ts = ref_time_ticks()
    {
        var m <- load_model_(cfg.model, qm)
        let ts_load = ref_time_ticks()
        if (cfg.flavor == Flavor.metal) {
            if (!metal_model_servable(m) || !convert_model_to_metal_blob(m)) {
                to_log(LOG_ERROR, "dasllama-convert: this config has no metal-servable blob flavor\n")
                rc = 1
            }
        }
        if (rc == 0 && cfg.flavor == Flavor.vulkan && !vulkan_bake_take(m)) {
            to_log(LOG_ERROR, "dasllama-convert: the GPU walk collected nothing — the tier declined this model under the supplied config\n")
            rc = 1
        }
        if (rc == 0 && cfg.trim && !trim_model_planes(m)) {
            to_log(LOG_ERROR, "dasllama-convert: --trim declined — see the log above (only pure resident-driver dense models qualify)\n")
            rc = 1
        }
        if (rc == 0 && !save_model_image(m, out, tag, qs)) {
            to_log(LOG_ERROR, "dasllama-convert: writing '{out}' failed — see the error above\n")
            rc = 1
        }
        if (rc == 0) {
            var st : FStat
            let bytes = stat(out, st) ? int64(st.size) : 0l
            let save_ms = int64(get_time_usec(ts_load)) / 1000l
            let total_ms = int64(get_time_usec(ts)) / 1000l
            let mb = bytes >> 20l
            to_log(LOG_INFO, "dasllama-convert: wrote {mb} MB in {save_ms} ms ({save_ms > 0l ? mb * 1000l / save_ms : 0l} MB/s), {total_ms} ms total\n")
        }
        delete m
    }
}
