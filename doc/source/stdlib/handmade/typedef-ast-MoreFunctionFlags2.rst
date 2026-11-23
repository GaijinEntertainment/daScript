Overflow word for function flags (moreFlags is full at 32 bits).
Function is a @@{} local function body - generated, but the block is verbatim user code.
Result is always a fresh string allocation (or null), never a passthrough of an input, never retained by the callee.
Executing the body may hit a temp-string queue site (a builder or a wrappable call, transitively) - a caller must not hold a parked temp across a call to this function.
Function may reach a heap collect point (transitively over direct calls) - its caller keeps a real stack frame, so carriers are denied fastcall.
Extern bound via addExternInline - the interpreter call node is per-function (NTTP) and the callee is eligible to inline into it.
Generator whose body had a statement-position unsafe block - inference dissolved the block and visits the whole function with unsafe allowed.
Function never returns to its caller (panic) - a call to it ends every control path, so no return is needed after it.
Function reads its context's function or global RTTI tables at runtime, or calls one that does - a standalone AOT context or JIT exe that uses it keeps those tables.
Function compiles, simulates or invokes other code (compile, require_module_now, invoke_in_context and the like): a builtin is marked where its module registers it, a function compiled from source when its body calls or takes the address of a marked function. Under a parallel compile, the modules that can use the macros of a macro context reaching such a function compile alone.
