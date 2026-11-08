options gen2

require dastest/testing_boost
require daslib/tty

// A test process may run either from a terminal or under a capture, so nothing here asserts
// WHICH answer the probes give — only the invariants that hold in both worlds.

[test(description = "is_stream_terminal is false for stream ids outside stdin, stdout and stderr")]
def test_is_stream_terminal_rejects_out_of_range(t : T?) {
    t |> success(!is_stream_terminal(-1))
    t |> success(!is_stream_terminal(3))
    t |> success(!is_stream_terminal(99))
}

[test(description = "the stdin, stdout and stderr predicates agree with is_stream_terminal")]
def test_named_predicates_agree_with_is_stream_terminal(t : T?) {
    t |> equal(is_stdin_terminal(), is_stream_terminal(TTY_STDIN))
    t |> equal(is_stdout_terminal(), is_stream_terminal(TTY_STDOUT))
    t |> equal(is_stderr_terminal(), is_stream_terminal(TTY_STDERR))
}

[test(description = "terminal_columns returns a positive width even with no terminal")]
def test_terminal_columns_is_always_usable(t : T?) {
    t |> success(terminal_columns() > 0)
    t |> success(terminal_columns(fallback = 77) > 0)
}

[test(description = "a zero fallback reads the raw width, never negative, and a nonzero fallback replaces only a zero reading")]
def test_terminal_columns_raw_probe_is_never_negative(t : T?) {
    let raw = terminal_columns(fallback = 0)
    t |> success(raw >= 0)
    t |> equal(terminal_columns(fallback = 77), raw > 0 ? raw : 77)
}
