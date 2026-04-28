# Debugging AOT Hash Desync (`error[50101]: AOT link failed`)

When `test_aot.exe -use-aot` (or any binary that links pre-built AOT stubs) reports `error[50101]: AOT link failed on <fn>`, it means the runtime computed a different semantic hash for `<fn>` than the AOT generator recorded. The AOT stub map has entries keyed by hash; lookup miss → link fails.

Background on the hash machinery is in [`skills/aot_testing.md`](aot_testing.md). This skill is the playbook for *finding why* the hashes disagree.

## First checks before investigating codegen

1. **Stale `test_aot.exe` vs `libDaScript_runtime.lib`.** If CI is green and only your local build fails, the first suspect is a local mismatch between the executable and the runtime static library. Compare timestamps:
   ```
   stat -c "%y %n" bin/Release/test_aot.exe lib/Release/libDaScript_runtime.lib
   ```
   If the runtime lib is older than `test_aot.exe`, force a static-lib rebuild (touch a runtime source or `cmake --build build --target libDaScript_runtime`) before chasing codegen.

2. **Stale per-test AOT cache after touching cross-module deps.** Editing `daslib` doesn't automatically invalidate per-test `_aot_generated/*.das.cpp`. The cached stub still encodes the old hash and link fails. Wipe and rebuild:
   ```
   rm -rf tests/<area>/_aot_generated/ daslib/_aot_generated/
   cmake --build build --config Release --target test_aot -- /nodeReuse:false
   ```

If neither applies, you have a real desync. Continue.

## Step 1 — Read the diagnostic

The error dumps the runtime's view:

```
error[50101]: AOT link failed on test_double_match C1<S<testing::T>>?
semantic hash is 5fa4c638bec10ddb
// test_double_match C1<S<testing::T>>? hash=0xedef0d103b30be69, @dep_a=0x..., @dep_b=0x..., ...
... full SimNode dump of <fn>'s body ...
```

What each piece means:

- **`semantic hash is X`** — runtime's *combined* hash (own-hash + sorted dep hashes), `getFunctionAotHash` in [`src/simulate/simulate_fn_hash.cpp`](../src/simulate/simulate_fn_hash.cpp).
- **`hash=0xY`** in the comment line — runtime's *own* hash for `<fn>` (`getFunctionHash`).
- **`@dep::name=0xZ`** — runtime's own-hash for each dependency.
- **The SimNode dump that follows** — the runtime function body, with per-node sub-hashes printed via `displayHash=true`.

The AOT-recorded values live in the generated `.cpp`:

```
grep "<fn>.*hash=" tests/<area>/_aot_generated/<file>.das.cpp
```

You'll see the AOT-time `hash=0x...` and matching dep entries. The numeric registration line `{ 0x..., false, (void*)&<fn>_..., ... }` holds the AOT-time *combined* hash.

## Step 2 — Localize the disagreement

Compare runtime vs AOT recorded.

**Case A — own hash matches, combined doesn't.** Some dep's runtime hash differs from AOT. Diff the `@dep::*=0x...` lists in the AOT comment vs the runtime printout. The first dep with a different hex is the one to investigate next (recurse: it now becomes the `<fn>` to debug).

**Case B — own hash itself differs.** The function body hashes differently between AOT-gen and runtime. The body diverges in *some* node. Continue to step 3.

## Step 3 — Side-by-side SimNode dumps

The trick: get the *same* function dumped from both code paths and `diff` them.

Add to the failing `.das` file:
```
options log_nodes
options log_nodes_aot_hash
```

`log_nodes` triggers `printSimFunction` in [`src/ast/ast_simulate.cpp`](../src/ast/ast_simulate.cpp) line ~3814. `log_nodes_aot_hash` adds per-node hash annotations.

Then capture from both directions:

```
# Runtime dump (the failing path):
bin/Release/daslang.exe -dry-run tests/<area>/<failing_test>.das > /tmp/runtime.txt 2>&1

# AOT-generation dump (the recorded path):
bin/Release/daslang.exe utils/aot/main.das -- -aot tests/<area>/<failing_test>.das /tmp/aot.cpp > /tmp/aot.txt 2>&1
```

If the AOT-tool side surfaces only `[I] Aot to ...` without log_nodes output, that path runs `compile_and_simulate` from [`daslib/aot_cpp.das`](../daslib/aot_cpp.das), which discards logs. Use `daslang.exe -dry-run` on the same file as a **second runtime path** and diff the two — both runtime paths going through `simulate()` should produce identical hashes for the same source. If they DON'T, the issue is non-deterministic codegen (your bug) and the diff pinpoints it. If they DO match, the AOT-recorded value differs from both — search for special handling along the AOT-generation pipeline (e.g. `is_in_aot()`, `set_aot()`, `policies.aot_module`).

In the dump, find the failing function:
```
grep -n "^// <fn> <fn>" /tmp/runtime.txt   # the function body block starts here
awk 'NR==<line>,/^$/' /tmp/runtime.txt > /tmp/runtime_fn.txt
# repeat for the other dump
diff /tmp/runtime_fn.txt /tmp/other_fn.txt | head -60
```

The first differing line is the divergence point. Each SimNode prints with hex hashes annotated; the change of hash on a particular node tells you which subtree differs.

## Step 4 — Two common divergence shapes

### Shape 1 — Constant value bytes differ (this PR's bug)

Diff shows a single `ConstValue` brace with **same low half, different high half**:

```
< {1374389535,1074339512, 2265841977,32766}
> {1374389535,1074339512, 3662712544,32759}
```

The first 8 bytes are the actual scalar (here `3.14lf`); the trailing 8 bytes look like pointer addresses (`0x7FFE...`/`0x7FF7...`).

**Diagnosis: a `cast<T>::from` or similar conversion is leaking uninitialized stack/union bytes into the `vec4f` it returns.** Some primitive's `cast` specialization in [`include/daScript/simulate/cast.h`](../include/daScript/simulate/cast.h) is using the union trick:

```cpp
union { vec4f v; T t; } A;   // A.v UNINITIALIZED
A.t = x;                     // writes sizeof(T) bytes
return A.v;                  // returns 16 bytes — high bytes = stack garbage
```

Compare with the proper pattern (e.g. `cast<int64_t>::from`):
```cpp
return v_cast_vec4f(v_ldui_half(&x));   // v_ldui_half = _mm_loadl_epi64 → zeros upper
```

Fix the broken specialization to either zero the union first (`A.v = v_zero(); A.t = x;`) or use the SSE intrinsic. SimSource hashes the full vec4f via `V_ARG(value)` in [`src/simulate/simulate_visit.cpp`](../src/simulate/simulate_visit.cpp) — there's no type info at hash time, so the fix has to be at the construction site, not the visit site.

### Shape 2 — `is_in_aot()` branching

Diff shows entire SimNode subtrees that exist in one path but not the other. The macro expansion produced different code in AOT mode vs runtime mode.

**Diagnosis: something in the macro chain checks `is_in_aot()` (or `set_aot()`'s `g_isInAot` flag) and emits different AST in each mode.** Grep:

```
grep -rln "is_in_aot\|set_aot\|aot_module" daslib/ <required modules>
```

The AOT tool sets `g_isInAot = true` via `set_aot()` in [`daslib/aot_cpp.das:3988`](../daslib/aot_cpp.das). Code that branches on it is fundamentally non-portable across AOT-gen ↔ runtime — fix by removing the branch (produce the same AST in both contexts) or by ensuring the branch only affects details that don't reach the SimNode tree.

## Step 5 — Verify the fix

After fixing:

1. **Wipe the AOT cache** (the fix likely changed bytes the AOT-side had recorded):
   ```
   rm -rf tests/<area>/_aot_generated/ daslib/_aot_generated/test_aot_daslib_modules_*.das.cpp
   ```
2. **Rebuild and re-run**:
   ```
   cmake --build build --config Release --target test_aot -- /nodeReuse:false
   bin/Release/test_aot.exe -use-aot dastest/dastest.das -- --use-aot --test tests/<area>
   ```
3. **Strip the debug options** from the test file before committing.

## C++ debug helpers (last resort)

If `log_nodes` isn't enough, [`src/simulate/simulate_fn_hash.cpp`](../src/simulate/simulate_fn_hash.cpp) has two `#if 1` switches at the top:

```cpp
#if 1
#define debug_hash(...)
#else
#define debug_hash  printf
#endif
```

Flip `1` → `0` to turn on byte-by-byte hash trace (`debug_hash`) or per-function combined-hash trace (`debug_aot_hash`). **Caveat:** these print on every hash call during compilation. Cmake AOT custom-commands re-run daslang.exe per AOT file and capture stdout; if the build invokes daslang for many files, the build log explodes (~30k lines per file). Only enable when you can isolate one file with `daslang.exe -dry-run` outside of cmake.

For one-off byte inspection at SimSource construction, drop a `fprintf(stderr, ...)` into `setConstValue` or wherever you want to verify the bytes the simulator stores. Just remember to revert before rebuilding the static libs (`libDaScript`, `libDaScript_runtime`) — debug printf in a hot path is fine for diagnosis, never check it in.
