require daslib/linq

def test_concat() {
    var query <- Concat(
        [iterator for(x in 0..5); x],
        [iterator for(x in 5..10); x]
    )
    for (c, i in query, 0..10) {
        assert(c == i)
    }

}

def test_reverse() {
    var query <- Reverse(
        [iterator for(x in 0..5); x]
    )
    for (c, i in query, 0..5) {
        assert(c == 4 - i)
    }
}

def test_toarray() {
    var query <- ToArray(
        [iterator for(x in 0..5); x]
    )
    for (c, i in query, 0..5) {
        assert(c == i)
    }
}

struct Person {
    name : string
    age : int
}

var people <- [
    Person(name = "Alice", age = 30),
    Person(name = "Bob", age = 25),
    Person(name = "Charlie", age = 35),
    Person(name = "Bob", age = 20),
    Person(name = "Alice", age = 25)
]

var sorted <- [
    Person(name = "Alice", age = 25),
    Person(name = "Alice", age = 30),
    Person(name = "Bob", age = 20),
    Person(name = "Bob", age = 25),
    Person(name = "Charlie", age = 35)
]

var sorted_r <- [
    Person(name = "Charlie", age = 35),
    Person(name = "Bob", age = 25),
    Person(name = "Bob", age = 20),
    Person(name = "Alice", age = 30),
    Person(name = "Alice", age = 25)
]

def test_orderby() {
    var numbers <- [
        5, 3, 8, 1, 4
    ]
    var query_n <- OrderBy(
        numbers.ToSequence(),
        $(n : int) => n
    )
    var sorted_n <- [1, 3, 4, 5, 8]
    for (n, i in query_n, 0..4) {
        assert(n == sorted_n[i])
    }
    var query <- OrderBy(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    for (p, i in query, 0..5) {
        assert(p.name == sorted[i].name)
        assert(p.age == sorted[i].age)
    }
    var rquery <- OrderByDescending(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    for (p, i in rquery, 0..5) {
        assert(p.name == sorted_r[i].name)
        assert(p.age == sorted_r[i].age)
    }
}

def test_order() {
    var numbers <- [
        5, 3, 8, 1, 4
    ]
    var query_n <- Order(
        numbers.ToSequence(),
        $(a, b : int) => a < b
    )
    var sorted_n <- [1, 3, 4, 5, 8]
    for (n, i in query_n, 0..4) {
        assert(n == sorted_n[i])
    }
    var query <- Order(
        people.ToSequence(),
        $(a, b : Person) => Less((a.name, a.age), (b.name, b.age))
    )
    for (p, i in query, 0..5) {
        assert(p.name == sorted[i].name)
        assert(p.age == sorted[i].age)
    }
    var rquery <- OrderDescending(
        people.ToSequence(),
        $(a, b : Person) => Less((a.name, a.age), (b.name, b.age))
    )
    for (p, i in rquery, 0..5) {
        assert(p.name == sorted_r[i].name)
        assert(p.age == sorted_r[i].age)
    }
}

def test_distinct() {
    var query <- Distinct(
        [iterator for(x in 0..5); x]
    )
    for (c, i in query, 0..5) {
        assert(c == i)
    }
}

def test_distinctby() {
    var query <- DistinctBy(
        people.ToSequence(),
        $(p : Person) => p.name
    )
    var distinct_names <- ["Alice", "Bob", "Charlie"]
    for (qn, dn in query, distinct_names) {
        assert(qn.name  == dn)
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
}

def test_where() {
    var query <- Where(
        [iterator for(x in 0..5); x],
        $(x : int) => x % 2 == 0
    )
    for (c, i in query, 0..3) {
        assert(c == i * 2)
    }
    var qp <- Where(
        people.ToSequence(),
        $(p : Person) => p.age >= 30
    )
    var count = 0
    for (p in qp) {
        assert(p.age >= 30)
        count ++
    }
    assert(count == 2)
}

def test_skip() {
    var query <- Skip(
        [iterator for(x in 0..5); x],
        2
    )
    for (c, i in query, 0..3) {
        assert(c == i + 2)
    }

    var query_skip <- SkipWhile(
        [iterator for(x in 0..5); x],
        $(x : int) => x < 3
    )
    for (c, i in query_skip, 0..2) {
        assert(c == i + 3)
    }
}

def test_take() {
    var query <- Take(
        [iterator for(x in 0..5); x],
        3
    )
    for (c, i in query, 0..2) {
        assert(c == i)
    }

    var query_take_while <- TakeWhile(
        [iterator for(x in 0..5); x],
        $(x : int) => x < 3
    )
    for (c, i in query_take_while, 0..2) {
        assert(c == i)
    }

    var query_take_range <- Take(
        [iterator for(x in 0..5); x],
        1..4
    )
    for (c, i in query_take_range, 0..2) {
        assert(c == i + 1)
    }
}

def test_min() {
    var numbers <- [
        5, 3, 8, 1, 4
    ]
    var query_n = Min(
        numbers.ToSequence()
    )
    assert(query_n == 1)

    var query <- MinBy(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    assert(query.name == "Alice")
    assert(query.age == 25)

    var rquery <- MinBy(
        people.ToSequence(),
        $(p : Person) => (p.age, p.name)
    )
    assert(rquery.name == "Bob")
    assert(rquery.age == 20)
}

def test_max() {
    var numbers <- [
        5, 3, 8, 1, 4
    ]
    var query_n = Max(
        numbers.ToSequence()
    )
    assert(query_n == 8)

    var query <- MaxBy(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    assert(query.name == "Charlie")
    assert(query.age == 35)

    var rquery <- MaxBy(
        people.ToSequence(),
        $(p : Person) => (p.age, p.name)
    )
    assert(rquery.name == "Charlie")
    assert(rquery.age == 35)
}

def test_sum() {
    var query = Sum(
        [iterator for(x in 0..5); x]
    )
    assert(query == 15)
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
}

