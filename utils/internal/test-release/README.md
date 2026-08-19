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
   (all phases; `--phase compile|utils` for one; `--network` adds the daspkg rows, which
   install into the bundle's example dirs — a scratch extract, never the one you keep).
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
- **utils** (implemented, `utils_phase.das`) — every shipped tool exercised in its
  capacity, from the bundle alone: the shipped suites (`utils/{common,lint,dascov,find-dupe,
  jobque-timeline}`) through the shipped runner, then `bin/dastest.exe` on one of them
  (the exe form has to compile a test at all); `lint.exe` and `das-fmt.exe --verify` over
  the shipped `daslib` (the tree is at zero and formatter-clean, and stays so); the analysis
  tools on their shipped fixtures (`detect-dupe` on `utils/detect-dupe/fixture`, `dascov`
  on its test script, `benchctl` reset/query, `aot` emitting `hello_world`); the LSP
  subtools on the MCP fixtures; `daslang-live` on `examples/daslive/test_api`. Every row
  is exit code plus substrings only a working run prints — `0 issue(s), 0 error(s)`,
  `N tests, N passed, 0 failed`, `Verified!` — never rc alone. With `--network`, `daspkg`
  installs the packages the gated examples need (sequence → das-cards, the daspkg tutorial
  projects, telegram, the local C/C++ build example), proves the unlock by compiling or
  running the example, and `daspkg cleanup` returns each dir to shipped state. Rows are the
  verified command lines from the RC1 utils audit; `--only <substr>` runs a subset.
- **run** (planned) — console-pure allowlist executed with timeout, rc==0 required.
- **headless** (planned) — dasImgui apps through the harness headless arm.

## expected_compile.txt / expected_utils.txt

`<bundle-relative-path-or-row-name> <reason>`, one per line, `#` comments. Discipline: an entry is a
RULING (why this file legitimately cannot compile from a bare bundle), not a mute button —
`SHIP-DEFECT(...)` reasons mark known defects awaiting a fix and must disappear when the
fix lands; the stale-expected check reds the gate if an entry starts passing, so the list
can only shrink truthfully.
