# dasLLVM Ledger

**Living document.** Planned work on the JIT, the AOT and standalone-exe cards, and the
cross-compile toolchain: what each entry costs today and what the change would do. An entry
leaves when it lands or is refuted.

## Entries

- **Cross-target handled-type layouts from the target, not the host.** Today a cross-compiled
  exe bakes the host's `sizeof`/`offsetof` of every handled type (`BasicStructureAnnotation`
  fields, `TypeDecl::getSizeOf`/`getAlignOf` for `tHandle`), so any platform-sized member ahead
  of a das-visible field - a mutex (64 bytes on darwin, 40 under Linux libc++ and emscripten), a
  `struct stat` (144 against 104), a `std::function` (48 under Linux libc++ 19, 32 under
  emscripten's and darwin's) - shifts what the bundle reads; three such cases were found across
  two hosts (Context, FStat, Program) and fixed by member order. `--jit-check-abi` finds a
  mismatch at the bundle's first launch, and only for the types the bundle links. The change:
  `build --wasm` runs a small emscripten program under node that walks every registered
  annotation (the check's own walk) and writes a per-toolchain table of size, alignment and
  field offsets; a program compiled with `--jit-target` answers the four layout calls from that
  table. The hard part is per-Program dispatch: macro and tooling programs running on the host
  during the same compile keep host layouts (reached through the bound `g_Program`), the
  cross-target program takes the table, and struct layouts fix at inference, so the switch
  cannot be codegen-only. Sized at two to three days; the check stays as the proof and gate.
