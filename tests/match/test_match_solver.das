options gen2

require dastest/testing_boost public
require daslib/match_solver
require daslib/option
require daslib/result

def private key(k : string) : SolverCell => SolverCell(keys <- [k])

def private span(lo, hi : int64) : SolverCell => SolverCell(spans <- [interval(lo, hi)])

def private one(var c : SolverCell; var guard : SolverGuard = SolverGuard(unguarded)) : SolverArm =>
    SolverArm(rows <- [[c]], guard <- guard)

def private coverage(columns : array<SolverColumn>; var arms : array<SolverArm>) : SolverCoverage {
    //! The exact backend's answer, for arms it decides.
    var res <- solve_coverage(columns, arms)
    return <- move_unwrap(res)
}

def private cmp(op : SolverCompare; a, b : int) : SolverGuard {
    let terms = (op = op, left = SolverTerm(value = a), right = SolverTerm(value = b))
    return <- SolverGuard(fact = SolverFact(node = SolverFactNode(compare = terms)))
}

def private fact_guard(var node : SolverFactNode) : SolverGuard =>
    SolverGuard(fact <- SolverFact(node <- node))

def private dir_column() : array<SolverColumn> =>
    [SolverColumn(domain <- some(["north", "south", "east", "west"]))]

[test]
def test_solve_coverage(t : T?) {
    t |> run("an arm the arms before it already cover is unreachable") @(t : T?) {
        let cov <- coverage(dir_column(),
                            [one(key("north")), SolverArm(rows <- [[key("south")], [key("east")]]),
                             one(key("west")), one(key("south"))])
        t |> equal(length(cov.unreachable), 1)
        t |> equal(cov.unreachable[0], 3)
        t |> success(cov.missing is covered)
        t |> success(cov.all_finite)
    }
    t |> run("an alternative an earlier arm takes is dead, the arm is not") @(t : T?) {
        let cov <- coverage(dir_column(),
                            [one(key("north")), one(SolverCell(keys <- ["north", "south"])),
                             one(SolverCell(any = true))])
        t |> success(empty(cov.unreachable))
        t |> equal(length(cov.dead_alternatives), 1)
        t |> equal(cov.dead_alternatives[0].arm, 1)
        t |> equal(cov.dead_alternatives[0].what, "north")
    }
    t |> run("missing values of a finite column are named, three at most") @(t : T?) {
        let few <- coverage(dir_column(), [one(key("north"))])
        t |> success(few.missing is named)
        t |> equal(length((few.missing as named).shown), 3)
        t |> success(!(few.missing as named).more)
        t |> equal((few.missing as named).text, "`south`, `east`, `west`")
        let many <- coverage([SolverColumn(domain <- some(["a", "b", "c", "d", "e"]))],
                             [one(key("a"))])
        t |> success((many.missing as named).more)
        t |> equal((many.missing as named).text, "`b`, `c`, `d` and more")
    }
    t |> run("an open column misses the values no row names") @(t : T?) {
        let cov <- coverage([SolverColumn()], [one(key("GET")), one(key("PUT"))])
        t |> success(cov.missing is unnamed)
        t |> success(!cov.all_finite)
    }
    t |> run("a range and a constant cut the integers into the same pieces") @(t : T?) {
        let cov <- coverage([SolverColumn()],
                            [one(span(0l, 10l)), one(span(5l, 6l)), one(span(10l, 100l))])
        t |> equal(length(cov.unreachable), 1)
        t |> equal(cov.unreachable[0], 1)
    }
    t |> run("several columns miss combinations") @(t : T?) {
        let bools <- [SolverColumn(domain <- some(["true", "false"])),
                      SolverColumn(domain <- some(["true", "false"]))]
        let cov <- coverage(bools,
                            [SolverArm(rows <- [[key("true"), SolverCell(any = true)]]),
                             SolverArm(rows <- [[key("false"), key("true")]])])
        t |> equal(length((cov.missing as named).shown), 1)
        assume w = (cov.missing as named).shown[0]
        t |> success(w.values[0] is named && w.values[0] as named == "false"
                     && w.values[1] as named == "false")
        t |> equal((cov.missing as named).text, "`(false, false)`")
    }
    t |> run("an arm with no rows is skipped and keeps its index") @(t : T?) {
        let cov <- coverage(dir_column(),
                            [one(key("north")), SolverArm(), one(key("north")),
                             one(SolverCell(any = true))])
        t |> equal(length(cov.unreachable), 1)
        t |> equal(cov.unreachable[0], 2)
    }
}

[test]
def test_solve_coverage_guards(t : T?) {
    t |> run("a guarded arm covers nothing") @(t : T?) {
        let cov <- coverage(dir_column(),
                            [one(key("north"), SolverGuard(opaque)), one(key("south")),
                             one(key("east")), one(key("west"))])
        t |> success(cov.has_guards)
        t |> equal((cov.missing as named).text, "`north`")
    }
    t |> run("arms asking one question cover their rows once the outcomes are complete") @(t : T?) {
        let cov <- coverage([SolverColumn()],
                            [one(SolverCell(any = true), cmp(SolverCompare.less, 0, 1)),
                             one(SolverCell(any = true), cmp(SolverCompare.equal, 0, 1)),
                             one(SolverCell(any = true), cmp(SolverCompare.greater, 0, 1))])
        t |> success(cov.missing is covered)
    }
    t |> run("the same two terms in the other order are the same question") @(t : T?) {
        let cov <- coverage([SolverColumn()],
                            [one(SolverCell(any = true), cmp(SolverCompare.less, 0, 1)),
                             one(SolverCell(any = true), cmp(SolverCompare.less_equal, 1, 0))])
        t |> success(cov.missing is covered)
    }
    t |> run("one question over different rows does not join") @(t : T?) {
        let cov <- coverage([SolverColumn(domain <- some(["a", "b"]))],
                            [one(key("b"), cmp(SolverCompare.less, 0, 1)),
                             one(key("a"), cmp(SolverCompare.greater_equal, 0, 1))])
        t |> success(cov.missing is unnamed)
    }
    t |> run("none_of negates the outcomes of one fact") @(t : T?) {
        var b <- fact_guard(SolverFactNode(truth = SolverTerm(value = 0)))
        var not_b <- fact_guard(SolverFactNode(
            none_of <- [SolverFact(node = SolverFactNode(truth = SolverTerm(value = 0)))]))
        let cov <- coverage([SolverColumn()],
                            [one(SolverCell(any = true), b), one(SolverCell(any = true), not_b)])
        t |> success(is_exhaustive(cov))
    }
    t |> run("arms asking different questions do not join their outcomes") @(t : T?) {
        let cov <- coverage([SolverColumn()],
                            [one(SolverCell(any = true), cmp(SolverCompare.less, 0, 1)),
                             one(SolverCell(any = true), cmp(SolverCompare.greater_equal, 0, 2))])
        t |> success(cov.missing is unnamed)
    }
}

def private overlap(var a, b : array<SolverCell>) : bool {
    //! The second of two arms shares a value with the first, as the
    //! solver reports it.
    let cov <- coverage([for (_c in a); SolverColumn()],
                        [SolverArm(rows <- [a]), SolverArm(rows <- [b])])
    return has_value(cov.overlaps[1], 0)
}

def private both(var a, b : SolverCell) : SolverCell => SolverCell(all_of <- [a, b])

[test]
def test_overlaps(t : T?) {
    t |> run("keys overlap when they share one") @(t : T?) {
        t |> success(overlap([SolverCell(keys <- ["a", "b"])], [SolverCell(keys <- ["b", "c"])]))
        t |> success(!overlap([key("a")], [key("b")]))
    }
    t |> run("spans overlap when they share an integer") @(t : T?) {
        t |> success(overlap([span(0l, 10l)], [span(9l, 20l)]))
        t |> success(!overlap([span(0l, 10l)], [span(10l, 20l)]))
    }
    t |> run("any and opaque may share a value with anything") @(t : T?) {
        t |> success(overlap([SolverCell(any = true)], [key("a")]))
        t |> success(overlap([SolverCell(opaque = true)], [key("a")]))
    }
    t |> run("rows overlap only when every column does") @(t : T?) {
        t |> success(overlap([key("a"), key("x")], [key("a"), SolverCell(any = true)]))
        t |> success(!overlap([key("a"), key("x")], [key("a"), key("y")]))
    }
    t |> run("an arm lists only the earlier arms it overlaps") @(t : T?) {
        let cov <- coverage([SolverColumn()],
                            [one(key("a")), one(key("b")), one(SolverCell(keys <- ["a", "c"]))])
        t |> equal(length(cov.overlaps), 3)
        t |> success(empty(cov.overlaps[0]) && empty(cov.overlaps[1]))
        t |> equal(length(cov.overlaps[2]), 1)
        t |> equal(cov.overlaps[2][0], 0)
    }
}

[test]
def test_all_of(t : T?) {
    t |> run("any is the identity") @(t : T?) {
        t |> success(overlap([both(SolverCell(any = true), key("a"))], [key("a")]))
        t |> success(!overlap([both(SolverCell(any = true), key("a"))], [key("b")]))
    }
    t |> run("keys and spans intersect as sets") @(t : T?) {
        let k = @() => both(SolverCell(keys <- ["a", "b"]), SolverCell(keys <- ["b", "c"]))
        t |> success(overlap([invoke(k)], [key("b")]))
        t |> success(!overlap([invoke(k)], [SolverCell(keys <- ["a", "c"])]))
        let sp = @() => both(span(0l, 10l), span(5l, 20l))
        t |> success(overlap([invoke(sp)], [span(9l, 10l)]))
        t |> success(!overlap([invoke(sp)], [span(0l, 5l)]))
        t |> success(!overlap([invoke(sp)], [span(10l, 20l)]))
    }
    t |> run("disjoint cells intersect to nothing, so the arm never runs") @(t : T?) {
        let cov <- coverage([SolverColumn()],
                            [one(both(key("a"), key("b"))), one(SolverCell(any = true))])
        t |> equal(length(cov.unreachable), 1)
        t |> equal(cov.unreachable[0], 0)
        t |> success(!overlap([both(key("a"), key("b"))], [SolverCell(keys <- ["a", "b"])]))
    }
    t |> run("opaque stays opaque") @(t : T?) {
        t |> success(overlap([both(SolverCell(opaque = true), key("a"))], [key("b")]))
    }
}

[test]
def test_questions(t : T?) {
    t |> run("all_reachable: no arm and no alternative is dead") @(t : T?) {
        t |> success(all_reachable(coverage(dir_column(), [one(key("north")), one(key("south"))])))
        t |> success(!all_reachable(coverage(dir_column(), [one(key("north")), one(key("north"))])))
        let dead <- coverage(dir_column(),
                             [one(key("north")), one(SolverCell(keys <- ["north", "south"]))])
        t |> success(!all_reachable(dead))
    }
    t |> run("is_reachable: one arm") @(t : T?) {
        let cov <- coverage(dir_column(), [one(key("north")), one(key("north")), one(key("south"))])
        t |> success(is_reachable(cov, 0) && !is_reachable(cov, 1) && is_reachable(cov, 2))
    }
    t |> run("is_exhaustive: every value reaches an arm") @(t : T?) {
        let full <- coverage(dir_column(), [one(key("north")), one(SolverCell(any = true))])
        t |> success(is_exhaustive(full))
        t |> success(!is_exhaustive(coverage(dir_column(), [one(key("north"))])))
    }
    t |> run("arms_overlap: either order, never an arm with itself") @(t : T?) {
        let cov <- coverage([SolverColumn()],
                            [one(key("a")), one(key("b")), one(SolverCell(keys <- ["a", "c"]))])
        t |> success(arms_overlap(cov, 0, 2) && arms_overlap(cov, 2, 0))
        t |> success(!arms_overlap(cov, 0, 1) && !arms_overlap(cov, 1, 2)
                     && !arms_overlap(cov, 0, 0))
    }
    t |> run("missing_fully_listed: the missing values are named and none is left out") @(t : T?) {
        t |> success(missing_fully_listed(coverage(dir_column(), [one(key("north"))])))
        let many <- coverage([SolverColumn(domain <- some(["a", "b", "c", "d", "e"]))],
                             [one(key("a"))])
        t |> success(!missing_fully_listed(many))
        t |> success(!missing_fully_listed(coverage([SolverColumn()], [one(key("GET"))])))
        t |> success(!missing_fully_listed(coverage(dir_column(), [one(SolverCell(any = true))])))
    }
}

def private unsupported(var node : SolverFactNode) : Result<SolverCoverage; SolverUnsupported> {
    //! A guard holding ``node`` on the second arm, the first arm plain.
    var arms <- [one(key("north")), one(key("south"), fact_guard(node))]
    return <- solve_coverage(dir_column(), arms)
}

[test]
def test_unsupported_facts(t : T?) {
    t |> run("the exact backend names the arm whose fact it can not decide") @(t : T?) {
        let truth = @() => SolverFact(node = SolverFactNode(truth = SolverTerm(value = 0)))
        var shapes <- [
            SolverFactNode(all_of <- [invoke(truth), invoke(truth)]),
            SolverFactNode(any_of <- [invoke(truth), invoke(truth)]),
            SolverFactNode(none_of <- [invoke(truth), invoke(truth)]),
            SolverFactNode(truth = SolverTerm(int_const = 1l))
        ]
        for (node in shapes) {
            let res <- unsupported(node)
            t |> success(res is err)
            if (res is err) {
                t |> equal((res as err).arm, 1)
            }
        }
    }
    t |> run("a decidable fact is no error") @(t : T?) {
        let res <- unsupported(SolverFactNode(truth = SolverTerm(value = 0)))
        t |> success(res is ok)
    }
}
