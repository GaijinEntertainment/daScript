# libDaScriptNano - architecture

## What nano is

A daslang runtime with no compiler in it. Standalone AOT (`daslang utils/aot/main.das -- -ctx
script.das out/`) turns a script into C++ that an embedder compiles into their program; nano is
what that C++ links against instead of `libDaScript`.

The full runtime is one library because the compiler and the runtime share headers. An embedder
who only wants to run already-compiled code still pays for the AST, the module registry, the
debugger, the serializer, fmt, and the STL those drag in. On a desktop that is a link-time
annoyance. On a microcontroller it is the difference between shipping and not.

nano is not a fork of the runtime. It is the same runtime with a different set of headers in
front of it.

## The mechanism: a shadow include root

`nano/include` goes **before** `include/` in the header search order. A header that exists in
both resolves to nano's; every other header is the upstream file, byte for byte.

That is the whole trick, and it buys the property that matters: the shared sources compile
against nano's `Context` without one `#ifdef`, and regular AOT output keeps its exact shape. A
build with `DAS_CONFIG_INCLUDE_DIR` (`cmake/das_config_eastl/`) already proved the pattern for
`das_config.h` alone; nano widens it to four headers.

### The include-order trap

CMake searches a directory's `include_directories()` **before** any target's own. A target that
links `libDaScriptNano` from a directory that inherited the root's `include_directories(include)`
compiles against the full runtime's `Context` while linking nano's - a mismatch with no
diagnostic, because most of the two agree. Every directory holding a nano-linked target clears
that property:

```cmake
set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")
```

This repo's `nano/REVIEW.das` checks it mechanically, because it is the one mistake in this
design that produces a program rather than an error.

## The include contract

Generated standalone code includes exactly eight `daScript/...` paths, and nothing it emits
widens that set:

```
daScript/misc/platform.h
daScript/simulate/simulate.h
daScript/simulate/aot.h
daScript/simulate/aot_library.h
daScript/simulate/standalone_ctx_utils.h
daScript/simulate/bin_serializer.h      // require $
daScript/simulate/runtime_profile.h     // require $
daScript/misc/performance_time.h        // require $
```

The last three arrive because the `$` builtin module declares them, never because generated code
names a symbol from them. nano shadows all three with empty headers, which is compensation: the
real fix is for a standalone context not to emit those three includes at all, and it lands with
the next change to this folder.

## What nano shadows

`daScript/das_config.h` - the header everything else reaches the standard library through, which
is why replacing it re-points the whole runtime at a smaller set. It drops the STL headers the
runtime does not use, turns `DAS_ENABLE_EXCEPTIONS`, `DAS_DEBUGGER`, `DAS_FUSION`,
`DAS_BIND_EXTERNAL` and the crash handler off, and routes
`das_to_stdout_level_prefix_text` at the embedder's print sink. Those five are `#undef`ed and
redefined rather than defaulted with `#ifndef`, so a build that sets one on the command line
cannot silently get a runtime nano was not built to be - the panic path in particular is
`setjmp`/`longjmp`, not a C++ exception, and there is no second version of it here.

`daScript/simulate/simulate.h` - the `Context` subset standalone AOT actually touches: a stack,
two heaps, the function and global tables, the three mangled-name lookups, and a panic path.
Gone: debug agents, stack walkers, GC roots, job-fork pools, the profiler, JIT hooks,
instrumentation, context cloning, code relocation, the init and shutdown scripts (the generated
constructor runs the init script itself).

`daScript/ast/ast.h` - not the compiler front-end but the handful of names two reused runtime
sources want from it: `Annotation`, `TypeAnnotation`, and a `Module` with a name.

The three `require $` headers above.

## What nano owns

`src/nano_context.cpp` - the `Context` implementation. Every function here has a counterpart in
`src/runtime/context.cpp` or `src/simulate/simulate_exceptions.cpp`.

`src/nano_stubs.cpp` - the seams where nano ends, plus the smart-pointer tracking globals that
`src/misc/globals.cpp` owns upstream (that file also carries the job-queue globals, and
`job_que.h` is threads).

`src/nano_string_writer.cpp` - `daScript/misc/string_writer.h` implemented over `snprintf`. The
header is upstream and unmodified; only fmt had to go, and it was reached through one template.

## What nano reuses verbatim

Thirteen sources compile straight out of `src/`, listed in `nano/CMakeLists.txt`. Adding one is a
decision: it must compile with no edit to the shared tree. When it needs an edit, the fix goes
upstream as a **carve** - splitting the runtime half of a file away from its compiler half - not
into a fork here. `src/simulate/simulate_gc_pod.cpp`, `src/simulate/annotation_arguments.cpp`,
`src/simulate/escape_string.cpp`, `src/simulate/builtin_array_ops.cpp` and
`src/simulate/builtin_runtime_ops.cpp` all exist because of that rule, and every one of them left
the full runtime better factored than it found it.

## Fail-closed seams

A stub that quietly does nothing is worse than no stub: on a board with no debugger, the moment
of the call is the only diagnosis anyone gets. Each of these stops the program instead:

- `SimNode::copyNode`, `SimNode::visit`, `SimNode_CallBase::copyNode`, `SimVisitor::sub` - code
  relocation and node dumping, neither of which nano has.
- `TypeAnnotation::walk` - walking a handled (C++-bound) type needs a registration only the
  compiler makes.
- `makeAotJitNode` - a context generated with JIT nodes was generated for a different runtime.
- `FileInfo::serialize` - nano carries no serializer.
- `getSemanticHash` - nano carries no semantic hashes.

`Context::setup` checks the `CodeOfPolicies` ABI stamp and stops on a mismatch. On nano both
sides are always built together, so a mismatch means the generated code and these headers came
from different daslang builds - the one failure that cannot be detected any later.

Three neighbours of those seams answer instead of stopping, each matching what the full runtime
does when the thing it would consult is absent. `TypeInfo::resolveAnnotation` does nothing,
because resolving means asking a `Module`. `getCancelLimit` returns zero, which upstream means
"no cap" and is what an unbound environment returns there too. `print_current_stack_trace` is
empty; `os_debug_break` traps.

## Panic

Same contract as the full runtime built with `DAS_ENABLE_EXCEPTIONS=0`: fill in the message,
then `longjmp` to whoever armed `throwBuf`, or report and stop. One difference - the message
lands in a fixed 256-byte buffer rather than a `das::string`, because running out of heap is one
of the things that panics, and a longer message is truncated rather than allocated for.

## Tiers

A tier is what a script uses, not a build option: linking simply fails when a script reaches past
what nano carries. `examples/standalone/` has one example per tier, and this repo additionally
links all four into one program as the `nano_ctx` test.

| tier | what it uses | example |
|---|---|---|
| A | POD compute - structs, enums, dims, no heap | `01_pure` |
| B | arrays, tables, `new`/`delete` through the scope-free path | `02_heap` |
| C | lambdas, function pointers, generators - the runtime function tables | `03_closures` |
| output | `print` reaching the embedder's sink | `04_c_binding` |

Above these sits everything `src/simulate/runtime_string.cpp` provides - string interpolation and
the string builders - which nano leaves out because it is where fmt comes back. A script that
uses them fails to link, which is the boundary being honest rather than a program that silently
grew by a hundred kilobytes.

Two builtin modules a host takes for granted are also absent, for the same reason: a script that
calls `max` needs `require math`, and the math module is a registration nano has no compiler to
make.

## Ledgered cases

**nano is not freestanding yet.** It builds where the full runtime builds, on the same
toolchains, and drops the compiler. Cross-compiling it for a bare-metal target does not work
today. What stands in the way, measured against arm-none-eabi with newlib: it has no
`posix_memalign`, `malloc_usable_size` or `madvise`; its libstdc++ is built without threads, so
`<mutex>` declares nothing and neither `smart_ptr.h`'s ref-count lock nor this folder's
`contextMutex` compiles; its `uint32_t` is `unsigned long`, which makes every `BitfieldAny`
conversion and every `vec4<uint32_t>` load ambiguous; and `alloca` needs its own include. Those
sit in `platform.h`, `smart_ptr.h`, `arraytype.h`, `vectypes.h` and `interop.h` - shared headers
every platform compiles - so the port is its own change, not a corner of nano.

**`<mutex>` and `<functional>` are still included.** `smart_ptr.h` declares a `static mutex` for
its ref-count tracking list and `memory_model.h` types its custom-grow hook as `das::function`.
Shadowing those two headers is part of the freestanding port above.

**Floats print differently.** The full runtime formats through fmt, which prints the shortest
round-tripping form; nano prints `%g`. Same value, shorter text. This shows only in log output.

**No stack walk on panic.** The prologue writes stay on, because the generated code's frame sizes
were computed assuming them, but nothing reads them - a panic reports its message and its
`LineInfo`. An unwinder is the obvious thing to add here and nobody has needed it yet.

**No GC.** `simulate_gc_pod.cpp` gives explicit `delete` its scope-free path; there is no
collector behind it. What a script does not delete stays allocated until the context dies. For a
program with a fixed working set - which is what this tier is for - that is the correct trade,
but it is a trade.

## Heap

nano reuses `MemoryModel` and the heap allocators unchanged, so `options heap_size_hint` reaches
`setInitialSize` and the allocator makes one big-path allocation up front. `options
persistent_heap` picks `PersistentHeapAllocator` over the linear one, exactly as on a host.
`options stack = N` is honored exactly - the 16384 floor applies only when a script sets no stack
at all.
