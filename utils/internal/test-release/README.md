# test-release — the release-audit batch runner

Points at an EXTRACTED release bundle and proves it sane using the bundle's own binary —
no repo files, no build tree. This is the tool tier of the release audit (tools → agents →
people, fail-fix at every layer); agent and human testing start only on a bundle this tool
passes.

## The per-RC process

1. Download the platform bundle from the release tag; extract to a CLEAN directory
   (never over a previous install — a mixed tree tests nothing):
   `tar -xf daslang-bundle-<os>-<arch>.zip -C <dir> --strip-components=1`
2. From the repo root: `bin/daslang utils/internal/test-release/main.das -- --bundle <dir>`
3. Triage the report: `UNEXPECTED` and `TIMEOUT` lines are the audit's yield — each is a
   bundle defect, a missing payload, or a new file needing a tier ruling. `STALE-EXPECTED`
   means the tree improved: drop the entry.
4. Findings that are real defects get fixed in the repo (and usually a gate so the class
   cannot return); findings that are tier classifications get an `expected_compile.txt`
   entry with the reason.

## Phases

- **compile** (implemented) — the floor: every `.das` under the bundle's `examples/`,
  `tutorials/`, and `modules/*/{examples,tutorials}` compiles with
  `<bundle>/bin/daslang -compile-only`, per-file timeout, parallel self-spawned workers
  (`utils/common/parallel_workers`). No execution, so windows/devices/models never engage;
  the timeout covers the one thing that can still spin — compile-time macro work.
- **run** (planned) — console-pure allowlist executed with timeout, rc==0 required.
- **daspkg** (planned) — `daspkg install` for the package-requiring set (network-gated),
  doubling as a live smoke of the shipped daspkg; then their compile tier re-runs.
- **headless** (planned) — dasImgui apps through the harness headless arm.

## expected_compile.txt

`<bundle-relative-path> <reason>`, one per line, `#` comments. Discipline: an entry is a
RULING (why this file legitimately cannot compile from a bare bundle), not a mute button —
`SHIP-DEFECT(...)` reasons mark known defects awaiting a fix and must disappear when the
fix lands; the stale-expected check reds the gate if an entry starts passing, so the list
can only shrink truthfully.
