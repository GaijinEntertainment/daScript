# AST Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening `REVIEW.das` (beside this file) is a defect:** dropping its scan of the prints in
  `trySerializeProgramModule` (`ast_parse.cpp`, `ARCHITECTURE.md` sec.1), or a finding text that
  no longer names what failed. What the gate enforces is read from the gate itself.

- **A diff that moves a module-cache read diagnostic out of `trySerializeProgramModule` - into
  a helper it calls, or another function - extends `REVIEW.das`'s scan to the new home in the
  same change.** The gate scans that one function's body, so a print moved out of it is a print
  the gate no longer checks, and an ungated line there is output every user of the default cache
  sees.

- **A diff that changes what a manifest row means - a field added, removed, reordered or
  re-typed in `read_manifest` or `write_manifest`, or a line added to the manifest's key - its
  `stamp`, `dll`, `root`, `dasroot`, `target` and `dep` lines - in either (`dyn_modules.cpp`) -
  bumps the version in `MANIFEST_HEADER` in the same change.** A reader
  accepts a manifest whose first line equals `MANIFEST_HEADER`, so without the bump an older
  manifest decodes the changed bytes as a wrong registration with no diagnostic.

- **A diff that gives `read_manifest` a new kind of row - one it pushes into `rows` - gives the
  replay loop in `init_dyn_modules` (`dyn_modules.cpp`) a branch for it in the same change.** The loop
  dispatches on one flag with `replay_native_path` as the other arm, so a kind it does not know
  replays as a native path.
