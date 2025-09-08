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
//  Concatenation Operations
//      Concat - Concatenates two iterators
//  Converting sequence
//      ToArray - Converts an iterator to an array
//      ToSequence - Converts an array to an iterator
//      MoveToSequence - Moves an array to an iterator
// Comparators
//      Less - Compares two values, returns true if first is less than second

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
    sort(arr, $(v1, v2) => Less(key(v1), key(v2)))
    return MoveToSequence(arr)
}

def OrderByDescending(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an iterator in descending order
    var arr <- ToArray(a)
    sort(arr, $(v1, v2) => Less(key(v2), key(v1)))
    return MoveToSequence(arr)
}

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

def test_orderby() {
    var numbers <- [
        5, 3, 8, 1, 4
    ]
    var query_n <- OrderBy(
        numbers.ToSequence(),
        $(n : int) => n
    )
    var expected_n <- [1, 3, 4, 5, 8]
    for (n, i in query_n, 0..4) {
        assert(n == expected_n[i])
    }
    var people <- [
        Person(name = "Alice", age = 30),
        Person(name = "Bob", age = 25),
        Person(name = "Charlie", age = 35),
        Person(name = "Bob", age = 20),
        Person(name = "Alice", age = 25)
    ]
    var query <- OrderBy(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    var expected <- [
        Person(name = "Alice", age = 25),
        Person(name = "Alice", age = 30),
        Person(name = "Bob", age = 20),
        Person(name = "Bob", age = 25),
        Person(name = "Charlie", age = 35)
    ]
    for (p, i in query, 0..5) {
        assert(p.name == expected[i].name)
        assert(p.age == expected[i].age)
    }
    var rquery <- OrderByDescending(
        people.ToSequence(),
        $(p : Person) => (p.name, p.age)
    )
    var expected_r <- [
        Person(name = "Charlie", age = 35),
        Person(name = "Bob", age = 25),
        Person(name = "Bob", age = 20),
        Person(name = "Alice", age = 30),
        Person(name = "Alice", age = 25)
    ]
    for (p, i in rquery, 0..5) {
        assert(p.name == expected_r[i].name)
        assert(p.age == expected_r[i].age)
    }
}

def test_order() {
    var numbers <- [
        5, 3, 8, 1, 4
    ]
    var query_n <- Order(
        numbers.ToSequence(),
        $(a,b : int) => a<b
    )
    var expected_n <- [1, 3, 4, 5, 8]
    for (n, i in query_n, 0..4) {
        assert(n == expected_n[i])
    }
    var people <- [
        Person(name = "Alice", age = 30),
        Person(name = "Bob", age = 25),
        Person(name = "Charlie", age = 35),
        Person(name = "Bob", age = 20),
        Person(name = "Alice", age = 25)
    ]
    var query <- Order(
        people.ToSequence(),
        $(a,b : Person) => Less((a.name, a.age), (b.name, b.age))
    )
    var expected <- [
        Person(name = "Alice", age = 25),
        Person(name = "Alice", age = 30),
        Person(name = "Bob", age = 20),
        Person(name = "Bob", age = 25),
        Person(name = "Charlie", age = 35)
    ]
    for (p, i in query, 0..5) {
        assert(p.name == expected[i].name)
        assert(p.age == expected[i].age)
    }
    var rquery <- OrderDescending(
        people.ToSequence(),
        $(a,b : Person) => Less((a.name, a.age), (b.name, b.age))
    )
    var expected_r <- [
        Person(name = "Charlie", age = 35),
        Person(name = "Bob", age = 25),
        Person(name = "Bob", age = 20),
        Person(name = "Alice", age = 30),
        Person(name = "Alice", age = 25)
    ]
    for (p, i in rquery, 0..5) {
        assert(p.name == expected_r[i].name)
        assert(p.age == expected_r[i].age)
    }
}

[export]
def main() {
    test_concat()
    test_reverse()
    test_toarray()
    test_orderby()
    test_order()
}

