# dasLLVM Ledger

**Living document.** Planned work on the JIT, the AOT and standalone-exe cards, and the
cross-compile toolchain: what each entry costs today and what the change would do. An entry
leaves when it lands or is refuted.

## Entries

- **The split obj cache shares nothing between programs.** A partition's key is the running
  chain over every module before it (`ARCHITECTURE.md` sec.2.1) and its object sits in the
  program's own `.jitted_scripts/<namespace>/`, so two programs over one engine share no
  object: two dasLLAMA unit tests (`tests/test_batch_grid.das` and `tests/test_box_profile.das`,
  each a `-jit` child of `modules/dasLLAMA/tests/run.das -- --suite model-free`, the `.o` names
  listed from the namespace folder its log names) read 0 of 19 key-named objects in common, while their
  partitions' own content - each function's mangled name and semantic hash under
  `DAS_JIT_DUMP_HASHES=1` on the same two runs - is identical in 11 of 19 partitions carrying 524 of 757 functions,
  the tune-stamped kernel partition among them (about 12 s of O3 codegen in every file that
  reaches the engine, on a Threadripper 3990X at 16 lanes). No function carried two hashes; partitions differ only by which
  functions a program uses. After an engine edit each of the module's 146 test children
  re-emits the engine, 13 to 31 s apiece on that box. The change: key a partition by its own content - the
  semantic hash is what the AOT tier already binds stubs by, and partitions are separate LLVM
  modules - and keep the objects in one folder shared by every program, written to a temp name
  and renamed, swept by age instead of the per-program keep-set.

- **The vector `log` returns finite values for zero and negative inputs on an AVX2 box.**
  `tests/llvm_vector_math.das`'s `test_vector_log_special_values` reads `log(0) = -127` and
  `log(-x) = 2` where the scalar answers are `-inf` and NaN - the shape of an exponent-field
  extraction with no special-value select on that lane path (a Zen 2 3990X; CI's lane reads the
  cell green). Its own small change: the select over the special inputs in the vector rail's
  `log`, proven by the same cell on an AVX2 box.
- **A shipped `-exe` builds split and links with LTO.** An `-exe` is one codegen unit: the
  facade's exe takes ten minutes to build on the zen2 box (`modules/dasLLAMA/tests/test_exe_smoke.das`
  reds at its 600 s wall on master too), and the one-unit form is what buys its cross-module
  inlining - about 5% over a split build on the decode path. The split codegen builds in seconds
  after the first emit (the per-module object cache), and `DAS_JIT_PROBE_LTO` already carries the
  shape that keeps both: the partitions emit bitcode and lld's LTO link inlines across them. The
  change: promote that rail from a probe to the `-exe` path (`--jit-split-modules` honored by
  `-exe`, the link at `/opt:lldlto=2`), measure the 5% back on the records rig against the
  one-unit exe on the same GGUF, then the gate and the release rig both build the fast form and
  the build-and-debug skill's "a released `-exe` always is one unit" becomes what the producer
  pins. A short PR of its own, after the batched-decode arc lands.
- **Cross-target handled-type layouts from the target, not the host.** Today a cross-compiled
  exe bakes the host's `sizeof`/`offsetof` of every handled type (`BasicStructureAnnotation`
  fields, `TypeDecl::getSizeOf`/`getAlignOf` for `tHandle`), so any platform-sized member ahead
  of a das-visible field - a mutex (64 bytes on darwin, 40 under Linux libc++ and emscripten), a
  `struct stat` (144 against 104), a `std::function` (48 under Linux libc++ 19, 32 under
  emscripten's and darwin's) - shifts what the bundle reads; cases found and fixed by member
  order: `Context`, `FStat`, `Program`. `--jit-check-abi` finds a
  mismatch at the bundle's first launch, and only for the types the bundle links. The change:
  `build --wasm` runs a small emscripten program under node that walks every registered
  annotation (the check's own walk) and writes a per-toolchain table of size, alignment and
  field offsets; a program compiled with `--jit-target` answers the layout calls named above
  from that table. The hard part is per-Program dispatch: macro and tooling programs running on the host
  during the same compile keep host layouts (reached through the bound `g_Program`), the
  cross-target program takes the table, and struct layouts fix at inference, so the switch
  cannot be codegen-only. Sized at two to three days; the check stays as the proof and gate.
