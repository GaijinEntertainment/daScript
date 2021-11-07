
require dastest/testing_boost


[test]
def tassert()
    assert(1 + 2 == 3)


[test]
def sum_test(t: T?)
    var sum = 0
    for i in range(100000)
        sum += i
    // print("inside `cool test` 2\n")
    // var a: int?
    // *a = 1
    equal(t, sum, 100000 * (100000 - 1) / 2)
    t->log("used time {t->getTimeSec()}")


[test(name="sub tests")]
def sub(t: T?)
    // warmup
    t->run("sub1") <| @@(tt:T?)
        tt->log("hello sub1")
    // tear down


[test]
def skipped(t: T?)
    t->skip("skip {t.name} now")
    t->error("not implemented")


[test]
def t1(t: testing::T?)
    t->log("test name {t.name}")
    equal(t, "{1}", "1")
    equal(t, "{2}", "2")

    t->log("test {t.name} was {t.failed ? "failed" : "success"}")

