---
slug: call-macro-typeof-arg-struct-fields-annotations
title: in a call_macro how do I parse a type<Foo> argument and read the struct fields + annotations to synthesize code?
created: 2026-05-20
last_verified: 2026-05-20
links: []
---

For a `[call_macro]` that takes `type<Foo>` as an argument (where `Foo` is a struct):

```das
let typeExpr = expr.arguments[0] as ExprTypeDecl     // cast — fails if user didn't write type<...>
let td = typeExpr.typeexpr                            // TypeDecl
macro_verify(td != null && td.baseType == Type.tStructure, ...)
let st = td.structType                                // StructurePtr
```

For struct annotations + fields:
```das
// Annotations on the struct
for (ann in st.annotations) {
    if (ann.annotation.name == "my_marker") {
        let argList = ann.arguments      // AnnotationArgumentList — pass to your prefix/arg-extraction helpers
        ...
    }
}
// Fields
for (fld in st.fields) {
    fld.name      // das_string
    fld._type     // TypeDecl — clone_type before stuffing into new AST
    fld.init      // ExpressionPtr (default value, may be null) — clone_expression(fld.init) preserves null
    fld.at        // LineInfo
}
```

**Key gotchas:**
- `expr.arguments[0]` is `ExprTypeDecl` (cast directly), NOT `ExprMakeBlock`. Both `_type` and `(arg as ExprTypeDecl).typeexpr` give the TypeDecl — prefer `typeexpr` when you cast.
- Clone before reuse: `clone_type(fld._type)` / `clone_expression(fld.init)`. AST nodes have unique ownership — putting `fld._type` directly into a new TypeDecl's `argTypes` would dual-parent it.
- `fld._type.flags.ref = false` / `flags.constant = false` strips ref/const after cloning — needed when materializing into a heap-stored tuple.
- Reference impl: [daslib/decs_boost.das:707](daslib/decs_boost.das#L707) `FromDecsTemplateMacro` and the older block-arg form `FromDecsMacro` at line 633. Prefix-from-annotation helper `decs_prefix` at line 142 takes the `AnnotationArgumentList` from `ann.arguments`.

Also for cross-referencing: `decltype` macro at [daslib/templates.das:30](daslib/templates.das#L30) shows the simpler `expr.arguments[0]._type` form when you don't need to insist on a literal `type<...>` and accept any type-producing expression.

## Questions
- in a call_macro how do I parse a type<Foo> argument and read the struct fields + annotations to synthesize code?
