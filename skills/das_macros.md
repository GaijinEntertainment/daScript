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

## AST type introspection

- **`typeDecl.isConst`** — `true` if a `TypeDecl` has the `const` modifier. Useful for checking whether function parameters (e.g., `self` in interface methods) are declared const.
- **`typeDecl.argTypes`** — array of `TypeDeclPtr` representing function-type arguments (indices: 0 = first parameter). For interface method fields, `argTypes[0]` is the `self` parameter — check `.isConst` to determine if the method is const.
- **Interior mutability pattern** — when a const getter needs to lazily mutate a cache: declare param as `self : T ==const`, then `var pS = unsafe(reinterpret<T? -const>(addr(self)))` to strip const for cache mutation. Used in `daslib/interfaces.das` for const-only interface proxy caching.

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

### Generating statements with `qmacro_expr`

`qmacro_expr(${ statement; })` generates a statement-level expression — assignments, variable declarations, returns. The `${ }` block requires a trailing semicolon.

```das
// Generate an assignment statement
blk |> emplace_new <| qmacro_expr(${ my_var = some_function(); })

// Generate a variable declaration
blk |> emplace_new <| qmacro_expr(${ var $i(varName) : int; })

// Generate a return statement
blk |> emplace_new <| qmacro_expr(${ return $i(resName); })
```

**Key difference from `qmacro()`**: `qmacro()` generates expressions (function calls, field access, etc.). `qmacro_expr()` generates statements that include `=`, `var`, `return`, etc. — things that aren't valid as standalone expressions.

**Caveat**: Identifiers used in `qmacro_expr` are resolved in the **macro's** scope, not the generated code's scope. If generating references to variables that only exist in the generated code (not in the macro), the identifier must exist somewhere the macro can see it, or use manual AST construction instead.

### Default-initializing generated struct variables

In macro-generated code, `var x : $t(st)` fails with "uninitialized variable" for structs without field defaults. Use `default<T>` instead:

```das
// WRONG — fails if struct has uninitialized fields
blk |> emplace_new <| qmacro_block() { var entity : $t(st) }

// CORRECT — default-initializes all fields
blk |> emplace_new <| qmacro_block() { var entity := default<$t(st)> }
```

### Generating complex function bodies

For functions with variable declarations, nested blocks, and captured parameters, the `pass` + `reinterpret<ExprBlock?>` + `qmacro_block()` pattern works reliably:

```das
var inscope fn <- qmacro_function("my_func") $(count : int) : void {
    pass
}
var blk = unsafe(reinterpret<ExprBlock?> fn.body)
blk.list |> clear()
blk.list |> emplace_new <| qmacro_block() {
    var cache : array<int>
    // ... complex body with nested blocks, invokes, etc.
}
```

### Pattern-matching call shapes — never rely on leading `!`

When a macro walks an AST and pattern-matches specific call shapes to
route them differently (e.g. `_sql` detects nested
`select_from(...)._any(...)` and emits SQL `EXISTS`, vs. `_none(...)`
emitting `NOT EXISTS`), **do not rely on detecting `!expr` to flip the
emitted output**. Boris's standing rule (2026-04-24): "we don't have
leading `!` support" for AST walkers. The `!` often sits across
intermediate AST nodes — parentheses, `if` expressions, local `let`
bindings, constant-folded wrappers — that break a naive pattern match.
Pattern-matching `!any(...)` is correct for some cases, silently wrong
for any user that parenthesizes or refactors the condition.

**Ship explicit positive/negative name pairs instead.** Convention:
the negative form gets a `_not_` or `_no` prefix on the positive name.

| Positive | Negative | Emits |
|---|---|---|
| `_any` | `_none` | `EXISTS` / `NOT EXISTS` |
| `_in` | `_not_in` | `IN` / `NOT IN` |
| `_is_null` | `_is_not_null` | `IS NULL` / `IS NOT NULL` |
| `_between` | `_not_between` | `BETWEEN` / `NOT BETWEEN` |

Same rule applies outside `_sql` — any macro that needs to discriminate
"do X" vs "do NOT X" call shapes ships matched name pairs.

**Does NOT apply to runtime `!`.** Plain daslang code with `!any(...)`
or `!x._in(...)` compiles and evaluates fine when there's no
AST-walking macro routing through that expression. The constraint is
specifically for macro-expansion-time pattern matching.

**Docs:** when documenting a macro with negated forms, list the
positive/negative pair together — do not say "negate with `!`".
Discoverability is part of the UX.

This avoids parser issues with `qmacro_function` body that contains complex nested blocks or variable declarations.

## `[call_macro]` entry-guard contract

Every `[call_macro]` `class : AstCallMacro` declares an implicit contract
about how its `visit()` sees its arguments. Get this wrong and you'll
either expand against unresolved AST (and produce gibberish) or miss
inner macro expansions entirely.

### `canVisitArgument=true` is the default — inner-first expansion

Daslang's macro pass walks each call_macro argument before firing the
outer macro's `visit`. The walk runs the typer **and** any inner
call_macros recursively. By the time `visit(call)` runs:

- inner `_where(...)`, `_select(...)`, etc. — and any user-defined
  wrapper that expands to one of them — have **already cascaded** into
  their canonical `where_(...,$(_:T)=>...)` / `select(...)` shapes;
- type-checked sub-expressions carry resolved `_type`;
- field reads on those resolved types may be wrapped in
  `ExprRef2Value` (the value-from-ref adapter the typer inserts);
- raw arguments that genuinely can't type yet (e.g. `_.Field` outside
  a lambda, where `_` is unbound) stay as raw AST with `null _type` —
  the macro pass tolerates these because the outer macro is going to
  rewrite them anyway.

You almost never want to override this default. The Mode-1/Mode-2 story
documented in `modules/dasSQLITE/API_REWORK.md` chunk-2 section is the
cautionary tale: `_sql` shipped briefly with `canVisitArgument → false`
overrides because a synthetic test seemed to show inner expansion not
firing. The real cause was a missing `require daslib/linq_boost public`
in `sqlite_linq.das` — without that, `_where` / `_select` weren't
registered as call_macros at user-file scope, so they had nothing to
expand into. Removing the override + adding the require made everything
work. **If inner macros aren't expanding, look for a missing require
before changing visit semantics.**

### Entry guards: `macro_verify`, not `return null`

When `visit()` must assume a property of its arguments (typed input,
non-auto chain, expected arity), check it loudly with `macro_verify`,
including a `{describe(arg)}` in the message:

```
[call_macro(name="_sql")]
class private SqlMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at,
            "_sql expects one argument: a chain expression")
        var argT = call.arguments[0]._type
        macro_verify(argT != null, prog, call.at,
            "_sql: chain argument has null _type — inner call_macros did not expand " +
            "before this fired. Arg: {describe(call.arguments[0])}")
        macro_verify(!argT.isAutoOrAlias, prog, call.at,
            "_sql: chain type is auto/alias after _type is set — compiler bug")
        // ... real work
    }
}
```

A null `_type` on the chain argument doesn't mean "Mode 2 is broken" —
it means an inner call_macro that should have expanded didn't, almost
always because the user didn't `require` the module that defines it.
Loud failure tells the user that immediately; silent `return null`
re-queues the macro and lets the daslang pipeline emit a confusing
infer-time cascade instead.

### Peel `ExprRef2Value` before `qmatch`

Post-Mode-2-expansion AST walking will see field reads wrapped in
`ExprRef2Value`. `qmatch` is RTTI-strict — it matches `ExprField` but
not `ExprRef2Value(ExprField(...))`. Peel before matching:

```
if (node is ExprRef2Value) {
    node = (node as ExprRef2Value).subexpr
    if (node == null) {
        macro_error(prog, at, "_where: ExprRef2Value with null subexpr")
        return ""
    }
}
// now `qmatch(node, _.$f(name))` etc. work as expected
```

Auto-peel inside `qmatch` itself is a TODO documented in
`daslib/ast_match.das`. Until then, every analyzer entry point that
takes an expression coming out of post-expansion (predicate body,
projection body, classifier helpers like `is_const_or_captured_var`)
needs the peel at the top.

### When you really do need raw arguments

If a macro genuinely needs un-expanded raw AST (rare — mostly for
qmacro-only sugar that builds new code without inspecting it), override
`canVisitArgument` to return `false` for those argument indices. Pair
with a unit test that exercises the inner-macro composition case
specifically — that's the path that breaks first when the override is
wrong.
