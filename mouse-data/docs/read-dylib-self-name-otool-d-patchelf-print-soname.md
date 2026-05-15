---
slug: read-dylib-self-name-otool-d-patchelf-print-soname
title: How do I read a dylib's install_name / SONAME portably from daslang (macOS otool -D vs Linux patchelf --print-soname)?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Goal.** Read the "self-identity" a dylib advertises (the name everything else uses to dlopen / dyld-resolve it):
- macOS: `LC_ID_DYLIB` — typically `@rpath/libfoo.N.dylib`.
- Linux: `DT_SONAME` — typically `libfoo.so.N`.

This is the install_name / SONAME, distinct from the on-disk filename. dyld and ld.so look up dependencies by this token in their respective rpath search dirs.

**macOS (`otool -D`).**

```bash
otool -D libglfw.3.4.dylib
```

Output:
```
libglfw.3.4.dylib:
@rpath/libglfw.3.dylib
```

Read the last non-empty line. If output is just the path with a `:` and no second line, the dylib has no install_name (rare; treat as basename).

**Linux (`patchelf --print-soname`).** Preferred when patchelf is installed:

```bash
patchelf --print-soname libglfw.so.3.4
```

prints `libglfw.so.3` or empty if no SONAME.

**Linux fallback (`readelf -d`).** When patchelf isn't available:

```bash
readelf -d libglfw.so.3.4 | awk '/SONAME/ { print }'
```

The line looks like:
```
 0x000000000000000e (SONAME)             Library soname: [libglfw.so.3]
```

Extract between `[` and `]`. Empty brackets = no SONAME.

**Why this matters.** If you ship a dylib whose on-disk filename differs from its install_name / SONAME — common when versioned releases ship `libfoo.N.M.dylib` with install_name `@rpath/libfoo.N.dylib` — your recipient's dyld/ld.so will look for the install_name and miss. You need a symlink alias to bridge.

This is exactly the diagnostic that `utils/daspkg/commands.das:read_dylib_self_name` performs before `ensure_dylib_alias` materializes the symlink. PR #2656 added both.

**See also.** [[daspkg-release-dep-dylib-symlink-chain]] (the bigger picture).

## Questions
- How do I read a dylib's install_name / SONAME portably from daslang (macOS otool -D vs Linux patchelf --print-soname)?
