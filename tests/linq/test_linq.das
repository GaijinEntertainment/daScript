options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq
require math

struct ComplexType { // this one can't be copied
    a : array<int>
    def operator +=(b : ComplexType) {  // we need this one for Sum and Average
        a.resize(b.a.length())
        for (i, j in a, b.a) {
            i += j
        }
    }
    def operator /=(b : uint64) {    // we need this one for Average
        let divisor = int64(b)
        for (i in a) {
            i = int(int64(i) / divisor)
        }
    }
    def operator ==(b : ComplexType) : bool {
        for (i, j in a, b.a) {
            if (i != j) {
                return false
            }
        }
        return true
    }

    def operator !=(b : ComplexType) : bool {
        for (i, j in a, b.a) {
            if (i != j) {
                return true
            }
        }
        return false
    }
}


def Equal(a, b : array<int>) : bool {
    if (a.length() != b.length()) {
        return false
    }
    for (i, j in a, b) {
        if (i != j) {
            return false
        }
    }
    return true
}


[test]
def test_concat(t : T?) {
    t |> run("basic concat") <| @(t : T?) {
        var query = concat(
            [iterator for(x in 0..5); x],
            [iterator for(x in 5..10); x]
        )
        for (c, i in query, 0..10) {
            t |> equal(c, i)
        }
    }
    t |> run("complex type concat") <| @(t : T?) {
        var qcomplex = concat(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 5..10); ComplexType(a = [x, x * 10])]
        )
        for (c, i in qcomplex, 0..10) {
            assert(c.a.Equal([i, i * 10]))
        }
    }
}

[test]
def test_reverse(t : T?) {
    t |> run("basic reverse") <| @(t : T?) {
        var query = reverse(
            [iterator for(x in 0..5); x]
        )
        for (c, i in query, 0..5) {
            t |> equal(c, 4 - i)
        }
    }
    t |> run("complex type reverse") <| @(t : T?) {
        var qcomplex = reverse(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([4 - i, (4 - i) * 10]))
        }
    }
}

[test]
def test_totable(t : T?) {
    t |> run("basic totable") <| @(t : T?) {
        var query = to_table(
            [iterator for(x in 0..5); x],
            $(x : int) => x,
            $(x : int) => x * 10
        )
        var expected = { 0 => 0, 1 => 10, 2 => 20, 3 => 30, 4 => 40 }
        for (ek, ev in keys(expected), values(expected)) {
            let found = query.get(unique_key(ek), $(v) {
                t |> equal(ev, v)
            })
            t |> success(found)
        }
    }
    t |> run("complex type totable") <| @(t : T?) {
        var qcomplex = to_table(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1]),
            $(c : ComplexType) => (c.a[0] * 2, c.a[1] * 2)
        )
        var expected = { 0 => 0, 1 => 10, 2 => 20, 3 => 30, 4 => 40 }
        for (ek, ev in keys(expected), values(expected)) {
            let k = unique_key((ek, ek * 10))
            let found = qcomplex.get(k, $(v) {
                t |> equal(v._0, ek * 2)
                t |> equal(v._1, ek * 2 * 10)
            })
            t |> success(found)
        }
    }
}

struct Person {
    name : string
    age : int
}

var people = [
    Person(name = "Alice", age = 30),
    Person(name = "Bob", age = 25),
    Person(name = "Charlie", age = 35),
    Person(name = "Bob", age = 20),
    Person(name = "Alice", age = 25)
]

var sorted = [
    Person(name = "Alice", age = 25),
    Person(name = "Alice", age = 30),
    Person(name = "Bob", age = 20),
    Person(name = "Bob", age = 25),
    Person(name = "Charlie", age = 35)
]

var sorted_r = [
    Person(name = "Charlie", age = 35),
    Person(name = "Bob", age = 25),
    Person(name = "Bob", age = 20),
    Person(name = "Alice", age = 30),
    Person(name = "Alice", age = 25)
]

[test]
def test_orderby(t : T?) {
    t |> run("basic orderby") <| @(t : T?) {
        var numbers = [
            5, 3, 8, 1, 4
        ]
        var query_n = order_by(
            numbers.to_sequence(),
            $(n : int) => n
        )
        var sorted_n = [1, 3, 4, 5, 8]
        for (n, i in query_n, 0..4) {
            t |> equal(n, sorted_n[i])
        }
    }
    t |> run("complex orderby") <| @(t : T?) {
        var query = order_by(
            people.to_sequence(),
            $(p : Person) => (p.name, p.age)
        )
        for (p, i in query, 0..5) {
            t |> equal(p.name, sorted[i].name)
            t |> equal(p.age, sorted[i].age)
        }
    }
    t |> run("reverse orderby") <| @(t : T?) {
        var rquery = order_by_descending(
            people.to_sequence(),
            $(p : Person) => (p.name, p.age)
        )
        for (p, i in rquery, 0..5) {
            t |> equal(p.name, sorted_r[i].name)
            t |> equal(p.age, sorted_r[i].age)
        }
    }
    t |> run("complex orderby") <| @(t : T?) {
        var qcomplex = order_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1])
        )
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
    t |> run("reverse complex orderby") <| @(t : T?) {
        var qcomplex_r = order_by_descending(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1])
        )
        for (c, i in qcomplex_r, 0..5) {
            t |> success(c.a.Equal([4 - i, (4 - i) * 10]))
        }
    }
}

[test]
def test_order(t : T?) {
    t |> run("basic order") <| @(t : T?) {
        var numbers = [
            5, 3, 8, 1, 4
        ]
        var query_n = order(
            numbers.to_sequence(),
            $(a, b : int) => a < b
        )
        var sorted_n = [1, 3, 4, 5, 8]
        for (n, i in query_n, 0..4) {
            t |> equal(n, sorted_n[i])
        }
    }
    t |> run("complex order") <| @(t : T?) {
        var query = order(
            people.to_sequence(),
            $(a, b : Person) => less((a.name, a.age), (b.name, b.age))
        )
        for (p, i in query, 0..5) {
            t |> equal(p.name, sorted[i].name)
            t |> equal(p.age, sorted[i].age)
        }
    }
    t |> run("reverse order") <| @(t : T?) {
        var rquery = order_descending(
            people.to_sequence(),
            $(a, b : Person) => less((a.name, a.age), (b.name, b.age))
        )
        for (p, i in rquery, 0..5) {
            t |> equal(p.name, sorted_r[i].name)
            t |> equal(p.age, sorted_r[i].age)
        }
    }
    t |> run("complex order") <| @(t : T?) {
        var qcomplex = order(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(a, b : ComplexType) => less((a.a[0], a.a[1]), (b.a[0], b.a[1]))
        )
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
    t |> run("reverse complex order") <| @(t : T?) {
        var qcomplex_r = order_descending(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(a, b : ComplexType) => less((a.a[0], a.a[1]), (b.a[0], b.a[1]))
        )
        for (c, i in qcomplex_r, 0..5) {
            t |> success(c.a.Equal([4 - i, (4 - i) * 10]))
        }
    }
}

[test]
def test_distinct(t : T?) {
    t |> run("basic distinct") <| @(t : T?) {
        var query = distinct(
            [iterator for(x in 0..5); x]
        )
        for (c, i in query, 0..5) {
            t |> equal(c, i)
        }
    }
    t |> run("complex distinct") <| @(t : T?) {
        var qcomplex = distinct(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
}

[test]
def test_distinctby(t : T?) {
    t |> run("basic distinct by") <| @(t : T?) {
        var query = distinct_by(
            people.to_sequence(),
            $(p : Person) => p.name
        )
        var distinct_names = ["Alice", "Bob", "Charlie"]
        for (qn, dn in query, distinct_names) {
            t |> equal(qn.name, dn)
        }
    }
    t |> run("complex distinct by") <| @(t : T?) {
        var qcomplex = distinct_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1])
        )
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
}

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
def test_where(t : T?) {
    t |> run("basic where") <| @(t : T?) {
        var query = where_(
            [iterator for(x in 0..5); x],
            $(x : int) => x % 2 == 0
        )
        for (c, i in query, 0..3) {
            t |> equal(c, i * 2)
        }
    }
    t |> run("people where") <| @(t : T?) {
        var qp = where_(
            people.to_sequence(),
            $(p : Person) => p.age >= 30
        )
        var count = 0
        for (p in qp) {
            t |> success(p.age >= 30)
            count ++
        }
        t |> equal(count, 2)
    }
    t |> run("complex where") <| @(t : T?) {
        var qcomplex = where_(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0] % 2 == 0
        )
        for (c, i in qcomplex, 0..3) {
            t |> equal(c.a[0], i * 2)
        }
    }
}

[test]
def test_skip(t : T?) {
    t |> run("basic skip") <| @(t : T?) {
        var query = skip(
            [iterator for(x in 0..5); x],
            2
        )
        for (c, i in query, 0..3) {
            t |> equal(c, i + 2)
        }
    }
    t |> run("skip while") <| @(t : T?) {
        var query_skip = skip_while(
            [iterator for(x in 0..5); x],
            $(x : int) => x < 3
        )
        for (c, i in query_skip, 0..2) {
            t |> equal(c, i + 3)
        }
    }
    t |> run("complex skip") <| @(t : T?) {
        var qcomplex = skip(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            2
        )
        for (c, i in qcomplex, 0..3) {
            t |> success(c.a.Equal([i + 2, (i + 2) * 10]))
        }
    }
    t |> run("complex skip while") <| @(t : T?) {
        var qcomplex_skip_while = skip_while(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0] < 3
        )
        for (c, i in qcomplex_skip_while, 0..2) {
            t |> success(c.a.Equal([i + 3, (i + 3) * 10]))
        }
    }
}

[test]
def test_take(t : T?) {
    t |> run("basic take") <| @(t : T?) {
        var query = take(
            [iterator for(x in 0..5); x],
            3
        )
        for (c, i in query, 0..2) {
            t |> equal(c, i)
        }
    }
    t |> run("complex take while") <| @(t : T?) {
        var query_take_while = take_while(
            [iterator for(x in 0..5); x],
            $(x : int) => x < 3
        )
        for (c, i in query_take_while, 0..2) {
            t |> equal(c, i)
        }
    }
    t |> run("complex take range") <| @(t : T?) {
        var query_take_range = take(
            [iterator for(x in 0..5); x],
            1..4
        )
        for (c, i in query_take_range, 0..2) {
            t |> equal(c, i + 1)
        }
    }
    t |> run("complex take") <| @(t : T?) {
        var qcomplex = take(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            3
        )
        for (c, i in qcomplex, 0..2) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
    t |> run("complex take while") <| @(t : T?) {
        var qcomplex_take_while = take_while(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0] < 3
        )
        for (c, i in qcomplex_take_while, 0..2) {
            t |> success(c.a.Equal([i, i * 10]))
        }
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
        var query = min_by(
            people.to_sequence(),
            $(p : Person) => (p.name, p.age)
        )
        t |> equal(query.name, "Alice")
        t |> equal(query.age, 25)
    }
    t |> run("complex min by") <| @(t : T?) {
        var rquery = min_by(
            people.to_sequence(),
            $(p : Person) => (p.age, p.name)
        )
        t |> equal(rquery.name, "Bob")
        t |> equal(rquery.age, 20)
    }
    t |> run("complex min by") <| @(t : T?) {
        var qcomplex = min_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1])
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
        var query = max_by(
            people.to_sequence(),
            $(p : Person) => (p.name, p.age)
        )
        t |> equal(query.name, "Charlie")
        t |> equal(query.age, 35)
    }
    t |> run("complex max by") <| @(t : T?) {
        var rquery = max_by(
            people.to_sequence(),
            $(p : Person) => (p.age, p.name)
        )
        t |> equal(rquery.name, "Charlie")
        t |> equal(rquery.age, 35)
    }
    t |> run("complex max by") <| @(t : T?) {
        var qcomplex = max_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1])
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

struct Pet {
    name : string
    owner : Person
    age : float
}

let magnus = Person(name = "Hedlund, Magnus")
let terry = Person(name = "Adams, Terry")
let charlotte = Person(name = "Weiss, Charlotte")
let barley = Pet(name = "Barley", owner = terry, age = 8.3)
let boots = Pet(name = "Boots", owner = terry, age = 4.9)
let whiskers = Pet(name = "Whiskers", owner = charlotte, age = 1.5)
let daisy = Pet(name = "Daisy", owner = magnus, age = 4.3)
let persons = [
    magnus, terry, charlotte
]
let pets = [
    barley, boots, whiskers, daisy
]

[test]
def test_join(t : T?) {
    t |> run("basic join") <| @(t : T?) {
        var query = join(
            persons.to_sequence(),
            pets.to_sequence(),
            $(p : Person) => p,
            $(pet : Pet) => pet.owner,
            $(p : Person, pet : Pet) => (p.name, pet.name)
        )
        var expected = [
            ("Hedlund, Magnus", "Daisy"),
            ("Adams, Terry", "Barley"),
            ("Adams, Terry", "Boots"),
            ("Weiss, Charlotte", "Whiskers")
        ]
        for (c, e in query, expected) {
            t |> equal(c._0, e._0)
            t |> equal(c._1, e._1)
        }
    }
    t |> run("complex join") <| @(t : T?) {
        var qcomplex = join(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0],
            $(c : ComplexType) => c.a[0],
            $(c1 : ComplexType, c2 : ComplexType) => ComplexType(a = [c1.a[0], c2.a[1]])
        )
        var expected_complex = [
            (0, 0),
            (1, 10),
            (2, 20),
            (3, 30),
            (4, 40)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_groupjoin(t : T?) {
    t |> run("basic group join") <| @(t : T?) {
        var query = group_join(
            persons.to_sequence(),
            pets.to_sequence(),
            $(p : Person) => p.name,
            $(pet : Pet) => pet.owner.name,
            $(p : Person, var pets : iterator<Pet>) => (p.name, pets.to_array())
        )
        var expected = [
            ("Hedlund, Magnus", [daisy]),
            ("Adams, Terry", [barley, boots]),
            ("Weiss, Charlotte", [whiskers])
        ]
        for (c, e in query, expected) {
            t |> equal(c._0, e._0)
            for (p1, p2 in c._1, e._1) {
                t |> equal(p1.name, p2.name)
                t |> equal(p1.owner.name, p2.owner.name)
            }
        }
    }
    t |> run("complex group join") <| @(t : T?) {
        var qcomplex = group_join(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0],
            $(c : ComplexType) => c.a[1] / 10,
            $(c : ComplexType, var pets : iterator<ComplexType>) => (c.a[0], pets.to_array())
        )
        var expected_complex = [
            (0, [ComplexType(a = [0, 0])]),
            (1, [ComplexType(a = [1, 10])]),
            (2, [ComplexType(a = [2, 20])]),
            (3, [ComplexType(a = [3, 30])]),
            (4, [ComplexType(a = [4, 40])])
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c._0, e._0)
            for (p1, p2 in c._1, e._1) {
                t |> equal(p1.a[0], p2.a[0])
                t |> equal(p1.a[1], p2.a[1])
            }
        }
    }
}

[test]
def test_union(t : T?) {
    t |> run("basic union") <| @(t : T?) {
        var query = union(
            [iterator for(x in 0..5); x],
            [iterator for(x in 3..8); x]
        )
        var expected = [0, 1, 2, 3, 4, 5, 6, 7, 8]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
    t |> run("complex union") <| @(t : T?) {
        var qcomplex = union(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 3..8); ComplexType(a = [x, x * 10])]
        )
        var expected_complex = [
            (0, 0),
            (1, 10),
            (2, 20),
            (3, 30),
            (4, 40),
            (5, 50),
            (6, 60),
            (7, 70),
            (8, 80)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_unionby(t : T?) {
    t |> run("basic union by") <| @(t : T?) {
        var query = union_by(
            [iterator for(x in 0..5); x],
            [iterator for(x in 3..8); x],
            $(x : int) => x
        )
        var expected = [0, 1, 2, 3, 4, 5, 6, 7, 8]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
    t |> run("complex union by") <| @(t : T?) {
        var qcomplex = union_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 3..8); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0]
        )
        var expected_complex = [
            (0, 0),
            (1, 10),
            (2, 20),
            (3, 30),
            (4, 40),
            (5, 50),
            (6, 60),
            (7, 70),
            (8, 80)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_any(t : T?) {
    t |> run("basic any") <| @(t : T?) {
        var query = any(
            [iterator for(x in 0..5); x]
        )
        t |> success(query)
    }
    t |> run("complex any") <| @(t : T?) {
        var qcomplex = any(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        t |> success(qcomplex)
    }
}

[test]
def test_all(t : T?) {
    t |> run("basic all") <| @(t : T?) {
        var query = all(
            [iterator for(x in 0..5); x],
            $(x : int) => x < 5
        )
        t |> success(query)
    }
    t |> run("complex all") <| @(t : T?) {
        var qcomplex = all(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0] < 5
        )
        t |> success(qcomplex)
    }
}

[test]
def test_except(t : T?) {
    t |> run("basic except") <| @(t : T?) {
        var query = except(
            [iterator for(x in 0..5); x],
            [iterator for(x in 3..8); x]
        )
        var expected = [0, 1, 2]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
    t |> run("complex except") <| @(t : T?) {
        var qcomplex = except(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 3..8); ComplexType(a = [x, x * 10])]
        )
        var expected_complex = [
            (0, 0),
            (1, 10),
            (2, 20)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_exceptby(t : T?) {
    t |> run("basic except by") <| @(t : T?) {
        var query = except_by(
            [iterator for(x in 0..5); x],
            [iterator for(x in 3..8); x],
            $(x : int) => x
        )
        var expected = [0, 1, 2]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
    t |> run("complex except by") <| @(t : T?) {
        var qcomplex = except_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 3..8); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0]
        )
        var expected_complex = [
            (0, 0),
            (1, 10),
            (2, 20)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_intersect(t : T?) {
    t |> run("basic intersect") <| @(t : T?) {
        var query = intersect(
            [iterator for(x in 0..5); x],
            [iterator for(x in 3..8); x]
        )
        var expected = [3, 4, 5]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
    t |> run("complex intersect") <| @(t : T?) {
        var qcomplex = intersect(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 3..8); ComplexType(a = [x, x * 10])]
        )
        var expected_complex = [
            (3, 30),
            (4, 40),
            (5, 50)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_intersectby(t : T?) {
    t |> run("basic intersect by") <| @(t : T?) {
        var query = intersect_by(
            [iterator for(x in 0..5); x],
            [iterator for(x in 3..8); x],
            $(x : int) => x
        )
        var expected = [3, 4, 5]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
    t |> run("complex intersect by") <| @(t : T?) {
        var qcomplex = intersect_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 3..8); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0]
        )
        var expected_complex = [
            (3, 30),
            (4, 40),
            (5, 50)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_contains(t : T?) {
    t |> run("basic contains") <| @(t : T?) {
        var query = contains(
            [iterator for(x in 0..5); x],
            3
        )
        t |> success(query)
    }
    t |> run("complex contains") <| @(t : T?) {
        var qcomplex = contains(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [3, 30])
        )
        t |> success(qcomplex)
    }
}

[test]
def test_groupby(t : T?) {
    t |> run("basic group by") <| @(t : T?) {
        var query = group_by(
            pets.to_sequence(),
            $(pet : Pet) => floor(pet.age),
            $(pet : Pet) => pet.age,
            $(baseAge : float, var ages : iterator<float>) {
                var inscope aa = ages.to_array()
                return (baseAge, aa.length(), aa.to_sequence().min(), aa.to_sequence().max())
            }
        )
        var expected = [
            (4., 2, 4.3, 4.9),
            (1., 1, 1.5, 1.5),
            (8., 1, 8.3, 8.3)
        ]
        for (c, e in query, expected) {
            t |> equal(c._0, e._0)
            t |> equal(c._1, e._1)
            t |> equal(c._2, e._2)
            t |> equal(c._3, e._3)
        }
    }
    t |> run("complex group by") <| @(t : T?) {
        var qcomplex = group_by(
            [iterator for(x in 0..5); ComplexType(a = [x % 2, x * 10])],
            $(c : ComplexType) => c.a[0],
            $(c : ComplexType) => c.a[1],
            $(base : int, var vals : iterator<int>) {
                var inscope aa = vals.to_array()
                return (base, aa.length(), aa.to_sequence().min(), aa.to_sequence().max())
            }
        )
        var expected_complex = [
            (0, 3, 0, 40),      // even
            (1, 2, 10, 30)      // odd
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c._0, e._0)
            t |> equal(c._1, e._1)
            t |> equal(c._2, e._2)
            t |> equal(c._3, e._3)
        }
    }
}

[test]
def test_aggregate(t : T?) {
    t |> run("basic aggregate") <| @(t : T?) {
        var query = aggregate(
            [iterator for(x in 0..5); x],
            0,
            $(acc, x : int) => acc + x
        )
        t |> equal(query, 10)
    }
    t |> run("complex aggregate") <| @(t : T?) {
        var qcomplex = aggregate(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [0, 0]),
            $(acc, x : ComplexType) => ComplexType(a = [acc.a[0] + x.a[0], acc.a[1] + x.a[1]])
        )
        t |> equal(qcomplex.a[0], 10)
        t |> equal(qcomplex.a[1], 100)
    }
}

[test]
def test_defaultempty(t : T?) {
    t |> run("basic default empty") <| @(t : T?) {
        var query = default_empty(
            [iterator for(x in 0..5); x]
        )
        var expected = [0, 1, 2, 3, 4, 5]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
    t |> run("empty default") <| @(t : T?) {
        var q2 = default_empty(
            [iterator for(x in 0..0); x]
        )
        var expected2 = [0]
        for (c, e in q2, expected2) {
            t |> equal(c, e)
        }
    }
    t |> run("complex default") <| @(t : T?) {
        var qcomplex = default_empty(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        var expected_complex = [
            (0, 0),
            (1, 10),
            (2, 20),
            (3, 30),
            (4, 40),
            (5, 50)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_empty(t : T?) {
    t |> run("basic empty") <| @(t : T?) {
        var query = empty(type<int>)
        verify(empty(query))
    }
    t |> run("complex empty") <| @(t : T?) {
        var qcomplex = empty(type<ComplexType>)
        verify(empty(qcomplex))
    }
}

[test]
def test_range(t : T?) {
    t |> run("basic range_sequence") <| @(t : T?) {
        var query = range_sequence(0, 5)
        var expected = [0, 1, 2, 3, 4, 5]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
}

[test]
def test_repeat(t : T?) {
    t |> run("basic repeat") <| @(t : T?) {
        var query = repeat(42, 5)
        var expected = [42, 42, 42, 42, 42]
        for (c, e in query, expected) {
            t |> equal(c, e)
        }
    }
    t |> run("complex repeat") <| @(t : T?) {
        var qcomplex = repeat(ComplexType(a = [1, 2]), 3)
        var expected_complex = [
            (1, 2),
            (1, 2),
            (1, 2)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_sequenceequal(t : T?) {
    t |> run("basic sequence equal") <| @(t : T?) {
        var query = sequence_equal(
            [iterator for(x in 0..5); x],
            [iterator for(x in 0..5); x]
        )
        t |> success(query)
    }
    t |> run("basic sequence not equal") <| @(t : T?) {
        var qneq = sequence_equal(
            [iterator for(x in 0..5); x],
            [iterator for(x in 0..5); x + 1]
        )
        t |> success(!qneq)
    }
    t |> run("complex sequence equal") <| @(t : T?) {
        var qcomplex = sequence_equal(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        t |> success(qcomplex)
    }
}

[test]
def test_sequenceequalby(t : T?) {
    t |> run("basic sequence equal by") <| @(t : T?) {
        var query = sequence_equal_by(
            [iterator for(x in 0..5); x],
            [iterator for(x in 0..5); x],
            $(x : int) => x
        )
        t |> success(query)
    }
    t |> run("basic sequence not equal by") <| @(t : T?) {
        var qneq = sequence_equal_by(
            [iterator for(x in 0..5); x],
            [iterator for(x in 0..5); x + 1],
            $(x : int) => x
        )
        t |> success(!qneq)
    }
    t |> run("complex sequence not equal by") <| @(t : T?) {
        var qcomplex = sequence_equal_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 0..5); ComplexType(a = [x, x * 9])],
            $(x : ComplexType) => x.a[0]
        )
        t |> success(qcomplex)
    }
}

[test]
def test_elementat(t : T?) {
    t |> run("basic element at") <| @(t : T?) {
        var query = element_at(
            [iterator for(x in 0..5); x],
            2
        )
        t |> equal(query, 2)
    }
    t |> run("complex element at") <| @(t : T?) {
        var qcomplex = element_at(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            3
        )
        t |> equal(qcomplex.a[0], 3)
        t |> equal(qcomplex.a[1], 30)
    }
}

[test]
def test_elementatordefault(t : T?) {
    t |> run("basic element at or default") <| @(t : T?) {
        var query = element_at_or_default(
            [iterator for(x in 0..5); x],
            2
        )
        t |> equal(query, 2)
    }
    t |> run("complex element at or default") <| @(t : T?) {
        var qcomplex = element_at_or_default(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            3
        )
        t |> equal(qcomplex.a[0], 3)
        t |> equal(qcomplex.a[1], 30)
    }
}

[test]
def test_first(t : T?) {
    t |> run("basic first") <| @(t : T?) {
        var query = first(
            [iterator for(x in 0..5); x]
        )
        t |> equal(query, 0)
    }
    t |> run("complex first") <| @(t : T?) {
        var qcomplex = first(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        t |> equal(qcomplex.a[0], 0)
        t |> equal(qcomplex.a[1], 0)
    }
}

[test]
def test_firstordefault(t : T?) {
    t |> run("basic first or default") <| @(t : T?) {
        var query = first_or_default(
            [iterator for(x in 0..5); x],
            -1
        )
        t |> equal(query, 0)
    }
    t |> run("complex first or default") <| @(t : T?) {
        var qcomplex = first_or_default(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [0, 0])
        )
        t |> equal(qcomplex.a[0], 0)
        t |> equal(qcomplex.a[1], 0)
    }
    t |> run("default first or default") <| @(t : T?) {
        var qdefault = first_or_default(
            [iterator for(x in 0..0); x],
            -1
        )
        t |> equal(qdefault, -1)
    }
    t |> run("complex first or default") <| @(t : T?) {
        var qcomplex = first_or_default(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [0, 0])
        )
        t |> equal(qcomplex.a[0], 0)
        t |> equal(qcomplex.a[1], 0)
    }
}

[test]
def test_last(t : T?) {
    t |> run("basic last") <| @(t : T?) {
        var query = last(
            [iterator for(x in 0..5); x]
        )
        t |> equal(query, 4)
    }
    t |> run("complex last") <| @(t : T?) {
        var qcomplex = last(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        t |> equal(qcomplex.a[0], 4)
        t |> equal(qcomplex.a[1], 40)
    }
}

[test]
def test_lastordefault(t : T?) {
    t |> run("basic last or default") <| @(t : T?) {
        var query = last_or_default(
            [iterator for(x in 0..5); x],
            -1
        )
        t |> equal(query, 4)
    }
    t |> run("default last or default") <| @(t : T?) {
        var qdefault = last_or_default(
            [iterator for(x in 0..0); x],
            -1
        )
        t |> equal(qdefault, -1)
    }
    t |> run("complex last or default") <| @(t : T?) {
        var qcomplex = last_or_default(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [0, 0])
        )
        t |> equal(qcomplex.a[0], 4)
        t |> equal(qcomplex.a[1], 40)
    }
}

[test]
def test_single(t : T?) {
    t |> run("basic single") <| @(t : T?) {
        var query = single(
            [iterator for(x in 0..1); x]
        )
        t |> equal(query, 0)
    }
    t |> run("basic single not") <| @(t : T?) {
        var qone = single(
            [iterator for(x in 1..2); x]
        )
        t |> equal(qone, 1)
    }
    t |> run("complex single") <| @(t : T?) {
        var qcomplex = single(
            [iterator for(x in 0..1); ComplexType(a = [x, x * 10])]
        )
        t |> equal(qcomplex.a[0], 0)
        t |> equal(qcomplex.a[1], 0)
    }
}

[test]
def test_singleordefault(t : T?) {
    t |> run("basic single or default") <| @(t : T?) {
        var query = single_or_default(
            [iterator for(x in 0..5); x],
            -1
        )
        t |> equal(query, -1)
    }
    t |> run("basic single or default not") <| @(t : T?) {
        var qone = single_or_default(
            [iterator for(x in 0..1); x],
            -1
        )
        t |> equal(qone, 0)
    }
    t |> run("complex single or default not") <| @(t : T?) {
        var qcomplex = single_or_default(
            [iterator for(x in 2..3); ComplexType(a = [x, x * 10])],
            ComplexType(a = [-1, -1])
        )
        t |> equal(qcomplex.a[0], 2)
        t |> equal(qcomplex.a[1], 20)
    }
}

