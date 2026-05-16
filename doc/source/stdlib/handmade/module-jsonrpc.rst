The JSON-RPC module is a transport-agnostic JSON-RPC 2.0 implementation
(https://www.jsonrpc.org/specification). It provides envelope builders,
request/response parsers, optional §6 batch handling, and a high-level
``dispatch_line`` convenience for stdio-style servers.

See :ref:`tutorial_jsonrpc_request_response` for a hands-on tutorial.

All functions and symbols are in the ``jsonrpc`` module, use require to get access to it:

.. code-block:: das

    require daslib/jsonrpc

The library is permissive by default — the ``jsonrpc`` request member may
be absent or carry a value other than ``"2.0"`` (real-world clients are
lazy about it). Pass ``strict=true`` to ``parse_request`` / ``parse_batch``
/ ``dispatch_line`` for full §4 compliance.

Standard error codes are exposed as ``let public`` constants:

============================ =========================================
Constant                     Code
============================ =========================================
``PARSE_ERROR``              ``-32700`` Invalid JSON received
``INVALID_REQUEST``          ``-32600`` Not a valid request object
``METHOD_NOT_FOUND``         ``-32601`` Method does not exist
``INVALID_PARAMS``           ``-32602`` Invalid method params
``INTERNAL_ERROR``           ``-32603`` Internal JSON-RPC error
============================ =========================================

Server-side example (one-shot):

.. code-block:: das

    require daslib/jsonrpc

    [export]
    def main() {
        let wire = "\{\"id\":1,\"method\":\"ping\"}"
        let response = jsonrpc::dispatch_line(wire, false) $(method, params_json) {
            if (method == "ping") return "\"pong\""
            return "\"unknown\""
        }
        print("{response}\n")
        // → {"jsonrpc":"2.0","id":1,"result":"pong"}
    }

Client-side example (build + parse):

.. code-block:: das

    require daslib/jsonrpc

    [export]
    def main() {
        let wire = make_request("echo", "[1,2,3]", 7)
        // → {"jsonrpc":"2.0","id":7,"method":"echo","params":[1,2,3]}
        let reply_wire = "\{\"jsonrpc\":\"2.0\",\"id\":7,\"result\":[1,2,3]}"
        let r = parse_response(reply_wire)
        if (r.is_success) print("got result for id={r.id_str}\n")
    }

§6 batch — multiple messages in one wire payload:

.. code-block:: das

    let entries <- [
        make_request("a", "null", 1),
        make_notification("log", "\{\"msg\":\"hi\"}"),
        make_request("b", "null", 2)
    ]
    let batch = make_batch(entries)
    let response = jsonrpc::dispatch_line(batch, false) $(method, params_json) {
        return "\"ok-{method}\""
    }
    // response is a JSON array with two entries (notification suppressed).

The ``compact_json_whitespace`` helper flattens pretty-printed
``write_json`` output to a single line — useful when piping
``daslib/json`` output through a newline-framed JSON-RPC transport.

In-tree consumers include
:doc:`/reference/utils/daslang_live` (stdio JSON-RPC transport for live
commands) and the daslang MCP server (``utils/mcp/protocol.das``). For a
complete pedagogical client+server pair, see ``examples/mcp/echo/``.
