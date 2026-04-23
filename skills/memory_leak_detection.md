# Memory Leak Detection

daslang ships six distinct leak-detection mechanisms, each narrow. This skill
is the master index — read it first, pick the right tool, then dive into the
per-mechanism details (or the dedicated skill for #5, `jobque_debugging.md`).

The user-facing version lives at
`doc/source/reference/utils/memory_leak_detection.rst` and covers the same
material in docs style.

## Quick decision tree

| Symptom | Tool |
|---|---|
| daslang run exits 0 but prints `GC COMPILE LEAK` / `GC APP LEAK` | **#3 gc_node** (`skills/gc_migration.md`) |
| Script leaks a daslang `new Foo()` / `array<T>` / table, want per-alloc call stack | **#1 `--das-profiler-leaks`** (`doc/source/reference/utils/profiler.rst`) |
| Long-running run keeps growing, or want per-context heap dump at exit | **#2 `-track-allocations -heap-report`** (single dash) |
| Know a specific smart_ptr id is misbehaving, want debug-break on every addRef/delRef | **#4 `--track-smart-ptr <hexId>`** |
| Script uses channels/jobs/lockboxes, `DumpJobQueLeaks` printed survivors | **#5 `--track-job-status`** (`skills/jobque_debugging.md`) |
| Long-running dasHV server suspected to leak WebSocket-client handles | **#6 `HandleRegistry` auto-dump** — `Handle<TypeName>` lines at process exit list unreleased handles |

**CLI dash convention:** `-track-allocations` and `-heap-report` use **one**
leading dash; `--track-smart-ptr`, `--track-job-status`, and
`--das-profiler-leaks` use **two**. This is a historical inconsistency in
`utils/daScript/main.cpp` — preserve it as-is in the commands you suggest.

Order of investigation if you see multiple leak reports at exit: fix #3
(gc_node) first (any survivor indicates an ownership bug that can cascade),
then #5 (job-que primitives), then #2/#1 (heap), then #4/#6.

---

## #1 — `--das-profiler-leaks` (daslang heap with captured call stacks)

**Scope:** every live allocation on a daslang `Context` heap, grouped per
context, with the full daslang call stack that produced it.

**Invoke:**
```bash
bin/Release/daslang.exe --das-profiler --das-profiler-leaks path/to/script.das
```

Optional `--das-profiler-log-file <path>` to write the report to a file
instead of `to_log(LOG_INFO, ...)`.

**Output (per context, on context destroy):**
```
=== Memory leaks in context '<unnamed>' (3 allocations, 0x2b0 bytes) ===
[leak] size=0x200 bytes
  at builtin`push`4379756157886752001 daslib/builtin.das:117
  at make_big_leak examples/leak_smoke.das:14
  at main examples/leak_smoke.das:37
```

Sizes in hex, stacks leaf-first, `file:line` is VSCode-terminal-clickable.
Multi-context programs (strudel audio thread, jobque workers) get one block
per context.

**Read it:** the leaf frame is where the allocation call sits; walk up the
stack to find the scope that owns the memory and failed to free it.

**Don't:** enable this for a shipping build. Every allocation clones the
shadow call stack — it's a debug tool.

See `doc/source/reference/utils/profiler.rst` for the full guide (also covers
the performance profiler mode).

---

## #2 — `-track-allocations -heap-report` (C++ heap dump)

**Scope:** one report per main context at exit, listing all live blocks in
the Context's heap + string heap. Captures each allocation's size, pointer,
id, optional comment, and daslang `file:line`.

**Invoke:**
```bash
bin/Release/daslang.exe -track-allocations -heap-report path/to/script.das
```

**Output (default linear heap — one line per chunk):**
```
--- heap report ---
2b0202057	944 of 65536
--- string heap report ---
2b0191a05	52 of 65536
```

Columns: chunk address, bytes in use, chunk size. The linear allocator is
bulk-reset, so you don't see individual blocks.

**Output (with `options persistent_heap = true`):**
```
--- heap report ---
big stuff:
	size	pointer		id
	512	0x2069eeae630	1	array
	48	0x2069ece4c20	2	new [[ ]]	D:/script.das:22:12
	128	0x2069f1f4fa0	3	array
bytes per location:
48	D:/script.das:22:12
```

**Read it:** the persistent allocator shows each live block separately.
"big stuff" rows: size, pointer, sequential id, optional comment (`array`,
`new [[ ]]`, `table`, etc. — stamped by the runtime helper), optional
`file:line` from the LineInfo at the allocation site. "decks" (when
present) shows slab occupancy per size class. "bytes per location"
aggregates totals by source site.

**Build-time gate:** tracking is compiled into every build by default via
`DAS_TRACK_ALLOCATIONS=1` in `include/daScript/misc/platform.h`. Shipping
builds can set it to 0 to dead-code-eliminate the infrastructure. Runtime
gate: the `-track-allocations` CLI flag sets
`policies.track_allocations = true` which propagates to each heap's
`setTrackAllocations(true)`.

**vs #1 (`--das-profiler-leaks`):** #2 is per-Context and shows raw blocks
with a single LineInfo; #1 is cross-context and shows full call stacks.
Use #1 when you want to know *how* a block was allocated, #2 when you want a
quick survey of "what's alive right now".

---

## #3 — gc_node leak detection (automatic, AST nodes)

**Scope:** every `gc_node`-derived AST type (TypeDecl, Expression, Function,
Structure, Enumeration, Variable, MakeFieldDecl, MakeStruct, etc.) that
outlives compilation or execution.

**Invoke:** nothing. `utils/daScript/main.cpp` and
`utils/daslang-live/main.cpp` check `gc_root::gc_get_thread_root().gc_count`
at two points: after compile+simulate (`GC COMPILE LEAK`) and after main
returns (`GC APP LEAK`). Any non-zero count triggers `gc_report()`.

**Output:**
```
GC APP LEAK: 3 gc_node(s) after execution
gc_root 0x7ff...: count=3
  node 0x7ff...: id=1234 type=TypeDecl magic=0xDA5C0001
  node 0x7ff...: id=1235 type=Expression magic=0xDA5C0002
  ...
```

**Narrow it down:** set `DAS_GC_BREAK_ON_ID=<id>` as an environment variable
(pick an id from the report), then run under a debugger. `gc_break_on_id` is
checked in the gc_node constructor and triggers `os_debug_break()` — you get
the full C++ + daslang stack trace for the creation site.

```bash
DAS_GC_BREAK_ON_ID=1234 bin/Debug/daslang.exe path/to/script.das
```

**Common causes:** daslang code that creates AST nodes at runtime
(`clone_type`, `new TypeDecl`, `qmacro`) without wrapping in
`ast_gc_guard() { ... }`. See `skills/gc_migration.md` for the full
migration background and fix patterns.

**Don't:** ignore GC leak reports. A non-zero count at exit usually means
ownership is wrong somewhere, and the symptom can cascade into broken AOT
hashes or crashes on subsequent runs.

---

## #4 — `--track-smart-ptr <hexId>` (single smart_ptr trace)

**Scope:** one specific `ptr_ref_count` subclass instance (Context, Program,
FileAccess, gc-migrated types via their policy, any other smart_ptr).

**Invoke:**
```bash
bin/Release/daslang.exe --track-smart-ptr 0x5a path/to/script.das
```

The id is the `ref_count_id` of the target object — usually read from a
prior `DumpTrackPtr()` dump line like
`0x7ffee1301000 (rc=2, id=5a) Context main_ctx`.

**Behavior:** every `addRef`, `delRef`, and destructor on that specific id
hits `os_debug_break()`. Attach a debugger (or `-das-wait-debugger`) to
collect stack traces.

**Automatic companion:** at exit `daslang.exe` calls
`ptr_ref_count::DumpTrackPtr()` (`utils/daScript/main.cpp:723`) which prints
all surviving smart pointers. Read a suspect id from there, rerun with
`--track-smart-ptr <id>`, debug.

**Sister flag:** `ref_count_track_destructor` is a second static id (not
exposed on the CLI yet) that breaks only on the destructor call. Set via
debugger if you need it.

---

## #5 — JobStatus / Channel / LockBox / Feature (threading primitives)

**Scope:** `JobStatus` and its subclasses (`Channel`, `LockBox`) plus
`Feature` value-type — manually-refcounted synchronization primitives.

**Invoke:**
```bash
bin/Release/daslang.exe --track-job-status <id> path/to/script.das
```

`DumpJobQueLeaks()` runs automatically at exit. See the dedicated skill
[`skills/jobque_debugging.md`](jobque_debugging.md) for the full workflow
(refcount accounting, shutdown order, `--max-frames`, capture-macro hidden
refs, lockbox fill/grab/join lifecycle).

---

## #6 — `HandleRegistry<T>` (dasHV WebSocket handles) — automatic

**Scope:** value-sized handles backed by `std::shared_ptr<T>` on the C++
side. Used by dasHV for `hv::WebSocketClient`, `hv::WebSocketServer`,
`hv::WebSocketChannel`. Infrastructure in
`include/daScript/misc/handle_registry.h`.

**Invoke:** nothing. `handleRegistry_dumpAll()` runs inside
`Module::Shutdown(dumpLeaks)` in both `daslang.exe` and `daslang-live.exe`,
in the window between the module destructor loop (which drains job
threads via `Module_JobQue::~Module_JobQue`) and the
`DynamicModuleInfo` teardown that unloads shared modules. That ordering
is deliberate: before the window, live job threads legitimately hold
handles; after it, the `dumpHandleLeaks<T>` function pointers registered
from shared-module DLLs are dangling. Every handle type registered via
`addHandleAnnotation<T>` auto-registers a per-type dump callback — no
per-module boilerplate.

**Output:**
```
  Handle<WebSocketClient> idx=3 gen=1 (rc=1)
  Handle<WebSocketServer> idx=0 gen=9 (rc=1)
total 1 leaked handles of type WebSocketClient
total 1 leaked handles of type WebSocketServer
```

Columns: slot index, generation counter (rolls on release/reacquire), and
`shared_ptr::use_count()` at dump time. `rc=1` = registry is the sole
owner (textbook leak). `rc>1` = another strong ref is keeping the object
alive — look for forgotten captures.

**Type names** come from `typeName<T>::name()` — for handle types this is
wired via `MAKE_EXTERNAL_TYPE_FACTORY(Name, hv::Name)` in
`modules/dasHV/src/dasHV.h`. Any new handle type introduced via
`addHandleAnnotation<T>` that does NOT have a `typeName<T>` specialization
fails to compile (by design — compile-time enforcement of a readable name).

**Manual query** (still useful for in-process programmatic inspection):
```cpp
auto & reg = HandleRegistry<hv::WebSocketClient>::instance();
auto n = reg.live_count();
if ( n ) {
    reg.for_each_live([](Handle<hv::WebSocketClient> h, auto & p){
        // log h.value, p.use_count(), p.get()
    });
}
```

**Read it:** match leaked `Handle<TypeName>` against the acquire site (e.g.
`makeWebSocketClient`, `makeWebSocketServer`). The leak means user code
reached script exit without calling the matching destroy function (e.g.
`destroy_web_socket_client`) or without triggering the wrapping class's
`operator delete` (e.g. via `inscope` / explicit `delete`).

**Disabled modules:** `handleRegistry_dumpAll()` is cheap even when dasHV
is disabled via `DAS_HV_DISABLED=ON` — the module's TUs aren't compiled, so
no callbacks are registered, and the dump iterates an empty hooks vector.

**Advisory, not fatal:** the dump prints but does not change exit code
(matches `DumpJobQueLeaks` precedent). Only `ptr_ref_count` leaks force
`exit(1)`.

**Silencing all three exit-time dumps:** pass `--no-dump-leaks` to
`daslang.exe` / `daslang-live.exe` and the JobStatus, HandleRegistry, and
smart_ptr TextPrinter dumps all become quiet (the exit(1) on smart_ptr
leak is preserved — it's a failure signal, not diagnostic noise).
Default is on. Use this in environments where pre-existing noisy leaks
would drown out the signal you're looking for, or when another tool is
consuming daslang output.

---

## Before reporting a leak

Common non-leaks that look like leaks:

- **Missing `inscope`** on a `smart_ptr<T>` / `var inscope` on array/table-like
  types — the value lives for the context's lifetime by design. Fix: add
  `inscope`, or explicit `delete`.
- **`var arr : array<int>` with no `delete arr` and no `inscope`** — arrays
  have NO scope-based RAII (see CLAUDE.md memory); the heap-side data
  survives until context destroy. `inscope` triggers a finalizer; `delete`
  is explicit.
- **Channel / Stream not released** — holding the Stream past the last
  `s |> release()` / `s |> notify_and_release()` call leaks the primitive.
  See `skills/jobque_debugging.md`.
- **Capture-macro hidden addRef** — lambdas over JobStatus/Channel bump the
  refcount via `capture_macro` in `jobque_boost.das`; the matching release
  fires at lambda destruction. If the lambda outlives expectations, the ref
  outlives it too.
- **String interning** — `intern`'d strings live in the `constStringHeap`
  for the program lifetime. Not a leak.
- **Persistent heap option** — `options persistent_heap = true` keeps
  allocated memory across context runs intentionally.

---

## Which tool for which allocation class

| Allocation class | Best tool |
|---|---|
| `new Foo()`, `new SomeStruct(...)` in daslang | #1 (call stack) or #2 (per-block id+site) |
| `array<T>` / `table<K;V>` growth | #1 (shows `builtin\`reserve\``/`push`) |
| String allocations | #2 `stringHeap->report()` |
| AST node leak from daslang macro | #3 gc_node (always on) |
| `smart_ptr<Context>` / `smart_ptr<Program>` stuck | #4 via the `DumpTrackPtr` exit dump |
| Channel / LockBox / JobStatus survivor | #5 (`skills/jobque_debugging.md`) |
| Native `hv::WebSocketClient` / `Channel` / `Server` | #6 |

---

## Cross-references

- `doc/source/reference/utils/profiler.rst` — user-facing guide for #1.
- `doc/source/reference/utils/memory_leak_detection.rst` — user-facing version of this skill.
- `skills/jobque_debugging.md` — full workflow for #5.
- `skills/gc_migration.md` — background for #3 and `ast_gc_guard`.
- `include/daScript/misc/handle_registry.h` — dasHV handle infrastructure (#6).
- `include/daScript/misc/gc_node.h` — gc_node header (`DAS_GC_BREAK_ON_ID`).
- `include/daScript/misc/smart_ptr.h` — `ptr_ref_count`, `ref_count_track` (#4).
- `include/daScript/misc/platform.h:448-456` — `DAS_TRACK_ALLOCATIONS` compile-time gate (#2).
