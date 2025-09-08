// LINQ
// requirement:
//  - types can be moved, and kept in the container or the collection

// OPERATIONS
//  Sorting Data
//      Reverse - Reverses the order of elements in an iterator
//      Order - Sorts the elements of an iterator
//      OrderDescending - Sorts the elements of an iterator in descending order
//      OrderBy - Sorts the elements of an iterator
//      OrderByDescending - Sorts the elements of an iterator in descending order
//  Set operations
//      Distinct - Returns distinct elements from an iterator
//  Concatenation Operations
//      Concat - Concatenates two iterators
//  Aggregation Operations
//      Count - Counts elements in an iterator
//      LongCount - Counts elements in an iterator, using a long integer
//  Filtering data
//      Where - Filters elements in an iterator
//  Partitioning data
//      Skip - Skips the first `n` elements of an iterator
//      SkipWhile - Skips all elements of an iterator while the predicate is true
//      Take - Takes the first `n` elements of an iterator or range of elements in the iterator
//      TakeWhile - Takes elements from an iterator while the predicate is true
//  Converting sequence
//      ToArray - Converts an iterator to an array
//      ToSequence - Converts an array to an iterator
//      MoveToSequence - Moves an array to an iterator
// Comparators and keys
//      Less - Compares two values, returns true if first is less than second
//      UniqueKey - Generates a unique key for a value

// TODO:
//      change to stable-sort?

def ToSequence(a : array<auto(TT)>) : iterator<TT -const -&> {
    //! Converts an array to an iterator
    var b := a
    return MoveToSequence(b)
}

def MoveToSequence(var a : array<auto(TT)>) : iterator<TT -const -&> {
    //! Converts an array to an iterator, captures input
    return generator<TT -const -&> capture(<- a) () <| $ {
        for (it in a) {
            yield it
        }
        delete a
        return false
    }
}

def ToArray(var a : iterator<auto(TT)>) : array<TT -const -&> {
    //! Converts an iterator to an array
    var result : array<TT -const -&>
    for (it in a) {
        result.push_clone(it)
    }
    return <- result
}

def Concat(var a, b : iterator<auto(TT)>) : iterator<TT> {
    //! Concatenates two iterators
    return generator<TT> capture(<- a, <- b) () <| $ {
        for (itA in a) {
            yield itA
        }
        for (itB in b) {
            yield itB
        }
        return false
    }
}

def Reverse(var a : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Reverses an iterator
    return generator<TT -const -&> capture(<- a) () <| $ {
        var buffer : array<TT -const -&>
        for (it in a) {
            buffer.push_clone(it)
        }
        let len = buffer.length()
        for (i in 0..len) {
            yield buffer[len - i - 1]
        }
        delete buffer
        return false
    }
}

def Order(var a : iterator<auto(TT)>; fun : auto(SortT)) : iterator<TT -const -&> {
    //! Sorts an iterator
    var arr <- ToArray(a)
    sort(arr, $(v1, v2) => fun(v1, v2))
    return MoveToSequence(arr)
}

def OrderDescending(var a : iterator<auto(TT)>; fun : auto(SortT)) : iterator<TT -const -&> {
    //! Sorts an iterator in descending order
    var arr <- ToArray(a)
    sort(arr, $(v1, v2) => fun(v2, v1))
    return MoveToSequence(arr)
}

def Less(a, b) : bool {
    return a < b
}

def Less(a, b : tuple<auto(TT)>) : bool {
    return a._0 < b._0
}

def Less(a, b : tuple<auto(TT), auto(UU)>) : bool {
    return true if (a._0 < b._0)
    return false if (a._0 > b._0)
    return a._1 < b._1
}

def Less(a, b : tuple<auto(TT), auto(UU), auto(VV)>) : bool {
    return true if (a._0 < b._0)
    return false if (a._0 > b._0)
    return true if (a._1 < b._1)
    return false if (a._1 > b._1)
    return a._2 < b._2
}

def Less(a, b : tuple<auto(TT), auto(UU), auto(VV), auto(WW)>) : bool {
    return true if (a._0 < b._0)
    return false if (a._0 > b._0)
    return true if (a._1 < b._1)
    return false if (a._1 > b._1)
    return true if (a._2 < b._2)
    return false if (a._2 > b._2)
    return a._3 < b._3
}

def OrderBy(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an iterator
    var arr <- ToArray(a)
    sort(arr, $(v1, v2) => _::Less(key(v1), key(v2)))
    return MoveToSequence(arr)
}

def OrderByDescending(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an iterator in descending order
    var arr <- ToArray(a)
    sort(arr, $(v1, v2) => _::Less(key(v2), key(v1)))
    return MoveToSequence(arr)
}

def UniqueKey(a) {
    ///! generates unique key of workhorse type for the value
    static_if (typeinfo is_workhorse(a)) {
        return a
    } else {
        return "{a}"
    }
}

def Distinct(var a : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns distinct elements from an iterator
    return generator<TT -const -&> capture(<- a) () <| $ {
        var seen : table<typedecl(UniqueKey(type<TT>))>
        for (it in a) {
            let k = UniqueKey(it)
            if (!seen.key_exists(k)) {
                yield it
                seen.insert(k)
            }
        }
        delete seen
        return false
    }
}

def DistinctBy(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Returns distinct elements from an iterator based on a key
    // TODO: specialize with const value vs var value of key, which can be captured.
    var seen : table<typedecl(UniqueKey(key(type<TT>)))>
    var buffer : array<TT -const -&>
    for (it in a) {
        let k = UniqueKey(key(it))
        if (!seen.key_exists(k)) {
            buffer.push_clone(it)
            seen.insert(k)
        }
    }
    delete seen
    return buffer.MoveToSequence()

}

def Count(var a : iterator<auto(TT)>) : int {
    //! Counts elements in an iterator
    var count = 0
    for (it in a) {
        count ++
    }
    return count
}

def LongCount(var a : iterator<auto(TT)>) : int64 {
    //! Counts elements in an iterator, using a long integer
    var count = 0l
    for (it in a) {
        count ++
    }
    return count
}

def Where(var src : iterator<auto(TT)>; predicate) : iterator<TT -& -const> {
    //! Filters elements in an iterator based on a predicate
    // TODO: specialize with const value vs var value of predicate, which can be captured.
    var buffer : array<TT -& -const>
    for (x in src) {
        if (predicate(x)) {
            buffer.push_clone(x)
        }
    }
    return buffer.MoveToSequence()
}

def Skip(var src : iterator<auto(TT)>; var total : int) {
    //! Yields all but the first `total` elements
    return <- generator<TT -&> capture(<- src) () <| $() {
        for ( x in src) {
            if (total > 0) {
                total --
                continue
            }
            yield x
        }
        return false
    }
}

def SkipWhile(var src : iterator<auto(TT)>; predicate) {
    //! Skips all elements of an iterator while the predicate is true
    // TODO: specialize with const value vs var value of predicate, which can be captured.
    var buffer : array<TT -& -const>
    var skipping = true
    for (x in src) {
        if (skipping ) {
            if (predicate(x)) {
                continue
            } else {
                skipping = false
            }
        }
        buffer.push_clone(x)
    }
    return buffer.MoveToSequence()
}

def Take(var src : iterator<auto(TT)>; var total : int) {
    //! Yields only the first `total` elements
    return <- generator<TT -&> capture(<- src) () <| $() {
        for ( x in src) {
            if (total <= 0) {
                break
            }
            total --
            yield x
        }
        return false
    }
}

def Take(var src : iterator<auto(TT)>; interval : range) {
    //! Yields only the elements of an iterator within a specified range
    return <- generator<TT -&> capture(<- src) () <| $() {
        var count = 0
        for (x in src) {
            if (count >= interval.x && count < interval.y) {
                yield x
            } elif (count >= interval.y) {
                break
            }
            count ++
        }
        return false
    }
}

def TakeWhile(var src : iterator<auto(TT)>; predicate) {
    //! Yields only the elements of an iterator while the predicate is true
    var buffer : array<TT -& -const>
    var taking = true
    for (x in src) {
        if (taking) {
            if (predicate(x)) {
                buffer.push_clone(x)
            } else {
                taking = false
            }
        }
    }
    return buffer.MoveToSequence()
}

///////////////////////////////////////////////////////////////////////////////////////////////

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
}

