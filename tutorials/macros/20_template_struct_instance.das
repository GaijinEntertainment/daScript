options gen2

// Tutorial 20 — Template struct/class instances (reification)
//
// `[template_struct_instance]` from daslib/typemacro_boost stamps a
// `class template` (or `struct template`) into ordinary concrete classes.
// An instance is just a class that inherits from the template: its
// `typedef`s pick the types, its `override`s pick the constants, and its
// own methods replace the template's.
//
// Three patterns are demonstrated:
//   1. Type parameter     — typedef KT = float
//   2. Constant parameter — override KEEP_LATEST = true, folded by static_if
//   3. Method override    — the instance's own method wins
//
// Run: daslang tutorials/macros/20_template_struct_instance.das

require template_struct_instance_mod // nolint:STYLE029 — TopKeeperT comes from here; the reifier erases the parent link before lint can see it

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
    }
}

// Expected output:
//   FloatTop:  kept 3.25 out of 3
//   IntLatest: kept 7 out of 3
//   IntShouty: BEST 10 OF 3!
