# Macro & AST Programming

Read this skill file before writing compile-time macros, AST manipulation
code, structure macros, qmacro/quote code generation, or any code that
manipulates AST node pointers (e.g., `TypeDeclPtr`, `ExpressionPtr`,
`FunctionPtr`, `StructurePtr`).

## AST nodes are gc_node — unique, no `inscope`, no `<-` for assignment

Every AST type — `TypeDecl`, `Expression` (every subclass: `ExprBlock`,
`ExprCall`, `ExprFor`, …), `Function`, `Structure`, `Enumeration`,
`Variable`, `MakeFieldDecl`, `MakeStruct`, every `Annotation` subclass —
is a **plain raw pointer** (`gc_node`), not a smart_ptr. Their lifetime
is owned by a `gc_root` (typically the Module), and the GC sweeps
unreachable nodes.

**Rules:**

- **No `var inscope`** for AST pointer types. Plain `var x = new ExprConstInt(...)` or `var x = clone_expression(e)`.
- **No `<-` for assignment** to AST node fields. Use plain `=`:
  - `fn.body = newBlock`
  - `td.firstType = elemType`
  - `field |> move_new <| expr` is also legacy — write `field = expr`.
- **No `move_new`, no `move`, no `add_ptr_ref` on AST pointers** — those were smart_ptr-era helpers. `add_ptr_ref` still exists for bridging to the residual `smart_ptr<T>` types (e.g., `smart_ptr<Program>`), not for AST.
- **Don't call `get_ptr(x)` on AST pointers** — `x` is already a raw pointer. `expr is ExprVar` works directly, `x.field` works directly, `x == null` works directly.
- **AST is unique — clone, don't copy.** Each AST node lives at exactly one location. Inserting the same `ExpressionPtr` into two parent expressions creates aliasing — both think they own the child, gc_collect walks it twice, mutations leak across, and the AST validator complains. To duplicate, use the matching `clone_*`:

  | Type | Clone fn | Notes |
  |---|---|---|
  | `TypeDeclPtr` | `clone_type(t)` | |
  | `ExpressionPtr` | `clone_expression(e)` | recursive deep clone |
  | `FunctionPtr` | `clone_function(f)` | returned via move: `var x <- clone_function(f)` |
  | `VariablePtr` | `clone_variable(v)` | |
  | `StructurePtr` | `clone_structure(s)` | (no `get_ptr(st)` wrapping needed) |

- **Don't substitute `clone_to_move`** for the type-specific `clone_*` — `clone_to_move` is the generic copy-then-move helper for non-copyable values like `array<T>`. AST pointers want the type-specific clone.
- **Tools/utilities that build AST at runtime** (outside the normal compile pipeline) must wrap the scope in `ast_gc_guard() { ... }` from `daslib/ast`, otherwise the leak detector reports `GC APP LEAK` at exit.

If you find yourself reading older guidance about `var inscope`, `<-`,
`move_new`, `add_ptr_ref` for AST types, the source is pre-migration.
The post-migration rules above are correct as of daslang 0.6.x.

## The few residual smart_ptr types — `Program`, `Context`, `FileAccess`

A small set of types are still `smart_ptr<T>` (refcounted with manual
addRef/releaseRef on the C++ side). These DO follow the older
`var inscope` / `<-` patterns:

- `ProgramPtr` = `smart_ptr<Program>`
- `ContextPtr` = `smart_ptr<Context>`
- `FileAccessPtr` = `smart_ptr<FileAccess>`
- `DebugAgentPtr`, `VisitorAdapterPtr` (from `make_visitor`) — internal

For these:

- **`<-` operator**: ALWAYS `memcpy(dest, src) + memset(src, 0)` — it is a raw memory operation, NOT smart_ptr-aware. It zeros the source regardless of type.
- **`move` function**: Bound via C++ `builtin_smart_ptr_move*` family in `module_builtin_runtime.cpp` — proper smart pointer move with reference counting. Use `move(dest, src)` for `smart_ptr<T>` transfers when the refcount needs to be tracked.
- **`return <- expr`**: Moves value to return slot and zeroes `expr`. If `expr` is a `&` ref parameter, this zeroes the *caller's* variable since they share memory.
- **Visitor adapters** (`make_visitor` returns `VisitorAdapterPtr`) need `var inscope adapter <- make_visitor(*v)` and an `unsafe` block at the call site — see `daslib/ast.das` for examples.

## Structure macros — generating types and functions

- **`[structure_macro(name=foo)]`** — annotation on a class inheriting `AstStructureAnnotation`; the `apply` method runs at compile time when a struct has `[foo]`. Use to generate companion types, operators, and functions.
- **`clone_structure(st)`** — deep-copies a `StructurePtr` for creating modified companion types (e.g., SOA layout where every field becomes `array<FieldType>`). Pass the pointer directly — no `get_ptr` wrapping.
- **`compiling_module() |> add_function(fn)`** — registers a concrete function in the current module
- **`compiling_module() |> add_generic(fn)`** — registers a generic function (instanced per call site)
- **`compiling_module() |> add_structure(st)`** — registers a generated struct
- **`compiling_module() |> add_alias(tdef)`** — registers a type alias
- **`fn.flags |= FunctionFlags.generated`** — marks a function as compiler-generated (suppresses "unused" warnings, enables special error messages)
- **`add_structure_field(st, name, typeDeclPtr, defaultExprPtr)`** — adds a field. Pass `clone_type(qmacro_type(type<int>))` for the type and `default<ExpressionPtr>` for "no default value":
  ```das
  st |> add_structure_field("count", clone_type(qmacro_type(type<int>)), default<ExpressionPtr>)
  ```
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
- **`$b(arrayOfExprPtr)`** — splice an `array<ExpressionPtr>` as a **block body** (sequence of statements). Build the array with `push <| qmacro_expr(...)`, then `$b(bodyExprs)` inlines all statements into the function body

### Default-initializing generated struct variables

In macro-generated code, `var x : $t(st)` fails with "uninitialized variable" for structs without field defaults. Use `default<T>` instead:

```das
// WRONG — fails if struct has uninitialized fields
blk |> push <| qmacro_block() { var entity : $t(st) }

// CORRECT — default-initializes all fields
blk |> push <| qmacro_block() { var entity := default<$t(st)> }
```

## Pattern-matching call shapes — never rely on leading `!`

When a macro walks an AST and pattern-matches specific call shapes to
route them differently (e.g. `_sql` detects nested
`select_from(...)._any(...)` and emits SQL `EXISTS`, vs. `_none(...)`
emitting `NOT EXISTS`), **do not rely on detecting `!expr` to flip the
emitted output**. Standing rule: AST walkers do not have leading-`!`
support. The `!` often sits across intermediate AST nodes —
parentheses, `if` expressions, local `let` bindings, constant-folded
wrappers — that break a naive pattern match. Pattern-matching `!any(...)`
is correct for some cases, silently wrong for any user that
parenthesizes or refactors the condition.

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
