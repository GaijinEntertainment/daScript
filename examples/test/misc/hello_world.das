require daslib/linq
require math

struct ComplexType { // this one can't be copied
    a : array<int>
    def operator +=(b : ComplexType) {  // we need this one for Sum and Average
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

def test_concat() {
    var query = Concat(
        [iterator for(x in 0..5); x],
        [iterator for(x in 5..10); x]
    )
    for (c, i in query, 0..10) {
        assert(c == i)
    }
    var qcomplex = Concat(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        [iterator for(x in 5..10); ComplexType(a = [x, x * 10])]
    )
    for (c, i in qcomplex, 0..10) {
        assert(c.a.Equal([i, i * 10]))
    }
}

def test_reverse() {
    var query = Reverse(
        [iterator for(x in 0..5); x]
    )
    for (c, i in query, 0..5) {
        assert(c == 4 - i)
    }
    var qcomplex = Reverse(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
    )
    for (c, i in qcomplex, 0..5) {
        assert(c.a.Equal([4 - i, (4 - i) * 10]))
    }
}

def test_toarray() {
    var query = ToArray(
        [iterator for(x in 0..5); x]
    )
    for (c, i in query, 0..5) {
        assert(c == i)
    }
    var qcomplex = ToArray(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
    )
    for (c, i in qcomplex, 0..5) {
        assert(c.a.Equal([i, i * 10]))
    }
}

def test_totable() {
    var query = ToTable(
        [iterator for(x in 0..5); x],
        $(x : int) => x,
        $(x : int) => x * 10
    )
    var expected = { 0 => 0, 1 => 10, 2 => 20, 3 => 30, 4 => 40 }
    for (ek, ev in keys(expected), values(expected)) {
        let found = query.get(UniqueKey(ek), $(v) {
            verify(ev == v)
        })
        assert(found)
    }
    var qcomplex = ToTable(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => (c.a[0], c.a[1]),
        $(c : ComplexType) => (c.a[0] * 2, c.a[1] * 2)
    )
    for (ek, ev in keys(expected), values(expected)) {
        let k = UniqueKey((ek, ek * 10))
        let found = qcomplex.get(UniqueKey(k), $(v) {
            assert(v._0 == ek * 2)
            assert(v._1 == ek * 2 * 10)
        })
        assert(found)
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

def test_orderby() {
    var numbers = [
        5, 3, 8, 1, 4
    ]
    var query_n = OrderBy(
        numbers.ToSequence(),
        $(n : int) => n
    )
    var sorted_n = [1, 3, 4, 5, 8]
    for (n, i in query_n, 0..4) {
        assert(n == sorted_n[i])
    }
    var query = OrderBy(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    for (p, i in query, 0..5) {
        assert(p.name == sorted[i].name)
        assert(p.age == sorted[i].age)
    }
    var rquery = OrderByDescending(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    for (p, i in rquery, 0..5) {
        assert(p.name == sorted_r[i].name)
        assert(p.age == sorted_r[i].age)
    }
    var qcomplex = OrderBy(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => (c.a[0], c.a[1])
    )
    for (c, i in qcomplex, 0..5) {
        assert(c.a.Equal([i, i * 10]))
    }
    var qcomplex_r = OrderByDescending(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => (c.a[0], c.a[1])
    )
    for (c, i in qcomplex_r, 0..5) {
        assert(c.a.Equal([4 - i, (4 - i) * 10]))
    }
}

def test_order() {
    var numbers = [
        5, 3, 8, 1, 4
    ]
    var query_n = Order(
        numbers.ToSequence(),
        $(a, b : int) => a < b
    )
    var sorted_n = [1, 3, 4, 5, 8]
    for (n, i in query_n, 0..4) {
        assert(n == sorted_n[i])
    }
    var query = Order(
        people.ToSequence(),
        $(a, b : Person) => Less((a.name, a.age), (b.name, b.age))
    )
    for (p, i in query, 0..5) {
        assert(p.name == sorted[i].name)
        assert(p.age == sorted[i].age)
    }
    var rquery = OrderDescending(
        people.ToSequence(),
        $(a, b : Person) => Less((a.name, a.age), (b.name, b.age))
    )
    for (p, i in rquery, 0..5) {
        assert(p.name == sorted_r[i].name)
        assert(p.age == sorted_r[i].age)
    }
    var qcomplex = Order(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(a, b : ComplexType) => Less((a.a[0], a.a[1]), (b.a[0], b.a[1]))
    )
    for (c, i in qcomplex, 0..5) {
        assert(c.a.Equal([i, i * 10]))
    }
    var qcomplex_r = OrderDescending(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(a, b : ComplexType) => Less((a.a[0], a.a[1]), (b.a[0], b.a[1]))
    )
    for (c, i in qcomplex_r, 0..5) {
        assert(c.a.Equal([4 - i, (4 - i) * 10]))
    }
}

def test_distinct() {
    var query = Distinct(
        [iterator for(x in 0..5); x]
    )
    for (c, i in query, 0..5) {
        assert(c == i)
    }
    var qcomplex = Distinct(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
    )
    for (c, i in qcomplex, 0..5) {
        assert(c.a.Equal([i, i * 10]))
    }
}

def test_distinctby() {
    var query = DistinctBy(
        people.ToSequence(),
        $(p : Person) => p.name
    )
    var distinct_names = ["Alice", "Bob", "Charlie"]
    for (qn, dn in query, distinct_names) {
        assert(qn.name  == dn)
    }
    var qcomplex = DistinctBy(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => (c.a[0], c.a[1])
    )
    for (c, i in qcomplex, 0..5) {
        assert(c.a.Equal([i, i * 10]))
    }
}

def test_count() {
    ///! Counts elements in an iterator
    var total = Count(
        [iterator for(x in 0..5); x]
    )
    assert(total == 5) // its 5, because the range is 0..5 and range iteration goes up to but not including
    var ltotal = LongCount(
        [iterator for(x in 0..5); x]
    )
    assert(ltotal == 5l)
    var totalComplex = LongCount(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
    )
    assert(totalComplex == 5l)
}

def test_where() {
    var query = Where(
        [iterator for(x in 0..5); x],
        $(x : int) => x % 2 == 0
    )
    for (c, i in query, 0..3) {
        assert(c == i * 2)
    }
    var qp = Where(
        people.ToSequence(),
        $(p : Person) => p.age >= 30
    )
    var count = 0
    for (p in qp) {
        assert(p.age >= 30)
        count ++
    }
    assert(count == 2)
    var qcomplex = Where(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => c.a[0] % 2 == 0
    )
    for (c, i in qcomplex, 0..3) {
        assert(c.a[0] == i * 2)
    }
}

def test_skip() {
    var query = Skip(
        [iterator for(x in 0..5); x],
        2
    )
    for (c, i in query, 0..3) {
        assert(c == i + 2)
    }
    var query_skip = SkipWhile(
        [iterator for(x in 0..5); x],
        $(x : int) => x < 3
    )
    for (c, i in query_skip, 0..2) {
        assert(c == i + 3)
    }
    var qcomplex = Skip(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        2
    )
    for (c, i in qcomplex, 0..3) {
        assert(c.a.Equal([i + 2, (i + 2) * 10]))
    }
    var qcomplex_skip_while = SkipWhile(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => c.a[0] < 3
    )
    for (c, i in qcomplex_skip_while, 0..2) {
        assert(c.a.Equal([i + 3, (i + 3) * 10]))
    }
}

def test_take() {
    var query = Take(
        [iterator for(x in 0..5); x],
        3
    )
    for (c, i in query, 0..2) {
        assert(c == i)
    }
    var query_take_while = TakeWhile(
        [iterator for(x in 0..5); x],
        $(x : int) => x < 3
    )
    for (c, i in query_take_while, 0..2) {
        assert(c == i)
    }
    var query_take_range = Take(
        [iterator for(x in 0..5); x],
        1..4
    )
    for (c, i in query_take_range, 0..2) {
        assert(c == i + 1)
    }
    var qcomplex = Take(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        3
    )
    for (c, i in qcomplex, 0..2) {
        assert(c.a.Equal([i, i * 10]))
    }
    var qcomplex_take_while = TakeWhile(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => c.a[0] < 3
    )
    for (c, i in qcomplex_take_while, 0..2) {
        assert(c.a.Equal([i, i * 10]))
    }
}

def test_min() {
    var numbers = [
        5, 3, 8, 1, 4
   ]
    var query_n = Min(
        numbers.ToSequence()
    )
    assert(query_n == 1)
    var query = MinBy(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    assert(query.name == "Alice")
    assert(query.age == 25)
    var rquery = MinBy(
        people.ToSequence(),
        $(p : Person) => (p.age, p.name)
    )
    assert(rquery.name == "Bob")
    assert(rquery.age == 20)
    var qcomplex = MinBy(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => (c.a[0], c.a[1])
    )
    assert(qcomplex.a.Equal([0, 0]))
}

def test_max() {
    var numbers = [
        5, 3, 8, 1, 4
    ]
    var query_n = Max(
        numbers.ToSequence()
    )
    assert(query_n == 8)
    var query = MaxBy(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    assert(query.name == "Charlie")
    assert(query.age == 35)
    var rquery = MaxBy(
        people.ToSequence(),
        $(p : Person) => (p.age, p.name)
    )
    assert(rquery.name == "Charlie")
    assert(rquery.age == 35)
    var qcomplex = MaxBy(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => (c.a[0], c.a[1])
    )
    assert(qcomplex.a.Equal([4, 40]))
}

def test_sum() {
    var query = Sum(
        [iterator for(x in 0..5); x]
    )
    assert(query == 15)
    var qcomplex = Sum(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
    )
    assert(qcomplex.a.Equal([10, 100]))
}

def test_average() {
    var query = Average(
        [iterator for(x in 0..5); x]
    )
    assert(query == 2)
    var queryf = Average(
        [iterator for(x in 0..5); float(x)]
    )
    assert(queryf == 2.0)
    var qcomplex = Average(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
    )
    assert(qcomplex.a.Equal([2, 20]))
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

def test_join() {
    var query = Join(
        persons.ToSequence(),
        pets.ToSequence(),
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
        assert(c._0 == e._0 && c._1 == e._1)
    }
    var qcomplex = Join(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => c.a[0],
        $(c : ComplexType) => c.a[1],
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
        assert(c.a[0] == e._0 && c.a[1] == e._1)
    }
}

def test_groupjoin() {
    var query = GroupJoin(
        persons.ToSequence(),
        pets.ToSequence(),
        $(p : Person) => p.name,
        $(pet : Pet) => pet.owner.name,
        $(p : Person, var pets : iterator<Pet>) => (p.name, pets.ToArray())
    )
    var expected = [
        ("Hedlund, Magnus", [daisy]),
        ("Adams, Terry", [barley, boots]),
        ("Weiss, Charlotte", [whiskers])
    ]
    for (c, e in query, expected) {
        assert(c._0 == e._0)
        for (p1, p2 in c._1, e._1) {
            assert(p1.name == p2.name)
            assert(p1.owner.name == p2.owner.name)
        }
    }
    var qcomplex = GroupJoin(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => c.a[0],
        $(c : ComplexType) => c.a[1] / 10,
        $(c : ComplexType, var pets : iterator<ComplexType>) => (c.a[0], pets.ToArray())
    )
    var expected_complex = [
        (0, [ComplexType(a = [0, 0])]),
        (1, [ComplexType(a = [1, 10])]),
        (2, [ComplexType(a = [2, 20])]),
        (3, [ComplexType(a = [3, 30])]),
        (4, [ComplexType(a = [4, 40])])
    ]
    for (c, e in qcomplex, expected_complex) {
        assert(c._0 == e._0)
        for (p1, p2 in c._1, e._1) {
            assert(p1.a[0] == p2.a[0])
            assert(p1.a[1] == p2.a[1])
        }
    }
}

def test_union() {
    var query = Union(
        [iterator for(x in 0..5); x],
        [iterator for(x in 3..8); x]
    )
    var expected = [0, 1, 2, 3, 4, 5, 6, 7, 8]
    for (c, e in query, expected) {
        assert(c == e)
    }
    var qcomplex = Union(
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
        assert(c.a[0] == e._0)
        assert(c.a[1] == e._1)
    }
}

def test_unionby() {
    var query = UnionBy(
        [iterator for(x in 0..5); x],
        [iterator for(x in 3..8); x],
        $(x : int) => x
    )
    var expected = [0, 1, 2, 3, 4, 5, 6, 7, 8]
    for (c, e in query, expected) {
        assert(c == e)
    }
    var qcomplex = UnionBy(
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
        assert(c.a[0] == e._0)
        assert(c.a[1] == e._1)
    }
}

def test_any() {
    var query = Any(
        [iterator for(x in 0..5); x]
    )
    assert(query == true)
    var qcomplex = Any(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
    )
    assert(qcomplex == true)
}

def test_all() {
    var query = All(
        [iterator for(x in 0..5); x],
        $(x : int) => x < 5
    )
    assert(query == true)
    var qcomplex = All(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        $(c : ComplexType) => c.a[0] < 5
    )
    assert(qcomplex == true)
}

def test_except() {
    var query = Except(
        [iterator for(x in 0..5); x],
        [iterator for(x in 3..8); x]
    )
    var expected = [0, 1, 2]
    for (c, e in query, expected) {
        assert(c == e)
    }
    var qcomplex = Except(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        [iterator for(x in 3..8); ComplexType(a = [x, x * 10])]
    )
    var expected_complex = [
        (0, 0),
        (1, 10),
        (2, 20)
    ]
    for (c, e in qcomplex, expected_complex) {
        assert(c.a[0] == e._0)
        assert(c.a[1] == e._1)
    }
}

def test_exceptby() {
    var query = ExceptBy(
        [iterator for(x in 0..5); x],
        [iterator for(x in 3..8); x],
        $(x : int) => x
    )
    var expected = [0, 1, 2]
    for (c, e in query, expected) {
        assert(c == e)
    }
    var qcomplex = ExceptBy(
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
        assert(c.a[0] == e._0)
        assert(c.a[1] == e._1)
    }
}

def test_intersect() {
    var query = Intersect(
        [iterator for(x in 0..5); x],
        [iterator for(x in 3..8); x]
    )
    var expected = [3, 4, 5]
    for (c, e in query, expected) {
        assert(c == e)
    }
    var qcomplex = Intersect(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        [iterator for(x in 3..8); ComplexType(a = [x, x * 10])]
    )
    var expected_complex = [
        (3, 30),
        (4, 40),
        (5, 50)
    ]
    for (c, e in qcomplex, expected_complex) {
        assert(c.a[0] == e._0)
        assert(c.a[1] == e._1)
    }
}

def test_intersectby() {
    var query = IntersectBy(
        [iterator for(x in 0..5); x],
        [iterator for(x in 3..8); x],
        $(x : int) => x
    )
    var expected = [3, 4, 5]
    for (c, e in query, expected) {
        assert(c == e)
    }
    var qcomplex = IntersectBy(
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
        assert(c.a[0] == e._0)
        assert(c.a[1] == e._1)
    }
}

def test_contains() {
    var query = Contains(
        [iterator for(x in 0..5); x],
        3
    )
    assert(query == true)
    var qcomplex = Contains(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        ComplexType(a = [3, 30])
    )
    assert(qcomplex == true)
}

def test_groupby() {
    var query = GroupBy(
        pets.ToSequence(),
        $(pet : Pet) => floor(pet.age),
        $(pet : Pet) => pet.age,
        $(baseAge : float, var ages : iterator<float>) {
            var inscope aa = ages.ToArray()
            return (baseAge, aa.length(), aa.ToSequence().Min(), aa.ToSequence().Max())
        }
    )
    var expected = [
        (4., 2, 4.3, 4.9),
        (1., 1, 1.5, 1.5),
        (8., 1, 8.3, 8.3)
    ]
    for (c, e in query, expected) {
        assert(c._0 == e._0)
        assert(c._1 == e._1)
        assert(c._2 == e._2)
        assert(c._3 == e._3)
    }
    var qcomplex = GroupBy(
        [iterator for(x in 0..5); ComplexType(a = [x % 2, x * 10])],
        $(c : ComplexType) => c.a[0],
        $(c : ComplexType) => c.a[1],
        $(base : int, var vals : iterator<int>) {
            var inscope aa = vals.ToArray()
            return (base, aa.length(), aa.ToSequence().Min(), aa.ToSequence().Max())
        }
    )
    var expected_complex = [
        (0, 3, 0, 40),      // even
        (1, 2, 10, 30)      // odd
    ]
    for (c, e in qcomplex, expected_complex) {
        assert(c._0 == e._0)
        assert(c._1 == e._1)
        assert(c._2 == e._2)
        assert(c._3 == e._3)
    }
}

def test_aggregate() {
    var query = Aggregate(
        [iterator for(x in 0..5); x],
        0,
        $(acc, x : int) => acc + x
    )
    assert(query == 15)
    var qcomplex = Aggregate(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        ComplexType(a = [0, 0]),
        $(acc, x : ComplexType) => ComplexType(a = [acc.a[0] + x.a[0], acc.a[1] + x.a[1]])
    )
    assert(qcomplex.a[0] == 15)
    assert(qcomplex.a[1] == 150)
}

def test_defaultempty() {
    var query = DefaultEmpty(
        [iterator for(x in 0..5); x]
    )
    var expected = [0, 1, 2, 3, 4, 5]
    for (c, e in query, expected) {
        assert(c == e)
    }
    var q2 = DefaultEmpty(
        [iterator for(x in 0..0); x]
    )
    var expected2 = [0]
    for (c, e in q2, expected2) {
        assert(c == e)
    }
    var qcomplex = DefaultEmpty(
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
        assert(c.a[0] == e._0)
        assert(c.a[1] == e._1)
    }
}

def test_empty() {
    var query = Empty(type<int>)
    verify(empty(query))
    var qcomplex = Empty(type<ComplexType>)
    verify(empty(qcomplex))
}

def test_range() {
    var query = Range(0, 5)
    var expected = [0, 1, 2, 3, 4, 5]
    for (c, e in query, expected) {
        assert(c == e)
    }
}

def test_repeat() {
    var query = Repeat(42, 5)
    var expected = [42, 42, 42, 42, 42]
    for (c, e in query, expected) {
        assert(c == e)
    }
    var qcomplex = Repeat(ComplexType(a = [1, 2]), 3)
    var expected_complex = [
        (1, 2),
        (1, 2),
        (1, 2)
    ]
    for (c, e in qcomplex, expected_complex) {
        assert(c.a[0] == e._0)
        assert(c.a[1] == e._1)
    }
}

def test_sequenceequal() {
    var query = SequenceEqual(
        [iterator for(x in 0..5); x],
        [iterator for(x in 0..5); x]
    )
    assert(query == true)
    var qneq = SequenceEqual(
        [iterator for(x in 0..5); x],
        [iterator for(x in 0..5); x + 1]
    )
    assert(qneq == false)
    var qcomplex = SequenceEqual(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
    )
    assert(qcomplex == true)
}

def test_sequenceequalby() {
    var query = SequenceEqualBy(
        [iterator for(x in 0..5); x],
        [iterator for(x in 0..5); x],
        $(x : int) => x
    )
    assert(query == true)
    var qneq = SequenceEqualBy(
        [iterator for(x in 0..5); x],
        [iterator for(x in 0..5); x + 1],
        $(x : int) => x
    )
    assert(qneq == false)
    var qcomplex = SequenceEqualBy(
        [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
        [iterator for(x in 0..5); ComplexType(a = [x, x * 9])],
        $(x : ComplexType) => x.a[0]
    )
    assert(qcomplex == true)
}

[export]
def main() {
    test_concat()
    test_reverse()
    test_toarray()
    test_orderby()
    test_order()
    test_distinct()
    test_distinctby()
    test_count()
    test_where()
    test_skip()
    test_min()
    test_max()
    test_join()
    test_groupjoin()
    test_union()
    test_unionby()
    test_any()
    test_all()
    test_except()
    test_exceptby()
    test_intersect()
    test_intersectby()
    test_contains()
    test_groupby()
    test_totable()
    test_empty()
    test_defaultempty()
    test_range()
    test_repeat()
    test_sequenceequal()
    test_sequenceequalby()
}
