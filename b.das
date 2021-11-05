
require dastest/testing_boost

class A {}

[test(name="cool test")]
def t2()
    print("inside `cool test`\n")
    var sum = 0
    for i in range(100000)
        sum += i
    // unsafe
    //     var a: A
    //     debug(a)
    // print("inside `cool test` 2\n")
    // var a: int?
    // *a = 1
    assert(sum == 100000 * (100000 - 1) / 2)
