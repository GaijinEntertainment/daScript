
require dastest/testing_boost


[test]
def t1(t: testing::T?)
    t->log("test name {t.name}")
    equal(t, "{1}", "1")
    equal(t, "{2}", "2")

    // equal(t, "{2}", "4")
    // t->fatal("test fatal")

    // success(t, 1 == 2)
    // success(t, 1 == 2)
    // success(t, 1 == 2)
    // success(t, 1 == 2)
    // success(t, 1 == 2)
    t->log("test {t.name} was {t.failed ? "failed" : "success"}")


// [test]
// def t2(t: T?)
//     pass
//     t->skip("skip {t.name} currently")
//     t->error("not implemented")

[test]
def tassert()
    assert(1 + 2 == 3)
