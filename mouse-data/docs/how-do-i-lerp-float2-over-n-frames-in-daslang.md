---
slug: how-do-i-lerp-float2-over-n-frames-in-daslang
title: How do I lerp between two float2 positions over N frames in daslang? (Frame-counter animation pattern.)
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**Use `math::lerp(a, b, t)`** for the per-frame value. The vector overloads exist for `float2` / `float3` / `float4` directly — no hand-rolled component arithmetic needed.

```das
require math

struct LerpState {
    from         : float2
    to           : float2
    frames_total : int
    frames_left  : int
    active       : bool
}

var g_lerp : LerpState

def start_lerp_to(target : float2; frames : int) {
    g_lerp.from = g_lerp.active ? tick_lerp() : g_lerp.to    // start from where we are
    g_lerp.to = target
    g_lerp.frames_total = frames > 0 ? frames : 1
    g_lerp.frames_left = g_lerp.frames_total
    g_lerp.active = true
}

def tick_lerp : float2 {
    return g_lerp.to if (!g_lerp.active)
    if (g_lerp.frames_left > 0) {
        g_lerp.frames_left--
        let t = 1.0f - float(g_lerp.frames_left) / float(g_lerp.frames_total)
        return lerp(g_lerp.from, g_lerp.to, t)
    }
    return g_lerp.to     // arrived; stay
}
```

Call `tick_lerp()` once per frame from a per-frame hook (e.g. `register_end_of_frame_hook` in dasImgui, or `[before_update]` in daslang-live).

## `lerp` overloads (math module)

Registered in [`src/builtin/module_builtin_math.cpp:718-720`](https://github.com/GaijinEntertainment/daScript/blob/master/src/builtin/module_builtin_math.cpp#L718) — `lerp(float2, float2, float)`, `lerp(float3, float3, float)`, `lerp(float4, float4, float)` all bind to `lerp_vec_float`. Scalar `lerp(T, T, T)` is the generic templated overload at line 223. **`require math`** to bring them into scope.

## Easing variants

The linear-`t` formula above is uniform velocity. For ease-out (decelerate to target — usually what mouse-cursor moves want):

```das
let t = 1.0f - g_lerp.frames_left / g_lerp.frames_total
let eased = t * (2.0f - t)        // quadratic ease-out, no math import needed
```

For ease-in-out (smooth-step): `t*t*(3.0f - 2.0f*t)`. All cheap, no transcendentals.

## Caller dt vs frame counter

A frame counter (`frames_left`) is right when the animation is **frame-locked** (driven from the same per-frame hook that calls `tick_lerp`). For wall-clock duration (300ms regardless of fps), use `get_uptime()` deltas instead — but that's a different state struct (`end_time : float`). Frame-counter is simpler when the consumer is already a per-frame hook (mouse-trail repaint, etc).

## Common shape outside daslang

Same pattern as engines/imgui internals — see imgui's `ImLerp` or any per-frame state machine. The daslang version is verbose only because there's no syntactic `vec += other * t` sugar; the math is the same.

Surfaced 2026-05-11 while planning the synthetic mouse driver for dasImgui Phase 5 recording.

## Questions
- How do I lerp between two float2 positions over N frames in daslang? (Frame-counter animation pattern.)
- What's the frame-counter animation pattern in daslang for cursor moves / camera tweens?
- Does `lerp(float2, float2, float)` exist in the math module?
- How do I add ease-in / ease-out to a linear lerp?
