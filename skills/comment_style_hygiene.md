# Comment and style hygiene

Read this before writing or reviewing comments, names, or local code shape — in any
language. The rules are language-agnostic: examples lean C-family, but the shapes they
name appear in C++, das, JS, Python, and build scripts alike.

## Before everything

**Do not narrate code with comments.** If what a comment says comes out of the code
for free — or with minor reasoning — it is narration; delete it. The test for every
comment that remains: **does it add information the reader would otherwise have to go
look for?** Every rule below is this test applied to a specific shape. Style target:
less War & Peace, more O. Henry.

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
- **Exceptions justify themselves.** There is no exception marker. A comment that
  needs to be long must make the reason obvious from its content alone — an
  injectivity argument, a wire-format contract, a miscompile it prevents. If a reader
  who never saw this guide would ask "why is this comment so long?", that is the
  finding.
- **The audit is not a gate.** Style review runs on every PR; findings persuade, lint
  compels. Fix or consciously decline — there is no re-run-until-clean loop.

Rules marked *(lintable)* are mechanical enough for a lint to enforce; das already has
some (noted), C++/JS lints are follow-up work.

## Comments

**Short or absent.** 1–2 lines preferred, 3 the cap — das lint STYLE014 enforces it under
`daslib/` and wherever `options _comment_hygiene = true`; everywhere else the cap is the
reviewer's. It covers per-symbol and in-body comments; a file-header map (below) is exempt —
its test is prose-vs-enumeration, not length. A comment that doesn't fit is the signal to
interrogate it: why does this need prose at all, and does the detail belong at the use
site — or nowhere?

**No banners, no preambles.** No `// ===== name — desc =====` block above a function
that already carries its own doc-comment, and no multi-paragraph architectural essay
at the head of a section. Code reads well; design docs and tutorials carry the WHY.
Terse section dividers stay.

**A file header is a map, not an essay.** A pass or subsystem file may open with a
numbered overview — one line per fact — stating the whole contract: when it runs, what
it consumes and produces, the kinds and tiers it deals in. A map legitimately repeats
what per-symbol comments say; a reader entering cold needs the shape before any
symbol. What stays banned is prose — paragraphs argue, maps enumerate.

**Private symbols don't get public-style docs.** Doc-comment syntax (`//!` and kin) is
for tooling-visible public API. On a private symbol a docstring restates the name to a
reader who already has it; if there is a genuine one-line WHY, write a plain comment.
The bar for any comment on a private symbol: a maintainer reading the symbol alone
would be surprised without it.

**Field comments ride the declaration line** *(lintable)*, never a line above — fewer
lines, and the struct scans as a table.

**Branch hints ride the branch line.** A few words on the `if` identifying what lands
in the branch (`// retries exhausted upstream, not here`). A WHY the line can't hold
sits directly above the `if`, two lines at most; prose inside the branch body is
never right. A self-describing predicate — a named helper, a compare against a named
constant — gets nothing: restating `fn.neverInline` as "explicit opt-out" is
narration. The comment must say something the identifiers don't.

**No incident citations.** The banned form is a citation only a reviewer can check —
a PR or issue number, a date, "proven: <module> lost <bug>". The required form is the
failure mode named in present tense at the code that guards it: "a defaulted operand
no call site can supply crashes simulation", not "used to crash". A mechanism note
stays when a maintainer needs it to change the code without breaking the guard (why
an encoding stays injective); it goes when it only argues the guard was right.

**The message is the comment.** Any site that already carries a human-readable failure
string — a branch's error/decline/log, an assertion or panic message, a test
expectation — gets no comment restating it; put the fact the reader needs *into* the
string instead. This makes good diagnostics double as documentation — one more reason
to write them well.

**Counterpart pointers earn their line.** When a condition handles one half of a split
responsibility, a few words naming where the other half lives is a good same-line
comment: `if (isLiteral(a)) return true;   // variables resolve in BindingScan`.

**Define a term once, on the signature that coins it.** A predicate family's first
member carries the tail comment defining the term; siblings say `// same, for X`. When
the name alone carries the meaning, no header at all — a comment the reader
understands the function *despite* is worse than none.

**Show the transform.** For anything that rewrites code or data, a two-line
before/after example beats a paragraph: `x = f(a) + 1  =>  x = (a * 2) + 1`. Prose
stays only for the hazard the example can't show.

**A danger named is a disposition stated.** "There is a hazard" without "and therefore
the code refuses / hoists / defers / renames" is just ominous — the reader can't tell
a bug-we-prevent from a bad-output-we-sometimes-emit. Every hazard comment ends with
what the code does about it.

**A comment describes the code it sits on** — not consequences realized elsewhere, not
what a later pass will do. Emergent-behavior notes go at the line that produces them,
if anywhere; forward references to later phases go nowhere.

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

**Diagnostics name the concrete thing.** If the code knows which one — which operator,
which type, which file — the message says which one; a category without an instance
reads as "trust us". User-facing messages carry no vocabulary that exists only inside
the implementation.

## Shape

**A flag set-then-returned is a return in disguise** *(lintable — STYLE041)*.
A bool initialized false, set on failure paths, then immediately
consumed by a single `if (flag) return/error` — collapse to a direct return at each
set site. The flag form often keeps looping after the answer is known and reports
repeatedly. When the set site is inside a callback or visitor that cannot return
through its caller, the collapse is a walk-abort instead: guard the callback's first
line on the flag.

**One guard style per function.** Message-setting early-outs are one-liners:
`if (cond) { why = "..."; return false; }` — never a mix of one-liners and expanded
blocks in the same function.

**Declarations don't promise.** Declare at first use, not in an up-front batch — a
block of aliases computed long before their consumers is a list of promises the
reader must carry. When the values live in a struct already in scope, reading them at
the use site beats aliasing them at all.

**Member-assembly is a constructor written inline.** `Thing t; t.x = ...; t.y = ...;`
at every call site is a ctor the type refused to write — write it, and the null-checks
on "optional" members usually die with it.

**Class bodies are dense** *(lintable)*. No blank lines between data members — blank
separation is for top-level declarations; inline member-function bodies separate
normally. Section-marker comments may subdivide a large class.

**When in doubt: delete.** If reading the code plus the public doc-comments doesn't
make the WHY clear, the comment was load-bearing. Otherwise it was noise.
