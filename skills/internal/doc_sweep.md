# Doc sweep — verifying authored RST code blocks (repo-only)

Read this before running the each-release documentation sweep, before adding or editing
`.. das-doc:` markers on a doc page, and before extending `utils/internal/doc-verify/`. Repo-only:
it is about `doc/source/`, the checker tool, and the audit procedure.

The problem it solves: authored RST pages carry hand-written das code blocks that nothing
compiles, so they drift (the API moves under them) or are born wrong. `doc/reflections/`
das2rst output is generated and out of scope; everything hand-written under
`doc/source/reference/{tutorials,language,utils}` and `doc/source/stdlib/handmade` is in.

## Rule 0 — the checker binary carries the corpus

Before any page verdict counts, every module the corpus requires must load in the checker
binary. The tool probes this itself: companion-sourced requires that fail are a **binary
gap and abort the audit**; page-sourced requires that fail only red their page. Never
`--no-probes` an audit run. A "missing module" scare is as often a wrong require spelling
as a lean binary — the probe catches both.

## Running

```bash
bin/daslang utils/internal/doc-verify/main.das                      # full corpus, report.json + exit code
bin/daslang utils/internal/doc-verify/main.das -- --page 53_clargs  # one page (substring filter —
                                                           #   suffix with .rst to disambiguate)
```

Default out dir `build/doc_verify/`; use a private `--out` when iterating so parallel runs
don't clobber each other, and `rm` the report before a run you intend to read (a crashed
run leaves a stale report silently). Per-page synthetic modules land in `<out>/pages/` —
read the emitted `.das` to understand an error. Cascade tip: a missing `given` can surface
as macro exceptions (`error[31206]`) listed before the real
`error[30838] can't locate variable` — scan down the list first.

## The page model

A page is a literate program: its das blocks concatenate in document order into one
synthetic module. Declarations (def/struct/class/enum/variant/typedef/bitfield/tuple, and
`var private`/`let private`/shared/public globals) go to module scope; statements go into
one function so locals flow block to block; `require`/`options`/`module` lines hoist
(module lines first). The preamble comes from the page's companion tutorial
(`tutorials/<fam>/…`, several naming conventions, or the page's own directory for subdir
families) — companions are compile-gated ground truth.

Narrative re-declarations rename (`name__2`) — daslang bans shadowing. Typedefs and
exact-duplicate `def` headers rename too (annotations join a GENERIC def's identity, so
contract-differentiated overloads stay distinct); a class re-declared with a leading `...`
merges into the original when its modifiers match. String-literal text is never renamed
but `{interpolation}` regions are. `...` on a line of its own elides (`pass` in statement
position, dropped in decl bodies); inline `{ ... }` compiles as `{ pass }`; any other
inline `...` reaches the compiler — expand it to the companion's real form or mark.

## Markers

RST comment on its own line directly above the block; invisible when rendered.

| marker | meaning |
|---|---|
| `.. das-doc: skip` | not code at all (output dumps, wire formats, generated internals with unspellable `__`/backtick names) |
| `.. das-doc: fragment` | real syntax deliberately without context. Prefer given/member/alt — compiling code finds bugs that fragment hides (the dasHV family found 4 the moment its fragments became `member`) |
| `.. das-doc: signatures` | API-surface listing; compiler skips it, so YOU must verify each signature against the implementation before marking |
| `.. das-doc: fresh` | start a new program at this block (independent example; second `[export] def main`) |
| `.. das-doc: given <one-line das>` | page-wide context. `var`/`let` givens become module-scope globals (`inscope` stripped; block params may shadow them); type/def givens go to module scope; other statements run at the top of the main function. Givens seed the renamer, so an on-page decl of the same name renames — a follow-up block that needs the *given* binding back uses `alt` |
| `.. das-doc: alt` | compile this block as its own isolated program (hoists + givens + block only): equivalent-to restatements, alternative spellings, follow-ups reusing an inner-scope name |
| `.. das-doc: member <Class>` | block content lives inside the named class: hoists route out, `def override` bodies splice as members of a synthesized subclass, statements wrap in a `: auto` helper (so `return <expr>` tails compile) |
| `.. das-doc: file <name.das>` | the block IS a sibling module ("put this in helpers.das") — written next to the synthetic page so a bare same-dir require resolves it; repeated markers with the same name append (a literate tangle). A minted module shadows the companion's require of the real one — per page pick mint (page text verified) or companion (real macros verified), whichever compiles more |
| `.. das-doc: expect error[NNNNN]` | deliberate error demo appended to the page program; must FAIL with that code. Limits: page-hoisted requires apply (an error that depends on a require's absence can't be expressed — fragment it), and expect blocks are not renamed — give their defs unique names |

## Authoring rules

- Checkable code goes in `.. code-block:: das` — `::` literal blocks and `literalinclude`
  are invisible to the checker. Shell in `bash`, C++ in `cpp`, CMake in `cmake`, XML in
  `xml`: retag mislabeled blocks instead of marking them.
- Name example params/fields distinctly from page locals: the renamer can rewrite block
  parameter names, named-argument labels, and struct member names that collide with an
  aliased page local.
- `// output:` comments are claims — run the example and make them match. An output block
  whose snippet contains no `print` is lying.
- Quoted error codes and messages are claims too — probe them; renumbered diagnostics were
  one of the most common drift classes found.
- Two-line lead-in + indented list is a Sphinx `Unexpected indentation` error; keep
  lead-ins to one line or add a blank line.

## The each-release procedure

1. Full run of doc-verify; it must exit 0 (Rule 0 gates first). Triage any red: GENUINE
   (fix the RST — verify the correct form against the companion and module source, probe
   uncertain syntax before writing it) / MARKER (scaffolding) / HARNESS GAP (fix the tool;
   never mark around a tool defect).
2. Prose re-sweep: mechanical green does not verify prose. Scope the agent-read pass to
   pages whose subject area changed since the last sweep (git-dateable), with probe access
   and the same GENUINE/MARKER discipline. Companions' comments are not compile-gated —
   when a page and its companion carry the same wrong claim, fix both.
3. Regen traps: check `plans/doc-sweep.md`'s ledger before re-running any doc generator —
   `doc/reflections/gen_module_examples.py` in particular would revert the handmade
   fragments wholesale.

Backlog, ledger, and the wiring spec for the nightly lane live in `plans/doc-sweep.md`.
