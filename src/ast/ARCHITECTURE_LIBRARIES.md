# src/ast architecture notes - library split

Companion to `ARCHITECTURE.md` in this folder.

## The interpreter lives in the compiler library {#simulate-compiler-lib}

`ast_simulate.cpp` (the AST to SimNode tree walk) and the interpreter's node bodies -
`simulate.cpp`, `simulate_visit.cpp`, `simulate_print.cpp`, `simulate_try_catch.cpp`, and
`simulate_container_nodes.cpp` for the array, table and string nodes - link into
the compiler library, because only a simulated program has SimNode trees to run: a standalone exe,
a `-lib` library and a standalone context carry no interpreter. The runtime keeps the node ABI
the rest of the runtime builds on, in `simulate_node_base.cpp`: the `SimNode` base, the call
node every builtin binding instantiates (`SimNode_CallBase`, `SimNode_Op1`/`SimNode_Op2`), the
call nodes `Function::makeSimNode` builds, the nodes the runtime's handled-type templates derive
from (`SimNode_Delete`, `SimNode_Ptr2Ref`), and the block nodes
`SimNode_Final`/`SimNode_Block`/`SimNode_ClosureBlock`, which JIT and AOT blocks derive from. Walks
over an existing tree - `getSemanticHash`, the debugger's instrumentation - stay in the runtime
too: they work on any context.

The runtime never calls into the compiler library. A handled type's `simulateGetAt`,
`simulateGetAtR2V` and `simulateGetIterator` receive their operands as `SimNode *` the compiler
already simulated, and a type whose constant index folds into a fixed offset (the matrix) reports
its row stride and count through `constIndexLayout`, so the fold itself stays in `ast_simulate.cpp`.
`Program::simulate` is defined in the compiler library; a host that links the shared libraries
calls the exported `simulateProgram`, because a member of the runtime's `Program` defined there is
not exported.

An export macro names the library that holds the body: `DAS_CC_API` for the compiler library,
`DAS_API` for a runtime body the compiler half calls. MSVC emits an exported class's vtable in
every translation unit that includes its header, so a `DAS_API` class whose virtuals live in the
compiler library leaves the runtime DLL unlinkable; the interpreter-only node classes in
`simulate.h` and `simulate_nodes.h` are `DAS_CC_API`.

The front end's file classes only the compiler uses link into the compiler library too:
`TextFileInfo` and `ModuleFileAccess` (`ast_file_access.h` / `ast_file_access.cpp`), the AST
serializer and module cache (`module_builtin_ast_serialize.cpp`), and the `!` / `&&` / `||` /
`^^` annotation combinators only the parser and the serializer build (`ast_logic_annotation.cpp`).
The program passes the serializer and the compile pipeline share - stack allocation
(`ast_allocate_stack.cpp`) and symbol marking (`ast_export.cpp`) - link there with them.
The runtime keeps `FileInfo`, which every `LineInfo` points at, and `FileAccess`, which every
`Module` holds as `promotedAccess` and the rtti module binds; neither carries a virtual whose body
lives in the compiler library, and the serializer writes a file record's name and tab size itself.
