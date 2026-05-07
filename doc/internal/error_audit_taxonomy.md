# Error-tag taxonomy for daslang error-code audit

This is the canonical vocabulary that ALL inventory agents share. The output
column `tag_composition` MUST draw tags only from these lists. If a site
genuinely needs a tag not present here, add it to `tag_composition` AND emit
an `UNCLASSIFIED:<reason>` note in the `wording_change` column so we can
review and add to taxonomy.

The proposed enum name is composed in fixed order:

    <facet>_<subject>[_<modifier>]

So `function | argument | type | invalid` -> `invalid_function_argument_type`.
Multi-subject errors order by importance: function > argument > type, etc.
When unsure, prefer the order shown below (more general -> more specific).

---

## Facets (the kind of fault)

| facet | meaning | examples of old codes |
|---|---|---|
| `invalid` | the construct itself is malformed | invalid_type, invalid_argument_type, invalid_swizzle_mask |
| `lookup` | identifier didn't resolve to anything (was: `<x>_not_found`) | type_not_found, variable_not_found, function_not_found |
| `ambiguous` | identifier resolves to multiple candidates | function_not_found_ambiguous |
| `already_declared` | name collision | type_alias_already_declared, structure_already_declared |
| `cant` | operation forbidden in this context (was: `cant_<verb>`) | cant_get_field, cant_index, cant_dereference, cant_be_null |
| `mismatching` | two things expected to match don't | mismatching_parentheses, mismatching_curly_bracers |
| `not_resolved_yet` | follow-on; might resolve in a later pass | (new — replaces several "is not fully resolved yet" sites) |
| `internal` | compiler invariant violation; not user fault | (new — for current `unspecified` validate_ast / changed-fn sites) |
| `exceeds` | size/count limit busted | too_many_arguments, integer_constant_out_of_range, comment_contains_eof |
| `unsafe` | requires `unsafe()` block | (existing CompilationError::unsafe code) |
| `runtime` | exception during compile-time evaluation | exception_during_macro |
| `missing` | required thing absent (return path, init, etc) | not_all_paths_return_a_value |
| `recursion` | unbounded / forbidden recursion | (currently emitted as 4-arg uncoded errors) |
| `runtime` | exception/error during the runtime side of compile-time eval (DataWalker, hash, serializer) | (added 2026-05-06) |
| `performance_lint` | perf-lint warning — distinct output channel from primary errors | (added 2026-05-06) |
| `style_lint` | style-lint warning | (added 2026-05-06) |

## Subjects (what construct)

```
type, name, function, argument, result, parameter, body,
structure, field, class, interface,
enumeration, enumerator,
variable, global, local, constant,
type_alias, bitfield,
array, table, tuple, variant,
lambda, generator, block,
macro, module, require, options, annotation,
expression, statement, label,
file, line, escape,
string, comment, indentation,        // added 2026-05-06 for parser/lexer
iterator, pointer, buffer, context,  // added 2026-05-06 for runtime/builtin
```

If a subject is missing, add it AND flag `UNCLASSIFIED:<subject>` in
`wording_change` so we can add to taxonomy on review.

## Modifiers (rare, only when needed to disambiguate)

```
mix      — heterogeneous types (e.g. invalid_argument_count_mix)
ref      — reference variant
ambiguous, sealed, override, void, auto,
loop, redirect,
```

---

## Classification (in column `classification`)

One of:

- `primary` — first-fault, user-facing, should always emit
- `follow_on` — fires *because* of an earlier primary; should be `not_resolved_yet`-tagged or `verbose`-gated
- `mid_inference` — emits every infer pass; expensive describe()/candidate computation; should be `verbose`-gated
- `internal` — invariant violation / compiler-bug path; should carry an `internal_*` tag
- `wording_fix` — text needs cleanup (typos, vague phrasing) regardless of code
- `recategorize` — current cerr is wrong; needs reassignment

A site can have ONE classification. If multiple apply, pick the most
significant in this order: `internal > mid_inference > follow_on > recategorize > wording_fix > primary`.

## CSV format (one row per `error(...)` call site)

Columns in order, comma-separated, fields with commas/quotes/newlines wrapped in
double-quotes (CSV standard):

```
file, line, current_cerr, message_template, context_function,
classification, tag_composition, proposed_enum_name, wording_change
```

- `file` — repo-relative path, forward slashes (e.g. `src/ast/ast_infer_type.cpp`)
- `line` — 1-based line number of the `error(` call
- `current_cerr` — enum value passed (e.g. `type_not_found`, `unspecified`); empty if not passed
- `message_template` — first ~80 chars of the error message; replace `+ var +` and string concat with `<var>`; trim trailing punctuation
- `context_function` — surrounding C++ function name (best effort)
- `classification` — one of the six labels above
- `tag_composition` — pipe-separated tags from facets/subjects/modifiers, e.g. `invalid | function | argument | type`
- `proposed_enum_name` — the composed name, e.g. `invalid_function_argument_type`
- `wording_change` — empty if no change; else terse note (e.g. `typo: subexpresion -> subexpression`); UNCLASSIFIED markers go here too
