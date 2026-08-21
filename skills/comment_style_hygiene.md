# Comment and style hygiene

Read this before writing or reviewing comments, names, or local code shape — in any
language. The rules are language-agnostic: examples lean C-family, but the shapes they
name appear in C++, das, JS, Python, and build scripts alike.

## Before everything

**Keep rules simple.** One rule here is one short paragraph. An entry that needs more
is describing an essay, not a shape — split it or drop it.

**This guide reviews itself: a rule a reviewer cannot apply as written is a defect of
this guide.** Mark it like any other finding — a guide defect blocks nothing, but its
fix (a rewrite or a removal, never silent tolerance) lands with the round's other
fixes.

## How this guide is applied

- **Findings speak plain.** A review finding — and the commit fixing one — states the
  problem and the fix in its own words ("this comment narrates the assignment below
  it"), never by citing this guide. The guide teaches writing; it is not an authority
  to invoke.
- **Exceptions justify themselves.** There is no exception marker; a rule's own text
  carries the boundary for when a departure stands.
- **The audit is not a gate.** Style review runs on every PR; findings persuade, lint
  compels. Fix or consciously decline — there is no re-run-until-clean loop.

Rules marked *(lintable)* are mechanical enough for a lint to enforce; where a rule notes
an existing lint, the lint compels — an unnoted rule is the reviewer's.

## Comments

**.das outside teaching code: ABSOLUTELY NO comments that are not documentation or lint
suppression.** The kept set is exactly: `//!` docs on public API (never on private
symbols), `// nolint:CODE` / `@nolint` suppressions carrying their one-line why, `//fmt:`
formatter directives, and the file's leading header block (which may sit below the
`options` / `module` / `require` preamble). Everything else — narration, banners, section
dividers, commented-out code — does not exist. The MCP `format_file` tool applies this
file-wide by default, fail-closed (a strip must compile or the file is restored). A `//`
comment outside the kept set is therefore EPHEMERAL: anything worth preserving becomes
code (a name, a `//!` doc, an assert) or lands in an `.md` beside the code. In tutorials
and examples the prose IS the deliverable — format those with `keep_comments='true'`;
comments that add nothing to their line still go.

**C and C++: no NEW comments.** Comments go stale, code does not — new C-family code says
it in a name, a shape, or a test, or documents itself in the module's `.md`. Kept when one
is earned: `//!` and `/** */` docs on public API, `NOLINT` / `clang-format` suppressions,
the leading header block. Existing comments answer to the deletion test below.

**Other languages: the test for every comment is deletion** — remove it and re-read; if the
WHY goes dark, it was load-bearing, restore it, otherwise it was noise. 1–2 lines is the
default, not a limit — a longer comment may be earned when the WHY is genuinely load-bearing
and has no better home; if a reader would ask "why is this long?", that is the finding.

**Outside `.das`, a file's leading header block (license, provenance) is kept; below it,
nothing decorates** — no banner blocks (rules of dashes or asterisks, boxed headers), no
up-front preamble essays. (For `.das` the kept set above governs.)

**No ceremonial doc templates on non-public symbols** — `@param`/`@return` boilerplate
restating a signature. Where docstrings are the idiom (Python), a private helper's
docstring is judged by the same deletion test as any other comment.

**No incident citations** — no PR numbers, no dates, no "used to crash"; name the failure
mode in present tense at the code that guards it.

**The message is the comment:** a site with a good error string needs no comment restating
it.

## Names

**A name beats a comment.** When a reader has to ask what a symbol is (`cfg` — member?
local? whose config?), fix the name (`moduleCfg` vs `callerCfg`), not the doc. Applies
double to members read inside long methods, where member-ness is invisible at the use
site.

**Magic numbers are comments in the wrong place.** A value whose meanings live in a
comment (`kind: 0 = eager, 1 = conditional, 2 = ...`, a `vector<bool>` whose true and
false need explaining) is an enum refusing to be written — name the values, and the
meaning-list dissolves into them at every use site.

**`can` / `is` / `has` document a lot.** `can` = would the operation succeed, `is` =
property of the thing, `has` = containment scan. A predicate named as its question
needs no header; a vague name (`fragile`, `worthIt`) is what forces the essay on top —
rename first, then see what's left to say.

**One name per concept** — in identifiers, comments, and messages alike. Three
synonyms for one idea ("timeout" / "deadline" / "budget") are three chances to
misread; pick the accurate one and sweep the rest.

**One mechanism, one family word.** When one mechanism surfaces as several public
names — a verb and its variants, a flag and the helpers that set it for one call —
one word runs through every member (`push` / `push_clone` / `push_from`). A member
named out of a second vocabulary makes the reader hunt for a difference that isn't
there.

**Diagnostics name the concrete thing.** If the code knows which one — which operator,
which type, which file — the message says which one; a category without an instance
reads as "trust us". User-facing messages carry no vocabulary that exists only inside
the implementation.

## Shape

**A flag set-then-returned is a return in disguise** *(partly lintable — STYLE041; a set
that keeps looping and the walk-abort-in-callback form stay the reviewer's)*. A bool
initialized false, set on failure paths, then immediately consumed by a single
`if (flag) return/error` — collapse to a direct return at each set site. The flag form
often keeps looping after the answer is known and reports repeatedly. When the set site is
inside a callback or visitor that cannot return through its caller, the collapse is a
walk-abort instead: guard the callback's first line on the flag.

**One guard style per function.** Message-setting early-outs are one-liners:
`if (cond) { why = "..."; return false; }` — never a *gratuitous* mix of one-liners
and expanded blocks in the same function. A guard whose message needs a local to
build it stays expanded, and that's not a mix.

**Declarations don't promise.** Declare at first use, not in an up-front batch — a
block of aliases computed long before their consumers is a list of promises the
reader must carry. When the values live in a struct already in scope, reading them at
the use site beats aliasing them at all.

**Member-assembly is a constructor written inline.** `Thing t; t.x = ...; t.y = ...;`
at every call site is a ctor the type refused to write — write it, and the null-checks
on "optional" members usually die with it.

**Class bodies are dense** *(lintable)*. No blank lines between data members — blank
separation is for top-level declarations; inline member-function bodies separate
normally. A one-line section marker may subdivide a large class body in languages whose
comments survive; in `.das` the kept set governs — no divider survives the formatter.
