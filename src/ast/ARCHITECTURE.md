# src/ast - the front end's AST

Contract: `ARCHITECTURE_COMMON.md` (repo root). This document carries the mechanisms of this folder
that a rule cites; `ARCHITECTURE_INFER.md` the inference mechanisms code cites, `ARCHITECTURE_BUILT.md`
host-built programs, `ARCHITECTURE_GC.md` gc roots, `ARCHITECTURE_SHARED.md` shared-module promotion,
`ARCHITECTURE_MODULE_SCAN.md` the module scan and the descriptor manifest, `ARCHITECTURE_INLINE.md`
the inliner's conditional call lowering.
The rest of inference, with parsing and simulation, is in `skills/internal/cpp_codebase_notes.md` (repo-only).

## Returned locals captured by blocks or heap literals {#return-local-storage}

`VarCMRes` can alias a returned local to the caller's result buffer. That storage is
addressed through the current context's `abiCMRES`. A heap-mode array literal
temporarily redirects `abiCMRES` to its element buffer. A block invoked by another
function can run with that function's result buffer; its captured stack frame does
not restore the defining function's `abiCMRES`. Locals referenced inside either
shape therefore retain stack storage and are copied or moved on return. The check
covers both reads and writes, including references nested in another block.

## The module-cache read in `trySerializeProgramModule` (`ast_parse.cpp`) {#module-cache-read}

The front end parses modules in require order. When a module cache is installed
(`ModuleFileCache`), each module first tries `trySerializeProgramModule`. The reader stands at
the next record and checks that the record names this file, that the size and content hash of
the bytes the compile's `FileAccess` serves for that name match (never a stat, so a file that is
not on disk under that name is covered, and identical rewrites serve).
The header also stamps every textual include through that same access. Its
include count is bounded before allocation, and served records retain those
stamps for writeback. A changed or missing include cuts off the stream.
The reader checks that every compile-time input the record's macros pinned
(`add_module_cache_dependency` - a file's bytes, an environment variable's value under
`env:NAME`, a command-line flag's occurrences under `arg:--flag`; and the builtin module's
presence under `module:NAME`, which the `builtin_module_exists` fold pins itself, so a build
that gains or loses a witness module refuses the record a compile folded under the other
answer) still has the same content. A
match deserializes the module and counts it as served; the module object is a `ModuleDas`, the
class the parser builds, because the AOT emitter asks each module whether it can be AOT'd and a
plain `Module` answers no. A served module's file record carries the file's name and length,
not its text; the reader resolves the name through the compile's `FileAccess` - the file system,
`.das_project` hook or in-memory access a parse would have gone through - and every line
reference in the module points at the object the access returns, source included, so a lint
reading a `nolint:` marker or an error printing a source line sees the bytes the access serves.
The module takes its own file from the access the way a parse leaves it (`letGoOfFileInfo`);
a name the access cannot produce keeps the record's source-less object. The record also carries
the `CodeOfPolicies` of the compile that wrote it, and a record whose policies differ from the
reading compile's is never served - a lint compile and a run compile of one graph never share a
record. The mismatch fails the record the way damage does, reparsed in place rather than cutting
the stream (damage landing on the policy bytes must not cut it either), and the writeback that
repairs it carries the new policies. The record header also carries the text collector's
answer for the file when the record was written (`collectRequireNames` over `getAllRequire`,
guards and groups already applied, names sorted), and the reader runs the same collector over the
file it is about to serve and compares: a member a group gained or a guard a build flipped changes
a module's dependencies without touching its bytes, and a record served then would carry the old
dependency set. Both sides are one function, so a require only the walk knows - a host's
`parseCustomRequire` - compares equal to itself instead of cutting the stream on every warm read.
That mismatch is a cutoff (`require set changed`), since the modules after it may have moved too.
The first mismatch is the cutoff: the reader marks the stream failed, the
module and everything after it parse from source, and the writer rewrites the whole file - the
served records re-serialized from the modules the reader restored, then the freshly parsed
ones. A record whose header matched but whose payload fails to deserialize reparses in place,
without cutting the stream, whenever the record carries a usable payload length.
A record that fails to write leaves the compiled program as it is - only a failed read marks the
program failed. The writer warns (gated like the read's lines) and `ModuleFileCache::finish`
writes no file, since the stream stops inside the failed record; the next run finds the cache it
found before.

A served module replays what its own init registered - the macro classes a `[call_macro]` or
`[reader_macro]` annotation adds through the module's init function come back when the record
re-instantiates its macro program - but not a registration made while the module compiled,
into itself or into another module: an annotation's `apply`, running in another module's macro
context, that calls `add_call_macro` (or any other `add_*_macro` / `add_*_annotation`) on
`compiling_module()`; the `[extern]` binder's `apply`, which adds the proxy every call site is
retargeted to into the `dasbind` builtin. The registering site marks the compiling module
(`Module::registersAtCompile` - the adapters set it when the target has no macro context yet,
which is what a module being compiled looks like; the binder sets it on the program's own
module) and the record header carries the mark. A marked record is never served: it reparses in
place, the records after it still serve, and the cache is not rewritten for it; a marked record
whose length word cannot skip it cuts the stream like a changed file. The writer gives a marked
record its header and a one-byte payload in place of the module: nothing reads that payload, and
the module's own would name what only its compile registered - a generic's body is stored
uninferred, so a call macro registered into the module survives there as an `ExprCallMacro`, a
reference the payload cannot carry. Serving it would leave any
module parsed from source against it - after a cutoff in the same read, or in a later compile of
the same process once a `shared` module is promoted - without the registrations, and its call
sites would fail to resolve (or, for a binder, find no proxy).

A builtin module's embedded source (`compileBuiltinModule`, the `unit_test.das` of `UnitTest`)
compiles with the stream hidden: its program is never read from or written to the cache. That
compile runs inside the module's constructor, and every record pins each builtin module it
references by the module's cumulative hash, a sum over the functions, globals and types it
binds. With the stream visible the embedded program would be a record like any other,
written by a cold run and served by a warm one, and the module's hash would then differ between the
two kinds of run - so every record naming the module would fail its read on the other kind, the
entry program's included, and the hash-drift resume would reparse each of them on every start.

Every diagnostic the read prints - a record for another file, a changed file, a changed
dependency, changed policies, a truncated or version-mismatched stream, a failed payload, a
reparse in place - is
gated on the serializer's `quietCache`. The host sets that flag for the default cache, which is
on unasked for an ordinary run; ungated, each of those lines would be output every user sees on
an ordinary edit. An explicit `-module-cache` leaves the flag off, so those runs get the lines
together with the host's verdict.

A served record's time splits three ways: the decode, the finalize (`finalizeModule` -
annotations, macro-module re-simulation, `gc_collect`) and the setup `Program::serialize` runs
after the stream (symbol marking, stack allocation). The serializer accumulates the last two over
every program it read, das reading them back through `ast_serializer_finalize_usec` and
`ast_serializer_setup_usec`, and the decode is the read minus both; `-log-compile-time`'s cache
line prints decode against finalize, with the macro simulate named inside finalize.

## A require after the walk (`requireModuleNow`, `ast_parse.cpp`) {#require-after-walk}

`requireModuleNow(requireName, access, logs, policies)` is a `require` issued by code that runs
after the prerequisite walk - a macro, a simulate macro, an `[init]` - for a module the walk
never saw. It is a compile's: the das entry (`daslib/cross_context`, the `ast` module's
`require_module_now`) refuses a call with no program compiling or simulating, since a running
script has no compile whose stream, policies and access the walk could join, and a null access
is the compiling program's own (`Program::access`, set by `parseDaScriptEx` for a parse and by
`deserialize_program`'s access argument for a program a reader restores - a stream carries no
access, so a reader that gives none leaves a late require nothing to walk with), so a
`-project` mapping or a host's custom access reaches the late walk as it reaches the host's; a
caller restricts by passing an access of its own. It answers the module already in the process when
there is one (a promoted shared module, a linked C++ module, or a deferred manifest row, which
it loads), and otherwise walks the target's own prerequisites, compiles the missing ones the way
`compileDaScript` does - each parsed as a dependency, promoted when its program asks to be
shared, the debugger switching serialization off as it does for the host - and then the target
itself, which must be `shared`: a module that is not promoted lives only in the walk's
`ModuleGroup` and dies with it, so a non-shared target is refused by name rather than returned
dangling. The target is named by its file: the already-there answer looks the module up by the
file's stem before any parse, so a file declaring another name would compile and promote a new
module on every call, and the walk refuses the mismatch by name instead. The answer is the
module or null, and every error the walk, the dependency parses and the target's parse produced
is written to the caller's `logs`, so a caller reports the text it got rather than the compiler's
own log. The module is not a dependency of any program: its symbols are not visible to the
caller and its macros do not apply to the caller's program; the caller reaches it through its
macro context (`Module::macroContext`, `daslib/cross_context`'s `macro_context_of`). Every macro
context carries a context mutex from its creation (`makeMacroModule`), because
`invoke_in_context` locks its target and a mutex created at the first call would be created
twice by two callers. The call saves and restores the environment's bound program, compiler log
and compiling names around the walk in a scope object, since it may run mid-parse of another
module and an unwind must put them back; the symbol state its passes compute lives on its own
`Program` (`#program-scoped-symbol-state`), so the caller's - which may be mid-simulate with a JIT reading it - is
untouched. One recursive mutex serializes every late require in the process. The late walk is
a compile of its own and never joins another compile's stream: under `CodeOfPolicies::module_cache`
it reads and writes its own `ModuleFileCache` at the default path keyed by the target's file, as a
script's `compile_file` does. A module whose parse failed pushes no record.

## Program-scoped symbol state (`ast.h`, `ast_export.cpp`, `ast_allocate_stack.cpp`) {#program-scoped-symbol-state}

Whether a program uses a function or a global, and the slot each holds in that program's
context, are decided per program - the symbol-use pass (`markSymbolUse` and its variants,
`ast_export.cpp`) walks the whole library from the program's roots, and `allocateStack` numbers
what it marked; `markFunctionDependencies`, which `collect_dependencies` runs for a macro, walks
one function's closure only, refreshing the use-edges it reaches and leaving the rest of the
program's symbol state as it found it - while the `Function` and `Variable` objects of a shared module are one instance
for every program in the process. The state therefore lives on `Program`, one record per object:
`functionStates` maps a function to its `FunctionSymbolState`, `variableStates` a global to its
`VariableSymbolState`, read through `isUsed` and `indexOf` and written through `setUsed` and
`setIndex`; `clearSymbolUse` drops every use flag and `clearIndices` every slot.
An index is `-1` for an object the allocation never saw, `-2` for one it saw and found unused,
the context slot otherwise; the constant folder tells the first from the rest. Simulate reads
the tables through `context.thisProgram`, and das code through the `ast` module's `is_used`,
`function_index` and `variable_index`, each taking the program - a null program there is a
holder nobody filled and throws, never an answer. `Program::simulate` binds itself to the
context it fills, and clears the binding at the end when the `rtti` option is off; a macro
context's binding is cleared after its simulate whatever the option, since the context lives in
the module for the process while its program is a temporary. A context that simulates an
expression outside that call takes the compiling program from the bound environment instead:
`eval_single_expression` sets it on the context it makes, and on a caller's context only when
that context has none, putting back what was there. A context that still has no program answers
-1 for every function and variable (`programIndexOf`, `ast_simulate.cpp`) - with no tables to
read, nothing is used and nothing holds a slot. The index tables are per allocation:
`allocateStack` clears both before numbering, so -1 means this pass never saw the object, and a
folding round's slots do not survive into the final pass. A compile nested inside another - a
macro calling `compile`, a late `require` (`#require-after-walk`), the folding program - fills its own tables
and leaves the outer program's answers standing. The stream a module-cache record carries has
no flag, slot or hash: the reading program recomputes them. Three per-compile
values still live on the objects: `Variable::stackTop` and `Function::totalStackSize` /
`totalGenLabel`, which `allocateStack` writes and the same program's simulate reads before any
nested compile can run. The JIT's selection - which functions of a program a DLL holds - is a
flag of the same record, read through `is_jit_selected` and written through
`set_jit_selected` / `clear_jit_selection`; `Function::requestJit` is the `[jit]` annotation
only, an author's request the selection reads and never writes. A function's semantic hash is
in the record too: simulate hashes the nodes the program built for it (`setHash`), so the hash
says how this program calls what the body calls, and das code reads it through
`function_hash`. The AOT hash folds the hashes of the function's dependency closure; it is
cached in the record by `getFunctionAotHash`, which takes the program; `hashFunctions`, the one
writer of the hashes, drops every cached AOT hash before it rehashes. The records are compile state: after
the main simulate the program frees every function record and every global's slot, keeping only a
global's use flag, which hosts read. A reader after that point declares itself - a host that
inspects the program sets `keep_symbol_tables`; `rtti`, which leaves the program bound to the
context, the AOT generator, the JIT, the folder and a macro compile keep the tables by the same
test.

## The size of a generic instance's argument types (`ast_infer_type_function.cpp`) {#instance-type-size}

A generic that calls itself with a type built from its own argument type makes a new instance on
every call, and the pass limit stops that chain only while each step adds a constant amount: the
fiftieth instance of `foo([a])` holds an array nested fifty deep. A step that uses the argument
type twice - `foo((a, a))`, a block `$(x : typedecl(a)) { return x }`, two arguments feeding
each other - doubles the type instead, so the thirtieth instance holds, without a limit, a billion type
nodes, and every whole-type walk inference runs (`isAuto`, `canCopy`, the copy itself) is that
large long before the pass limit counts fifty. `inferFunctionCall` therefore counts the type
nodes of the argument types where it is about to create an instance
(`argumentTypesFitInstance`) and reports `exceeds_instance_type` past 4096, which is one walk
per new instance and none per expression. The count is of nodes, never of depth: these types
are a few dozen levels deep when they stop fitting in memory. A structure counts as one node
whatever its fields, so the limit bounds only what type constructors nest.

That error is the only one of its compile. A call that gets no instance leaves its caller
without a type, so the same pass reports errors that only restate it, and `inferTypes` drops
every other error once one `exceeds_instance_type` is present.

An inference error names the function it was compiling and every function that instance came
from, each with its argument types. `Function::getLocationExtra` prints a type of more than 64
nodes as its constructor and `<...>`, so an error on an oversized instance stays short.

A type as written has its own limit, in the parser (`parser_impl.cpp`): both grammars stop at
64 levels of `<...>` nesting (`das_type_arrow_enter`), and a pointer wraps a type only while
the nesting under it fits the same limit (`das_check_pointer_nesting`), so no path through a
parsed type is longer than twice that. A postfix `?` is left-recursive and costs no parser
stack, so without the limit a pointer chain of any length parses and the first recursive walk
over it overflows the C++ stack. The limit is sized for a host that compiles on a 1 MB thread
stack in a build that inlines little: a type the parser accepts has to survive every recursive
walk inference runs over it. Past the limit the parse ends with one `exceeds_type_nesting`
error.

The stack one nesting level costs is the frame of the walk that recurses per level, and in a
build that inlines little every string temporary of every error message a function can raise
has its own slot in that frame. `InferTypes::verifyType` recurses once per level and can raise
about thirty errors, so it builds none of their text itself: `typeError`,
`malformedTypeError` and `templateStructureError` do, in frames that exist only while an
error is reported. With the text inline, one level costs 4176 bytes in a Debug build and a type
at the limit needs more than 512 KB; with it in the helpers the same type compiles in 256 KB.

## Lint before rewrites (`inferAndFinalizeModule`, `ast_parse.cpp`) {#lint-before-rewrites}

A module's diagnostics read the program as written. Once infer converges and the annotation
patches are quiet, `Program::lint` runs before any pass that changes the shape of code:
`[inline]` and auto-inline splicing, escape analysis's `scope_free` insertion, the annotation
fixups, `foldUnsafe` and `optimizeProgram`. A rewrite is free to reshape a function - splice a
callee's body over a call, drop a read, merge statements - so a rule checked after it would
judge the rewrite rather than the source: a spliced `delete super.self` no longer reads as a
call to the parent finalizer, and a spliced read leaves its holder looking unused.

Escape analysis runs after `buildAccessFlags`, so callee side-effect flags are final, and before
`foldUnsafe`, so the dirty re-infer of the `scope_free` calls it inserts does not re-trip unsafe
checks that are already folded. The inserted call is a generated terminal call that creates no
new candidate and changes no side-effect flags, so one dirty re-infer is the fixpoint.

`fixupAnnotations` runs after infer converges, when types are final, and before `foldUnsafe`
and `optimizeProgram`, so what a `fixup()` generates - the shader-blob globals dasGlsl, dasSpirv
and dasMetal fill - is folded and optimized like ordinary code. A call-shaped initializer a
fixup sets is uninferred; the gated dirty re-infer resolves it before `foldUnsafe`.

## Devirtualization (`ast_devirtualize.cpp`) {#devirtualization}

A class method is a function-pointer field every instance carries, and every class below the one
that first declares it - the family root - shares that slot. A call through static class `C`
becomes a direct call of `C`'s function only when, for every object whose class is `C` or below,
no class overrides the method and nothing assigns the field. A write through any class of the
family reaches those objects, so the guarantee covers the whole family, not `C` alone.

Two things give it. A promise: every class from `C` up to the family root seals the method - in
practice a method a `class sealed` declares, since `def sealed` always overrides a base field that
stays assignable. A closed world: a set of modules that holds every class of the family and every
write to it. `FixedMethods` takes the compiling module as that world, where a family is closed when
its root and every class deriving from it are private, since no other module can name them. The
scan covers the module's functions - its generic instances live there - and its global
initializers. A write is an assignment, a struct literal naming another function than the class's
own, or any use of the field other than a read or a method call.

Both proofs hold in every program that links the module, so `devirtualize` rewrites the invoke into
a typed direct call in the module's AST, in optimized builds, after lint and before inline
splicing: auto-inline splices it like a free function, and AOT and the JIT see a plain call.

## Whole-program facts (`ast_whole_program.cpp`) {#whole-program-facts}

Some conclusions need every module a program links: whether any class overrides a method, whether
any code writes a method field. A shared module's AST is one instance for every program
(`#program-scoped-symbol-state`), so such a conclusion is never written into it.
`Program::wholeProgram` holds them for one program: a handle to a `WholeProgramFacts`
(`ast_whole_program.h`), and `ast.h` declares only the handle, so `Program` carries no optimization's
details. `analyzeWholeProgram` fills it at the start of `Program::simulate`, after linking, when
symbol use and slots are final: it drops the old table, runs every pass of `wholeProgramPasses` in
order into a fresh one, and installs it. An unoptimized program gets no table. The interpreter and
the JIT read the table while they emit code and leave the AST as it is. The semantic hash hashes
the nodes the interpreter built (`#program-scoped-symbol-state`), so a call the table makes direct
is in it, and the JIT's cache keys, which fold it, keep one program's direct-call object from
another program that overrides the method. AOT C++ does not read the table yet. A new
whole-program optimization is a field of `WholeProgramFacts`, a pass that fills it, and a row in
`wholeProgramPasses`.

Devirtualization's whole-program pass (`collectDevirtualizedCalls`) takes the program's library as
the world (`#devirtualization`): a method invoke left in a used function is fixed when no class in
the library overrides the method below `C` and nothing writes its family. The scan reads every
module of the library, used or not, and reads the optimized AST, so a call the optimizer removed
writes nothing. The interpreter and the JIT emit a listed invoke as a direct call of its target.
