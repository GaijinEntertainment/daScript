# REVIEW.md - the common contract

Every folder-scoped `REVIEW.md` is a binding per-change review checklist for the files under
its folder: run it on every such change before it ships, including a change to the checklist
itself. This file is the part all checklists share; a checklist's own text carries only its
folder's criteria, and its opening block points here. An external repo that adopts `REVIEW.md`
vendors a copy of this file at its root. A statement's home is the folder's architecture doc -
`ARCHITECTURE_COMMON.md` (repo root) is that document type's contract.

**What stays in a checklist: rules that can be checked against a diff. Nothing else.** A
reader must be able to apply every rule **without reading the code and without prior knowledge
of the folder.**

**A term of art is defined in place, in a dozen words, or the rule is rewritten around plain
words.**

**A checklist never quotes, cites, or restates another rule document** - coupling between rule
documents is ROUTING ("a diff touching X applies that checklist too", "a `[tune]` change is
reviewed with `skills/tune.md`").

**Every checklist reviews itself: a rule a reviewer cannot apply as written is a defect of the
checklist.** Mark it like any other finding - a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**New functionality ships with tests - same PR, no follow-up promises.** A new or changed
reachable branch ships a test that fails without it; a diff that adds a branch no test
distinguishes is a defect. The audit procedure - including how to settle "would this test fail
without the change?" - is `skills/tdd_audit.md`.

**A diff that changes a function carrying `[arch(at="<doc>#<anchor>")]` - beyond comments -
audits the anchor's other citing functions and the cited section: read the section, check it
still describes the code and each citer still conforms, verdict per function.** The citers of
one anchor share one mechanism; MCP `arch_sites` lists them.

**A diff that adds, removes, or retargets an `[arch(at=...)]` citation audits the cited
section against the code - both sections on a retarget.** The citation claims the section
describes this function; verify it does.

**A diff that changes an anchored section audits every function citing that anchor.** One
audit pass over an anchor's section text and citer set discharges every audit duty the diff
triggers on that anchor - the duties never cascade.

**A diff that moves a fact out of a function's comment into an `ARCHITECTURE*.md` lands three
things in the same change: the statement, a `{#anchor}` on the heading of the section it joins,
and an `[arch(at=...)]` citation on that function - and the comment goes.** The citation is
what keeps the section true: a section nothing cites is never re-checked when the code changes.

**A rule that a test, a lint, or the folder's `REVIEW.das` enforces is deleted.** Automation
replaces the rule; the checklist keeps at most "weakening that check is a defect." A rule
that COULD be automated is a lint or `REVIEW.das` candidate - say so in the review round.

**A `REVIEW.das` runs standalone and reports in one shape** - `daslang <folder>/REVIEW.das`
from the repo root prints one `path: finding` line per finding (`path:line: finding` when a
line is known) and exits nonzero when any fired. A gate that reports otherwise is a defect.
Support library: `dastest/review_gate`.

**Every review round runs the folder's `REVIEW.das` before it reads the diff, and a red gate
stops the round until the gate is green.**

**A `REVIEW.das` without its sibling `REVIEW.md` is a defect.**

**A rule is absolute; sanctioned violations live in the architecture doc.** No "except",
"exempt", "carve-out", or "the one sanctioned" - move the boundary inside the trigger, fix
the code so the exception has no reason to exist, or ledger the case in the architecture doc.
A ledgered case firing the absolute rule is expected: the author answers "yes, ledgered," and
that handshake is the system working.

**A rule lives in the checklist of the narrowest folder that contains its trigger.**
Subfolder-scoped rules move to that subfolder's own `REVIEW.md` - creating it is never an
objection; checklists are cheap, thousands can sit in a repository and few fire per PR. A rule
whose trigger lies entirely outside the folder can never fire and is deleted.

**A parent checklist routes strays by KIND, not by location** - "a `[test]` file, wherever the
diff puts it, answers to the `tests/` subfolder's checklist".

**One rule is one short paragraph - bold criterion first, blank line between rules.** A rule
that needs more than that is describing how to write code, not how to review it. Split it or
move it.

**A checklist entry that lists files to touch is a defect** - a files-to-touch how-to is
procedure, and it moves to the architecture doc or the skill that owns the task.

**A rule tells the diff what it must or must not do - it never describes what the system is.**
A ban says what the system must not be: "never X - do Y instead", or "a diff that adds X ...
is a defect" - the defect spelling binds the reviewer's verdict. A duty says what a change
must also do: "a diff that changes X also does Y, in the same change". A sentence that
describes how the code is ("state is a stack", "X and Y agree") is a statement: the first
violating diff turns it false. Rewrite it as the ban or duty of the diff that could break it,
or move it to the architecture doc.

**A rule is written in plain English - short sentences, common words, no idioms.** A rule that
needs a second read is a defect of the rule.

**A rule spends no spare words.** A clause that restates what the head already binds - a
defect verdict repeating its own duty, a doubled negation, a filler phrase - is cut.

**No sections.** A section header is a bucket asking to be filled: a grouping either decouples
into the flat list or is a separable concern that earns its own routed file.

**Rules are unnumbered and unnicknamed** - numbering invites citation. Anything that needs a
stable reference lives in the checklist's architecture doc.

**A path in a rule resolves against the checklist's own folder.** A path from the repo root
starts with a root directory the folder does not contain (`modules/...`, `daslib/...`) or says
"`(repo root)`"; when both readings exist - a `tests/` beside the checklist and one at the
root - the rule must say which.

**Cite files by name; cite the folder's own architecture doc by section, and never require
that section to be read before the rule can be applied.** No file cites a checklist rule by
number, position, or nickname. A file outside the rule documents - source, commit message,
doc - quotes the rule's words; a rule document routes to the checklist instead, and never
quotes or restates its rule. A criterion whose home is another folder's architecture doc is
restated in place and cited nowhere. A quote states the reason the code is shaped this way and
survives any rewording; a position points at whatever sits there today.

**Name the API a rule is about; never name an example of it.** A rule governing specific
functions or files must name them or it cannot be checked - that name is the criterion. An
illustrative aside has no such excuse: nothing keeps it in sync, and a stale example is worse
than none.

**A ban names its replacement.** "Never X" is half a rule; " - do Y instead" completes it for
a few words.

**One sentence of WHY is allowed where it makes the criterion decidable; anything longer
belongs in the architecture doc.** No history, no PR numbers, no direction of travel; planned
work lives in the folder's follow-up ledger (a checklist that keeps one elsewhere names it in
its opening).

**The placement block, when a checklist carries one, closes the file**: one bold criterion
("Placement - one file, one line: a diff keeps each file inside its line, and a new file adds
its line here, with its tests, in the same change.") over `` `file` `` - role lines, optionally
a bare never-phrase ("Zero network"). A clause with its own defect-verdict or same-change
obligation is a rule, and it lives in the flat list above.

**Adding a rule starts with reading the whole checklist** - duplication, drift, and homeless
placement all start with a rule appended by an author who had not just read the file.

**A rule the diff adds that is longer than every rule already in the file is split, its
exception dissolved, or its extra prose moved to the architecture doc.**
