# dasLLAMA Architecture - the board at a release boundary

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor.

### Board rows land at a release boundary {#board-rows-release-boundary}

The published board is a release's. Each das row of `performance/records/<box>.json` carries the
`dasllama_version` of the exe that minted it, and only a release's exe ships: a row minted between
releases would carry the current release's version while timing kernels no shipped binary has, so
the site's per-model display would show a speed no user can get. The release re-mint
(`performance/gen_bench_records.das` over every board box, `BRINGUP.md`) writes the das rows.
Between releases, a kernel change's measured wall-clock times live in a dated `PERF_LEDGER.md`
entry, and that entry is the regression baseline until the re-mint writes the row.
