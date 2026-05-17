options gen2
require daslib/linq
require dastest/testing_boost public
require math
require strings

require daslib/linq_boost

require _common

[test]
def test_comprehension_fold(t : T?) {
    t |> run("basic comprehension") @(t : T?) {
        var t1 <- ([1, 2, 3, 4, 5].to_sequence()
            ._where(_ <= 3)
            .to_array()
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t1), "array<int>")
        for (i, v in 0..3, t1) {
            t |> equal(i + 1, v)
        }
    }

    t |> run("comprehension with select") @(t : T?) {
        var t2 <- ([1, 2, 3, 4, 5].to_sequence()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t2), "iterator<int>")
        for (i, v in 0..3, t2) {
            t |> equal((i + 1) * 2, v)
        }
    }

    t |> run("comprehension with select to array") @(t : T?) {
        var t3 <- ([1, 2, 3, 4, 5].to_sequence()
            ._select(_ * 2)
            .to_array()
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t3), "array<int>")
        for (i, v in 1..6, t3) {
            t |> equal(i * 2, v)
        }
    }

    t |> run("selct-where comprehension") @(t : T?) {
        var t4 <- ([1, 2, 3, 4, 5].to_sequence()
            ._select(_ * 2)
            ._where(_ <= 6)
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t4), "iterator<int>")
        for (i, v in 0..3, t4) {
            t |> equal((i + 1) * 2, v)
        }
    }

    t |> run("select-where from array comprehension") @(t : T?) {
        var t5 <- ([1, 2, 3, 4, 5]
            ._select(_ * 2)
            ._where(_ <= 6)
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t5), "array<int>")
        for (i, v in 0..3, t5) {
            t |> equal((i + 1) * 2, v)
        }
    }

    // returns array of ComplexType
    t |> run("ComplexType select-where comprehension") @(t : T?) {
        var t6 <- ([1, 2, 3, 4, 5].to_sequence()
            ._select(ComplexType(a = [_ * 2, _ * 3]))
            ._where(_.a[0] <= 6)
            .to_array()
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t6), "array<_common::ComplexType>")
        for (i, v in 0..3, t6) {
            t |> success(v.a.Equal([(i + 1) * 2, (i + 1) * 3]))
        }
    }
    // return iterator of ComplexType
    t |> run("ComplexType where-select comprehension") @(t : T?) {
        var t7 <- ([1, 2, 3, 4, 5].to_sequence()
            ._select(ComplexType(a = [_ * 2, _ * 3]))
            ._where(_.a[0] <= 3)
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t7), "iterator<_common::ComplexType>")
        for (i, v in 0..3, t7) {
            t |> success(v.a.Equal([(i + 1) * 2, (i + 1) * 3]))
        }
    }
}

[test]
def test_generic_fold(t : T?) {
    t |> run("default array to array") @(t : T?) {
        var t1 <- ([1, 2, 3, 4, 5]
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()
            )
        t |> equal(typeinfo typename(t1), "array<int>")
        for (i, v in 0..3, t1) {
            t |> equal((3 - i) * 2, v)
        }
    }
    t |> run("default iterator to iterator") @(t : T?) {
        var t2 <- ([1, 2, 3, 4, 5].to_sequence()
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()
            )
        t |> equal(typeinfo typename(t2), "iterator<int>")
        for (i, v in 0..3, t2) {
            t |> equal((3 - i) * 2, v)
        }
    }
    t |> run("reverse_to_array") @(t : T?) {
        var iterv <- [iterator for(x in 1..6); x]
        var t2a <- (iterv
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()
            )
        t |> equal(typeinfo typename(t2a), "iterator<int>")
        for (i, v in 0..3, t2a) {
            t |> equal((3 - i) * 2, v)
        }
    }
    t |> run("default array to iterator") @(t : T?) {
        var t3 <- ([1, 2, 3, 4, 5]
            .to_sequence()
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()
            )
        t |> equal(typeinfo typename(t3), "iterator<int>")
        for (i, v in 0..3, t3) {
            t |> equal((3 - i) * 2, v)
        }
    }
    t |> run("default iterator to array") @(t : T?) {
        var t4 <- ([1, 2, 3, 4, 5].to_sequence()
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            .to_array()
            ._fold()
            )
        t |> equal(typeinfo typename(t4), "array<int>")
        for (i, v in 0..3, t4) {
            t |> equal((3 - i) * 2, v)
        }
    }
}

[test]
def test_generic_fold_order(t : T?) {
    t |> run("order fold") @(t : T?) {
        var t5 <- ([5, 3, 1, 4, 2]
            .to_sequence()
            .order($(a, b) => a < b)
            ._fold()
            )
        t |> equal(typeinfo typename(t5), "iterator<int>")
        for (i, v in 0..5, t5) {
            t |> equal(i + 1, v)
        }
    }
    t |> run("order_descending fold") @(t : T?) {
        var t6 <- ([5, 3, 1, 4, 2]
            .to_sequence()
            .order_descending($(a, b) => a < b)
            ._fold()
            )
        t |> equal(typeinfo typename(t6), "iterator<int>")
        for (i, v in 0..5, t6) {
            t |> equal(5 - i, v)
        }
    }
    t |> run("order_by fold") @(t : T?) {
        var t7 = (["apple", "banana", "kiwi", "cherry", "blueberry"]
            .to_sequence()
            ._order_by(length(_))
            ._fold()
        )
        t |> equal(typeinfo typename(t7), "iterator<string>")
        var sorted = ["kiwi", "apple", "banana", "cherry", "blueberry"]    // nolint:LINT003
        for (i, v in 0..5, t7) {
            t |> equal(sorted[i], v)
        }
    }
    t |> run("order_by_descending fold") @(t : T?) {
        var t8 = (["apple", "banana", "kiwi", "cherry", "blueberry"]
            ._order_by_descending(length(_))
            ._fold()
        )
        t |> equal(typeinfo typename(t8), "array<string>")
        var sorted = ["blueberry", "banana", "cherry", "apple", "kiwi"]    // nolint:LINT003
        for (i, v in 0..5, t8) {
            t |> equal(sorted[i], v)
        }
    }
}

[test]
def test_generic_fold_aggregates(t : T?) {
    t |> run("min fold") @(t : T?) {
        var t1 = ([5, 3, 8, 1, 4]    // nolint:LINT003 — `var` required for the `typename(t1) == "int"` assertion below (let adds const)
            .to_sequence()
            .min()
            ._fold()
        )
        t |> equal(typeinfo typename(t1), "int")
        t |> equal(1, t1)
    }
    t |> run("min_by fold") @(t : T?) {
        var t2 = (["apple", "banana", "kiwi", "cherry", "blueberry"]    // nolint:LINT003
            .to_sequence()
            ._min_by(length(_))
            ._fold()
        )
        t |> equal(typeinfo typename(t2), "string")
        t |> equal("kiwi", t2)
    }
    t |> run("max fold") @(t : T?) {
        var t2 = ([5, 3, 8, 1, 4]    // nolint:LINT003
            .to_sequence()
            .max()
            ._fold()
        )
        t |> equal(typeinfo typename(t2), "int")
        t |> equal(8, t2)
    }
    t |> run("max_by fold") @(t : T?) {
        var t2 = (["apple", "banana", "kiwi", "cherry", "blueberry"]    // nolint:LINT003
            .to_sequence()
            ._max_by(length(_))
            ._fold()
        )
        t |> equal(typeinfo typename(t2), "string")
        t |> equal("blueberry", t2)
    }
    t |> run("min_max fold") @(t : T?) {
        var (tmin, tmax) = ([5, 3, 8, 1, 4]
            .to_sequence()
            .min_max()
            ._fold()
        )
        t |> equal(typeinfo typename(tmin), "int")
        t |> equal(typeinfo typename(tmax), "int")
        t |> equal(1, tmin)
        t |> equal(8, tmax)
    }
    t |> run("min_max_by fold") @(t : T?) {
        var (tmin, tmax) = (["apple", "banana", "kiwi", "cherry", "blueberry"]
            .to_sequence()
            ._min_max_by(length(_))
            ._fold()
        )
        t |> equal(typeinfo typename(tmin), "string")
        t |> equal(typeinfo typename(tmax), "string")
        t |> equal("kiwi", tmin)
        t |> equal("blueberry", tmax)
    }
    t |> run("average fold") @(t : T?) {
        var t2 = ([1, 2, 3, 4, 5, 6, 7, 8]    // nolint:LINT003
            .to_sequence()
            .average()
            ._fold()
        )
        t |> equal(typeinfo typename(t2), "double")
        t |> equal(4.5lf, t2)
    }
    t |> run("min_max_avarage fold") @(t : T?) {
        var (tmin, tmax, tavg) = ([1, 2, 3, 4, 5, 6, 7, 8]
            .to_sequence()
            .min_max_average()
            ._fold()
        )
        t |> equal(typeinfo typename(tmin), "int")
        t |> equal(typeinfo typename(tmax), "int")
        t |> equal(typeinfo typename(tavg), "int")
        t |> equal(1, tmin)
        t |> equal(8, tmax)
        t |> equal(4, tavg)
    }
    t |> run("sum fold") @(t : T?) {
        var t2 = ([1, 2, 3, 4, 5, 6, 7, 8]    // nolint:LINT003
            .to_sequence()
            .sum()
            ._fold()
        )
        t |> equal(typeinfo typename(t2), "int")
        t |> equal(36, t2)
    }
    t |> run("aggregate fold") @(t : T?) {
        var t2 = ([1, 2, 3, 4, 5, 6, 7, 8]    // nolint:LINT003
            .to_sequence()
            .aggregate("", $(a, b) => "{a},{b}")
            ._fold()
        )
        t |> equal(typeinfo typename(t2), "string")
        t |> equal(",1,2,3,4,5,6,7,8", t2)
    }
}

[test]
def test_generic_fold_skip_and_take(t : T?) {
    t |> run("skip fold") @(t : T?) {
        var t1 = ([5, 3, 8, 1, 4]    // nolint:LINT003
            .to_sequence()
            .skip(2)
            .min()
            ._fold()
        )
        t |> equal(typeinfo typename(t1), "int")
        t |> equal(1, t1)
    }
    t |> run("skip_while fold") @(t : T?) {
        var t2 = ([5, 3, 8, 1, 4]    // nolint:LINT003
            .to_sequence()
            ._skip_while(_ > 3)
            .min()
            ._fold()
        )
        t |> equal(typeinfo typename(t2), "int")
        t |> equal(1, t2)
    }
    t |> run("take fold") @(t : T?) {
        var t3 = ([5, 3, 8, 1, 4]    // nolint:LINT003
            .to_sequence()
            .take(3)
            .max()
            ._fold()
        )
        t |> equal(typeinfo typename(t3), "int")
        t |> equal(8, t3)
    }
    t |> run("take_while fold") @(t : T?) {
        var t4 = ([5, 3, 8, 1, 4]    // nolint:LINT003
            .to_sequence()
            ._take_while(_ < 8)
            .max()
            ._fold()
        )
        t |> equal(typeinfo typename(t4), "int")
        t |> equal(5, t4)
    }
    t |> run("take range fold") @(t : T?) {
        var t5 = ([5, 3, 8, 1, 4]    // nolint:LINT003
            .to_sequence()
            .take(1..4)
            .max()
            ._fold()
        )
        t |> equal(typeinfo typename(t5), "int")
        t |> equal(8, t5)
    }
}

[test]
def test_concat(t : T?) {
    t |> run("basic concat") @(t : T?) {
        var q <- (
            concat(
                [1, 2, 3, 4, 5]
                .reverse()
                ._where(_ <= 3)
                ._select(_ * 2)
            ,
                [6, 7, 8, 9, 10]
                .reverse()
                ._where(_ >= 8)
                ._select(_ * 3)
            )
            ._fold()
        )
        t |> equal(typeinfo typename(q), "array<int>")
        t |> equal(length(q), 6)
        var expected = [6, 4, 2, 30, 27, 24]    // nolint:LINT003
        for (i, v in 0..6, q) {
            t |> equal(expected[i], v)
        }
    }
    t |> run("basic append") @(t : T?) {
        var q <- (
            append(
                [1, 2, 3, 4, 5]
                .reverse()
                ._where(_ <= 3)
                ._select(_ * 2)
            ,
                100
            )
            ._fold()
        )
        t |> equal(typeinfo typename(q), "array<int>")
        t |> equal(length(q), 4)
        var expected = [6, 4, 2, 100]    // nolint:LINT003
        for (i, v in 0..4, q) {
            t |> equal(expected[i], v)
        }
    }
    t |> run("basic prepend") @(t : T?) {
        var q <- (
            prepend(
                [1, 2, 3, 4, 5]
                .reverse()
                ._where(_ <= 3)
                ._select(_ * 2)
            ,
                100
            )
            ._fold()
        )
        t |> equal(typeinfo typename(q), "array<int>")
        t |> equal(length(q), 4)
        var expected = [100, 6, 4, 2]    // nolint:LINT003
        for (i, v in 0..4, q) {
            t |> equal(expected[i], v)
        }
    }
}

[test]
def test_any_all_contains(t : T?) {
    t |> run("fold any") @(t : T?) {
        var query = (    // nolint:LINT003
            [for (x in 0..5); x]
            ._any(_ > 3)
            ._fold()
        )
        t |> success(query)
    }
    t |> run("fold any false") @(t : T?) {
        var qpred2 = (    // nolint:LINT003
            [iterator for(x in 0..5); x]
            ._any(_ > 10)
            ._fold()
        )
        t |> success(!qpred2)
    }
    t |> run("fold all") @(t : T?) {
        var query = (    // nolint:LINT003
            [iterator for(x in 0..5); x]
            ._all(_ < 5)
            ._fold()
        )
        t |> success(query)
    }
    t |> run("fold all empty") @(t : T?) {
        var qempty = (    // nolint:LINT003
            empty(type<int>)
            ._all(_ < 5)
            ._fold()
        )
        t |> success(qempty)
    }
    t |> run("fold contains") @(t : T?) {
        var query = (    // nolint:LINT003
            [iterator for(x in 0..5); x]
            .contains(3)
            ._fold()
        )
        t |> success(query)
    }
    t |> run("fold complex contains") @(t : T?) {
        var qcomplex = (    // nolint:LINT003
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
            .contains(ComplexType(a = [3, 30]))
            ._fold()
        )
        t |> success(qcomplex)
    }
    t |> run("fold complex contains false") @(t : T?) {
        var qcomplex = (    // nolint:LINT003
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
            .contains(ComplexType(a = [3, 31]))
            ._fold()
        )
        t |> success(!qcomplex)
    }
}

[test]
def test_distinct_and_distinct_by(t : T?) {
    t |> run("basic distinct") @(t : T?) {
        var query = distinct(
            [iterator for(x in 0..5); x]
        )._fold()
        for (c, i in query, 0..5) {
            t |> equal(c, i)
        }
    }
    t |> run("complex distinct") @(t : T?) {
        var qcomplex = distinct(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )._fold()
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
    t |> run("distinc by") @(t : T?) {
        var qdistinctby = (
            [iterator for(x in 0..5); ComplexType(a = [x % 3, x * 10])]
            ._distinct_by(_.a[0])
            ._fold()
        )
        var found = [false, false, false]
        for (c in qdistinctby) {
            t |> success(!found[c.a[0]])
            found[c.a[0]] = true
        }
        for (i in 0..3) {
            t |> success(found[i])
        }
    }
}

[test]
def test_unique(t : T?) {
    t |> run("basic unique") @(t : T?) {
        var t1 <- (
            [1, 2, 3, 4, 5]
            ._select(_ % 4)
            .union(
                [3, 4, 5, 6, 7]
                ._select(_ * _)
            )
            ._order_by(_) // sort
            ._fold()
        )
        var expected = [0, 1, 2, 3, 9, 16, 25, 36, 49]    // nolint:LINT003
        t |> equal(length(t1), length(expected))
        for (i, v in 0..length(expected), t1) {
            t |> equal(expected[i], v)
        }
    }
    t |> run("basic unique by") @(t : T?) {
        var t1 <- (
            [1, 2, 3, 4, 5]
            ._select(_ % 4)
            ._union_by(
                [3, 4, 5, 6, 7]
                ._select(_ * _),
                _ % 4
            )
            ._order_by(_) // sort
            ._fold()
        )
        var expected = [0, 1, 2, 3]    // nolint:LINT003
        t |> equal(length(t1), length(expected))
        for (i, v in 0..length(expected), t1) {
            t |> equal(expected[i], v)
        }
    }
}

[test]
def test_except(t : T?) {
    t |> run("basic except") @(t : T?) {
        var t1 <- (
            [1, 2, 3, 4, 5]
            ._select(_ % 4)
            .except(
                [3, 4, 5, 6, 7]
                ._select(_ * _)
            )
            ._order_by(_) // sort
            .distinct()
            ._fold()
        )
        var expected = [0, 1, 2, 3]    // nolint:LINT003
        t |> equal(length(t1), length(expected))
        for (i, v in 0..length(expected), t1) {
            t |> equal(expected[i], v)
        }
    }
    t |> run("basic except by") @(t : T?) {
        var t1 <- (
            [1, 2, 3, 4, 5]
            ._select(_ % 4)
            ._except_by(
                [3, 4, 5, 6, 7]
                ._select(_ * _),
                _ % 4
            )
            ._order_by(_) // sort
            .distinct()
            ._fold()
        )
        var expected = [2, 3]    // nolint:LINT003
        t |> equal(length(t1), length(expected))
        for (i, v in 0..length(expected), t1) {
            t |> equal(expected[i], v)
        }
    }
}

[test]
def test_intersect(t : T?) {
    t |> run("basic intersect") @(t : T?) {
        var t1 <- (
            [1, 2, 3, 4, 5]
            ._select(_)
            .intersect(
                [3, 4, 5, 6, 7]
                ._select(_)
            )
            ._order_by(_) // sort
            ._fold()
        )
        var expected = [3, 4, 5]    // nolint:LINT003
        t |> equal(length(t1), length(expected))
        for (i, v in 0..length(expected), t1) {
            t |> equal(expected[i], v)
        }
    }
    t |> run("basic intersect by") @(t : T?) {
        var t1 <- (
            [1, 2, 3, 4, 5]
            ._select(_)
            ._intersect_by(
                [3, 4, 5, 6, 7]
                ._select(_),
                _ % 4
            )
            ._order_by(_) // sort
            ._fold()
        )
        var expected = [3, 4, 5, 6]    // nolint:LINT003
        t |> equal(length(t1), length(expected))
        for (i, v in 0..length(expected), t1) {
            t |> equal(expected[i], v)
        }
    }
}

[test]
def test_chunk(t : T?) {
    t |> run("basic chunk") @(t : T?) {
        var query = (
            [for (x in 0..7); x]
            .chunk(2)
            ._select(length(_))
            ._fold()
        )
        t |> equal(typeinfo typename(query), "array<int>")
        let expected = [2, 2, 2, 1]
        t |> equal(length(expected), length(query))
        for (i, v in 0..length(expected), query) {
            t |> equal(expected[i], v)
        }
    }
}

[test]
def test_zip(t : T?) {
    t |> run("basic zip") @(t : T?) {
        var query = (
            [for (x in 0..6); x]
            ._select(_ * _)
            .zip(
                [for (y in 10..16); y]
                ._select(_)
            )
            ._fold()
        )
        t |> equal(typeinfo typename(query), "array<tuple<int;int>>")
        let expected = [(0, 10), (1, 11), (4, 12), (9, 13), (16, 14), (25, 15)]
        t |> equal(length(expected), length(query))
        for (i, v in 0..length(expected), query) {
            t |> equal(expected[i]._0, v._0)
            t |> equal(expected[i]._1, v._1)
        }
    }
    t |> run("zip with predicate") @(t : T?) {
        var query = (
            [for (x in 0..6); x]
            .zip(
                [for (y in 10..16); y]
                ._select(_),
                $(a, b : int) => a + b
            )
            ._fold()
        )
        t |> equal(typeinfo typename(query), "array<int>")
        let expected = [10, 12, 14, 16, 18, 20]
        t |> equal(length(expected), length(query))
        for (i, v in 0..length(expected), query) {
            t |> equal(expected[i], v)
        }
    }
}

[test]
def test_order_distinct(t : T?) {
    t |> run("basic order distinct") @(t : T?) {
        var t7 = (    // nolint:LINT003
            [5, 4, 4, 3, 4, 2, 1, 1, 4, 3]
            ._select(_ * 10)
            .order()
            .distinct()
            .min()
            ._fold()
        )
        t |> equal(typeinfo typename(t7), "int")
        t |> equal(10, t7)
    }
    t |> run("first position order distinct") @(t : T?) {
        var t7 = (    // nolint:LINT003
            [5, 4, 4, 3, 4, 2, 1, 1, 4, 3]
            .order()
            .distinct()
            .min()
            ._fold()
        )
        t |> equal(typeinfo typename(t7), "int")
        t |> equal(1, t7)
    }
}

[test]
def test_where_count_fold(t : T?) {
    // Guards the where + count fold rule in linq_boost (fold_where_count).
    // The fused loop emits a single counter pass with no intermediate filter array.
    t |> run("where + count: half match") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        let c = _fold(each(arr)._where(_ > 5).count())
        t |> equal(typeinfo typename(c), "int const")
        t |> equal(5, c)
    }

    t |> run("where + count: zero matches") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let c = _fold(each(arr)._where(_ > 999).count())
        t |> equal(0, c)
    }

    t |> run("where + count: all match") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let c = _fold(each(arr)._where(_ > 0).count())
        t |> equal(5, c)
    }

    t |> run("where + count: empty source") @(t : T?) {
        let arr : array<int>
        let c = _fold(each(arr)._where(_ > 0).count())
        t |> equal(0, c)
    }
}

var g_proj_hits = 0

def projection_with_side_effect(x : int) : int {
    g_proj_hits ++
    return x * 2
}

[test]
def test_counter_lane_projection_side_effects(t : T?) {
    // Counter lane must evaluate the projection per matched element so user-visible
    // side effects fire — matches raw `count(select(src, f))` semantics. Tests guard
    // the projection-is-evaluated invariant after the Phase-2A planner fix.
    t |> run("select|count fires projection once per element") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        g_proj_hits = 0
        let c = _fold(each(arr)._select(projection_with_side_effect(_)).count())
        t |> equal(5, c)
        t |> equal(5, g_proj_hits)
    }
    t |> run("where|select|count fires projection only on matches") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        g_proj_hits = 0
        let c = _fold(each(arr)._where(_ > 5)._select(projection_with_side_effect(_)).count())
        t |> equal(5, c)
        t |> equal(5, g_proj_hits)
    }
}

// ===== Phase 2B Ring 1 — Accumulator lane (sum / min / max / average / long_count) =====

[test]
def test_sum_accumulator(t : T?) {
    t |> run("sum: empty") @(t : T?) {
        let arr : array<int>
        let s = _fold(each(arr).sum())
        t |> equal(0, s)
    }
    t |> run("sum: singleton") @(t : T?) {
        let arr <- [42]
        let s = _fold(each(arr).sum())
        t |> equal(42, s)
    }
    t |> run("sum: many") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let s = _fold(each(arr).sum())
        t |> equal(15, s)
        // parity vs _old_fold
        let s_old = _old_fold(each(arr).sum())
        t |> equal(s_old, s)
    }
    t |> run("sum: where filter") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let s = _fold(each(arr)._where(_ > 2).sum())
        t |> equal(3 + 4 + 5, s)
    }
    t |> run("sum: select projection") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let s = _fold(each(arr)._select(_ * 10).sum())
        t |> equal(150, s)
    }
    t |> run("sum: where + select") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let s = _fold(each(arr)._where(_ > 2)._select(_ * 10).sum())
        t |> equal(120, s)
    }
}

[test]
def test_min_accumulator(t : T?) {
    t |> run("min: empty → default") @(t : T?) {
        let arr : array<int>
        let m = _fold(each(arr).min())
        // Matches linq.das min_impl: never assigned, returns default<T> = 0.
        t |> equal(0, m)
    }
    t |> run("min: singleton") @(t : T?) {
        let arr <- [7]
        let m = _fold(each(arr).min())
        t |> equal(7, m)
    }
    t |> run("min: many") @(t : T?) {
        let arr <- [5, 3, 8, 1, 4]
        let m = _fold(each(arr).min())
        t |> equal(1, m)
        let m_old = _old_fold(each(arr).min())
        t |> equal(m_old, m)
    }
    t |> run("min: where filter") @(t : T?) {
        let arr <- [5, 3, 8, 1, 4]
        let m = _fold(each(arr)._where(_ >= 3).min())
        t |> equal(3, m)
    }
    t |> run("min: select projection") @(t : T?) {
        let arr <- [5, 3, 8, 1, 4]
        let m = _fold(each(arr)._select(_ * -1).min())
        t |> equal(-8, m)
    }
}

[test]
def test_max_accumulator(t : T?) {
    t |> run("max: empty → default") @(t : T?) {
        let arr : array<int>
        let m = _fold(each(arr).max())
        t |> equal(0, m)
    }
    t |> run("max: singleton") @(t : T?) {
        let arr <- [7]
        let m = _fold(each(arr).max())
        t |> equal(7, m)
    }
    t |> run("max: many") @(t : T?) {
        let arr <- [5, 3, 8, 1, 4]
        let m = _fold(each(arr).max())
        t |> equal(8, m)
        let m_old = _old_fold(each(arr).max())
        t |> equal(m_old, m)
    }
    t |> run("max: where filter") @(t : T?) {
        let arr <- [5, 3, 8, 1, 4]
        let m = _fold(each(arr)._where(_ <= 5).max())
        t |> equal(5, m)
    }
    t |> run("max: select projection") @(t : T?) {
        let arr <- [5, 3, 8, 1, 4]
        let m = _fold(each(arr)._select(_ * 2).max())
        t |> equal(16, m)
    }
}

[test]
def test_average_accumulator(t : T?) {
    t |> run("average: empty → NaN") @(t : T?) {
        let arr : array<int>
        let a = _fold(each(arr).average())
        t |> equal(true, is_nan(a))
    }
    t |> run("average: singleton") @(t : T?) {
        let arr <- [42]
        let a = _fold(each(arr).average())
        t |> equal(42.0lf, a)
    }
    t |> run("average: many") @(t : T?) {
        let arr <- [2, 4, 6, 8]
        let a = _fold(each(arr).average())
        t |> equal(5.0lf, a)
        let a_old = _old_fold(each(arr).average())
        t |> equal(a_old, a)
    }
    t |> run("average: where filter") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let a = _fold(each(arr)._where(_ > 2).average())
        t |> equal(4.0lf, a)
    }
    t |> run("average: returns double for int source") @(t : T?) {
        let arr <- [1, 2, 3]
        let a = _fold(each(arr).average())
        t |> equal("double const", typeinfo typename(a))
        t |> equal(2.0lf, a)
    }
}

[test]
def test_long_count_accumulator(t : T?) {
    t |> run("long_count: empty") @(t : T?) {
        let arr : array<int>
        let c = _fold(each(arr).long_count())
        t |> equal(0l, c)
    }
    t |> run("long_count: many") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let c = _fold(each(arr).long_count())
        t |> equal(5l, c)
        let c_old = _old_fold(each(arr).long_count())
        t |> equal(c_old, c)
    }
    t |> run("long_count: where filter") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let c = _fold(each(arr)._where(_ > 2).long_count())
        t |> equal(3l, c)
    }
    t |> run("long_count: pure select hits length shortcut") @(t : T?) {
        // Pure projection + length-bearing source → planner elides loop, emits int64(length(src)).
        // Verified functionally here; AST shape test in test_linq_fold_ast.das.
        let arr <- [1, 2, 3, 4, 5]
        let c = _fold(each(arr)._select(_ * 2).long_count())
        t |> equal(5l, c)
    }
}
[test]
def test_first_early_exit(t : T?) {
    t |> run("first: singleton") @(t : T?) {
        let arr <- [42]
        let f = _fold(each(arr).first())
        t |> equal(42, f)
    }
    t |> run("first: many returns first") @(t : T?) {
        let arr <- [7, 8, 9]
        let f = _fold(each(arr).first())
        t |> equal(7, f)
        let f_old = _old_fold(each(arr).first())
        t |> equal(f_old, f)
    }
    t |> run("first: where matches returns first match") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let f = _fold(each(arr)._where(_ > 2).first())
        t |> equal(3, f)
    }
    t |> run("first: select returns projected first") @(t : T?) {
        let arr <- [1, 2, 3]
        let f = _fold(each(arr)._select(_ * 10).first())
        t |> equal(10, f)
    }
}

[test]
def test_first_or_default_early_exit(t : T?) {
    t |> run("first_or_default: empty returns default") @(t : T?) {
        let arr : array<int>
        let f = _fold(each(arr).first_or_default(99))
        t |> equal(99, f)
    }
    t |> run("first_or_default: non-empty returns first") @(t : T?) {
        let arr <- [7, 8, 9]
        let f = _fold(each(arr).first_or_default(99))
        t |> equal(7, f)
        let f_old = _old_fold(each(arr).first_or_default(99))
        t |> equal(f_old, f)
    }
    t |> run("first_or_default: where matches returns first match") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let f = _fold(each(arr)._where(_ > 3).first_or_default(0))
        t |> equal(4, f)
    }
    t |> run("first_or_default: where no match returns default") @(t : T?) {
        let arr <- [1, 2, 3]
        let f = _fold(each(arr)._where(_ > 100).first_or_default(-1))
        t |> equal(-1, f)
    }
}

[test]
def test_any_early_exit(t : T?) {
    t |> run("any: empty → false") @(t : T?) {
        let arr : array<int>
        let r = _fold(each(arr).any())
        t |> equal(false, r)
    }
    t |> run("any: non-empty (no pred) → true (shortcut path)") @(t : T?) {
        let arr <- [1]
        let r = _fold(each(arr).any())
        t |> equal(true, r)
    }
    t |> run("any: with where matching → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)._where(_ > 3).any())
        t |> equal(true, r)
    }
    t |> run("any: with where not matching → false") @(t : T?) {
        let arr <- [1, 2, 3]
        let r = _fold(each(arr)._where(_ > 100).any())
        t |> equal(false, r)
    }
    t |> run("any: with pred matching → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)._any(_ > 3))
        t |> equal(true, r)
    }
}

[test]
def test_all_early_exit(t : T?) {
    t |> run("all: empty → true (vacuous)") @(t : T?) {
        let arr : array<int>
        let r = _fold(each(arr)._all(_ > 0))
        t |> equal(true, r)
    }
    t |> run("all: all match → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)._all(_ > 0))
        t |> equal(true, r)
    }
    t |> run("all: one fails → false") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)._all(_ > 2))
        t |> equal(false, r)
    }
    t |> run("all: with upstream where") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        // Only elements > 2 pass: 3, 4, 5. All of those > 0 → true.
        let r = _fold(each(arr)._where(_ > 2)._all(_ > 0))
        t |> equal(true, r)
    }
}

[test]
def test_contains_early_exit(t : T?) {
    t |> run("contains: empty → false") @(t : T?) {
        let arr : array<int>
        let r = _fold(each(arr).contains(3))
        t |> equal(false, r)
    }
    t |> run("contains: found → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr).contains(3))
        t |> equal(true, r)
    }
    t |> run("contains: not found → false") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr).contains(99))
        t |> equal(false, r)
    }
    t |> run("contains: select then compare against projected") @(t : T?) {
        let arr <- [1, 2, 3]
        // Projected values: [10, 20, 30]. contains(20) → true.
        let r = _fold(each(arr)._select(_ * 10).contains(20))
        t |> equal(true, r)
    }
}

[test]
def test_take_skip_counter_lane(t : T?) {
    t |> run("counter: where.take.count") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // Even values: 2,4,6,8,10 — take 3 → 3.
        let r = _fold(each(arr)._where(_ % 2 == 0).take(3).count())
        t |> equal(3, r)
    }
    t |> run("counter: where.skip.count") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // Even values: 2,4,6,8,10. skip 2 → 6,8,10 → 3.
        let r = _fold(each(arr)._where(_ % 2 == 0).skip(2).count())
        t |> equal(3, r)
    }
    t |> run("counter: where.skip.take.count") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // Even values: 2,4,6,8,10. skip 1 take 3 → 4,6,8 → 3.
        let r = _fold(each(arr)._where(_ % 2 == 0).skip(1).take(3).count())
        t |> equal(3, r)
    }
    t |> run("counter: take(0).count → 0") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr).take(0).count())
        t |> equal(0, r)
    }
    t |> run("counter: take(huge).count → all source") @(t : T?) {
        let arr <- [1, 2, 3]
        let r = _fold(each(arr).take(1000).count())
        t |> equal(3, r)
    }
    t |> run("counter: skip(huge).count → 0") @(t : T?) {
        let arr <- [1, 2, 3]
        let r = _fold(each(arr).skip(1000).count())
        t |> equal(0, r)
    }
}

[test]
def test_take_skip_accumulator_lane(t : T?) {
    t |> run("accumulator: select.take.sum") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        // Projected: 10,20,30,40,50. take 3 → 10+20+30 = 60.
        let r = _fold(each(arr)._select(_ * 10).take(3).sum())
        t |> equal(60, r)
    }
    t |> run("accumulator: select.skip.take.average") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8]
        // Projected (* 10): 10,20,30,40,50,60,70,80. skip 2 take 4 → 30+40+50+60=180 / 4 = 45.
        let r = _fold(each(arr)._select(_ * 10).skip(2).take(4).average())
        t |> equal(45.0lf, r)
    }
    t |> run("accumulator: take.min") @(t : T?) {
        let arr <- [5, 3, 8, 1, 4]
        // take 3 → 5,3,8 → min=3.
        let r = _fold(each(arr).take(3).min())
        t |> equal(3, r)
    }
    t |> run("accumulator: where.take.long_count → int64") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // odd values: 1,3,5,7,9. take 3 → 3.
        let r = _fold(each(arr)._where(_ % 2 == 1).take(3).long_count())
        t |> equal(3l, r)
    }
    t |> run("accumulator: take(0).sum → 0") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr).take(0).sum())
        t |> equal(0, r)
    }
}

[test]
def test_take_skip_early_exit_lane(t : T?) {
    t |> run("early-exit: take.first") @(t : T?) {
        let arr <- [5, 3, 8, 1, 4]
        // take 3 → 5,3,8. first → 5.
        let r = _fold(each(arr).take(3).first())
        t |> equal(5, r)
    }
    t |> run("early-exit: where.take.any → true") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        // even values: 2,4. take 5 → 2,4. any → true.
        let r = _fold(each(arr)._where(_ % 2 == 0).take(5).any())
        t |> equal(true, r)
    }
    t |> run("early-exit: where.skip.take.first → 6") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // even: 2,4,6,8,10. skip 2 take 3 → 6,8,10. first → 6.
        let r = _fold(each(arr)._where(_ % 2 == 0).skip(2).take(3).first())
        t |> equal(6, r)
    }
    t |> run("early-exit: take.all(odd) → false (even hits within take limit)") @(t : T?) {
        let arr <- [1, 3, 5, 2, 7]
        // take 5 → all five. all(odd) → false (2 fails).
        let r = _fold(each(arr).take(5).all($(x : int) => x % 2 == 1))
        t |> equal(false, r)
    }
    t |> run("early-exit: take(3).all(odd) → true (even excluded by take)") @(t : T?) {
        let arr <- [1, 3, 5, 2, 7]
        // take 3 → 1,3,5. all(odd) → true.
        let r = _fold(each(arr).take(3).all($(x : int) => x % 2 == 1))
        t |> equal(true, r)
    }
    t |> run("early-exit: take(0).any → false (empty)") @(t : T?) {
        let arr <- [1, 2, 3]
        let r = _fold(each(arr).take(0).any())
        t |> equal(false, r)
    }
    t |> run("early-exit: take(0).all(p) → true (vacuous)") @(t : T?) {
        let arr <- [1, 2, 3]
        let r = _fold(each(arr).take(0).all($(x : int) => x > 100))
        t |> equal(true, r)
    }
    t |> run("early-exit: take.first_or_default") @(t : T?) {
        let arr : array<int>
        let r = _fold(each(arr).take(5).first_or_default(-1))
        t |> equal(-1, r)
    }
    t |> run("early-exit: take.contains") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        // take 3 → 1,2,3. contains(4) → false (excluded by take).
        let r = _fold(each(arr).take(3).contains(4))
        t |> equal(false, r)
    }
}

[test]
def test_take_skip_array_lane(t : T?) {
    t |> run("array: take.to_array") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r <- _fold(each(arr).take(3).to_array())
        t |> equal(3, length(r))
        t |> equal(1, r[0])
        t |> equal(2, r[1])
        t |> equal(3, r[2])
    }
    t |> run("array: skip.take.to_array") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7]
        let r <- _fold(each(arr).skip(2).take(3).to_array())
        t |> equal(3, length(r))
        t |> equal(3, r[0])
        t |> equal(4, r[1])
        t |> equal(5, r[2])
    }
    t |> run("array: where.skip.take.to_array") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        // even: 2,4,6,8,10. skip 1 take 3 → 4,6,8.
        let r <- _fold(each(arr)._where(_ % 2 == 0).skip(1).take(3).to_array())
        t |> equal(3, length(r))
        t |> equal(4, r[0])
        t |> equal(6, r[1])
        t |> equal(8, r[2])
    }
    t |> run("array: where.select.take.to_array") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5, 6, 7]
        // _ > 2 → 3,4,5,6,7. * 2 → 6,8,10,12,14. take 2 → 6,8.
        let r <- _fold(each(arr)._where(_ > 2)._select(_ * 2).take(2).to_array())
        t |> equal(2, length(r))
        t |> equal(6, r[0])
        t |> equal(8, r[1])
    }
    t |> run("array: take(0).to_array → empty") @(t : T?) {
        let arr <- [1, 2, 3]
        let r <- _fold(each(arr).take(0).to_array())
        t |> equal(0, length(r))
    }
    t |> run("array: skip(huge).to_array → empty") @(t : T?) {
        let arr <- [1, 2, 3]
        let r <- _fold(each(arr).skip(100).to_array())
        t |> equal(0, length(r))
    }
}

[test]
def test_chained_non_workhorse_select(t : T?) {
    // Pre-Commit-2: planner rejected any chained _select whose previous projection was
    // non-workhorse (prevWorkhorse=false → return null), falling back to the iterator path.
    // Commit 2: chained-bind emission switched to `:=` (clone) — safe for every type — so
    // ComplexType intermediates now splice.
    t |> run("int → ComplexType → int → sum") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)
            ._select(ComplexType(a = [_ * 2]))    // int → ComplexType (non-workhorse)
            ._select(_.a[0])                       // ComplexType → int (reads field)
            .sum())
        // sum(2 + 4 + 6 + 8 + 10) = 30
        t |> equal(30, r)
    }
    t |> run("where then ComplexType intermediate then sum") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)
            ._where(_ >= 2)                        // predicate before selects
            ._select(ComplexType(a = [_ * 2, _ * 3]))
            ._select(_.a[0] + _.a[1])              // ComplexType → int (reads two fields)
            .sum())
        // filtered [2,3,4,5] → [(4+6),(6+9),(8+12),(10+15)] = [10,15,20,25] → sum = 70
        t |> equal(70, r)
    }
    t |> run("workhorse → ComplexType → workhorse → max") @(t : T?) {
        let arr <- [1, 2, 3, 4, 5]
        let r = _fold(each(arr)
            ._select(_ * 2)                        // int → int (workhorse)
            ._select(ComplexType(a = [_, _ + 1]))  // int → ComplexType (non-workhorse)
            ._select(_.a[1])                       // ComplexType → int
            .max())
        // _ * 2 → [2,4,6,8,10]; .a[1] → [3,5,7,9,11]; max = 11
        t |> equal(11, r)
    }
}
