options gen2
options persistent_heap
module live_capture shared public
require live/live_commands public
require daslib/fio public
require math
require strings
require daslib/json_boost public

// Opt-in only: capturing never enumerates/invokes arbitrary mutating live commands.
typedef LiveCaptureRead = function<(args : JsonValue?) : JsonValue?>
typedef LiveCaptureValidate = function<(state : JsonValue?; var error : string&) : bool>
typedef LiveCaptureApply = function<(state : JsonValue?) : void>
struct LiveCaptureProvider {
    name : string
    version : int = 1
    read : LiveCaptureRead
    validate : LiveCaptureValidate
    apply : LiveCaptureApply
}
struct LiveCaptureEvent { sequence : int; category, payload : string }
var capture_providers : array<LiveCaptureProvider>
var capture_diagnostics : array<string>
var capture_history : array<LiveCaptureEvent>
var capture_application, capture_build : string
var capture_sequence = 0
var capture_event_sequence = 0
var capture_frame = 0
var capture_pending = false
var capture_mailbox = ""
var capture_request_id = ""
var capture_message = ""
var capture_last_file = ""
var capture_artifact : function<(file : string) : bool>

def live_capture_configure(application, build, mailbox : string) {
    capture_application = application; capture_build = build; capture_mailbox = mailbox
    mkdir_rec(mailbox)
}
def live_capture_provider(provider : LiveCaptureProvider) {
    for (p in capture_providers){if (p.name == provider.name){panic("Duplicate capture provider: {p.name}")}}
    capture_providers |> push(provider)
}
def live_capture_diagnostic(command : string){capture_diagnostics |> push(clone(command))}
def live_capture_event(category, payload : string) {
    if (length(capture_history) >= 256){erase(capture_history, 0)}
    capture_history |> push(LiveCaptureEvent(sequence = capture_event_sequence++, category = category, payload = slice(payload, 0, min(length(payload), 4096))))
}
def private capture_clone(value : JsonValue?) : JsonValue? {
    var error : string; return read_json(write_json(value), error)
}
[live_command(description="Capture all registered live providers and opt-in read-only diagnostics. Does not run unregistered commands.")]
def live_capture(args : JsonValue?) : JsonValue? {
    var providers <- {for (p in capture_providers); p.name => JV((version = p.version, state = invoke(p.read, null)))}
    var diagnostics <- {for (name in capture_diagnostics); name => invoke(live_command_registry[name], null); where key_exists(live_command_registry, name)}
    return JV((schema = "daslang.live.capture/1", application = capture_application, build = capture_build,
        platform = get_running_platform_name(), sequence = capture_sequence++, frame = capture_frame,
        description = args?.description ?? "", providers = JV(providers), diagnostics = JV(diagnostics), history = JV(capture_history)))
}
def private capture_document(args : JsonValue?) : JsonValue? {
    return capture_clone(key_exists(args, "snapshot") ? args?.snapshot : args)
}
def live_capture_validate(document : JsonValue?; var error : string&) : bool {
    if ((document?.schema ?? "") != "daslang.live.capture/1"){error = "Unsupported capture schema"; return false}
    if ((document?.application ?? "") != capture_application){error = "Capture belongs to a different application"; return false}
    if ((document?.build ?? "") != capture_build){error = "Build mismatch: use the capture's matching build"; return false}
    for (p in capture_providers){
        if (p.apply == null){continue}
        let entry = document?.providers?[p.name]
        if (entry == null || (entry?.version ?? 0) != p.version){error = "Missing/incompatible provider: {p.name}"; return false}
        if (p.validate == null || !invoke(p.validate, entry?.state, error)){return false}
    }
    return true
}
[live_command(description="Restore registered logical state from a matching-build capture. Validates all providers before applying any.")]
def live_capture_restore(args : JsonValue?) : JsonValue? {
    var document = capture_document(args)
    var error : string
    if (!live_capture_validate(document, error)){delete_json(document); return JV((ok = false, error = error))}
    for (p in capture_providers){if (p.apply != null){invoke(p.apply, document?.providers?[p.name]?.state)}}
    delete_json(document)
    live_capture_event("restore", "Capture restored")
    return JV((ok = true, build = capture_build))
}
[live_command(description="Inspect a saved live capture file, optionally selecting a captured diagnostic command. No commands are executed.")]
def live_capture_inspect_file(args : JsonValue?) : JsonValue? {
    let file = args?.file ?? ""
    if (empty(file) || !fexist(file)){return JV((ok = false, error = "Capture file not found"))}
    var error : string; var report = read_json(fread(file), error)
    if (report == null){return JV((ok = false, error = error))}
    var doc = capture_document(report)
    let command = args?.command ?? ""
    var result = capture_clone(empty(command) ? report : doc?.diagnostics?[command])
    delete_json(doc); delete_json(report); return result
}
[live_command(description="Restore a saved capture/report file using registered providers; requires matching application and build.")]
def live_capture_restore_file(args : JsonValue?) : JsonValue? {
    let file = args?.file ?? ""
    if (empty(file) || !fexist(file)){return JV((ok = false, error = "Capture file not found"))}
    var error : string; var report = read_json(fread(file), error)
    if (report == null){return JV((ok = false, error = error))}
    var result = live_capture_restore(report); delete_json(report); return result
}
def live_capture_request(){capture_pending = true; capture_request_id = "button-{capture_sequence}"}
def live_capture_poll() {
    let file = "{capture_mailbox}/request.json"
    if (empty(capture_mailbox) || !fexist(file)){return }
    var error : string; var request = read_json(fread(file), error); remove(file)
    capture_request_id = clone(request?.id ?? "")
    let op = request?.operation ?? ""
    if (op == "capture"){capture_pending = true}
    else{
        var result = op == "restore" ? live_capture_restore(request?.report) : JV((ok = false, error = "Unknown capture operation"))
        var reply = JV((id = capture_request_id, operation = op, result = result))
        fwrite("{capture_mailbox}/response.json", write_json(reply)); delete_json(reply)
    }
    delete_json(request)
}
def live_capture_flush(frame : int) {
    capture_frame = frame
    if (!capture_pending){return }
    capture_pending = false
    let png = "{capture_mailbox}/scene.png"
    let image_ok = capture_artifact != null && invoke(capture_artifact, png)
    var document = live_capture(null)
    capture_last_file = "{capture_mailbox}/capture-{capture_sequence}.json"
    fwrite(capture_last_file, write_json(document))
    var reply = JV((id = capture_request_id, operation = "capture", snapshot = document, image = image_ok ? png : "", file = capture_last_file))
    fwrite("{capture_mailbox}/response.json", write_json(reply)); delete_json(reply)
    capture_message = "Captured: {capture_last_file}"
}
