# Memory Leak Detection

`bin/daslang` ships several leak-detection mechanisms. Read this skill first to pick the right tool, then jump to the relevant section.

## Quick decision tree

| Symptom | Tool |
|---|---|
| Script leaks a `new Foo()` / `array<T>` / table — want per-allocation call stack | **`--das-profiler-leaks`** (#1) |
| Long-running run keeps growing — want per-context heap dump at exit | **`-track-allocations -heap-report`** (#2) |
| Exit prints `GC COMPILE LEAK` / `GC APP LEAK` (gc_node leaks) | **`DAS_GC_BREAK_ON_ID` env var** (#3) |
| Suspect a specific smart_ptr id (Context, Program, FileAccess) — want a debug-break on every addRef/delRef | **`--track-smart-ptr <hexId>`** (#4) |
| Script uses channels/jobs/lockboxes — `DumpJobQueLeaks` printed survivors | **`--track-job-status`** (`skills/jobque_debugging.md`) |
| Long-running dasHV server suspected to leak handles | **`HandleRegistry` auto-dump** (#6) |

**CLI dash convention:** `-track-allocations` and `-heap-report` use **one** leading dash; `--track-smart-ptr`, `--track-job-status`, and `--das-profiler-leaks` use **two**. Preserve the dashes exactly as shown.

If multiple leak reports fire at exit: fix gc_node (#3) first, then job-que primitives (#5), then heap (#1/#2), then smart_ptr / handles (#4/#6).

---

## #1 — `--das-profiler-leaks` (heap with captured call stacks)

**Scope:** every live allocation on a daslang `Context` heap, grouped per context, with the full daslang call stack that produced it.

```bash
bin/daslang --das-profiler --das-profiler-leaks path/to/script.das
```

Optional `--das-profiler-log-file <path>` writes the report to a file instead of `to_log(LOG_INFO, ...)`.

**Output (per context, on context destroy):**
```
=== Memory leaks in context '<unnamed>' (3 allocations, 0x2b0 bytes) ===
[leak] size=0x200 bytes
  at builtin`push`4379756157886752001 daslib/builtin.das:117
  at make_big_leak examples/leak_smoke.das:14
  at main examples/leak_smoke.das:37
```

Sizes are in hex, stacks are leaf-first, `file:line` is terminal-clickable. Multi-context programs (audio thread, jobque workers) get one block per context.

**Read it:** the leaf frame is where the allocation call sits; walk up the stack to find the scope that owns the memory and failed to free it.

**Don't:** enable this for a shipping build. Every allocation clones the shadow call stack — it's a debug tool.

---

## #2 — `-track-allocations -heap-report` (per-Context heap dump)

**Scope:** one report per main context at exit, listing all live blocks in the Context's heap + string heap.

```bash
bin/daslang -track-allocations -heap-report path/to/script.das
```

**Output (default linear heap — one line per chunk):**
```
--- heap report ---
2b0202057	944 of 65536
--- string heap report ---
2b0191a05	52 of 65536
```

Columns: chunk address, bytes in use, chunk size. The linear allocator is bulk-reset, so you don't see individual blocks.

**Output (with `options persistent_heap = true`):**
```
--- heap report ---
big stuff:
	size	pointer		id
	512	0x2069eeae630	1	array
	48	0x2069ece4c20	2	new [[ ]]	D:/script.das:22:12
bytes per location:
48	D:/script.das:22:12
```

The persistent allocator shows each live block separately: size, pointer, sequential id, optional comment (`array`, `new [[ ]]`, `table`, …), optional `file:line` from the LineInfo at the allocation site. "bytes per location" aggregates totals by source site.

**vs #1:** #2 is per-Context with raw blocks and a single LineInfo; #1 is cross-context with full call stacks. Use #1 when you want to know *how* a block was allocated; use #2 for a quick survey of "what's alive right now."

---

## #3 — gc_node leak detection (automatic, AST nodes)

**Scope:** every `gc_node`-derived AST type (`TypeDecl`, `Expression`, `Function`, `Structure`, `Enumeration`, `Variable`, `MakeFieldDecl`, `MakeStruct`, …) that outlives compilation or execution.

**Invoke:** nothing. The host checks `gc_root::gc_get_thread_root().gc_count` after compile+simulate (`GC COMPILE LEAK`) and after main returns (`GC APP LEAK`). Any non-zero count triggers `gc_report()`.

**Output:**
```
GC APP LEAK: 3 gc_node(s) after execution
gc_root 0x7ff...: count=3
  node 0x7ff...: id=1234 type=TypeDecl magic=0xDA5C0001
  ...
```

**Narrow it down:** set `DAS_GC_BREAK_ON_ID=<id>` as an environment variable (pick an id from the report), then run under a debugger. The check fires in the gc_node constructor and triggers `os_debug_break()` — you get the full C++ + daslang stack trace for the creation site.

```bash
DAS_GC_BREAK_ON_ID=1234 bin/daslang path/to/script.das
```

**Common cause:** daslang code that builds AST at runtime (`clone_type`, `new TypeDecl`, `qmacro`) without wrapping in `ast_gc_guard() { ... }` from `daslib/ast`. Wrap the build scope in `ast_gc_guard` and the leak goes away.

**Don't:** ignore GC leak reports. A non-zero count at exit usually means ownership is wrong somewhere, and the symptom can cascade into broken AOT hashes or crashes on subsequent runs.

---

## #4 — `--track-smart-ptr <hexId>` (single smart_ptr trace)

**Scope:** one specific `ptr_ref_count` instance — Context, Program, FileAccess, or any other residual smart_ptr.

```bash
bin/daslang --track-smart-ptr 0x5a path/to/script.das
```

The id is the `ref_count_id` of the target object — usually read from a prior exit-time `DumpTrackPtr` line like `0x7ffee1301000 (rc=2, id=5a) Context main_ctx`.

**Behavior:** every `addRef`, `delRef`, and destructor on that specific id hits `os_debug_break()`. Attach a debugger (or `-das-wait-debugger`) to collect stack traces.

**Automatic companion:** at exit `bin/daslang` prints all surviving smart pointers. Read a suspect id from there, rerun with `--track-smart-ptr <id>`, debug.

---

## #5 — JobStatus / Channel / LockBox / Feature

`DumpJobQueLeaks()` runs automatically at exit when `JobStatus`/`Channel`/`LockBox`/`Feature` survivors exist. See the dedicated [`skills/jobque_debugging.md`](jobque_debugging.md) for the full workflow (refcount accounting, shutdown order, `--max-frames`, capture-macro hidden refs, lockbox fill/grab/join lifecycle).

---

## #6 — `HandleRegistry<T>` (dasHV WebSocket and similar handles)

**Scope:** value-sized handles backed by `std::shared_ptr<T>` on the C++ side — used by dasHV for `hv::WebSocketClient`, `hv::WebSocketServer`, `hv::WebSocketChannel`, and any custom handle type registered via `addHandleAnnotation<T>`.

**Invoke:** nothing. `handleRegistry_dumpAll()` runs inside `Module::Shutdown(dumpLeaks)`.

**Output:**
```
  Handle<WebSocketClient> idx=3 gen=1 (rc=1)
  Handle<WebSocketServer> idx=0 gen=9 (rc=1)
total 1 leaked handles of type WebSocketClient
total 1 leaked handles of type WebSocketServer
```

Columns: slot index, generation counter (rolls on release/reacquire), `shared_ptr::use_count()` at dump time. `rc=1` = registry is the sole owner (textbook leak). `rc>1` = another strong ref is keeping the object alive — look for forgotten captures.

**Read it:** match leaked `Handle<TypeName>` against the acquire site (e.g. `makeWebSocketClient`, `makeWebSocketServer`). The leak means user code reached script exit without calling the matching destroy function (e.g. `destroy_web_socket_client`) or without triggering the wrapping class's `operator delete` (e.g. via `inscope` / explicit `delete`).

**Advisory, not fatal:** the dump prints but does not change exit code. Only `ptr_ref_count` leaks force `exit(1)`.

---

## Silencing exit-time dumps

Pass `--no-dump-leaks` to make all three exit-time dumps (JobStatus, HandleRegistry, smart_ptr TextPrinter) quiet. The `exit(1)` on smart_ptr leak is preserved — it's a failure signal, not diagnostic noise. Useful when pre-existing noisy leaks would drown out the signal you're chasing.

---

## Before reporting a leak

Common non-leaks that look like leaks:

- **Missing `inscope` on a smart_ptr** (`ProgramPtr`, `ContextPtr`, `FileAccessPtr`) — the value lives for the context's lifetime by design. Fix: add `inscope`, or explicit `delete`.
- **`var arr : array<int>` with no `delete arr` and no `inscope`** — arrays have NO scope-based RAII; the heap-side data survives until context destroy. Use explicit `delete arr` or move ownership out.
- **Channel / Stream not released** — holding past the last `s |> release()` / `s |> notify_and_release()` leaks the primitive. See `skills/jobque_debugging.md`.
- **Capture-macro hidden addRef** — lambdas over JobStatus/Channel/Stream bump the refcount via `capture_macro` in `jobque_boost.das`; the matching release fires at lambda destruction. If the lambda outlives expectations, the ref outlives it too.
- **String interning** — `intern`'d strings live in `constStringHeap` for the program lifetime. Not a leak.
- **Persistent heap option** — `options persistent_heap = true` keeps allocated memory across context runs intentionally.
