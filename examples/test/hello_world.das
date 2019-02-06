options log = true// , optimize = false // , logInferPasses = true, logOptimizationPasses = true

[sideeffects]
def test_if_finally
    let t : int[10]
    let count = 0
    let inin = 0
    for x in range(0,10)
        t[x] = x+1
        if x==5
            break
        finally
            t[9] = 13
            count ++
    assert(count==1 & t[9]==13)
    return true

[sideeffects]
def test_for_finally
    let t : int[10]
    let count = 0
    for x in range(0,10)
        t[x] = x+1
        if x==5
            break
    finally
        t[9] = 13
        count ++
    assert(count==1 & t[9]==13)
    return true

[sideeffects]
def test_while_finally
    let t : int[10]
    let count = 0
    let x = 0
    while x != 10
        t[x] = x+1
        if x==5
            break
        ++x
    finally
        t[9] = 13
        count ++
    assert(count==1 & t[9]==13)
    return true

let
    glob = 0
    glob_count = 0

def func_finally
    return 12
finally
    glob = 13
    glob_count ++

def test_func_finally()
    glob = 0
    glob_count = 0
    let t = func_finally()
    assert(t==12 & glob==13 & glob_count==1)

[sideeffects]
def test_block_finally()
    let t = 0
    invoke() <| $() { t=1; } finally { t=2; }
    assert(t==2)

[export]
def test
    test_block_finally()
    test_for_finally()
    test_if_finally()
    test_while_finally()
    test_func_finally()
    return true
