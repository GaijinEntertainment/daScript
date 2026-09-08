# tests/module_cache - the module cache's process tests

Contract: `ARCHITECTURE_COMMON.md` (repo root). These tests exercise the module cache the
daslang host installs unasked - its per-compile key, and the front end's record-by-record read;
this document states what the folder is and why its tests take the shape they do.

## 1. File charters

- `test_default_cache_path.das` - the default cache as the host installs it with no flag: that
  it stays silent on every read outcome, that its file is keyed per compile input, that each
  flag which turns it off or makes it loud does so, and that the loud verdict names the served
  count and the cutoff; on an explicit cache it also pins the two stamps a record carries - the
  compile's policies, whose change reparses every module in place and rewrites the file, and
  the module's source, stamped by content so a byte-identical rewrite serves and a same-size
  edit cuts off; and the directory's size cap - children run inside a temp directory, so the
  default cache they fill and prune is their own, never the tree's.
- `test_macro_dep_invalidate.das` - a compile-time input a macro pinned through
  `add_module_cache_dependency` is compared by content, not mtime: a byte-identical rewrite
  serves the record, a changed file re-parses from that module on and says so.
- `test_generic_instance_origin.das` - a generic instance restored from the cache keeps its
  origin generic, so a fresh program compiled in the same process after the restore resolves it.
- `test_descriptor_manifest.das` - the manifest the module scan keeps beside each `.das_module`
  descriptor, replaying its registrations instead of compiling the descriptor, on a project root
  the test writes, three modules in it: one whose descriptor registers
  a require path, one whose descriptor registers an absent C++ artifact (the `dm` row, replayed
  and retried), one whose descriptor does not compile (no manifest, the run goes on). Over the
  first: the first start compiles the descriptor and writes the manifest beside it, the second
  replays it, an edited descriptor recompiles once, every rejection the reader knows - a table
  over each mutated line - recompiles and rewrites, an edited module the descriptor requires
  recompiles it once, a `--jit-target` run keys apart from a native one, a `no_manifest()` descriptor compiles on every start, a recorded argument with a
  tab leaves the manifest unwritten, and a directory sitting where the `.tmp` or the manifest
  goes fails the create or the rename so the start just compiles; each verdict is read from
  the `DAS_TRACE_MODULE_LOAD=1` line the child prints.
- `test_deferred_modules.das` - a replayed `dm` row is not loaded by the scan. The project root
  holds a copy of the tree's `dasUnitTest` (descriptor and artifact), which shadows the tree's,
  so the copy's manifest is the test's to make cold or warm: a cold start compiles the
  descriptor, loads the module to record its name, and a `require ?UnitTest x` is taken; a
  warm start defers the row and loads nothing for a program that requires nothing; the first
  `require UnitTest` loads it and the program calls into it; `typeinfo builtin_module_exists`
  loads it with no require naming it, while rtti `has_module` answers true and loads nothing;
  a lazy start and an eager start count the same functions in `$`, so a load registers
  nothing into another module; a guard alone loads the module
  and is taken, as it is with a `require UnitTest` above it, below it, or in the entry while
  the guard sits in a module walked earlier;
  `-ignore-manifest` compiles every descriptor, loads every C++ module on start and writes no
  manifest; a manifest row hand-edited to an absent artifact makes the require bring every
  deferred module in and then fail on the missing prerequisite; a module cache an eager start
  wrote serves a lazy start, with `-log-compile-time` printing the reads and the startup
  timeline; and, where the tree holds dasImgui and dasGlfw, `require imgui_app` brings every
  deferred module in because its `initDependencies` asks for two more. A static build, whose
  tree holds no `.shared_module`, has nothing to observe and the test says so and returns.
- `_fixtures/` - the driver and module scripts the spawned children compile (`mc_dep_*`,
  `mc_generic_origin_*`); a case needing a macro-bearing module graph puts it here instead of
  writing the script inline.
- `_common.das` - the spawn helpers every test here shares: the binary to spawn (`das_exe`),
  the scan-trace command prefix (`trace_prefix`), the stderr-joining child run (`run_child`)
  and the failure report that echoes the child's output (`report_child`). A test whose child
  needs a different spawn shape - an argv spawn, an environment variable - keeps that one
  helper local.

## 2. Why every case is a spawned process

The host installs the cache before `compileDaScript` and finishes it after; the default path,
the quiet flag and the verdict are host decisions (`utils/daslang/main.cpp`). A test compiling
in-process sees none of them. Each case therefore spawns the daslang binary dastest itself runs
under (`argv[0]`): the default-path case on scripts it writes to a temp directory, asserting on
the child's stdout and on the files under `.jitted_scripts/module_cache/` in the cwd; the
explicit-cache cases on a driver under `_fixtures/` with `-module-cache` pointed into a temp
directory, asserting on the child's stdout; the manifest case on a project root under a temp
directory, asserting on the scan trace. Each removes what it wrote under its own directory,
with one ledgered write outside it: the default-path case's child writes the default cache,
which is `.jitted_scripts/module_cache/` under the cwd it shares with dastest by definition,
and the test removes what that child wrote before it returns. The `.das_module.manifest`
sidecars every child's module scan warms beside the tree's descriptors are the scan's,
gitignored, and stay. A child's stdout is echoed on failure,
because the exit code alone would turn a one-line answer into an exit-code hunt.
