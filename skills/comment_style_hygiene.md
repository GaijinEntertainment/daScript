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

**Teaching code narrates on purpose.** In tutorials and examples the prose IS the
deliverable: the cap and the narration ban apply to comments that add nothing to the line
they sit on (`// increment i`, a tail comment restating the call), not to section prose
carrying the lesson. Duplication still counts — a preamble and the tail comments telling
the same sequence twice is one telling too many.

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

Rules marked *(lintable)* are mechanical enough for a lint to enforce; das already has
some (noted), C++ and JS have none — there the rule is the reviewer's.

## Comments

**Short or absent.** 1–2 lines preferred, 3 the cap. A comment's volume is the number
of lines its text would occupy re-wrapped at the width the file's other comments use —
count those against the cap. das lint STYLE014 enforces the physical line count under
`daslib/` and wherever `options _comment_hygiene = true`; everywhere else the cap is
the reviewer's. The cap covers per-symbol and in-body comments; the file header — the
block before the first SYMBOL declaration, which is STYLE014's boundary — is governed by
the map rule below. Prologue lines (`options`, `module`, `require`, imports) are not
declarations; the header sits after them. In a file with no symbol declarations at all
(YAML, TOML, JSON-with-comments), the leading comment block is the header and everything
after the first data key is body. Over the cap, a comment stands only when its content alone shows why — an
injectivity argument, a wire-format contract, a miscompile it prevents; the test is
that a cold reader never asks "why is this comment so long?". Anything else over the
cap gets interrogated: does the detail belong at the use site, in the file-header map
(shared contract moves up, the symbol keeps what is specific to it) — or nowhere?

**No banners, no preambles.** No `// ===== name — desc =====` block above a function
that already carries its own doc-comment, and no section-head essay restating what the
code, the doc-comments, or the file-header map already carry — architectural WHY with
no home in the file goes to a design doc. Terse section dividers stay.

**A file header is a map, not an essay.** At or under the comment cap a header may stay
prose; past it, it either becomes a map — one line per fact of the file's contract — or
gets trimmed. A header may repeat per-symbol comments (a cold reader needs the shape
before any symbol), never text the code already prints. The map allowance is not
header-only: a comment whose lines are one-fact-each, sitting above a list-shaped
declaration, is a map too — capped by the list it describes, not by the prose cap.

**Private symbols don't get public-style docs.** Doc-comment syntax (`//!` and kin) is
for tooling-visible public API — a symbol a consumer outside the owning tree can require,
or one a doc generator reads. A symbol exported only for its sibling tests (a `_`-prefixed
test-support module beside them) takes the private-symbol bar. On a private symbol
a docstring restates the name to a reader who already has it; if there is a genuine
one-line WHY, write a plain comment. The bar for any comment on a private symbol: a
maintainer reading the symbol alone would be surprised without it.

**The body of a private function caps comments at ONE line** — a `def private`, a C++
static or anonymous-namespace helper, a NAMED non-exported JS function (das: STYLE015,
same gating as STYLE014). An anonymous body handed to a framework (a test callback, a
route handler) takes the ordinary 1–2-preferred / 3-cap instead. A second line stands only when its content alone shows why — the
same bar as *Short or absent* above; three or more never. Body-only: a comment
*attached to* a private symbol takes the ordinary cap plus the bar in
*Private symbols don't get public-style docs* above.

**Field comments ride the declaration line** *(lintable)*, never a line above — fewer
lines, and the struct scans as a table. A note that cannot fit the line belongs at the use
site; one that explains a GROUP of fields rather than any single one may sit above the
group, within the cap.

**Branch hints ride the branch line.** A few words on the `if` identifying what lands
in the branch (`// retries exhausted upstream, not here`). A WHY the line can't hold
sits directly above the `if`, two lines at most. A self-describing predicate — a named
helper, a compare against a named constant — gets nothing: restating `fn.neverInline`
as "explicit opt-out" is narration. The comment must say something the identifiers
don't.

**A hint inside a branch body rides its statement's line.** A note about one statement
sits on that statement's line (`pass` included); when that line has no room, directly
above that statement. A note spanning several statements is about the branch — it goes
on the `if` line or in the WHY above it.

**No incident citations.** The banned form is a citation of an event that can only be
verified outside the code — a PR or issue number, a date pinning when something broke or
was fixed, "proven: <module> lost <bug>", or a reference to a past state of the code
("came via X before the split", "moved here from Y"). A date stamping when the data pinned beside it
was captured or verified (a frozen expected-output fixture beside its ids, a
`probe-verified <date>` claim) points at no event and is not a citation. The required
form is the failure mode named in present tense at the code that guards it:
"a defaulted operand no call site can supply crashes simulation", not "used to crash".
A mechanism note stays when a maintainer needs it to change the code without breaking
the guard (why an encoding stays injective); it goes when it only argues the guard was
right.

**The message is the comment.** Any site that already carries a human-readable failure
string — a branch's error/decline/log, an assertion or panic message, a test
expectation — gets no comment restating it; put the fact the reader needs *into* the
string instead. This makes good diagnostics double as documentation — one more reason
to write them well.

**Counterpart pointers earn their line.** When code has a counterpart that must change
with it — the other half of a split condition, a block duplicated in a second file — a
few words naming where the counterpart lives are a good comment:
`if (isLiteral(a)) return true;   // variables resolve in BindingScan`. The pointer
rides the pointed-from line when it fits; when the pointed-from code is a whole region
— its half of a duplicated block — or the pointer overflows the line, it sits directly
above the pointed-from code, each copy carrying its own.
The discriminator against the consequence-note ban below: a pointer names WHERE the other
half lives, so a maintainer changing this line knows what else to open. The absence is a
finding too: two halves that must change together with no pointer on either half — fix by
adding the pointer to one half
(`// consumed by control.html's chat panel`); a note describing WHAT the effect looks like
downstream (`// the page shows its attach button on this`) is the banned kind.

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
that keeps looping and the walk-abort-in-callback form stay the reviewer's)*.
A bool initialized false, set on failure paths, then immediately
consumed by a single `if (flag) return/error` — collapse to a direct return at each
set site. The flag form often keeps looping after the answer is known and reports
repeatedly. When the set site is inside a callback or visitor that cannot return
through its caller, the collapse is a walk-abort instead: guard the callback's first
line on the flag.

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
normally. Section-marker comments may subdivide a large class.

**When in doubt: delete.** If reading the code plus the public doc-comments doesn't
make the WHY clear, the comment was load-bearing. Otherwise it was noise.
