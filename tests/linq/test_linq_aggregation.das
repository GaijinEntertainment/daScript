options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq_boost

require _common

[test]
def test_count(t : T?) {
    t |> run("basic count") <| @(t : T?) {
        var total = count(
            [iterator for(x in 0..5); x]
        )
        t |> equal(total, 5) // its 5, because the range is 0..5 and range iteration goes up to but not including
    }
    t |> run("long count") <| @(t : T?) {
        var ltotal = long_count(
            [iterator for(x in 0..5); x]
        )
        t |> equal(ltotal, 5l)
    }
    t |> run("complex long count") <| @(t : T?) {
        var totalComplex = long_count(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        t |> equal(totalComplex, 5l)
    }
}

[test]
def test_min(t : T?) {
    t |> run("basic min") <| @(t : T?) {
        var numbers = [
            5, 3, 8, 1, 4
        ]
        var query_n = min(
            numbers.to_sequence()
        )
        t |> equal(query_n, 1)
    }
    t |> run("complex min by") <| @(t : T?) {
        var query = _min_by(
            people.to_sequence(),
            (_.name, _.age)
        )
        t |> equal(query.name, "Alice")
        t |> equal(query.age, 25)
    }
    t |> run("complex min by") <| @(t : T?) {
        var rquery = _min_by(
            people.to_sequence(),
            (_.age, _.name)
        )
        t |> equal(rquery.name, "Bob")
        t |> equal(rquery.age, 20)
    }
    t |> run("complex min by") <| @(t : T?) {
        var qcomplex = _min_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            (_.a[0], _.a[1])
        )
        t |> success(qcomplex.a.Equal([0, 0]))
    }
}

[test]
def test_max(t : T?) {
    t |> run("basic max") <| @(t : T?) {
        var numbers = [
            5, 3, 8, 1, 4
        ]
        var query_n = max(
            numbers.to_sequence()
        )
        t |> equal(query_n, 8)
    }
    t |> run("complex max by") <| @(t : T?) {
        var query = _max_by(
            people.to_sequence(),
            (_.name, _.age)
        )
        t |> equal(query.name, "Charlie")
        t |> equal(query.age, 35)
    }
    t |> run("complex max by") <| @(t : T?) {
        var rquery = _max_by(
            people.to_sequence(),
            (_.age, _.name)
        )
        t |> equal(rquery.name, "Charlie")
        t |> equal(rquery.age, 35)
    }
    t |> run("complex max by") <| @(t : T?) {
        var qcomplex = _max_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            (_.a[0], _.a[1])
        )
        t |> success(qcomplex.a.Equal([4, 40]))
    }
}

[test]
def test_sum(t : T?) {
    t |> run("basic sum") <| @(t : T?) {
        var query = sum(
            [iterator for(x in 0..5); x]
        )
        t |> equal(query, 10)
    }
    t |> run("complex sum") <| @(t : T?) {
        var qcomplex = sum(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        t |> success(qcomplex.a.Equal([10, 100]))
    }
}

[test]
def test_average(t : T?) {
    t |> run("basic average") <| @(t : T?) {
        var query = average(
            [iterator for(x in 0..5); x]
        )
        t |> equal(query, 2)
    }
    t |> run("complex average") <| @(t : T?) {
        var queryf = average(
            [iterator for(x in 0..5); float(x)]
        )
        t |> equal(queryf, 2.0)
    }
    t |> run("complex average") <| @(t : T?) {
        var qcomplex = average(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        t |> success(qcomplex.a.Equal([2, 20]))
    }
}

[test]
def test_aggregate(t : T?) {
    t |> run("basic aggregate") <| @(t : T?) {
        var query = aggregate(
            [iterator for(x in 0..5); x],
            0,
            $(acc, x) => acc + x
        )
        t |> equal(query, 10)
    }
    t |> run("complex aggregate") <| @(t : T?) {
        var qcomplex = aggregate(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [0, 0]),
            $(acc, x) => ComplexType(a = [acc.a[0] + x.a[0], acc.a[1] + x.a[1]])
        )
        t |> equal(qcomplex.a[0], 10)
        t |> equal(qcomplex.a[1], 100)
    }
}
