[sideeffects]
def basic_goto()
    var fail = false
    var arr : int[10]
    var i = 0
    label 0:
    arr[i] = i
    i ++
    if i < 10
        goto label 0
        fail = true
        static_assert(false,"unreachable code")
    for t in range(0,10)
        assert(arr[t]==t)
    assert(!fail)

[sideeffects]
def nested_goto()
    var fail = false
    var arr : int[10]
    var i = 0
    label 0:
    arr[i] = i
    i ++
    if i < 10
        if i < 10
            goto label 0
            fail = true
        fail = true
    for t in range(0,10)
        assert(arr[t]==t)

[sideeffects]
def for_goto()
    var fail = false
    var arr : int[10]
    for j in range(0,10)
        var i = 0
        label 0:
        arr[i] = i + j
        i ++
        if i < 10
            goto label 0
            fail = true
        for t in range(0,10)
            assert(arr[t]==t + j)
        assert(!fail)
    for t in range(0,10)
        assert(arr[t]==t + 9)
    assert(!fail)

[sideeffects]
def while_goto()
    var fail = false
    var arr : int[10]
    var j = 0
    while j < 10
        var i = 0
        label 0:
        arr[i] = i + j
        i ++
        if i < 10
            goto label 0
            fail = true
        for t in range(0,10)
            assert(arr[t]==t + j)
        assert(!fail)
        j ++
    for t in range(0,10)
        assert(arr[t]==t + 9)
    assert(!fail)

[sideeffects]
def finally_goto(yes:bool = true)
    var fail = true
    if yes
        fail = true
        goto label 0
    finally
        fail = false
    label 0:
    assert(!fail)

[export]
def test()
    basic_goto()
    nested_goto()
    for_goto()
    while_goto()
    finally_goto()
    return true
