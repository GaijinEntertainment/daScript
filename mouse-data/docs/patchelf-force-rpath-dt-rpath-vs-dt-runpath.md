---
slug: patchelf-force-rpath-dt-rpath-vs-dt-runpath
title: When does patchelf need --force-rpath, and what's the difference between DT_RPATH and DT_RUNPATH on Linux?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**The two tags.** ELF binaries can carry either or both:
- `DT_RPATH` — legacy. Searched **before** `LD_LIBRARY_PATH`. Cannot be overridden at runtime.
- `DT_RUNPATH` — modern. Searched **after** `LD_LIBRARY_PATH`. Affects only the *direct* loader's lookup, not transitive deps.

Most modern toolchains emit `DT_RUNPATH` only. Older toolchains, statically-linked tooling, or hand-built dependencies can still emit `DT_RPATH`. Some binaries carry both, accidentally or otherwise.

**How to inspect.**

```bash
readelf -d <bin> | grep -E '(RPATH|RUNPATH)'
```

Either, both, or neither will appear, with the value in `[brackets]`.

**`patchelf --set-rpath` default behavior.** With no flags, it sets `DT_RUNPATH` (and removes `DT_RPATH` if present). This is what you want >95% of the time.

**When you need `--force-rpath`.** The flag forces `DT_RPATH` instead. Use it when:
- The binary currently has `DT_RPATH` only (no `DT_RUNPATH`) and you want to preserve that semantics — e.g. you need search-before-LD_LIBRARY_PATH, or transitive deps need to inherit the search path (DT_RPATH affects loader lookups for transitive deps; DT_RUNPATH does not).
- A consuming linker / tool can't read RUNPATH and only honors RPATH. (Rare on glibc, can come up with embedded / musl / cross toolchains.)

Otherwise, omit it — RUNPATH is the safer default.

**Reading both via `readelf -d`.** Both tags can coexist on one binary. When mirroring search paths through a relocate/scrub helper, accumulate values from BOTH lines and join with `:`:

```das
struct LinuxRpathTags {
    has_rpath, has_runpath : bool
    value : string  // accumulated via "{prev}:{next}"
}
```

Then choose `--force-rpath` only when `has_rpath && !has_runpath`.

**Practical example (daspkg's scrub_external_rpaths).** Reads RPATH+RUNPATH together, strips any absolute path (anything not starting with `

), re-sets the remainder. The `--force-rpath` decision is the heuristic above. See `utils/daspkg/commands.das`.

**See also.** [[daspkg-release-dep-dylib-symlink-chain]], [[daslang-run-cmd-popen-shell-expansion-dollar-origin]] (`

 escaping when invoking patchelf through `popen`).

## Questions
- When does patchelf need --force-rpath, and what's the difference between DT_RPATH and DT_RUNPATH on Linux?
