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
//      DistinctBy - Returns distinct elements from an iterator by key
//      Union - Returns the union of two iterators
//      UnionBy - Returns the union of two iterators by key
//      Except - Returns elements from the first iterator that are not in the second iterator
//      ExceptBy - Returns elements from the first iterator that are not in the second iterator by key
//      Intersect - Returns elements that are present in both iterators
//      IntersectBy - Returns elements that are present in both iterators by key
//  Concatenation Operations
//      Concat - Concatenates two iterators
//  Generation Operations
//      Empty - Returns an empty iterator of a default type
//      DefaultEmpty - Returns the elements of the iterator, or a default value if the iterator is empty
//      Range - Generates a sequence of integers within a specified range
//      Repeat - Generates a sequence that contains one repeated value
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
//      Aggregate - Aggregates the elements in an iterator
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
//  Querying data
//      Any - Returns true if the iterator has at least one element
//      All - Returns true if all elements in the iterator satisfy the predicate
//      Contains - Returns true if the element is present in the iterator
//  Grouping data
//      GroupBy - Groups elements in an iterator by a key
//  Converting sequence
//      ToArray - Converts an iterator to an array
//      ToTable - Converts an iterator to a table
//      ToSequence - Converts an array to an iterator
//      MoveToSequence - Moves an array to an iterator
// Comparators and keys
//      Less - Compares two values, returns true if first is less than second
//      UniqueKey - Generates a unique key for a value
//      SequenceEqual - Checks if two sequences are equal
//      SequenceEqualBy - Checks if two sequences are equal by key

// TODO:
//      change to stable-sort?

[skip_lock_check]
def ToSequence(a : array<auto(TT)>) : iterator<TT -const -&> {
    //! Converts an array to an iterator
    var b := a
    return MoveToSequence(b)
}

[skip_lock_check]
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

[skip_lock_check]
def ToArray(var a : iterator<auto(TT)>) : array<TT -const -&> {
    //! Converts an iterator to an array
    var result : array<TT -const -&>
    for (it in a) {
        result.push_clone(it)
    }
    return <- result
}

[skip_lock_check]
def ToTable(var a : iterator<auto(TT)>; key; elementSelector) : table<typedecl(UniqueKey(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&> {
    var result : table<typedecl(UniqueKey(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&>
    for (it in a) {
        let k = key(it)
        static_if (typeinfo can_copy(it)) {
            unsafe(result[UniqueKey(k)]) = elementSelector(it)
        } else {
            unsafe(result[UniqueKey(k)]) <- clone_to_move(elementSelector(it))
        }
    }
    return <- result
}

[skip_lock_check]
def Concat(var a, b : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Concatenates two iterators
    return generator<TT -const -&> capture(<- a, <- b) () <| $ {
        for (itA in a) {
            static_if (typeinfo can_copy(itA)) {
                yield itA
            } else {
                yield <- clone_to_move(itA)
            }
        }
        for (itB in b) {
            static_if (typeinfo can_copy(itB)) {
                yield itB
            } else {
                yield <- clone_to_move(itB)
            }
        }
        return false
    }
}

[skip_lock_check]
def Reverse(var a : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Reverses an iterator
    return generator<TT -const -&> capture(<- a) () <| $ {
        var inscope buffer : array<TT -const -&>
        for (it in a) {
            buffer.push_clone(it)
        }
        let len = buffer.length()
        for (i in 0..len) {
            yield <- buffer[len - i - 1]
        }
        return false
    }
}

[skip_lock_check]
def Order(var a : iterator<auto(TT)>; fun : auto(SortT)) : iterator<TT -const -&> {
    //! Sorts an iterator
    var arr <- ToArray(a)
    sort(arr, $(v1, v2) => fun(v1, v2))
    return MoveToSequence(arr)
}

[skip_lock_check]
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

[skip_lock_check]
def OrderBy(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an iterator
    var arr <- ToArray(a)
    sort(arr, $(v1, v2) => _::Less(key(v1), key(v2)))
    return MoveToSequence(arr)
}

[skip_lock_check]
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

[skip_lock_check]
def Distinct(var a : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns distinct elements from an iterator
    return generator<TT -const -&> capture(<- a) () <| $ {
        var inscope seen : table<typedecl(UniqueKey(type<TT>))>
        for (it in a) {
            let k = UniqueKey(it)
            if (!seen.key_exists(k)) {
                static_if (typeinfo can_copy(it)) {
                    yield it
                } else {
                    yield <- clone_to_move(it)
                }
                seen.insert(k)
            }
        }
        return false
    }
}

[skip_lock_check]
def DistinctBy(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Returns distinct elements from an iterator based on a key
    // TODO: specialize with const value vs var value of key, which can be captured.
    var inscope seen : table<typedecl(UniqueKey(key(type<TT>)))>
    var buffer : array<TT -const -&>
    for (it in a) {
        let k = UniqueKey(key(it))
        if (!seen.key_exists(k)) {
            buffer.push_clone(it)
            seen.insert(k)
        }
    }
    return buffer.MoveToSequence()

}

[skip_lock_check]
def Count(var a : iterator<auto(TT)>) : int {
    //! Counts elements in an iterator
    var count = 0
    for (it in a) {
        count ++
    }
    return count
}

[skip_lock_check]
def LongCount(var a : iterator<auto(TT)>) : int64 {
    //! Counts elements in an iterator, using a long integer
    var count = 0l
    for (it in a) {
        count ++
    }
    return count
}

[skip_lock_check]
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

[skip_lock_check]
def Skip(var src : iterator<auto(TT)>; var total : int) {
    //! Yields all but the first `total` elements
    return <- generator<TT -&> capture(<- src) () <| $() {
        for (x in src) {
            if (total > 0) {
                total --
                continue
            }
            static_if (typeinfo can_copy(x)) {
                yield x
            } else {
                yield <- clone_to_move(x)
            }
        }
        return false
    }
}

[skip_lock_check]
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

[skip_lock_check]
def Take(var src : iterator<auto(TT)>; var total : int) {
    //! Yields only the first `total` elements
    return <- generator<TT -&> capture(<- src) () <| $() {
        for (x in src) {
            if (total <= 0) {
                break
            }
            total --
            static_if (typeinfo can_copy(x)) {
                yield x
            } else {
                yield <- clone_to_move(x)
            }
        }
        return false
    }
}

[skip_lock_check]
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

[skip_lock_check]
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

[skip_lock_check]
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
    return <- minn
}

[skip_lock_check]
def MinBy(var src : iterator<auto(TT)>; key) : TT -& -const {
    //! Finds the minimum element in an iterator by key
    var minn : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            static_if (typeinfo can_copy(x)) {
                minn = x
            } else {
                minn <- clone_to_move(x)
            }
            first = false
        } elif (_::Less(key(x), key(minn))) {
            static_if (typeinfo can_copy(x)) {
                minn = x
            } else {
                minn <- clone_to_move(x)
            }
        }
    }
    return <- minn
}

[skip_lock_check]
def Max(var src : iterator<auto(TT)>) : TT -& -const {
    //! Finds the maximum element in an iterator
    var maxx : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            static_if (typeinfo can_copy(x)) {
                maxx = x
            } else {
                maxx <- clone_to_move(x)
            }
            first = false
        } elif (_::Less(maxx, x)) {
            static_if (typeinfo can_copy(x)) {
                maxx = x
            } else {
                maxx <- clone_to_move(x)
            }
        }
    }
    return <- maxx
}

[skip_lock_check]
def MaxBy(var src : iterator<auto(TT)>; key) : TT -& -const {
    //! Finds the maximum element in an iterator by key
    var maxx : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            static_if (typeinfo can_copy(x)) {
                maxx = x
            } else {
                maxx <- clone_to_move(x)
            }
            first = false
        } elif (_::Less(key(maxx), key(x))) {
            static_if (typeinfo can_copy(x)) {
                maxx = x
            } else {
                maxx <- clone_to_move(x)
            }
        }
    }
    return <- maxx
}

[skip_lock_check]
def Aggregate(var src : iterator<auto(TT)>; seed : TT -&; func) : TT -const -& {
    //! Aggregates elements in an iterator using a seed and a function
    var acc : TT -& -const
    static_if (typeinfo can_copy(seed)) {
        acc = seed
    } else {
        acc <- clone_to_move(seed)
    }
    for (x in src) {
        static_if (typeinfo can_copy(x)) {
            acc = func(acc, x)
        } else {
            acc <- func(acc, clone_to_move(x))
        }
    }
    return <- acc
}

[skip_lock_check]
def Sum(var src : iterator<auto(TT)>) : TT -const -& {
    //! Sums elements in an iterator
    var total : TT -const -&
    for (x in src) {
        total += x
    }
    return <- total
}

[skip_lock_check]
def Average(var src : iterator<auto(TT)>) : TT -const -& {
    //! Averages elements in an iterator
    var total : TT -const -&
    var count : uint64
    for (x in src) {
        total += x
        count ++
    }
    if (count != 0ul) {
        static_if (typeinfo is_workhorse(type<TT>)) {
            total /= TT(count)
        } else {
            total /= count
        }
    }
    return <- total
}

[skip_lock_check]
def Join(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : iterator<typedecl(result(type<TA>, type<TB>)) -const -&> {
    //! Joins two iterators based on matching keys (inner join)
    var inscope tableb : table<typedecl(UniqueKey(keyb(type<TB>))); array<TB -const -&>>
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
    return buffer.ToSequence()
}

[skip_lock_check]
def GroupJoin(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : iterator<typedecl(result(type<TA>, type<iterator<TB -const -&>>)) -const -&> {
    // we pass TA, and sequence of TB to 'result'
    var inscope tableb : table<typedecl(UniqueKey(keyb(type<TB>))); array<TB -const -&>>
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
    return buffer.ToSequence()
}

[skip_lock_check]
def GroupBy(var source : iterator<auto(TT)>; key; elementSelector; resultSelector)  {
    //! Groups the elements of an iterator according to a specified key selector function
    var tab : table<typedecl(UniqueKey(key(type<TT>))); tuple<typedecl(key(type<TT>)), array<typedecl(elementSelector(type<TT>)) -const -&>>>
    for (it in source) {
        let kIT = key(it)
        let k = UniqueKey(kIT)
        if (tab.key_exists(k)) {
            unsafe(tab[k])._1.push_clone(elementSelector(it))
        } else {
            unsafe(tab[k]) = (kIT, [elementSelector(it)])
        }
    }
    var buffer : array<typedecl(resultSelector(type<typedecl(key(type<TT>))>, type<iterator<typedecl(elementSelector(type<TT>)) -const -&>>)) -const -&>
    for (it in values(tab)) {
        buffer.push_clone(resultSelector(it._0, it._1.ToSequence()))
    }
    return buffer.ToSequence()
}

[skip_lock_check]
def Union(var srca : iterator<auto(TT)>; var srcb : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators
    var buffer : array<TT -const -&>
    var inscope seen : table<typedecl(UniqueKey(type<TT>))>
    for (ita in srca) {
        let k = UniqueKey(ita)
        if (!seen.key_exists(k)) {
            buffer.push_clone(ita)
            seen.insert(k)
        }
    }
    for (itb in srcb) {
        let k = UniqueKey(itb)
        if (!seen.key_exists(k)) {
            buffer.push_clone(itb)
            seen.insert(k)
        }
    }
    return buffer.MoveToSequence()
}

[skip_lock_check]
def UnionBy(var srca : iterator<auto(TT)>; var srcb : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators by key
    var buffer : array<TT -const -&>
    var inscope seen : table<typedecl(UniqueKey(key(type<TT>)))>
    for (ita in srca) {
        let k = UniqueKey(key(ita))
        if (!seen.key_exists(k)) {
            buffer.push_clone(ita)
            seen.insert(k)
        }
    }
    for (itb in srcb) {
        let k = UniqueKey(key(itb))
        if (!seen.key_exists(k)) {
            buffer.push_clone(itb)
            seen.insert(k)
        }
    }
    return buffer.MoveToSequence()
}


[skip_lock_check]
def Any(var src : iterator<auto(TT)>) : bool {
    //! Returns true if the iterator has at least one element
    for (it in src) {
        return true
    }
    return false
}

[skip_lock_check]
def All(var src : iterator<auto(TT)>; predicate) : bool {
    //! Returns true if all elements in the iterator satisfy the predicate
    for (it in src) {
        if (!predicate(it)) {
            return false
        }
    }
    return true
}

[skip_lock_check]
def Except(var src : iterator<auto(TT)>; var exclude : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator
    var inscope seen : table<typedecl(UniqueKey(type<TT>))>
    for (it in exclude) {
        seen.insert(UniqueKey(it))
    }
    var buffer : array<TT -const -&>
    for (it in src) {
        if (!seen.key_exists(UniqueKey(it))) {
            buffer.push_clone(it)
        }
    }
    return buffer.MoveToSequence()
}

[skip_lock_check]
def ExceptBy(var src : iterator<auto(TT)>; var exclude : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator by key
    var inscope seen : table<typedecl(UniqueKey(key(type<TT>)))>
    for (it in exclude) {
        seen.insert(UniqueKey(key(it)))
    }
    var buffer : array<TT -const -&>
    for (it in src) {
        if (!seen.key_exists(UniqueKey(key(it)))) {
            buffer.push_clone(it)
        }
    }
    return buffer.MoveToSequence()
}

[skip_lock_check]
def Intersect(var srca : iterator<auto(TT)>; var srcb : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns elements that are present in both iterators
    var inscope seen : table<typedecl(UniqueKey(type<TT>))>
    for (it in srca) {
        seen.insert(UniqueKey(it))
    }
    var buffer : array<TT -const -&>
    for (it in srcb) {
        if (seen.key_exists(UniqueKey(it))) {
            buffer.push_clone(it)
        }
    }
    return buffer.MoveToSequence()
}

[skip_lock_check]
def IntersectBy(var srca : iterator<auto(TT)>; var srcb : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Returns elements that are present in both iterators by key
    var inscope seen : table<typedecl(UniqueKey(key(type<TT>)))>
    for (it in srca) {
        seen.insert(UniqueKey(key(it)))
    }
    var buffer : array<TT -const -&>
    for (it in srcb) {
        if (seen.key_exists(UniqueKey(key(it)))) {
            buffer.push_clone(it)
        }
    }
    return buffer.MoveToSequence()
}

[skip_lock_check]
def Contains(var src : iterator<auto(TT)>; element : TT -&) : bool {
    //! Returns true if the element is present in the iterator
    for (it in src) {
        if (it == element) {
            return true
        }
    }
    return false
}

[skip_lock_check, unused_argument(typ)]
def Empty(var typ : auto(TT)) : iterator<TT -const -&> {
    //! Returns an empty iterator of the specified type
    return <- default<iterator<TT -const -&>>
}

[skip_lock_check]
def DefaultEmpty(var src : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns the elements of the iterator, or a default value if the iterator is empty
    return generator<TT -const -&> capture(<- src) () <| $ {
        var found = false
        for (it in src) {
            found = true
            static_if (typeinfo can_copy(it)) {
                yield it
            } else {
                yield <- clone_to_move(it)
            }
        }
        if (!found) {
            var t : TT -const -&
            static_if (typeinfo can_copy(t)) {
                yield t
            } else {
                yield <- clone_to_move(t)
            }
        }
        return false
    }
}

def Range(start : int; count : int) : iterator<int> {
    //! Generates a sequence of integers within a specified range
    return generator<int>() <| $ {
        for (i in start..start + count) {
            yield i
        }
        return false
    }
}

def Repeat(element : auto(TT); count : int) : iterator<TT -const -&> {
    //! Generates a sequence that contains one repeated value
    return generator<TT -const -&> capture(:= element) () <| $ {
        for (i in 0..count) {
            static_if (typeinfo can_copy(element)) {
                yield element
            } else {
                yield <- clone_to_move(element)
            }
        }
        return false
    }
}

def SequenceEqual(var first : iterator<auto(TT)>; var second : iterator<auto(TT)>) : bool {
    //! Checks if two sequences are equal
    for (f, s in first, second) {
        if (f != s) {
            return false
        }
    }
    return true
}

def SequenceEqualBy(var first : iterator<auto(TT)>; var second : iterator<auto(TT)>; key) : bool {
    //! Checks if two sequences are equal by key
    for (f, s in first, second) {
        if (key(f) != key(s)) {
            return false
        }
    }
    return true
}