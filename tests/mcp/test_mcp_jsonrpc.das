options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost
require daslib/fio
require daslib/json_boost
require daslib/command_line
require daslib/strings_boost
require strings

//! Subprocess-level smoke tests for the MCP server. Spawns
//! ``daslang.exe utils/mcp/main.das`` via ``popen_argv_pipe`` and exercises
//! the JSON-RPC wire — verifies framing, protocol shape, and tool
//! dispatch end-to-end (not just the in-process dispatch table).

let MCP_LIST_MODULES_REQ = "\{\"jsonrpc\":\"2.0\",\"id\":42,\"method\":\"tools/call\",\"params\":\{\"name\":\"list_modules\",\"arguments\":\{}}}"
let MCP_INITIALIZE_REQ   = "\{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\"}"
let MCP_TOOLS_LIST_REQ   = "\{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"tools/list\"}"

def private mcp_script() : string {
    return path_join(get_das_root(), "utils/mcp/main.das")
}

// fgets reads up to one buffer-full (~16KB on most platforms) — large MCP
// responses (e.g. tools/list) exceed that and arrive as multiple chunks.
// Mirror MCP's own read loop: keep appending until we see a trailing '\n'.
def private read_complete_line(stdout_r : FILE const?) : string {
    return build_string() $(var w) {
        while (!feof(stdout_r)) {
            let chunk = fgets(stdout_r)
            break if (empty(chunk))
            var done = false
            var trim = 0
            peek_data(chunk) $(bytes) {
                let n = length(bytes)
                if (n > 0u && bytes[n - 1u] == uint8('\n')) {
                    done = true
                    trim = (n > 1u && bytes[n - 2u] == uint8('\r')) ? 2 : 1
                }
            }
            if (done) {
                // Strip trailing LF (or CRLF) for clean JSON parse.
                write(w, slice(chunk, 0, length(chunk) - trim))
                break
            } else {
                write(w, chunk)
            }
        }
    }
}

// Drive an MCP subprocess through `requests` (one per line), collect the
// response line that matches each request. Returns the exit code.
def run_mcp_session(requests : array<string>; var responses : array<string>) : int {
    let exe = get_das_exe()
    let script = mcp_script()
    var rc : int
    unsafe {
        rc = popen_argv_pipe([exe, script]) $(stdin_w, stdout_r) {
            return if (stdin_w == null || stdout_r == null)
            for (req in requests) {
                fprint(stdin_w, "{req}\n")
                fflush(stdin_w)
                let line = read_complete_line(stdout_r)
                if (!empty(line)) {
                    responses |> push(line)
                }
            }
        }
    }
    return rc
}

def private parse_response(t : T?; line : string) : JsonValue? {
    var err : string
    var v = read_json(line, err)
    if (v == null) {
        t |> failure("response not JSON: {err} (raw: '{line}')")
    }
    return v
}

// JSON integers parse as _longint, doubles as _number. Treat both as
// numeric — tests don't care about the source representation.
def private is_numeric(v : JsonValue?) : bool {
    return v != null && (v.value is _number || v.value is _longint)
}

def private to_int(v : JsonValue?) : int {
    if (v == null) return 0
    if (v.value is _longint) return int(v.value as _longint)
    if (v.value is _number) return int(v.value as _number)
    return 0
}

// ── Tests ────────────────────────────────────────────────────────────

[test]
def test_initialize(t : T?) {
    t |> run("initialize returns result.serverInfo.name='daslang'") @(t : T?) {
        var responses : array<string>
        var reqs : array<string>
        reqs |> push(MCP_INITIALIZE_REQ)
        let exit_code = run_mcp_session(reqs, responses)
        t |> equal(exit_code, 0, "MCP exit code")
        t |> equal(length(responses), 1, "one response")
        return if (empty(responses))
        let r = parse_response(t, responses[0])
        let server_name = r?["result"]?["serverInfo"]?["name"]
        if (server_name == null || !(server_name.value is _string)) {
            t |> failure("missing result.serverInfo.name")
            return
        }
        t |> equal(server_name.value as _string, "daslang")
        // id round-trips
        let id = r?["id"]
        if (is_numeric(id)) {
            t |> equal(to_int(id), 1)
        } else {
            t |> failure("id is not numeric")
        }
    }
}

[test]
def test_tools_list_non_empty(t : T?) {
    t |> run("tools/list returns a non-empty tools array") @(t : T?) {
        var responses : array<string>
        var reqs : array<string>
        reqs |> push(MCP_INITIALIZE_REQ)
        reqs |> push(MCP_TOOLS_LIST_REQ)
        let exit_code = run_mcp_session(reqs, responses)
        t |> equal(exit_code, 0)
        t |> equal(length(responses), 2)
        return if (length(responses) != 2)
        let r = parse_response(t, responses[1])
        let tools = r?["result"]?["tools"]
        if (tools == null || !(tools.value is _array)) {
            t |> failure("missing result.tools array")
            return
        }
        let arr & = unsafe(tools.value as _array)
        t |> success(length(arr) > 0, "expected ≥1 tool, got {length(arr)}")
        // Every tool advertises name + description + inputSchema
        for (tool in arr) {
            let n = tool?["name"]
            let d = tool?["description"]
            let s = tool?["inputSchema"]
            t |> success(n != null && n.value is _string, "tool.name")
            t |> success(d != null && d.value is _string, "tool.description")
            t |> success(s != null && s.value is _object, "tool.inputSchema")
        }
    }
}

[test]
def test_tools_call_list_modules(t : T?) {
    t |> run("tools/call list_modules returns non-empty content") @(t : T?) {
        var responses : array<string>
        var reqs : array<string>
        reqs |> push(MCP_INITIALIZE_REQ)
        reqs |> push(MCP_LIST_MODULES_REQ)
        let exit_code = run_mcp_session(reqs, responses)
        t |> equal(exit_code, 0)
        t |> equal(length(responses), 2)
        return if (length(responses) != 2)
        let r = parse_response(t, responses[1])
        let id = r?["id"]
        if (!is_numeric(id)) {
            t |> failure("missing or non-numeric id")
            return
        }
        t |> equal(to_int(id), 42)
        let content = r?["result"]?["content"]
        if (content == null || !(content.value is _array)) {
            t |> failure("missing result.content array")
            return
        }
        let arr & = unsafe(content.value as _array)
        t |> success(length(arr) >= 1, "expected ≥1 content item")
        // isError must be false (or absent — both are "success")
        let is_err = r?["result"]?["isError"]
        if (is_err != null && is_err.value is _bool) {
            t |> equal(is_err.value as _bool, false, "isError")
        }
    }
}

[test]
def test_method_not_found(t : T?) {
    t |> run("unknown method returns JSON-RPC error") @(t : T?) {
        var responses : array<string>
        var reqs : array<string>
        reqs |> push(MCP_INITIALIZE_REQ)
        reqs |> push("\{\"jsonrpc\":\"2.0\",\"id\":99,\"method\":\"this/does/not/exist\"}")
        let exit_code = run_mcp_session(reqs, responses)
        t |> equal(exit_code, 0)
        t |> equal(length(responses), 2)
        return if (length(responses) != 2)
        let r = parse_response(t, responses[1])
        let err = r?["error"]
        if (err == null || !(err.value is _object)) {
            t |> failure("expected error envelope")
            return
        }
        let code = err?["code"]
        t |> success(is_numeric(code), "error.code")
        if (is_numeric(code)) {
            // jsonrpc::METHOD_NOT_FOUND = -32601
            t |> equal(to_int(code), -32601)
        }
    }
}
