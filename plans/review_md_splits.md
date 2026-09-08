# Checklist splits owed - a docs-only PR at the end of the startup chain

Four rules the per-PR audits found working but hard to apply: each fuses two checks, or takes
the wrong subject. Splitting is a restructuring, so it rides its own docs-only PR (those merge
gate-free) rather than a code PR. Meaning-preserving; the dragon reads the result.

- `utils/REVIEW.md` - the three CI-row rules take "an arm" as subject while the definition
  above them says one arm may hold both assertions a CI row can run and assertions none can,
  so a mixed arm fits neither rule. Make the assertion set the subject: "the load-bearing
  assertions a CI row can run ship with a row that executes them on every pull request; the
  ones no CI row can run ship with a compile-check row and a recorded run". Then split each
  60-word rule into a trigger sentence and a duty sentence, and drop "one arm may hold both",
  which follows.
- `doc/REVIEW.md` - the fetch/run/type rule's subject and verb sit 45 words apart in two nested
  dash lists. Split into the duty ("A diff that adds anything a reader is told to fetch, run,
  or type to text that reaches a built page states, in the PR body, that each exists and works
  at merge, and where that was checked"), then the scope sentence naming the three page kinds
  and the `.. include::` arm (a diff touching only an included `.md` never surfaces this
  checklist in the folder walk, so that arm needs a routing line in the opening), then the WHY.
- `modules/dasLLVM/REVIEW.md` - the module-cache flag rule fuses two checks (a child asserting
  a compile-time macro line spawns with `-no-module-cache`; a test whose subject is the cache
  pins its own file with `-module-cache <temp>`) behind a five-clause qualifier and a five-line
  mechanism tail. One rule per check; the replay mechanism moves to `ARCHITECTURE.md`. Same
  shape, weaker: the intrinsic emitter reference-cell rule fuses three obligations.
- `doc/source/stdlib/handmade/REVIEW.md` - the type-file format rule (one type-description
  line, one line per member, declaration order) carries a placement duty in a subordinate
  clause (a property's description is its own `function-` file). Two rules; drop "handmade
  file in this folder" from the head, which the folder walk already scopes.
