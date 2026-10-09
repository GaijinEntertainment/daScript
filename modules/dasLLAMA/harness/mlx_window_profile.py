"""The house window under mlx-lm: a PREFIX-token prompt cache, then a WINDOW-token prefill timed as served,
then the same window with every module class's call forced to evaluate for a per-class exclusive breakdown.
The recipe is ARCHITECTURE_MEASUREMENT.md#mlx-window-recipe; its readings sit in PERF_LEDGER.md as `external`.

python mlx_window_profile.py [model] [--prefix 4500] [--window 375] [--reps 4]
"""

import argparse
import collections
import statistics
import time

import mlx.core as mx
import mlx.nn as nn
from mlx_lm import load
from mlx_lm.models.cache import make_prompt_cache

TEXT = ("The kitchen light is on, the bedroom lights are off, the sprinkler runs at seven in the morning and again at dusk. "
        "The thermostat holds twenty-one degrees while anyone is home and drops to eighteen overnight. "
        "The garage door reports closed; the front door camera saw a courier at ten past two. "
        "Groceries arrive on Thursday; the dishwasher finished its cycle an hour ago and wants emptying. ")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("model", nargs="?", default="mlx-community/Qwen3.6-35B-A3B-4bit")
    parser.add_argument("--prefix", type=int, default=4500)
    parser.add_argument("--window", type=int, default=375)
    parser.add_argument("--reps", type=int, default=4)
    parser.add_argument("--chunk", type=int, default=512)
    parser.add_argument("--top", type=int, default=30)
    args = parser.parse_args()

    model, tok = load(args.model)
    ids = []
    while len(ids) < args.prefix + args.window:
        ids += tok.encode(TEXT)
    ids = mx.array(ids[: args.prefix + args.window])
    print(f"model {args.model}: mlx {mx.__version__}, device {mx.default_device()}, {len(ids)} tokens")

    def run_window(cache, start, end):
        for s in range(start, end, args.chunk):
            e = min(s + args.chunk, end)
            out = model(ids[s:e][None], cache=cache)
            mx.eval(out, [c.state for c in cache])
        return out

    cache = make_prompt_cache(model)
    t0 = time.perf_counter()
    run_window(cache, 0, args.prefix)
    print(f"prefix {args.prefix} tokens: {1000 * (time.perf_counter() - t0):.1f} ms")
    snap = [c.state for c in cache]
    mx.eval(snap)

    def restore():
        for c, s in zip(cache, snap):
            c.state = s

    # the window as served: median of reps after a warmup rep, each from the same cache state
    walls = []
    for rep in range(args.reps + 1):
        restore()
        mx.synchronize()
        t0 = time.perf_counter()
        out = model(ids[args.prefix:][None], cache=cache)
        mx.eval(out, [c.state for c in cache])
        wall = 1000 * (time.perf_counter() - t0)
        if rep > 0:
            walls.append(wall)
    print(f"window {args.window} tokens as served: median {statistics.median(walls):.1f} ms, reps {[round(w, 1) for w in walls]}")

    # the attributed pass: every module class's __call__ evaluates its result; exclusive = inclusive - children
    classes = {type(m) for _, m in model.named_modules()}
    stack = []
    excl = collections.defaultdict(float)
    incl = collections.defaultdict(float)
    counts = collections.Counter()
    originals = {}

    def make_timed(cls, orig):
        def timed(self, *a, **kw):
            frame = {"children": 0.0}
            stack.append(frame)
            t0 = time.perf_counter()
            result = orig(self, *a, **kw)
            mx.eval(result)
            dt = time.perf_counter() - t0
            stack.pop()
            if stack:
                stack[-1]["children"] += dt
            incl[cls.__name__] += dt
            excl[cls.__name__] += dt - frame["children"]
            counts[cls.__name__] += 1
            return result
        return timed

    for cls in classes:
        if "__call__" in cls.__dict__:
            originals[cls] = cls.__dict__["__call__"]
            cls.__call__ = make_timed(cls, originals[cls])
    attributed = []
    for rep in range(2):   # the first attributed rep warms the per-module graphs; the second is reported
        restore()
        excl.clear()
        incl.clear()
        counts.clear()
        mx.synchronize()
        t0 = time.perf_counter()
        out = model(ids[args.prefix:][None], cache=cache)
        mx.eval(out, [c.state for c in cache])
        attributed.append(1000 * (time.perf_counter() - t0))
    for cls, orig in originals.items():
        cls.__call__ = orig
    total = sum(excl.values()) * 1000
    print(f"\nattributed pass: {attributed[-1]:.1f} ms wall ({sum(counts.values())} module calls, each forced to evaluate)")
    print(f"{'class':34s} {'calls':>6s} {'exclusive ms':>13s} {'share':>7s} {'inclusive ms':>13s}")
    for name, ms in sorted(excl.items(), key=lambda kv: -kv[1])[: args.top]:
        print(f"{name:34s} {counts[name]:6d} {1000 * ms:13.2f} {100 * 1000 * ms / total:6.1f}% {1000 * incl[name]:13.2f}")
    print(f"{'sum of exclusive':34s} {'':6s} {total:13.2f}")


main()
