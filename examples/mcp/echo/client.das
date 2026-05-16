options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Companion to server.das — demonstrates the client side of daslib/jsonrpc.
//
// Two modes (daslang script-host uses `--` to forward flags):
//   daslang client.das -- --mode=emit    print sample requests to stdout
//   daslang client.das -- --mode=demo    self-contained build + parse demo
//   daslang client.das                   defaults to demo
//
// Pipe through the echo server for a real round-trip:
//   daslang client.das -- --mode=emit | daslang server.das

require daslib/jsonrpc
require daslib/clargs

enum Mode {
    Demo
    Emit
}

[CommandLineArgs]
struct Config {
    @clarg_short = "m"
    @clarg_doc = "Output mode: Demo (build + parse inline) or Emit (print requests to stdout)"
    mode : Mode

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def emit_requests() {
    let echo_req = make_request("echo", "[\"hello\"]", 1)
    print("{echo_req}\n")
    let add_req = make_request("add", "\{\"a\":2,\"b\":3\}", 2)
    print("{add_req}\n")
    // Notification — no id; server runs the method but emits no response.
    let status_notif = make_notification("status", "null")
    print("{status_notif}\n")
    // §6 batch: two requests + one notification.
    let entries <- [
        make_request("status", "null", 10),
        make_notification("log", "\{\"msg\":\"client says hi\"}"),
        make_request("add", "\{\"a\":40,\"b\":2\}", 11)
    ]
    let batch_wire = make_batch(entries)
    print("{batch_wire}\n")
}

def demo() {
    // === Client side: build outgoing wires ===
    print("=== Building requests ===\n")
    let req_echo = make_request("echo", "[\"hello\"]", 1)
    let req_add = make_request("add", "\{\"a\":2,\"b\":3\}", "call-2")
    let notif = make_notification("status", "null")
    let batch <- [
        make_request("status", "null", 10),
        make_request("add", "\{\"a\":40,\"b\":2\}", 11)
    ]
    let req_batch = make_batch(batch)
    print("  single (int id):    {req_echo}\n")
    print("  single (string id): {req_add}\n")
    print("  notification:       {notif}\n")
    print("  batch:              {req_batch}\n")

    // === Server side: parse + dispatch (inline, no subprocess) ===
    print("\n=== Server parses + dispatches ===\n")
    let resp = jsonrpc::dispatch_line(req_echo, false) $(method, params_json) {
        if (method == "echo") return jsonrpc::compact_json_whitespace(params_json)
        return "\"unknown\""
    }
    print("  echo response wire: {resp}\n")

    // === Client side: parse incoming responses ===
    print("\n=== Parsing responses ===\n")
    let r = parse_response(resp)
    print("  is_success={r.is_success}, id={r.id_str}, result={r.result_json |> compact_json_whitespace()}\n")

    // Error response parse
    let err_wire = error("\"call-2\"", METHOD_NOT_FOUND, "method not found")
    let er = parse_response(err_wire)
    print("  is_success={er.is_success}, error_code={er.error_code}, error_msg={er.error_msg}\n")

    // Batch response parse
    let batch_resp = "[\{\"jsonrpc\":\"2.0\",\"id\":10,\"result\":\"ok\"\},\{\"jsonrpc\":\"2.0\",\"id\":11,\"result\":42\}]"
    let rb = parse_response_batch(batch_resp)
    print("  batch is_batch={rb.is_batch}, count={length(rb.responses)}\n")
    for (entry in rb.responses) {
        print("    id={entry.id_str}, success={entry.is_success}, result={entry.result_json}\n")
    }
}

[export]
def main() {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "client")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "client")
        return
    }
    if (cfg.mode == Mode.Emit) {
        emit_requests()
    } else {
        demo()
    }
}
