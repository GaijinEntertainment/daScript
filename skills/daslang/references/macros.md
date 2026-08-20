# Macros and annotations

Macros are daslang classes that run **during compilation** and manipulate the syntax tree. AST
API: `daslib/ast`; registration helpers: `daslib/ast_boost`; building AST from source patterns
(reification): `daslib/templates_boost`; matching AST: `daslib/ast_match`.

## Compilation pipeline

Modules compile in `require` order; per module, errors in any phase stop the compile:

1. **Parse** — reader macros, on raw characters.
2. **Apply** — `apply` on every function / structure / enumeration annotation.
3. **Infer** — repeats until stable: `[pre_infer_macro]`, type inference (firing `transform`,
   call, variant, for-loop and type macros), `[dirty_infer_macro]`. Once the passes settle with no
   errors `[infer_macro]` runs; returning `true` restarts them, numbered from 0 again.
4. **Finish** — `finish` hooks. Fully typed; no more edits.
5. **Lint** — `lint` / `verifyCall`, `[lint_macro]` / `[global_lint_macro]`. Read-only.
6. **Optimize** — repeats: built-in optimization plus `[optimization_macro]`.
7. **Simulate** — the module's own context is created; `[_macro]` functions run.

## Registering a macro

**A macro cannot be used in the module that defines it** — the annotation fails with
`annotation <name> is not found`. Put macros in their own module and `require` it.

Annotating the class registers it; the optional `name` argument is the spelling users write,
defaulting to the class name.

```das
options gen2

module my_macros public

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost

[function_macro(name="trace")]
class TraceMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        func.body = qmacro_block() {
            print("{$v(string(func.name))} enter\n")
            $e(func.body)
        }
        return true
    }
}
```

For what the annotations do not cover, register manually: a `[_macro] def private setup` function
runs at the end of every module compile. Guard it with `is_compiling_macros_in_module("my_macros")`
so it fires once, then `add_new_function_annotation("note", new NoteMacro())` — there is an
`add_new_*_annotation` / `add_new_*_macro` entry point for every macro kind. `[_macro, private]`
is an error.

## Macro kinds

| Annotation | Base class | Hooks |
|---|---|---|
| `[function_macro]` | `AstFunctionAnnotation` | `apply` (pre-infer body rewrite), `transform` (rewrite calls to it), `verifyCall`, `generic_apply`, `patch`, `fixup`, `finish`, `lint`, `complete` |
| `[block_macro]` | `AstBlockAnnotation` | `apply`, `finish` — blocks / lambdas / local functions |
| `[structure_macro]` | `AstStructureAnnotation` | `apply`, `patch`, `finish`, `complete` |
| `[enumeration_macro]` | `AstEnumerationAnnotation` | `apply` |
| `[call_macro]` | `AstCallMacro` | `visit`, `preVisit`, `canVisitArgument`, `canFoldReturnResult` |
| `[reader_macro]` | `AstReaderMacro` | `accept`, `visit`, `suffix` |
| `[variant_macro]` | `AstVariantMacro` | `visitExprIsVariant` / `…AsVariant` / `…SafeAsVariant` (`is` / `as` / `?as`) |
| `[for_loop_macro]` | `AstForLoopMacro` | `visitExprFor` |
| `[capture_macro]` | `AstCaptureMacro` | `captureExpression`, `captureFunction`, `releaseFunction` |
| `[type_macro]` | `AstTypeMacro` | `visit` — custom `name(type<T>, N)` in type position |
| `[typeinfo_macro]` | `AstTypeInfoMacro` | `getAstChange`, `getAstType` — custom `typeinfo trait(type<T>)` |
| `[contract]` | `AstFunctionAnnotation` | `isSpecialized`, `isCompatible` |
| `[tag_function_macro(tag="t")]` | `AstFunctionAnnotation` | `transform` on calls to `[tag_function(t)]` functions |
| `[comment_reader]` | `AstCommentReader` | comment text plus `before*`/`after*` declaration hooks |
| `[simulate_macro]` | `AstSimulateMacro` | `preSimulate`, `simulate` |

**Pass macros** see the whole program and derive from `AstPassMacro` (there is no `AstLintMacro` /
`AstInferMacro`), with `apply(prog : ProgramPtr; mod : Module?) : bool` — plus, for
`[pre_infer_macro]` only, an optional `canVisitPass(prog, mod, index) : bool` (`false` skips that
pass; `index` is the pass number within the current inference run, 0 after every restart).
The annotation picks the phase: `[pre_infer_macro]`, `[infer_macro]`,
`[dirty_infer_macro]`, `[post_infer_macro]`, `[optimization_macro]`, `[lint_macro]` (per module),
`[global_lint_macro]` (once, after all modules), `[pre_simulate_macro]`, `[post_compile_macro]`
(after gc-root collection).

Inside a macro `compiling_module()` is the module being compiled (`mod` is the module owning the
macro). Walk it with `compiling_module() |> for_each_function("") $(var func : FunctionPtr) { … }`;
for expressions subclass `AstVisitor` (prefix/postfix callbacks per expression kind, postfix may
return a replacement) and run `make_visitor(*v) $(adapter) { visit(prog, adapter) }`. `visit`
accepts `ProgramPtr`, `FunctionPtr`, `ExpressionPtr` or `TypeDeclPtr`, **not** `Module?`.

A call macro's `visit` runs *after* its arguments, so inner macros have expanded and
sub-expressions carry a resolved `_type`. Fail with `macro_verify` (`daslib/macro_boost`);
returning null instead re-queues the macro.

```das
[call_macro(name="twice")]
class TwiceMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?;
                       var expr : ExprCallMacro?) : ExpressionPtr {
        macro_verify(length(expr.arguments) == 1, prog, expr.at, "twice takes one argument")
        return qmacro($e(expr.arguments[0]) * 2)
    }
}
```

A structure macro's `apply(var st : StructurePtr; …)` edits the declaration before inference:
`st |> add_structure_field("tag", clone_type(qmacro_type(type<int>)), default<ExpressionPtr>)`.
`compiling_module() |> add_function(fn)` / `add_generic` / `add_structure` / `add_alias` register
generated declarations.

A reader macro embeds foreign syntax as `%name~ ... %%`: `accept` takes one character at a time
(`false` at the terminator), then `visit` turns the collected text into AST. `suffix` instead
returns **daslang source** the parser re-reads — module level with `~`, expression position with
`!` (`%name! ... %%`).

## AST nodes are plain pointers

Every AST type — `TypeDecl`, every `Expression` subclass, `Function`, `Structure`, `Enumeration`,
`Variable`, `MakeFieldDecl`, `MakeStruct`, every `Annotation` subclass — is a **plain raw pointer**
tracked by the compiler's node GC, not a `smart_ptr`:

- Assign with plain `=` (`func.body = blk`); pass by value. **No `var inscope`, no `<-`** for AST
  pointers, and no `move_new` / `add_ptr_ref`.
- No `get_ptr(x)` — the value already is a pointer. `x == null`, `x.field`, `x is ExprVar` work.
- **`is` / `as` on a node are EXACT-type tests, not "kind of".** `expr is ExprField` is `false`
  when `expr` is an `ExprSafeField`, and `as` on the wrong type crashes. Enumerate every concrete
  node class, or use a matcher (below).
- **One parent per node** — inserting the same pointer twice makes every later pass edit it twice.
  To reuse a subtree, clone it: `clone_type`, `clone_expression` (deep), `clone_function`,
  `clone_variable`, `clone_structure` (not the generic `clone_to_move`).
- AST built at **runtime** (outside a compile pass) must sit inside `ast_gc_guard() { ... }` from
  `daslib/ast`, or the leak detector reports a GC leak at exit.

Types still refcounted as `smart_ptr` do use `var inscope` / `<-`: `ProgramPtr`,
`FileAccessPtr`, and the adapter `make_visitor` returns (the block form hides it). `ContextPtr`
is a C++-side typedef only — `var c : ContextPtr` does not compile. (probe-verified 2026-08-16)

An emitted `Variable` **must** have `_type` — `new TypeDecl(baseType = Type.autoinfer)` lets
inference fill it. An emitted `ExprVar` needs one too when it flows into a generic call;
`clone_expression` copies `_type` faithfully, so a null propagates to every clone.

Fixed arrays are a chain of `TypeDecl` nodes, one per dimension, element in `firstType`, outermost
first — size in `fixedDim` (`fixedDimExpr` while it is still an expression), never a `dim` field.
Build one with `make_fixed_array_type(total, element)` from `daslib/ast_boost`. A type macro's
payload arrives in `typeMacroExpr`.

## Reification — building AST from source patterns

`daslib/templates_boost` turns written daslang syntax into AST, `$…()` escapes splicing runtime
values in. The pattern is **not type-inferred**; it stays raw AST.

| Builder | Returns |
|---|---|
| `qmacro(expr)` | the expression |
| `qmacro_block() { stmts }` | `ExprBlock` (undecorated) |
| `qmacro <\| $(a, b) { stmts }` | `ExprMakeBlock` — a block literal, i.e. decorated |
| `qmacro_expr() { stmt }` | one statement as an expression — the way to quote `return`, `let`, … |
| `qmacro_block_to_array() { stmts }` | `array<ExpressionPtr>`, one element per statement |
| `qmacro_type <\| type<T>` | `TypeDeclPtr` |
| `qmacro_function("name") <\| $(args) { body }` | `FunctionPtr` (move-returned) |
| `qmacro_variable("name", type<T>)` | `VariablePtr` (move-returned) |
| `quote(expr)` | as `qmacro`, but **no** escapes — prefer it when there are none |

| Escape | Input | Substitutes |
|---|---|---|
| `$i(s)` | `string` / `das_string` | an identifier — declarations, uses, `for` iterator slots, block arg names |
| `$f(s)` | `string` | a field name: `obj.$f(s)` |
| `$c(s)` | `string` | a call name: `$c(s)(1, 2)` |
| `$v(x)` | any value | an expression producing that value (evaluated first) |
| `$e(x)` | `ExpressionPtr` | the expression, as-is |
| `$t(x)` | `TypeDeclPtr` | a type, in any type position |
| `$b(xs)` | `array<ExpressionPtr>` | each element as a statement, in order |
| `$a(xs)` | `array<ExpressionPtr>` / `array<VariablePtr>` | call arguments, or declared arguments, among the fixed ones |

```das
var nameOfVar = "scale"
var fun <- qmacro_function("madd") <| $ ( a, b ) {
    return $i(nameOfVar) * a + b
}
// def madd(a:auto const; b:auto const) : auto { return (scale * a) + b; }
```

`$e(...)` clones each substitution independently — pre-cloning with `clone_expression` is wasted
work even when one source feeds several slots. `describe(node)` renders any AST node back to source.

`$c(name)` cannot be qualified — `_::$c(name)(...)` is
`error[30151] syntax error, unexpected $c, expecting name`. The spliced name resolves at the splice
site, in the user's require chain. (probe-verified 2026-08-16)

## Matching AST — `qmatch`

`daslib/ast_match` is reification in reverse: the same tags, extracting instead of substituting.
Bind variables are declared **before** the call and filled only when the whole match succeeds, so a
qmatch ladder can reuse capture variables freely. Result:
`QMatchResult { matched : bool; error : QMatchError; expr : Expression const? }` — `expr` points at
the mismatched node.

```das
var expr = qmacro(a + b)
var lhs, rhs : ExpressionPtr
let r = qmatch(expr, $e(lhs) + $e(rhs))
if (r.matched) {
    print("{describe(lhs)} {describe(rhs)}\n")
}
```

`qmatch(expr, pattern)` matches one expression; `qmatch_block(blk) $ { stmts }` a block's
statements, optionally its arguments and return type (`$(x : int) : int { … }`, matched strictly);
`qmatch_function(func) $(args) : Ret { stmts }` a compiled function.

Tags: `$e(v)` captures a cloned sub-expression, `$i(v)` an identifier name, `$f(v)` a field name,
`$c(v)` a call name, `$v(v)` a constant (the variable's type selects the expected `ExprConst*`),
`$t(v)` a `TypeDecl?`, `$a(v)` the remaining arguments, `$b(v)` the statements a wildcard matched;
`_` is an anonymous wildcard. Statement wildcards in block/function patterns: `_wildcard()` (0+),
`_wildcard1()` (1+), `_optional()` (0 or 1), `_any()` (exactly 1), each taking an optional `$b(v)`.
`auto` in a type position matches any type; generic instantiations match by their original
(unmangled) name; a module-qualified call name matches an unqualified pattern on its last segment.

Both matchers see through the `ExprRef2Value` wrapper the typer inserts around field reads —
hand-written analyzers must peel it themselves. `daslib/match` is the complementary matcher, for
patterns spelled as node classes and fields rather than as source syntax.

## Annotations

Bracketed before the declaration; several combine with commas (`[export, no_aot]`).

**Function.** Lifecycle: `[export]` (callable from the host), `[init]` / `[init(tag=…)]` /
`[init(before=…)]` / `[init(after=…)]`, `[finalize]`, `[run]` (runs at compile time). Safety:
`[unsafe_deref]`, `[unsafe_operation]` (callers need `unsafe`), `[unsafe_outside_of_for]`.
Diagnostics: `[unused_argument(x, y)]`, `[nodiscard]`, `[deprecated(message=…)]`, `[no_lint]`,
`[sideeffects]`. Generics: `[generic]`, `[expect_ref]`, `[expect_dim]`, `[expect_any_vector]`,
`[local_only(arg=true)]`. Codegen: `[inline]`, `[never_inline]`, `[no_aot]`, `[no_jit]`, `[jit]`,
`[hybrid]`, `[alias_cmres]`, `[never_alias_cmres]`, `[pinvoke]`, `[type_function]`. Macro
plumbing: `[_macro]`, `[macro_function]` (exists only in the macro context), plus `[macro]` and
`[tag_function(t)]` from `daslib/ast_boost`. Inert carriers: `[hint(k=v)]`, `[marker]`.

**Structure / class:** `[cpp_layout]` (`pod=false` allows non-POD), `[persistent]`
(`non_pod=true`), `[safe_when_uninitialized]`, `[no_default_initializer]`, `[macro_interface]`,
`[comment]`, `[tag_structure(t)]`.

**Contracts** from `daslib/contracts` constrain generic arguments: `[expect_any_array]`,
`[expect_any_enum]`, `[expect_any_bitfield]`, `[expect_any_vector_type]`, `[expect_any_struct]`,
`[expect_any_numeric]`, `[expect_any_workhorse]`, `[expect_any_workhorse_raw]`,
`[expect_any_tuple]`, `[expect_any_variant]`, `[expect_any_function]`, `[expect_any_lambda]`,
`[expect_ref]`, `[expect_pointer]`, `[expect_class]`, `[expect_value_handle]`. They compose with
`&&` and negate with `!` — `[expect_any_array(a) && expect_any_numeric(b)]`,
`[!expect_any_array(a)]`. Careful: `expect_ref` exists both as a built-in and in
`daslib/contracts`, so with that module required the bare name is ambiguous.

**Field annotations use `@name` only** — the bracket form does not parse on a field. The metadata
is readable from `typeinfo` and from macros:

```das
class Foo {
    @big
    @min = 13
    @max = 42
    value : int
}
```

`@name` also goes on variables — after `let`/`var` (and `inscope`), before the name:
`var @exact_size buf : array<float>`, `let @tag = "x" n = 3` — and on parameters:
`def f(@scratch var a : array<int>&)`. It surfaces as `Variable.annotation` in macros and lints;
no runtime effect (probe-verified 2026-08-18).

Visibility is never an annotation: `def private helper`, `struct private Foo`, `enum private E`.

## Gotchas

- **Annotation-argument names allow exactly three keywords — `type`, `in`, `default` — plus
  plain names.** `[myanno(default = "x", type = "y", in = "z")]` compiles and the macro reads
  all three; any other keyword or type token is `error[30151]` (`@range = 5` fails — pick a
  synonym). (probe-verified 2026-08-16)
- **A generated `var x : $t(st)` with no initializer trips
  `error[31016] uninitialized variable is unsafe`** for any struct type — field initializers do
  not exempt it. Add `= default<$t(st)>` when the type is default-constructible; for a bound
  native type, where `default<>` is `error[50503] unsupported variable type`, set
  `flags.safeWhenUninitialized = true` on the cloned declaration type instead.
- **`options` are module-local for pass macros.** The macro fires once per module in the require
  chain and reads *that* module's options table.
- **Each macro module compiles into its own context**: compile-time globals one macro mutates are
  invisible to another macro module. Cross-macro state travels through the AST, files or the
  environment — never a shared global.
- **Never pattern-match a leading `!` to detect negation.** The `!` often sits behind parentheses,
  a `let`, or a folded wrapper. Ship explicit positive/negative name pairs (`_any` / `_none`).

## Debugging a macro

Slightly-wrong AST crashes a later inference or codegen pass, with no hint which macro did it.
`daslib/ast_verify` reports it at the offending node: `daslang --ast-verify prog.das`, or from a
macro `verify_module(prog, mod)` / `verify_expression(expr)` / `verify_function(fn)`. Each repairs
what it reports so the scan finishes; any report fails the compile. `--ast-verify` re-checks before
every inference pass, catching a break on the pass right after it happens; `--ast-verify-batch`
checks only the finished tree — cheaper for many files. Silence is not proof: the verifier only
knows shapes that crash the compiler. A macro that re-breaks the same node every pass never
converges — apply such a change once.

It enforces two invariants on anything a macro builds, both on by default: one parent per node, and
an `at` on **every** node. Without one, later diagnostics, profile rows and debug entries over that
node point at nothing; pass it at construction, and thread an `at` through node-building helpers
rather than defaulting it:

```das
var lit = new ExprConstInt(at = expr.at, value = 0)   // not new ExprConstInt(value = 0)
```

For a subtree you did not build node-by-node — a `$v()` value conversion, a clone from a native
binding — `stamp_missing_at(subtree, at)` fills only the empty locations; setting the root's `at`
alone leaves the leaves blank.
