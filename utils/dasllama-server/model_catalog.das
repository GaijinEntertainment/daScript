options gen2
options persistent_heap
options _dasllama_internal = true   // the env rail (g_env_*) is knob-internal; this module consumes no inference facade

// The curated model catalog + the download rail behind GET/POST /catalog.
// entries: commit-pinned HF files, canonical sha256 per entry
// download: sha verified before `<file>.partial` renames into place — never waived
// finish: the house `.sha` sidecar lands beside the file

require daslib/fio
require daslib/defer
require dasllama/dasllama_env   // g_env_harness.models_dir / g_env_core home fields — the env rail
require ../../modules/dasLLAMA/performance/model_specs.das public   // the ONE model set — the catalog is a view over it (re-exported: the tests gate its serve rows)
require daslib/json_boost
require daslib/jobque_boost
require daslib/strings_boost
require strings

//! One curated model: a commit-pinned, ungated HF file this engine serves today.
//! `vram_hint_gb` is the advertised working-set hint, not a fit check. The `vision_*`
//! fields carry the row's pinned vision tower (mmproj) when it has one — "" = text only.
struct public CatalogEntry {
    name : string          // route/catalog id (kebab, stable)
    display : string       // human name on the card
    note : string          // one-line what-it-is
    file : string          // upstream filename — the on-disk name in models_dir
    url : string           // commit-pinned resolve URL
    bytes : int64          // canonical size
    sha256 : string        // canonical content sha
    ctx : int              // trained context length
    vram_hint_gb : float
    is_default : bool      // the one the getting-started path recommends
    vision_file : string   // pinned vision-tower artifact ("" = text only)
    vision_url : string
    vision_bytes : int64
    vision_sha256 : string
}

def private catalog_entry_of(s : ModelSpec) : CatalogEntry {
    let tower <- serve_vision_tower(s.serve_vision)
    return CatalogEntry(name = s.serve_name, display = s.serve_display,
        note = s.serve_note, file = s.file, url = s.url, bytes = s.bytes,
        sha256 = s.sha256, ctx = s.serve_ctx, vram_hint_gb = s.serve_vram_gb,
        is_default = s.serve_default,
        vision_file = tower.name, vision_url = tower.url,
        vision_bytes = tower.bytes, vision_sha256 = tower.sha256)
}

//! The serving catalog: a VIEW over `model_specs()` (the one model set) — rows carrying a
//! `serve_name`. Every entry is commit-pinned + sha-canonical there; every one is
//! load-tested on this engine before it ships.
def public model_catalog() : array<CatalogEntry> {
    return <- [for (s in model_specs()); catalog_entry_of(s); where !empty(s.serve_name)]
}

//! The pure resolution core: env override > config key > `<home>/.dasllama/models`;
//! no trailing separator. Split from the wrapper so every arm is testable without
//! touching the process environment.
def public catalog_models_dir_from(cfg_models_dir, env_override, home : string) : string {
    if (!empty(env_override)) {
        return strip_trailing_sep(env_override)
    }
    if (!empty(cfg_models_dir)) {
        return strip_trailing_sep(cfg_models_dir)
    }
    return path_join(path_join(home, ".dasllama"), "models")
}

//! The SERVER's models-dir resolution: DASLLAMA_MODELS_DIR > the `models_dir` config key >
//! `~/.dasllama/models` (USERPROFILE on Windows). Distinct from the rig/test resolution
//! `models_dir_resolved()` (dasllama_env), whose fallback is the rig dir.
def public catalog_models_dir(cfg_models_dir : string) : string {
    let home = !empty(g_env_core.user_profile) ? g_env_core.user_profile : g_env_core.home
    return catalog_models_dir_from(cfg_models_dir, catalog_env_override(), home)
}

//! The env-rail override the resolution honours ("" = unset) — exposed so facade-only
//! callers (main's boot announce, the tests) never touch g_env_* themselves.
def public catalog_env_override : string
    => g_env_harness.models_dir

//! The env rail's GPU master switch read as an explicit OFF (`DASLLAMA_GPU=0`) — exposed like
//! [[catalog_env_override]] so the facade-only server root never touches g_env_* itself. The
//! defaults-first backend pick treats it as the opt-out that knob documents.
def public gpu_env_master_off : bool
    => is_some(g_env_gpu.gpu) && !unwrap(g_env_gpu.gpu)

//! The engine noisy-diagnostics switch (`DASLLAMA_NOISY`) read the facade-only way, like
//! [[gpu_env_master_off]]. The server gates its optional [tickprof] serving-loop profile on it,
//! so a production server pays no clock reads and emits no profile lines unless noisy is on.
def public env_noisy_diag : bool
    => g_env_engine.noisy

def private strip_trailing_sep(p : string) : string {
    if (!ends_with(p, "/") && !ends_with(p, "\\")) {
        return p
    }
    let trimmed = slice(p, 0, length(p) - 1)
    // a bare root must keep its separator: "/" and "C:\" would otherwise turn into the
    // working directory ("") or a drive-relative path ("C:")
    return empty(trimmed) || ends_with(trimmed, ":") ? p : trimmed
}

def public catalog_local_path(dir : string; e : CatalogEntry) : string {
    let p = path_join(dir, e.file)
    return stat(p).is_valid ? p : ""
}

// ===== download state (bake-pattern: worker thread + Channel, drained on the HTTP tick) =====

var private g_dl_state = "idle"          // idle | downloading | verifying | done | failed
var private g_dl_name = ""               // catalog name of the running/last download
var private g_dl_error = ""
var private g_dl_partial = ""            // the .partial path — progress = its size
var private g_dl_want_bytes = 0l
var private g_dl_events : Channel?
// parent-side anchors for the worker's captured strings: the capture block of a context-clone
// thread is invisible to the parent GC, so these keep the sources alive until the run ends
var private g_dl_url = ""
var private g_dl_final = ""
var private g_dl_sha_path = ""
var private g_dl_want_sha = ""

enum private DlEventKind {
    verifying
    done
    failed
}

struct private DlEvent {
    kind : DlEventKind
    text : string      // failed: the reason
}

def public catalog_download_running() : bool {
    return g_dl_state == "downloading" || g_dl_state == "verifying"
}

//! Create the standing download-events channel (call from init_server, like the bake channel).
def public catalog_events_init {
    g_dl_events = unsafe(channel_create())
}

//! Destroy it (call from the server shutdown path, like the bake channel). A still-running
//! worker holds a ref and channel_remove throws on outstanding refs — leave the channel to
//! the worker (bench/bake shutdown carries the same guard) instead of aborting the stop.
def public catalog_events_shutdown {
    var ev = g_dl_events
    g_dl_events = null
    if (ev != null) {
        if (catalog_download_running()) {
            to_log(LOG_WARNING, "dasllama-server: shutdown with a catalog download still running — channel left to the worker\n")
        } else {
            unsafe {
                ev |> channel_remove
            }
        }
    }
}

def private sha_cmd(path : string) : array<string> {
    if (get_platform_name() == "windows") {
        return <- ["certutil", "-hashfile", path, "SHA256"]
    }
    if (get_platform_name() == "darwin") {
        return <- ["shasum", "-a", "256", path]
    }
    return <- ["sha256sum", path]
}

// hex sha256 via the OS tool (certutil / sha256sum / shasum) — "" on failure
def private file_sha256(path : string) : string {
    var out = ""
    var args <- sha_cmd(path)
    var captured = ""
    let rc = run_and_capture(args, captured, 3600.0)
    delete args
    if (rc != 0) {
        return ""
    }
    // certutil: the digest is line 2; sha256sum/shasum: first token of line 1
    var lines <- split(captured, "\n")
    defer() {
        delete lines
    }
    if (get_platform_name() == "windows") {
        if (length(lines) >= 2) {
            out = to_lower(replace(strip(lines[1]), " ", ""))
        }
    } elif (!empty(lines)) {
        var toks <- split(strip(lines[0]), " ")
        if (!empty(toks)) {
            out = to_lower(toks[0])
        }
        delete toks
    }
    return length(out) == 64 ? out : ""
}

def private dl_worker(url_, partial_, final_path_, sha_path_, want_sha_ : string; want_bytes : int64; var events : Channel?&) {
    defer() {
        events |> release
    }
    // context-clone: captured handles live in the PARENT string heap, so a mid-download GC
    // can retarget these paths - copy into this context first
    let url = clone_string(url_)
    let partial = clone_string(partial_)
    let final_path = clone_string(final_path_)
    let sha_path = clone_string(sha_path_)
    let want_sha = clone_string(want_sha_)
    // curl owns resume (-C -) and retries; its exit status is advisory — the verify is the gate
    var args <- ["curl", "-sSL", "-C", "-", "--retry", "5", "--retry-delay", "10", "-o", partial, url]
    var out = ""
    run_and_capture(args, out, 86400.0)
    delete args
    let st = stat(partial)
    if (!st.is_valid || int64(st.size) != want_bytes) {
        events |> push_clone(DlEvent(kind = DlEventKind.failed,
            text = "download incomplete ({st.is_valid ? int64(st.size) : 0l} of {want_bytes} bytes) - network trouble; retry resumes where it stopped"))
        return
    }
    events |> push_clone(DlEvent(kind = DlEventKind.verifying, text = ""))
    let have = file_sha256(partial)
    if (have != want_sha) {
        remove(partial)   // wrong bytes re-download from zero — resume would keep the corruption
        events |> push_clone(DlEvent(kind = DlEventKind.failed,
            text = "sha256 MISMATCH (have {empty(have) ? "unhashable" : slice(have, 0, 12)}..., want {slice(want_sha, 0, 12)}...) - deleted; the pinned upstream file changed or the transfer corrupted"))
        return
    }
    if (!rename(partial, final_path)) {
        events |> push_clone(DlEvent(kind = DlEventKind.failed, text = "verified but could not rename into place: {final_path}"))
        return
    }
    fopen(sha_path, "wb") $(f) {
        if (f != null) {
            let fst = stat(final_path)
            // the exact file_identity sidecar shape (profile_common): size prints as the raw
            // uint64 (hex) and the mtime as ISO seconds - any drift and the cache silently re-hashes
            f |> fwrite("{want_sha}\t{fst.size}\t{format_time(fst.mtime, "%Y-%m-%dT%H:%M:%S")}")
        }
    }
    events |> push_clone(DlEvent(kind = DlEventKind.done, text = ""))
}

// the shared start core: one download at a time, any pinned catalog artifact (model or tower)
def private download_start_item(dir, id, file, url, want_sha : string; want_bytes : int64) : string {
    if (catalog_download_running()) {
        return "a download is already running ({g_dl_name})"
    }
    if (empty(url) || empty(want_sha)) {
        return "no pinned artifact for '{id}'"
    }
    if (stat(path_join(dir, file)).is_valid) {
        return "{file} is already present"
    }
    if (!mkdir_rec(dir)) {
        return "cannot create models dir {dir}"
    }
    let free = int64(disk_space(dir).available)
    if (free > 0l && free < want_bytes + (1l << 30l)) {
        return "not enough disk space in {dir}: need {(want_bytes >> 30l) + 2l} GB free, have {free >> 30l} GB"
    }
    if (g_dl_events == null) {
        return "the download rail is not initialized (server boot owns the channel)"
    }
    g_dl_state = "downloading"
    g_dl_name = id
    g_dl_error = ""
    g_dl_want_bytes = want_bytes
    g_dl_partial = path_join(dir, "{file}.partial")
    g_dl_url = url
    g_dl_final = path_join(dir, file)
    g_dl_sha_path = path_join(dir, "{file}.sha")
    g_dl_want_sha = want_sha
    let url_c = g_dl_url
    let partial = g_dl_partial
    let final_path = g_dl_final
    let sha_path = g_dl_sha_path
    let want_sha_c = g_dl_want_sha
    var events_capture = g_dl_events
    new_thread() <| @capture(= url_c, = partial, = final_path, = sha_path, = want_sha_c, = want_bytes, = events_capture) {
        dl_worker(url_c, partial, final_path, sha_path, want_sha_c, want_bytes, events_capture)
    }
    return ""
}

//! Start downloading catalog entry `e` into `dir`. One at a time; "" on success, else the refusal.
def public catalog_download_start(dir : string; e : CatalogEntry) : string {
    return download_start_item(dir, e.name, e.file, e.url, e.sha256, e.bytes)
}

//! Start downloading entry `e`'s vision tower (download id "vision:<name>"). "" on success.
def public catalog_download_start_vision(dir : string; e : CatalogEntry) : string {
    if (empty(e.vision_file)) {
        return "'{e.name}' has no vision tower"
    }
    return download_start_item(dir, "vision:{e.name}", e.vision_file, e.vision_url, e.vision_sha256, e.vision_bytes)
}

//! Start downloading the ASR tower (parakeet v3; download id "asr"). "" on success.
def public catalog_download_start_asr(dir : string) : string {
    let tower <- serve_asr_tower()
    return download_start_item(dir, "asr", tower.name, tower.url, tower.sha256, tower.bytes)
}

//! Drain worker events; call once per HTTP tick.
def public catalog_download_tick() {
    if (g_dl_events == null) {
        return
    }
    var more = true
    while (more) {
        more = g_dl_events |> try_pop_clone() $(ev : DlEvent#) {
            if (ev.kind == DlEventKind.verifying) {
                g_dl_state = "verifying"
            } elif (ev.kind == DlEventKind.done) {
                g_dl_state = "done"
            } else {
                g_dl_state = "failed"
                g_dl_error = clone_string(ev.text)
            }
        }
    }
}

//! The GET /catalog document: entries with local presence (model AND vision tower), the ASR
//! tower, the box's memory facts (`ram_gb` in GiB, `vram_mb` in MiB, 0 = unknown — the page's fit badges),
//! plus the download state machine.
def public catalog_state_json(dir : string; ram_gb : int = 0; vram_mb : int64 = 0l) : string {
    var cat <- model_catalog()
    var entries : array<JsonValue?>
    entries |> reserve(length(cat))
    for (e in cat) {
        let local = catalog_local_path(dir, e)
        let vision_path = empty(e.vision_file) ? "" : path_join(dir, e.vision_file)
        let vision_present = !empty(vision_path) && stat(vision_path).is_valid
        entries |> push(JV((
            name = e.name, display = e.display, note = e.note, file = e.file,
            bytes = e.bytes, sha256 = e.sha256, ctx = e.ctx,
            vram_hint_gb = e.vram_hint_gb, is_default = e.is_default,
            present = !empty(local), path = local,
            vision_file = e.vision_file, vision_bytes = e.vision_bytes,
            vision_present = vision_present,
            vision_path = vision_present ? vision_path : "")))
    }
    let asr_tower <- serve_asr_tower()
    let asr_path = path_join(dir, asr_tower.name)
    let asr_present = stat(asr_path).is_valid
    var got = 0l
    if (g_dl_state == "downloading") {
        let st = stat(g_dl_partial)
        got = st.is_valid ? int64(st.size) : 0l
    } elif (g_dl_state == "verifying") {
        got = g_dl_want_bytes
    }
    var doc = JV((
        models_dir = dir,
        entries = JV(entries),
        asr = (file = asr_tower.name, bytes = asr_tower.bytes,
            present = asr_present, path = asr_present ? asr_path : ""),
        box = (ram_gb = ram_gb, vram_mb = vram_mb),
        download = (state = g_dl_state, name = g_dl_name, got = got,
            total = g_dl_want_bytes, error = g_dl_error)))
    let text = write_json_compact(doc)
    unsafe {
        delete doc
    }
    return text
}
