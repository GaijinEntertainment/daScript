# Building a long-running dasImgui application

Use this recipe when creating or reviewing a dasImgui program that runs as a
standalone `daslang` process, under `daslang-live`, or both. The production
reference for heap ownership is `utils/dasllama-server/main.das`; the compact UI
reference is `examples/tutorial/application_lifecycle.das`.

## Required lifecycle

Expose exactly these lifecycle functions:

```das
[export] def init() { ... }
[export] def update() { ... }
[export] def shutdown() { ... }

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
        maybe_collect_gc()
    }
    shutdown()
}
```

`daslang-live` owns the exported lifecycle and does not call `main`. Standalone
`daslang` calls `main`, which must own the same ordering.

## Heap contract

Long-running allocation-heavy programs need both:

```das
options persistent_heap
options gc
```

- `persistent_heap` makes the context heap collectable.
- `gc` enables collection in that context. It does **not** make ordinary JIT
  stack locals traceable.
- Durable state belongs in module globals, not in a `main` local held across a
  collection.
- Never collect in the middle of `update`, a widget tree, a callback, or a
  request handler while transient collectable values are still required.

## Delete first; collect rarely

GC is unavoidable as a safety net, but it should run once in a blue moon—not
as routine frame cleanup.

- `delete` uniquely-owned transient arrays, strings, tables, JSON graphs, and
  replaced caches when their ownership ends.
- Cache stable derived data. Rebuild and delete the previous value only when an
  input invalidates it.
- Prefer move ownership (`<-`) so there is one obvious owner to delete.
- Treat GC as the cleanup for unreachable leftovers, cycles, complex error
  paths, and heap compaction—not as a substitute for ownership.

Calling a `maybe_collect_gc` helper every loop is only a cheap eligibility
check. The helper must use fragmentation/growth thresholds (and, for expensive
services, a time throttle) so actual collections remain rare. The
dasllama-server implementation is the production reference.

Collecting while a live string, array, table, JSON value, closure, or other
collectable object exists only in a JIT local can reclaim it and crash. The safe
standalone boundary is after `update()` returns, with no collectable locals in
the caller's `main` frame. A raw service follows the dasllama-server shape:

```das
while (!exit_requested()) {
    update()
    maybe_collect_gc()
}
```

The collector should return immediately in `is_live_mode()` because the host
already collects between update calls. For high-throughput services, copy the
server's rate limit and heap/string-heap fragmentation checks rather than
forcing a full collection every tick.

## dasImgui harness rule

`imgui/imgui_harness` exposes the equivalent boundary as
`harness_maybe_collect_gc()`. Call it from standalone `main` immediately after
`update` returns. Under `daslang-live` that call is a no-op:

```das
[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    // build UI
    harness_end_frame()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
        harness_maybe_collect_gc()
    }
    shutdown()
}
```

## Ownership checklist

- [ ] `init`, `update`, and `shutdown` are exported.
- [ ] Standalone `main` uses `init -> update* -> shutdown`.
- [ ] `options persistent_heap` and `options gc` are enabled.
- [ ] Long-lived state is global or otherwise rooted.
- [ ] Standalone `main` holds no collectable local across the GC call.
- [ ] Transient owned values and invalidated caches are explicitly deleted.
- [ ] Stable derived state is cached instead of rebuilt every frame.
- [ ] Actual collections are thresholded/rate-limited, not forced every tick.
- [ ] Collection occurs only between completed updates.
- [ ] Live mode does not perform a second application-owned collection.
- [ ] `harness_maybe_collect_gc()` follows `update()` in standalone `main`.
- [ ] Every successful frame pairs `harness_new_frame` with
      `harness_end_frame`.
- [ ] `shutdown` releases resources in reverse ownership order.
- [ ] Headless standalone runs have a frame or uptime exit cap.

## Verification

Run the application headless for enough frames to expose monotonic heap growth,
and sample process private bytes. A collectable heap can reserve memory and
plateau above its initial footprint; the failure signal is unbounded growth, not
the plateau's absolute size. Also exercise the live host: collection and
teardown must remain host-owned across reloads.
