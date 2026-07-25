# Annotation info rework — self-contained debug info

## Problem

Runtime debug info (`debug_info.h`) leaks AST pointers into the Context:

- `VarInfo::annotation_arguments : void*` — points at `Structure::FieldDeclaration::annotation`
  (an `AnnotationArgumentList` living inside the gc_node AST).
- `StructInfo::annotation_list : void*` — points at `Structure::annotations` (`AnnotationList`).
- `TypeInfo::annotation_or_name : mutable TypeAnnotation*` — tagged pointer: low bit set means
  `~`-padded `module::name` string, lazily resolved against the bound environment; otherwise a
  direct `TypeAnnotation*`.

A Context can outlive its Program (program unload while context runs), so the first two dangle
unless the embedder keeps the ProgramPtr alive — which is what `options rtti` signals (it merely
keeps `context.thisProgram` non-null; the lifetime discipline is on the embedder). Function and
Enumeration annotations are not reflected at all. Both non-interpreter tiers already deep-copy:
AOT emits standalone static `AnnotationArguments` objects plus an init-time pointer fixup; JIT
heap-allocates copies via `jit_make_varinfo_annotations` / `jit_free_varinfo_annotations`.

## Design

Debug info becomes self-contained: annotation data is deep-copied into the per-context
`DebugInfoAllocator` at `DebugInfoHelper` time, unconditionally. Since that allocator is linear
and never runs destructors, the copies are POD mirrors with `allocateCachedName` strings:

```cpp
struct AnnotationArgumentInfo {            // POD, debug-heap allocated
    Type         type;                     // tBool/tInt/tFloat/tString
    const char * name;
    const char * sValue;
    union { bool bValue; int iValue; float fValue; };
};
struct AnnotationInfo {                    // mirrors one AnnotationDeclaration (or a handled-type ref)
    const char *             name;        // annotation name
    const char *             module_name; // declaring module
    AnnotationArgumentInfo * arguments;   // flat array
    uint32_t                 count;
    mutable Annotation *     resolved;    // lazy env-lookup cache
};
```

Field changes:

| struct | before | after |
|---|---|---|
| `VarInfo` | `void * annotation_arguments` | `AnnotationArgumentInfo * annotation_arguments; uint32_t annotation_count` |
| `StructInfo` | `void * annotation_list` | `AnnotationInfo * annotations; uint32_t annotation_count` |
| `FuncInfo` | — | `AnnotationInfo * annotations; uint32_t annotation_count` (NEW) |
| `EnumInfo` | — | `AnnotationInfo * annotations; uint32_t annotation_count` (NEW) |
| `TypeInfo` | `mutable TypeAnnotation * annotation_or_name` (union) | `AnnotationInfo * annotation_info` (union) |

Globals (`Variable::annotation` is a bare `AnnotationArgumentList`, same shape as struct fields)
are captured into `VarInfo.annotation_arguments` too (NEW).

Resolution: `TypeInfo::getAnnotation()` (and the new `resolve_annotation` das builtin) look up
`module_name::name` in the bound environment's module list and cache into `resolved`
(`g_resolve_annotations` gates the cache write, as today). The `~`-padding alignment hack and
the low-bit pointer tag in `Module::resolveAnnotation` are deleted; both
`DAS_THREAD_SAFE_ANNOTATIONS` paths unify into always-name-based + lazy resolve. The invariant
"modules outlive contexts" is unchanged; what's removed is the dependence on *Program* lifetime.

`DebugInfoHelper::rtti` dies. Everything it gated becomes unconditional: annotation capture,
field/global constexpr init-value capture, builtin `cppName`. `options rtti`'s only remaining
effect is keeping `context.thisProgram` for compile-time/macro introspection (`this_program()`,
`ast_typedecl`). Memory cost is near-zero for unannotated entities (null + 0) and dedup of
strings comes free via `allocateCachedName`.

## das API

New (rtti module):

- `each_annotation(s : StructInfo) : iterator<AnnotationInfo>` — same for `FuncInfo`, `EnumInfo`
- `each_annotation_argument(v : VarInfo)` / direct indexed access to `annotation_arguments`
- `get_annotation_argument_value(arg : AnnotationArgumentInfo) : RttiValue`
- `resolve_annotation(ann : AnnotationInfo) : Annotation?` — live object, env lookup + cache

Deprecated (kept working, `[deprecated]`):

- `structure_for_each_annotation(st, $(ann : Annotation; args : AnnotationArguments))` — shim
  reconstructs a stack `AnnotationArgumentList` from the POD copies and env-resolves the
  `Annotation*` for the duration of the block invoke. Works without `options rtti` now.

## Tier impact

- **AOT** (`daslib/aot_cpp.das`): emits static `AnnotationArgumentInfo` / `AnnotationInfo`
  arrays (now also for functions/enums/globals). The init-time field-annotation fixup function
  dies — the mirrors are static-initializable, no `std::string` construction at load. Init-time
  `resolveAnnotation()` pre-warm for handled types stays. Ctor/layout change makes stale external
  AOT files fail loudly at compile (desired).
- **JIT** (`modules/dasLLVM/daslib/llvm_jit.das`): hard-coded VarInfo/StructInfo/EnumInfo/
  TypeInfo layout tables updated; `jit_initialize_varinfo_annotations` /
  `jit_free_varinfo_annotations` shims AND the tagged-pointer
  `handle_typeinfo_constructor` deleted — `AnnotationInfo` globals are plain static data
  (the old ptrtoint|1 scheme existed because wasm32 forbids widening ptrtoint in .data).
  JIT *gains* struct/enum annotation lists (previously always null).
  `LLVM_JIT_CODEGEN_VERSION` bump.
- **Interpreter**: the fix itself.

## das-side string fields and AOT

`AnnotationInfo.name` / `AnnotationArgumentInfo.name` (and `sValue`, `module_name`) are
`const char *` bound as das `string` — same as `VarInfo.name`. Historically, passing such
a field *directly as a function argument* compiled interpreted but failed AOT C++
compilation (`const char*` → `char*`): the emitter cast const-owner string field reads to
`(const char * const)`. Fixed in the follow-up to PR #3101 — const-owner reads now emit a
two-step cast `((char *)(const char *)(...))`: the inner cast picks the const-qualified
conversion operator on class-typed members (the `SimpleString` case from f38081856), the
outer strips pointee-const, which is safe because das strings are immutable and always
lower to `char *`. Direct field passes now work in all tiers; no interpolation needed.

## Known pre-existing gap (unchanged)

AOT-emitted `VarInfo` statics carry `flag_hasInitValue` in flags but never emit the actual
`value`/`sValue` payload (stays zero) — true before this rework under `options rtti`, true
after it for everyone. `get_variable_value` consumers go through helper-built debug info
(fresh from the live Module AST), so nothing in-tree reads the static payload.

## Affected consumers (in-tree)

`src/simulate/json_print.cpp`, `src/simulate/json_scan.cpp`, `src/builtin/module_jit.cpp`,
`src/builtin/module_builtin_rtti.cpp`, `daslib/rtti.das`, `daslib/aot_cpp.das`,
`modules/dasLLVM/daslib/llvm_jit.das`, `utils/daslang-live/main.cpp`,
`tutorials/integration/cpp/06_interop.cpp` (+ RST), `tests/language/reflection.das`.

## Tests

- function/enum/global annotation reflection (new capability)
- annotations readable WITHOUT `options rtti`
- tests-cpp: read annotations from a live Context after the ProgramPtr is released
- existing reflection.das / json annotation behavior preserved

## Post-merge

- daspkg externals sweep for `annotation_arguments` / `annotation_or_name` consumers
  (same drill as FIXED_ARRAY_REWORK).
