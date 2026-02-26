# Macro & AST Programming

Read this skill file before writing compile-time macros, AST manipulation code, structure macros, qmacro/quote code generation, or any code that uses `smart_ptr<T>` ownership patterns (e.g., `TypeDeclPtr`, `ExpressionPtr`, `FunctionPtr`, `StructurePtr`).

## Move semantics for smart pointers (`<-` vs `move`)

- **`<-` operator**: ALWAYS `memcpy(dest, src) + memset(src, 0)` — it is a raw memory operation, NOT smart_ptr-aware. It zeros the source regardless of type.
- **`move` function**: Bound via C++ `builtin_smart_ptr_move*` family in `module_builtin_runtime.cpp` — proper smart pointer move with reference counting. Use `move(dest, src)` or `move(dest) src` for `smart_ptr<T>` transfers.
- **`return <- expr`**: Moves value to return slot and zeroes `expr`. If `expr` is a `&` ref parameter, this zeroes the *caller's* variable since they share memory.
- **Key subtlety**: When a function takes `var x : smart_ptr<T>&` (by reference) and internally calls a function that takes `var x : smart_ptr<T>` (by value/move), the `<-` inside `return` zeroes the `&` ref. Callers MUST capture the return value: `unsafe { expr <- apply_template(expr) $() { ... } }`
- **Safe pattern**: `unsafe { variable <- function_returning_smart_ptr(variable) $() { ... } }` — captures return value back into the same variable

## AST smart pointer ownership

- Many AST functions take `smart_ptr<T>` **by value** — this **moves** the pointer from the caller. After the call, the caller's variable is null/zeroed.
- **`var inscope` + move = double-free crash**: If you declare `var inscope p = make_something()` and pass `p` to a function that moves it, `p` is zeroed by the move. Then `inscope` destroys `p` at scope exit → double-free → Access Violation.
- **`clone_type(typeExpr)`** — deep-copies a `TypeDeclPtr`. Use when passing a type to a consuming function while keeping the original, or when the source is a `var inscope` variable.
- **`clone_expression(expr)`** — deep-copies an `ExpressionPtr`. Same ownership rules as `clone_type`.
- **Safe pattern for `add_structure_field`**: pass temporaries directly — `st |> add_structure_field("name", clone_type(qmacro_type(type<int>)), qmacro($v(val)))` — the `clone_type` result is a temporary safely consumed by the move.
- **`move_new`**: `field |> move_new <| expr` — idiomatic way to assign a newly created `smart_ptr<T>` into a field. Equivalent to `field <- expr` but does not require `unsafe`. Preferred over `field := null; unsafe { field <- expr }` when setting AST node fields (e.g. `cm.init |> move_new <| qmacro(...)`).
- **`add_ptr_ref(raw_ptr)`** — wraps a raw pointer (`T?`) into a `smart_ptr<T>` by adding a reference. AST node fields are often raw pointers (e.g. `typeDecl.structType` is `Structure?`, `typeDecl.enumType` is `Enumeration?`), but many API functions expect `smart_ptr<T>`. Use `add_ptr_ref` to bridge: `var inscope st <- add_ptr_ref(pair_type.structType)` gives a `StructurePtr` from a `Structure?` field. Also accepts `smart_ptr<T>` input (adds a ref and returns a new smart_ptr). Always use `var inscope` for the result to manage lifetime.
- **Rule of thumb**: if a function signature takes `var x : smart_ptr<T>` (not `&`), it **consumes** `x`. Pass a temporary, a clone, or accept that your variable will be null after the call.

## Structure macros — generating types and functions

- **`[structure_macro(name=foo)]`** — annotation on a class inheriting `AstStructureAnnotation`; the `apply` method runs at compile time when a struct has `[foo]`. Use to generate companion types, operators, and functions.
- **`clone_structure(get_ptr(st))`** — deep-copies a `StructurePtr` for creating modified companion types (e.g., SOA layout where every field becomes `array<FieldType>`)
- **`get_ptr(st)`** — extracts raw pointer from `smart_ptr<Structure>` for use in `TypeDecl.structType` fields
- **`compiling_module() |> add_function(fn)`** — registers a concrete function in the current module
- **`compiling_module() |> add_generic(fn)`** — registers a generic function (instanced per call site)
- **`compiling_module() |> add_structure(st)`** — registers a generated struct
- **`compiling_module() |> add_alias(tdef)`** — registers a type alias
- **`fn.flags |= FunctionFlags.generated`** — marks a function as compiler-generated (suppresses "unused" warnings, enables special error messages)
- **`ExprFieldFieldFlags.no_promotion`** / **`ExprAtFlags.no_promotion`** — prevent the compiler from promoting field access or index access to a different type; needed in generated AST to preserve exact types
- **`[tag_function(tag_name)]`** on a function + **`[tag_function_macro(tag="tag_name")]`** on a class — intercepts calls to the tagged function and rewrites them in the `transform` method. Used for compile-time call rewriting (e.g., SOA `operator .` rewrites `soa[i].field` → `soa.field[i]`).
- **`[for_loop_macro(name=foo)]`** on a class inheriting `AstForLoopMacro` — intercepts `for` loops whose source is a matching type. Override `visitExprFor` to rewrite the loop AST (e.g., SOA for-loop expands `for (it in soa)` into per-field array iteration).

## `qmacro` vs `quote` (code generation)

- **`qmacro(expr)`** — quasi-quote with reification splices (`$v()`, `$e()`, `$c()`, `$t()`, `$i()`, `$f()`, `$a()`, `$b()` etc.). Use when the generated code contains interpolated values.
- **`qmacro_function("name") $(args) { body }`** — generates an entire `FunctionPtr` with spliced arguments/body. The `$t(typeExpr)` splice in the signature sets parameter/return types from `TypeDeclPtr` variables.
- **`qmacro_expr(${ statement; })`** — generates a statement-level expression (e.g., assignment). The `${ }` block allows semicolons.
- **`quote(expr)`** — plain quote with NO reification. Use when the expression is a simple literal or constant with no splices — e.g. `quote(true)`, `quote(false)`, `quote(0)`.
- **Rule**: if the expression contains no `$…()` reification operators, prefer `quote()` over `qmacro()`.

### Reification operators (inside `qmacro`)

- **`$v(daslangVar)`** — splice the runtime **value** of a variable into the generated code
- **`$e(exprPtr)`** — splice an `ExpressionPtr` as a sub-expression
- **`$c(stringVar)`** — splice a `string` as a **call name** (function name). Example: `$c(callName)(arr, val)` generates a call to whatever function name `callName` holds
- **`$t(typeDeclPtr)`** — splice a `TypeDeclPtr` as a type annotation in signatures or declarations
- **`$i(stringVar)`** — splice a string as an **identifier** (variable name)
- **`$f(stringVar)`** — splice a string as a **field name**. Example: `st.$f(fieldName)` becomes `st.x` when `fieldName="x"`
- **`$a(arrayOfExprPtr)`** — splice an `array<ExpressionPtr>` as function call **arguments**
- **`$b(arrayOfExprPtr)`** — splice an `array<ExpressionPtr>` as a **block body** (sequence of statements). Build the array with `emplace_new`, then `$b(bodyExprs)` inlines all statements into the function body
