# Writing and reviewing a skill (repo-only)

Read this before adding a `skills/*.md` file, before moving content between skills, and
before reviewing someone else's skill change. It is repo-only: it is about *this* tree's
`skills/` directory, `install/skills.list`, and the shipping gate.

Shipping mechanics — how `install/skills.list` works, the closure invariant, the ASCII
rule — live in `skills/install_instructions.md`. This file is about the *content*.

## The one question that matters: who reads this?

A skill is read by two different audiences and they are NOT interchangeable:

- **SDK readers** — a human or a Claude session working inside an installed daslang SDK.
  They have `bin/daslang`, `daslib/`, `include/`, `tutorials/`, `dastest/`, `utils/`. They
  do **not** have `src/`, `tests/`, `benchmarks/`, `doc/source/`, `modules/*/src`, or any
  build plumbing.
- **Repo readers** — someone working on daslang itself, who has all of it.

Decide the audience **before** writing, because the failure mode is not a typo — it is a
file that serves neither. `aot_testing.md` spent a release as 400 lines with its
SDK-relevant half (AOT concepts, `Module::aotRequire`, semantic-hash diagnosis) *interleaved*
with repo plumbing (the `test_aot` binary, `tests/aot` registration, `libDaScriptAot` regen,
CI wiring). Splitting it afterwards is a reorganization, not a cut. Writing it as two files
costs nothing up front.

Rules of thumb:

- If most of it is repo plumbing, make it repo-only and leave it off `install/skills.list`.
- If most of it is usable from an SDK, ship it and push the repo bits into a `(repo-only)`
  section or a separate repo-only skill (`writing_tests.md` + `tests_in_repo.md` is the
  worked example).
- **Never** solve an audience mismatch by shipping `src/` or `tests/`. Mark the line
  instead.

## Before you add a new file: is it actually new?

Two skills covering one topic is worse than one long skill, and the collision is easy to
miss because the names differ by a word. Check:

- Does an existing skill already own this topic? Extend it.
- Does a `doc/source/**` page already own it? Then the content should have ONE source —
  see the pattern in `skills/install_instructions.md` (markdown is the source; the `.rst`
  includes it).
- Is your new name a near-duplicate of an existing one? `detect_dupe.md` (agent-facing,
  short) vs `detect_dupe_reference.md` (long-form, every flag) is deliberate and the names
  say which is which. Two files called the same thing in different directories is not.

## What the gate checks — run it before you push

```bash
python3 ci/check_shipped_skills.py <bundle-root> install/skills.list
```

It is wired into `ci/smoke_test_bundle.sh`, so `build.yml`'s `bundle_smoke` job runs it on
every PR. It fails on, in a shipped skill:

| Class | Why |
|---|---|
| `src/`, `doc/source/`, `benchmarks/`, `modules/*/{src,tests,tutorial,harness}` | Not in a bundle, ever |
| `bin/Release/…`, `bin/Debug/…` | Windows repo layout; a bundle has `bin/daslang` |
| `daslang.exe script.das` | `.exe` is Windows-only — invocations must be portable |
| `C:\Users\…`, `/home/<name>/…`, `AppData` | Someone's machine leaked into a doc |
| A relative link whose target is absent from the bundle | Dead link for every SDK reader |
| `skills/<other>.md` where `<other>` is not shipped | Dead link (the closure invariant) |

Escape hatch, when the path is genuinely repo internals worth naming: put `repo-only` on
the line, or in the **heading** of a section that is entirely repo internals. A heading
marker exempts that section only and resets at the next heading. Fenced code blocks are
exempt from the path checks, because they quote tool output that must stay verbatim.

Do not reach for the marker to silence a real problem. If a *shipped* skill needs a
`repo-only` marker on half its lines, the answer is a split, not markers.

## What the gate cannot check — the review checklist

None of the following is mechanical, and each has bitten this tree:

1. **Is the substance right for the audience?** A skill can pass every path check and still
   be 60% useless to its stated reader.
2. **Is it duplicative?** Two documents drifting apart is worse than one imperfect one.
3. **Is the trigger row accurate?** Every shipped skill needs a row in
   `install/CLAUDE.md`, and every repo skill a row in the top-level `CLAUDE.md`, saying
   *when to read it*. A wrong trigger means the skill is never opened, or always opened.
4. **Are the claims verified or assumed?** Syntax and behavior claims must be
   probe-compiled with the current binary, not recalled. `ds2_parser.ypp` is grammar truth.
   A confidently wrong skill is worse than a missing one, because it is trusted.
5. **Does it say what it does, not narrate?** Follow `skills/tutorial_prose.md` for
   general-reader prose and the comment-hygiene rules in `CLAUDE.md`.
6. **Do the commands actually run?** Paste them. `bin/daslang …` from the bundle root is
   the shipped form.
7. **Are the rules general?** A skill states the rule and the mechanism that makes it
   true — never the incident that taught it. No PR numbers, no dates, no machine-local
   paths, no session war stories ("cost 20 minutes on #NNNN"). Incidents age into noise
   and pin the reader to a context they don't have; the mechanism ("fail-fast:false
   matrices stay in_progress around dead jobs") is what transfers. Provenance belongs in
   memory or the PR description, not the skill.

## Registering it

1. File at `skills/<name>.md`.
2. Shipped? Add `<name>.md` to `install/skills.list` **and** a row to the
   `install/CLAUDE.md` skill table.
3. Add a row to the top-level `CLAUDE.md` table (repo readers need to find it either way).
4. Run the gate. If it fires, fix the path — do not reach for the marker first.
