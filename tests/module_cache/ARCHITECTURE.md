# tests/module_cache - the module cache's process tests

Contract: `ARCHITECTURE_COMMON.md` (repo root). The mechanism these tests exercise is stated in
`src/builtin/ARCHITECTURE.md` sec.2 (the default cache file and its key) and
`src/ast/ARCHITECTURE.md` sec.1 (the read); this document states what the folder is and why its
tests take the shape they do.

## 1. File charters

- `test_default_cache_path.das` - the default cache as the host installs it: silent cold, warm
  and over a truncated file; one file per key (the script, the host flags before `--`, the
  `DAS*` environment, the binary - a launch by bare name through `PATH` keys the same file as a
  launch by path); off under `-no-module-cache` and `-compile-only`; `-no-module-cache` over an
  explicit `-module-cache` and refused beside `-ser`/`-deser`; and the verdict an explicit
  `-module-cache` prints, naming the served count and the cutoff.
- `test_macro_dep_invalidate.das` - a compile-time input a macro pinned through
  `add_module_cache_dependency` is compared by content, not mtime: a byte-identical rewrite
  serves the record, a changed file re-parses from that module on and says so.
- `test_generic_instance_origin.das` - a generic instance restored from the cache keeps its
  origin generic, so a fresh program compiled in the same process after the restore resolves it.

## 2. Why every case is a spawned process

The host installs the cache before `compileDaScript` and finishes it after; the default path,
the quiet flag and the verdict are host decisions (`utils/daslang/main.cpp`). A test compiling
in-process sees none of them. Each case therefore spawns the daslang binary dastest itself runs
under (`argv[0]`) on scripts it writes to a temp directory, and asserts on the child's stdout
and on the files under `.jitted_scripts/module_cache/` in the cwd, removing what it wrote. A
child's stdout is echoed on failure, because the exit code alone would turn a one-line answer
into an exit-code hunt.
