# Benchmark follow-ups

Compiled-tier and runtime findings from the dasProfile records that are not being acted on in
the arc that found them. Numbers are per-call medians from the budgeted-batch records of
2026-08-30 (3990X cold, M1 Max); the interpreter's own list is `interpreter_followups.md`.

## mandelbrot on the JIT and AOT lanes

3990X: C++ 304 us, .NET 311 us, das JIT 449 us, das AOT 632 us. M1: .NET 380 us, das JIT
463 us, das AOT 565 us, C++ 566 us. The interpreter wins its board on both boxes (3.26 ms
against Quirrel's 8.9 ms), but the compiled lanes trail C++ by 1.5x (JIT) and 2.1x (AOT) on
the zen2 box on a tight float loop. Candidates: the AOT's scalar float math going through the
vec4f `SimPolicy<float>` shape (the same path the exp-loop fix touched), and what the JIT's
loop vectorizer makes of the inner iteration.

## float2string

3990X: Luau `--codegen` 2.53 ms, .NET 3.56 ms, das AOT 3.64 ms, das JIT 3.69 ms, LuaJIT
6.33 ms, C++ 10.2 ms. M1: das JIT 1.64 ms, das AOT 1.82 ms, Luau `--codegen` 2.06 ms, .NET
2.62 ms, C++ 5.9 ms. Luau's number formatting beats das by 1.4x on zen2 while das wins on the
M1, and the C++ reference lane is the slowest compiled lane on both - the float-to-string
path (the formatter behind `string(float)` and `"{f}"`, and the C++ twin's `snprintf`) wants a
look on its own: which algorithm, what precision contract, and why the two boxes disagree.
