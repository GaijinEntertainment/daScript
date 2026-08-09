options gen2

// Tutorial 20 — Template struct/class instances (reification)
//
// `[template_struct_instance]` from daslib/typemacro_boost stamps a
// `class template` (or `struct template`) into ordinary concrete classes.
// An instance is just a class that inherits from the template: its
// `typedef`s pick the types, its `override`s pick the constants, and its
// own methods replace the template's.
//
// Five patterns are demonstrated:
//   1. Type parameter     — typedef KT = float
//   2. Constant parameter — override KEEP_LATEST = true, folded by static_if
//   3. Method override    — the instance's own method wins
//   4. Call parameter     — override dot_i = @@dot_i_scaled redirects a call
//   5. Gated field        — @template_gate = WEIGHTED: the field exists only
//                           in stamps that turn the axis on
//
// Run: daslang tutorials/macros/20_template_struct_instance.das

require template_struct_instance_mod // nolint:STYLE029,LINT019 — TopKeeperT comes from here; the reifier erases the parent link before STYLE029 (MCP rail) can see it

// --- 1. Type parameter ---
// `typedef KT = float` binds the template's type parameter. Everything else
// comes from the template unchanged: KEEP_LATEST stays false, so feed()
// keeps the maximum.

class FloatTop : TopKeeperT {
    typedef KT = float
}

// --- 2. Constant parameter ---
// `override KEEP_LATEST = true` replaces the constant for this instance
// only. The `static_if` inside feed() folds at compile time, so this class
// carries no branch at runtime — just the "keep the latest value" body.

class IntLatest : TopKeeperT {
    typedef KT = int
    override KEEP_LATEST = true
}

// --- 3. Method override ---
// An instance may replace any template method with `def override`. The
// template's feed() still calls the replacement, exactly like a virtual
// call in plain class inheritance — except here it is resolved at stamp
// time, with no virtual dispatch left in the stamped class.

class IntShouty : TopKeeperT {
    typedef KT = int

    def override report : string {
        return "BEST {best} OF {n}!"
    }
}

// --- 4. Call parameter ---
// MixT's feed() calls dot_i(). MixPlain keeps that call on the real dot_i;
// MixLoud redirects every dot_i call — and every @@dot_i address — to
// dot_i_scaled. The redirect happens at stamp time, so the stamped class
// makes a plain direct call: no function pointer, nothing to inline through.

class MixPlain : MixT {
}

class MixLoud : MixT {
    override dot_i = @@dot_i_scaled
}

// --- 5. Gated field ---
// SumT's `w` carries `@template_gate = WEIGHTED`. PlainSum never has the
// field — not zeroed, not hidden: erased, so it costs nothing and cannot be
// misused. WeightedSum has it like any ordinary field. The body's static_if
// picks the matching arm, and the dead arm may still name the erased field.
// static_if arms ONLY: a `COND ? a : b` ternary is inferred on BOTH arms, so
// a dead ternary arm naming the erased field is a compile error.

class PlainSum : SumT {
}

class WeightedSum : SumT {
    override WEIGHTED = true
}

[export]
def main {
    unsafe {
        var inscope ftop <- new FloatTop()
        ftop->feed(1.5)
        ftop->feed(3.25)
        ftop->feed(2.0)
        print("FloatTop:  {ftop->report()}\n")

        var inscope latest <- new IntLatest()
        latest->feed(10)
        latest->feed(3)
        latest->feed(7)
        print("IntLatest: {latest->report()}\n")

        var inscope shouty <- new IntShouty()
        shouty->feed(10)
        shouty->feed(3)
        shouty->feed(7)
        print("IntShouty: {shouty->report()}\n")

        var inscope plain <- new MixPlain()
        var inscope loud <- new MixLoud()
        plain->feed(2, 3)
        loud->feed(2, 3)
        print("MixPlain:  acc={plain.acc}\n")
        print("MixLoud:   acc={loud.acc}\n")

        var inscope psum <- new PlainSum()
        var inscope wsum <- new WeightedSum()
        wsum.w = 10 // only WeightedSum has the field at all
        psum->add(2)
        psum->add(3)
        wsum->add(2)
        wsum->add(3)
        print("PlainSum:    total={psum.total}\n")
        print("WeightedSum: total={wsum.total}\n")
    }
}

// Expected output:
//   FloatTop:  kept 3.25 out of 3
//   IntLatest: kept 7 out of 3
//   IntShouty: BEST 10 OF 3!
//   MixPlain:  acc=6
//   MixLoud:   acc=600
//   PlainSum:    total=5
//   WeightedSum: total=50
