options gen2

require dastest/testing_boost public

// Per-iteration finally in generator loops.
// Generators linearize loops to label/goto form (ast_generate.cpp: replaceGeneratorWhile /
// replaceGeneratorFor). Finally lowered to the iter_end / mid_loop label so yield-from-finally
// fires interleaved with body yields instead of once after the loop.

[sideeffects]
def checkSeq(t : T?; var gen : iterator<int>; expected : array<int>) {
    var i = 0
    for (x in gen) {
        if (i >= length(expected)) {
            t |> failure("output too long")
            return
        }
        t |> equal(x, expected[i])
        i ++
    }
    t |> equal(i, length(expected))
}

[test(description = "for and while loops inside generators run their finally blocks through plain, break, continue, return, empty and nested cases")]
def test_generator_loops(t : T?) { // nolint:STYLE038 one flat arm per generator loop/finally form under test
    t |> run("for plain finally") @(t : T?) {
        var gen <- generator<int> {
            for (i in range(3)) {
                yield i
            } finally {
                yield 99
            }
            return false
        }
        // body and finally interleaved per iteration
        checkSeq(t, gen, [0, 99, 1, 99, 2, 99])
    }
    t |> run("for finally break") @(t : T?) {
        var gen <- generator<int> {
            for (i in range(10)) {
                yield i
                if (i == 1) {
                    break
                }
            } finally {
                yield 99
            }
            return false
        }
        // iter 0 body/finally, iter 1 body, break (finally still runs for this iter), exit
        checkSeq(t, gen, [0, 99, 1, 99])
    }
    t |> run("for finally continue") @(t : T?) {
        var gen <- generator<int> {
            for (i in range(4)) {
                if (i == 1) {
                    continue
                }
                yield i
            } finally {
                yield 99
            }
            return false
        }
        // i=0 yield 0, finally 99
        // i=1 continue, finally 99
        // i=2 yield 2, finally 99
        // i=3 yield 3, finally 99
        checkSeq(t, gen, [0, 99, 99, 2, 99, 3, 99])
    }
    t |> run("for finally empty") @(t : T?) {
        var gen <- generator<int> {
            for (i in range(0)) {
                yield i
            } finally {
                yield 99
            }
            return false
        }
        checkSeq(t, gen, [])
    }
    t |> run("while plain finally") @(t : T?) {
        var gen <- generator<int> {
            var i = 0
            while (i < 3) {
                yield i
                i ++
            } finally {
                yield 99
            }
            return false
        }
        checkSeq(t, gen, [0, 99, 1, 99, 2, 99])
    }
    t |> run("while finally break") @(t : T?) {
        var gen <- generator<int> {
            var i = 0
            while (i < 100) {
                yield i
                i ++
                if (i == 2) {
                    break
                }
            } finally {
                yield 99
            }
            return false
        }
        checkSeq(t, gen, [0, 99, 1, 99])
    }
    t |> run("while finally empty") @(t : T?) {
        var gen <- generator<int> {
            while (false) {
                yield 1
            } finally {
                yield 99
            }
            return false
        }
        checkSeq(t, gen, [])
    }
    t |> run("nested for finally") @(t : T?) {
        var gen <- generator<int> {
            for (y in range(2)) {
                for (x in range(2)) {
                    yield y * 10 + x
                } finally {
                    yield 77  // inner finally per inner iter
                }
            } finally {
                yield 99  // outer finally per outer iter
            }
            return false
        }
        // y=0:
        //   x=0 yield 0, inner finally 77
        //   x=1 yield 1, inner finally 77
        //   outer finally 99
        // y=1:
        //   x=0 yield 10, inner finally 77
        //   x=1 yield 11, inner finally 77
        //   outer finally 99
        checkSeq(t, gen, [0, 77, 1, 77, 99, 10, 77, 11, 77, 99])
    }
    t |> run("for finally return") @(t : T?) {
        var gen <- generator<int> {
            for (i in range(5)) {
                yield i
                if (i == 1) {
                    return false   // early generator end — iter 1's finally must fire first
                }
            } finally {
                yield 99
            }
            return false
        }
        // iter 0: yield 0, finally 99
        // iter 1: yield 1, return triggers finally (yield 99), then generator ends
        checkSeq(t, gen, [0, 99, 1, 99])
    }
    t |> run("while finally return") @(t : T?) {
        var gen <- generator<int> {
            var i = 0
            while (i < 10) {
                yield i
                if (i == 2) {
                    return false
                }
                i ++
            } finally {
                yield 99
            }
            return false
        }
        // iter i=0: yield 0, finally 99
        // iter i=1: yield 1, finally 99
        // iter i=2: yield 2, return triggers finally (yield 99), then generator ends
        checkSeq(t, gen, [0, 99, 1, 99, 2, 99])
    }
    t |> run("for finally return nested") @(t : T?) {
        var gen <- generator<int> {
            for (y in range(3)) {
                for (x in range(3)) {
                    yield y * 10 + x
                    if (x == 1 && y == 1) {
                        return false
                    }
                } finally {
                    yield 77   // inner finally must fire per inner iter + on return
                }
            } finally {
                yield 99   // outer finally must fire on the return path too
            }
            return false
        }
        // y=0: x=0 yield 0, 77; x=1 yield 1, 77; x=2 yield 2, 77; outer 99
        // y=1: x=0 yield 10, 77; x=1 yield 11, return -> inner 77, outer 99, generator ends
        checkSeq(t, gen, [0, 77, 1, 77, 2, 77, 99, 10, 77, 11, 77, 99])
    }
}

// Per-iteration finally correctness for nested loops of mixed kinds.

[test(description = "nested for and while loops run the right number of iterations and an inner break leaves the outer loop running")]
def test_nested(t : T?) { // nolint:STYLE038 one flat arm per nested loop-kind pairing under test
    t |> run("for in for") @(t : T?) {
        var outer = 0
        var inner = 0
        for (_y in range(4)) {
            for (_x in range(3)) {
            } finally {
                inner ++
            }
        } finally {
            outer ++
        }
        t |> equal(outer, 4)
        t |> equal(inner, 4 * 3)
    }
    t |> run("while in for") @(t : T?) {
        var outer = 0
        var inner = 0
        for (_y in range(3)) {
            var x = 0
            while (x < 5) {
                x ++
            } finally {
                inner ++
            }
        } finally {
            outer ++
        }
        t |> equal(outer, 3)
        t |> equal(inner, 3 * 5)
    }
    t |> run("for in while") @(t : T?) {
        var y = 0
        var outer = 0
        var inner = 0
        while (y < 3) {
            for (_x in range(4)) {
            } finally {
                inner ++
            }
            y ++
        } finally {
            outer ++
        }
        t |> equal(outer, 3)
        t |> equal(inner, 3 * 4)
    }
    t |> run("triple nested") @(t : T?) {
        var outer = 0
        var mid = 0
        var inner = 0
        for (_z in range(2)) {
            for (_y in range(3)) {
                for (_x in range(4)) {
                } finally {
                    inner ++
                }
            } finally {
                mid ++
            }
        } finally {
            outer ++
        }
        t |> equal(outer, 2)
        t |> equal(mid, 2 * 3)
        t |> equal(inner, 2 * 3 * 4)
    }
    t |> run("inner break preserves outer") @(t : T?) {
        var outer = 0
        var inner = 0
        for (_y in range(3)) {
            for (x in range(10)) {
                if (x == 2) {
                    break
                }
            } finally {
                inner ++
            }
        } finally {
            outer ++
        }
        // inner break at x==2: 3 finally fires per outer iter; outer runs full 3 times
        t |> equal(outer, 3)
        t |> equal(inner, 3 * 3)
    }
    t |> run("outer break after inner finishes") @(t : T?) {
        var outer = 0
        var inner = 0
        for (y in range(5)) {
            for (_x in range(2)) {
            } finally {
                inner ++
            }
            if (y == 1) {
                break
            }
        } finally {
            outer ++
        }
        // outer breaks on y==1, so outer iterations 0 and 1 fire: outer finally = 2
        t |> equal(outer, 2)
        t |> equal(inner, 2 * 2)
    }
}

// Per-iteration finally on while loops.

[test(description = "a while loop runs its finally block per iteration through plain, break, continue, return, nested, inscope and never-taken cases")]
def test_while(t : T?) { // nolint:STYLE038 one flat arm per while-loop form under test
    t |> run("plain") @(t : T?) {
        var i = 0
        var count = 0
        while (i < 5) {
            i ++
        } finally {
            count ++
        }
        t |> equal(count, 5)
    }
    t |> run("break") @(t : T?) {
        var i = 0
        var count = 0
        while (i < 10) {
            if (i == 3) {
                break
            }
            i ++
        } finally {
            count ++
        }
        // iterations i=0,1,2,3 each fire finally (break runs after i==3's finally)
        t |> equal(count, 4)
    }
    t |> run("continue") @(t : T?) {
        var i = 0
        var count = 0
        var hits = 0
        while (i < 6) {
            i ++
            if (i % 2 == 0) {
                continue
            }
            hits ++
        } finally {
            count ++
        }
        t |> equal(count, 6)
        t |> equal(hits, 3)
    }
    t |> run("return") @(t : T?) {
        t |> equal(while_return(), 2)
    }
    t |> run("nested") @(t : T?) {
        var outer_count = 0
        var inner_count = 0
        var y = 0
        while (y < 3) {
            var x = 0
            while (x < 4) {
                x ++
            } finally {
                inner_count ++
            }
            y ++
        } finally {
            outer_count ++
        }
        t |> equal(outer_count, 3)
        t |> equal(inner_count, 12)
    }
    t |> run("inscope") @(t : T?) {
        var i = 0
        while (i < 5) {
            var inscope tmp : array<int>
            tmp |> push(i)
            t |> equal(tmp[0], i)
            i ++
        }
    }
    t |> run("never taken") @(t : T?) {
        var count = 0
        while (false) {
        } finally {
            count ++
        }
        // condition false from the start: body never enters, finally never runs
        t |> equal(count, 0)
    }
}

def while_return() : int {
    var i = 0
    var count = 0
    while (i < 10) {
        if (i == 2) {
            return count
        }
        i ++
    } finally {
        count ++
    }
    return -1
}
