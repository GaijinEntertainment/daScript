
require dastest/testing_boost

class A {}

[test(name="cool test")]
def t2(t: T?)
    t->log("inside `cool test`")
    var sum = 0
    for i in range(100000)
        sum += i
    // unsafe
    //     var a: A
    //     debug(a)
    // print("inside `cool test` 2\n")
    // var a: int?
    // *a = 1
    success(t, sum == 100000 * (100000 - 1) / 2)

[test(name="sub tests")]
def sub(t: T?)
    // before
    t->run("sub1") <| @@(tt:T?)
        tt->log("hello sub1")
    // after
