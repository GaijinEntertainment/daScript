options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_exchange shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_exchange_schema public   // SidecarInfo/BoxIdentity + the validators
require dasllama/dasllama_version      // DASLLAMA_RELEASE — the exchange lookup key
require dasllama/dasllama_env          // g_env_exchange — the env override rail
require llvm/daslib/llvm_tune          // box identity, manifest path, the scope-resolver seam
require dashv/dashv_boost              // outbound HTTP + url_encode
require daslib/clargs                  // get_user_args — the --config discovery scan
require daslib/defer
require daslib/json_boost
require daslib/sha_256
require daslib/toml
require daslib/strings_boost
require daslib/fio
require daslib/tty
require strings

// The dasllama.io exchange client: at an untuned boot, download a matching per-box tune
// sidecar instead of paying ~12 quiet-box minutes; after a local tune, offer/submit it back.
// Registered as llvm_tune's scope resolver by dasllama-server ([init] — before the guard);
// lcpp_bench uses only the submit half. The service contract lives in the repo's
// dasllama-ladder README §1.1; validation is dasllama_exchange_schema, shared with the service.

//! The baked-in exchange; `exchange_url` in the app config or `DASLLAMA_EXCHANGE_URL` override it.
let public EXCHANGE_URL_DEFAULT = "https://dasllama.io"

//! One row of a lookup-ladder response (`GET /api/sidecars?version&box`), newest first within
//! tier ("exact" > same-cpu > same-arch), verified first within a tier.
struct ExchangeHit {
    sha : string
    box : string
    tier : string
    noise : string
    written : string
    engine_sha : string
    verified : bool
    kernel_count : int
    dasllama_version : int
}

//! The exchange policy knobs: config-file keys `exchange_url` / `exchange_accept` /
//! `exchange_submit`, env overrides `DASLLAMA_EXCHANGE_URL` / `DASLLAMA_EXCHANGE_ACCEPT`. The
//! exchange is closed: both policies default to off, and only an explicit key turns them on.
struct ExchangePolicy {
    url : string = EXCHANGE_URL_DEFAULT
    accept : string = "off"         // verified | any | off
    submit : string = "never"       // ask | always | never
    configured : bool = false       // any exchange_* key came from the config file or env — an explicit choice
    configured_via : string = ""    // the knob spelling that expressed it, for the waiver announce
}

//! The first-contact notice, shown before ANY request leaves for the exchange (GDPR: the
//! lookup transmits the box identity, and the server sees the connection's IP like any web
//! server). PLACEHOLDER wording — Gaijin legal owns the final text; the watchdog dialog
//! (utils/watchdog/watchdog.py CONSENT_*) carries the same wording, keep the two in sync.
let public EXCHANGE_CONSENT_NOTICE = "dasLLAMA can download a ready performance-tuning preset (a \"sidecar\") for this machine from dasllama.io, instead of tuning locally.\nThe request sends only your hardware class - platform, CPU model and OS build. No serial numbers, user names or other personal data; no cookies. Sharing your own tuning results back is always confirmed separately.\nDetails: https://legal.gaijin.net/privacypolicy"

// structured events for the watchdog (same contract as @tune): folded into supervisor STATE
def private sc_event(kind : string; kv : string) {
    print("@sidecar {kind}{empty(kv) ? "" : " "}{kv}\n")
}

// ===== transport =====

// A valid sidecar/records response is small; a hostile or MITM'd exchange serving a giant body
// would OOM the client at boot. libhv buffers the body before the callback, so this bounds the
// second (das-heap) copy + the parse — the connect/read timeouts bound the wall-clock.
let private MAX_DOWNLOAD_BYTES = 8 * 1024 * 1024

def private http_get_body(url : string; timeout_s : int; var status : int&) : string {
    var body = ""
    status = 0
    with_http_request() $(var req) {
        req.method = http_method.GET
        req.url := url
        req.connect_timeout = 5
        req.timeout = uint16(timeout_s)
        allow_redirect(req, false)   // never follow the exchange to another host/scheme
        request(req) $(resp) {
            if (resp != null) {
                status = int(resp.status_code)
                if (resp.content_length > MAX_DOWNLOAD_BYTES) {
                    status = -1   // refuse an oversized body before copying it into the das heap
                } else {
                    body = string(resp.body)
                }
            }
        }
    }
    return body
}

def private http_post_body(url : string; payload : string; timeout_s : int; var status : int&) : string {
    var body = ""
    status = 0
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := url
        req.body := payload
        req.connect_timeout = 5
        req.timeout = uint16(timeout_s)
        set_content_type(req, "application/json")
        allow_redirect(req, false)
        request(req) $(resp) {
            if (resp != null) {
                status = int(resp.status_code)
                if (resp.content_length <= MAX_DOWNLOAD_BYTES) {
                    body = string(resp.body)
                }
            }
        }
    }
    return body
}

// ===== the lookup half =====

//! Query the lookup ladder for `box` at `version`. False = transport/shape failure (an empty
//! hit list on a reachable exchange returns true); `hits` comes back in the ladder's order.
def exchange_lookup(base_url : string; version : int; box : string; var hits : array<ExchangeHit>&) : bool {
    var status = 0
    let body = http_get_body("{base_url}/api/sidecars?version={version}&box={url_encode(box)}", 10, status)
    if (status != 200) {
        return false
    }
    var jerr = ""
    var doc = read_json(body, jerr)
    if (doc == null) {
        return false
    }
    let hv = doc?["hits"]
    if (hv == null || !(hv.value is _array)) {
        delete_json(doc)
        return false
    }
    hits |> reserve(length(hv.value as _array))
    for (h in hv.value as _array) {
        hits |> push(ExchangeHit(
            sha = h?["sha"] ?? "", box = h?["box"] ?? "", tier = h?["tier"] ?? "",
            noise = h?["noise"] ?? "", written = h?["written"] ?? "",
            engine_sha = h?["engine_sha"] ?? "", verified = h?["verified"] ?? false,
            kernel_count = int(h?["kernel_count"] ?? 0l),
            dasllama_version = int(h?["dasllama_version"] ?? 0l)))
    }
    delete_json(doc)
    return true
}

//! First hit the accept policy allows (the ladder pre-orders by tier, verified, recency);
//! -1 when nothing qualifies. `accept` off never reaches here — the resolver gates first.
def exchange_pick(hits : array<ExchangeHit>; accept : string) : int {
    for (i in range(length(hits))) {
        if (hits[i].verified || accept == "any") {
            return i
        }
    }
    return -1
}

// a content sha is 64 lowercase hex — validate before it goes into the request path, so an
// exchange listing can't inject path/query characters into the URL we build (the content-hash
// gate refuses a mismatch afterward, but the URL is built from this value up front)
def private is_hex_sha(s : string) : bool {
    if (length(s) != 64) {
        return false
    }
    var ok = true
    peek_data(s) $(arr) {
        for (b in arr) {
            let c = int(b)
            ok = ok && (is_number(c) || (c >= 'a' && c <= 'f'))
        }
    }
    return ok
}

def private exchange_download(base_url : string; sha : string) : string {
    if (!is_hex_sha(sha)) {
        return ""
    }
    var status = 0
    let body = http_get_body("{base_url}/api/sidecar/{sha}", 30, status)
    return status == 200 ? body : ""
}

// stamp provenance and write: origin/exchange_sha always; applied_box only when adopting a
// sibling box's mint (provenance.box keeps naming the box that measured it — see llvm_tune)
def private exchange_write_applied(text : string; sha, doc_box, manifest_path, local_box : string) : bool {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null) {
        return false
    }
    var prov = doc?["provenance"]
    if (prov == null || !(prov.value is _object)) {
        delete_json(doc)
        return false
    }
    update(prov, "origin", JV("exchange"))
    update(prov, "exchange_sha", JV(sha))
    // applied_box only for a genuinely different box — a same-box-modulo-UBR download (this box
    // after a Windows patch) is not a cross-box adoption, so it needs no applied_box override
    if (box_match_key(doc_box) != box_match_key(local_box)) {
        update(prov, "applied_box", JV(local_box))
    }
    let ok = fwrite(manifest_path, write_json(doc))
    delete_json(doc)
    return ok
}

//! Download sidecar `sha` and adopt it as the sidecar at `manifest_path`, behind the full
//! client-side gate: content hash, schema, `DASLLAMA_RELEASE` (the load-bearing staleness
//! check — the mtime rail reads any fresh download as valid), optional listing-box match.
//! Returns "" on success, else the refusal reason. Never trusts the exchange.
def exchange_fetch_and_apply(base_url : string; sha : string; manifest_path : string;
                             expected_box : string = "") : string {
    let text = exchange_download(base_url, sha)
    if (empty(text)) {
        return "download failed"
    }
    if (sha256_hex(text) != sha) {
        return "content hash mismatch"
    }
    var errs : array<string>
    if (!validate_sidecar(text, errs)) {
        let r = "schema: {errs[0]}"
        delete errs
        return r
    }
    delete errs
    var info : SidecarInfo
    if (!parse_sidecar_info(text, info)) {
        return "malformed sidecar"
    }
    if (info.dasllama_version != DASLLAMA_RELEASE) {
        return "made for dasllama version {info.dasllama_version}, this engine is {DASLLAMA_RELEASE}"
    }
    if (!empty(expected_box) && info.box != expected_box) {
        return "box does not match the listing"
    }
    if (!exchange_write_applied(text, sha, info.box, manifest_path, tune_box_identity())) {
        return "could not write {manifest_path}"
    }
    return ""
}

// ===== policy + config discovery =====

//! Locate the app's config file the same way the app does: `--config X` / `--config=X` / `-c X`
//! on the command line, else `<cwd>/<name>`, else `<name>` beside the app script. "" = none.
def exchange_discover_config(app_config_name : string) : string {
    var args <- get_user_args()
    var found = ""
    var take_next = false
    for (a in args) {
        if (take_next) {
            found = "{a}"
            take_next = false
        } elif (a == "--config" || a == "-c") {
            take_next = true
        } elif (a |> starts_with("--config=")) {
            found = slice(a, 9)
        }
    }
    delete args
    if (!empty(found)) {
        return found
    }
    if (stat(app_config_name).is_valid) {
        return app_config_name
    }
    let beside = path_join(dir_name(tune_app_script()), app_config_name)
    return stat(beside).is_valid ? beside : ""
}

//! Resolve the exchange policy: baked defaults < the TOML at `config_path` < env overrides.
//! Unknown values degrade to the safe default, loudly — a typo must never widen acceptance.
def exchange_policy_from_config(config_path : string) : ExchangePolicy {
    var p = ExchangePolicy()
    if (!empty(config_path)) {
        let body = fread(config_path)
        if (!empty(body)) {
            var terr = ""
            var root = read_toml(body, terr)
            if (root != null) {
                // an AUTHORITATIVE toml is what the control page saves, and its gatherConfig
                // persists the exchange defaults on EVERY save - so its exchange keys are not
                // evidence of a choice; only a hand-written config's are (env overrides too)
                let hand_written = !(root?["authoritative"] ?? false)
                let u : string = root?["exchange_url"] ?? ""
                if (!empty(u)) {
                    p.url = u
                    if (hand_written) {
                        p.configured = true
                        p.configured_via = "exchange_url (config)"
                    }
                }
                let a : string = root?["exchange_accept"] ?? ""
                if (!empty(a)) {
                    p.accept = a
                    if (hand_written) {
                        p.configured = true
                        p.configured_via = "exchange_accept (config)"
                    }
                }
                let s : string = root?["exchange_submit"] ?? ""
                if (!empty(s)) {
                    p.submit = s
                    if (hand_written) {
                        p.configured = true
                        p.configured_via = "exchange_submit (config)"
                    }
                }
                unsafe {
                    delete root
                }
            }
        }
    }
    if (!empty(g_env_exchange.exchange_url)) {
        p.url = g_env_exchange.exchange_url
        p.configured = true
        p.configured_via = "DASLLAMA_EXCHANGE_URL (env)"
    }
    if (!empty(g_env_exchange.exchange_accept)) {
        p.accept = g_env_exchange.exchange_accept
        p.configured = true
        p.configured_via = "DASLLAMA_EXCHANGE_ACCEPT (env)"
    }
    if (p.accept != "verified" && p.accept != "any" && p.accept != "off") {
        print("exchange: unknown exchange_accept '{p.accept}' - using off (valid: verified | any | off)\n")
        p.accept = "off"
    }
    if (p.submit != "ask" && p.submit != "always" && p.submit != "never") {
        print("exchange: unknown exchange_submit '{p.submit}' - using never (valid: ask | always | never)\n")
        p.submit = "never"
    }
    return p
}

// ===== consent (GDPR first-contact gate) =====

// the sidecar-sibling naming rule: foo.tune.json and foo.tune both yield foo.tune.<ext>
def private sidecar_sibling_path(manifest : string; ext : string) : string {
    return (ends_with(manifest, ".json")
        ? "{slice(manifest, 0, length(manifest) - 5)}.{ext}"
        : "{manifest}.{ext}")
}

// the sidecar-sibling consent file: "accepted" / "declined", one word. An explicit exchange
// config (any exchange_* key, TOML or env) is itself consent, so the file only governs the
// zero-config default path.
def private exchange_consent_path() : string {
    return sidecar_sibling_path(tune_manifest_path(), "consent")
}

//! The recorded first-contact choice: "accepted" | "declined" | "" (never asked).
def exchange_consent_status() : string {
    let text = fread(exchange_consent_path())
    let word = strip(text)
    return (word == "accepted" || word == "declined") ? word : ""
}

//! Record the first-contact choice (console prompt, watchdog dialog, control page — every
//! surface lands here). Emits the `@sidecar consent` event so the supervisor state follows.
def exchange_record_consent(accept : bool) : bool {
    let word = accept ? "accepted" : "declined"
    if (!fwrite(exchange_consent_path(), "{word}\n")) {
        return false
    }
    sc_event("consent", "state={word}")
    return true
}

// The interactive arm: notice + Accept/Decline on the console. Enter defaults to Accept
// (the dialog's default button); anything starting d/D/n/N declines. Only reached on a real
// terminal — a supervised or piped boot must never block on stdin.
def private consent_prompt_console() : bool {
    print("\n{EXCHANGE_CONSENT_NOTICE}\n\n")
    print("Use the exchange? [Accept]/decline: ")
    let line = fgets(fstdin())
    if (empty(line)) {
        // EOF (Ctrl+D / closed stdin) is a walkaway, not an answer - record nothing, ask
        // again next boot. A bare Enter arrives as "\n", so it never lands here.
        print("exchange: no answer - tuning locally; the question returns next boot\n")
        return false
    }
    let word = to_lower(strip(line))
    let accepted = !(word |> starts_with("d") || word |> starts_with("n"))
    if (!exchange_record_consent(accepted)) {
        print("exchange: could not record the choice at {exchange_consent_path()} - not contacting the exchange\n")
        return false
    }
    print(accepted ? "exchange: enabled - this choice is remembered\n" : "exchange: declined - this box tunes locally; the control page can enable it later\n")
    return accepted
}

// The gate the resolver runs before the first request leaves. True = a request may be made.
// No recorded choice: a real terminal asks inline; anything else stays offline this boot and
// surfaces the question where a human is (watchdog dialog, control page) via the event.
def private exchange_consent_gate() : bool {
    let status = exchange_consent_status()
    if (status == "accepted") {
        print("exchange: consent on record at {exchange_consent_path()} (exchange_record_consent) - proceeding\n")
        return true
    }
    if (status == "declined") {
        print("exchange: disabled by the recorded choice - tuning locally (the control page can enable it)\n")
        return false
    }
    if (is_stdin_terminal() && is_stdout_terminal()) {
        return consent_prompt_console()
    }
    sc_event("consent", "state=needed path={url_encode(exchange_consent_path())}")
    print("exchange: no recorded choice about contacting dasllama.io - tuning locally; answer the tray dialog or use the control page to enable the exchange\n")
    return false
}

// ===== the boot resolver (llvm_tune scope-resolver seam) =====

var private g_resolver_config = ""
var private g_resolve_attempted = false
var private g_resolve_applied = false

//! Register the exchange as the tune-scope resolver — call from the app's `[init]` so the
//! registration lands before the policy guard at the top of `main`. `app_config_name` is the
//! config FILE NAME the app auto-discovers (e.g. "dasllama-server.toml").
def exchange_register_resolver(app_config_name : string) {
    g_resolver_config = app_config_name
    tune_set_scope_resolver(@@exchange_scope_resolver)
}

//! The pre-request decision the boot resolver enforces: nothing leaves for the exchange
//! without an expressed choice. `accept = off` never contacts; an explicit exchange config
//! IS the choice (announced by the knob that expressed it); otherwise the recorded consent
//! file governs - unset asks on a real terminal or emits the `@sidecar consent` event.
def exchange_may_contact(policy : ExchangePolicy) : bool {
    if (policy.accept == "off") {
        return false
    }
    if (policy.configured) {
        // the override announce: name the knob that expressed the choice and waived the question
        print("exchange: consent expressed via {policy.configured_via} - not asking\n")
        return true
    }
    return exchange_consent_gate()
}

def private exchange_scope_resolver(scope_name : string; manifest_path : string; _fnames : string) : bool {
    // one lookup per process — a second scope re-checks completeness off the same applied file
    if (g_resolve_attempted) {
        return g_resolve_applied
    }
    g_resolve_attempted = true
    // the one-shot re-tune marker (exchange_request_retune): tune locally this boot
    let skip = "{manifest_path}.retune"
    if (stat(skip).is_valid) {
        remove(skip)
        print("exchange: re-tune requested from the control page - tuning locally this boot\n")
        return false
    }
    let policy = exchange_policy_from_config(exchange_discover_config(g_resolver_config))
    // GDPR first-contact gate: nothing leaves for the exchange without an expressed choice
    if (!exchange_may_contact(policy)) {
        return false
    }
    let box = tune_box_identity()
    print("exchange: scope '{scope_name}' is untuned - looking up a sidecar for this box (dasllama version {DASLLAMA_RELEASE})\n")
    sc_event("lookup", "version={DASLLAMA_RELEASE}")
    var hits : array<ExchangeHit>
    defer() {
        delete hits
    }
    if (!exchange_lookup(policy.url, DASLLAMA_RELEASE, box, hits)) {
        print("exchange: {policy.url} is unreachable - tuning locally\n")
        sc_event("none", "reason=unreachable")
        return false
    }
    if (empty(hits)) {
        print("exchange: nobody has shared a sidecar for this box yet (version {DASLLAMA_RELEASE}) - tuning locally\n")
        sc_event("none", "reason=no_hits")
        return false
    }
    let pick = exchange_pick(hits, policy.accept)
    if (pick < 0) {
        // unverified never auto-applies and is never a boot question — surface the offer;
        // the live tray menu can adopt it DURING the tune, doing nothing finishes the tune
        let h = hits[0]
        print("exchange: {length(hits)} unverified sidecar(s) match this box - not auto-applied; the tray menu can adopt one instead of finishing the tune\n")
        sc_event("offer", "sha={h.sha} verified=0 tier={h.tier} count={length(hits)}")
        return false
    }
    let h = hits[pick]
    let why = exchange_fetch_and_apply(policy.url, h.sha, manifest_path, h.box)
    if (!empty(why)) {
        print("exchange: rejected sidecar {slice(h.sha, 0, 12)}: {why} - tuning locally\n")
        sc_event("none", "reason=invalid")
        return false
    }
    g_resolve_applied = true
    print("exchange: applied {h.verified ? "verified" : "unverified"} sidecar {slice(h.sha, 0, 12)} ({h.tier} match, {h.kernel_count} kernel families)\n")
    sc_event("apply", "sha={h.sha} verified={h.verified ? 1 : 0} tier={h.tier} kernels={h.kernel_count}")
    print("exchange: sidecar applied, restart to apply the winners\n")
    return true
}

// ===== the submit half =====

// the sidecar-sibling state file remembering what this box already shared
def private exchange_state_path(manifest : string) : string {
    return sidecar_sibling_path(manifest, "exchange.json")
}

def private state_submitted_for(manifest : string) : string {
    let text = fread(exchange_state_path(manifest))
    if (empty(text)) {
        return ""
    }
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null) {
        return ""
    }
    let s : string = doc?["submitted_for"] ?? ""
    let r = "{s}"
    delete_json(doc)
    return r
}

def private state_write_submitted(manifest : string; raw_sha, ladder_sha : string) {
    var doc = JV((submitted_for = raw_sha, ladder_sha = ladder_sha))
    fwrite(exchange_state_path(manifest), write_json(doc))
    delete_json(doc)
}

def private sidecar_origin(text : string) : string {
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null) {
        return ""
    }
    let o : string = doc?["provenance"]?["origin"] ?? ""
    let r = "{o}"
    delete_json(doc)
    return r
}

//! POST an already-stripped sidecar document to the exchange. "" on success (`out_sha` gets
//! the ladder's content sha), else the error. Validates submission grade first — fail early.
def exchange_submit_sidecar_doc(base_url : string; text : string; var out_sha : string&) : string {
    var errs : array<string>
    if (!validate_sidecar_submission(text, errs)) {
        let r = "not submission-grade: {errs[0]}"
        delete errs
        return r
    }
    delete errs
    var status = 0
    let body = http_post_body("{base_url}/api/submit/sidecar", text, 30, status)
    if (status == 0) {
        return "{base_url} is unreachable"
    }
    var jerr = ""
    var doc = read_json(body, jerr)
    if (status != 200) {
        let e : string = doc?["error"] ?? "http {status}"
        let r = "{e}"
        delete_json(doc)
        return r
    }
    let s : string = doc?["sha"] ?? ""
    out_sha = "{s}"
    delete_json(doc)
    return ""
}

//! POST a stamped bench-record store (the lcpp_bench shape) to the exchange. "" on success.
//! The cmd/env/file paths are redacted to basenames first — a community record must not carry
//! the submitter's home layout (the server redacts again; the two agree by construction).
def exchange_submit_records_doc(base_url : string; text : string) : string {
    let redacted = redact_record_paths(text)
    if (empty(redacted)) {
        return "record store does not parse"
    }
    var errs : array<string>
    if (!validate_record_submission(redacted, errs)) {
        let r = "not submission-grade: {errs[0]}"
        delete errs
        return r
    }
    delete errs
    var status = 0
    let body = http_post_body("{base_url}/api/submit/records", redacted, 30, status)
    if (status == 0) {
        return "{base_url} is unreachable"
    }
    if (status != 200) {
        var jerr = ""
        var doc = read_json(body, jerr)
        let e : string = doc?["error"] ?? "http {status}"
        let r = "{e}"
        delete_json(doc)
        return r
    }
    return ""
}

//! Submit this box's CURRENT sidecar (privacy-stripped). Refuses anything not measured here:
//! exchange-sourced files and foreign boxes never go back up. "" on success, else the reason.
def exchange_submit_current(policy : ExchangePolicy) : string {
    let manifest = tune_manifest_path()
    let text = fread(manifest)
    if (empty(text)) {
        return "no sidecar at {manifest}"
    }
    var info : SidecarInfo
    if (!parse_sidecar_info(text, info)) {
        return "sidecar is malformed"
    }
    if (box_match_key(info.box) != box_match_key(tune_box_identity())) {
        return "sidecar was not measured on this box"
    }
    if (sidecar_origin(text) == "exchange") {
        return "sidecar came from the exchange - nothing new to share"
    }
    let stripped = exchange_strip_private(text)
    if (empty(stripped)) {
        return "sidecar is malformed"
    }
    var ladder_sha = ""
    let err = exchange_submit_sidecar_doc(policy.url, stripped, ladder_sha)
    if (!empty(err)) {
        return err
    }
    state_write_submitted(manifest, sha256_hex(text), ladder_sha)
    sc_event("submitted", "sha={ladder_sha}")
    return ""
}

//! Arm a local re-tune: the next boot skips the exchange once and races this box (~12 min,
//! quiet machine). The marker is written FIRST and only then is the sidecar removed — a failed
//! marker write leaves the sidecar intact and returns the error, so the box is never left with
//! no sidecar AND no re-tune armed (which would silently fall back to the exchange). "" on success.
def exchange_request_retune() : string {
    let manifest = tune_manifest_path()
    if (!fwrite("{manifest}.retune", "retune requested\n")) {
        return "could not arm the re-tune (marker write failed) - the current sidecar is unchanged"
    }
    remove(manifest)
    return ""
}

// ===== the control-page surface =====

//! Policy + the current sidecar's identity and share state, one JSON document. No network —
//! the matches list is a separate, explicitly-requested call (exchange_matches_json).
def exchange_status_json(policy : ExchangePolicy) : string {
    let manifest = tune_manifest_path()
    let text = fread(manifest)
    var cur_tab : table<string; JsonValue?>
    if (empty(text)) {
        cur_tab |> insert("present", JV(false))
    } else {
        var info : SidecarInfo
        let parsed = parse_sidecar_info(text, info)
        cur_tab |> insert("present", JV(true))
        cur_tab |> insert("stale", JV(tune_sidecar_stale(manifest)))
        cur_tab |> insert("sha", JV(sha256_hex(text)))
        cur_tab |> insert("box", JV(info.box))
        cur_tab |> insert("engine_sha", JV(info.engine_sha))
        cur_tab |> insert("written", JV(info.written))
        cur_tab |> insert("noise", JV(info.noise))
        cur_tab |> insert("kernel_count", JV(int64(info.kernel_count)))
        cur_tab |> insert("dasllama_version", JV(int64(info.dasllama_version)))
        var jerr = ""
        var doc = read_json(text, jerr)
        if (doc != null) {
            cur_tab |> insert("origin", JV("{doc?["provenance"]?["origin"] ?? ""}"))
            cur_tab |> insert("exchange_sha", JV("{doc?["provenance"]?["exchange_sha"] ?? ""}"))
            cur_tab |> insert("applied_box", JV("{doc?["provenance"]?["applied_box"] ?? ""}"))
            delete_json(doc)
        }
        cur_tab |> insert("shared", JV(state_submitted_for(manifest) == sha256_hex(text)))
        cur_tab |> insert("local", JV(parsed && box_match_key(info.box) == box_match_key(tune_box_identity()) && sidecar_origin(text) != "exchange"))
        cur_tab |> insert("version_ok", JV(info.dasllama_version == DASLLAMA_RELEASE))
    }
    var top <- {
        "policy" => JV((url = policy.url, accept = policy.accept, submit = policy.submit,
            configured = policy.configured)),
        "consent" => JV(exchange_consent_status()),
        "consent_notice" => JV(EXCHANGE_CONSENT_NOTICE),
        "engine_version" => JV(int64(DASLLAMA_RELEASE)),
        "box" => JV(tune_box_identity()),
        "manifest" => JV(manifest),
        "current" => JV(cur_tab)
    }
    var docj = JV(top)
    let out = write_json_compact(docj)
    delete_json(docj)
    return out
}

//! The on-demand matches list for the control page — a NETWORK call (seconds; the caller
//! owns when that is acceptable). `{reachable, version, hits:[...]}` in the ladder's order.
def exchange_matches_json(policy : ExchangePolicy) : string {
    var hits : array<ExchangeHit>
    let ok = exchange_lookup(policy.url, DASLLAMA_RELEASE, tune_box_identity(), hits)
    var arr : array<JsonValue?>
    arr |> reserve(length(hits))
    for (h in hits) {
        arr |> push(JV((sha = h.sha, box = h.box, tier = h.tier, verified = h.verified,
            noise = h.noise, written = h.written, kernel_count = int64(h.kernel_count),
            engine_sha = h.engine_sha, dasllama_version = int64(h.dasllama_version))))
    }
    var doc = JV((reachable = ok, version = int64(DASLLAMA_RELEASE), hits = JV(arr)))
    let out = write_json_compact(doc)
    delete_json(doc)
    delete hits
    return out
}

//! The boot-time submit check (call once the app is up): a fresh LOCAL tune that has not been
//! shared surfaces per policy — `always` submits now, `ask` emits the pending offer for the
//! watchdog balloon + control page, `never` stays silent. Quiet on every non-candidate.
def exchange_boot_submit_check(policy : ExchangePolicy) {
    if (policy.submit == "never") {
        return
    }
    let manifest = tune_manifest_path()
    let text = fread(manifest)
    var info : SidecarInfo
    if (empty(text) || tune_sidecar_stale(manifest) || !parse_sidecar_info(text, info)
            || box_match_key(info.box) != box_match_key(tune_box_identity())
            || sidecar_origin(text) == "exchange") {
        return
    }
    if (info.dasllama_version != DASLLAMA_RELEASE) {
        // the release keys EXCHANGE validity only (the local rail keys on mtime + box) — say why
        // this box's sidecar quietly stops being offered after a release bump
        to_log(LOG_INFO, "dasllama exchange: local sidecar is v{info.dasllama_version}, engine is v{DASLLAMA_RELEASE} - not offered for submission (re-tune to publish)\n")
        return
    }
    let raw_sha = sha256_hex(text)
    if (state_submitted_for(manifest) == raw_sha) {
        return
    }
    // submit=always out of an authoritative toml is not consent either (the page persists it
    // on every save) - without a recorded choice the auto-share degrades to the ask offer
    if (policy.submit == "always" && (policy.configured || exchange_consent_status() == "accepted")) {
        let err = exchange_submit_current(policy)
        if (empty(err)) {
            print("exchange: shared this box's tune (exchange_submit=always)\n")
        } else {
            print("exchange: could not share this box's tune: {err}\n")
        }
    } else {
        sc_event("pending_submit", "sha={slice(raw_sha, 0, 12)}")
        print("exchange: this box's tune has not been shared - the control page can submit it\n")
    }
}
