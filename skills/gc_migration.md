# GC Migration Skill — smart_ptr to gc_node

Read this skill file before migrating daslang code (`.das` files in external repositories, modules, or user projects) from the old `smart_ptr<T>` AST patterns to the new gc_node raw-pointer patterns. This covers **all migrated types** and the concrete code changes needed.

## Which types migrated

The following AST types changed from `smart_ptr<T>` (reference-counted) to `gc_node` (garbage-collected raw pointers):

| C++ type | Old C++ typedef | New C++ typedef | daScript type |
|---|---|---|---|
| `TypeDecl` | `smart_ptr<TypeDecl>` | `TypeDecl*` | `TypeDecl?` (alias `TypeDeclPtr`) |
| `Expression` (all subclasses) | `smart_ptr<Expression>` | `Expression*` | `Expression?` (alias `ExpressionPtr`) |
| `Function` | `smart_ptr<Function>` | `Function*` | `Function?` (alias `FunctionPtr`) |
| `Structure` | `smart_ptr<Structure>` | `Structure*` | `Structure?` (alias `StructurePtr`) |
| `Enumeration` | `smart_ptr<Enumeration>` | `Enumeration*` | `Enumeration?` (alias `EnumerationPtr`) |
| `Variable` | `smart_ptr<Variable>` | `Variable*` | `Variable?` (alias `VariablePtr`) |
| `MakeFieldDecl` | `smart_ptr<MakeFieldDecl>` | `MakeFieldDecl*` | `MakeFieldDecl?` (alias `MakeFieldDeclPtr`) |
| `MakeStruct` | `smart_ptr<MakeStruct>` | `MakeStruct*` | — |

**NOT migrated** (still smart_ptr):
- `ProgramPtr = smart_ptr<Program>`
- `FunctionAnnotationPtr`, `StructureAnnotationPtr`, `EnumerationAnnotationPtr`
- `VisitorAdapterPtr` (from `make_visitor`)
- `smart_ptr<Context>`

## daScript code migration (.das files)

This is the main section for migrating user/module `.das` code.

### Variable declarations

| Before | After | Notes |
|---|---|---|
| `var inscope x <- new ExprCall(...)` | `var x = new ExprCall(...)` | No `inscope`, no `<-` |
| `var inscope x <- clone_expression(e)` | `var x = clone_expression(e)` | |
| `var inscope x <- clone_type(t)` | `var x = clone_type(t)` | |
| `var inscope x <- clone_function(f)` | `var x <- clone_function(f)` | `clone_function` still returns via move |
| `var inscope x <- qmacro(...)` | `var x = qmacro(...)` | |
| `var inscope x <- qmacro_block() { ... }` | `var x = qmacro_block() { ... }` | |
| `var inscope x <- qmacro_function(...) $(args) { ... }` | `var x <- qmacro_function(...) $(args) { ... }` | `qmacro_function` returns via move |
| `var inscope x <- qmacro_type(type<T>)` | `var x = qmacro_type(type<T>)` | |
| `var inscope x <- typeinfo ast_typedecl(type<T>)` | `var x = typeinfo ast_typedecl(type<T>)` | |
| `var inscope x : array<ExpressionPtr>` | `var x : array<ExpressionPtr>` | No `inscope` for arrays of gc types |
| `var inscope x : array<VariablePtr>` | `var x : array<VariablePtr>` | |

**Rule of thumb:** If the type is a gc_node pointer (`ExpressionPtr`, `TypeDeclPtr`, `FunctionPtr`, `StructurePtr`, `EnumerationPtr`, `VariablePtr`, `MakeFieldDeclPtr`) or an array/table of one, remove `inscope` and change `<-` to `=`. Exception: `clone_function` and `qmacro_function` still use `<-` because they return via move semantics on the daScript side.

**Still needs `var inscope <-`:** `make_visitor(...)` returns `smart_ptr<VisitorAdapter>` — keep `var inscope adapter <- make_visitor(*v)`.

### Return statements

| Before | After |
|---|---|
| `return <- expr` (returning ExpressionPtr) | `return expr` |
| `return <- default<ExpressionPtr>` | `return default<ExpressionPtr>` or `return null` |
| `return <- qmacro(...)` | `return qmacro(...)` |
| `return <- new ExprConstInt(...)` | `return new ExprConstInt(...)` |
| `return <- fun` (in visitor `visitFunction`) | `return <- fun` — **keep** (visitor returns use move) |

**Rule:** Remove `<-` from `return` when returning gc_node pointers. Exception: visitor `visitFunction`/`visitExprXxx` methods that return the node back — these still use `return <- fun` because the visitor adapter binding expects move semantics.

### Field assignment

| Before | After |
|---|---|
| `fn.result <- new TypeDecl(...)` | `fn.result = new TypeDecl(...)` |
| `fn.body <- new_body` | `fn.body = new_body` |
| `td.firstType <- expr` | `td.firstType = expr` |
| `field |> move_new <| new ExprConstInt(...)` | `field = new ExprConstInt(...)` |
| `field |> move <| expr` | `field = expr` |
| `func.body |> move <| new_body` | `func.body = new_body` |

### Array operations

| Before | After | When |
|---|---|---|
| `list |> emplace_new <| qmacro_block() { ... }` | `list |> push <| qmacro_block() { ... }` | Local `array<ExpressionPtr>` built by macro |
| `hashExprs |> emplace_new <| qmacro(...)` | `hashExprs |> push <| qmacro(...)` | Local `array<ExpressionPtr>` |
| `stmts |> emplace_new <| clone_expression(s)` | `stmts |> push <| clone_expression(s)` | Local `array<ExpressionPtr>` |
| `sbuilder.elements |> emplace_new <| expr` | `sbuilder.elements |> emplace_new <| expr` | **Keep** — AST node member arrays |
| `pCall.arguments |> emplace_new <| expr` | `pCall.arguments |> emplace_new <| expr` | **Keep** — AST node member arrays |
| `blk.list |> emplace_new <| expr` | `blk.list |> emplace_new <| expr` | **Keep** — AST node member arrays |
| `arr.values |> emplace <| nameExpr` | `arr.values |> emplace(nameExpr)` | Note: pipe vs call syntax |

**Rule:** For **local** `array<ExpressionPtr>` variables that you build up and then splice via `$b()` or `$a()`, use `push`. For **AST node member** arrays (`.elements`, `.arguments`, `.list`, `.values`), keep `emplace_new` or `emplace`.

### Scoping blocks

| Before | After |
|---|---|
| `if (true) { var inscope x <- ... }` | `{ var x = ... }` |

The `if (true)` pattern was needed to create a scope for `var inscope` inside loops. With gc_node, use bare blocks `{ ... }` for scoping.

### Macro method signatures

All overridable methods on macro base classes changed their parameter types:

| Before | After |
|---|---|
| `var expr : smart_ptr<ExprCallMacro>` | `var expr : ExprCallMacro?` |
| `var expr : smart_ptr<ExprCallFunc>` | `var expr : ExprCallFunc?` |
| `var blk : smart_ptr<ExprBlock>` | `var blk : ExprBlock?` |
| `expr : smart_ptr<ExprReader>` | `expr : ExprReader?` |
| `expr : smart_ptr<ExprTypeInfo>` | `expr : ExprTypeInfo?` |
| `expr : smart_ptr<ExprIsVariant>` | `expr : ExprIsVariant?` |
| `expr : smart_ptr<ExprAsVariant>` | `expr : ExprAsVariant?` |
| `expr : smart_ptr<ExprSafeAsVariant>` | `expr : ExprSafeAsVariant?` |
| `expr : smart_ptr<ExprFor>` | `expr : ExprFor?` |
| `expr : smart_ptr<ExprFind>` | `expr : ExprFind?` |
| `var call : smart_ptr<ExprCall>` | `var call : ExprCall?` |

Additionally, `canVisitArguments(expr)` → `canVisitArgument(expr; argIndex : int)` and `canFoldReturnResult(expr)` was added to `AstCallMacro`.

### Visitor method signatures

All `AstVisitor` pre/post methods changed from `smart_ptr<ExprXxx>` to `ExprXxx?`:

```das
// Before
def override preVisitExprCall(expr : smart_ptr<ExprCall>) : void
def override visitExprCall(expr : smart_ptr<ExprCall>) : ExpressionPtr

// After  
def override preVisitExprCall(expr : ExprCall?) : void
def override visitExprCall(expr : ExprCall?) : ExpressionPtr
```

This applies to **every** expression visitor method (ExprFor, ExprWhile, ExprBlock, ExprNew, etc.).

### Duplicate overload removal

When `TPtr` and `T?` become the same type, overload pairs become ambiguous:

```das
// REMOVE this trampoline — TypeDecl? IS TypeDeclPtr now
def foo(td : TypeDecl?) {
    unsafe { return foo(reinterpret<TypeDeclPtr> td) }
}
// KEEP the real implementation  
def foo(td : TypeDeclPtr) { ... }
```

Search pattern: look for `reinterpret<TypeDeclPtr>`, `reinterpret<ExpressionPtr>`, `reinterpret<FunctionPtr>` in trampoline overloads.

### get_ptr() removal

`get_ptr()` extracts a raw pointer from `smart_ptr`. After migration, the value is already a raw pointer:

| Before | After |
|---|---|
| `get_ptr(expr)` | `expr` |
| `func = get_ptr(fun)` | `func = fun` |
| `get_ptr(expr) is ExprVar` | `expr is ExprVar` |

### enu.get_ptr() for enumType

TypeDecl's `enumType` field changed from `Enumeration*` (already raw) to... it's still `Enumeration*`. But `EnumerationPtr` is now `Enumeration*` too. So:

| Before | After |
|---|---|
| `enumType = enu.get_ptr()` | `enumType = enu` |

### build_string pipe syntax

Some code changed `build_string() <| $(var w) { ... }` to `build_string() $(var w) { ... }` (dropping the `<|`). Both work, but the codebase standardized on the shorter form.

## C++ code migration

### Allocation

| Before | After |
|---|---|
| `make_smart<TypeDecl>(...)` | `new TypeDecl(...)` |
| `make_smart<ExprConstInt>(...)` | `new ExprConstInt(...)` |
| `make_smart<Function>()` | `new Function()` |
| `make_smart<Structure>(name)` | `new Structure(name)` |
| `make_smart<Enumeration>(name)` | `new Enumeration(name)` |
| `make_smart<Variable>()` | `new Variable()` |
| `make_smart<MakeFieldDecl>(...)` | `new MakeFieldDecl(...)` |

### Smart pointer operations

| Before | After |
|---|---|
| `ptr.get()` | `ptr` |
| `ptr.reset()` | `ptr = nullptr` |
| `ptr.orphan()` | `auto tmp = ptr; ptr = nullptr;` |
| `ptr.use_count()` | Remove — no refcount |
| `ptr->addRef()` / `ptr->delRef()` | Remove |
| `move_new(field, expr)` | `field = expr` |
| `swap(a, b)` | `swap(a, b)` (unchanged) |

### Function signatures

| Before | After |
|---|---|
| `const TypeDeclPtr &` (param) | `TypeDeclPtr` (by value — it's a pointer) |
| `smart_ptr<Expression> &` | `ExpressionPtr &` or `Expression * &` |
| `smart_ptr_raw<TypeDecl>` | `TypeDecl *` (remove `.marshal()` calls) |

### Containers

| Before | After |
|---|---|
| `safebox<TypeDecl>` | `safebox<TypeDecl, TypeDeclPtr>` |
| `safebox<Structure>` | `safebox<Structure, StructurePtr>` |
| `safebox<Enumeration>` | `safebox<Enumeration, EnumerationPtr>` |

### Parser (.ypp files)

1. Replace `make_smart<T>(...)` → `new T(...)` for all migrated types
2. Change `%destructor` for migrated types to no-ops: `{ /* gc owns T */ }`
3. Remove explicit `delete` of gc_node types in error recovery paths
4. Edit only `.ypp` files — CMake regenerates `.cpp` via bison

### Serializer

- `smart_ptr<T>` serialize/deserialize becomes raw pointer with identity tracking (pointer→ID map during serialize, ID→pointer during deserialize)
- Remove `ptr_ref_count::serialize()` calls
- `make_smart<T>` → `new T` in deserializer

### Uninitialized pointer fields

**CRITICAL:** `smart_ptr<T>` auto-initializes to null. `T*` does NOT. Every member field and local variable that was `smart_ptr<T>` must get `= nullptr`. Failure causes random crashes from garbage pointer dereference.

### Temporary gc_nodes in C++

Use `gc_local<T>` for C++ code that creates temporary AST nodes outside of compilation:

```cpp
gc_local<TypeDecl> td(new TypeDecl(Type::tVoid));
fakeExpr->type = td;  // implicit conversion to T*
// td deleted when gc_local goes out of scope
```

Use `gc_guard` for scoped GC collection in C++ code that creates many temporaries.

## GC architecture (for understanding, not usually needed for migration)

- `gc_node`: base class with doubly-linked list, `gc_magic` tag, unique `gc_id`, `gc_owner` pointer
- `gc_root`: owns a linked list of gc_nodes. Thread-local root is the default allocation target
- `new T(...)` for gc_node types: constructor links to `gc_root::gc_active_root`
- `Module::gc_collect(from)`: walks all reachable AST nodes, moves them from `from` root to module root
- `gc_root::gc_sweep()`: deletes all nodes remaining on a root (unreachable = garbage)
- `gc_guard`: RAII — redirects active root to a local root, sweeps on destruction
- `gc_local<T>`: RAII — unlinks from GC, deletes via C++ destructor (for temporaries)
- `ast_gc_guard()`: daScript-side wrapper for tools/utilities creating AST nodes at runtime

## ast_gc_guard for daScript tools

Any daScript program that creates AST nodes at runtime (via `clone_type`, `new TypeDecl`, `qmacro`, etc.) outside of normal compilation should wrap the scope in `ast_gc_guard()`:

```das
require daslib/ast

[export]
def main() {
    ast_gc_guard() {
        // ... code that creates AST nodes ...
    }
}
```

Without this, the leak detector reports "GC APP LEAK" at exit.

## Debugging GC leaks

1. Run program → check output for "GC COMPILE LEAK" / "GC APP LEAK" with node IDs
2. Set `DAS_GC_BREAK_ON_ID=<id>` env var → get full C++ + daScript stack trace at allocation
3. Find the `.das` code creating the node
4. Wrap the scope in `ast_gc_guard() { ... }` or fix the ownership path

## Verification workflow

After migrating `.das` code:

1. **Compile-only check:** `bin/Release/daslang.exe -compile-only your_file.das`
2. **Run tests:** `bin/Release/daslang.exe dastest/dastest.das -- --test your_tests/`
3. **Check for GC leaks:** look for "GC COMPILE LEAK" / "GC APP LEAK" in output
4. **Common compile errors after migration:**
   - `"can only move to from a reference"` → change `<-` to `=`
   - `"too many matching functions"` → duplicate overload, remove trampoline
   - `"variable is not a smart pointer"` → remove `inscope`
   - `"function not found: get_ptr"` → remove `get_ptr()`, value is already raw pointer

## Search patterns for finding code that needs migration

```bash
# In .das files — find smart_ptr usage for migrated types
grep -rn "smart_ptr<Expr\|smart_ptr<Function\|smart_ptr<Structure\|smart_ptr<Enumeration\|smart_ptr<Variable\|smart_ptr<MakeFieldDecl\|smart_ptr<MakeStruct\|smart_ptr<TypeDecl" *.das

# Find inscope with migrated types
grep -rn "var inscope.*ExpressionPtr\|var inscope.*TypeDeclPtr\|var inscope.*FunctionPtr\|var inscope.*StructurePtr\|var inscope.*EnumerationPtr\|var inscope.*VariablePtr\|var inscope.*MakeFieldDeclPtr" *.das

# Find inscope with new/clone/qmacro for migrated types
grep -rn "var inscope.*<-.*new Expr\|var inscope.*<-.*clone_expression\|var inscope.*<-.*clone_type\|var inscope.*<-.*qmacro\|var inscope.*<-.*new TypeDecl\|var inscope.*<-.*new Function\|var inscope.*<-.*new Variable" *.das

# Find return <- for migrated types
grep -rn "return <- .*qmacro\|return <- .*new Expr\|return <- .*clone_expression\|return <- default<ExpressionPtr>\|return <- .*new TypeDecl" *.das

# Find move_new for migrated types
grep -rn "move_new\|move <|" *.das

# Find get_ptr() that may be redundant
grep -rn "get_ptr(" *.das

# In C++ files — find make_smart for migrated types
grep -rn "make_smart<TypeDecl>\|make_smart<Expr\|make_smart<Function>\|make_smart<Structure>\|make_smart<Enumeration>\|make_smart<Variable>\|make_smart<MakeFieldDecl>\|make_smart<MakeStruct>" *.cpp *.h
```
