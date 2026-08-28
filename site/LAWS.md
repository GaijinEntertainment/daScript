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
