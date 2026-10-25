# src/ast architecture notes - the module scan

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family.

## The module scan and the descriptor manifest (`dyn_modules.cpp`) {#module-scan-manifest}

`require_dynamic_modules` walks `<dasroot>/modules/`, then the project root's, then each
`-load_module` folder, and for every `.das_module` it finds calls `init_dyn_modules`. A
descriptor is a daslang program whose `initialize(project_path)` registers require paths
(`register_native_path`) and C++ modules (`register_dynamic_module`); those two builtins in
`module_builtin_fio.cpp` are the whole surface a descriptor's effect reaches. A descriptor compiles
under `ignore_shared_modules`: the shared daslib modules it requires neither come from nor land in
the environment's promoted set, so the program compiled after the scan has the same module set -
and the same module-cache records, `daslib/builtin` first - whether the scan compiled, replayed
or was skipped with `-no-dynamic-modules`. Compiling and running the descriptors is the scan's
cost, and it grows with the number of descriptors in the tree, so the scan keeps a manifest beside each descriptor, holding the rows the
registry received from it.

The manifest is a property of the module tree, not of the script or the cwd: the rows depend on
the descriptor's bytes and on four process-wide inputs a descriptor can read - its folder
(`project_path`), the das root (`get_das_root()`), the binary kind (`das_is_dll_build()`) and the
cross-compile target (`get_cross_platform_name()`, which `dasOpenGL`'s descriptor consults to
register its module for the web target only) - so the file sits next to the descriptor, and a
read-only tree simply compiles on every start. The binary kind and the compile target pick the
file's name - `.das_module.manifest`, `.das_module.static.manifest` for `daslang_static`, the
platform name before `.manifest` for a `--jit-target` run (`.das_module.emscripten.manifest`) -
so the kinds and targets sharing one tree (both suites at once, a sweep mixing native and wasm
runs) never rewrite each other's rows, and each replays its own from its second start on. Its key
is the descriptor's size and content hash (`hash_block64`, no stat) plus those four inputs, one
line each (`root`, `dll`, `dasroot`, `target`), plus one `dep` line per file the descriptor's
compile read - every module in its program with a file name, the daslib ones included - carrying
that file's size and hash as the scan's `FileAccess` serves it (hashed once per file per
access, whatever the descriptor count); a mismatch on any of them recompiles that descriptor
and rewrites its manifest, so a descriptor whose rows come from a module it requires recompiles
when that module changes. A named `dm` row's artifact is part of the key too: a row whose file
is gone - the package was configured out and the reconfigure removed its `.shared_module` -
makes the manifest stale, so the scan reruns the descriptor and the Quiet register fails as on
a cold start, and `has_module` answers absent rather than vouching for a row it would defer.
The file is line-oriented, tab-separated, with a
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
pending retry runs - and the require then finds the module or fails as a cold start would.
The deserializer (`Program::serialize` reading, and the module-cache record header) fetches a
builtin module by name the same way: a stream written where a module had loaded lazily is read
where nothing required it yet. `Module::Initialize` takes it too: on a half-warm tree - one descriptor compiled cold, so its
module loaded on start, beside replayed ones whose rows wait - the eager fixed point fails on
the first pass, brings every deferred module in and runs once more. A tree is half-warm
whenever two processes warm it at once, which parallel AOT batches do. The
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
and the post-scan retry of a sibling `DT_NEEDED` dlopen behave as on a compiled start. A row
still pending after that retry whose artifact is on disk failed on something other than a
missing file - on a platform that resolves a library's imports at load (Windows, Linux without
a RUNPATH), a sibling a deferred row holds, which the retry alone can never bring in - so the
scan brings every deferred module in and runs the retry once more, the set an eager start has;
a pending row whose artifact is absent stays pending and silent, as Quiet means, so a build
without a module's artifact pays nothing for it. A replayed nameless row whose module is
registered by the end of the scan - loaded at the scan, or by that fallback - gets the module's
name written back into its manifest (the same key and dependency stamps, the row named), so the
next start defers it as a start that recorded a clean load would; a row that never loads stays
nameless. A
require guard (`require ?mod`) and `builtin_module_exists` ask whether the build has the
module (`guardModuleAvailable`): linked in, or waiting in a manifest row, which the guard
loads then - so `require ?das_metal metal/das_metal_boost` still means "on a build with
Metal", a cold start and a warm start answer alike, and `llvm`, a witness module no das file
requires unguarded, comes in through the guards dasLLAMA places on it. A guard whose name
holds a `/` is a path instead: the guard is taken when the guard's own file resolves through the
compile's `FileAccess` - the rail for a pure-das package, which has nothing C++ to guard on, and
for a witness of a cross-package dependency. A guard the build does not have skips the require
silently and without resolving the target, so a skipped require probes no file paths; a guard the
build has over a target that does not resolve is the ordinary missing-module error. Neither form
falls back to the target's own resolvability: a module's source directory sits in every checkout
whatever the build configured. A load adds
nothing to `$`: a module-cache record carries each builtin module's cumulative hash of
its content, and a process that loaded a different set of C++ modules would otherwise fail
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
manifest carries the stamp and the flag and no rows, and is not rewritten. A module group is a
name that `require [group]` expands to a list: `register_module_group(group, member, guard)` in
a descriptor - recorded as a `grp` row and replayed - or `registerModuleGroupMember` from a C++
module adds a member, once (`ast_module.cpp`, one process-wide registry under a mutex, kept for
the life of the process like the native paths: a descriptor registers once per process, so a
cleared registry could only lose members); the list comes back sorted by member path, since the
scan registers in `readdir` order, which no platform promises. A member's guard is the guard a
hand-written `require ?guard member` would carry - a module name, or a path when it holds a
`/` - and is evaluated wherever the list is read; a descriptor whose das files sit in every
checkout guards its row on the C++ module only a build configured with it has, since a member
requiring a module the build lacks fails every requirer of the group, and a manifest cannot
record what a build has - its key does not see the build's artifacts. The text collector
(`getAllRequireReq`) and the parser (`ast_requireModuleGroup`) expand the same list into one
require per member, the group's guard and `public` on each, a member's own guard on that member;
a member resolves and fails as a require spelled by hand would, and a group nothing registered
adds nothing. The collector reads `require[group]` with no space as the parser does. The
requirer calls the members through `daslib/module_group`, whose `call_module_group` reads the
same list at macro time (`module_group_for_each_member`, the `rtti` module, a path guard
resolved through the compiling program's own access) and emits one qualified call per member, so
the expansion and the calls agree on the set. Membership is tree-level, so the answer does not
depend on the walk order - a module joins from its own descriptor, and the requirer names only
the group; the module-cache record stamps the expansion (`ARCHITECTURE.md#module-cache-read`), so a member joining later
re-parses the modules that require the group. With
`DAS_TRACE_MODULE_LOAD=1` the scan prints one line per descriptor - `replayed N row(s) in <sec>
(shared module load <sec>, deferred K)`, `compiled (<why>), manifest written (N row(s))`,
`compiled (no_manifest)`, `compiled (manifests ignored)`, or why a manifest was not written -
and a deferred load prints `[module] require <name>: loading the deferred <class>`, the
fallback `[module] loading every deferred module (K)`, and the scan's own fallback announces
itself first with `[module] a pending module's artifact exists - loading every deferred module`.
A replayed descriptor's time is its manifest read plus its rows, and the second number is the
share the `.shared_module` dlopen and module constructor took.
