# Macros and annotations

daslang macros are ordinary daslang classes that run **during compilation** and manipulate the
syntax tree. The AST API is `daslib/ast`; registration helpers are in `daslib/ast_boost`; building
AST from source patterns (reification) is `daslib/templates_boost`; matching AST is
`daslib/ast_match`. A working overview, gen2 throughout (gen2 is the default parser).

## Compilation pipeline

Modules compile in `require` order. Per module — any phase that reports errors stops the compile:

1. **Parse** — source becomes AST; reader macros run here, on raw characters.
2. **Apply** — `apply` on every function / structure / enumeration annotation.
3. **Infer** — repeats until stable: type inference, during which `transform`, call macros,
   variant macros, for-loop macros and type macros fire; then `[pre_infer_macro]`,
   `[infer_macro]`, `[dirty_infer_macro]` pass macros.
4. **Finish** — `finish` hooks. Fully typed; no more edits.
5. **Lint** — `lint` / `verifyCall` hooks, `[lint_macro]` / `[global_lint_macro]`. Read-only.
6. **Optimize** — repeats: built-in optimization plus `[optimization_macro]`.
7. **Simulate** — the module's own context is created; `[_macro]` functions run.

## Registering a macro

**A macro cannot be used in the module that defines it** — registration completes only when the
defining module finishes compiling. Put macros in their own module and `require` it; using the
annotation in the same file fails with `annotation <name> is not found`.

Preferred form: annotate the class and `daslib/ast_boost` registers it. The optional `name`
argument is the spelling users write; it defaults to the class name.

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

User code then writes `require my_macros` and tags a function `[trace]`.

Manual form, for what the annotations do not cover: a `[_macro] def private setup` function runs at
the end of every module compile. Guard it with `is_compiling_macros_in_module("my_macros")` so it
fires once, and register inside — `add_new_function_annotation("note", new NoteMacro())`. There is
an `add_new_*_annotation` / `add_new_*_macro` entry point for every macro kind. Visibility is the
prefix keyword `private`; `[_macro, private]` is an error.

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

**Pass macros** see the whole program and all derive from `AstPassMacro` (there is no
`AstLintMacro` / `AstInferMacro` class), with one method
`apply(prog : ProgramPtr; mod : Module?) : bool`. The annotation picks the phase:
`[pre_infer_macro]`, `[infer_macro]` (return `true` to re-infer), `[dirty_infer_macro]`,
`[optimization_macro]`, `[lint_macro]` (per module), `[global_lint_macro]` (once, after all
modules), `[pre_simulate_macro]`.

Inside any macro `compiling_module()` is the module being compiled now (`mod` is the module owning
the macro). Walk its declarations with
`compiling_module() |> for_each_function("") $(var func : FunctionPtr) { … }`; to walk
expressions, subclass `AstVisitor` (prefix/postfix callbacks for every expression kind, postfix may
return a replacement) and run `make_visitor(*v) $(adapter) { visit(prog, adapter) }`. `visit`
accepts a `ProgramPtr`, `FunctionPtr`, `ExpressionPtr` or `TypeDeclPtr`, **not** a `Module?`.

A call macro's `visit` runs *after* its arguments were visited, so inner macros have expanded and
typed sub-expressions carry a resolved `_type`. Fail loudly with `macro_verify`
(`daslib/macro_boost`) rather than returning null — a silent null re-queues the macro and becomes a
confusing inference cascade later.

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

A structure macro's `apply(var st : StructurePtr; …)` edits the declaration before inference, so
generated members behave like written ones:
`st |> add_structure_field("tag", clone_type(qmacro_type(type<int>)), default<ExpressionPtr>)`.
`compiling_module() |> add_function(fn)` / `add_generic` / `add_structure` / `add_alias` register
generated declarations.

A reader macro embeds foreign syntax, invoked as `%name~ ... %%` (e.g.
`let names <- %csv~ Alice, Bob, Carol %%`): `accept` takes one character at a time (return `false`
at the terminator), then `visit` turns the collected text into AST. The alternative `suffix` hook
instead returns **daslang source** the parser re-reads — at module level with `~`, in expression
position with `!` (`%name! ... %%`).

## AST nodes are plain pointers

Every AST type — `TypeDecl`, every `Expression` subclass, `Function`, `Structure`, `Enumeration`,
`Variable`, `MakeFieldDecl`, `MakeStruct`, every `Annotation` subclass — is a **plain raw pointer**
tracked by the compiler's node GC, not a `smart_ptr`:

- Assign with plain `=` (`func.body = blk`, `td.firstType = elem`); pass by value. **No `var
  inscope`, no `<-`** for AST pointers, and no `move_new` / `add_ptr_ref`.
- No `get_ptr(x)` — the value already is a pointer. `x == null`, `x.field`, `x is ExprVar` work.
- **Unique ownership**: a node lives under exactly one parent. To reuse a subtree, clone it —
  `clone_type`, `clone_expression` (deep), `clone_function`, `clone_variable`, `clone_structure`
  (not the generic `clone_to_move`).
- A tool that builds AST at **runtime** (outside a compile pass) must wrap its scope in
  `ast_gc_guard() { ... }` from `daslib/ast`, or the leak detector reports a GC leak at exit.

```das
let at = LineInfo()
var sum = new ExprOp2(at = at, op := "+",
    left  = new ExprConstInt(at = at, value = 2),
    right = new ExprConstInt(at = at, value = 40))
// the same node must not sit under two parents
var twice = new ExprOp2(at = at, op := "+", left = sum, right = clone_expression(sum))
```

The few compiler types still refcounted as `smart_ptr` do use `var inscope` / `<-`: `ProgramPtr`,
`ContextPtr`, `FileAccessPtr`, and the adapter `make_visitor` returns (the block form hides it).

A `Variable` you emit **must** have `_type` set — `new TypeDecl(baseType = Type.autoinfer)` lets
inference fill it. Give an emitted `ExprVar` a `_type` too when it flows into a generic call:
`clone_expression` copies `_type` faithfully, so a null propagates to every clone.

## Reification — building AST from source patterns

`daslib/templates_boost` turns written daslang syntax into AST, with `$…()` escapes splicing
runtime values in. The pattern is **not type-inferred**; it stays raw AST.

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

Splice inputs are cloned for you — `$e(...)` clones each substitution independently, so
pre-cloning with `clone_expression` is wasted work even when one source feeds several slots.
`describe(node)` renders any AST node back to source, the workhorse for debugging macro output.

## Matching AST — `qmatch`

`daslib/ast_match` is reification in reverse: the same tags, extracting instead of substituting.
Prefer it over hand-rolled `is X` / `as X` ladders. Bind variables are declared **before** the call
and filled only when the whole match succeeds — a failed match leaves every binding untouched, so
a qmatch ladder can reuse capture variables freely. The result is
`QMatchResult { matched : bool; error : QMatchError; expr : Expression const? }`, whose `expr`
points at the mismatched node.

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

Bracketed before the declaration; several combine with commas (`[export, no_aot]`); some take
arguments.

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

**Field annotations use `@name` only** — the bracket form does not parse on a field. They attach
metadata readable from `typeinfo` and from macros:

```das
class Foo {
    @big
    @min = 13
    @max = 42
    value : int
}
```

Visibility is never an annotation: write `def private helper`, `struct private Foo`,
`enum private E`.

## Gotchas

- **Annotation argument names cannot be grammar keywords.** `[myanno(default = "x")]` is a syntax
  error; pick a synonym (`kind`, `fallback`).
- **`options` are module-local for pass macros.** The macro fires once per module in the require
  chain and reads *that* module's options table; an option set in one file does not cascade.
- **Each macro module compiles into its own context**, so compile-time globals one macro mutates
  are invisible to another macro module. Cross-macro state travels through the AST, files or the
  environment — never a shared global.
- **Never pattern-match a leading `!` to detect negation.** The `!` often sits behind parentheses,
  a `let`, or a folded wrapper. Ship explicit positive/negative name pairs (`_any` / `_none`).

## Debugging a macro

Slightly-wrong AST does not fail where the mistake is — it crashes passes later inside inference or
codegen, with no line number and no hint which macro did it. `daslib/ast_verify` turns that into a
diagnostic at the offending node: `daslang --ast-verify prog.das`, or from inside a macro
`verify_module(prog, mod)` / `verify_expression(expr)` / `verify_function(fn)`. Each repairs what
it reports so the scan finishes. It only knows shapes that crash the compiler, so silence is not
proof; and a macro that re-breaks the same node every pass never converges — apply such a change
once.
