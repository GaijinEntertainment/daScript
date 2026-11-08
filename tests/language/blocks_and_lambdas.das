options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public

def bi_twice(b : block) {
    invoke(b)
    invoke(b)
}

def bi_twice_good(b : block<int>) : int {
    var tf = 0
    tf += invoke(b)
    tf += invoke(b)
    return tf
}

[sideeffects]
def bi_crapOnStack() {
    var a : int[20]
    for (b in a) {
        b = 13
    }
}

def bi_func1(b : block) {
    bi_crapOnStack()
    invoke(b)
    bi_crapOnStack()
}

def bi_func2(b : block) {
    bi_crapOnStack()
    bi_func3(b)
    bi_crapOnStack()
}

def bi_func3(b : block) {
    bi_crapOnStack()
    invoke(b)
    bi_crapOnStack()
}

def bi_func4(b : block) {
    bi_crapOnStack()
    invoke(b)
    bi_crapOnStack()
}

def bi_badd(ext : int; b : block<(arg : int) : int>) : int => invoke(b, ext)

def bi_radd(var ext : int&; b : block<(var arg : int&) : int>) : int => invoke(b, ext)

[test(description = "a block invokes as a variable, repeatedly, nested inside other blocks over a dirtied stack, and with value, nested and by-reference arguments")]
def test_block_invoke(t : T?) {
    t |> run("block variable invoke") @(t : T?) {
        var blk = $(a, b : int) {
            return a + b
        }
        t |> equal(invoke(blk, 1, 2), 3)
        t |> equal(invoke(blk, 3, 4), 7)
    }
    t |> run("twice and twice_good") @(t : T?) {
        var a = 0
        var b = 0
        bi_twice() ${ a++; }
        t |> equal(a, 2)
        b = bi_twice_good() ${ return a++; }
        t |> equal(a, 4)
        t |> equal(b, 2 + 3)
        b = bi_twice_good() {
            return a++
        }
        t |> equal(b, 4 + 5)
        b = bi_twice_good(${ return ++a; })
        t |> equal(a, 8)
        t |> equal(b, 7 + 8)
    }
    t |> run("nested block chaining") @(t : T?) {
        var x = 0
        var y = 0
        bi_func1() {
            x++
            y += 2
            bi_func2() {
                x++
                y += 3
            }
            bi_func4() {
                x++
                y += 4
            }
        }
        t |> equal(x, 3)
        t |> equal(y, 9)
    }
    t |> run("badd with value capture") @(t : T?) {
        var v1 = 1
        var res = bi_badd(1000, $(a1 : int) : int { return v1++ + a1; })
        t |> equal(res, 1001)
        res = bi_badd(1000) $(a1 : int) : int { return v1 + a1; }
        t |> equal(res, 1002)
        v1 = 3
        res = bi_badd(1000) $(a1 : int) : int {
            return v1 + a1
        }
        t |> equal(res, 1003)
    }
    t |> run("nested badd") @(t : T?) {
        let v1 = 4
        let res = bi_badd(1000) $(a2 : int) : int {
            return bi_badd(2000, $(a3 : int) : int { return a3 + a2 + v1; })
        }
        t |> equal(res, 3004)
    }
    t |> run("radd with ref") @(t : T?) {
        var v1 = 3
        let res = bi_radd(v1) $(var a : int&) : int {
            return a++
        }
        t |> equal(res, 3)
        t |> equal(v1, 4)
    }
}

[test(description = "a block held in a local variable returns its result for each invoke")]
def test_block(t : T?) {
    var blk = $(a, b : int) {
        return a + b
    }
    t |> equal(3, invoke(blk, 1, 2))
    t |> equal(7, invoke(blk, 3, 4))
}

def addValue(a, blk) => invoke(blk, a)

[test(description = "a block nested inside a lambda reads the lambda's captured variable")]
def test_block_access_function_arg(t : T?) {
    t |> run("nested block accesses outer lambda var") @(t : T?) {
        var CNT = 0     // nolint:LINT003 - `let` makes the lambda's captured copy const, breaking CNT++
        let counter <- @(extra : int) : int {
            let e1 = addValue(1) $(extraBlock : int) : int {
                return extraBlock + CNT
            }
            return CNT++ + e1
        }
        t |> equal(invoke(counter, 0), 1)
    }
}

def passthrough(a : int; blk : block<(b : int) : void>) {
    invoke(blk, a)
}

def passthroughref(a : int&; blk : block<(b : int&) : void>) {
    invoke(blk, a)
}

def passthroughptr(a : int?; blk : block<(b : int?) : void>) {
    invoke(blk, a)
}

struct BlockFoo {
    v : int
}

def passthroughFoo(a : BlockFoo; blk : block<(b : BlockFoo) : void>) {
    invoke(blk, a)
}

[test(description = "three nested blocks each still see every outer block's argument, by value, by reference, by pointer and as a struct")]
def test_block_args_nested(t : T?) {
    t |> run("nested int blocks") @(t : T?) {
        passthrough(1) $(a) {
            t |> equal(a, 1)
            passthrough(2) $(b) {
                t |> equal(a, 1)
                t |> equal(b, 2)
                passthrough(3) $(c) {
                    t |> equal(a, 1)
                    t |> equal(b, 2)
                    t |> equal(c, 3)
                }
            }
        }
    }
    t |> run("nested ref blocks") @(t : T?) {
        let A = 1
        let B = 2
        let C = 3
        passthroughref(A) $(a) {
            t |> equal(a, 1)
            passthroughref(B) $(b) {
                t |> equal(a, 1)
                t |> equal(b, 2)
                passthroughref(C) $(c) {
                    t |> equal(a, 1)
                    t |> equal(b, 2)
                    t |> equal(c, 3)
                }
            }
        }
    }
    t |> run("nested pointer blocks") @(t : T?) {
        var A = 1
        var B = 2
        var C = 3
        unsafe {
            passthroughptr(addr(A)) $(a) {
                t |> equal(*a, 1)
                passthroughptr(addr(B)) $(b) {
                    t |> equal(*a, 1)
                    t |> equal(*b, 2)
                    passthroughptr(addr(C)) $(c) {
                        t |> equal(*a, 1)
                        t |> equal(*b, 2)
                        t |> equal(*c, 3)
                    }
                }
            }
        }
    }
    t |> run("nested struct blocks") @(t : T?) {
        passthroughFoo(BlockFoo(v = 1)) $(a) {
            t |> equal(a.v, 1)
            passthroughFoo(BlockFoo(v = 2)) $(b) {
                t |> equal(a.v, 1)
                t |> equal(b.v, 2)
                passthroughFoo(BlockFoo(v = 3)) $(c) {
                    t |> equal(a.v, 1)
                    t |> equal(b.v, 2)
                    t |> equal(c.v, 3)
                }
            }
        }
    }
}

struct BlockVarFoo {
    a : int
}

var g_block_var = 0

[test(description = "a local block variable with or without arguments and with a void, value or struct result reads its captured local when invoked")]
def test_block_variable(t : T?) {
    t |> run("void no-arg block") @(t : T?) {
        let localVar = BlockVarFoo(a = 34)
        var localVoidNoArgBlock = $ {
            g_block_var = localVar.a
        }
        g_block_var = 0
        localVoidNoArgBlock |> invoke()
        t |> equal(g_block_var, localVar.a)
    }
    t |> run("void with-arg block") @(t : T?) {
        let localVar = BlockVarFoo(a = 34)
        var localVoidArgBlock = $(x : int) : void {
            g_block_var = localVar.a + x
        }
        g_block_var = 0
        localVoidArgBlock |> invoke(1)
        t |> equal(g_block_var, localVar.a + 1)
    }
    t |> run("result no-arg block") @(t : T?) {
        let localVar = BlockVarFoo(a = 34)
        let localResultNoArgBlock = $ {
            g_block_var = localVar.a
            return localVar.a
        }
        g_block_var = 0
        let res1 = localResultNoArgBlock |> invoke()
        t |> equal(g_block_var, localVar.a)
        t |> equal(res1, localVar.a)
    }
    t |> run("result with-arg block") @(t : T?) {
        let localVar = BlockVarFoo(a = 34)
        let localResultArgBlock = $(x : int) : int {
            g_block_var = localVar.a + x
            return localVar.a + x
        }
        g_block_var = 0
        let res2 = localResultArgBlock |> invoke(3)
        t |> equal(g_block_var, localVar.a + 3)
        t |> equal(res2, localVar.a + 3)
    }
    t |> run("cmres result no-arg block") @(t : T?) {
        let localVar = BlockVarFoo(a = 34)
        let localCmresResultNoArgBlock = $ {
            g_block_var = localVar.a
            return localVar
        }
        g_block_var = 0
        let res3 = localCmresResultNoArgBlock |> invoke()
        t |> equal(g_block_var, localVar.a)
        t |> equal(res3.a, localVar.a)
    }
    t |> run("cmres result with-arg block") @(t : T?) {
        let localVar = BlockVarFoo(a = 34)
        let localCmresResultArgBlock = $(x : int) : BlockVarFoo {
            g_block_var = localVar.a + x
            return BlockVarFoo(a = localVar.a + x)
        }
        g_block_var = 0
        let res4 = localCmresResultArgBlock |> invoke(3)
        t |> equal(g_block_var, localVar.a + 3)
        t |> equal(res4.a, localVar.a + 3)
    }
}

struct BvlFoo {
    a : int
}

var g_bvl = 0

def radd_bvl(var ext : int; b : block<(arg : int)>) {
    invoke(b, ext)
}

[test(description = "a block passed at the call site and the same block held in a local variable produce the same result")]
def test_block_vs_local_block(t : T?) {
    t |> run("block via pipe vs local block variable") @(t : T?) {
        let localVar = BvlFoo(a = 34)
        var localBlock = $(arg : int) {
            g_bvl = localVar.a + arg
        }
        g_bvl = 0
        radd_bvl(100) $(arg : int) {
            g_bvl = localVar.a + arg
        }
        t |> equal(g_bvl, 134)
        g_bvl = 0
        invoke(localBlock, 100)
        t |> equal(g_bvl, 134)
    }
}

def apply_to_7(a : block<(x : int) : int>) : int => a(7)

def build_with_7(a : block<(x : int) : array<int>>) : array<int> => <- a(7)

[test(description = "a call-site block takes an arrow body in value and move form, beside the braced and <| piped forms")]
def test_call_site_block_arrow_body(t : T?) {
    t |> run("call-site block arrow value form") @(t : T?) {
        let res = apply_to_7() $(x) => x * 2
        t |> equal(res, 14)
    }

    t |> run("call-site block arrow move form") @(t : T?) {
        let xs <- build_with_7() $(x) => <- [x, x + 1]
        t |> equal(length(xs), 2)
        t |> equal(xs[0], 7)
        t |> equal(xs[1], 8)
    }

    t |> run("braced call-site block still works") @(t : T?) {
        let res = apply_to_7() $(x) {
            return x + 5
        }
        t |> equal(res, 12)
    }

    t |> run("pipe-based block arrow still works") @(t : T?) {
        let res = apply_to_7() <| $(x) => x - 3
        t |> equal(res, 4)
    }
}

[sideeffects]
def onlyNull(bar : lambda<(a : int) : int>) {
    if (bar != null) {
        panic("has to be null")
    }
}

def addX(a : int) {
    return @(b : int) {
        return a + b
    }
}

[test(description = "a lambda captures by copy so later writes to the outer variable do not reach it, a default lambda is null, and a returned lambda keeps its capture")]
def test_lambda(t : T?) {
    t |> run("capture and invoke") @(t : T?) {
        var CNT = 0
        let counter <- @(extra : int) : int {
            return CNT++ + extra
        }
        let x = invoke(counter, 13)
        CNT = 100500
        let y = invoke(counter, 13)
        t |> equal(x, 13)
        t |> equal(y, 14)
    }
    t |> run("null lambda check") @(t : T?) {
        let l : lambda<(a : int) : int>
        onlyNull(l)
        t |> success(true)
    }
    t |> run("addX returns lambda") @(t : T?) {
        let tt <- addX(1)
        let q = invoke(tt, 2)
        t |> equal(q, 3)
    }
}

struct LcmFoo {
    dummy : int
}

var g_lcm_counter = 0

[unused_argument(f)]
def operator delete(f : LcmFoo) {
    g_lcm_counter++
}

[test(description = "capture(& x) shares the outer variable, capture(<- x) moves it out and capture(:= x) clones it, and deleting the lambda finalizes the moved capture once")]
def test_lambda_capture_modes(t : T?) {
    t |> run("capture ref, move, clone") @(t : T?) {
        var a1 <- array<int>(1, 2)
        var a2 <- array<int>(1, 2)
        var a3 <- array<int>(1, 2)
        unsafe {
            let lam <- @ capture(& a1, <- a2, := a3) {
                push(a1, 1)
                push(a2, 1)
                push(a3, 1)
            }
            invoke(lam)
        }
        t |> equal(length(a1), 3)
        t |> equal(length(a2), 0)
        t |> equal(length(a3), 2)
    }
    t |> run("capture move and clone with delete") @(t : T?) {
        g_lcm_counter = 0
        var pA = new LcmFoo
        var pB = new LcmFoo
        var C = LcmFoo() // nolint:LINT003 -- captured by clone; const-let changes capture semantics
        var lam <- @ capture(<- pB, := pC) {
            assert(pA.dummy == 0)
            assert(pB.dummy == 0)
            assert(C.dummy == 0)
        }
        t |> equal(pB == null, true)
        invoke(lam)
        unsafe { delete lam; }
        t |> equal(g_lcm_counter, 1)
    }
}

// a lambda is a copyable, pointer-shaped value: '=' aliases the
// capture frame, and deleting one is unsafe
typedef Counter = lambda<() : int>

def invoke_twice(g : Counter) : int => invoke(g) + invoke(g)

[test(description = "copying a lambda aliases its capture frame, so a mutation through one copy is seen through the other")]
def test_lambda_copy_aliases_capture(t : T?) {
    t |> run("copy assignment shares capture frame") @(t : T?) {
        var counter = 0 // nolint:LINT003 -- captured by lambda, mutated inside body
        var inc <- @() : int {
            counter++      // mutates the lambda's own captured counter
            return counter
        }
        let inc2 = inc     // copy - aliases the same capture frame
        t |> equal(invoke(inc), 1)
        t |> equal(invoke(inc2), 2)    // sees inc's mutation through the alias
        t |> equal(invoke(inc), 3)     // and vice versa
        unsafe { delete inc; }
    }
}

[test(description = "a lambda passed by value invokes the same body with fresh locals on each call")]
def test_lambda_pass_by_value(t : T?) {
    t |> run("lambda passes by value (one capture frame, two refs)") @(t : T?) {
        var lam <- @() : int {
            var x = 0
            x++
            return x
        }
        let sum = invoke_twice(lam)
        t |> equal(sum, 2)             // each invoke returns 1 (independent x), sum = 2
        unsafe { delete lam; }
    }
}

struct LcHolder {
    action : Counter
}

[test(description = "copying a struct with a lambda field aliases the lambda, and both copies invoke it")]
def test_lambda_struct_field_copy(t : T?) {
    t |> run("struct copy aliases lambda field") @(t : T?) {
        var s1 = LcHolder(action <- @() : int {
            var n = 0
            n++
            return n
        })
        let s2 = s1                    // copies the struct; lambda field is aliased
        t |> equal(invoke(s1.action), 1)
        t |> equal(invoke(s2.action), 1)   // independent n per invoke
        unsafe { delete s1.action; }
    }
}

[test(description = "a lambda captured implicitly by copy inside another lambda needs no explicit capture clause and invokes from it")]
def test_lambda_implicit_capture(t : T?) {
    t |> run("inner lambda captured implicitly inside another lambda") @(t : T?) {
        var inner <- @() : int {
            var n = 0
            n++
            return n
        }
        var outer <- @() : int {
            return invoke(inner) + invoke(inner)
        }
        t |> equal(invoke(outer), 2)
        unsafe {
            delete inner
            delete outer
        }
    }
}

struct LamIterFoo {
    value : int
}

[test(description = "a lambda returning bool and writing its reference argument iterates as each(), for an int and for a struct element")]
def test_lambda_to_iter(t : T?) {
    t |> run("int ref lambda as iterator") @(t : T?) {
        var count = 0   // nolint:LINT003 - `let` makes the lambda's captured copy const, breaking count++
        let lam <- @(var a : int&) : bool {
            if (count < 10) {
                a = count++
                return true
            } else {
                return false
            }
        }
        for (x, tx in each(lam), range(10)) {
            t |> equal(x, tx)
        }
    }
    t |> run("struct lambda as iterator") @(t : T?) {
        var count = 0   // nolint:LINT003 - `let` makes the lambda's captured copy const, breaking count++
        let lamF <- @(var a : LamIterFoo) : bool {
            if (count < 10) {
                a.value = count++
                return true
            } else {
                return false
            }
        }
        for (y, ty in each(lamF), range(10)) {
            t |> equal(y.value, ty)
        }
    }
}

class CaptureRefFoo {
    i : int
    k : array<int>
    def CaptureRefFoo {
        i = 5
    }
    def kaboom : lambda<int> {
        return <- @() {
            return i++
        }
    }
}

[test(description = "a lambda made in a class method captures self by reference and building it leaves the object's fields untouched")]
def test_capture_as_ref(t : T?) {
    t |> run("class lambda captures self as ref") @(t : T?) {
        var x = new CaptureRefFoo()
        let boom <- x->kaboom()
        t |> equal(x.i, 5)
    }
}

struct MoveLamFoo {
    bar : lambda<() : array<int>>
}

def MakeBar(var arr : array<int>) {
    return @ capture(<- arr) () {
        return <- clone_to_move(arr)
    }
}

let {
    g_move_lam_foo = new MoveLamFoo(
        bar <- MakeBar([1, 2, 3])
    )
}

[test(description = "an array moved into a lambda's capture survives in a global struct field and the lambda returns a copy of it")]
def test_move_lambda_local_ref(t : T?) {
    t |> run("capture move array into lambda") @(t : T?) {
        let l <- invoke(g_move_lam_foo.bar)
        t |> equal(l[0], 1)
        t |> equal(l[1], 2)
        t |> equal(l[2], 3)
    }
}
