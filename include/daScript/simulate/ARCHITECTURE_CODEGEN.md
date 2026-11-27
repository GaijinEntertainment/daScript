# Simulate headers - the machine code the nodes compile to

A companion of `ARCHITECTURE.md` in this folder. It states where the shape of the compiled
node code depends on the compiler, and what the headers do about it.

## The eval calling convention {#eval-calling-convention}

`DAS_EVAL_ABI` (`misc/platform.h`) is the calling convention of every node's `eval`, of the
`Context` calls that return a `vec4f`, and of `FoldingVisitor::eval`. It is `__vectorcall`
under cl and clang on x64 Windows, and empty everywhere else: Linux, macOS, Windows on ARM,
and 32-bit Windows, where both compilers keep their default.

The System V and the ARM64 conventions return a `vec4f` in a register with no keyword. On x64
Windows the two compilers differ: cl returns a `vec4f` from a member function through a hidden
pointer, and clang returns it in `xmm0`. With the keyword both return it in `xmm0`, so a
module built by one compiler agrees on `eval` with a runtime built by the other, and no result
goes through memory.

cl takes a calling convention only between the return type and the name, so the macro sits
there: `virtual vec4f DAS_EVAL_ABI eval ( Context & )`. An `eval` override that leaves the
macro out differs from its base only by calling convention. It compiles wherever the macro is
empty, Linux and macOS included, and does not compile on x64 Windows.
