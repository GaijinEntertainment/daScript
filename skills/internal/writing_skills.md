# Writing and reviewing a skill (repo-only)

Read this before adding a `skills/*.md` file, before moving content between skills, and
before reviewing someone else's skill change.

**Where a skill lives — and whether it ships — is `skills/internal/skill_taxonomy.md`'s
law**: the folder is the shipping decision, one fact has one home, and the move rules.
Decide the folder before writing. This file is about the *content*.

Audience facts the content must respect: SDK readers have `bin/daslang`, `daslib/`,
`include/`, `tutorials/`, `dastest/`, `utils/` — and do **not** have `src/`, `tests/`,
`benchmarks/`, `doc/source/`, `modules/*/src`, or build plumbing. Never solve an
audience mismatch by shipping those paths — the split-vs-marker threshold is the
taxonomy's rule 3; the marker mechanics are below.

## Before you add a new file: is it actually new?

Two skills covering one topic is worse than one long skill, and the collision is easy to
miss because the names differ by a word. Check:

- Does an existing skill already own this topic? Extend it.
- Does a `doc/source/**` page already own it? Then the content should have ONE source —
  see the pattern in `skills/design_philosophy.md` (markdown is the source; the `.rst`
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
| `src/`, `tests/`, `tests-cpp/`, `doc/source/`, `benchmarks/`, `modules/*/{src,tests,tutorial,harness}` | Not in a bundle, ever |
| `bin/Release/…`, `bin/Debug/…` | Windows repo layout; a bundle has `bin/daslang` |
| `daslang.exe script.das` | `.exe` is Windows-only — invocations must be portable |
| `C:\Users\…`, `/home/<name>/…`, `…\AppData\…` (path form; the bare word is the env-var name) | Someone's machine leaked into a doc |
| A relative link whose target is absent from the bundle | Dead link for every SDK reader |
| `skills/<other>.md` where `<other>` is not shipped | Dead link (the closure invariant) |

Escape hatch, when the path is genuinely repo internals worth naming: put `repo-only` on
the line, or in the **heading** of a section that is entirely repo internals. A heading
marker exempts that section only and resets at the next heading. Fenced code blocks are
exempt from the repo-path and link checks; the command checks (`bin/Release/…`, `.exe`
invocations, machine-local paths) run inside fences too, because fences are exactly
where command lines live.

Do not reach for the marker to silence a real problem — the split-vs-marker threshold
is the taxonomy's rule 3.

## What the gate cannot check — the review checklist

None of the following is mechanical, and each has bitten this tree:

1. **Is the substance right for the audience?** A skill can pass every path check and still
   be 60% useless to its stated reader.
2. **Is it duplicative?** Two documents drifting apart is worse than one imperfect one.
3. **Is the trigger row accurate?** Every skill gets a row in the top-level `CLAUDE.md`;
   a shipped skill additionally gets one in `install/CLAUDE.md` — each saying *when to
   read it*. A wrong trigger means the skill is never opened, or always opened.
4. **Are the claims verified or assumed?** Syntax and behavior claims must be
   probe-compiled with the current binary, not recalled. `ds2_parser.ypp` is grammar truth.
   A confidently wrong skill is worse than a missing one, because it is trusted.
5. **Does it say what it does, not narrate?** Follow `skills/internal/tutorial_prose.md` for
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

1. File it in the folder `skills/internal/skill_taxonomy.md` assigns.
2. Shipped? Add a row to the `install/CLAUDE.md` skill table.
3. Add a row to the top-level `CLAUDE.md` table (repo readers need to find it either way).
4. Run the gate. If it fires, fix the path — do not reach for the marker first.
