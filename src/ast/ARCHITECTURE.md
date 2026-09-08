# src/ast - the front end's AST

Contract: `ARCHITECTURE_COMMON.md` (repo root). This document carries the mechanisms of this
folder that a rule cites. The folder's other subsystems - parsing, inference, simulation - are
described in `skills/internal/cpp_codebase_notes.md` (repo-only).

## 1. The module-cache read in `trySerializeProgramModule` (`ast_parse.cpp`)

The front end parses modules in require order. When a module cache is installed
(`ModuleFileCache`), each module first tries `trySerializeProgramModule`. The reader stands at
the next record and checks that the record names this file, that the size and content hash of
the bytes the compile's `FileAccess` serves for that name match (never a stat, so a file that is
not on disk under that name is covered, and a rewrite that leaves the bytes alone serves), and
that every compile-time input the record's macros pinned
(`add_module_cache_dependency` - a file's bytes, an environment variable's value under
`env:NAME`, a command-line flag's occurrences under `arg:--flag`) still has the same content. A
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
repairs it carries the new policies. The first mismatch is the cutoff: the reader marks the stream failed, the
module and everything after it parse from source, and the writer rewrites the whole file - the
served records re-serialized from the modules the reader restored, then the freshly parsed
ones. A record whose header matched but whose payload fails to deserialize reparses in place,
without cutting the stream, whenever the record carries a usable payload length.

Every diagnostic the read prints - a record for another file, a changed file, a changed
dependency, changed policies, a truncated or version-mismatched stream, a failed payload, a
reparse in place - is
gated on the serializer's `quietCache`. The host sets that flag for the default cache, which is
on unasked for an ordinary run; ungated, each of those lines would be output every user sees on
an ordinary edit. An explicit `-module-cache` leaves the flag off, so those runs get the lines
together with the host's verdict.

## 2. The module scan and the descriptor manifest (`dyn_modules.cpp`)

`require_dynamic_modules` walks `<dasroot>/modules/`, then the project root's, then each
`-load_module` folder, and for every `.das_module` it finds calls `init_dyn_modules`. A
descriptor is a daslang program whose `initialize(project_path)` registers require paths
(`register_native_path`) and C++ modules (`register_dynamic_module`); those two builtins in
`module_builtin_fio.cpp` are the whole surface a descriptor's effect reaches. A descriptor compiles
under `ignore_shared_modules`: the shared daslib modules it requires neither come from nor land in
the environment's promoted set, so the program compiled after the scan has the same module set -
and the same module-cache records, `daslib/builtin` first - whether the scan compiled, replayed
or was skipped with `-no-dynamic-modules`. Compiling and running the descriptors is the scan's
cost, and it grows with the number of descriptors in the tree, so the scan keeps a manifest beside each descriptor, `.das_module.manifest`, holding the rows the
registry received from it.

The manifest is a property of the module tree, not of the script or the cwd: the rows depend on
the descriptor's bytes and on four process-wide inputs a descriptor can read - its folder
(`project_path`), the das root (`get_das_root()`), the binary kind (`das_is_dll_build()`) and the
cross-compile target (`get_cross_platform_name()`, which `dasOpenGL`'s descriptor consults to
register its module for the web target only) - so the file sits next to the descriptor, and a
read-only tree simply compiles on every start. Its key
is the descriptor's size and content hash (`hash_block64`, no stat) plus those four inputs, one
line each (`root`, `dll`, `dasroot`, `target`), plus one `dep` line per file the descriptor's
compile read - every module in its program with a file name, the daslib ones included - carrying
that file's size and hash as the scan's `FileAccess` serves it (hashed once per file per
access, whatever the descriptor count); a mismatch on any of them recompiles that descriptor
and rewrites its manifest, so a native run and a `--jit-target` run of one tree alternate
rewrites rather than serve each other's rows, and a descriptor whose rows come from a module it
requires recompiles when that module changes. The file is line-oriented, tab-separated, with a
format version (`MANIFEST_HEADER`) on its first line and an `end` line carrying the row count
(`dep` lines are key, not rows); a missing `end`, a count mismatch, an unknown row kind, a wrong
field count or a `dm` row whose `on_error` is not one of `RegisterOnError`'s three values is
damage, and the reader answers damage with a recompile and a rewrite, not a partial replay. The
writer refuses a descriptor whose recorded string holds a tab or newline (`field_ok`), rather
than an escaped form the reader would have to decode, and goes through a `.tmp` and a rename.

Recording is armed around one descriptor run: each builtin appends the arguments it actually
received, in order, and `register_dynamic_module` records its call whatever the outcome and adds
the das-visible module name once the load succeeded. Replay registers every `np` row as
recorded. A `dm` row carrying that name is not loaded by the scan: the row waits under the name
(`defer_dynamic_module`), and the load happens at the first require that names it. The
prerequisite walk (`getPrerequisits`) finds no module under the name and asks the loader the
scan installed (`setDeferredModuleLoader`); the loader dlopens and registers the module, runs the
`initDependencies` fixed point that `Module::Initialize` runs (`Module::InitializeDependencies`) over
the grown list, and when a module reports it cannot initialize - what it needs is deferred too -
brings every deferred module in and runs the fixed point again, which is the set an eager start
has. A row whose own dlopen fails takes the same road - every deferred module comes in, the
pending retry runs - and the require then finds the module or fails as a cold start would. The
rows and the loader are the scan's: `require_dynamic_modules` clears the rows before its
walk and installs the loader, and `Module::Shutdown` clears both, so an environment that
follows sees neither the last one's rows nor its loader. A parse that no prerequisite walk
precedes - the `compile` of a string - meets a deferred module at the parser's own require
(`ast_requireModule`) and loads it there; the loader puts the parse's program back as the
bound one, since a module's builtin das part parses under a program of its own. The load runs under one gc root of its own with the thread root's nodes parked meanwhile,
because a constructor's nodes go to the active root while a builtin das module it compiles
dumps its leftovers on the thread root, and a collect stops at a node owned by another root;
after the load every module, not only the new ones, collects from that root, since a
constructor registers into modules that exist already, and the rest is swept. A `dm` row
with no name - the recording start's load failed - replays as recorded, so the Quiet deferral
and the post-scan retry of a sibling `DT_NEEDED` dlopen behave as on a compiled start. A
require guard (`require ?mod`) and `builtin_module_exists` ask whether the build has the
module (`guardModuleAvailable`): linked in, or waiting in a manifest row, which the guard
loads then - so `require ?das_metal metal/das_metal_boost` still means "on a build with
Metal", a cold start and a warm start answer alike, and `llvm`, a witness module no das file
requires unguarded, comes in through the guards `daslib/tune` places on it. A load adds
nothing to `$`: a module-cache record carries each builtin module's cumulative hash of
mangled names, and a process that loaded a different set of C++ modules would otherwise fail
every record on `$`, so a `vector<T>` of a handled element registers into the element's
module (`vectorHomeModule`, `ast_handle.h`) whichever module builds it - a module that exists
already, when the element is another module's - and only a vector of a builtin element lands
in `$`, which every library lists first because
`ModuleLibrary::addModule` puts a module's dependencies before it. The described name of such
a vector carries that module - ``ast::dasvector`ptr`Expression``, not ``$::...`` - so code
that names one compares the part after `::` (`daslib/ast_boost`'s printer). `-ignore-manifest`
reads and writes no manifest: every descriptor compiles and every C++ module loads on start,
the form a tool that enumerates modules - the MCP server - runs under.
`no_manifest()` inside `initialize` marks the descriptor as one that runs on every start: its
manifest carries the stamp and the flag and no rows, and is not rewritten. With
`DAS_TRACE_MODULE_LOAD=1` the scan prints one line per descriptor - `replayed N row(s) in <sec>
(shared module load <sec>, deferred K)`, `compiled (<why>), manifest written (N row(s))`,
`compiled (no_manifest)`, `compiled (manifests ignored)`, or why a manifest was not written -
and a deferred load prints `[module] require <name>: loading the deferred <class>`, the
fallback `[module] loading every deferred module (K)`. A replayed descriptor's time is its
manifest read plus its rows, and the second number is the share the `.shared_module` dlopen and
module constructor took.
