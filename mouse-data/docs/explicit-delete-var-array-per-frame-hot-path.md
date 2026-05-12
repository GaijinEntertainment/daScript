---
slug: explicit-delete-var-array-per-frame-hot-path
title: How do I explicitly free a per-frame var array in a daslang hot-path function so the heap doesn't grow each call?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

`var arr : array<T>` in a function body **does NOT finalize on scope exit** in daslang. The array's heap allocation outlives the function — per-frame use leaks one allocation per call, the heap grows linearly with frame count.

CLAUDE.md spells it out explicitly:
> daslang has garbage collection, but plain `var arr : array<T>` does NOT finalize on scope exit. Either declare with `var inscope` (smart_ptr only), call `delete` explicitly, or move out via `<-`. Per-frame leaks in hot paths usually trace to a local `var arr` never deleted.

**Fix — one line, `delete arr` at end of function:**

```das
[before_update]
def record_tick() {
    return if (!recorder_active)
    ...
    var pixels : array<uint8>
    pixels |> resize(recorder.width * recorder.height * 4)
    glReadPixels(..., addr(pixels[0]))
    stbi_apng_frame(recorder.writer, addr(pixels[0]), row_bytes, delay_ms)
    // Local `var array<T>` doesn't finalize on scope exit (no smart_ptr).
    // Without this, the heap grows by width*height*4 bytes per captured frame.
    delete pixels
}
```

For a 1920×1080 RGBA recording at 30 fps over a minute: ~8 MB × 1800 = ~14 GB of leaked allocations without the `delete`.

**Alternative — persistent buffer in a struct:** Move `pixels` into the surrounding state struct (e.g. `RecorderState`), resize only when the framebuffer dims change. Trades per-frame alloc churn for permanent state, fine but more code than the one-liner.

**When `delete` is wrong:** if you intend to return the array (`return <- arr`) or move it via `<-` into a longer-lived owner, don't delete — the move transfers the heap allocation. Only `delete` when the array is genuinely scope-local *and* never moved out.

A persistent buffer would also work and avoids the per-frame allocation cost (not just the leak); `delete` accepts the alloc but fixes the leak. Choose by whether allocation churn or leak avoidance is the bigger concern.

Caught originally in dasOpenGL/opengl/opengl_live.das `record_tick` (APNG recorder) — Copilot flagged it as "GC pressure" but the real bug was leak, not pressure.

## Questions
- How do I explicitly free a per-frame var array in a daslang hot-path function so the heap doesn't grow each call?
