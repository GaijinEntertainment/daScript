options gen2
options no_aot
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/fio
require daslib/json_boost
require daslib/command_line
require strings

//! the stdio front over a pipe, through both hosts where the exe is built: `initialize` and `ping`
//! are answered before any child exists, the first tool call spawns the daslang MCP server, the
//! server's own `shutdown` tool kills it, and the next tool call respawns it unseen

def private das_exe() : string {
    return get_full_file_name(get_das_exe())
}

def private watchdog_exe() : string {
    let beside = path_join(dir_name(das_exe()), "watchdog{get_platform_name() == "windows" ? ".exe" : ""}")
    return fexist(beside) ? beside : ""
}

def private make_temp(t : T?) : string {
    let tmp = create_temp_directory_result("das_stdio_front")
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return ""
    }
    return tmp as value
}

//! the front's argv after the host: the daslang MCP server as the program, eager like .mcp.json
def private front_args(root, log : string) : array<string> {
    return <- ["--stdio", "--name", "wdstdio", "--cwd", root, "--log", log,
               "--program", das_exe(), "--", "-ignore-manifest", "utils/mcp/main.das"]
}

def private write_line(w : file; line : string) {
    fprint(w, line)
    fprint(w, "\n")
    fflush(w)
}

//! one line from the front, however many fgets chunks it spans; empty on EOF
def private read_reply_line(r : file) : string {
    return build_string() $(var w) {
        while (!feof(r)) {
            let chunk = fgets(r)
            let len = length(chunk)
            if (len == 0) break
            write(w, chunk)
            if (character_at(chunk, len - 1) == '\n') break // nolint:PERF003
        }
    }
}

//! one JSON line from the front; null on EOF
def private read_reply(r : file) : JsonValue? {
    let line = read_reply_line(r)
    return null if (empty(line))
    var err : string
    return read_json(line, err)
}

def private tool_text(js : JsonValue?) : string {
    let text = js?["result"]?["content"]?[0]?["text"]
    return text != null && text.value is _string ? text.value as _string : ""
}

def private id_of(js : JsonValue?) : int {
    let id = js?["id"]
    return id != null && id.value is _longint ? int(id.value as _longint) : -1
}

def private count_event(log_path, event : string) : int {
    var count = 0
    fopen(log_path, "rb") $(f) {
        return if (f == null)
        while (!feof(f)) {
            let line = fgets(f)
            break if (empty(line))
            if (find(line, "\"event\": \"{event}\"") >= 0 || find(line, "\"event\":\"{event}\"") >= 0) {
                count++
            }
        }
    }
    return count
}

//! the front is spawned from the temp directory, as an MCP client spawns it from wherever it
//! runs: `--cwd` is what makes the server's relative `utils/mcp/main.das` reach the tree
def private session(t : T?; host : array<string>; root : string) {
    let log = path_join(root, "front.log")
    var argv := host
    argv |> push_from(front_args(get_das_root(), log))
    let here = getcwd()
    t |> success(chdir(root), "entered the temp directory before spawning the front")
    var rc : int
    unsafe {
        rc = popen_argv_pipe(argv) $(w, r) {
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\",\"params\":\{\"protocolVersion\":\"2025-11-25\",\"capabilities\":\{\},\"clientInfo\":\{\"name\":\"dastest\",\"version\":\"0\"\}\}\}")
            var init = read_reply(r)
            t |> success(init != null && id_of(init) == 1, "initialize is answered")
            let version = init?["result"]?["protocolVersion"]
            t |> success(version != null && version.value is _string && (version.value as _string) == "2025-11-25", "the server's protocol version")
            let name = init?["result"]?["serverInfo"]?["name"]
            t |> success(name != null && name.value is _string && (name.value as _string) == "daslang", "the server's name")
            t |> equal(count_event(log, "child_started"), 0, "no child was spawned to answer initialize")
            unsafe {
                delete init
            }
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"method\":\"notifications/initialized\"\}")
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"ping\"\}")
            var pong = read_reply(r)
            t |> success(pong != null && id_of(pong) == 2 && pong?["result"] != null, "ping is answered locally")
            t |> equal(count_event(log, "child_started"), 0, "no child was spawned to answer ping")
            unsafe {
                delete pong
            }
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"tools/call\",\"params\":\{\"name\":\"list_modules\",\"arguments\":\{\}\}\}")
            var listing = read_reply(r)
            t |> success(listing != null && id_of(listing) == 3, "the first tool call is answered")
            t |> success(find(tool_text(listing), "daslib/json") >= 0, "the child served list_modules")
            t |> equal(count_event(log, "child_started"), 1, "the first tool call spawned the child")
            unsafe {
                delete listing
            }
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"tools/list\"\}")
            let catalog_line = read_reply_line(r)
            t |> success(length(catalog_line) > 16384, "the tool catalog is longer than one fgets chunk, so it crossed the front's line reader in pieces")
            var err : string
            var catalog = read_json(catalog_line, err)
            t |> success(catalog != null && id_of(catalog) == 7, "the catalog line is one whole JSON message: {err}")
            let tools = catalog?["result"]?["tools"]
            t |> success(tools != null && tools.value is _array && length(tools.value as _array) > 10, "the child's catalog came through the front")
            unsafe {
                delete catalog
            }
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"id\":4,\"method\":\"tools/call\",\"params\":\{\"name\":\"shutdown\",\"arguments\":\{\}\}\}")
            var gone = read_reply(r)
            t |> success(gone != null && id_of(gone) == 4, "the shutdown call gets an answer, result or death report")
            unsafe {
                delete gone
            }
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"id\":5,\"method\":\"tools/call\",\"params\":\{\"name\":\"list_modules\",\"arguments\":\{\"json\":\"true\"\}\}\}")
            var again = read_reply(r)
            t |> success(again != null && id_of(again) == 5, "the call after the shutdown is answered")
            t |> success(find(tool_text(again), "\"daslib\"") >= 0, "a respawned child served it")
            t |> equal(count_event(log, "child_started"), 2, "the call after the shutdown spawned a second child")
            unsafe {
                delete again
            }
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"id\":6,\"method\":\"resources/list\"\}")
            var refused = read_reply(r)
            t |> success(refused != null && id_of(refused) == 6 && refused?["error"] != null, "a method the server has no answer for is refused without a child")
            unsafe {
                delete refused
            }
        }
    }
    chdir(here)
    t |> equal(rc, 0, "the front exits 0 when the client closes its input")
}

//! the `result` of one `initialize` from a program spawned with `argv`, compact; empty on no reply
def private initialize_result(argv : array<string>) : string {
    var text : string
    unsafe {
        popen_argv_pipe(argv) $(w, r) {
            write_line(w, "\{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\",\"params\":\{\"protocolVersion\":\"2025-11-25\",\"capabilities\":\{\},\"clientInfo\":\{\"name\":\"dastest\",\"version\":\"0\"\}\}\}")
            var reply = read_reply(r)
            let result = reply?["result"]
            text = result != null ? write_json_compact(result) : ""
            unsafe {
                delete reply
            }
        }
    }
    return text
}

[test]
def test_stdio_front(t : T?) {
    t |> run("the front's handshake is the server's") @(t : T?) {
        let root = make_temp(t)
        return if (empty(root))
        let server = initialize_result([das_exe(), "-ignore-manifest", path_join(get_das_root(), "utils/mcp/main.das")])
        var front_argv <- [das_exe(), path_join(get_das_root(), "utils/watchdog/main.das"), "--"]
        front_argv |> push_from(front_args(get_das_root(), path_join(root, "front.log")))
        let front = initialize_result(front_argv)
        t |> success(!empty(server), "the server answered initialize")
        t |> equal(front, server, "the front answers initialize with the server's own result")
    }
    t |> run("through the interpreter host") @(t : T?) {
        let root = make_temp(t)
        return if (empty(root))
        session(t, [das_exe(), path_join(get_das_root(), "utils/watchdog/main.das"), "--"], root)
    }
    t |> run("through the static exe") @(t : T?) {
        let exe = watchdog_exe()
        if (empty(exe)) {
            t |> skip("bin/watchdog is not built here")
            return
        }
        let root = make_temp(t)
        return if (empty(root))
        session(t, [exe], root)
    }
}
