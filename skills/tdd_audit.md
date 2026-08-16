# TDD audit — every branch has a test that fails without it

Read this before auditing any diff for test coverage, before claiming a change is
adequately tested, and before writing tests for code that already exists. The procedure is
language-agnostic; test-writing mechanics for daslang live in `skills/writing_tests.md`.

## The rule

**New functionality ships with tests — same PR, no follow-up promises.** A new or changed
reachable branch ships a test that fails without it; a diff that adds a branch no test
distinguishes is a defect.

"Distinguishes" is the load-bearing word. A test that *executes* a branch but would still
pass with the branch broken proves nothing — coverage tools measure execution, not
distinction. The question is never "is this line covered?" but "which test FAILS if this
line is wrong?"

## What counts as a branch

Enumerate from the diff, not from the whole file:

- every new `if` / `elif` / `else` arm, `match` arm, loop guard, early return
- every new error path: a panic, a returned error, a fail-closed rejection
- every changed condition — the old and new predicates differ on some input, and THAT
  input is the test
- a new function (its call is the arm; a function nothing distinguishes is untested
  whatever runs through it)
- a new default: a parameter default, a config fallback, an empty-input path

Reachable means a public entry point can drive it. An unreachable branch is a different
finding (dead code), not a missing test.

## The audit procedure

For each branch, in order of preference:

1. **Name the distinguishing test** — a test in the diff, or an existing test, that fails
   when this branch is broken. Name it specifically: file and case, not "the suite".
2. **When reading can't settle it, run the negative control**: break the branch (invert
   the condition, comment the body, return the wrong arm), run the one candidate test,
   watch it FAIL, restore the code exactly, and verify the tree is back to its
   pre-mutation state. A test that stays green under the mutation distinguishes nothing —
   the branch is untested no matter what coverage says.
3. **Nothing fails under mutation → the branch is untested.** The fix is a test, written
   in the same change — not a follow-up promise.

The negative control is cheap — one mutation, one targeted test run — and it is the only
evidence that settles the question. "The test obviously covers it" is a prediction; the
control is a measurement.

## The cheat check — audit the diff's own test edits

A diff can pass the branch audit and still cheat by adjusting the instrument. Enumerate
every test the diff touches in one of these shapes:

- an expected value changed
- an assertion weakened — `==` to a range, exact to tolerance, a tolerance widened, an
  assert removed
- a test or case deleted
- a skip, exclude, or platform gate added to a test that ran before
- a test rewritten wholesale in the same change that rewrites the code it tests

For each, run the **reverse control**: the OLD test against the NEW code.

- Old test FAILS → the diff changed behavior AND re-tuned the test to match. Legitimate
  only as a conscious flip — the change states why the new behavior is the right one. No
  stated reason means the test now pins whatever the code happens to do, including the
  bug: verdict RETUNED.
- Old test passes and the edit only adds to or rewords the instrument (new cases, tighter
  asserts, refactors that keep every assertion's strength) — justified; move on.
- Old test passes but the edit REDUCES the instrument — a weakened assertion, a deleted
  case, an added skip — then the reduction hid nothing, but it is coverage loss all the
  same, and nothing in the diff required it. It needs a stated reason too: verdict
  WEAKENED.

## Rules for the tests themselves

- **A new test is shown to fail first.** Run it against the pre-change code (stash the
  change, or check out the base) and watch it fail for the stated reason. A test born
  green may be testing nothing.
- **An expectation change is a conscious flip.** When a diff edits an expected value, the
  change states why the new value is the right one. Weakening an expectation until the
  implementation passes is a defect — the test now pins the bug. The cheat check above is
  how an auditor settles it.
- **Pins assert probed behavior.** A pin test (one that freezes current behavior) records
  what the code DOES: probe first, then write the assert from the probe's output. A pin
  written from what the author hopes is documentation, not a test.
- **Never mutate a test to change a verdict.** The mutation goes in the code under test;
  the test is the instrument.
- **A test is named for the invariant it pins, never the ticket.** `test_issue_NNNN`
  says nothing without leaving the code; `test_stackwalk_tail_split` states what must
  hold. Same for fixture files and test-case labels.

## Reporting

Per branch: `BRANCH` (file:line, one-line description) and `VERDICT` —

- `DISTINGUISHED by <test>` — named from reading, with the one-line reasoning;
- `CONTROLLED by <test>` — negative control run: the mutation, the test, the observed
  failure;
- `UNPROVEN` — the settling run is too heavy to run here; give the exact command that
  would settle it;
- `UNTESTED` — the mutations tried and the tests that stayed green.

Per test edit the cheat check caught: `TEST EDIT` (file:case, what changed) and `VERDICT` —
`JUSTIFIED` (the stated reason, or the reverse control passed and the edit did not reduce
the instrument),
`RETUNED` (the old test fails against the new code and the change states no reason — the
test was re-tuned to pass), or `WEAKENED` (the old test still passes but the edit reduced
the instrument with no stated reason).

A claim of absence — "no caller", "no test" — carries the search command and its result in
the report; an unshown sweep is not evidence.

Then the summary lines: `N branches: X distinguished, Y controlled, Z untested, W unproven`
and `M test edits: J justified, K retuned, L weakened`. An all-green audit that names its
branches and tests is a real result; "tests pass" is not an audit.

## Where this runs in the daslang repo (repo-only)

- The rule is constitutional: every `REVIEW.md` carries it verbatim in its opening
  (template: `skills/review_md.md`).
- The per-PR audit is the `tdd-auditor` agent (`.claude/agents/tdd-auditor.md`), launched
  in `skills/make_pr.md` step 0a beside the REVIEW.md auditors — one instance for the
  whole diff, so folders with no checklist (`daslib/`, `src/`, `utils/`) are covered too.
- Repo test placement (AOT registration, the `tests/.das_test` gate) is
  `skills/tests_in_repo.md`.
