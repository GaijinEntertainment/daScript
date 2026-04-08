# GC Migration Skill — TypeDecl (Phase 1)

Read this skill file before working with `TypeDecl` gc_node patterns, fixing GC leaks, or migrating other types to gc_node in the future. This is the authoritative reference for all migration patterns, verified against the `gc-typedecl` branch (191 files, 6024 tests passing).

## Architecture

- `gc_node` (`include/daScript/misc/gc_node.h`) is a doubly-linked list node with magic, unique ID, and owner pointer
- `gc_root` owns a list of gc_nodes. Thread-local `gc_root::gc_active_root` is the default allocation target
- New `TypeDecl` instances land on the thread-local root via `gc_node()` constructor
- `Module::gc_collect()` (`src/ast/ast_gc_collect.cpp`) walks all reachable TypeDecl and moves them to the module's root
- Whatever remains on the thread-local root is garbage — swept by `gc_root::gc_sweep()`
- `gc_guard` RAII struct redirects thread-local root to a local root for scoped cleanup
- `gc_local<TypeDecl>` RAII — unlinks from GC on construction, deletes normally (for C++ temporaries)
- `ast_gc_guard()` — daScript-side function wrapping gc_guard for use in macros, tools, visitors

## C++ Migration Patterns

### Allocation

| Before | After | Notes |
|---|---|---|
| `make_smart<TypeDecl>(...)` | `new TypeDecl(...)` | gc_node ctor links to active root |
| `make_smart<TypeDecl>(*src)` | `new TypeDecl(*src)` | copy ctor creates new gc_node |

### TypeDeclPtr typedef

```cpp
// Before: typedef smart_ptr<TypeDecl> TypeDeclPtr;
// After:  typedef TypeDecl * TypeDeclPtr;
```

TypeDeclPtr is now a raw pointer. Most code that uses TypeDeclPtr doesn't change — pointer semantics are the same.

### smart_ptr operations -> raw pointer

| Before | After | Notes |
|---|---|---|
| `foo.get()` | `foo` | Already a raw pointer |
| `foo.reset()` | `foo = nullptr` | |
| `foo->method()` | `foo->method()` | Unchanged |
| `foo = make_smart<TypeDecl>(...)` | `foo = new TypeDecl(...)` | |
| `if (foo)` | `if (foo)` | Unchanged |
| `foo == nullptr` | `foo == nullptr` | Unchanged |
| `move_new(foo, expr)` | `foo = expr` | No ownership transfer needed |
| `move(foo.firstType) <\| expr` | `foo.firstType = expr` | Direct assignment for sub-types |
| `foo.orphan()` | `auto tmp = foo; foo = nullptr;` | 4 locations |
| `swap(a, b)` | `swap(a, b)` | Works for raw pointers |

### Function signatures

| Before | After | Notes |
|---|---|---|
| `smart_ptr<TypeDecl>` | `TypeDecl *` or `TypeDeclPtr` | |
| `const TypeDeclPtr &` (param) | `TypeDeclPtr` (by value) | It's just a pointer now |
| `TypeDeclPtr &` (out param) | `TypeDeclPtr &` | Still by reference |
| `smart_ptr<TypeDecl> &` | `TypeDecl * &` or `TypeDeclPtr &` | |

### smart_ptr_raw<TypeDecl>

~37 core references. Becomes plain `TypeDecl *`:

| Before | After |
|---|---|
| `smart_ptr_raw<TypeDecl>` | `TypeDecl *` |
| `smart_ptr_raw<TypeDecl> foo` | `TypeDecl * foo` |
| `.marshal()` calls | remove — already raw pointer |

### Containers

| Before | After |
|---|---|
| `vector<TypeDeclPtr>` (was `vector<smart_ptr<TypeDecl>>`) | `vector<TypeDecl*>` (unchanged with new typedef) |
| `safebox<TypeDecl>` | `safebox<TypeDecl, TypeDeclPtr>` — second template param for raw pointer |
| `das_map<string, TypeDeclPtr>` | `das_map<string, TypeDecl*>` (typedef handles it) |

### Removed APIs

| API | Action |
|---|---|
| `use_count()` on TypeDeclPtr | Remove — no refcount. Remove assertions using it |
| `addRef()` / `delRef()` on TypeDecl | Remove — no refcount |
| `TypeDecl::clone()` use_count==1 optimization | Disabled — always allocate fresh |

### Parser (.ypp files)

Both `ds_parser.ypp` and `ds2_parser.ypp` have `make_smart<TypeDecl>` in grammar actions. Change to `new TypeDecl(...)`. The `%destructor` for `<pTypeDecl>`, `<pTypeDeclList>`, and `<aTypePair>` must be no-ops (`{ /* gc owns TypeDecl */ }`) — gc handles TypeDecl lifetime, not parser error recovery. Remove explicit `delete` of TypeDecl in grammar actions (e.g. `delete $dimlist`). Edit only `.ypp` files — CMake regenerates `.cpp` via bison automatically.

### Serializer (ast_serializer.cpp)

TypeDecl serialization changes — `smart_ptr<TypeDecl>` serialize/deserialize becomes raw pointer. The serializer needs to track TypeDecl identity for cross-references (same pointer serialized twice should deserialize to same object). Use a pointer->ID map during serialize, ID->pointer map during deserialize.

## daScript Migration Patterns

### Variable declarations and assignment

| Before | After | Notes |
|---|---|---|
| `var inscope td <- new TypeDecl(...)` | `var td = new TypeDecl(...)` | No inscope, no move |
| `var inscope td <- clone_type(x)` | `var td = clone_type(x)` | |
| `var inscope td <- qmacro_type(type<T>)` | `var td = qmacro_type(type<T>)` | |
| `var inscope td <- typeinfo ast_typedecl(type<T>)` | `var td = typeinfo ast_typedecl(type<T>)` | |
| `var inscope td <- qmacro_template_class(...)` | `var td = qmacro_template_class(...)` | |
| `fn.result <- new TypeDecl(...)` | `fn.result = new TypeDecl(...)` | Direct assign, not move |
| `move(td.firstType) <\| expr` | `td.firstType = expr` | Direct assign for sub-types |
| `move(td.secondType) <\| expr` | `td.secondType = expr` | |
| `move_new(ptr) <\| clone_type(x)` | `ptr = clone_type(x)` | |
| `clone(info.firstType)` on TypeDecl | `clone_type(info.firstType)` | `clone()` doesn't work on raw ptrs |

### Type annotations

| Before | After | Notes |
|---|---|---|
| `smart_ptr<TypeDecl>` in types | `TypeDecl?` | |
| `TypeDeclPtr` in function params | Still works | Typedef alias for `TypeDecl?` |
| `var foo : TypeDeclPtr` | `var foo : TypeDecl?` | Or keep `TypeDeclPtr` — same thing |
| `inscope` on TypeDecl vars | remove | gc handles lifetime |
| `get_ptr(td)` on TypeDecl | `td` | Already a raw pointer |

### Overload disambiguation

When a module has both `def foo(td : TypeDecl?)` and `def foo(td : TypeDeclPtr)` overloads, they are now **ambiguous** (same type). The trampoline overload (typically `TypeDecl?` → `reinterpret<TypeDeclPtr>`) must be removed. Real example from `daslib/rst.das`:

```das
// REMOVE this trampoline — TypeDecl? IS TypeDeclPtr now
def describe_type_short(td : TypeDecl?) {
    unsafe { return describe_type_short(reinterpret<TypeDeclPtr> td) }
}
// KEEP the real implementation
def describe_type_short(td : TypeDeclPtr) { ... }
```

### emplace_new still works

`emplace_new(vec, expr)` is still valid for `VectorTypeDeclPtr`. For `argTypes`, both `emplace_new` and `push` work:
```das
td.argTypes |> push(clone_type(arg._type))        // OK
td.argTypes |> emplace_new(clone_type(arg._type))  // also OK
```

## Temporary TypeDecl in C++ — gc_local<T>

When C++ code creates a temporary `TypeDecl` that is only needed locally (not stored in any module or AST), use `gc_local<TypeDecl>` from `gc_node.h` to prevent GC leaks. This is the replacement for the old pattern where `smart_ptr<TypeDecl>` auto-deleted temporaries.

```cpp
#include "daScript/misc/gc_node.h"

// Simple case — single temporary
gc_local<TypeDecl> td(new TypeDecl(Type::tVoid));
fakeExpr->type = td;  // implicit conversion to TypeDecl*
// td is deleted when gc_local goes out of scope

// Complex case — multiple temporaries with vector
vector<gc_local<TypeDecl>> locals;
locals.emplace_back(new TypeDecl(Type::tHandle));
fakeVariable->type = locals.back();
locals.emplace_back(new TypeDecl(Type::tInt));
indexExpr->type = locals.back();
// all deleted when vector goes out of scope
```

**When to use:**
- `DebugInfoHelper::makeTypeInfo` / `makeFunctionDebugInfo` — creates TypeDecl for debug info, discarded after
- `ast_simulate.cpp` — fake expressions/variables for simulation use TypeDecl temporarily
- Any C++ code that creates `new TypeDecl(...)` outside of compilation (where gc_guard doesn't cover)

**When NOT needed:**
- TypeDecl created during compilation (covered by compile gc_guard + gc_collect)
- TypeDecl stored in module AST (reachable from module root, collected by gc_collect)
- TypeDecl created inside a `gc_guard` scope (swept when guard exits)

**Beware of TypeDecl copy constructor:** `new TypeDecl(*src)` recursively creates `new TypeDecl` for `firstType`, `secondType`, and each `argType`. A single `gc_local` on the root does NOT clean up children. Solutions:
- **Best:** Don't copy — use the original pointer directly if you don't modify it: `varExpr->type = var->type;`
- **If you must copy a simple type** (no sub-types): `gc_local` on root is sufficient
- **If the copy has sub-types:** Use `vector<gc_local<TypeDecl>>` and collect all nodes, or use `makeBlockType`-style collection

## ast_gc_guard for daScript tools and utilities

Any daScript program or utility that creates TypeDecl nodes at runtime (via `clone_type`, `new TypeDecl`, `qmacro_type`, `typeinfo ast_typedecl`, etc.) should wrap the relevant scope in `ast_gc_guard()`. Without it, the leak detector reports "GC APP LEAK" at exit.

```das
require daslib/ast

[export]
def main {
    ast_gc_guard() {
        // ... code that creates TypeDecl nodes ...
    }
}
```

**Known sites requiring ast_gc_guard:**
- `doc/reflections/das2rst.das` — creates 300+ TypeDecl nodes during documentation generation
- `utils/aot/main.das` — AOT compilation creates temporary TypeDecl during each file
- `modules/dasLLVM/daslib/llvm_jit.das` — JIT compilation creates TypeDecl during visitor walks
- `modules/dasLLVM/daslib/llvm_exe.das` — same pattern for EXE generation
- `modules/dasLLVM/daslib/llvm_macro.das` — simulate macros create TypeDecl in loops

## Common Gotchas

1. **`const TypeDeclPtr &` params** — many C++ functions take these. Now just `TypeDeclPtr` by value (pointer copy). The `const &` was only there because smart_ptr copy bumped refcount.

2. **Functions returning TypeDeclPtr** — now return `TypeDecl *`. No ownership transfer, no refcount change. The pointer points to a gc_node on some root.

3. **TypeDecl copy constructor** — creates a NEW gc_node on the active root. Deep-clones sub-types. This is correct: copied TypeDecl is a new allocation.

4. **No cascading delete** — when a TypeDecl is swept, its `firstType`/`secondType`/`argTypes` are NOT deleted (they're raw pointers to other gc_nodes, independently managed). `dimExpr` (still `vector<ExpressionPtr>`) DOES cascade via smart_ptr.

5. **Annotation TypeDeclPtr fields** — 8 annotation classes store TypeDeclPtr. These are initialized during module construction and collected via `Annotation::gc_collect(gc_root*)` virtual.

6. **Parser error recovery** — `deleteTypeDeclarationList()` in `parser_impl.cpp` manually deletes TypeDecl during error recovery. With gc, these are already on the thread-local root and will be swept. Remove the manual delete.

7. **`astTypeInfo` cache** — `Program::astTypeInfo` stores raw `TypeDecl*`. These point to TypeDecl owned by ExprTypeInfo expressions. Safe as long as the owning expression is reachable.

8. **CRITICAL: All `TypeDeclPtr` fields/locals MUST be initialized to `nullptr`** — `smart_ptr<TypeDecl>` auto-initialized to null. `TypeDecl*` does NOT. Every struct field, class member, and local variable of type `TypeDeclPtr` that was previously `smart_ptr<TypeDecl>` must get `= nullptr` explicitly. Failure to do this causes random crashes from uninitialized pointer dereference (typically reads from 0xFFFFFFFFFFFFFFFF or 0xCDCDCDCDCDCDCDCD). Use this regex to find violations: `TypeDeclPtr\s+\w+;` (without `= nullptr`).

9. **Parser .ypp files are bison/flex sources** — `ds_parser.cpp`, `ds2_parser.cpp` are generated from `.ypp` by bison. Edit only the `.ypp` source files — CMake regenerates the `.cpp` automatically via bison when `.ypp` changes. Never manually edit generated `.cpp` parser files.

10. **safebox template changed** — `safebox<DataType, VT>` now takes a second template parameter (default `smart_ptr<DataType>`). For gc_node types, pass `TypeDeclPtr` explicitly: `safebox<TypeDecl, TypeDeclPtr>`.

11. **DLL interface cannot export `thread_local`** — On Windows, `__declspec(dllexport)` on a struct/class with `static thread_local` members causes C2492. Solution: use accessor functions (`gc_get_thread_root()`, `gc_get_active_root()`) with `static thread_local` in the .cpp file.

12. **Stack-allocated TypeDecl is fatal** — TypeDecl inherits gc_node. Stack allocation registers with gc_root in constructor. Destructor fires outside gc_sweep, corrupting gc_root linked list. Always use `gc_local<TypeDecl>` for temporaries or `new TypeDecl` for heap.

13. **Duplicate overloads after migration** — any daScript module with both `def foo(td : TypeDecl?)` and `def foo(td : TypeDeclPtr)` overloads now has ambiguous dispatch. Remove the trampoline overload. Found in: `daslib/rst.das` (`describe_type_short`, `describe_type`).

14. **`get_ptr()` on raw TypeDecl pointer** — `get_ptr()` works on `smart_ptr<T>` only. After migration, `field._type` is already `TypeDecl?` — calling `get_ptr()` on it is a compile error. Found in: `daslib/rst.das` (3 sites with `get_ptr(arg._type)`).

## Documentation Update Checklist

After migrating TypeDecl (or any future type), update documentation:

### Generated stdlib docs (das2rst)
1. Wrap `das2rst.das` main in `ast_gc_guard()` if TypeDecl nodes are created
2. Run `bin/Release/daslang.exe doc/reflections/das2rst.das`
3. Fill new `// stub` files in `doc/source/stdlib/handmade/` — check old-hash versions for descriptions to reuse
4. Add new functions to `group_by_regex` calls in `das2rst.das` to prevent "Uncategorized" sections
5. Regenerate, verify no Uncategorized: `grep -c Uncategorized doc/source/stdlib/generated/*.rst | grep -v ':0$'`

### RST language and tutorial docs
Update all `.rst` files under `doc/source/reference/` that reference changed types:
- **Prose**: `TypeDeclPtr` -> `TypeDecl?` in descriptions, `smart_ptr<TypeDecl>` -> `TypeDecl?`
- **Code blocks**: `var inscope td <- clone_type(x)` -> `var td = clone_type(x)`, etc.
- **Removed patterns**: delete explanations about `inscope` double-free, `move()` for sub-types
- **API signatures** in `macros.rst`: keep `TypeDeclPtr` — it's still a valid typedef used in `daslib/ast.das`
- **C++ context** (`cpp_api.rst`): use `TypeDecl*` not `TypeDecl?`

Search patterns to find stale RST content:
```bash
grep -rn "var inscope.*TypeDecl\|var inscope.*clone_type\|smart_ptr<TypeDecl>\|move.*firstType.*<|" doc/source/reference/
```

### Sphinx verification
```bash
cd doc && rm -rf sphinx-build ../site/doc
.venv/Scripts/sphinx-build.exe -b html -d sphinx-build source ../site/doc 2>&1 | grep -iE "warning:|error:"
```
Must introduce no new warnings.

## Verification Workflow

After completing the migration, verify with this sequence:

1. **Build**: `cmake --build build --config Release -j 64 -- /nodeReuse:false`
2. **Lint all modified .das files**: `bin/Release/daslang.exe utils/lint/main.das -- daslib tutorials examples`
   - 0 crashes = good. Compile errors from missing C++ prereqs are expected.
   - Watch for: "can only move to from a reference" (need `=` not `<-`), "too many matching functions" (duplicate overloads), "can't delete" (stale `var inscope`)
3. **Format all modified .das files**: Use MCP `format_file` tool on each
4. **Run das2rst**: `bin/Release/daslang.exe doc/reflections/das2rst.das` — should produce no output (no leaks)
5. **Run tests**: `bin/Release/daslang.exe dastest/dastest.das -- --test tests`
6. **AOT build+test**: `cmake --build build --config Release --target test_aot` then `bin/Release/test_aot.exe -use-aot dastest/dastest.das -- --use-aot --test tests`
7. **Sphinx build**: see above

## Debugging GC Leaks

1. Build debug, run the leaking program -> get node IDs from "GC APP LEAK" / "GC COMPILE LEAK" output
2. Set `DAS_GC_BREAK_ON_ID=<first_id>` env var -> get full C++ + daScript stack trace
3. Find the `.das` code creating the TypeDecl (usually `clone_type` in a visitor or macro)
4. Wrap the visitor/macro scope in `ast_gc_guard() { ... }`
