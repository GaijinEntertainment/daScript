# Platform services

## WebAssembly monotonic ticks {#wasm-monotonic-ticks}

On Emscripten, `ref_time_ticks` obtains floating-point milliseconds from
`emscripten_get_now` and rounds their nanosecond conversion inside WebAssembly.
This avoids the integer WASI clock bridge's JavaScript BigInt allocations and
diagnostic builds' eager check-message formatting on every clock query.
