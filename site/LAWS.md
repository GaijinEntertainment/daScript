# LAWS.md - Boris's rulings

Append-only intent provenance for rule-document edits in this folder's documents (the
mechanism: CLAUDE.md sec. "Boris's rulings get a `LAWS.md` sidecar"). Never groomed,
compacted, or cited as rules.

| Date | Document | The ask |
|---|---|---|
| 2026-08-22 | REVIEW.md (performance_bench.json record rule) | "we need to reprofile linq benchmarks and hook to webpage. [...] add a performance page under daslang.io / performance - separate page, similar how dasllama page is added - only this one will contain the two tables, switch between interpreter and jit" |
| 2026-08-22 | REVIEW.md (performance_engines.json record rule) | on what was missing from the page's tables: "missing - duckdb and postgresql. [...] there should be implementations already" - the four-engine suite (examples/benchmarks/sql) joins the page as its own board and record |
| 2026-08-24 | REVIEW.md (code-sample rule) | "if code example appears anywhere on the website, its linked to 'try it on playground' - and its verified to compile and run. here is an example - daslang.io/dasllama.html - code on that card"; on partial snippets: "nop. full examples" |
| 2026-08-24 | REVIEW.md (dasprofile_bench.json record rule) | the cross-language benchmarks page: "i'd like a separate page though, under /performance menu - the very first thing on that submenu"; shape rulings: "bars should be default", heatmap removed, benchmark names link to playground samples ("we refresh samples"), no native-loop sample ("all it does is measures interop"); ship: "lets push the cards and new web page" |
| 2026-08-29 | REVIEW.md (the sample and playground-run rules) | Port-convergence Q&A: approved moving the sample-source rules out of the blanket import ("yes"), batched into the convergence PR ("we ammend this one. its a bit more prose, not worth separate one"). The embedded-sample rules scope to pages, the Playwright trigger becomes what-the-runtime-loads with a coverage clause, and the artifact clause gets a diff-visible trigger |

- **2026-08-30** (`REVIEW.md`): on the receipt-rule contradiction over ASR reference rows
  (one run's cmd covering several per-clip rows), Boris ruled the rendered cmd identifies
  the RUN, not the row - "2. agree" to amending the first rule rather than storing per-clip
  argvs.

- **2026-08-31** (`REVIEW.md`): review-round flashlight items 2 and 4. Boris ruled ("yes")
  the stated-suite-run rule is tip-pinned per PR with a restatement duty on later edits;
  and ("yes") `site/README.md` is blessed as `site/`'s architecture doc (the skill takes a
  carve-out rather than a doc split). The artifact-list and "page"-definition rewrites rode
  along as auditor-identified defects.

- **2026-09-01** (`REVIEW.md`): after a cookie audit of daslang.io and dasllama.io found
  both sites cookie-free but nothing gating a re-introduction, Boris ruled "lets just land
  review gate" - the checklist keeps only the weakening ban and the allowed-host disclosure
  duty; the check itself lives in `REVIEW.das`.

- **2026-09-01** (`REVIEW.md`): dragon round on the two gate rules. The weakening rule was
  re-plained ("any resource a page loads from a host its allowed list does not name") and
  the disclosure rule now names `ALLOWED_HOSTS` rather than pointing at its neighbour -
  rules are unordered, so an adjacency referent points at nothing.

- **2026-09-01** (`REVIEW.md`): flashlight round after PR 3920, dragon findings on this
  checklist. Boris ruled "yes" to defining "editor content" in place (sample bundles are
  not pages - `web/examples/ui/REVIEW.md` binds them), to dropping "under this folder" from
  the two embedded-sample rules and the "link every embedded sample" tail, to "playground
  sample" in the stated-run rule, and to rewrapping the one over-wide line. The run-not-row
  tail on the first rule stays - the 2026-08-30 ruling placed it there.

- **2026-09-01** (`REVIEW.md`): on the bench receipts carrying his local paths - "we really
  don't want to show my local paths. its annoying" - Boris ruled option B: the home
  directory is spelled `~` in every recorded line, the recorder does it, a gate enforces it,
  and the 386 existing lines are migrated once rather than re-minted. The exact-argv rule
  gained that one clause.

- **2026-09-01** (`REVIEW.md`): flashlight round, lint candidate (a) ruled "yes" - the
  tests-entry receipt rule is now `REVIEW.das`'s check and the checklist keeps the
  weakening residue; the row rule keeps its renderer half.

- **2026-09-01** (`REVIEW.md`): flashlight round, lint candidate (b) ruled "yes" - the two
  cell-parity rules are now one `REVIEW.das` check (JSON record against the INTERP and JIT
  tables, both pairs) and the checklist keeps one weakening residue; the two same-change
  duties above them stay.

- **2026-09-01** (`REVIEW.md`, round 2): the dragon caught the `~` clause on the exact-argv
  rule duplicating the new home-path gate (the other three gates got residues, this one
  kept a duty) - the clause went and the residue joined the others; the opening's last
  "it" now says "that editor"; the row rule lost its third-copy tail (the 2026-08-30 tail on
  the first rule stays); "the web build" names `web/` (repo root); "today" became "before
  the change".

- **2026-09-01** (`REVIEW.md`, round 2 rulings, "all as suggested"): the dl-* definition
  duty became `REVIEW.das`'s census over dasllama.html and its script against the inline
  style and the shared sheet, mirroring the dasllama.io gate; the checklist keeps the
  weakening residue.

- **2026-09-01** (`REVIEW.md`, round 2 rulings, "all as suggested"): the three embedded-sample
  rules - full program, playground link, gen2 - became one `REVIEW.das` check that compiles
  each block with the running binary; the checklist keeps the weakening residue.

- **2026-09-01** (`REVIEW.md`, the embedded-sample residue): the gate's first run found the
  four blog fences and the dasllama.html hero sample failing the sample rules. Boris ruled:
  "lets skip blog, for sure. its blog after all. i can post whatever i feel like there" -
  blog posts are out of the sample check; and "lets opt out dasllama hero sample. one day
  we'll get dasLLAMA to work on playground, maybe. until then out" - the block carries
  `data-playground="none"`, is still compiled, and owes no link.

- **2026-09-01** (`REVIEW.md`, round 3): the review-md auditor showed the round-2 cut of the
  `~` clause left the exact-argv rule contradicting the home-path residue - the clause was a
  boundary, not a duplicate - so it returned, with "outside the quotes" after the dasLLAMA
  auditor found 116 quoted tildes no shell expands; the two record-to-table duties went to
  the parity gate that enforces them; the sample residue split into the residue and a ledger
  duty; the dl-* residue lost its cross-site sentence and names the id subtraction.
