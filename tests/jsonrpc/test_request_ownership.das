options gen2
options persistent_heap
options no_unused_block_arguments = false
options no_unused_function_arguments = false
require dastest/testing_boost public
require daslib/jsonrpc
require daslib/json

let SINGLE = "\{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"echo\",\"params\":\{\"a\":[1,2,3],\"b\":\"text\"}}"
let BATCH = "[\{\"id\":1,\"method\":\"a\",\"params\":[1,2]},\{\"method\":\"n\"},\{\"id\":3,\"method\":\"b\"}]"
let MALFORMED = "\{\"id\":1,\"method\":42,\"params\":[1,2]}"

def echo_dispatch(line : string) : string {
    return dispatch_line(line, false) $(method, params_json) {
        return "\{\"m\":\"{method}\"}"
    }
}

[test]
def test_dispatch_line_owns_the_document(t : T?) {
    t |> run("a dispatched single request leaves nothing on the heap") @(t : T?) {
        let before = heap_bytes_allocated()
        for (_ in range(8)) {
            let r = echo_dispatch(SINGLE)
            t |> success(!empty(r), "the request produced a response")
        }
        t |> equal(heap_bytes_allocated(), before, "no parse tree survives dispatch")
    }
    t |> run("a dispatched batch leaves nothing on the heap") @(t : T?) {
        let before = heap_bytes_allocated()
        for (_ in range(8)) {
            let r = echo_dispatch(BATCH)
            t |> success(!empty(r), "the batch produced a response")
        }
        t |> equal(heap_bytes_allocated(), before, "no parse tree survives dispatch")
    }
    t |> run("a malformed request leaves nothing on the heap") @(t : T?) {
        let before = heap_bytes_allocated()
        for (_ in range(8)) {
            let r = echo_dispatch(MALFORMED)
            t |> success(!empty(r), "the error envelope came back")
        }
        t |> equal(heap_bytes_allocated(), before, "no parse tree survives dispatch")
    }
    t |> run("unparseable input leaves nothing on the heap") @(t : T?) {
        let before = heap_bytes_allocated()
        for (_ in range(8)) {
            let r = echo_dispatch("not json at all")
            t |> success(!empty(r), "the framing error came back")
        }
        t |> equal(heap_bytes_allocated(), before, "no parse tree survives dispatch")
    }
}

[test]
def test_response_parsers_own_their_document(t : T?) {
    t |> run("parse_response leaves nothing on the heap") @(t : T?) {
        let before = heap_bytes_allocated()
        for (_ in range(8)) {
            let r = parse_response("\{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":\{\"x\":[1,2,3]}}")
            t |> equal(r.is_success, true)
        }
        t |> equal(heap_bytes_allocated(), before, "no parse tree survives parse_response")
    }
    t |> run("parse_response_batch leaves nothing on the heap") @(t : T?) {
        let before = heap_bytes_allocated()
        for (_ in range(8)) {
            let rb = parse_response_batch("[\{\"id\":1,\"result\":1},\{\"id\":2,\"result\":[1,2]}]")
            t |> equal(length(rb.responses), 2)
        }
        t |> equal(heap_bytes_allocated(), before, "no parse tree survives parse_response_batch")
    }
}
