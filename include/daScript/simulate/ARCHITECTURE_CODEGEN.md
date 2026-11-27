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

## A two-arm node keeps its branch {#two-arm-branch}

A two-arm node evaluates a condition and then exactly one of two child nodes:
`SimNode_IfThenElse`, the `IfZeroThenElse` and `IfNotZeroThenElse` nodes, their debugger
twins, and the fused `IfZeroThenElse` and `IfNotZeroThenElse` nodes of
`src/simulate/simulate_fusion_if.cpp`. Both arms end in the same call on a different child.

Clang merges the two calls into one. It picks the child pointer with a data select and makes
one indirect jump, so no conditional branch is left in the node. The target of that one jump
then changes with the condition, and the processor predicts it badly: a recursive function
whose body is an `if` runs more than twice as slow in the interpreter as it does with the
branch kept.

`DAS_NO_MERGE` (`misc/platform.h`) is `[[clang::nomerge]]`, and it sits on the `return` of
each arm. It keeps the two calls apart, and the conditional branch stays with them. GCC and
cl keep the branch without help, and the macro is empty for them. A node with one arm has
nothing to merge and carries no macro.
