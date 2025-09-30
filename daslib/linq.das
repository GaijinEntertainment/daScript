options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module linq shared public

/*
LINQ
requirement:
 - types can be moved, and kept in the container or the collection

OPERATIONS
Sorting data (*)
     reverse, reverse_inplace, reverse_to_array, reverse_to_sequence - Reverses the order of elements in an iterable
     order, order_inplace, order_to_array, order_to_sequence - Sorts the elements of an iterable
     order_descending,  order_descending_inplace, order_descending_to_array, order_descending_to_sequence - Sorts the elements of an iterable in descending order
     order_by, order_by_inplace, order_by_to_array, order_by_to_sequence - Sorts the elements of an iterable
     order_by_descending,  order_by_descending_inplace, order_by_descending_to_array, order_by_descending_to_sequence - Sorts the elements of an iterable in descending order
Set operations (*)
     distinct, distinct_inplace, distinct_to_array, distinct_to_sequence, distinct_by_to_array - Returns distinct elements from an iterable
     distinct_by, distinct_by_inplace, distinct_by_to_array, distinct_by_to_sequence - Returns distinct elements from an iterable (or by key)
     union, union_to_array, union_to_sequence - Returns the union of two iterables
     union_by, union_by_to_array, union_by_to_sequence - Returns the union of two iterables (or by key)
     except, except_to_array, except_to_sequence - Returns elements from the first iterable that are not in the second iterable
     except_by, except_by_to_array, except_by_to_sequence - Returns elements from the first iterable that are not in the second iterable (or by key)
     intersect - Returns elements that are present in both iterables (or by key)
     intersect_by - Returns elements that are present in both iterables (or by key)
Concatenation operations (*)
     append, append_to_array, append_to_sequence, append_inplace - Appends an element to the end of an iterable
     prepend, prepend_to_array, prepend_to_sequence, prepend_inplace - Prepends an element to the beginning of an iterable
     concat, concat_to_array, concat_to_sequence, concat_inplace - Concatenates two iterables
Generation operations (-)
     empty - Returns an empty iterable of a default type
     default_empty - Returns the elements of the iterable, or a default value if the iterable is empty
     range_sequence - Generates a sequence of integers within a specified range
     repeat - Generates a sequence that contains one repeated value
Aggregation operations (*)
     count - Counts elements in an iterable
     long_count - Counts elements in an iterable, using a long integer
     min, min_by - Finds the minimum element in an iterable
     max, max_by - Finds the maximum element in an iterable
     min_max, min_max_by - Finds both the minimum and maximum elements in an iterable
     min_max_average, min_max_average_by - Finds the minimum, maximum, and average elements in an iterable
     sum - Sums the elements in an iterable
     average - Averages the elements in an iterable
     aggregate - Aggregates the elements in an iterable
Filtering data (*)
     where_, where_to_array, where_to_sequence - Filters elements in an iterable
Partitioning data (*)
     skip, skip_to_array, skip_to_sequence, skip_inplace - Skips the first `n` elements of an iterable
     skip_while, skip_while_to_array, skip_while_to_sequence - Skips all elements of an iterable while the predicate is true
     take, take_to_array, take_to_sequence, take_inplace - Takes the first `n` elements of an iterable or range of elements in the iterable
     take_while, take_while_to_array, take_while_to_sequence - Takes elements from an iterable while the predicate is true
     chunk, chunk_to_array, chunk_to_sequence - Splits an iterable into chunks of a specified size
Join operations
     join - Joins two iterables based on a key (inner join)
     group_join - Joins two iterables based on a key, passing a sequence of the second iterable to the result (group join)
Querying data (*)
     any - Returns true if the iterable has at least one element (or at least one element which satisfies the predicate)
     all - Returns true if all elements in the iterable satisfy the predicate
     contains - Returns true if the element is present in the iterable
Grouping data
     group_by - Groups elements in an iterable by a key
Element operations
     element_at - Returns the element at the specified index
     element_at_or_default - Returns the element at the specified index, or a default value if the index is out of range
     first - Returns the first element in an iterable
     first_or_default - Returns the first element in an iterable, or a default value if the iterable is empty
     last - Returns the last element in an iterable
     last_or_default - Returns the last element in an iterable, or a default value if the iterable is empty
     single - Returns the only element of an iterable, and throws if there is not exactly one element
     single_or_default - Returns the only element of an iterable, or a default value if there is not exactly one element
Transform operations (*)
     select, select_to_array, select_to_sequence - Projects each element of an iterable into a new form
     select_many - Projects each element of an iterable to an iterable and flattens the resulting iterables into one iterable
     zip - Merges iterables into an iterable of tuples
Converting sequence
     to_table - Converts an iterable to a table
     to_sequence - Converts an array to an iterable
     move_to_sequence - Moves an array to an iterable
Comparators and keys
     less - Compares two values, returns true if first is less than second
     unique_key - Generates a unique key for a value
     sequence_equal - Checks if two sequences are equal
     sequence_equal_by - Checks if two sequences are equal by key
*/

// TODO:
//      change to stable-sort?

[skip_lock_check]
def to_sequence(a : array<auto(TT)>) : iterator<TT -const -&> {
    //! Converts an array to an iterator
    var b := a
    return move_to_sequence(b)
}

[skip_lock_check]
def move_to_sequence(var a : array<auto(TT)>) : iterator<TT -const -&> {
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
def to_table(var a : iterator<auto(TT)>; key; elementSelector) : table<typedecl(_::unique_key(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&> {
    var result : table<typedecl(_::unique_key(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&>
    for (it in a) {
        let k = key(it)
        unsafe(result[_::unique_key(k)]) := elementSelector(it)
    }
    return <- result
}

[skip_lock_check, unused_argument(tt)]
def private concat_impl(var a; var b; tt : auto(TT); reserveSize : int) : array<TT -const -&> {
    //! Concatenates two arrays or iterators
    var c : array<TT -const -&>
    static_if (typeinfo need_lock_check(c)) {
        unsafe(set_verify_array_locks(c, false))
    }
    if (reserveSize > 0) {
        c.reserve(reserveSize)
    }
    for (it in a) {
        c.push_clone(it)
    }
    for (it in b) {
        c.push_clone(it)
    }
    static_if (typeinfo need_lock_check(c)) {
        unsafe(set_verify_array_locks(c, true))
    }
    return <- c

}

[skip_lock_check]
def concat(var a, b : array<auto(TT)>) : array<TT -const -&> {
    return <- concat_impl(a, b, type<TT -const -&>, length(a) + length(b))
}

[skip_lock_check]
def concat_to_array(var a, b : iterator<auto(TT)>) : array<TT -const -&> {
    return <- concat_impl(a, b, type<TT -const -&>, 0)
}

[skip_lock_check]
def concat_to_sequence(var a, b : array<auto(TT)>) : iterator<TT -const -&> {
    return <- concat_impl(a, b, type<TT -const -&>, length(a) + length(b)).move_to_sequence()
}

[skip_lock_check]
def concat(var a, b : iterator<auto(TT)>) : iterator<TT -const -&> {
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
def concat_inplace(var a : array<auto(TT)>, b : array<auto(TT)>) {
    //! Concatenates two arrays in place
    static_if (typeinfo need_lock_check(a)) {
        _builtin_verify_locks(a)
        unsafe(set_verify_array_locks(a, false))
    }
    a.reserve(length(a) + length(b))
    for (it in b) {
        a.push_clone(it)
    }
    static_if (typeinfo need_lock_check(a)) {
        unsafe(set_verify_array_locks(a, true))
    }
}

[skip_lock_check]
def reverse_inplace(var buffer : array<auto(TT)>) {
    //! Reverses an array in place
    static_if (typeinfo need_lock_check(buffer)) {
        _builtin_verify_locks(buffer)
        unsafe(set_verify_array_locks(buffer, false))
    }
    let l = length(buffer)
    let half = l / 2
    let lm1 = l - 1
    for (i in range(half)) {
        var t : TT -& <- buffer[i]
        buffer[i] <- buffer[lm1 - i]
        buffer[lm1 - i] <- t
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
}

[skip_lock_check]
def reverse(var a : array<auto(TT)>) : array<TT -const -&> {
    //! Reverses an array
    var b := a
    reverse_inplace(b)
    return <- b
}

[skip_lock_check]
def reverse(var a : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Reverses an iterator
    return generator<TT -const -&> capture(<- a) () <| $ {
        var inscope buffer : array<TT -const -&>
        static_if (typeinfo need_lock_check(buffer)) {
            unsafe(set_verify_array_locks(buffer, false))
        }
        for (it in a) {
            buffer.push_clone(it)
        }
        let len = buffer.length()
        for (i in 0..len) {
            yield <- buffer[len - i - 1]
        }
        static_if (typeinfo need_lock_check(buffer)) {
            unsafe(set_verify_array_locks(buffer, true))
        }
        return false
    }
}

[skip_lock_check]
def reverse_to_array(var a : iterator<auto(TT)>) : array<TT -const -&> {
    //! Reverses an iterator and returns an array
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in a) {
        buffer.push_clone(it)
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    reverse_inplace(buffer)
    return <- buffer
}

[skip_lock_check]
def reverse_to_sequence(var a : array<auto(TT)>) : iterator<TT -const -&> {
    //! Reverses an array and returns an iterator
    var b := a
    reverse_inplace(b)
    return move_to_sequence(b)
}

[skip_lock_check]
def order_inplace(var buffer : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) {
    //! Sorts an array in place
    sort(buffer, fun)
}

[skip_lock_check]
def order(var a : iterator<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : iterator<TT -const -&> {
    //! Sorts an iterator
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => fun(v1, v2))
    return move_to_sequence(arr)
}

[skip_lock_check]
def order(var a : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : array<TT -const -&> {
    //! Sorts an array
    var b := a
    sort(b, fun)
    return <- b
}

[skip_lock_check]
def order_to_array(var a : iterator<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : array<TT -const -&> {
    //! Sorts an iterator and returns an array
    var arr <- to_array(a)
    sort(arr, fun)
    return <- arr
}

[skip_lock_check]
def order_to_sequence(var a : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : iterator<TT -const -&> {
    //! Sorts an array and returns an iterator
    var b := a
    sort(b, fun)
    return move_to_sequence(b)
}

[skip_lock_check]
def order_descending_inplace(var buffer : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) {
    //! Sorts an array in descending order in place
    sort(buffer, $(v1, v2) => fun(v2, v1))
}

[skip_lock_check]
def order_descending(var a : iterator<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : iterator<TT -const -&> {
    //! Sorts an iterator in descending order
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => fun(v2, v1))
    return move_to_sequence(arr)
}

[skip_lock_check]
def order_descending(var a : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : array<TT -const -&> {
    //! Sorts an array in descending order
    var b := a
    sort(b, $(v1, v2) => fun(v2, v1))
    return <- b
}

[skip_lock_check]
def order_descending_to_array(var a : iterator<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : array<TT -const -&> {
    //! Sorts an iterator in descending order and returns an array
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => fun(v2, v1))
    return <- arr
}

[skip_lock_check]
def order_descending_to_sequence(var a : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : iterator<TT -const -&> {
    //! Sorts an array in descending order and returns an iterator
    var b := a
    sort(b, $(v1, v2) => fun(v2, v1))
    return move_to_sequence(b)
}

def less(a, b) : bool {
    return a < b
}

def less(a, b : tuple<auto(TT)>) : bool {
    return a._0 < b._0
}

def less(a, b : tuple<auto(TT), auto(UU)>) : bool {
    return true if (a._0 < b._0)
    return false if (a._0 > b._0)
    return a._1 < b._1
}

def less(a, b : tuple<auto(TT), auto(UU), auto(VV)>) : bool {
    return true if (a._0 < b._0)
    return false if (a._0 > b._0)
    return true if (a._1 < b._1)
    return false if (a._1 > b._1)
    return a._2 < b._2
}

def less(a, b : tuple<auto(TT), auto(UU), auto(VV), auto(WW)>) : bool {
    return true if (a._0 < b._0)
    return false if (a._0 > b._0)
    return true if (a._1 < b._1)
    return false if (a._1 > b._1)
    return true if (a._2 < b._2)
    return false if (a._2 > b._2)
    return a._3 < b._3
}

[skip_lock_check]
def order_by_inplace(var buffer : array<auto(TT)>; key) {
    //! Sorts an array in place
    sort(buffer, $(v1, v2) => _::less(key(v1), key(v2)))
}

[skip_lock_check]
def order_by(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an iterator
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => _::less(key(v1), key(v2)))
    return move_to_sequence(arr)
}

[skip_lock_check]
def order_by(var a : array<auto(TT)>; key) : array<TT -const -&> {
    //! Sorts an array
    var b := a
    sort(b, $(v1, v2) => _::less(key(v1), key(v2)))
    return <- b
}

[skip_lock_check]
def order_by_to_array(var a : iterator<auto(TT)>; key) : array<TT -const -&> {
    //! Sorts an iterator and returns an array
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => _::less(key(v1), key(v2)))
    return <- arr
}

[skip_lock_check]
def order_by_to_sequence(var a : array<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an array and returns an iterator
    var b := a
    sort(b, $(v1, v2) => _::less(key(v1), key(v2)))
    return move_to_sequence(b)
}

[skip_lock_check]
def order_by_descending_inplace(var buffer : array<auto(TT)>; key) {
    //! Sorts an array in descending order in place
    sort(buffer, $(v1, v2) => _::less(key(v2), key(v1)))
}

[skip_lock_check]
def order_by_descending(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an iterator in descending order
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => _::less(key(v2), key(v1)))
    return move_to_sequence(arr)
}

[skip_lock_check]
def order_by_descending(var a : array<auto(TT)>; key) : array<TT -const -&> {
    //! Sorts an array in descending order
    var b := a
    sort(b, $(v1, v2) => _::less(key(v2), key(v1)))
    return <- b
}

[skip_lock_check]
def order_by_descending_to_array(var a : iterator<auto(TT)>; key) : array<TT -const -&> {
    //! Sorts an iterator in descending order and returns an array
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => _::less(key(v2), key(v1)))
    return <- arr
}

[skip_lock_check]
def order_by_descending_to_sequence(var a : array<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an array in descending order and returns an iterator
    var b := a
    sort(b, $(v1, v2) => _::less(key(v2), key(v1)))
    return move_to_sequence(b)
}

def unique_key(a) {
    ///! generates unique key of workhorse type for the value
    static_if (typeinfo is_workhorse(a)) {
        return a
    } else {
        return "{a}"
    }
}

[skip_lock_check]
def private distinct_impl(var a; tt : auto(TT)) : array<TT -const -&> {
    //! Returns distinct elements from an iterator
    var inscope seen : table<typedecl(_::unique_key(type<TT>))>
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in a) {
        let k = _::unique_key(it)
        if (!seen.key_exists(k)) {
            buffer.push_clone(it)
            seen.insert(k)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def distinct(var a : array<auto(TT)>) : array<TT -const -&> {
    //! Returns distinct elements from an array
    return <- distinct_impl(a, type<TT -const -&>)
}

[skip_lock_check]
def distinct_to_array(var a : iterator<auto(TT)>) : array<TT -const -&> {
    //! Returns distinct elements from an iterator and returns an array
    return <- distinct_impl(a, type<TT -const -&>)
}

[skip_lock_check]
def distinct_to_sequence(var a : array<auto(TT)>) : iterator<TT -const -&> {
    //! Returns distinct elements from an array and returns an iterator
    var b := a
    return distinct(b).move_to_sequence()
}

[skip_lock_check]
def distinct_inplace(var a : array<auto(TT)>) {
    //! Returns distinct elements from an array in place
    var to_index = 0
    var inscope seen : table<typedecl(_::unique_key(type<TT>))>
    static_if (typeinfo need_lock_check(a)) {
        _builtin_verify_locks(a)
        set_verify_array_locks(a, false)
    }
    for (from_index in 0 .. length(a)) {
        var it & = unsafe(a[from_index])
        let k = _::unique_key(it)
        if (!seen.key_exists(k)) {
            if (to_index != from_index) {
                a[to_index] <- it
            }
            to_index ++
            seen.insert(k)
        }
    }
    a.resize(to_index)
    static_if (typeinfo need_lock_check(a)) {
        set_verify_array_locks(a, true)
    }
}

[skip_lock_check]
def distinct(var a : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns distinct elements from an iterator
    return generator<TT -const -&> capture(<- a) () <| $ {
        var inscope seen : table<typedecl(_::unique_key(type<TT>))>
        for (it in a) {
            let k = _::unique_key(it)
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

[skip_lock_check, unused_argument(tt)]
def private distinct_by_impl(var a; tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from an iterator based on a key
    var inscope seen : table<typedecl(_::unique_key(key(type<TT>)))>
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in a) {
        let k = _::unique_key(key(it))
        if (!seen.key_exists(k)) {
            buffer.push_clone(it)
            seen.insert(k)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def distinct_by(var a : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from an array based on a key
    return <- distinct_by_impl(a, type<TT -const -&>, key)
}

[skip_lock_check]
def distinct_by_to_array(var a : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from an iterator based on a key and returns an array
    return <- distinct_by_impl(a, type<TT -const -&>, key)
}

[skip_lock_check]
def distinct_by_to_sequence(var a : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns distinct elements from an array based on a key and returns an iterator
    var b := a
    return distinct_by_impl(b, type<TT -const -&>, key).move_to_sequence()
}

[skip_lock_check]
def distinct_by(var a : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns distinct elements from an iterator based on a key
    // TODO: specialize with const value vs var value of key, which can be captured.
    return <- distinct_by_impl(a, type<TT -const -&>, key).move_to_sequence()
}

[skip_lock_check]
def distinct_by_inplace(var a : array<auto(TT)>; key : block<(arg : TT -&) : auto>) {
    //! Returns distinct elements from an array based on a key in place
    var to_index = 0
    var inscope seen : table<typedecl(_::unique_key(key(type<TT>)))>
    static_if (typeinfo need_lock_check(a)) {
        _builtin_verify_locks(a)
        set_verify_array_locks(a, false)
    }
    for (from_index in 0 .. length(a)) {
        var it & = unsafe(a[from_index])
        let k = _::unique_key(key(it))
        if (!seen.key_exists(k)) {
            if (to_index != from_index) {
                a[to_index] <- it
            }
            to_index ++
            seen.insert(k)
        }
    }
    a.resize(to_index)
    static_if (typeinfo need_lock_check(a)) {
        set_verify_array_locks(a, true)
    }
}

[skip_lock_check]
def count(var a : iterator<auto(TT)>) : int {
    //! Counts elements in an iterator
    var count = 0
    for (it in a) {
        count ++
    }
    return count
}

[skip_lock_check]
def count(var a : array<auto(TT)>) : int {
    //! Counts elements in an array
    return length(a)
}

[skip_lock_check]
def long_count(var a : iterator<auto(TT)>) : int64 {
    //! Counts elements in an iterator, using a long integer
    var count = 0l
    for (it in a) {
        count ++
    }
    return count
}

[skip_lock_check]
def long_count(var a : array<auto(TT)>) : int64 {
    //! Counts elements in an array, using a long integer
    return int64(length(a))
}

[skip_lock_check, unused_argument(tt)]
def private linq_where(var src; tt : auto(TT); len : int; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Filters elements in an iterator based on a predicate
    var buffer : array<TT -& -const>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    buffer.reserve(len)
    for (x in src) {
        if (predicate(x)) {
            buffer.push_clone(x)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def where_(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : iterator<TT -& -const> {
    //! Filters elements in an iterator based on a predicate
    return <- linq_where(src, type<TT -& -const>, 0, predicate).move_to_sequence()
}

[skip_lock_check]
def where_(var src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Filters elements in an array based on a predicate
    return <- linq_where(src, type<TT -& -const>, length(src), predicate)
}

[skip_lock_check]
def where_to_array(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Filters elements in an iterator based on a predicate and returns an array
    return <- linq_where(src, type<TT -& -const>, 0, predicate)
}

[skip_lock_check]
def where_to_sequence(var src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : iterator<TT -& -const> {
    //! Filters elements in an array based on a predicate and returns an iterator
    return <- linq_where(src, type<TT -& -const>, length(src), predicate).move_to_sequence()
}

[skip_lock_check]
def skip(var arr : array<auto(TT)>; var total : int) : array<TT -& -const> {
    //! Yields all but the first `total` elements
    let len = length(arr)
    let remaining = len - total
    if (remaining <= 0) {
        return <- default<array<TT -& -const>>
    }
    return <- subarray(arr, total..len)
}

[skip_lock_check]
def skip_inplace(var arr : array<auto(TT)>; var total : int) {
    //! Removes the first `total` elements from an array in place
    static_if (typeinfo need_lock_check(arr)) {
        _builtin_verify_locks(arr)
        unsafe(set_verify_array_locks(arr, false))
    }
    let len = length(arr)
    let remaining = len - total
    if (remaining <= 0) {
        arr.clear()
        return
    }
    for (i in 0..remaining) {
        arr[i] <- arr[i + total]
    }
    arr.resize(remaining)
    static_if (typeinfo need_lock_check(arr)) {
        unsafe(set_verify_array_locks(arr, true))
    }
}

[skip_lock_check, unused_argument(tt)]
def private skip_to_sequence(var src; tt : auto(TT); var total : int) : iterator<TT -& -const> {
    //! Yields all but the first `total` elements
    return <- generator<TT -& -const> capture(<- src) () <| $() {
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
def skip(var src : iterator<auto(TT)>; var total : int) : iterator<TT -& -const> {
    //! Yields all but the first `total` elements
    return skip_to_sequence(src, type<TT -& -const>, total)
}

[skip_lock_check]
def skip_to_sequence(var src : array<auto(TT)>; var total : int) : iterator<TT -& -const> {
    //! Yields all but the first `total` elements and returns an iterator
    return skip_to_sequence(src, type<TT -& -const>, total)
}

[skip_lock_check]
def skip_to_array(var src : iterator<auto(TT)>; var total : int) : array<TT -& -const> {
    //! Yields all but the first `total` elements and returns an array
    var buffer : array<TT -& -const>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (x in src) {
        if (total > 0) {
            total --
            continue
        }
        buffer.push_clone(x)
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private skip_while_impl(var src; tt : auto(TT); predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Skips all elements of an iterator while the predicate is true
    var buffer : array<TT -& -const>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
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
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def skip_while(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : iterator<TT -& -const> {
    //! Skips all elements of an iterator while the predicate is true
    // TODO: specialize with const value vs var value of predicate, which can be captured.
    return <- skip_while_impl(src, type<TT -& -const>, predicate).move_to_sequence()
}

[skip_lock_check]
def skip_while(var src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Skips all elements of an array while the predicate is true
    return <- skip_while_impl(src, type<TT -& -const>, predicate)
}

[skip_lock_check]
def skip_while_to_array(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Skips all elements of an iterator while the predicate is true and returns an array
    return <- skip_while_impl(src, type<TT -& -const>, predicate)
}

[skip_lock_check]
def skip_while_to_sequence(var src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : iterator<TT -& -const> {
    //! Skips all elements of an array while the predicate is true and returns an iterator
    return <- skip_while_impl(src, type<TT -& -const>, predicate).move_to_sequence()
}

[skip_lock_check]
def take(var arr : array<auto(TT)>; var total : int) : array<TT -& -const> {
    //! Yields only the first `total` elements
    let len = length(arr)
    let taking = (total < len)  ? total : len
    return <- subarray(arr, 0..taking)
}

[skip_lock_check]
def take_inplace(var arr : array<auto(TT)>; var total : int) {
    //! Keeps only the first `total` elements in an array in place
    let len = length(arr)
    let taking = (total < len)  ? total : len
    arr.resize(taking)
}

[skip_lock_check, unused_argument(tt)]
def private take_to_sequence(var src; tt : auto(TT); var total : int) : iterator<TT -& -const> {
    //! Yields only the first `total` elements
    return <- generator<TT -& -const> capture(<- src) () <| $() {
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
def take(var src : iterator<auto(TT)>; var total : int) : iterator<TT -& -const> {
    //! Yields only the first `total` elements
    return take_to_sequence(src, type<TT -& -const>, total)
}

[skip_lock_check]
def take_to_sequence(var src : array<auto(TT)>; var total : int) : iterator<TT -& -const> {
    //! Yields only the first `total` elements and returns an iterator
    return take_to_sequence(src, type<TT -& -const>, total)
}

[skip_lock_check]
def take_to_array(var src : iterator<auto(TT)>; var total : int) : array<TT -& -const> {
    //! Yields only the first `total` elements and returns an array
    var buffer : array<TT -& -const>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (x in src) {
        if (total <= 0) {
            break
        }
        total --
        buffer.push_clone(x)
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def take(var src : iterator<auto(TT)>; from : range) : iterator<TT -& -const> {
    //! Yields a range of elements from an iterator
    return <- generator<TT -& -const> capture(<- src) () <| $() {
        var index = 0
        for (x in src) {
            if (index >= from.x) {
                if (index >= from.y) {
                    break
                }
                static_if (typeinfo can_copy(x)) {
                    yield x
                } else {
                    yield <- clone_to_move(x)
                }
            }
            index ++
        }
        return false
    }
}

[skip_lock_check]
def take(var src : array<auto(TT)>; from : range) : array<TT -& -const> {
    //! Yields a range of elements from an array
    let len = length(src)
    let start = (from.x < 0) ? 0 : ((from.x > len) ? len : from.x)
    var end = (from.y < 0) ? 0 : ((from.y > len) ? len : from.y)
    if (end < start) {
        end = start
    }
    return <- subarray(src, start..end)
}

[skip_lock_check]
def take_to_array(var src : iterator<auto(TT)>; from : range) : array<TT -& -const> {
    //! Yields a range of elements from an iterator and returns an array
    var buffer : array<TT -& -const>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    var index = 0
    for (x in src) {
        if (index >= from.x) {
            if (index >= from.y) {
                break
            }
            buffer.push_clone(x)
        }
        index ++
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def take_to_sequence(var src : array<auto(TT)>; from : range) : iterator<TT -& -const> {
    //! Yields a range of elements from an array and returns an iterator
    let len = length(src)
    let start = (from.x < 0) ? 0 : ((from.x > len) ? len : from.x)
    var end = (from.y < 0) ? 0 : ((from.y > len) ? len : from.y)
    if (end < start) {
        end = start
    }
    return <- subarray(src, start..end).move_to_sequence()
}

[skip_lock_check]
def take_inplace(var arr : array<auto(TT)>; from : range) {
    //! Keeps only a range of elements in an array in place
    static_if (typeinfo need_lock_check(arr)) {
        _builtin_verify_locks(arr)
        unsafe(set_verify_array_locks(arr, false))
    }
    let len = length(arr)
    let start = (from.x < 0) ? 0 : ((from.x > len) ? len : from.x)
    var end = (from.y < 0) ? 0 : ((from.y > len) ? len : from.y)
    if (end < start) {
        end = start
    }
    let taking = end - start
    for (i in 0..taking) {
        arr[i] <- arr[start + i]
    }
    arr.resize(taking)
    static_if (typeinfo need_lock_check(arr)) {
        unsafe(set_verify_array_locks(arr, true))
    }
}

[skip_lock_check, unused_argument(tt)]
def private take_while_impl(var src; tt : auto(TT); predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Yields only the elements of an iterator while the predicate is true
    var buffer : array<TT -& -const>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
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
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def take_while(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) {
    //! Yields only the elements of an iterator while the predicate is true
    return <- take_while_impl(src, type<TT -& -const>, predicate).move_to_sequence()
}

[skip_lock_check]
def take_while(var src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Yields only the elements of an array while the predicate is true
    return <- take_while_impl(src, type<TT -& -const>, predicate)
}

[skip_lock_check]
def take_while_to_array(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Yields only the elements of an iterator while the predicate is true and returns an array
    return <- take_while_impl(src, type<TT -& -const>, predicate)
}

[skip_lock_check]
def take_while_to_sequence(var src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : iterator<TT -& -const> {
    //! Yields only the elements of an array while the predicate is true and returns an iterator
    return <- take_while_impl(src, type<TT -& -const>, predicate).move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private min_impl(var src; tt : auto(TT)) : TT -& -const {
    var minn : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            minn := x
            first = false
        } elif (_::less(x, minn)) {
            minn := x
        }
    }
    return <- minn
}

[skip_lock_check]
def min(var src : iterator<auto(TT)>) : TT -& -const {
    //! Finds the minimum element in an iterator
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_impl(src, type<TT -const -&>)
    } else {
        return <- min_impl(src, type<TT -const -&>)
    }
}

[skip_lock_check]
def min(var src : array<auto(TT)>) : TT -& -const {
    //! Finds the minimum element in an array
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_impl(src, type<TT -const -&>)
    } else {
        return <- min_impl(src, type<TT -const -&>)
    }
}

[skip_lock_check, unused_argument(tt)]
def private min_by_impl(var src; tt : auto(TT); key) : TT -& -const {
    var minn : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            minn := x
            first = false
        } elif (_::less(key(x), key(minn))) {
            minn := x
        }
    }
    return <- minn
}

[skip_lock_check]
def min_by(var src : iterator<auto(TT)>; key) : TT -& -const {
    //! Finds the minimum element in an iterator by key
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_by_impl(src, type<TT -const -&>, key)
    } else {
        return <- min_by_impl(src, type<TT -const -&>, key)
    }
}

[skip_lock_check]
def min_by(var src : array<auto(TT)>; key) : TT -& -const {
    //! Finds the minimum element in an array by key
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_by_impl(src, type<TT -const -&>, key)
    } else {
        return <- min_by_impl(src, type<TT -const -&>, key)
    }
}

[skip_lock_check, unused_argument(tt)]
def private max_impl(var src; tt : auto(TT)) : TT -& -const {
    var maxx : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            maxx := x
            first = false
        } elif (_::less(maxx, x)) {
            maxx := x
        }
    }
    return <- maxx
}

[skip_lock_check]
def max(var src : iterator<auto(TT)>) : TT -& -const {
    //! Finds the maximum element in an iterator
    static_if (typeinfo is_workhorse(type<TT>)) {
        return max_impl(src, type<TT -const -&>)
    } else {
        return <- max_impl(src, type<TT -const -&>)
    }
}

[skip_lock_check]
def max(var src : array<auto(TT)>) : TT -& -const {
    //! Finds the maximum element in an array
    static_if (typeinfo is_workhorse(type<TT>)) {
        return max_impl(src, type<TT -const -&>)
    } else {
        return <- max_impl(src, type<TT -const -&>)
    }
}

[skip_lock_check, unused_argument(tt)]
def private max_by_impl(var src; tt : auto(TT); key) : TT -& -const {
    var maxx : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            maxx := x
            first = false
        } elif (_::less(key(maxx), key(x))) {
            maxx := x
        }
    }
    return <- maxx
}

[skip_lock_check]
def max_by(var src : iterator<auto(TT)>; key) : TT -& -const {
    //! Finds the maximum element in an iterator by key
    static_if (typeinfo is_workhorse(type<TT>)) {
        return max_by_impl(src, type<TT -const -&>, key)
    } else {
        return <- max_by_impl(src, type<TT -const -&>, key)
    }
}

[skip_lock_check]
def max_by(var src : array<auto(TT)>; key) : TT -& -const {
    //! Finds the maximum element in an array by key
    static_if (typeinfo is_workhorse(type<TT>)) {
        return max_by_impl(src, type<TT -const -&>, key)
    } else {
        return <- max_by_impl(src, type<TT -const -&>, key)
    }
}

[skip_lock_check, unused_argument(tt)]
def private min_max_impl(var src; tt : auto(TT)) : tuple<TT -& -const, TT -& -const> {
    var minn : TT -& -const
    var maxx : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            minn := x
            maxx := x
            first = false
        } else {
            if (_::less(x, minn)) {
                minn := x
            } elif (_::less(maxx, x)) {
                maxx := x
            }
        }
    }
    return <- (minn, maxx)
}

[skip_lock_check]
def min_max(var src : iterator<auto(TT)>) : tuple<TT -& -const, TT -& -const> {
    //! Finds the minimum and maximum elements in an iterator
    return <- min_max_impl(src, type<TT -const -&>)
}

[skip_lock_check]
def min_max(var src : array<auto(TT)>) : tuple<TT -& -const, TT -& -const> {
    //! Finds the minimum and maximum elements in an array
    return <- min_max_impl(src, type<TT -const -&>)
}

[skip_lock_check, unused_argument(tt)]
def private min_max_by_impl(var src; tt : auto(TT); key) : tuple<TT -& -const, TT -& -const> {
    var minn : TT -& -const
    var maxx : TT -& -const
    var first : bool = true
    for (x in src) {
        if (first) {
            minn := x
            maxx := x
            first = false
        } else {
            if (_::less(key(x), key(minn))) {
                minn := x
            } elif (_::less(key(maxx), key(x))) {
                maxx := x
            }
        }
    }
    return <- (minn, maxx)
}

[skip_lock_check]
def min_max_by(var src : iterator<auto(TT)>; key) : tuple<TT -& -const, TT -& -const> {
    //! Finds the minimum and maximum elements in an iterator by key
    return <- min_max_by_impl(src, type<TT -const -&>, key)
}

[skip_lock_check]
def min_max_by(var src : array<auto(TT)>; key) : tuple<TT -& -const, TT -& -const> {
    //! Finds the minimum and maximum elements in an array by key
    return <- min_max_by_impl(src, type<TT -const -&>, key)
}

[skip_lock_check, unused_argument(tt)]
def private aggregate_impl(var src; tt : auto(TT); seed : auto(AGG); func : block<(acc : AGG -&, x : TT -&) : AGG -&>) : AGG -& -const {
    var acc : AGG -& := seed
    for (x in src) {
        static_if (typeinfo can_copy(x)) {
            acc = func(acc, x)
        } else {
            acc <- func(acc, x)
        }
    }
    return <- acc
}

[skip_lock_check]
def aggregate(var src : iterator<auto(TT)>; seed : auto(AGG); func : block<(acc : AGG -&, x : TT -&) : AGG -&>) : AGG -& -const {
    //! Aggregates elements in an iterator using a seed and a function
    static_if (typeinfo is_workhorse(seed)) {
        return aggregate_impl(src, type<TT -const -&>, seed, func)
    } else {
        return <- aggregate_impl(src, type<TT -const -&>, seed, func)
    }
}

[skip_lock_check]
def aggregate(var src : array<auto(TT)>; seed : auto(AGG); func : block<(acc : AGG -&, x : TT -&) : AGG -&>) : AGG -& -const {
    //! Aggregates elements in an array using a seed and a function
    static_if (typeinfo is_workhorse(seed)) {
        return aggregate_impl(src, type<TT -const -&>, seed, func)
    } else {
        return <- aggregate_impl(src, type<TT -const -&>, seed, func)
    }
}

[skip_lock_check, unused_argument(tt)]
def private sum_impl(var src; tt : auto(TT)) : TT -const -& {
    var total : TT -const -&
    for (x in src) {
        total += x
    }
    return <- total
}

[skip_lock_check]
def sum(var src : iterator<auto(TT)>) : TT -const -& {
    //! Sums elements in an iterator
    static_if (typeinfo is_workhorse(type<TT>)) {
        return sum_impl(src, type<TT -const -&>)
    } else {
        return <- sum_impl(src, type<TT -const -&>)
    }
}

[skip_lock_check]
def sum(var src : array<auto(TT)>) : TT -const -& {
    //! Sums elements in an array
    static_if (typeinfo is_workhorse(type<TT>)) {
        return sum_impl(src, type<TT -const -&>)
    } else {
        return <- sum_impl(src, type<TT -const -&>)
    }
}

[skip_lock_check, unused_argument(tt)]
def average_impl(var src; tt : auto(TT)) : TT -const -& {
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
def average(var src : iterator<auto(TT)>) : TT -const -& {
    //! Averages elements in an iterator
    static_if (typeinfo is_workhorse(type<TT>)) {
        return average_impl(src, type<TT -const -&>)
    } else {
        return <- average_impl(src, type<TT -const -&>)
    }
}

[skip_lock_check]
def average(var src : array<auto(TT)>) : TT -const -& {
    //! Averages elements in an array
    static_if (typeinfo is_workhorse(type<TT>)) {
        return average_impl(src, type<TT -const -&>)
    } else {
        return <- average_impl(src, type<TT -const -&>)
    }
}

[skip_lock_check, unused_argument(tt)]
def private min_max_average_impl(var src; tt : auto(TT)) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    var minn : TT -const -&
    var maxx : TT -const -&
    var total : TT -const -&
    var count : uint64
    var first : bool = true
    for (x in src) {
        if (first) {
            minn := x
            maxx := x
            total := x
            first = false
        } else {
            if (_::less(x, minn)) {
                minn := x
            } elif (_::less(maxx, x)) {
                maxx := x
            }
            total += x
        }
        count ++
    }
    if (count != 0ul) {
        static_if (typeinfo is_workhorse(type<TT>)) {
            total /= TT(count)
        } else {
            total /= count
        }
    }
    return <- (minn, maxx, total)
}

[skip_lock_check]
def min_max_average(var src : iterator<auto(TT)>) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    //! Finds the minimum, maximum, and average elements in an iterator
    return <- min_max_average_impl(src, type<TT -const -&>)
}

[skip_lock_check]
def min_max_average(var src : array<auto(TT)>) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    //! Finds the minimum, maximum, and average elements in an array
    return <- min_max_average_impl(src, type<TT -const -&>)
}

[skip_lock_check, unused_argument(tt)]
def private min_max_average_by_impl(var src; tt : auto(TT); key) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    var minn : TT -const -&
    var maxx : TT -const -&
    var total : TT -const -&
    var count : uint64
    var first : bool = true
    for (x in src) {
        if (first) {
            minn := x
            maxx := x
            total := x
            first = false
        } else {
            if (_::less(key(x), key(minn))) {
                minn := x
            } elif (_::less(key(maxx), key(x))) {
                maxx := x
            }
            total += x
        }
        count ++
    }
    if (count != 0ul) {
        static_if (typeinfo is_workhorse(type<TT>)) {
            total /= TT(count)
        } else {
            total /= count
        }
    }
    return <- (minn, maxx, total)
}

[skip_lock_check]
def min_max_average_by(var src : iterator<auto(TT)>; key) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    //! Finds the minimum, maximum, and average elements in an iterator by key
    return <- min_max_average_by_impl(src, type<TT -const -&>, key)
}

[skip_lock_check]
def min_max_average_by(var src : array<auto(TT)>; key) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    //! Finds the minimum, maximum, and average elements in an array by key
    return <- min_max_average_by_impl(src, type<TT -const -&>, key)
}

[skip_lock_check]
def join(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : iterator<typedecl(result(type<TA>, type<TB>)) -const -&> {
    //! Joins two iterators based on matching keys (inner join)
    var inscope tableb : table<typedecl(_::unique_key(keyb(type<TB>))); array<TB -const -&>>
    for (itb in srcb) {
        let k = _::unique_key(keyb(itb))
        unsafe(tableb[k]).push_clone(itb)
    }
    var buffer : array<typedecl(result(type<TA>, type<TB>)) -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (ita in srca) {
        let k = _::unique_key(keya(ita))
        tableb.get(k, $(arr){
            for (itb in arr) {
                buffer.push_clone(result(ita, itb))
            }
        })
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return buffer.move_to_sequence()
}

[skip_lock_check]
def group_join(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : iterator<typedecl(result(type<TA>, type<iterator<TB -const -&>>)) -const -&> {
    // we pass TA, and sequence of TB to 'result'
    var inscope tableb : table<typedecl(_::unique_key(keyb(type<TB>))); array<TB -const -&>>
    for (itb in srcb) {
        let k = _::unique_key(keyb(itb))
        unsafe(tableb[k]).push_clone(itb)
    }
    var buffer : array<typedecl(result(type<TA>, type<iterator<TB -const -&>>)) -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (ita in srca) {
        let k = _::unique_key(keya(ita))
        tableb.get(k, $(var arr){
            buffer.push_clone(result(ita, arr.to_sequence()))
        })
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return buffer.move_to_sequence()
}

[skip_lock_check]
def group_by(var source : iterator<auto(TT)>; key; element_selector; result_selector)  {
    //! Groups the elements of an iterator according to a specified key selector function
    var tab : table<typedecl(_::unique_key(key(type<TT>))); tuple<typedecl(key(type<TT>)), array<typedecl(element_selector(type<TT>)) -const -&>>>
    for (it in source) {
        let kIT = key(it)
        let k = _::unique_key(kIT)
        if (tab.key_exists(k)) {
            unsafe(tab[k])._1.push_clone(element_selector(it))
        } else {
            unsafe(tab[k]) = (kIT, [element_selector(it)])
        }
    }
    var buffer : array<typedecl(result_selector(type<typedecl(key(type<TT>))>, type<iterator<typedecl(element_selector(type<TT>)) -const -&>>)) -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in values(tab)) {
        buffer.push_clone(result_selector(it._0, it._1.to_sequence()))
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return buffer.move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private union_impl(var srca; var srcb; tt : auto(TT)) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    var inscope seen : table<typedecl(_::unique_key(type<TT>))>
    for (ita in srca) {
        let k = _::unique_key(ita)
        if (!seen.key_exists(k)) {
            buffer.push_clone(ita)
            seen.insert(k)
        }
    }
    for (itb in srcb) {
        let k = _::unique_key(itb)
        if (!seen.key_exists(k)) {
            buffer.push_clone(itb)
            seen.insert(k)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def union(var srca, srcb : array<auto(TT)>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two arrays
    return <- union_impl(srca, srcb, type<TT -const -&>)
}

[skip_lock_check]
def union(var srca, srcb : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators
    return <- union_impl(srca, srcb, type<TT -const -&>).move_to_sequence()
}

[skip_lock_check]
def union_to_array(var srca, srcb : iterator<auto(TT)>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators and returns an array
    return <- union_impl(srca, srcb, type<TT -const -&>)
}

[skip_lock_check]
def union_to_sequence(var srca, srcb : array<auto(TT)>) : iterator<TT -const -&> {
    //! Returns distinct elements from the concatenation of two arrays and returns an iterator
    return <- union_impl(srca, srcb, type<TT -const -&>).move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private union_by_impl(var srca; var srcb; tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators by key
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    var inscope seen : table<typedecl(_::unique_key(key(type<TT>)))>
    for (ita in srca) {
        let k = _::unique_key(key(ita))
        if (!seen.key_exists(k)) {
            buffer.push_clone(ita)
            seen.insert(k)
        }
    }
    for (itb in srcb) {
        let k = _::unique_key(key(itb))
        if (!seen.key_exists(k)) {
            buffer.push_clone(itb)
            seen.insert(k)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def union_by(var srca, srcb : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators by key
    return <- union_by_impl(srca, srcb, type<TT -const -&>, key).move_to_sequence()
}

[skip_lock_check]
def union_by(var srca, srcb : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two arrays by key
    return <- union_by_impl(srca, srcb, type<TT -const -&>, key)
}

[skip_lock_check]
def union_by_to_array(var srca, srcb : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators by key and returns an array
    return <- union_by_impl(srca, srcb, type<TT -const -&>, key)
}

[skip_lock_check]
def union_by_to_sequence(var srca, srcb : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns distinct elements from the concatenation of two arrays by key and returns an iterator
    return <- union_by_impl(srca, srcb, type<TT -const -&>, key).move_to_sequence()
}

[skip_lock_check]
def any(var src : array<auto(TT)>) : bool {
    //! Returns true if the array has at least one element
    return length(src) != 0
}

[skip_lock_check]
def any(var src : iterator<auto(TT)>) : bool {
    //! Returns true if the iterator has at least one element
    for (it in src) {
        return true
    }
    return false
}

[skip_lock_check, unused_argument(tt)]
def private any_impl(var src; tt : auto(TT); predicate : block<(arg : auto(TT) -&) : bool>) : bool {
    for (it in src) {
        if (predicate(it)) {
            return true
        }
    }
    return false
}

[skip_lock_check]
def any(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : bool {
    //! Returns true if any element in the iterator satisfies the predicate
    return any_impl(src, type<TT -const -&>, predicate)
}

[skip_lock_check]
def any(var src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : bool {
    //! Returns true if any element in the array satisfies the predicate
    return any_impl(src, type<TT -const -&>, predicate)
}

[skip_lock_check, unused_argument(tt)]
def private all_impl(var src; tt : auto(TT); predicate : block<(arg : auto(TT) -&) : bool>) : bool {
    for (it in src) {
        if (!predicate(it)) {
            return false
        }
    }
    return true
}

[skip_lock_check]
def all(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : bool {
    //! Returns true if all elements in the iterator satisfy the predicate
    return all_impl(src, type<TT -const -&>, predicate)
}

[skip_lock_check]
def all(var src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : bool {
    //! Returns true if all elements in the array satisfy the predicate
    return all_impl(src, type<TT -const -&>, predicate)
}

[skip_lock_check, unused_argument(tt)]
def private except_impl(var src, exclude; tt : auto(TT)) : array<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator
    var inscope seen : table<typedecl(_::unique_key(type<TT>))>
    for (it in exclude) {
        seen.insert(_::unique_key(it))
    }
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in src) {
        if (!seen.key_exists(_::unique_key(it))) {
            buffer.push_clone(it)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def except(var src, exclude : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator
    return <- except_impl(src, exclude, type<TT -const -&>).move_to_sequence()
}

[skip_lock_check]
def except(var src, exclude : array<auto(TT)>) : array<TT -const -&> {
    //! Returns elements from the first array that are not in the second array
    return <- except_impl(src, exclude, type<TT -const -&>)
}

[skip_lock_check]
def except_to_array(var src, exclude : iterator<auto(TT)>) : array<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator and returns an array
    return <- except_impl(src, exclude, type<TT -const -&>)
}

[skip_lock_check]
def except_to_sequence(var src, exclude : array<auto(TT)>) : iterator<TT -const -&> {
    //! Returns elements from the first array that are not in the second array and returns an iterator
    return <- except_impl(src, exclude, type<TT -const -&>).move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private except_by_impl(var src, exclude; tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator by key
    var inscope seen : table<typedecl(_::unique_key(key(type<TT>)))>
    for (it in exclude) {
        seen.insert(_::unique_key(key(it)))
    }
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in src) {
        if (!seen.key_exists(_::unique_key(key(it)))) {
            buffer.push_clone(it)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def except_by(var src, exclude : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator by key
    return <- except_by_impl(src, exclude, type<TT -const -&>, key).move_to_sequence()
}

[skip_lock_check]
def except_by(var src, exclude : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements from the first array that are not in the second array by key
    return <- except_by_impl(src, exclude, type<TT -const -&>, key)
}

[skip_lock_check]
def except_by_to_array(var src, exclude : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator by key and returns an array
    return <- except_by_impl(src, exclude, type<TT -const -&>, key)
}

[skip_lock_check]
def except_by_to_sequence(var src, exclude : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns elements from the first array that are not in the second array by key and returns an iterator
    return <- except_by_impl(src, exclude, type<TT -const -&>, key).move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private intersect_impl(var srca, srcb; tt : auto(TT)) : array<TT -const -&> {
    //! Returns elements that are present in both iterators
    var inscope seen : table<typedecl(_::unique_key(type<TT>))>
    for (it in srca) {
        seen.insert(_::unique_key(it))
    }
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in srcb) {
        if (seen.key_exists(_::unique_key(it))) {
            buffer.push_clone(it)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def intersect(var srca, srcb : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns elements that are present in both iterators
    return <- intersect_impl(srca, srcb, type<TT -const -&>).move_to_sequence()
}

[skip_lock_check]
def intersect(var srca, srcb : array<auto(TT)>) : array<TT -const -&> {
    //! Returns elements that are present in both arrays
    return <- intersect_impl(srca, srcb, type<TT -const -&>)
}

[skip_lock_check]
def intersect_to_array(var srca, srcb : iterator<auto(TT)>) : array<TT -const -&> {
    //! Returns elements that are present in both iterators and returns an array
    return <- intersect_impl(srca, srcb, type<TT -const -&>)
}

[skip_lock_check]
def intersect_to_sequence(var srca, srcb : array<auto(TT)>) : iterator<TT -const -&> {
    //! Returns elements that are present in both arrays and returns an iterator
    return <- intersect_impl(srca, srcb, type<TT -const -&>).move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private intersect_by_impl(var srca, srcb; tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements that are present in both iterators by key
    var inscope seen : table<typedecl(_::unique_key(key(type<TT>)))>
    for (it in srca) {
        seen.insert(_::unique_key(key(it)))
    }
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in srcb) {
        if (seen.key_exists(_::unique_key(key(it)))) {
            buffer.push_clone(it)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def intersect_by(var srca, srcb : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns elements that are present in both iterators by key
    return <- intersect_by_impl(srca, srcb, type<TT -const -&>, key).move_to_sequence()
}

[skip_lock_check]
def intersect_by(var srca, srcb : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements that are present in both arrays by key
    return <- intersect_by_impl(srca, srcb, type<TT -const -&>, key)
}

[skip_lock_check]
def intersect_by_to_array(var srca, srcb : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements that are present in both iterators by key and returns an array
    return <- intersect_by_impl(srca, srcb, type<TT -const -&>, key)
}

[skip_lock_check]
def intersect_by_to_sequence(var srca, srcb : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns elements that are present in both arrays by key and returns an iterator
    return <- intersect_by_impl(srca, srcb, type<TT -const -&>, key).move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private contains_impl(var src; tt : auto(TT); element : TT -&) : bool {
    for (it in src) {
        if (it == element) {
            return true
        }
    }
    return false
}

[skip_lock_check]
def contains(var src : iterator<auto(TT)>; element : TT -&) : bool {
    //! Returns true if the element is present in the iterator
    return contains_impl(src, type<TT -const -&>, element)
}

[skip_lock_check]
def contains(var src : array<auto(TT)>; element : TT -&) : bool {
    //! Returns true if the element is present in the array
    return contains_impl(src, type<TT -const -&>, element)
}

[skip_lock_check, unused_argument(typ)]
def empty(var typ : auto(TT)) : iterator<TT -const -&> {
    //! Returns an empty iterator of the specified type
    return <- generator<TT -const -&>() <| $ {
        return false
    }
}

[skip_lock_check]
def default_empty(var src : iterator<auto(TT)>) : iterator<TT -const -&> {
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

[skip_lock_check]
def range_sequence(start : int; count : int) : iterator<int> {
    //! Generates a sequence of integers within a specified range
    return generator<int>() <| $ {
        for (i in start..start + count) {
            yield i
        }
        return false
    }
}

[skip_lock_check]
def repeat(element : auto(TT); count : int) : iterator<TT -const -&> {
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

[skip_lock_check]
def sequence_equal(var first : iterator<auto(TT)>; var second : iterator<auto(TT)>) : bool {
    //! Checks if two sequences are equal
    for (f, s in first, second) {
        if (f != s) {
            return false
        }
    }
    return true
}

[skip_lock_check]
def sequence_equal_by(var first : iterator<auto(TT)>; var second : iterator<auto(TT)>; key) : bool {
    //! Checks if two sequences are equal by key
    for (f, s in first, second) {
        if (key(f) != key(s)) {
            return false
        }
    }
    return true
}

[skip_lock_check]
def element_at(var src : iterator<auto(TT)>; index : int) : TT -const -& {
    //! Returns the element at the specified index
    panic("element index out of range") if (index < 0)
    var i = 0
    for (it in src) {
        if (i == index) {
            static_if (typeinfo can_copy(it)) {
                return it
            } else {
                return <- clone_to_move(it)
            }
        }
        i ++
    }
    panic("element index out of range")
    var t : TT -const -&
    return <- t
}

[skip_lock_check]
def element_at_or_default(var src : iterator<auto(TT)>; index : int) : TT -const -& {
    //! Returns the element at the specified index, or a default value if the index is out of range
    if (index < 0) {
        var t : TT -const -&
        return <- t
    }
    var i = 0
    for (it in src) {
        if (i == index) {
            static_if (typeinfo can_copy(it)) {
                return it
            } else {
                return <- clone_to_move(it)
            }
        }
        i ++
    }
    var t : TT -const -&
    return <- t
}

[skip_lock_check]
def first(var src : iterator<auto(TT)>) : TT -const -& {
    //! Returns the first element of an iterator
    for (it in src) {
        static_if (typeinfo can_copy(it)) {
            return it
        } else {
            return <- clone_to_move(it)
        }
    }
    panic("sequence contains no elements")
    var t : TT -const -&
    return <- t
}

[skip_lock_check]
def first_or_default(var src : iterator<auto(TT)>; defaultValue : TT -&) : TT -const -& {
    //! Returns the first element of an iterator, or a default value if the iterator is empty
    for (it in src) {
        static_if (typeinfo can_copy(it)) {
            return it
        } else {
            return <- clone_to_move(it)
        }
    }
    static_if (typeinfo can_copy(defaultValue)) {
        return defaultValue
    } else {
        return <- clone_to_move(defaultValue)
    }
}

[skip_lock_check]
def last(var src : iterator<auto(TT)>) : TT -const -& {
    //! Returns the last element of an iterator
    var found = false
    var last : TT -const -&
    for (it in src) {
        found = true
        last := it
    }
    if (!found) {
        panic("sequence contains no elements")
    }
    return <- last
}

[skip_lock_check]
def last_or_default(var src : iterator<auto(TT)>; defaultValue : TT -&) : TT -const -& {
    //! Returns the last element of an iterator, or a default value if the iterator is empty
    var found = false
    var last : TT -const -&
    for (it in src) {
        found = true
        last := it
    }
    if (!found) {
        static_if (typeinfo can_copy(defaultValue)) {
            return defaultValue
        } else {
            return <- clone_to_move(defaultValue)
        }
    }
    return <- last
}

[skip_lock_check]
def single(var src : iterator<auto(TT)>) : TT -const -& {
    //! Returns the only element of an iterator, and throws if there is not exactly one element
    var found = false
    var single : TT -const -&
    for (it in src) {
        if (found) {
            panic("sequence contains more than one element")
        }
        found = true
        single := it
    }
    if (!found) {
        panic("sequence contains no elements")
    }
    return <- single
}

[skip_lock_check]
def single_or_default(var src : iterator<auto(TT)>; defaultValue : TT -&) : TT -const -& {
    //! Returns the only element of an iterator, or a default value if there is not exactly one element
    var found = false
    var single : TT -const -&
    for (it in src) {
        if (found) {
            static_if (typeinfo can_copy(defaultValue)) {
                return defaultValue
            } else {
                return <- clone_to_move(defaultValue)
            }
        }
        found = true
        single := it
    }
    if (!found) {
        static_if (typeinfo can_copy(defaultValue)) {
            return defaultValue
        } else {
            return <- clone_to_move(defaultValue)
        }
    }
    return <- single
}

[skip_lock_check]
def private prepend_impl(var arr; tt : auto(TT); reserveSize : int; value : TT -&) : array<TT -const -&> {
    //! Prepends a value to the beginning of an array
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    if (reserveSize > 0) {
        buffer.reserve(reserveSize)
    }
    buffer.push_clone(value)
    for (x in arr) {
        buffer.push_clone(x)
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def prepend(var arr : array<auto(TT)>; value : TT -&) : array<TT -const -&> {
    //! Prepends a value to the beginning of an array
    return <- prepend_impl(arr, type<TT -const -&>, length(arr) + 1, value)
}

[skip_lock_check]
def prepend_to_array(var it : iterator<auto(TT)>; value : TT -&) : array<TT -const -&> {
    //! Prepends a value to the beginning of an iterator and returns an array
    return <- prepend_impl(it, type<TT -const -&>, 0, value)
}

[skip_lock_check]
def prepend_to_sequence(var arr : array<auto(TT)>; value : TT -&) : iterator<TT -const -&> {
    //! Prepends a value to the beginning of an array and returns an iterator
    return <- prepend_impl(arr, type<TT -const -&>, length(arr) + 1, value).move_to_sequence()
}

[skip_lock_check]
def prepend(var it : iterator<auto(TT)>; value : TT -&) : iterator<TT -const -&> {
    //! Prepends a value to the beginning of an iterator
    return generator<TT -const -&> capture(<- it, := value) () <| $ {
        static_if (typeinfo can_copy(value)) {
            yield value
        } else {
            yield <- clone_to_move(value)
        }
        for (x in it) {
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
def prepend_inplace(var arr : array<auto(TT)>; value : TT -&) {
    //! Prepends a value to the beginning of an array in place
    arr.push_clone(value, 0)
}

[skip_lock_check, unused_argument(tt)]
def private append_impl(var arr; tt : auto(TT); reserveSize : int; value : TT -&) : array<TT -const -&> {
    //! Appends a value to the end of an array
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    if (reserveSize > 0) {
        buffer.reserve(reserveSize)
    }
    for (x in arr) {
        buffer.push_clone(x)
    }
    buffer.push_clone(value)
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def append(var arr : array<auto(TT)>; value : TT -&) : array<TT -const -&> {
    //! Appends a value to the end of an array
    return <- append_impl(arr, type<TT -const -&>, length(arr) + 1, value)
}

[skip_lock_check]
def append_to_array(var it : iterator<auto(TT)>; value : TT -&) : array<TT -const -&> {
    //! Appends a value to the end of an iterator and returns an array
    return <- append_impl(it, type<TT -const -&>, 0, value)
}

[skip_lock_check]
def append_to_sequence(var arr : array<auto(TT)>; value : TT -&) : iterator<TT -const -&> {
    //! Appends a value to the end of an array and returns an iterator
    return <- append_impl(arr, type<TT -const -&>, length(arr) + 1, value).move_to_sequence()
}

[skip_lock_check]
def append(var it : iterator<auto(TT)>; value : TT -&) : iterator<TT -const -&> {
    //! Appends a value to the end of an iterator
    return generator<TT -const -&> capture(<- it, := value) () <| $ {
        for (x in it) {
            static_if (typeinfo can_copy(x)) {
                yield x
            } else {
                yield <- clone_to_move(x)
            }
        }
        static_if (typeinfo can_copy(value)) {
            yield value
        } else {
            yield <- clone_to_move(value)
        }
        return false
    }
}

[skip_lock_check]
def append_inplace(var arr : array<auto(TT)>; value : TT -&) {
    //! Appends a value to the end of an array in place
    arr.push_clone(value)
}

[skip_lock_check, unused_argument(tt)]
def private linq_select_indexed(var src; tt : auto(TT)) : array<tuple<int, TT -const -&>> {
    //! Projects each element of an iterator into a new form with its index
    var buffer : array<tuple<int, TT -const -&>>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    for (it, index in src, count()) {
        buffer.emplace((index, it))
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    return <- buffer
}

[skip_lock_check]
def select(var src : iterator<auto(TT)>) : iterator<tuple<int, TT -const -&>> {
    //! Projects each element of an iterator into a new form
    return <- linq_select_indexed(src, type<TT -const -&>).move_to_sequence()
}

[skip_lock_check]
def select(var src : array<auto(TT)>) : array<tuple<int, TT -const -&>> {
    //! Projects each element of an array into a new form
    return <- linq_select_indexed(src, type<TT -const -&>)
}

[skip_lock_check]
def select_to_array(var src : iterator<auto(TT)>) : array<tuple<int, TT -const -&>> {
    //! Projects each element of an iterator into a new form and returns an array
    return <- linq_select_indexed(src, type<TT -const -&>)
}

[skip_lock_check]
def select_to_sequence(var src : array<auto(TT)>) : iterator<tuple<int, TT -const -&>> {
    //! Projects each element of an array into a new form and returns an iterator
    return <- linq_select_indexed(src, type<TT -const -&>).move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private linq_select(var src; tt : auto(TT); len : int; selector) : array<typedecl(selector(type<TT>)) -const -&> {
    //! Projects each element of an iterator into a new form using a selector function
    var buffer : array<typedecl(selector(type<TT>)) -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    buffer.reserve(len)
    for (it in src) {
        buffer.push_clone(selector(it))
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def select(var src : iterator<auto(TT)>; result_selector) : iterator<typedecl(result_selector(type<TT>)) -const -&> {
    return <- linq_select(src, type<TT -const -&>, 0, result_selector).move_to_sequence()
}

[skip_lock_check]
def select(var src : array<auto(TT)>; result_selector) : array<typedecl(result_selector(type<TT>)) -const -&> {
    return <- linq_select(src, type<TT -const -&>, length(src), result_selector)
}

[skip_lock_check]
def select_to_array(var src : iterator<auto(TT)>; result_selector) : array<typedecl(result_selector(type<TT>)) -const -&> {
    return <- linq_select(src, type<TT -const -&>, 0, result_selector)
}

[skip_lock_check]
def select_to_sequence(var src : array<auto(TT)>; result_selector) : iterator<typedecl(result_selector(type<TT>)) -const -&> {
    return <- linq_select(src, type<TT -const -&>, length(src), result_selector).move_to_sequence()
}

[skip_lock_check, unused_argument(tt)]
def private chunk_impl(var src; tt : auto(TT); size : int) : array<array<TT -const -&>> {
    //! Splits an array into chunks of a specified size
    panic("chunk size must be greater than 0") if (size <= 0)
    var buffer : array<array<TT -const -&>>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    var inscope chunk : array<TT -const -&>
    static_if (typeinfo need_lock_check(chunk)) {
        unsafe(set_verify_array_locks(chunk, false))
    }
    for (it in src) {
        chunk.push_clone(it)
        if (chunk.length() == size) {
            buffer.emplace(chunk)
        }
    }
    if (chunk.length() > 0) {
        buffer.emplace(chunk)
    }
    static_if (typeinfo need_lock_check(chunk)) {
        unsafe(set_verify_array_locks(chunk, true))
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def chunk(var src : array<auto(TT)>; size : int) : array<array<TT -const -&>> {
    //! Splits an array into chunks of a specified size
    return <- chunk_impl(src, type<TT -const -&>, size)
}

[skip_lock_check]
def chunk_to_array(var src : iterator<auto(TT)>; size : int) : array<array<TT -const -&>> {
    //! Splits an iterator into chunks of a specified size and returns an array
    return <- chunk_impl(src, type<TT -const -&>, size)
}

[skip_lock_check]
def chunk_to_sequence(var src : array<auto(TT)>; size : int) : iterator<array<TT -const -&>> {
    //! Splits an iterator into chunks of a specified size and returns a sequence
    return <- chunk_impl(src, type<TT -const -&>, size).move_to_sequence()
}

[skip_lock_check]
def chunk(var src : iterator<auto(TT)>; size : int) : iterator<array<TT -const -&>> {
    //! Splits an iterator into chunks of a specified size
    return <- chunk_impl(src, type<TT -const -&>, size).move_to_sequence()
}

[unused_argument(src)]
def private iter_type(var src : iterator<auto(TT)>) : TT -const -& {
    //! this type trait is used to extract the type of elements in an iterator
    var t : TT -const -&
    return <- t
}

[skip_lock_check]
def select_many(var src : iterator<auto(TT)>; result_selector) : iterator<typedecl(result_selector(iter_type(type<TT>))) -const -&> {
    //! Projects each element of an iterator to an iterator and flattens the resulting iterators into one iterator
    var buffer : array<typedecl(result_selector(iter_type(type<TT>))) -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in src) {
        for (innerIt in it) {
            buffer.push_clone(result_selector(innerIt))
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return buffer.move_to_sequence()
}

[skip_lock_check]
def select_many(var src : iterator<auto(TT)>; collection_selector; result_selector) : iterator<typedecl(result_selector(iter_type(collection_selector(type<TT>)))) -const -&> {
    //! Projects each element of an iterator to an iterator and flattens the resulting iterators into one iterator
    var buffer : array<typedecl(result_selector(iter_type(collection_selector(type<TT>)))) -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in src) {
        for (innerIt in collection_selector(it)) {
            buffer.push_clone(result_selector(innerIt))
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return buffer.move_to_sequence()
}

[skip_lock_check, unused_argument(tt), unused_argument(uu)]
def private zip_impl(var a; tt : auto(TT); var b; var uu : auto(UU)) : array<tuple<TT -const -&, UU -const -&>> {
    //! Merges two iterators into an array of tuples
    var buffer : array<tuple<TT -const -&, UU -const -&>>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (itA, itB in a, b) {
        buffer.emplace((itA, itB))
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def zip(var a : iterator<auto(TT)>; var b : iterator<auto(UU)>) : iterator<tuple<TT -const -&, UU -const -&>> {
    //! Merges two iterators into an iterator of tuples
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>).move_to_sequence()
}

[skip_lock_check]
def zip(var a : array<auto(TT)>; var b : array<auto(UU)>) : array<tuple<TT -const -&, UU -const -&>> {
    //! Merges two arrays into an array of tuples
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>)
}

[skip_lock_check]
def zip_to_array(var a : iterator<auto(TT)>; var b : iterator<auto(UU)>) : array<tuple<TT -const -&, UU -const -&>> {
    //! Merges two iterators into an array of tuples
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>)
}

[skip_lock_check]
def zip_to_sequence(var a : array<auto(TT)>; var b : array<auto(UU)>) : iterator<tuple<TT -const -&, UU -const -&>> {
    //! Merges two iterators into an iterator of tuples
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>).move_to_sequence()
}

[skip_lock_check, unused_argument(tt), unused_argument(uu)]
def private zip_impl(var a; tt : auto(TT); var b; uu : auto(UU); result_selector : block<(l : TT -&; r : UU -&) : auto>) : array<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    var buffer : array<typedecl(result_selector(type<TT>, type<UU>)) -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (itA, itB in a, b) {
        buffer.push_clone(result_selector(itA, itB))
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check]
def zip(var a : iterator<auto(TT)>; var b : iterator<auto(UU)>; result_selector : block<(l : TT -&; r : UU -&) : auto>) : iterator<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    //! Merges two iterators into an iterator by applying a specified function
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>, result_selector).move_to_sequence()
}

[skip_lock_check]
def zip(var a : array<auto(TT)>; var b : array<auto(UU)>; result_selector : block<(l : TT -&; r : UU -&) : auto>) : array<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    //! Merges two arrays into an array by applying a specified function
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>, result_selector)
}

[skip_lock_check]
def zip_to_array(var a : iterator<auto(TT)>; var b : iterator<auto(UU)>; result_selector : block<(l : TT -&; r : UU -&) : auto>) : array<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    //! Merges two iterators into an array by applying a specified function
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>, result_selector)
}

[skip_lock_check]
def zip_to_sequence(var a : array<auto(TT)>; var b : array<auto(UU)>; result_selector : block<(l : TT -&; r : UU -&) : auto>) : iterator<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    //! Merges two arrays into an iterator by applying a specified function
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>, result_selector).move_to_sequence()
}
