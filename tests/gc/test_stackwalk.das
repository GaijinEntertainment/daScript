options gen2
options rtti
options gc

require strings
require dastest/testing_boost public
require _stackwalk_common
require _gc_forceat_macro
require daslib/rtti public
require daslib/debugger public

// frame_position() resolves to the call site's position in its
// function's numbering space - the same value the GC locals gate
// reads from the frame handoff

def positions_pair : tuple<a : int; b : int> {
    let a = frame_position()
    let b = frame_position()
    return (a = a, b = b)
}

[inline]
def inlined_position : int => frame_position()

[test(description = "frame_position stamps call sites with nonzero, preorder-monotonic positions, spliced sites included")]
def test_frame_position(t : T?) {
    t |> run("positions are nonzero and preorder-monotonic within a body") @(t : T?) {
        let p = positions_pair()
        t |> success(p.a > 0, "first position must be stamped")
        t |> success(p.b > p.a, "later call sites number higher")
    }
    t |> run("a spliced call site carries a host-space position") @(t : T?) {
        t |> success(inlined_position() > 0, "spliced sites are stamped like native ones")
    }
}

// the force_at pattern collapses a generated body's locations onto
// one token; the generated function's locals must still gate live at
// parks inside it

[gen_forced_walker]
struct ForcedMarker {
    unused : int
}

[test(description = "a let in a force_at-stamped generated body is live at a park inside it")]
def test_stackwalk_forceat(t : T?) {
    t |> run("force_at-stamped generated body's let is live at its park") @(t : T?) {
        var w = new CollectingWalker()
        let r = forced_walker(w)
        t |> equal(r, length("FORCED_0"))
        t |> success(has_live_local(w, "forced_precious"),
                     "forced_precious must be live in the walk")
        unsafe {
            delete w
        }
    }
}

// under default auto-inlining the helper chain splices into the test
// arm; every LIVE local must still report in scope.
// uncategorized_auto_inline_functions_false_gc_rtti.das pins the
// unspliced shape.

def tiny_helper(var w : CollectingWalker?; tag : int) {
    var pad = tag + 1
    if (pad > 0) {
        run_walk(w)
    }
    pad++
}

def owner_frame(var w : CollectingWalker?; seed : int) : int {
    let counter = seed + 40
    let precious = "PRECIOUS_{counter}"
    tiny_helper(w, counter)
    return length(precious) + counter
}

[test(description = "live locals of a helper chain spliced into the caller all report in scope while the spliced frames vanish")]
def test_stackwalk_inlined_locals(t : T?) {
    t |> run("live locals of an inlined chain report in scope") @(t : T?) {
        var w = new CollectingWalker()
        let r = owner_frame(w, length(w.frames) + 2)
        t |> equal(r, length("PRECIOUS_42") + 42)
        t |> success(!has_frame(w, "tiny_helper"), "tiny_helper was spliced away")
        t |> success(!has_frame(w, "owner_frame"), "owner_frame was spliced away")
        t |> success(has_live_local(w, "counter"), "counter must be live in the walk")
        t |> success(has_live_local(w, "precious"), "precious must be live in the walk")
        t |> success(has_live_local(w, "pad"), "pad must be live in the walk")
        unsafe {
            delete w
        }
    }
}

// the liveness interval is field-readable das-side: a walker sees
// openPos/closePos on LocalVariableInfo and the owner space on
// FuncInfo

class IntervalWalker : CollectingWalker {
    intervals : table<string; tuple<open : uint; close : uint>>
    spaces : table<string; uint>
    def override onVariable(inf : FuncInfo; vinfo : LocalVariableInfo; arg : void?;
                            inScope : bool) : void {
        intervals["{vinfo.name}"] = (open = vinfo.openPos, close = vinfo.closePos)
        spaces["{inf.name}"] = inf.spaceId
    }
}

// not run_walk: make_stack_walker binds methods from the pointer's
// STATIC class info, so the walk must start from an
// IntervalWalker-typed pointer to reach the overrides
def run_interval_walk(var w : IntervalWalker?) {
    make_stack_walker(w) $(adapter) {
        adapter |> walk_stack(this_context(), w.walkAt)
    }
}

def probe_interval(var w : IntervalWalker?) : tuple<pos : int; tagLen : int> {
    let precious = "IV_{length(w.spaces)}"    // runtime-built so it survives to the park
    let pos = frame_position()
    run_interval_walk(w)
    return (pos = pos, tagLen = length(precious))
}

// substring match - auto-inline mangles the name to _inlN_l_precious
def find_interval(w : IntervalWalker?; needle : string) : tuple<found : bool; iv : tuple<open : uint; close : uint>> {
    for (k, v in keys(w.intervals), values(w.intervals)) {
        if (find(k, needle) >= 0) {
            return (found = true, iv = v)
        }
    }
    return (found = false, iv = (open = 0u, close = 0u))
}

[test(description = "a walker reads a local's openPos/closePos interval and its frame's nonzero owner space by field")]
def test_stackwalk_interval_fields(t : T?) {
    t |> run("a walker reads the gate's interval and owner space by field") @(t : T?) {
        var w = new IntervalWalker()
        let probe = probe_interval(w)
        t |> success(probe.pos > 0, "the probe returned a stamped position")
        t |> success(probe.tagLen == 4,
                     "the probe's runtime-built local reads back at its built length")
        let pos = uint(probe.pos)
        let hit = find_interval(w, "precious")
        t |> success(hit.found, "the local 'precious' reports its interval")
        t |> success(hit.iv.open < pos && pos <= hit.iv.close,
                     "a mid-scope call site sits inside (openPos, closePos]")
        var offender = ""
        for (k, v in keys(w.spaces), values(w.spaces)) {
            if (v == 0u) {
                offender = k
            }
        }
        t |> success(!empty(w.spaces) && empty(offender),
                     "a locals-bearing frame reports a zero owner space: '{offender}'")
        unsafe {
            delete w
        }
    }
}

// an invoked block literal splices through the InvokeBlock tier,
// whose manufactured scope wrapper holds the argument temps; a
// one-source-line invoke gives that wrapper a degenerate range, and
// the temps must still gate live at parks inside the body

[test(description = "the argument temp of a one-line invoked block literal is live at a park inside the body")]
def test_stackwalk_invoke_block(t : T?) {
    t |> run("arg temp of a one-line invoked block is live at a body park") @(t : T?) {
        var w = new CollectingWalker()
        let seed = length(w.frames) + 7
        let r = invoke($(blkarg : string) : int { let n = walk_len(w, blkarg); return n + 1; },
                       "arg_{seed}")
        t |> equal(r, length("arg_7") + 1)
        t |> success(has_live_local(w, "blkarg"),
                     "the block argument temp must be live in the walk")
        unsafe {
            delete w
        }
    }
}

// a user-written ONE-LINE braced block is a degenerate source range
// straight from the parser - no compiler transform involved; its
// locals must still gate live at parks inside it

[test(description = "a local of a user-written one-line braced block is live at a park inside it")]
def test_stackwalk_oneline_block(t : T?) {
    t |> run("local of a one-line user block is live at a park inside it") @(t : T?) {
        var w = new CollectingWalker()
        var r = 0
        let cond = empty(w.frames)
        if (cond) { let xlocal = "X_{length(w.locals)}"; r = walk_len(w, xlocal); }
        t |> equal(r, length("X_0"))
        t |> success(has_live_local(w, "xlocal"), "xlocal must be live in the walk")
        unsafe {
            delete w
        }
    }
}

// spliced sibling arms share the call-site line, and sibling scopes
// REUSE stack offsets - a let from the arm that never ran must NOT
// gate live at a park in the other arm (its slot holds the other
// arm's value: a type-confused walk), while the running arm's own let
// must. strings are runtime-built: a const-foldable single-use let
// gets legally forwarded past the park, and then there is nothing to
// walk

[inline]
def pick_arm(var w : CollectingWalker?; flag : bool) : int {
    if (flag) {
        let arm_a_local = "ARM_A_{length(w.frames)}"
        run_walk(w)
        return length(arm_a_local)
    } else {
        let arm_b_local = "ARM_B_{length(w.frames)}"
        run_walk(w)
        return length(arm_b_local)
    }
}

[test(description = "of two spliced sibling arms only the executing arm's let gates live at a park, never the arm that did not run")]
def test_stackwalk_sibling_arms(t : T?) {
    t |> run("only the executing else-arm's let is live at its park") @(t : T?) {
        var w = new CollectingWalker()
        let r = pick_arm(w, !empty(w.frames))
        t |> equal(r, length("ARM_B_0"))
        t |> success(has_live_local(w, "arm_b_local"), "the executing arm's let must be live")
        t |> success(!has_live_local(w, "arm_a_local"), "the never-run arm's let must not be live")
        unsafe {
            delete w
        }
    }
    t |> run("only the executing then-arm's let is live at its park") @(t : T?) {
        var w = new CollectingWalker()
        let r = pick_arm(w, empty(w.frames))
        t |> equal(r, length("ARM_A_0"))
        t |> success(has_live_local(w, "arm_a_local"), "the executing arm's let must be live")
        t |> success(!has_live_local(w, "arm_b_local"), "the never-run arm's let must not be live")
        unsafe {
            delete w
        }
    }
}

// the inliner's return rewrites move the statements after an early
// return into a manufactured tail block; locals declared in that tail
// must still gate live at parks inside it - in spliced bodies
// (ReturnStoreRewrite) and in the function's own frame
// (ReturnCanonicalization mutates candidate bodies whether or not
// they ever splice). strings are runtime-built throughout: a
// const-foldable single-use let gets legally forwarded past the park,
// and then there is nothing to walk

[inline]
def scan_then_walk(var w : CollectingWalker?; xs : array<int>; needle : int) : int {
    for (x in xs) {
        if (x == needle) {
            return x
        }
    }
    let tail_local = "TAIL_{needle}"
    run_walk(w)
    return length(tail_local)
}

// no [inline]: small and loop-free grants the canonicalization
// pre-pass candidacy on its own, and a plain function's address can
// be taken for the unspliced-call arm
def guard_then_walk(var w : CollectingWalker?; flag : bool) : int {
    if (flag) {
        return 0
    }
    let precious = "CANON_{length(w.frames) + 42}"
    run_walk(w)
    return length(precious)
}

[test(description = "a local declared in the tail block after an early return is live at a park, in a spliced body and in the function's own frame")]
def test_stackwalk_tail_split(t : T?) {
    t |> run("tail local after an in-loop return is live at a spliced park") @(t : T?) {
        var w = new CollectingWalker()
        var xs <- [1, 2, 3]
        let needle = 96 + length(xs)    // runtime value - the let must survive to the park
        let r = scan_then_walk(w, xs, needle)
        t |> equal(r, length("TAIL_99"))
        t |> success(has_live_local(w, "tail_local"), "tail_local must be live in the walk")
        unsafe {
            delete w
        }
    }
    t |> run("tail local after a canonicalized early return is live in the OWN frame") @(t : T?) {
        var w = new CollectingWalker()
        // a function-pointer call keeps a real frame; the body was still
        // permanently canonicalized by the pre-pass, so the hole lives in the
        // un-spliced function
        let fp = @@guard_then_walk
        let r = invoke(fp, w, !empty(w.frames))
        t |> equal(r, length("CANON_42"))
        t |> success(has_frame(w, "guard_then_walk"), "guard_then_walk keeps its frame")
        t |> success(has_live_local(w, "precious"), "precious must be live in the walk")
        unsafe {
            delete w
        }
    }
}
