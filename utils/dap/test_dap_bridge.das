options gen2
options no_aot
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/fio
require daslib/command_line
require daslib/json
require daslib/json_boost
require daslib/jsonrpc
require daslib/network
require daslib/strings_boost
require strings
require dap_bridge

//! The DAP MCP bridge end to end: the bridge over a stdio pipe launching, attaching to and
//! disconnecting from real debuggees, once with the instrumentation debugger and once with the
//! stepping debugger; the frame parser's limits; the resume-command mapping; and the debugger
//! fixtures that must run clean on their own.

def private das_exe() : string {
    return get_full_file_name(get_das_exe())
}

def private root() : string {
    return get_full_file_name(get_das_root())
}

def private fixture(name : string) : string {
    return path_join(root(), "utils/dap/{name}")
}

def private source_line(path, needle : string) : int {
    var line = 0
    for (text in split(fread(path), "\n")) {
        line++
        if (find(text, needle) >= 0) return line
    }
    return -1
}

def private free_port() : int {
    return probe_local_port("127.0.0.1", 0)
}

// ===== the MCP client over the pipe =====

struct private Mcp {
    w : file
    r : file
    next_id : int = 1
}

def private read_line(r : file) : string {
    return build_string() $(var wr) {
        while (true) {
            let chunk = fgets(r)
            let len = length(chunk)
            break if (len == 0)
            if (character_at(chunk, len - 1) == '\n') { // nolint:PERF003
                write(wr, rtrim(chunk, "\r\n"))
                break
            }
            write(wr, chunk)
        }
    }
}

//! one JSON-RPC request; the `result` node, or null with the failure on `t`
def private request(t : T?; var m : Mcp&; method, params_json : string) : JsonValue? {
    let id = m.next_id
    m.next_id++
    let params = empty(params_json) ? "" : ",\"params\":{params_json}"
    fprint(m.w, "\{\"jsonrpc\":\"2.0\",\"id\":{id},\"method\":\"{method}\"{params}\}\n")
    fflush(m.w)
    let line = read_line(m.r)
    if (empty(line)) {
        t |> failure("the bridge closed its stdout while handling {method}")
        return null
    }
    var err : string
    var response = read_json(line, err)
    if (response == null) {
        t |> failure("the bridge answered {method} with something that is not JSON: {err}")
        return null
    }
    t |> equal(response?["id"] ?? -1, id, "the response carries the request id")
    if (response?["error"] != null && !((response?["error"]).value is _null)) {
        t |> failure("{method} failed: {write_json_compact(response?["error"])}")
        delete_json(response)
        return null
    }
    var result = json_copy(response?["result"])
    delete_json(response)
    return result
}

//! a tool call that must succeed: the JSON its text carries
def private tool(t : T?; var m : Mcp&; name, args_json : string) : JsonValue? {
    var result = request(t, m, "tools/call", "\{\"name\":\"{name}\",\"arguments\":{args_json}\}")
    if (result == null) return null
    let is_error = result?["isError"] ?? false
    let text = result?["content"]?[0]?["text"] ?? ""
    t |> success(!is_error, "{name} succeeds: {text}")
    var err : string
    var value = read_json(text, err)
    delete_json(result)
    return value
}

//! a tool call that must fail: its error text
def private tool_error(t : T?; var m : Mcp&; name, args_json : string) : string {
    var result = request(t, m, "tools/call", "\{\"name\":\"{name}\",\"arguments\":{args_json}\}")
    if (result == null) return ""
    let is_error = result?["isError"] ?? false
    let text = result?["content"]?[0]?["text"] ?? ""
    t |> success(is_error, "{name} fails: {text}")
    delete_json(result)
    return text
}

def private count_of(v : JsonValue?) : int {
    if (v == null || !(v.value is _array)) return -1
    return length(v.value as _array)
}

def private has_line(lines : JsonValue?; needle : string) : bool {
    if (lines == null || !(lines.value is _array)) return false
    for (l in lines.value as _array) {
        let text : string = l ?? ""
        if (find(text, needle) >= 0) return true
    }
    return false
}

def private stepping_args(stepping : bool) : string {
    return stepping ? ",\"stepping_debugger\":true" : ""
}

def private launch_external(port : int; stepping : bool) : process {
    var argv <- [das_exe(), "--das-wait-debugger", fixture("_fixture.das"), "--", "--das-debug-port", "{port}"]
    if (stepping) {
        argv |> push("--das-stepping-debugger")
    }
    let noenv : array<string>
    return unsafe(spawn_process(argv, root(), noenv))
}

//! the debuggee `wait_owned` parks on a port the test holds, so a launch on it must refuse
class private PortHolder : Server {
    def PortHolder() {
        super()
        make_server_adapter()
    }
    def override onConnect() {
        pass
    }
    def override onDisconnect() {
        pass
    }
    def override onData(buf : uint8?; size : int) {
        pass
    }
    def override onError(msg : string; code : int) {
        pass
    }
    def override onLog(msg : string) {
        pass
    }
}

// ===== the fixtures on their own =====

def private run_fixture(name : string; args : array<string>; var out : string&) : int {
    var argv <- [das_exe(), fixture(name)]
    if (!empty(args)) {
        argv |> push("--")
        argv |> push_from(args)
    }
    return run_and_capture(argv, out, 30.0)
}

[test]
def test_debugger_fixtures(t : T?) {
    t |> run("a waiting debugger worker is cancelled at shutdown") @(t : T?) {
        var out : string
        t |> equal(run_fixture("_fixture_cancel.das", [], out), 0, out)
        t |> success(find(out, "DAP_CANCEL_READY") >= 0, out)
        t |> success(find(out, "cancelled debugger worker ran") < 0, out)
    }
    t |> run("a debugger callback runs without the thread lock") @(t : T?) {
        var out : string
        t |> equal(run_fixture("_fixture_callback_no_threadlock.das", [], out), 0, out)
        t |> success(find(out, "DAP_CALLBACK_NO_THREADLOCK=1") >= 0, out)
    }
    t |> run("a waiting worker survives its context being destroyed") @(t : T?) {
        let dir = create_temp_directory_result("das-dap-destroy-")
        if (!(dir is value)) {
            t |> failure("no temp directory: {dir as error}")
            return
        }
        var out : string
        t |> equal(run_fixture("_fixture_context_destroyed.das", [path_join(dir as value, "done")], out), 0, out)
        t |> success(find(out, "destroyed context worker ran") < 0, out)
        t |> success(find(out, "DAP_CONTEXT_DESTROY_RECOVERED=1") >= 0, out)
        rmdir_rec(dir as value)
    }
    t |> run("a worker starts again after it ended") @(t : T?) {
        let dir = create_temp_directory_result("das-dap-lifecycle-")
        if (!(dir is value)) {
            t |> failure("no temp directory: {dir as error}")
            return
        }
        var out : string
        t |> equal(run_fixture("_fixture_lifecycle.das", [path_join(dir as value, "first"), path_join(dir as value, "second")], out), 0, out)
        t |> success(find(out, "DAP_LIFECYCLE_WORKER=1") >= 0, out)
        t |> success(find(out, "DAP_LIFECYCLE_WORKER=2") >= 0, out)
        t |> success(find(out, "DAP_LIFECYCLE=2") >= 0, out)
        rmdir_rec(dir as value)
    }
    t |> run("a second debugger thread is refused") @(t : T?) {
        var out : string
        t |> success(run_fixture("_fixture_duplicate.das", [], out) != 0, out)
        t |> success(find(out, "debugger thread is already active") >= 0, out)
    }
}

// ===== the pure parts =====

def private repeated(text : string; count : int) : string {
    return build_string() $(var w) {
        for (_i in range(count)) {
            write(w, text)
        }
    }
}

def private bytes_of(text : string) : array<uint8> {
    var out : array<uint8>
    peek_data(text) $(d) {
        out |> reserve(length(d))
        for (b in d) {
            out |> push(b)
        }
    }
    return <- out
}

[test]
def test_dap_frame_limits(t : T?) {
    t |> run("every malformed frame is refused with its reason") @(t : T?) {
        let xs_8192 = repeated("x", 8192)
        let long_line = "X-Long: {xs_8192}\r\n\r\n"
        let xs_8170 = repeated("x", 8170)
        let many_line = "X-Many: {xs_8170}\r\n"
        let many_lines = repeated(many_line, 9)
        let many = "{many_lines}\r\n"
        let cases <- [
            ("Content-Length: 10", "truncated DAP header line"),
            ("Broken\r\n\r\n", "invalid DAP header"),
            ("X-Test: value\r\n\r\n", "missing or invalid DAP Content-Length"),
            ("Content-Length: nope\r\n\r\n", "missing or invalid DAP Content-Length"),
            ("Content-Length: -1\r\n\r\n", "DAP Content-Length is outside"),
            (long_line, "DAP header line is too long"),
            (many, "DAP headers are too large"),
            ("Content-Length: 67108865\r\n\r\n", "DAP Content-Length is outside"),
            ("Content-Length: 4\r\n\r\n\{\}", "truncated DAP payload"),
            ("Content-Length: 1\r\n\r\n1", "DAP payload must be a JSON object")
        ]
        for (c in cases) {
            var parsed = parse_dap_frame(bytes_of(c._0), true)
            t |> success(find(parsed.error, c._1) >= 0, "expected '{c._1}', got '{parsed.error}' for {slice(c._0, 0, 40)}")
            delete_json(parsed.message)
        }
    }
    t |> run("a header line past the cap is refused before its newline, and headers cut by EOF are truncated") @(t : T?) {
        let xs_8193 = repeated("x", 8193)
        var streaming = parse_dap_frame(bytes_of("X-Long: {xs_8193}"), false)
        t |> success(find(streaming.error, "DAP header line is too long") >= 0, streaming.error)
        delete_json(streaming.message)
        var cut = parse_dap_frame(bytes_of("Content-Length: 15\r\n"), true)
        t |> success(find(cut.error, "truncated DAP headers") >= 0, cut.error)
        delete_json(cut.message)
    }
    t |> run("a complete frame parses and an incomplete one waits") @(t : T?) {
        var whole = parse_dap_frame(bytes_of("Content-Length: 15\r\n\r\n\{\"seq\":1,\"a\":2\}"), false)
        t |> equal(whole.error, "")
        t |> equal(whole.consumed, 37)
        t |> equal(whole.message?["seq"] ?? 0, 1)
        delete_json(whole.message)
        let partial = parse_dap_frame(bytes_of("Content-Length: 15\r\n\r\n\{\"seq\""), false)
        t |> equal(partial.error, "")
        t |> equal(partial.consumed, 0)
        t |> success(partial.message == null, "no message yet")
    }
}

[test]
def test_execution_command_mapping(t : T?) {
    t |> run("the resuming tools map onto their DAP commands") @(t : T?) {
        t |> equal(resume_command("debug_continue"), "continue")
        t |> equal(resume_command("debug_step_in"), "stepIn")
        t |> equal(resume_command("debug_step_over"), "next")
        t |> equal(resume_command("debug_step_out"), "stepOut")
        t |> equal(resume_command("debug_pause"), "")
    }
}

// ===== the session =====

def private bridge_argv() : array<string> {
    return [das_exe(), path_join(root(), "utils/dap/main.das"), "--", "--repo-root", root(), "--executable", das_exe()]
}

//! launches `file`, arms `lines`, and returns the line of the first stop
def private first_stop_line(t : T?; var m : Mcp&; file : string; lines : string; optimize, stepping : bool) : int {
    var launched = tool(t, m, "debug_launch", "\{\"file\":{quoted(file)},\"timeout_sec\":20,\"optimize\":{optimize}{stepping_args(stepping)}\}")
    delete_json(launched)
    delete_json(tool(t, m, "debug_set_breakpoints", "\{\"file\":{quoted(file)},\"lines\":[{lines}]\}"))
    delete_json(tool(t, m, "debug_threads", "\{\}"))
    delete_json(tool(t, m, "debug_configuration_done", "\{\}"))
    var stopped = tool(t, m, "debug_wait_event", "\{\"event\":\"stopped\",\"timeout_sec\":20\}")
    t |> equal(stopped?["event"] ?? "", "stopped")
    let thread_id = stopped?["body"]?["threadId"] ?? 0l
    delete_json(stopped)
    var stack = tool(t, m, "debug_stack_trace", "\{\"thread_id\":{thread_id},\"levels\":1\}")
    let line = stack?["body"]?["stackFrames"]?[0]?["line"] ?? -1
    delete_json(stack)
    delete_json(tool(t, m, "debug_terminate", "\{\}"))
    delete_json(tool(t, m, "debug_wait_event", "\{\"event\":\"terminated\",\"timeout_sec\":20\}"))
    return line
}

def private check_tool_list(t : T?; var m : Mcp&) {
    var listed = request(t, m, "tools/list", "")
    var names : array<string>
    let tools = listed?["tools"]
    if (tools != null && tools.value is _array) {
        names |> reserve(length(tools.value as _array))
        for (tl in tools.value as _array) {
            names |> push(tl?["name"] ?? "")
        }
    }
    let expected <- ["debug_connect", "debug_initialize", "debug_launch", "debug_attach", "debug_set_breakpoints", "debug_data_breakpoint_info", "debug_set_data_breakpoints", "debug_configuration_done", "debug_threads", "debug_stack_trace", "debug_scopes", "debug_variables", "debug_evaluate", "debug_continue", "debug_pause", "debug_step_in", "debug_step_over", "debug_step_out", "debug_terminate", "debug_disconnect", "debug_wait_event"]
    t |> equal(join(names, ","), join(expected, ","), "the tool list, in order")
    let launch = tools?[2]
    t |> equal(launch?["inputSchema"]?["required"]?[0] ?? "", "file", "the schema is the verbatim one: launch requires file")
    t |> equal(launch?["inputSchema"]?["additionalProperties"] ?? true, false, "and closes its property set")
    t |> equal(launch?["inputSchema"]?["properties"]?["port"]?["maximum"] ?? 0, 65535, "with the port bounds")
    delete_json(listed)
}

//! the arguments every tool refuses, before any session exists
def private check_argument_rejections(t : T?; var m : Mcp&; file : string) {
    var bare = request(t, m, "tools/call", "\{\"name\":\"debug_threads\"\}")
    let bare_text : string = bare?["content"]?[0]?["text"] ?? ""
    t |> success(find(bare_text, "not connected") >= 0, "a call with no arguments reaches the tool: {bare_text}")
    delete_json(bare)
    let cases <- [
        ("debug_stack_trace", "\{\"thread_id\":0\}", "thread_id must be an integer >= 1"),
        ("debug_set_breakpoints", "\{\"file\":{quoted(file)},\"lines\":[0]\}", "lines must be an array of positive integers"),
        ("debug_set_breakpoints", "\{\"file\":\"/no/such/file.das\",\"lines\":[]\}", "path does not exist"),
        ("debug_wait_event", "\{\"event\":5\}", "event must be a string"),
        ("debug_connect", "\{\"port\":70000\}", "port must be an integer from 1 to 65535"),
        ("debug_evaluate", "\{\"expression\":\"\",\"frame_id\":1\}", "expression must be a non-empty string"),
        ("debug_variables", "\{\"variables_reference\":1,\"count\":0\}", "count must be an integer >= 1"),
        ("debug_launch", "\{\"file\":{quoted(file)},\"load_modules\":[1]\}", "load_modules must be an array of strings"),
        ("debug_launch", "\{\"file\":{quoted(file)},\"cwd\":\"/no/such/dir\"\}", "path does not exist"),
        ("debug_connect", "\{\"host\":\"192.0.2.1\",\"port\":9,\"timeout_sec\":0.3\}", "timed out connecting"),
        ("debug_step_in", "\{\"thread_id\":1\}", "not connected")
    ]
    for (c in cases) {
        let text = tool_error(t, m, c._0, c._1)
        t |> success(find(text, c._2) >= 0, "{c._0} {c._1}: expected '{c._2}', got '{text}'")
    }
}

//! the paused-program half of the session: scopes, variables, evaluation, data breakpoints,
//! pause, and the three steps
def private inspect_paused(t : T?; var m : Mcp&; file : string; thread_id : int64) {
    var stack = tool(t, m, "debug_stack_trace", "\{\"thread_id\":{thread_id},\"levels\":100\}")
    let frame_id = stack?["body"]?["stackFrames"]?[0]?["id"] ?? 0l
    delete_json(stack)
    t |> success(frame_id > 0l, "a frame id")
    var scopes = tool(t, m, "debug_scopes", "\{\"frame_id\":{frame_id}\}")
    var guard_value = -1
    var guard_scope_ref = 0l
    let scope_list = scopes?["body"]?["scopes"]
    if (scope_list != null && scope_list.value is _array) {
        for (scope in scope_list.value as _array) {
            let scope_name : string = scope?["name"] ?? ""
            continue if (!(scope_name |> starts_with("Locals")))
            let reference = scope?["variablesReference"] ?? 0l
            var variables = tool(t, m, "debug_variables", "\{\"variables_reference\":{reference}\}")
            let vars = variables?["body"]?["variables"]
            if (vars != null && vars.value is _array) {
                for (v in vars.value as _array) {
                    if ((v?["name"] ?? "") == "guard") {
                        guard_value = to_int(v?["value"] ?? "")
                        guard_scope_ref = reference
                    }
                }
            }
            delete_json(variables)
        }
    }
    delete_json(scopes)
    t |> success(guard_value >= 0 && guard_value < 1000, "guard is a small counter: {guard_value}")
    var evaluated = tool(t, m, "debug_evaluate", "\{\"expression\":\"guard\",\"frame_id\":{frame_id}\}")
    t |> success(find(evaluated?["body"]?["result"] ?? "", "{guard_value}") >= 0, "guard evaluates to its value")
    delete_json(evaluated)
    var summed = tool(t, m, "debug_evaluate", "\{\"expression\":\"guard + 1\",\"frame_id\":{frame_id}\}")
    t |> equal(summed?["body"]?["result"] ?? "", "{guard_value + 1}", "an expression takes the debug_eval path")
    t |> equal(summed?["body"]?["type"] ?? "", "int64", "debug_eval widens integer arithmetic")
    delete_json(summed)
    let failed = tool_error(t, m, "debug_evaluate", "\{\"expression\":\"nosuchvar\",\"frame_id\":{frame_id}\}")
    t |> success(find(failed, "unknown variable nosuchvar") >= 0, failed)
    var again = tool(t, m, "debug_evaluate", "\{\"expression\":\"guard\",\"frame_id\":{frame_id}\}")
    t |> success(find(again?["body"]?["result"] ?? "", "{guard_value}") >= 0, "evaluation works after a failure")
    delete_json(again)
    var info = tool(t, m, "debug_data_breakpoint_info", "\{\"variables_reference\":{guard_scope_ref},\"name\":\"guard\"\}")
    let data_id : string = info?["body"]?["dataId"] ?? ""
    t |> success(!empty(data_id), "a data breakpoint id")
    delete_json(info)
    var armed = tool(t, m, "debug_set_data_breakpoints", "\{\"breakpoints\":[\{\"data_id\":{quoted(data_id)}\}]\}")
    t |> equal(count_of(armed?["body"]?["breakpoints"]), 1)
    t |> success((armed?["body"]?["breakpoints"]?[0]?["id"] ?? 0l) != 0l, "the data breakpoint has an id")
    delete_json(armed)
    var cleared = tool(t, m, "debug_set_data_breakpoints", "\{\"breakpoints\":[]\}")
    t |> equal(count_of(cleared?["body"]?["breakpoints"]), 0)
    delete_json(cleared)
    delete_json(tool(t, m, "debug_set_breakpoints", "\{\"file\":{quoted(file)},\"lines\":[]\}"))
    delete_json(tool(t, m, "debug_continue", "\{\"thread_id\":{thread_id}\}"))
    delete_json(tool(t, m, "debug_wait_event", "\{\"event\":\"continued\",\"timeout_sec\":1\}"))
    delete_json(tool(t, m, "debug_pause", "\{\"thread_id\":{thread_id}\}"))
    var paused = tool(t, m, "debug_wait_event", "\{\"event\":\"stopped\",\"timeout_sec\":20\}")
    t |> equal(paused?["body"]?["reason"] ?? "", "pause")
    delete_json(paused)
    delete_json(tool(t, m, "debug_step_in", "\{\"thread_id\":{thread_id}\}"))
    var continued = tool(t, m, "debug_wait_event", "\{\"event\":\"continued\",\"timeout_sec\":1\}")
    t |> equal(continued?["event"] ?? "", "continued")
    delete_json(continued)
    for (step in ["debug_step_over", "debug_step_out"]) {
        var stepped = tool(t, m, "debug_wait_event", "\{\"event\":\"stopped\",\"timeout_sec\":20\}")
        t |> equal(stepped?["body"]?["reason"] ?? "", "step", "after the previous step")
        delete_json(stepped)
        delete_json(tool(t, m, step, "\{\"thread_id\":{thread_id}\}"))
    }
    var last = tool(t, m, "debug_wait_event", "\{\"event\":\"stopped\",\"timeout_sec\":20\}")
    t |> equal(last?["body"]?["reason"] ?? "", "step")
    delete_json(last)
}

//! the whole bridge session in one mode
def private session(t : T?; var m : Mcp&; stepping : bool) {   // nolint:STYLE038 - the scripted session, one arm per protocol step
    let file = fixture("_fixture.das")
    var initialized = request(t, m, "initialize", "\{\"protocolVersion\":\"2025-11-25\",\"capabilities\":\{\},\"clientInfo\":\{\"name\":\"test-dap-bridge\",\"version\":\"0\"\}\}")
    t |> equal(initialized?["serverInfo"]?["name"] ?? "", "daslang-dap")
    t |> success(find(initialized?["instructions"] ?? "", "debug_configuration_done") >= 0, "the instructions ride the initialize result")
    delete_json(initialized)
    check_tool_list(t, m)
    check_argument_rejections(t, m, file)

    var holder = new PortHolder()
    let occupied = free_port()
    t |> success(holder->init(occupied), "the test holds a port")
    let launch_error = tool_error(t, m, "debug_launch", "\{\"file\":{quoted(file)},\"port\":{occupied},\"timeout_sec\":2\}")
    t |> success(find(launch_error, "is unavailable") >= 0, launch_error)
    unsafe {
        delete holder
    }
    let ipv6_error = tool_error(t, m, "debug_launch", "\{\"file\":{quoted(file)},\"host\":\"::1\",\"timeout_sec\":2\}")
    t |> success(find(ipv6_error, "IPv4 loopback") >= 0, ipv6_error)

    let port = free_port()
    var launch = tool(t, m, "debug_launch", "\{\"file\":{quoted(file)},\"port\":{port},\"timeout_sec\":20{stepping_args(stepping)}\}")
    t |> success(launch?["initialize"]?["body"]?["supportsDataBreakpoints"] ?? false, "data breakpoints supported")
    t |> equal(launch?["initialized_event"]?["event"] ?? "", "initialized")
    delete_json(launch)
    var again = tool(t, m, "debug_initialize", "\{\}")
    t |> success(again?["cached"] ?? false, "a second initialize answers from the cached capabilities")
    t |> success(again?["body"]?["supportsDataBreakpoints"] ?? false, "with the same body")
    delete_json(again)

    let early_line = source_line(file, "var result = value + 1")
    let loop_line = source_line(file, "guard++")
    var set = tool(t, m, "debug_set_breakpoints", "\{\"file\":{quoted(file)},\"lines\":[{early_line},{loop_line}]\}")
    let bps = set?["body"]?["breakpoints"]
    t |> equal(count_of(bps), 2)
    if (count_of(bps) > 0) {
        for (bp in bps.value as _array) {
            t |> equal(bp?["verified"] ?? !stepping, stepping, "verified only under the stepping debugger")
        }
    }
    delete_json(set)
    var threads = tool(t, m, "debug_threads", "\{\}")
    t |> success(count_of(threads?["body"]?["threads"]) > 0, "threads listed")
    delete_json(threads)
    var configured = tool(t, m, "debug_configuration_done", "\{\}")
    t |> success(configured?["success"] ?? false, "configurationDone")
    delete_json(configured)
    var stopped = tool(t, m, "debug_wait_event", "\{\"event\":\"stopped\",\"timeout_sec\":20\}")
    t |> equal(stopped?["event"] ?? "", "stopped")
    let thread_id = stopped?["body"]?["threadId"] ?? 0l
    delete_json(stopped)
    var early_stack = tool(t, m, "debug_stack_trace", "\{\"thread_id\":{thread_id},\"levels\":1\}")
    t |> equal(early_stack?["body"]?["stackFrames"]?[0]?["line"] ?? -1, early_line, "the first stop is inside dap_add_one")
    delete_json(early_stack)
    delete_json(tool(t, m, "debug_continue", "\{\"thread_id\":{thread_id}\}"))
    var second = tool(t, m, "debug_wait_event", "\{\"event\":\"stopped\",\"timeout_sec\":20\}")
    t |> equal(second?["body"]?["threadId"] ?? 0l, thread_id)
    delete_json(second)
    var second_stack = tool(t, m, "debug_stack_trace", "\{\"thread_id\":{thread_id},\"levels\":1\}")
    t |> equal(second_stack?["body"]?["stackFrames"]?[0]?["line"] ?? -1, loop_line, "the second stop is the loop")
    delete_json(second_stack)
    let sleep_line = source_line(file, "sleep(1u)")
    var changed = tool(t, m, "debug_set_breakpoints", "\{\"file\":{quoted(file)},\"lines\":[{loop_line},{sleep_line}]\}")
    t |> equal(count_of(changed?["body"]?["breakpoints"]), 2)
    delete_json(changed)
    if (stepping) {
        delete_json(tool(t, m, "debug_terminate", "\{\}"))
        var terminated = tool(t, m, "debug_wait_event", "\{\"event\":\"terminated\",\"timeout_sec\":20\}")
        t |> equal(terminated?["event"] ?? "", "terminated")
        t |> success(terminated?["body"]?["session"]?["return_code"] != null && !((terminated?["body"]?["session"]?["return_code"]).value is _null), "the return code is known")
        t |> success(terminated?["body"]?["exitCode"] != null && !((terminated?["body"]?["exitCode"]).value is _null), "exitCode is carried")
        t |> success(!has_line(terminated?["body"]?["session"]?["process_output_tail"], "locked table"), "no locked-table noise")
        delete_json(terminated)
        return
    }
    inspect_paused(t, m, file, thread_id)
    delete_json(tool(t, m, "debug_continue", "\{\"thread_id\":{thread_id}\}"))
    var terminated = tool(t, m, "debug_wait_event", "\{\"event\":\"terminated\",\"timeout_sec\":20\}")
    t |> equal(terminated?["event"] ?? "", "terminated")
    t |> success(has_line(terminated?["body"]?["session"]?["process_output_tail"], "DAP_RESULT=42"), "the program's output is in the tail")
    var saw_stop = false
    let recent = terminated?["body"]?["session"]?["recent_dap_events"]
    if (recent != null && recent.value is _array) {
        for (e in recent.value as _array) {
            saw_stop = saw_stop || (e?["event"] ?? "") == "stopped"
        }
    }
    t |> success(saw_stop, "the history holds a stop")
    delete_json(terminated)

    attach_and_disconnect(t, m, stepping)
    auto_port_launch(t, m, file, stepping)
    killed_launch(t, m, file, stepping)
    let not_connected = tool_error(t, m, "debug_initialize", "\{\}")
    t |> success(find(not_connected, "not connected") >= 0, not_connected)
    optimize_launch_argument(t, m, stepping)
}

def private attach_and_disconnect(t : T?; var m : Mcp&; stepping : bool) {
    let attach_port = free_port()
    var attached_process = launch_external(attach_port, stepping)
    var connected = tool(t, m, "debug_connect", "\{\"port\":{attach_port},\"timeout_sec\":20\}")
    t |> equal(connected?["port"] ?? 0, attach_port)
    delete_json(connected)
    var dap_initialized = tool(t, m, "debug_initialize", "\{\}")
    t |> success(dap_initialized?["body"]?["supportsConfigurationDoneRequest"] ?? false, "capabilities on initialize")
    delete_json(dap_initialized)
    var attached = tool(t, m, "debug_attach", "\{\"port\":{attach_port},\"cwd\":{quoted(root())},\"timeout_sec\":20\}")
    t |> success(attached?["attach"]?["success"] ?? false, "attached")
    delete_json(attached)
    delete_json(tool(t, m, "debug_threads", "\{\}"))
    delete_json(tool(t, m, "debug_configuration_done", "\{\}"))
    delete_json(tool(t, m, "debug_terminate", "\{\}"))
    var terminated = tool(t, m, "debug_wait_event", "\{\"event\":\"terminated\",\"timeout_sec\":20\}")
    t |> equal(terminated?["event"] ?? "", "terminated")
    delete_json(terminated)
    unsafe(process_wait(attached_process, 10.0))
    unsafe(close_process(attached_process))

    let disconnect_port = free_port()
    var disconnected_process = launch_external(disconnect_port, stepping)
    delete_json(tool(t, m, "debug_connect", "\{\"port\":{disconnect_port},\"timeout_sec\":20\}"))
    delete_json(tool(t, m, "debug_initialize", "\{\}"))
    delete_json(tool(t, m, "debug_attach", "\{\"port\":{disconnect_port},\"cwd\":{quoted(root())},\"timeout_sec\":20\}"))
    delete_json(tool(t, m, "debug_threads", "\{\}"))
    delete_json(tool(t, m, "debug_configuration_done", "\{\}"))
    var disconnected = tool(t, m, "debug_disconnect", "\{\}")
    t |> success(disconnected?["success"] ?? false, "disconnected")
    delete_json(disconnected)
    unsafe(process_wait(disconnected_process, 10.0))
    unsafe(close_process(disconnected_process))
    var again = tool(t, m, "debug_disconnect", "\{\}")
    t |> success(again?["already_disconnected"] ?? false, "a second disconnect says so")
    t |> success((again?["session"]?["return_code"]).value is _null, "no owned process: no return code")
    delete_json(again)
}

def private auto_port_launch(t : T?; var m : Mcp&; file : string; stepping : bool) {
    // the default port is held when it is free, so the launch has to pick another either way
    var holder = new PortHolder()
    holder->init(10000)
    var auto_launch = tool(t, m, "debug_launch", "\{\"file\":{quoted(file)},\"timeout_sec\":20{stepping_args(stepping)}\}")
    let auto_port = auto_launch?["connection"]?["port"] ?? 0
    t |> success(auto_port != 10000 && auto_port > 0, "an automatic port that is not the default: {auto_port}")
    delete_json(auto_launch)
    unsafe {
        delete holder
    }
    let loop_line = source_line(file, "guard++")
    delete_json(tool(t, m, "debug_set_breakpoints", "\{\"file\":{quoted(file)},\"lines\":[{loop_line}]\}"))
    delete_json(tool(t, m, "debug_threads", "\{\}"))
    delete_json(tool(t, m, "debug_configuration_done", "\{\}"))
    var stopped = tool(t, m, "debug_wait_event", "\{\"event\":\"stopped\",\"timeout_sec\":20\}")
    t |> equal(stopped?["event"] ?? "", "stopped")
    delete_json(stopped)
    var disconnected = tool(t, m, "debug_disconnect", "\{\}")
    t |> success(disconnected?["success"] ?? false, "disconnected from a stopped debuggee")
    delete_json(disconnected)
    var again = tool(t, m, "debug_disconnect", "\{\}")
    t |> success(again?["already_disconnected"] ?? false, "already disconnected")
    var exit_polls = 0
    while ((again?["session"]?["return_code"]).value is _null && exit_polls < 20) {
        delete_json(again)
        again = tool(t, m, "debug_disconnect", "\{\}")
        exit_polls++
    }
    t |> equal(again?["session"]?["return_code"] ?? -1, 0, "the debuggee ran to its end after the disconnect")
    t |> success(!has_line(again?["session"]?["process_output_tail"], "locked table"), "no locked-table noise")
    delete_json(again)
}

def private killed_launch(t : T?; var m : Mcp&; file : string; stepping : bool) {
    var launched = tool(t, m, "debug_launch", "\{\"file\":{quoted(file)},\"timeout_sec\":20{stepping_args(stepping)}\}")
    let pid = launched?["pid"] ?? 0
    delete_json(launched)
    t |> success(pid > 0, "a pid")
    if (get_running_platform_name() == "windows") {
        unsafe(system("taskkill /F /T /PID {pid}"))
    } else {
        unsafe(system("kill -9 {pid}"))
    }
    var terminated = tool(t, m, "debug_wait_event", "\{\"event\":\"terminated\",\"timeout_sec\":20\}")
    t |> equal(terminated?["event"] ?? "", "terminated")
    t |> success((terminated?["body"]?["exitCode"] ?? 0) != 0, "a killed debuggee reports a non-zero exit")
    t |> success((terminated?["body"]?["session"]?["return_code"] ?? 0) != 0, "the session return code agrees")
    delete_json(terminated)
    var cleanup = tool(t, m, "debug_disconnect", "\{\}")
    t |> success(cleanup?["already_disconnected"] ?? false, "already disconnected after the kill")
    t |> success((cleanup?["session"]?["return_code"] ?? 0) != 0, "the return code survives the cleanup")
    delete_json(cleanup)
}

//! the debugged program is the optimized one unless `optimize=false`: optimized, the call into
//! dap_add_one folds at compile time and the breakpoint inside never stops; unoptimized it does
def private optimize_launch_argument(t : T?; var m : Mcp&; stepping : bool) {
    let file = fixture("_fixture_optimized.das")
    let callee_line = source_line(file, "var result = value + 1")
    let loop_line = source_line(file, "guard++")
    let lines = "{callee_line},{loop_line}"
    t |> equal(first_stop_line(t, m, file, lines, true, stepping), loop_line, "optimized: the folded call never stops")
    t |> equal(first_stop_line(t, m, file, lines, false, stepping), callee_line, "unoptimized: the call stops")
    let bad = tool_error(t, m, "debug_launch", "\{\"file\":{quoted(file)},\"optimize\":\"yes\"\}")
    t |> success(find(bad, "optimize must be a boolean") >= 0, bad)
}

def private run_session(t : T?; stepping : bool) {
    let argv <- bridge_argv()
    let rc = unsafe(popen_argv_pipe(argv) $(w, r) {
        if (w == null || r == null) {
            t |> failure("could not spawn the bridge: {argv}")
            return
        }
        var m = Mcp(w = w, r = r)
        session(t, m, stepping)
    })
    t |> equal(rc, 0, "the bridge exits 0 when its stdin closes")
}

[test]
def test_bridge_session(t : T?) {
    t |> run("the bridge over a pipe, instrumentation debugger") @(t : T?) {
        run_session(t, false)
    }
    t |> run("the bridge over a pipe, stepping debugger") @(t : T?) {
        run_session(t, true)
    }
}
