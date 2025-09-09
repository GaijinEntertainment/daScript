options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module linq shared public

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
//      Min - Finds the minimum element in an iterator
//      MinBy - Finds the minimum element in an iterator by a key
//      Max - Finds the maximum element in an iterator
//      MaxBy - Finds the maximum element in an iterator by a key
//      Sum - Sums the elements in an iterator
//      SumBy - Sums the elements in an iterator by a key
//      Average - Averages the elements in an iterator
//  Filtering data
//      Where - Filters elements in an iterator
//  Partitioning data
//      Skip - Skips the first `n` elements of an iterator
//      SkipWhile - Skips all elements of an iterator while the predicate is true
//      Take - Takes the first `n` elements of an iterator or range of elements in the iterator
//      TakeWhile - Takes elements from an iterator while the predicate is true
//  Join operations
//      Join - Joins two iterators based on a key (inner join)
//      GroupJoin - Joins two iterators based on a key, passing a sequence of the second iterator to the result (group join)
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
            yield <- it
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

def Concat(var a, b : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Concatenates two iterators
    return generator<TT -const -&> capture(<- a, <- b) () <| $ {
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
        for (x in src) {
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
        if (skipping) {
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
        for (x in src) {
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

def Min(var src : iterator<auto(TT)>) : TT -& -const {
    //! Finds the minimum element in an iterator
    var minn : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            minn = x
            first = false
        } elif (_::Less(x, minn)) {
            minn = x
        }
    }
    return minn
}

def MinBy(var src : iterator<auto(TT)>; key) : TT -& -const {
    //! Finds the minimum element in an iterator by key
    var minn : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            minn = x
            first = false
        } elif (_::Less(key(x), key(minn))) {
            minn = x
        }
    }
    return minn
}

def Max(var src : iterator<auto(TT)>) : TT -& -const {
    //! Finds the maximum element in an iterator
    var maxx : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            maxx = x
            first = false
        } elif (_::Less(maxx, x)) {
            maxx = x
        }
    }
    return maxx
}

def MaxBy(var src : iterator<auto(TT)>; key) : TT -& -const {
    //! Finds the maximum element in an iterator by key
    var maxx : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            maxx = x
            first = false
        } elif (_::Less(key(maxx), key(x))) {
            maxx = x
        }
    }
    return maxx
}

def Sum(var src : iterator<auto(TT)>) : TT -const -& {
    //! Sums elements in an iterator
    var total : TT -const -&
    for (x in src) {
        total += x
    }
    return <- total
}

def Average(var src : iterator<auto(TT)>) : TT -const -& {
    //! Averages elements in an iterator
    var total : TT -const -&
    var count : uint64
    for (x in src) {
        total += x
        count ++
    }
    if (count != 0ul) {
        total /= TT(count)
    }
    return <- total
}

[skip_lock_check]
def Join(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : iterator<typedecl(result(type<TA>, type<TB>)) -const -&> {
    //! Joins two iterators based on matching keys (inner join)
    var tableb : table<typedecl(UniqueKey(keyb(type<TB>))); array<TB -const -&>>
    for (itb in srcb) {
        let k = UniqueKey(keyb(itb))
        unsafe(tableb[k]).push_clone(itb)
    }
    var buffer : array<typedecl(result(type<TA>, type<TB>)) -const -&>
    for (ita in srca) {
        let k = UniqueKey(keya(ita))
        tableb.get(k, $(arr){
            for (itb in arr) {
                buffer.push_clone(result(ita, itb))
            }
        })
    }
    delete tableb
    return buffer.ToSequence()
}

def GroupJoin(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : iterator<typedecl(result(type<TA>, type<iterator<TB -const -&>>)) -const -&> {
    // we pass TA, and sequence of TB to 'result'
    var tableb : table<typedecl(UniqueKey(keyb(type<TB>))); array<TB -const -&>>
    for (itb in srcb) {
        let k = UniqueKey(keyb(itb))
        unsafe(tableb[k]).push_clone(itb)
    }
    var buffer : array<typedecl(result(type<TA>, type<iterator<TB -const -&>>)) -const -&>
    for (ita in srca) {
        let k = UniqueKey(keya(ita))
        tableb.get(k, $(var arr){
            buffer.push_clone(result(ita, arr.ToSequence()))
        })
    }
    delete tableb
    return buffer.ToSequence()
}
