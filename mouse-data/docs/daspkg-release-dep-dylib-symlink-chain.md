---
slug: daspkg-release-dep-dylib-symlink-chain
title: Why does a daspkg release bundle run fine on the build machine but fail on a separate laptop with "Failed to find @glfw::glfwWindowHint" / missing native dylib?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Status (2026-05-14).** Fixed in PR #2656 (merged). `ship_native_dlls` now preserves the symlink chain on copy AND post-copy reads the destination's install_name / SONAME and creates a matching alias when the basename differs. Same path also scrubs absolute LC_RPATH / DT_RPATH+DT_RUNPATH leaks. The diagnosis below is preserved as the WHY — and as the recipe for hand-checking a suspect bundle.

**Symptom.** `daspkg release` produces a bundle; the standalone exe runs cleanly on the build machine, but on a recipient machine the program logs e.g. `Failed to find @glfw::glfwWindowHint Ci Ci in module glfw.` and aborts. dyld did not error visibly; the shared_module loaded but its dylib transitive dep didn't.

**Root cause.** macOS versioned dylibs use a symlink chain:

```
libglfw.dylib -> libglfw.3.dylib -> libglfw.3.4.dylib   (only this is a real file)
```

The real file's install_name is `@rpath/libglfw.3.dylib` (the alias, not the filename). Anything linking against it asks dyld for `@rpath/libglfw.3.dylib`. dyld's lookup is by **filename in the rpath dir**, not install_name. `daspkg release`'s `ship_native_dlls` (utils/daspkg/commands.das:1442) copies only the literal name from `release_include_dll(...)` — typically the resolved file `libglfw.3.4.dylib` — and drops the symlinks. Result:

- Bundle dir contains `libglfw.3.4.dylib` only.
- Module asks for `libglfw.3.dylib` → dyld misses in `@loader_path`.
- On the **build machine**, the shared_module also carries a leftover LC_RPATH `/Users/<you>/Work/daScript/lib` and `.../glfw_dyn/Release/lib` (from build-time linking). dyld silently falls back through that path and finds `libglfw.3.4.dylib` there — which happens to match by install_name within that dir, so the program runs. **The build-machine "success" is the leak masking the bug.**
- On a recipient laptop none of those build-time paths exist, dyld misses, the symbol lookup fails at runtime, and you see the "Failed to find @glfw::..." line.

**How to confirm before shipping.** Don't trust running the bundle in place. Either rename/hide the build tree (`mv ~/Work/daScript/modules/dasGlfw/glfw_dyn ~/Work/daScript/modules/dasGlfw/glfw_dyn.HIDDEN; mv ~/Work/daScript/lib ~/Work/daScript/lib.HIDDEN`) and rerun, or do `otool -L` on every shared_module and check that each `@rpath/<lib>.dylib` it requests is reachable from the bundle's `@loader_path` (which today resolves to `<bundle>/Contents/MacOS/modules/<DepName>/`). `DYLD_PRINT_LIBRARIES=1` is the smoking gun — if you see dylibs being loaded from `/Users/...` paths, the bundle is not portable.

**Workaround on an already-released bundle.** Drop a symlink alongside the file matching the install_name:

```bash
cd <bundle>.app/Contents/MacOS/modules/dasGlfw
INSTALL_NAME=$(otool -D libglfw.3.4.dylib | tail -1)   # @rpath/libglfw.3.dylib
ln -s libglfw.3.4.dylib "$(basename "$INSTALL_NAME")"
```

**Proper fix in daspkg.** Two options for `ship_native_dlls`:

1. Preserve symlink chains: when the source `found_src` has sibling symlinks in the same dir whose `readlink` resolves to it (directly or transitively), copy those as symlinks too.
2. Post-copy: read the destination's install_name (`otool -D` on macOS / equivalent on Linux), and if `basename(install_name) != basename(dst)`, create the matching symlink.

Option 1 is more conservative (just reproduces the source layout). The Linux equivalent of the bug exists too — `.so.N`/`.so.N.M` chains have the same property.

**Related.** Built dylibs also leak the absolute build-tree LC_RPATH (`/Users/<builder>/Work/daScript/lib` and per-dep `.../glfw_dyn/Release/lib`). Harmless on a clean recipient — dyld skips paths it can't find — but it leaks the builder's home dir into shipped binaries and is what makes the bug invisible on the build machine. Worth scrubbing via `install_name_tool -delete_rpath` during release, but separable from the missing-symlink fix.

**See also:** [[exe_relative_pr_2579_merged]], [[relocated_bundle_pr_2588]] (the existing pieces that make `@executable_path` resolution work — this bug is the missing third leg).

## Questions
- Why does a daspkg release bundle run fine on the build machine but fail on a separate laptop with "Failed to find @glfw::glfwWindowHint" / missing native dylib?
