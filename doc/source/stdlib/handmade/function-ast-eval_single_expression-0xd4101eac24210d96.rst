Simulate and evaluate a single AST expression against the supplied runtime context, returning the result as a `float4` and setting `ok` to indicate success.

Unlike the 2-argument overload, this one uses the caller's context for both the SimNode allocation and the eval, so the expression can reference any global, function, or shared state that `ctx` was simulated with. Typical use: after `compile_file` + `simulate(program)`, evaluate AST fragments (struct field defaults, annotation expressions) extracted from the compiled program.

Saves and restores `ctx`'s exception and stop state so a panic from the evaluated expression does not leak into the caller's eval flow — `ok=false` is the only signal of failure. The function is marked unsafe.
