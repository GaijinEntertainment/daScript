---
slug: add-bit-to-ast-flag-union-checklist
title: How do I add a new bit to one of the AST flag unions (FieldDeclaration, Structure, Function) and expose it to daslang and docs without breaking anything?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

Adding a flag bit to `Structure::FieldDeclaration` / `Structure` / `Function` etc. requires changes at **four** positionally-linked sites. Miss one and the binding silently misaligns or the serializer caches stale state.

## The four sites

1. **C++ union** in `include/daScript/ast/ast.h` (or `ast_handle.h` etc.). Append the bit AFTER the existing last bit so existing bit positions don't shift. Example for FieldDeclaration:
   ```cpp
   bool            classMethod : 1;
   bool            _abstract : 1;     // NEW: leading underscore in C++ because daslang reserves "abstract"
   bool            inherited : 1;     // NEW: plain name (not a keyword in either side)
   ```
   Naming convention: if the daslang side needs a `_`-prefix to escape a keyword (`abstract`, `sealed`, `generator`), use the same underscore in C++ for symmetry. Otherwise plain identifier.

2. **Bitfield binding** in `src/builtin/module_builtin_ast_flags.cpp`. Find the matching `make<Name>Flags()` function and append the daslang name(s) to `argNames` in the **same order** as the C++ bits:
   ```cpp
   ft->argNames = { ... "implemented", "classMethod", "_abstract", "inherited" };
   ```
   The bitfield is positional — argNames[N] = C++ bit N. Reorder either side and every consumer of `fld.flags.<name>` reads the wrong bit.

3. **Handmade RST description** in `doc/source/stdlib/handmade/typedef-ast-<Name>Flags.rst`. Format: line 1 = type description, lines 2..N+1 = one short description per bit in argNames order. Append a line for each new bit:
   ```
   The method is declared abstract - body must be provided by a derived class.
   This field is inherited from a parent class and is not (re)declared in this class.
   ```
   Same positional invariant — line N+1 corresponds to bit N. Then run `bin/Release/daslang.exe doc/reflections/das2rst.das` to regenerate `doc/source/stdlib/generated/ast.rst` (gitignored — don't try to commit it).

4. **Serializer version bump** in `src/builtin/module_builtin_ast_serialize.cpp:2701`. Increment `currentVersion` (e.g. 84 → 85). `FieldDeclaration::serialize` writes the full `uint32_t flags`, so adding bits doesn't move existing ones — but bumping the version invalidates stale pre-bump cached AST blobs and surfaces version mismatches immediately rather than as subtle wrong-flag reads.

## Where to actually SET the bits

Most flag bits are set in the parser, not the typer. For FieldDeclaration in particular:
- `src/parser/parser_impl.cpp` — `ast_structureDeclaration` has three insertion points: clone-reset loop (~line 328), new-field branch (~line 388), override branch (~line 412). Each new bit needs explicit handling at the override branch (clear or set per semantics) — the clone-loop covers the inherit-from-parent case via `Structure::clone()` flag copy at `src/ast/ast.cpp:260`.
- Parser-side intent often lives in `src/parser/parser_impl.h` `VariableDeclaration` as a `bool isFoo = false` field, set at the parse site (e.g. `ast_structVarDefAbstract`) and consumed during the field-decl insertion.

## Don't touch parentType when adding "inherited-from-parent" semantics

`parentType` is a narrow auto-type-resolution flag (set when override declares with auto type, cleared by infer at `src/ast/ast_infer_type.cpp:251`). It is NOT "this field came from parent class". After inference it's almost always false. PR #2736 added a separate `inherited` bit precisely so macros could ask "did this field come from a parent class" without overloading `parentType`.

## Test pattern

Use a `[structure_macro]` helper module under `tests/language/_<helper>.das` (underscore-prefix = filtered from dastest discovery). In `apply()` or `finish()`, walk `st.fields`, compare `fld.flags.<bit>` against annotation args, and `errors := "..."; return false` on mismatch — compile fails on mismatch, so compile success IS the test. See `tests/language/_field_decl_flags_helper.das` for the canonical example.

## Annotation-arg name trap

If your test macro accepts a bool arg matching the flag name (e.g. `[probe(abstract = true)]`), the daslang **keyword shadow** rule rejects `abstract` / `sealed` / `inherited`-NOT-a-keyword as arg names. Prefix with `is_` / `has_`: `is_abstract`, `is_inherited`. Mirror this on the helper module's `find_arg` calls.

## Reference: PR #2736 (2026-05-19)

Added `_abstract` (0x400) and `inherited` (0x800) to FieldDeclaration. Diff is minimal (8 files, ~99 insertions) and serves as a complete worked example of the checklist above.

## Questions
- How do I add a new bit to one of the AST flag unions (FieldDeclaration, Structure, Function) and expose it to daslang and docs without breaking anything?
