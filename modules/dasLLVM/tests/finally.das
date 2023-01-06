require dastest/testing_boost

require llvm/llvm_jit
require llvm

[test]
def test_finally ( t:T? )
    t |> run("test if finally") <| @ ( t : T? )
        t |> success ( is_jit_function(@@test_if_finally) )
        t |> success ( test_if_finally() )
        t |> success ( is_jit_function(@@test_nested_if) )
        t |> success ( test_nested_if() )
        t |> success ( is_jit_function(@@test_nested_if_else) )
        t |> success ( test_nested_if_else() )
    t |> run("test for finally") <| @ ( t : T? )
        t |> success ( is_jit_function(@@test_for_finally) )
        t |> success ( test_for_finally() )
    t |> run("test while finally") <| @ ( t : T? )
        t |> success ( is_jit_function(@@test_while_finally) )
        t |> success ( test_while_finally() )
    t |> run("test func finally") <| @ ( t : T? )
        t |> success ( is_jit_function(@@test_func_finally) )
        t |> success ( test_func_finally() )
    t |> run("test block finally") <| @ ( t : T? )
        t |> success ( is_jit_function(@@test_block_finally) )
        t |> success ( test_block_finally() )

[jit, sideeffects]
def test_nested_if
    var t = 0
    for x in range(10)
        if x == 5
            if x > 4
                break
            finally
                t += 1
        finally
            t += 10
    return t==11

[jit, sideeffects]
def test_nested_if_else
    var t = 0
    var p = false
    for x in range(10)
        if x == 5
            if x < 4
                p = true
            else
                break
            finally
                t += 1
        finally
            t += 10
    return t==11 && p==false

[jit, sideeffects]
def test_if_finally
    var t : int[10]
    var count = 0
    for x in range(10)
        t[x] = x+1
        if x==5
            break
        finally
            t[9] = 13
            count ++
    return count==1 && t[9]==13

[jit, sideeffects]
def test_for_finally
    var t : int[10]
    var count = 0
    for x in range(10)
        let uno = one()
        t[x] = x+uno
        if x==5
            break
    finally
        t[9] = 13
        count += uno
    return count==1 && t[9]==13

var
    ONE = 1

[jit, sideeffects]
def one
    ONE = 1
    assert(ONE==1)
    return ONE

[jit, sideeffects]
def test_while_finally
    var t : int[10]
    var count = 0
    var x = 0
    while x != 10
        let uno = one()
        t[x] = x+uno
        if x==5
            break
        ++x
    finally
        t[9] = 12 + uno
        count ++
    return count==1 && t[9]==13

var
    glob = 0
    glob_count = 0

[jit, sideeffects]
def func_finally
    return 12
finally
    glob = 13
    glob_count ++

[jit, sideeffects]
def test_func_finally()
    glob = 0
    glob_count = 0
    let t = func_finally()
    return t==12 && glob==13 && glob_count==1

[jit, sideeffects]
def test_block_finally()
    var t = 0
    invoke() <| $() { t=1; } finally { t=2; }
    return t==2


