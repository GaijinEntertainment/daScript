# Review triage — verdict, disposition, presentation (repo-only)

Read this before triaging any review comment on a PR — Copilot, another bot, or a human
reviewer. It owns the judgment half of the review loop: what verdict a comment gets, when
an accepted fix lands, and what the user is asked. The loop mechanics — watching, replying,
resolving, re-requesting — stay in `skills/internal/babysit.md`.

## The verdict — three classes, tested in order

Every comment gets exactly one verdict. Apply the tests in order; the first that passes
decides.

**1. Defect** — all three tests pass:

- *Reachable*: a shipped caller and an input or state its contract allows lead to the
  flagged behavior. An unguarded state no caller can produce fails — cite which of the
  call-site inventory, an upstream guard, or the type makes it unproducible.
- *Real scale*: the triggering threshold fits real hardware, real data, and the process's
  lifetime. "The integer is finite" fails this test.
- *Named consequence*: a concrete consequence — the wrong result, crash, corruption,
  security exposure, or user-visible failure — exists, and the triager names it in the
  verdict; a claim that chases down to none rejects. "It would be more robust" names none.

A reasoned reachability chain is enough where reproduction is impractical; a rare race or
an attacker-controlled input passes when the chain is concrete, and "could theoretically
happen" is not a chain.

**2. Falsehood** — text in the repo — a code comment, doc, diagnostic, or message — states
something the code contradicts. The test is contradiction, not quality: text that is vague,
incomplete, or unpolished is not a falsehood.

**3. Reject** — everything else. Name the class in the reply:

- *mistaken premise* — the comment's claim about what the code does is contradicted by the
  code; the reply carries the contradicting evidence
- *style preference* — wording, symmetry, "could be clearer"; lint owns style
- *dead-path defect* — the flagged path already terminates the process or the compile
- *unreachable guard* — the guarded state has no producer
- *hardening* — a tightening of behavior with no failing input at real hardware, real
  data, real lifetime
- *cosmetics* — determinism or hygiene with no behavioral difference
- *repo-divergent* — a "better way" this codebase deliberately does differently
- *out of scope* — a change carrying no defect claim: a coverage ask, a scope expansion,
  a question. Answer it in the reply; ledger it when worth keeping

The classes are common names, not the criterion: when none fits a claim-carrying comment,
the reply names the Defect test the claim fails.

Fix cost is not an input to any test: the verdict on a comment must come out the same
whether its fix costs one line or one week. A verdict that changed when the fix turned out
to be cheap was not a verdict.

## Disposition — a defect is always fixed; the open question is when

- **Now**: the defect is in code this PR's hunks changed — a merely-touched file does not
  count — AND the fix is bounded: no new design, no new arc, focused gates cover it. Fix
  it in this round without asking.
- **Ledgered**: off the PR's surface, or the fix is its own arc. Record it — the PR
  body's `### Not done` section (`skills/internal/make_pr.md`, step 6), an issue, or memory — and
  reply with the entry. The entry travels in the verdict report for the user to confirm
  or reshape; a pending confirmation blocks neither thread resolution nor round completion.
- **In doubt, the user decides** — doubt about now-vs-ledgered, about whether the defect
  test really passes, or about fix direction. Ask, with a recommendation.

A falsehood is fixed in the round; when in doubt whether the text is actually false, or
whether its fix belongs in this PR, ask.

A reject's reply names its class plus one evidence sentence. Rejects never cause a push.

An accepted defect whose fix direction is not forced gets a probe before the fix — the
obvious fix and the right fix can differ. After any accepted fix, sweep the affected file
for the same defect pattern before pushing; one sweep saves one round.

## Presenting to the user — report decisions, ask questions

Comments the rules decide arrive as a report, not a question: a per-comment verdict list —
class, one-line evidence, and for defects the disposition.

Ask exactly where a rule says ask, and ask the decision, not permission:

- Never "should I fix this bug?" — a defect is being fixed; the open question is when.
- A now-vs-ledgered question carries the magnitude: what breaks, for whom, and what the
  bounded fix would touch — "fix now (three lines in the changed file) or ledger (touches
  the serializer, its own arc)?"
- A doubt question carries the evidence both ways and a recommendation.

## Round outcomes

- **All-reject round**: no push — proceed toward merge. This is a normal, healthy
  outcome, not a failure to engage.
- **Prose-only round** — every comment is a wording suggestion and none is a falsehood:
  same as all-reject.
- **In any round that already pushes for a substantive fix**, a directly related wording
  cleanup may ride that push — applying it does not change its Reject verdict. Prose never
  causes a push by itself.
- A round is complete when every comment carries a verdict and every now-fix is pushed;
  thread reply-and-resolve closure is `skills/internal/babysit.md`'s Section 0 invariant.
