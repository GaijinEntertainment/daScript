options gen2

// Minimal repro for https://github.com/GaijinEntertainment/daScript/issues/2830
// Multi-field `_where` predicate over `from_decs_template` triggers spurious
// `error[30151]` on the unrelated concept_assert in `decs.das`'s `set()`
// generic — under lint flags, on linux x64 only.

require daslib/decs_boost
require daslib/linq_boost
require daslib/linq_fold

[decs_template(prefix = "x_")]
struct X {
    a : int
    b : int
    c : int
}

[export]
def target() : int {
    return _fold(from_decs_template(type<X>)._where(_.a + _.b + _.c > 0).count())
}

[export]
def main {
    print("count={target()}\n")
}
