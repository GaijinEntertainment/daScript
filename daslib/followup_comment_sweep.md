# Comment-sweep follow-up ledger — daslib

TODO-grade findings surfaced by the rescue passes: comments whose content was a live
question, a recorded workaround, or a suspected defect. Each entry is triage material,
not documentation — it leaves this file by being fixed, probed to a verdict, or dropped
with a stated reason.

## Suspected defects (probe first)

- `faker.das:week_day` — month-convention mismatch: `afterFeb` tests 1-based `month > 2`
  while `offset[month]` indexes 0-based, and the only caller passes `month % 12u`;
  hand-check says 2028-03-01 yields 2 where the reference algorithm yields 3.
- `decs_boost.das:append_index_lookup` — an abandoned `tFixedArray` guard and its
  `macro_error` are commented out, so a fixed-array component in an eid query silently
  takes the ref path instead of failing closed.
- `flat_hash_table.das` — nothing normalizes or rejects a `hashFunctionName` returning
  0 or 1 (the empty/tombstone sentinels); `uint64(key)` as a hash silently loses entries.
- `fio.das:set_mtime_result` — the parameter type must be spelled `clock`; `Time`
  resolved to nothing and left the `[generic]` overload silently uncallable. Generalize:
  an unknown type name in a generic signature should be a diagnostic, not a dead overload.
- `debug.das:DAgent.onSingleStep` (step-over arm) — hand-inlines `wait_for_resume` with
  the opposite ordering (`afterPause()` before clearing `continueRequested`); recorded as
  deliberate but unproven.
- `daslib/decs.das` RST drift — `EntityId.id` gets the struct-level doc, `generation`
  gets `id`'s; `generation`'s own doc was spelled `///` and never reached the RST.

## Recorded open questions (from stripped TODO comments)

- `builtin.das` — five `unsafe(resize(...))` sites carry "is this safe? do we need init
  first?"; `clone(var args; var nargs)` has a commented-out `delete nargs` with an
  undecided fix.
- `sort_boost.das:QsortMacro.visit` — only `baseType == tBlock` is checked, not the
  block's argument/result types.
- `decs.das` — `cmp |> set("eid", eid)` after the user lambda marked "necessary?" at two
  sites; five panicking int-width caps queued for the int64 widening follow-up.
- `spoof.das` — `#`-string rule returns raw contents with an open "do we need unescape?"
  in both grammars.
- `safe_addr.das:temp_addr` — whole macro block commented out; the recorded hazard
  (`temp_addr(a[0])` then `resize`) dies with the strip. Delete or re-land behind a test.
- `cpp_gen.das` — alias-factory emitter exists only as a commented-out recipe
  (`alias_types` config silently omits the type); `[sideeffects]` on `onLinkageSpec`
  required for an unknown reason; bitfield fields skipped; mangled names emitted
  decorated.
- `aot_cpp.das` — `paranoid_validation` guards an empty block (`validateAotCpp` never
  ported); `aot_prefixes` looks always-empty; `needRenaming` returns true
  unconditionally; parent-cast TODO in `makeLocalTempName`; `visitExprOp2` fails on
  soa.das's `that.arguments |> clear()`; avoids `require math` because it cascades into
  match-based type-table inference (bit-pattern NaN/inf tests exist only for that).
- `templates_boost.das:apply_template` — "figure out why this is needed" on the
  local-rules/delete dance, three overloads.
- `jobque_boost.das` — clone-structure macro clones fields without a type check.
- `archive.das` — preallocated-string-with-RAII read path pending.
- `debug.das` — linear `getContextDataFor` lookup per instrument pass; a bare
  `TODO: , false` in `reqVariables`; `dap.das` lacks the spec's `accessTypes`.
- `regex_boost.das:RegexReader.visit` — macro-time compiled-regex conversion wants AOT.
- `flatten.das:STD_WHITELIST` — recorded intent to become a `FlattenConfig` argument;
  only the `[flatten]` annotation path is hardcoded.
- `quote.das:blacklist` — `at : LineInfo` fields of four node kinds skipped "for now".
- `shader_lingua_franca.das` — the module's semantic doc surface lived in `//` comments;
  at least one silent-wrong-answer trap is now undocumented: `mod(x,y)` is
  sign-of-divisor (OpFMod), `fmod` sign-of-dividend (OpFRem).

## Lint candidates (rules the sweep surfaced)

- Forward `erase(i)` while iterating the same container ascending (flatten_opt walks
  backwards at three sites; typemacro_boost hand-writes the reverse form three times).
- A `$e(subexpr)` spliced into more than one emitted statement without a local-ref bind.
- Printable-ASCII integer literal passed where a character is expected (`find(s, 58, 0)`).
- `slice(s, N)` where N is a literal equal to the length of a string literal just tested
  by `starts_with` on the same value.
- Empty statement-list arm/block left behind (post-strip `else { }` shape).
- A `///` or trailing-`//` doc on a public symbol or struct field — mis-spelled `//!`;
  the formatter strips it and the RST never sees it.
- `for (ch in <string>)` loop byte used as an index or compared unsigned without
  `uint8(...)` — signed under JIT.
- `LineInfo.fileInfo` dereferenced without a null guard.
- `addr(container[0])` with no preceding emptiness guard in the function.
- A `string` materialized from a `das_string` inside a visitor callback.
- SQL text built by interpolating a value instead of binding a parameter.
- A `return` inside an `apply`/`apply_imm` block argument (kills inlining).
- A `require` referenced only from qmacro-emitted code reads as unused to STYLE029/030.
- Shift `1ul << n` where `n` is not provably < 64.
