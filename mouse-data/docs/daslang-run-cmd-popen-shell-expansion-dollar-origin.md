---
slug: daslang-run-cmd-popen-shell-expansion-dollar-origin
title: Why does my patchelf --set-rpath "$ORIGIN/foo" silently set an empty RUNPATH when called from daslang's run_cmd?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Symptom.** Code like
```das
let joined = "$ORIGIN/runtime:$ORIGIN/modules"
run_cmd("patchelf --set-rpath \"{joined}\" \"{path}\"", op_out)
```
returns 0 and `readelf -d path | grep RUNPATH` shows an empty `RUNPATH: []`. The path was wiped, not set. Linker errors at recipient runtime.

**Root cause.** `run_cmd` (and any daslang facility built on `popen`) invokes the command through `/bin/sh -c`. Inside a double-quoted shell argument, `$ORIGIN` is interpreted as **shell variable expansion**, not as a literal token to be passed through to patchelf. `$ORIGIN` is unset in `/bin/sh`'s environment (it's an ELF runtime concept, not a shell variable), so `"$ORIGIN/runtime:$ORIGIN/modules"` expands to `"/runtime:/modules"` — and once patchelf interprets that, the values get collapsed/discarded and you get a blank RUNPATH.

Same trap applies to any `$NAME` literal you want to land on disk via `run_cmd` or `popen`: `$LIB`, `$PLATFORM`, future ELF tokens, or any value passed by a user.

**Fix.** Escape the `

 before interpolating into the shell command string:

```das
let joined_shell = replace(joined, "$", "\\$")
let force_flag = (tags.has_rpath && !tags.has_runpath) ? "--force-rpath " : ""
run_cmd("patchelf {force_flag}--set-rpath \"{joined_shell}\" \"{path}\"", op_out)
```

`\

 inside a double-quoted shell argument passes a literal `

 to the command. `replace(joined, "$", "\\$")` produces `\$ORIGIN/...` in the daslang string, which becomes `\

 in the shell argument, which is the literal `

 patchelf wants.

**How to confirm.** After `run_cmd` returns 0, verify via `readelf -d <bin> | grep -E '(RPATH|RUNPATH)'`. An empty bracket pair (`[]`) is the smoking gun — patchelf "succeeded" but received an empty value.

**Macro pattern.** macOS `install_name_tool -add_rpath '@executable_path/foo'` does NOT trip this — `@` is not a shell metacharacter. The trap is specifically for `

-prefixed ELF tokens going through `popen`.

**See also.** [[daspkg-release-dep-dylib-symlink-chain]] (the larger PR-#2656 fix this gotcha bit during).

## Questions
- Why does my patchelf --set-rpath "$ORIGIN/foo" silently set an empty RUNPATH when called from daslang's run_cmd?
