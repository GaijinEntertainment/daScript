
require dastest/testing_boost


[test(name="cool test")]
def t2()
    print("inside `cool test`\n")
    var sum = 0
    for i in range(100000)
        sum += i
    // var a: int?
    // *a = 1
    assert(sum == 100000 * (100000 - 1) / 2)
