# daslib architecture notes - the generated C API

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family.
The header this section describes is the one a standalone context ships.

## 30. c_api_header

- **One module writes the generated header, and it owns the C declarations in it.** The C
  declarations come first; the C++ half (`CppApi`) follows under `#ifdef __cplusplus`, and both
  hosts include one file without seeing the other's half. `aot_standalone` emits the matching
  `extern "C"` bodies off this describer, so the header and the source cannot disagree about what
  crosses or what it is called.
- **C sits ON TOP of C++, not under it.** The C++ methods are DEFINED in the generated `.cpp` and
  call the AOT functions directly; each `extern "C"` entry point is a wrapper over the method
  beside it. So the C++ API is the wide one - every export, in native C++ types, spelled by
  `aot_standalone` because it owns C++ types the way this module owns C ones - and C is the subset
  C can express, not a ceiling over it. A method keeps the daslang name: `[export_c(name = ...)]`
  renames the C symbol only.
- **The generated `.cpp` carries the header's text instead of including it.** The header is the
  HOST's file - it may move it or edit it - and the implementation must not be breakable that way.
  Types are declared once (`type_defs` never reaches the bodies), so inlining is a substitution for
  the include rather than a second copy. A host header that drifts from the library still fails
  loudly: a changed or removed signature is a link error, a changed layout trips the header's own
  size and offset asserts.
- **`Function.flags.exports` is the whole selection truth.** `[export_c]` (`ExportCAnnotation`,
  `daslib/export_c.das` - a das `[function_macro]`, so the C surface is decided entirely in
  daslang) sets it, and so does `[export]`, so a standalone context takes the bit however it was
  set. Accepting the bit however it was set is what licenses skipping an unspellable signature with
  a warning; a function that ASKED for C with `[export_c]` and cannot cross is an error. No second
  list can drift from the bit. Whether a signature CAN cross is decided here instead, after infer,
  because argument types do not exist when an annotation applies.
- **Refusal is per stage, not per module**: `collect_c_exports` returns its rejections and
  logs its skips. An `[export_c]` that cannot cross comes back for the caller to report -
  `macro_error` during compilation, the jit error log during codegen - so this module needs
  no `ProgramPtr` and no reporting policy of its own. A merely-public function is skipped
  with a warning naming it and the type.
- **The scalar widths and the vector layouts are C++-side facts this emitter mirrors.**
  `bool` is one byte (the `static_assert(sizeof(bool)==1)` in `getTypeBaseSize`,
  `src/simulate/debug_info.cpp`), so das `bool` meets C as `bool`. `float3` is `{x, y, z}` at
  12 bytes and 4-byte alignment, because `vec3<TT>` (`include/daScript/misc/vectypes.h`)
  is a plain three-field struct with no `alignas` - the 16-byte vec4f shape is the JIT's
  register ABI, not the memory layout a header has to mirror. So no vector or structure carries
  an alignment attribute, and every declared struct carries a size assert plus one offset assert
  per field. `Structure.sizeOf` is already rounded to the struct's alignment
  (`Structure::getSizeOf`, `src/ast/ast.cpp`), so `sizeof` in C matches it directly.
- **A bound value type crosses as the wrap type its annotation carries.** A
  `ManagedValueAnnotation` is not a ref type and holds `makeValueType()` - the `WrapType<OT>::type`
  das moves the value through, reachable from das as `get_underlying_value_type`. The header
  typedefs that shape under the das type's name and the body assigns the das type through it; a C++
  host still gets the real type, because the C++ half carries the module's `aotRequire` include. It
  is the one C type with an alignment attribute - a handle's alignment is not its wrap type's
  (`BigEntityId` is 16-aligned, four floats are 4-aligned) - on ONE declarator, since the attribute
  applies per declarator. Size and alignment are both asserted. A ref-type handle stays `void *`.
- **An enumeration is a typedef of its base integer plus loose enumerators, never a C
  `enum`** - a C enum's underlying type is implementation-defined, which would break the
  size assert on the 8/16/64-bit bases and on negative values. The values are read off the
  entry's folded constant, so no smart pointer is needed to reach `find_enum_value`.
- **Types are emitted only when a signature reaches them, in post-order.** A by-value
  field's structure is defined before the structure holding it; every structure also gets a
  forward typedef ahead of all definitions, which is what lets a self-referential
  (`Node?`) field compile. A pointer's target only has to be NAMEABLE, so one whose fields C
  cannot spell is forward-declared and never DEFINED - defining it emits fields with no type at
  all - while a representable target is defined, so a host can read through the pointer. A cycle
  terminates either way, and a pointer to something C cannot even name degrades to `void *`
  rather than refusing the function.
- **A `fixed_array` argument crosses (as `const T *`, which is what the das ABI already
  passes) but a `fixed_array` result does not** - that would be a CMRES of an array, a
  pointer nothing on the C side sizes.
