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
     reverse, reverse_inplace, reverse_to_array - Reverses the order of elements in an iterable
     order, order_inplace, order_to_array - Sorts the elements of an iterable
     order_descending,  order_descending_inplace, order_descending_to_array - Sorts the elements of an iterable in descending order
     order_by, order_by_inplace, order_by_to_array - Sorts the elements of an iterable
     order_by_descending,  order_by_descending_inplace, order_by_descending_to_array - Sorts the elements of an iterable in descending order
Set operations (*)
     distinct, distinct_inplace, distinct_to_array, distinct_by_to_array - Returns distinct elements from an iterable
     distinct_by, distinct_by_inplace, distinct_by_to_array - Returns distinct elements from an iterable (or by key)
     union, union_to_array - Returns the union of two iterables
     union_by, union_by_to_array - Returns the union of two iterables (or by key)
     except, except_to_array - Returns elements from the first iterable that are not in the second iterable
     except_by, except_by_to_array - Returns elements from the first iterable that are not in the second iterable (or by key)
     intersect, intersect_to_array - Returns elements that are present in both iterables
     intersect_by, intersect_by_to_array - Returns elements that are present in both iterables (or by key)
     unique, unique_inplace, unique_to_array - Returns unique elements from a sorted iterable (by compare function)
     unique_by, unique_by_inplace, unique_by_to_array - Returns unique elements from a sorted iterable (or by key)
Concatenation operations (*)
     append, append_to_array, append_inplace - Appends an element to the end of an iterable
     prepend, prepend_to_array, prepend_inplace - Prepends an element to the beginning of an iterable
     concat, concat_to_array, concat_inplace - Concatenates two iterables
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
     where_, where_to_array - Filters elements in an iterable
Partitioning data (*)
     skip, skip_to_array, skip_impl, skip_inplace - Skips the first `n` elements of an iterable
     skip_while, skip_while_to_array - Skips all elements of an iterable while the predicate is true
     take, take_to_array, take_impl, take_inplace - Takes the first `n` elements of an iterable or range of elements in the iterable
     take_while, take_while_to_array - Takes elements from an iterable while the predicate is true
     chunk, chunk_to_array - Splits an iterable into chunks of a specified size
Join and group operations (*)
     join, join_to_array - Joins two iterables based on a key (inner join)
     group_join, group_join_to_array - Joins two iterables based on a key, passing a sequence of the second iterable to the result (group join)
     group_by, group_by_to_array - Groups elements in an iterable by a key
Querying data (*)
     any - Returns true if the iterable has at least one element (or at least one element which satisfies the predicate)
     all - Returns true if all elements in the iterable satisfy the predicate
     contains - Returns true if the element is present in the iterable
Element operations (*)
     element_at - Returns the element at the specified index
     element_at_or_default - Returns the element at the specified index, or a default value if the index is out of range
     first - Returns the first element in an iterable
     first_or_default - Returns the first element in an iterable, or a default value if the iterable is empty
     last - Returns the last element in an iterable
     last_or_default - Returns the last element in an iterable, or a default value if the iterable is empty
     single - Returns the only element of an iterable, and throws if there is not exactly one element
     single_or_default - Returns the only element of an iterable, or a default value if there is not exactly one element
Transform operations (*)
     select, select_to_array - Projects each element of an iterable into a new form
     select_many, select_many_to_array - Projects each element of an iterable to an iterable and flattens the resulting iterables into one iterable
     zip, zip_to_array - Merges iterables into an iterable of tuples
Converting sequence (*)
     to_table - Converts an iterable to a table
     to_sequence - Converts an array to an iterable
     to_sequence_move - Moves an array to an iterable
Comparators and keys (*)
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
    return to_sequence_move(b)
}

[skip_lock_check]
def to_sequence_move(var a : array<auto(TT)>) : iterator<TT -const -&> {
    //! Converts an array to an iterator, captures input
    return generator<TT -const -&> capture(<- a) () <| $ {
        for (it in a) {
            yield <- it
        }
        delete a
        return false
    }
}

[skip_lock_check, unused_argument(tt)]
def private to_table_impl(var a; tt : auto(TT); key : block<(v : TT -&) : auto>; elementSelector : block<(v : TT -&) : auto>) : table<typedecl(_::unique_key(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&> {
    var result : table<typedecl(_::unique_key(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&>
    for (it in a) {
        let k = key(it)
        unsafe(result[_::unique_key(k)]) := elementSelector(it)
    }
    return <- result
}

[skip_lock_check, unused_argument(tt)]
def private to_table_impl_const(a : auto(ARGT); tt : auto(TT); key : block<(v : TT -&) : auto>; elementSelector : block<(v : TT -&) : auto>) : table<typedecl(_::unique_key(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&> {
    return <- to_table_impl(unsafe(reinterpret<ARGT -const>(a)), type<TT -const -&>, key, elementSelector)
}

[skip_lock_check]
def to_table(var a : iterator<auto(TT)>; key : block<(v : TT -&) : auto>; elementSelector : block<(v : TT -&) : auto>) : table<typedecl(_::unique_key(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&> {
    //! Converts an iterator to a table
    return <- to_table_impl(a, type<TT -const -&>, key, elementSelector)
}

[skip_lock_check]
def to_table(a : array<auto(TT)>; key : block<(v : TT -&) : auto>; elementSelector : block<(v : TT -&) : auto>) : table<typedecl(_::unique_key(type<TT>)) -const -&, typedecl(elementSelector(type<TT>)) -const -&> {
    //! Converts an array to a table
    return <- to_table_impl_const(a, type<TT -const -&>, key, elementSelector)
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

[skip_lock_check, unused_argument(tt)]
def private concat_impl_const(a : auto(ARGT); b : auto(ARGU); tt : auto(TT); reserveSize : int) : array<TT -const -&> {
    return <- concat_impl(unsafe(reinterpret<ARGT -const>(a)), unsafe(reinterpret<ARGU -const>(b)), type<TT -const -&>, reserveSize)
}

[skip_lock_check]
def concat(a, b : array<auto(TT)>) : array<TT -const -&> {
    //! Concatenates two arrays
    return <- concat_impl_const(a, b, type<TT -const -&>, length(a) + length(b))
}

[skip_lock_check]
def concat_to_array(var a, b : iterator<auto(TT)>) : array<TT -const -&> {
    //! Concatenates two iterators and returns an array
    return <- concat_impl(a, b, type<TT -const -&>, 0)
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
        _builtin_verify_locks(a, "array can't be concatenated in place because it contains locked elements")
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
        _builtin_verify_locks(buffer, "array can't be reversed because it contains locked elements")
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
def reverse(a : array<auto(TT)>) : array<TT -const -&> {
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
def order_inplace(var buffer : array<auto(TT)>) {
    //! Sorts an array in place
    sort(buffer, $(a, b) => _::unique_key(a) < _::unique_key(b))
}

[skip_lock_check]
def order(var a : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Sorts an iterator
    var arr <- to_array(a)
    sort(arr, $(a, b) => _::unique_key(a) < _::unique_key(b))
    return to_sequence_move(arr)
}

[skip_lock_check]
def order(arr : array<auto(TT)>) : array<TT -const -&> {
    //! Sorts an array
    var brr := arr
    sort(brr, $(a, b) => _::unique_key(a) < _::unique_key(b))
    return <- brr
}

[skip_lock_check]
def order_to_array(var a : iterator<auto(TT)>) : array<TT -const -&> {
    //! Sorts an iterator and returns an array
    var arr <- to_array(a)
    sort(arr, $(a, b) => _::unique_key(a) < _::unique_key(b))
    return <- arr
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
    return to_sequence_move(arr)
}

[skip_lock_check]
def order(a : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : array<TT -const -&> {
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
def order_descending_inplace(var buffer : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) {
    //! Sorts an array in descending order in place
    sort(buffer, $(v1, v2) => fun(v2, v1))
}

[skip_lock_check]
def order_descending(var a : iterator<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : iterator<TT -const -&> {
    //! Sorts an iterator in descending order
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => fun(v2, v1))
    return to_sequence_move(arr)
}

[skip_lock_check]
def order_descending(a : array<auto(TT)>; fun : block<(v1 : TT -&, v2 : TT -&) : bool>) : array<TT -const -&> {
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

def less(a, b) : bool {
    //! Compares two values, returns true if first is less than second
    return a < b
}

def less(a, b : tuple<auto(TT)>) : bool {
    //! Compares two tuples, returns true if first is less than second
    return a._0 < b._0
}

def less(a, b : tuple<auto(TT), auto(UU)>) : bool {
    //! Compares two tuples, returns true if first is less than second
    return true if (a._0 < b._0)
    return false if (a._0 > b._0)
    return a._1 < b._1
}

def less(a, b : tuple<auto(TT), auto(UU), auto(VV)>) : bool {
    //! Compares two tuples, returns true if first is less than second
    return true if (a._0 < b._0)
    return false if (a._0 > b._0)
    return true if (a._1 < b._1)
    return false if (a._1 > b._1)
    return a._2 < b._2
}

def less(a, b : tuple<auto(TT), auto(UU), auto(VV), auto(WW)>) : bool {
    //! Compares two tuples, returns true if first is less than second
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
    return to_sequence_move(arr)
}

[skip_lock_check]
def order_by(a : array<auto(TT)>; key) : array<TT -const -&> {
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
def order_by_descending_inplace(var buffer : array<auto(TT)>; key) {
    //! Sorts an array in descending order in place
    sort(buffer, $(v1, v2) => _::less(key(v2), key(v1)))
}

[skip_lock_check]
def order_by_descending(var a : iterator<auto(TT)>; key) : iterator<TT -const -&> {
    //! Sorts an iterator in descending order
    var arr <- to_array(a)
    sort(arr, $(v1, v2) => _::less(key(v2), key(v1)))
    return to_sequence_move(arr)
}

[skip_lock_check]
def order_by_descending(a : array<auto(TT)>; key) : array<TT -const -&> {
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

def unique_key(a) {
    //! generates unique key of workhorse type for the value
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
def private distinct_impl_const(a : auto(ARGT); tt : auto(TT)) : array<TT -const -&> {
    return <- distinct_impl(unsafe(reinterpret<ARGT -const>(a)), type<TT -const -&>)
}

[skip_lock_check]
def distinct(a : array<auto(TT)>) : array<TT -const -&> {
    //! Returns distinct elements from an array
    return <- distinct_impl_const(a, type<TT -const -&>)
}

[skip_lock_check]
def distinct_to_array(var a : iterator<auto(TT)>) : array<TT -const -&> {
    //! Returns distinct elements from an iterator and returns an array
    return <- distinct_impl(a, type<TT -const -&>)
}

[skip_lock_check]
def distinct_inplace(var a : array<auto(TT)>) {
    //! Returns distinct elements from an array in place
    var to_index = 0
    var inscope seen : table<typedecl(_::unique_key(type<TT>))>
    static_if (typeinfo need_lock_check(a)) {
        _builtin_verify_locks(a, "array can't be made distinct in place because it contains locked elements")
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

[skip_lock_check, unused_argument(tt)]
def private distinct_by_impl_const(a : auto(ARGT); tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    return <- distinct_by_impl(unsafe(reinterpret<ARGT -const>(a)), type<TT -const -&>, key)
}

[skip_lock_check]
def distinct_by(a : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from an array based on a key
    return <- distinct_by_impl_const(a, type<TT -const -&>, key)
}

[skip_lock_check]
def distinct_by_to_array(var a : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from an iterator based on a key and returns an array
    return <- distinct_by_impl(a, type<TT -const -&>, key)
}

[skip_lock_check]
def distinct_by(var a : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns distinct elements from an iterator based on a key
    // TODO: specialize with const value vs var value of key, which can be captured.
    return <- distinct_by_impl(a, type<TT -const -&>, key).to_sequence_move()
}

[skip_lock_check]
def distinct_by_inplace(var a : array<auto(TT)>; key : block<(arg : TT -&) : auto>) {
    //! Returns distinct elements from an array based on a key in place
    var to_index = 0
    var inscope seen : table<typedecl(_::unique_key(key(type<TT>)))>
    static_if (typeinfo need_lock_check(a)) {
        _builtin_verify_locks(a, "array can't be made distinct in place because it contains locked elements")
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
def unique(a : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! sort and remove duplicate elements from an iterator
    var inscope buffer <- to_array(a)
    unique_inplace(buffer)
    return to_sequence_move(buffer)
}

[skip_lock_check]
def unique(a : array<auto(TT)>) : array<TT -const -&> {
    //! sort and remove duplicate elements from an array
    var b := a
    unique_inplace(b)
    return <- b
}

[skip_lock_check]
def unique_to_array(a : iterator<auto(TT)>) : array<TT -const -&> {
    //! sort and remove duplicate elements from an iterator and returns an array
    var inscope buffer <- to_array(a)
    unique_inplace(buffer)
    return <- buffer
}

[skip_lock_check]
def unique_inplace(var a : array<auto(TT)>) {
    //! remove duplicate elements from sorted array in place
    if (length(a) <= 1) {
        return
    }
    static_if (typeinfo need_lock_check(a)) {
        _builtin_verify_locks(a, "array can't be made unique in place because it contains locked elements")
        set_verify_array_locks(a, false)
    }
    var write_index = 1
    for (from_index in 1 .. length(a)) {
        if (_::unique_key(a[from_index]) != _::unique_key(a[write_index - 1])) {
            a[write_index++] <- a[from_index]
        }
    }
    a.resize(write_index)
    static_if (typeinfo need_lock_check(a)) {
        set_verify_array_locks(a, true)
    }
}

[skip_lock_check]
def unique_by(a : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! sort and remove duplicate elements from an iterator based on a key
    var inscope buffer <- to_array(a)
    unique_by_inplace(buffer, key)
    return to_sequence_move(buffer)
}

[skip_lock_check]
def unique_by(a : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! sort and remove duplicate elements from an array based on a key
    var b := a
    unique_by_inplace(b, key)
    return <- b
}

[skip_lock_check]
def unique_by_to_array(a : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! sort and remove duplicate elements from an iterator based on a key and returns an array
    var inscope buffer <- to_array(a)
    unique_by_inplace(buffer, key)
    return <- buffer
}

[skip_lock_check]
def unique_by_inplace(var a : array<auto(TT)>; key : block<(arg : TT -&) : auto>) {
    //! remove duplicate elements from an array based on a key in place
    if (length(a) <= 1) {
        return
    }
    static_if (typeinfo need_lock_check(a)) {
        _builtin_verify_locks(a, "array can't be made unique in place because it contains locked elements")
        set_verify_array_locks(a, false)
    }
    var write_index = 1
    for (from_index in 1 .. length(a)) {
        if (key(a[from_index]) != key(a[write_index - 1])) {
            a[write_index++] <- a[from_index]
        }
    }
    a.resize(write_index)
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
def count(a : array<auto(TT)>) : int {
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
def long_count(a : array<auto(TT)>) : int64 {
    //! Counts elements in an array, using a long integer
    return int64(length(a))
}

[skip_lock_check, unused_argument(tt)]
def private where_impl(var src; tt : auto(TT); len : int; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
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

[skip_lock_check, unused_argument(tt)]
def private where_impl_const(src : auto(ARGT); tt : auto(TT); len : int; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    return <- where_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -& -const>, len, predicate)
}

[skip_lock_check]
def where_(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : iterator<TT -& -const> {
    //! Filters elements in an iterator based on a predicate
    return <- where_impl(src, type<TT -& -const>, 0, predicate).to_sequence_move()
}

[skip_lock_check]
def where_(src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Filters elements in an array based on a predicate
    return <- where_impl_const(src, type<TT -& -const>, length(src), predicate)
}

[skip_lock_check]
def where_to_array(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Filters elements in an iterator based on a predicate and returns an array
    return <- where_impl(src, type<TT -& -const>, 0, predicate)
}

[skip_lock_check]
def skip(arr : array<auto(TT)>; var total : int) : array<TT -& -const> {
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
        _builtin_verify_locks(arr, "array can't be skipped in place because it contains locked elements")
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
def private skip_impl(var src; tt : auto(TT); var total : int) : iterator<TT -& -const> {
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
    return skip_impl(src, type<TT -& -const>, total)
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

[skip_lock_check, unused_argument(tt)]
def private skip_while_impl_const(src : auto(ARGT); tt : auto(TT); predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    return <- skip_while_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -& -const>, predicate)
}

[skip_lock_check]
def skip_while(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : iterator<TT -& -const> {
    //! Skips all elements of an iterator while the predicate is true
    // TODO: specialize with const value vs var value of predicate, which can be captured.
    return <- skip_while_impl(src, type<TT -& -const>, predicate).to_sequence_move()
}

[skip_lock_check]
def skip_while(src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Skips all elements of an array while the predicate is true
    return <- skip_while_impl_const(src, type<TT -& -const>, predicate)
}

[skip_lock_check]
def skip_while_to_array(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Skips all elements of an iterator while the predicate is true and returns an array
    return <- skip_while_impl(src, type<TT -& -const>, predicate)
}

[skip_lock_check]
def take(arr : array<auto(TT)>; var total : int) : array<TT -& -const> {
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
def private take_impl(var src; tt : auto(TT); var total : int) : iterator<TT -& -const> {
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
    return take_impl(src, type<TT -& -const>, total)
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
def take(src : array<auto(TT)>; from : range) : array<TT -& -const> {
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
def take_inplace(var arr : array<auto(TT)>; from : range) {
    //! Keeps only a range of elements in an array in place
    static_if (typeinfo need_lock_check(arr)) {
        _builtin_verify_locks(arr, "array can't be taken in place because it contains locked elements")
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

[skip_lock_check, unused_argument(tt)]
def private take_while_impl_const(src : auto(ARGT); tt : auto(TT); predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    return <- take_while_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -& -const>, predicate)
}

[skip_lock_check]
def take_while(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) {
    //! Yields only the elements of an iterator while the predicate is true
    return <- take_while_impl(src, type<TT -& -const>, predicate).to_sequence_move()
}

[skip_lock_check]
def take_while(src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Yields only the elements of an array while the predicate is true
    return <- take_while_impl_const(src, type<TT -& -const>, predicate)
}

[skip_lock_check]
def take_while_to_array(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : array<TT -& -const> {
    //! Yields only the elements of an iterator while the predicate is true and returns an array
    return <- take_while_impl(src, type<TT -& -const>, predicate)
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

[skip_lock_check, unused_argument(tt)]
def private min_impl_const(src : auto(ARGT); tt : auto(TT)) : TT -& -const {
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -& -const>)
    } else {
        return <- min_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    }
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
def min(src : array<auto(TT)>) : TT -& -const {
    //! Finds the minimum element in an array
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_impl_const(src, type<TT -const -&>)
    } else {
        return <- min_impl_const(src, type<TT -const -&>)
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

[skip_lock_check, unused_argument(tt)]
def private min_by_impl_const(src : auto(ARGT); tt : auto(TT); key) : TT -& -const {
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_by_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, key)
    } else {
        return <- min_by_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, key)
    }
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
def min_by(src : array<auto(TT)>; key) : TT -& -const {
    //! Finds the minimum element in an array by key
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_by_impl_const(src, type<TT -const -&>, key)
    } else {
        return <- min_by_impl_const(src, type<TT -const -&>, key)
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

[skip_lock_check, unused_argument(tt)]
def private max_impl_const(src : auto(ARGT); tt : auto(TT)) : TT -& -const {
    static_if (typeinfo is_workhorse(type<TT>)) {
        return max_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    } else {
        return <- max_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    }
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
def max(src : array<auto(TT)>) : TT -& -const {
    //! Finds the maximum element in an array
    static_if (typeinfo is_workhorse(type<TT>)) {
        return max_impl_const(src, type<TT -const -&>)
    } else {
        return <- max_impl_const(src, type<TT -const -&>)
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

[skip_lock_check, unused_argument(tt)]
def private max_by_impl_const(src : auto(ARGT); tt : auto(TT); key) : TT -& -const {
    static_if (typeinfo is_workhorse(type<TT>)) {
        return max_by_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, key)
    } else {
        return <- max_by_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, key)
    }
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
def max_by(src : array<auto(TT)>; key) : TT -& -const {
    //! Finds the maximum element in an array by key
    static_if (typeinfo is_workhorse(type<TT>)) {
        return max_by_impl_const(src, type<TT -const -&>, key)
    } else {
        return <- max_by_impl_const(src, type<TT -const -&>, key)
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

[skip_lock_check, unused_argument(tt)]
def private min_max_impl_const(src : auto(ARGT); tt : auto(TT)) : tuple<TT -& -const, TT -& -const> {
    static_if (typeinfo is_workhorse(type<TT>)) {
        return min_max_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    } else {
        return <- min_max_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    }
}

[skip_lock_check]
def min_max(var src : iterator<auto(TT)>) : tuple<TT -& -const, TT -& -const> {
    //! Finds the minimum and maximum elements in an iterator
    return <- min_max_impl(src, type<TT -const -&>)
}

[skip_lock_check]
def min_max(src : array<auto(TT)>) : tuple<TT -& -const, TT -& -const> {
    //! Finds the minimum and maximum elements in an array
    return <- min_max_impl_const(src, type<TT -const -&>)
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

[skip_lock_check, unused_argument(tt)]
def private min_max_by_impl_const(src : auto(ARGT); tt : auto(TT); key) : tuple<TT -& -const, TT -& -const> {
    return <- min_max_by_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, key)
}

[skip_lock_check]
def min_max_by(var src : iterator<auto(TT)>; key) : tuple<TT -& -const, TT -& -const> {
    //! Finds the minimum and maximum elements in an iterator by key
    return <- min_max_by_impl(src, type<TT -const -&>, key)
}

[skip_lock_check]
def min_max_by(src : array<auto(TT)>; key) : tuple<TT -& -const, TT -& -const> {
    //! Finds the minimum and maximum elements in an array by key
    return <- min_max_by_impl_const(src, type<TT -const -&>, key)
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

[skip_lock_check, unused_argument(tt)]
def private aggregate_impl_const(src : auto(ARGT); tt : auto(TT); seed : auto(AGG); func : block<(acc : AGG -&, x : TT -&) : AGG -&>) : AGG -& -const {
    static_if (typeinfo is_workhorse(type<TT>)) {
        return aggregate_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, seed, func)
    } else {
        return <- aggregate_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, seed, func)
    }
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
def aggregate(src : array<auto(TT)>; seed : auto(AGG); func : block<(acc : AGG -&, x : TT -&) : AGG -&>) : AGG -& -const {
    //! Aggregates elements in an array using a seed and a function
    static_if (typeinfo is_workhorse(seed)) {
        return aggregate_impl_const(src, type<TT -const -&>, seed, func)
    } else {
        return <- aggregate_impl_const(src, type<TT -const -&>, seed, func)
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

[skip_lock_check, unused_argument(tt)]
def private sum_impl_const(src : auto(ARGT); tt : auto(TT)) : TT -const -& {
    static_if (typeinfo is_workhorse(type<TT>)) {
        return sum_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    } else {
        return <- sum_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    }
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
def sum(src : array<auto(TT)>) : TT -const -& {
    //! Sums elements in an array
    static_if (typeinfo is_workhorse(type<TT>)) {
        return sum_impl_const(src, type<TT -const -&>)
    } else {
        return <- sum_impl_const(src, type<TT -const -&>)
    }
}

[skip_lock_check, unused_argument(tt)]
def private average_impl(var src; tt : auto(TT)) : TT -const -& {
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

[skip_lock_check, unused_argument(tt)]
def private average_impl_const(src : auto(ARGT); tt : auto(TT)) : TT -const -& {
    static_if (typeinfo is_workhorse(type<TT>)) {
        return average_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    } else {
        return <- average_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
    }
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
def average(src : array<auto(TT)>) : TT -const -& {
    //! Averages elements in an array
    static_if (typeinfo is_workhorse(type<TT>)) {
        return average_impl_const(src, type<TT -const -&>)
    } else {
        return <- average_impl_const(src, type<TT -const -&>)
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

[skip_lock_check, unused_argument(tt)]
def private min_max_average_impl_const(src : auto(ARGT); tt : auto(TT)) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    return <- min_max_average_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
}

[skip_lock_check]
def min_max_average(var src : iterator<auto(TT)>) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    //! Finds the minimum, maximum, and average elements in an iterator
    return <- min_max_average_impl(src, type<TT -const -&>)
}

[skip_lock_check]
def min_max_average(src : array<auto(TT)>) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    //! Finds the minimum, maximum, and average elements in an array
    return <- min_max_average_impl_const(src, type<TT -const -&>)
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

[skip_lock_check, unused_argument(tt)]
def private min_max_average_by_impl_const(src : auto(ARGT); tt : auto(TT); key) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    return <- min_max_average_by_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, key)
}

[skip_lock_check]
def min_max_average_by(var src : iterator<auto(TT)>; key) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    //! Finds the minimum, maximum, and average elements in an iterator by key
    return <- min_max_average_by_impl(src, type<TT -const -&>, key)
}

[skip_lock_check]
def min_max_average_by(src : array<auto(TT)>; key) : tuple<TT -const -&, TT -const -&, TT -const -&> {
    //! Finds the minimum, maximum, and average elements in an array by key
    return <- min_max_average_by_impl_const(src, type<TT -const -&>, key)
}

[skip_lock_check]
def private join_impl(var srca; var srcb; ta : auto(TA); tb : auto(TB); keya, keyb; result) : array<typedecl(result(type<TA>, type<TB>)) -const -&> {
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
    return <- buffer
}

[skip_lock_check]
def private join_impl_const(srca : auto(ARGTA); srcb : auto(ARGTB); ta : auto(TA); tb : auto(TB); keya, keyb; result) : array<typedecl(result(type<TA>, type<TB>)) -const -&> {
    return <- join_impl(unsafe(reinterpret<ARGTA -const>(srca)), unsafe(reinterpret<ARGTB -const>(srcb)), type<TA -const -&>, type<TB -const -&>, keya, keyb, result)
}

[skip_lock_check]
def join(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : iterator<typedecl(result(type<TA>, type<TB>)) -const -&> {
    //! Joins two iterators based on matching keys (inner join)
    return <- join_impl(srca, srcb, type<TA -const -&>, type<TB -const -&>, keya, keyb, result).to_sequence_move()
}

[skip_lock_check]
def join(srca : array<auto(TA)>; srcb : array<auto(TB)>; keya, keyb; result) : array<typedecl(result(type<TA>, type<TB>)) -const -&> {
    //! Joins two arrays based on matching keys (inner join)
    return <- join_impl_const(srca, srcb, type<TA -const -&>, type<TB -const -&>, keya, keyb, result)
}

[skip_lock_check]
def join_to_array(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : array<typedecl(result(type<TA>, type<TB>)) -const -&> {
    //! Joins two iterators based on matching keys (inner join) and returns an array
    return <- join_impl(srca, srcb, type<TA -const -&>, type<TB -const -&>, keya, keyb, result)
}

[skip_lock_check, unused_argument(ta, tb)]
def private group_join_impl(var srca; var srcb; ta : auto(TA); tb : auto(TB); keya, keyb; result) : array<typedecl(result(type<TA>, type<array<TB -const -&>>)) -const -&> {
    // we pass TA, and sequence of TB to 'result'
    var inscope tableb : table<typedecl(_::unique_key(keyb(type<TB>))); array<TB -const -&>>
    for (itb in srcb) {
        let k = _::unique_key(keyb(itb))
        unsafe(tableb[k]).push_clone(itb)
    }
    var buffer : array<typedecl(result(type<TA>, type<array<TB -const -&>>)) -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (ita in srca) {
        let k = _::unique_key(keya(ita))
        tableb.get(k, $(var arr){
            buffer.push_clone(result(ita, arr))
        })
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check, unused_argument(ta, tb)]
def private group_join_impl_const(srca : auto(ARGTA); srcb : auto(ARGTB); ta : auto(TA); tb : auto(TB); keya, keyb; result) : array<typedecl(result(type<TA>, type<array<TB -const -&>>)) -const -&> {
    return <- group_join_impl(unsafe(reinterpret<ARGTA -const>(srca)), unsafe(reinterpret<ARGTB -const>(srcb)), type<TA -const -&>, type<TB -const -&>, keya, keyb, result)
}

[skip_lock_check]
def group_join(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : iterator<typedecl(result(type<TA>, type<array<TB -const -&>>)) -const -&> {
    //! we pass TA, and sequence of TB to 'result'
    return <- group_join_impl(srca, srcb, type<TA -const -&>, type<TB -const -&>, keya, keyb, result).to_sequence_move()
}

[skip_lock_check]
def group_join(srca : array<auto(TA)>; srcb : array<auto(TB)>; keya, keyb; result) : array<typedecl(result(type<TA>, type<array<TB -const -&>>)) -const -&> {
    //! we pass TA, and sequence of TB to 'result'
    return <- group_join_impl_const(srca, srcb, type<TA -const -&>, type<TB -const -&>, keya, keyb, result)
}

[skip_lock_check]
def group_join_to_array(var srca : iterator<auto(TA)>; var srcb : iterator<auto(TB)>; keya, keyb; result) : array<typedecl(result(type<TA>, type<array<TB -const -&>>)) -const -&> {
    //! we pass TA, and sequence of TB to 'result'
    return <- group_join_impl(srca, srcb, type<TA -const -&>, type<TB -const -&>, keya, keyb, result)
}

[skip_lock_check, unused_argument(tt)]
def private group_by_impl(var source; tt : auto(TT); key; element_selector; result_selector)  {
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
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private group_by_impl_const(source : auto(ARGT); tt : auto(TT); key; element_selector; result_selector)  {
    return <- group_by_impl(unsafe(reinterpret<ARGT -const>(source)), type<TT -const -&>, key, element_selector, result_selector)
}

[skip_lock_check]
def group_by(var source : iterator<auto(TT)>; key; element_selector; result_selector)  {
    //! Groups the elements of an iterator according to a specified key selector function
    return <- group_by_impl(source, type<TT -const -&>, key, element_selector, result_selector).to_sequence_move()
}

[skip_lock_check]
def group_by(source : array<auto(TT)>; key; element_selector; result_selector)  {
    //! Groups the elements of an array according to a specified key selector function
    return <- group_by_impl_const(source, type<TT -const -&>, key, element_selector, result_selector)
}

[skip_lock_check]
def group_by_to_array(var source : iterator<auto(TT)>; key; element_selector; result_selector)  {
    //! Groups the elements of an iterator according to a specified key selector function and returns an array
    return <- group_by_impl(source, type<TT -const -&>, key, element_selector, result_selector)
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

[skip_lock_check, unused_argument(tt)]
def private union_impl_const(srca : auto(ARGTA); srcb : auto(ARGTB); tt : auto(TT)) : array<TT -const -&> {
    return <- union_impl(unsafe(reinterpret<ARGTA -const>(srca)), unsafe(reinterpret<ARGTB -const>(srcb)), type<TT -const -&>)
}

[skip_lock_check]
def union(var srca, srcb : array<auto(TT)>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two arrays
    return <- union_impl_const(srca, srcb, type<TT -const -&>)
}

[skip_lock_check]
def union(var srca, srcb : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators
    return <- union_impl(srca, srcb, type<TT -const -&>).to_sequence_move()
}

[skip_lock_check]
def union_to_array(var srca, srcb : iterator<auto(TT)>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators and returns an array
    return <- union_impl(srca, srcb, type<TT -const -&>)
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

[skip_lock_check, unused_argument(tt)]
def private union_by_impl_const(srca : auto(ARGTA); srcb : auto(ARGTB); tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    return <- union_by_impl(unsafe(reinterpret<ARGTA -const>(srca)), unsafe(reinterpret<ARGTB -const>(srcb)), type<TT -const -&>, key)
}

[skip_lock_check]
def union_by(var srca, srcb : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators by key
    return <- union_by_impl(srca, srcb, type<TT -const -&>, key).to_sequence_move()
}

[skip_lock_check]
def union_by(srca, srcb : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two arrays by key
    return <- union_by_impl_const(srca, srcb, type<TT -const -&>, key)
}

[skip_lock_check]
def union_by_to_array(var srca, srcb : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from the concatenation of two iterators by key and returns an array
    return <- union_by_impl(srca, srcb, type<TT -const -&>, key)
}

[skip_lock_check]
def any(src : array<auto(TT)>) : bool {
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

[skip_lock_check, unused_argument(tt)]
def private any_impl_const(src : auto(ARGT); tt : auto(TT); predicate : block<(arg : auto(TT) -&) : bool>) : bool {
    return any_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, predicate)
}

[skip_lock_check]
def any(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : bool {
    //! Returns true if any element in the iterator satisfies the predicate
    return any_impl(src, type<TT -const -&>, predicate)
}

[skip_lock_check]
def any(src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : bool {
    //! Returns true if any element in the array satisfies the predicate
    return any_impl_const(src, type<TT -const -&>, predicate)
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

[skip_lock_check, unused_argument(tt)]
def private all_impl_const(src : auto(ARGT); tt : auto(TT); predicate : block<(arg : auto(TT) -&) : bool>) : bool {
    return all_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, predicate)
}

[skip_lock_check]
def all(var src : iterator<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : bool {
    //! Returns true if all elements in the iterator satisfy the predicate
    return all_impl(src, type<TT -const -&>, predicate)
}

[skip_lock_check]
def all(src : array<auto(TT)>; predicate : block<(arg : TT -&) : bool>) : bool {
    //! Returns true if all elements in the array satisfy the predicate
    return all_impl_const(src, type<TT -const -&>, predicate)
}

[skip_lock_check, unused_argument(tt)]
def private except_impl(var src, exclude; tt : auto(TT)) : array<TT -const -&> {
    //! Returns distinct elements from the first iterator that are not in the second iterator
    var inscope seen : table<typedecl(_::unique_key(type<TT>))>
    for (it in exclude) {
        seen.insert(_::unique_key(it))
    }
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in src) {
        let uk = _::unique_key(it)
        if (!seen.key_exists(uk)) {
            buffer.push_clone(it)
            seen.insert(uk)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private except_impl_const(src : auto(ARGT); exclude : auto(ARGTE); tt : auto(TT)) : array<TT -const -&> {
    return <- except_impl(unsafe(reinterpret<ARGT -const>(src)), unsafe(reinterpret<ARGTE -const>(exclude)), type<TT -const -&>)
}

[skip_lock_check]
def except(var src, exclude : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator
    return <- except_impl(src, exclude, type<TT -const -&>).to_sequence_move()
}

[skip_lock_check]
def except(src, exclude : array<auto(TT)>) : array<TT -const -&> {
    //! Returns elements from the first array that are not in the second array
    return <- except_impl_const(src, exclude, type<TT -const -&>)
}

[skip_lock_check]
def except_to_array(var src, exclude : iterator<auto(TT)>) : array<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator and returns an array
    return <- except_impl(src, exclude, type<TT -const -&>)
}

[skip_lock_check, unused_argument(tt)]
def private except_by_impl(var src, exclude; tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns distinct elements from the first iterator that are not in the second iterator by key
    var inscope seen : table<typedecl(_::unique_key(key(type<TT>)))>
    for (it in exclude) {
        seen.insert(_::unique_key(key(it)))
    }
    var buffer : array<TT -const -&>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it in src) {
        let uk = _::unique_key(key(it))
        if (!seen.key_exists(uk)) {
            buffer.push_clone(it)
            seen.insert(uk)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private except_by_impl_const(src : auto(ARGT); exclude : auto(ARGTE); tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    return <- except_by_impl(unsafe(reinterpret<ARGT -const>(src)), unsafe(reinterpret<ARGTE -const>(exclude)), type<TT -const -&>, key)
}

[skip_lock_check]
def except_by(var src, exclude : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator by key
    return <- except_by_impl(src, exclude, type<TT -const -&>, key).to_sequence_move()
}

[skip_lock_check]
def except_by(src, exclude : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements from the first array that are not in the second array by key
    return <- except_by_impl_const(src, exclude, type<TT -const -&>, key)
}

[skip_lock_check]
def except_by_to_array(var src, exclude : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements from the first iterator that are not in the second iterator by key and returns an array
    return <- except_by_impl(src, exclude, type<TT -const -&>, key)
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
        let uk = _::unique_key(it)
        if (seen.key_exists(uk)) {
            buffer.push_clone(it)
            seen.erase(uk)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private intersect_impl_const(srca : auto(ARGTA); srcb : auto(ARGTB); tt : auto(TT)) : array<TT -const -&> {
    return <- intersect_impl(unsafe(reinterpret<ARGTA -const>(srca)), unsafe(reinterpret<ARGTB -const>(srcb)), type<TT -const -&>)
}

[skip_lock_check]
def intersect(var srca, srcb : iterator<auto(TT)>) : iterator<TT -const -&> {
    //! Returns elements that are present in both iterators
    return <- intersect_impl(srca, srcb, type<TT -const -&>).to_sequence_move()
}

[skip_lock_check]
def intersect(srca, srcb : array<auto(TT)>) : array<TT -const -&> {
    //! Returns elements that are present in both arrays
    return <- intersect_impl_const(srca, srcb, type<TT -const -&>)
}

[skip_lock_check]
def intersect_to_array(var srca, srcb : iterator<auto(TT)>) : array<TT -const -&> {
    //! Returns elements that are present in both iterators and returns an array
    return <- intersect_impl(srca, srcb, type<TT -const -&>)
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
        let uk = _::unique_key(key(it))
        if (seen.key_exists(uk)) {
            buffer.push_clone(it)
            seen.erase(uk)
        }
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private intersect_by_impl_const(srca : auto(ARGTA); srcb : auto(ARGTB); tt : auto(TT); key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    return <- intersect_by_impl(unsafe(reinterpret<ARGTA -const>(srca)), unsafe(reinterpret<ARGTB -const>(srcb)), type<TT -const -&>, key)
}

[skip_lock_check]
def intersect_by(var srca, srcb : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : iterator<TT -const -&> {
    //! Returns elements that are present in both iterators by key
    return <- intersect_by_impl(srca, srcb, type<TT -const -&>, key).to_sequence_move()
}

[skip_lock_check]
def intersect_by(srca, srcb : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements that are present in both arrays by key
    return <- intersect_by_impl_const(srca, srcb, type<TT -const -&>, key)
}

[skip_lock_check]
def intersect_by_to_array(var srca, srcb : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : array<TT -const -&> {
    //! Returns elements that are present in both iterators by key and returns an array
    return <- intersect_by_impl(srca, srcb, type<TT -const -&>, key)
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

[skip_lock_check, unused_argument(tt)]
def private contains_impl_const(src : auto(ARGT); tt : auto(TT); element : TT -&) : bool {
    return contains_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, element)
}

[skip_lock_check]
def contains(var src : iterator<auto(TT)>; element : TT -&) : bool {
    //! Returns true if the element is present in the iterator
    return contains_impl(src, type<TT -const -&>, element)
}

[skip_lock_check]
def contains(src : array<auto(TT)>; element : TT -&) : bool {
    //! Returns true if the element is present in the array
    return contains_impl_const(src, type<TT -const -&>, element)
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
    return each(start..start + count)
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

[skip_lock_check, unused_argument(tt)]
def private sequence_equal_impl(var first, second; tt : auto(TT)) : bool {
    static_if (typeinfo can_copy(type<TT -const -&>)) {
        var f : TT -const -&
        var s : TT -const -&
        while (true) {
            let has_f = next(first, f)
            let has_s = next(second, s)
            if (has_f != has_s) {
                return false
            }
            if (!has_f) {
                return true
            }
            if (f != s) {
                return false
            }
        }
        return true
    } else {
        var inscope arr_first <- to_array(first)
        var inscope arr_second <- to_array(second)
        if (length(arr_first) != length(arr_second)) {
            return false
        }
        for (f, s in arr_first, arr_second) {
            if (f != s) {
                return false
            }
        }
        return true
    }
}

[skip_lock_check, unused_argument(tt)]
def private sequence_equal_impl_const(first : auto(ARGT); second : auto(ARGTS); tt : auto(TT)) : bool {
    if (length(first) != length(second)) {
        return false
    }
    for (f, s in first, second) {
        if (f != s) {
            return false
        }
    }
    return true
}

[skip_lock_check]
def sequence_equal(var first, second : iterator<auto(TT)>) : bool {
    //! Checks if two sequences are equal
    return sequence_equal_impl(first, second, type<TT -const -&>)
}

[skip_lock_check]
def sequence_equal(first, second : array<auto(TT)>) : bool {
    //! Checks if two arrays are equal
    return sequence_equal_impl_const(first, second, type<TT -const -&>)
}

[skip_lock_check, unused_argument(tt)]
def private sequence_equal_by_impl(var first, second; tt : auto(TT); key : block<(arg : TT -&) : auto>) : bool {
    static_if (typeinfo can_copy(type<TT -const -&>)) {
        var f : TT -const -&
        var s : TT -const -&
        while (true) {
            let has_f = next(first, f)
            let has_s = next(second, s)
            if (has_f != has_s) {
                return false
            }
            if (!has_f) {
                return true
            }
            if (key(f) != key(s)) {
                return false
            }
        }
        return true
    } else {
        var inscope arr_first <- to_array(first)
        var inscope arr_second <- to_array(second)
        if (length(arr_first) != length(arr_second)) {
            return false
        }
        for (f, s in arr_first, arr_second) {
            if (key(f) != key(s)) {
                return false
            }
        }
        return true
    }
}

[skip_lock_check, unused_argument(tt)]
def private sequence_equal_by_impl_const(first : auto(ARGT); second : auto(ARGTS); tt : auto(TT); key : block<(arg : TT -&) : auto>) : bool {
    if (length(first) != length(second)) {
        return false
    }
    for (f, s in first, second) {
        if (key(f) != key(s)) {
            return false
        }
    }
    return true
}

[skip_lock_check]
def sequence_equal_by(var first, second : iterator<auto(TT)>; key : block<(arg : TT -&) : auto>) : bool {
    //! Checks if two sequences are equal by key
    return sequence_equal_by_impl(first, second, type<TT -const -&>, key)
}

[skip_lock_check]
def sequence_equal_by(first, second : array<auto(TT)>; key : block<(arg : TT -&) : auto>) : bool {
    //! Checks if two arrays are equal by key
    return sequence_equal_by_impl_const(first, second, type<TT -const -&>, key)
}

[skip_lock_check]
def element_at(src : array<auto(TT)>; index : int) : TT -const -& {
    //! Returns the element at the specified index
    panic("element index out of range") if (index < 0 || index >= length(src))
    static_if (typeinfo can_copy(src[index])) {
        return src[index]
    } else {
        return <- clone_to_move(src[index])
    }
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
def element_at_or_default(src : array<auto(TT)>; index : int) : TT -const -& {
    //! Returns the element at the specified index, or a default value if the index is out of range
    if (index < 0 || index >= length(src)) {
        var t : TT -const -&
        return <- t
    }
    static_if (typeinfo can_copy(src[index])) {
        return src[index]
    } else {
        return <- clone_to_move(src[index])
    }
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
def first(src : array<auto(TT)>) : TT -const -& {
    //! Returns the first element of an array
    panic("sequence contains no elements") if (length(src) == 0)
    static_if (typeinfo can_copy(src[0])) {
        return src[0]
    } else {
        return <- clone_to_move(src[0])
    }
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
def first_or_default(src : array<auto(TT)>; defaultValue : TT -&) : TT -const -& {
    //! Returns the first element of an array, or a default value if the array is empty
    if (length(src) == 0) {
        static_if (typeinfo can_copy(defaultValue)) {
            return defaultValue
        } else {
            return <- clone_to_move(defaultValue)
        }
    }
    static_if (typeinfo can_copy(src[0])) {
        return src[0]
    } else {
        return <- clone_to_move(src[0])
    }
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
def last(src : array<auto(TT)>) : TT -const -& {
    //! Returns the last element of an array
    let srcl = length(src)
    panic("sequence contains no elements") if (srcl == 0)
    static_if (typeinfo can_copy(src[srcl - 1])) {
        return src[srcl - 1]
    } else {
        return <- clone_to_move(src[srcl - 1])
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
def last_or_default(src : array<auto(TT)>; defaultValue : TT -&) : TT -const -& {
    //! Returns the last element of an array, or a default value if the array is empty
    let srcl = length(src)
    if (srcl == 0) {
        static_if (typeinfo can_copy(defaultValue)) {
            return defaultValue
        } else {
            return <- clone_to_move(defaultValue)
        }
    }
    static_if (typeinfo can_copy(src[srcl - 1])) {
        return src[srcl - 1]
    } else {
        return <- clone_to_move(src[srcl - 1])
    }
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
def single(src : array<auto(TT)>) : TT -const -& {
    //! Returns the only element of an array, and throws if there is not exactly one element
    let srcl = length(src)
    if (srcl == 0) {
        panic("sequence contains no elements")
    }
    if (srcl > 1) {
        panic("sequence contains more than one element")
    }
    static_if (typeinfo can_copy(src[0])) {
        return src[0]
    } else {
        return <- clone_to_move(src[0])
    }
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
def single_or_default(src : array<auto(TT)>; defaultValue : TT -&) : TT -const -& {
    //! Returns the only element of an array, or a default value if there is not exactly one element
    let srcl = length(src)
    if (srcl == 0) {
        static_if (typeinfo can_copy(defaultValue)) {
            return defaultValue
        } else {
            return <- clone_to_move(defaultValue)
        }
    }
    if (srcl > 1) {
        static_if (typeinfo can_copy(defaultValue)) {
            return defaultValue
        } else {
            return <- clone_to_move(defaultValue)
        }
    }
    static_if (typeinfo can_copy(src[0])) {
        return src[0]
    } else {
        return <- clone_to_move(src[0])
    }
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
def private prepend_impl_const(arr : auto(ARGT); tt : auto(TT); reserveSize : int; value : TT -&) : array<TT -const -&> {
    return <- prepend_impl(unsafe(reinterpret<ARGT -const>(arr)), type<TT -const -&>, reserveSize, value)
}

[skip_lock_check]
def prepend(arr : array<auto(TT)>; value : TT -&) : array<TT -const -&> {
    //! Prepends a value to the beginning of an array
    return <- prepend_impl_const(arr, type<TT -const -&>, length(arr) + 1, value)
}

[skip_lock_check]
def prepend_to_array(var it : iterator<auto(TT)>; value : TT -&) : array<TT -const -&> {
    //! Prepends a value to the beginning of an iterator and returns an array
    return <- prepend_impl(it, type<TT -const -&>, 0, value)
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

[skip_lock_check, unused_argument(tt)]
def private append_impl_const(arr : auto(ARGT); tt : auto(TT); reserveSize : int; value : TT -&) : array<TT -const -&> {
    return <- append_impl(unsafe(reinterpret<ARGT -const>(arr)), type<TT -const -&>, reserveSize, value)
}

[skip_lock_check]
def append(arr : array<auto(TT)>; value : TT -&) : array<TT -const -&> {
    //! Appends a value to the end of an array
    return <- append_impl_const(arr, type<TT -const -&>, length(arr) + 1, value)
}

[skip_lock_check]
def append_to_array(var it : iterator<auto(TT)>; value : TT -&) : array<TT -const -&> {
    //! Appends a value to the end of an iterator and returns an array
    return <- append_impl(it, type<TT -const -&>, 0, value)
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
def private select_impl(var src; tt : auto(TT)) : array<tuple<int, TT -const -&>> {
    //! Projects each element of an iterator into a new form with its index
    var buffer : array<tuple<int, TT -const -&>>
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, false))
    }
    for (it, index in src, count()) {
        buffer.emplace((index, it))
    }
    static_if (typeinfo need_lock_check(buffer)) {
        unsafe(set_verify_array_locks(buffer, true))
    }
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private select_impl_const(src : auto(ARGT); tt : auto(TT)) : array<tuple<int, TT -const -&>> {
    return <- select_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>)
}

[skip_lock_check]
def select(var src : iterator<auto(TT)>) : iterator<tuple<int, TT -const -&>> {
    //! Projects each element of an iterator into a new form
    return <- select_impl(src, type<TT -const -&>).to_sequence_move()
}

[skip_lock_check]
def select(src : array<auto(TT)>) : array<tuple<int, TT -const -&>> {
    //! Projects each element of an array into a new form
    return <- select_impl_const(src, type<TT -const -&>)
}

[skip_lock_check]
def select_to_array(var src : iterator<auto(TT)>) : array<tuple<int, TT -const -&>> {
    //! Projects each element of an iterator into a new form and returns an array
    return <- select_impl(src, type<TT -const -&>)
}

[skip_lock_check, unused_argument(tt)]
def private select_impl(var src; tt : auto(TT); len : int; selector) : array<typedecl(selector(type<TT>)) -const -&> {
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

[skip_lock_check, unused_argument(tt)]
def private select_impl_const(src : auto(ARGT); tt : auto(TT); len : int; selector) : array<typedecl(selector(type<TT>)) -const -&> {
    return <- select_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, len, selector)
}

[skip_lock_check]
def select(var src : iterator<auto(TT)>; result_selector) : iterator<typedecl(result_selector(type<TT>)) -const -&> {
    //! Projects each element of an iterator into a new form using a selector function
    return <- select_impl(src, type<TT -const -&>, 0, result_selector).to_sequence_move()
}

[skip_lock_check]
def select(src : array<auto(TT)>; result_selector) : array<typedecl(result_selector(type<TT>)) -const -&> {
    //! Projects each element of an array into a new form using a selector function
    return <- select_impl_const(src, type<TT -const -&>, length(src), result_selector)
}

[skip_lock_check]
def select_to_array(var src : iterator<auto(TT)>; result_selector) : array<typedecl(result_selector(type<TT>)) -const -&> {
    //! Projects each element of an iterator into a new form using a selector function and returns an array
    return <- select_impl(src, type<TT -const -&>, 0, result_selector)
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

[skip_lock_check, unused_argument(tt)]
def private chunk_impl_const(src : auto(ARGT); tt : auto(TT); size : int) : array<array<TT -const -&>> {
    return <- chunk_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, size)
}

[skip_lock_check]
def chunk(src : array<auto(TT)>; size : int) : array<array<TT -const -&>> {
    //! Splits an array into chunks of a specified size
    return <- chunk_impl_const(src, type<TT -const -&>, size)
}

[skip_lock_check]
def chunk_to_array(var src : iterator<auto(TT)>; size : int) : array<array<TT -const -&>> {
    //! Splits an iterator into chunks of a specified size and returns an array
    return <- chunk_impl(src, type<TT -const -&>, size)
}

[skip_lock_check]
def chunk(var src : iterator<auto(TT)>; size : int) : iterator<array<TT -const -&>> {
    //! Splits an iterator into chunks of a specified size
    return <- chunk_impl(src, type<TT -const -&>, size).to_sequence_move()
}

[unused_argument(src)]
def private iter_type(var src : iterator<auto(TT)>) : TT -const -& {
    //! this type trait is used to extract the type of elements in an iterator
    var t : TT -const -&
    return <- t
}

[unused_argument(src)]
def private iter_type(src : array<auto(TT)>) : TT -const -& {
    //! this type trait is used to extract the type of elements in an iterator
    var t : TT -const -&
    return <- t
}

[skip_lock_check, unused_argument(tt)]
def private select_many_impl(var src; tt : auto(TT); result_selector) : array<typedecl(result_selector(iter_type(type<TT>))) -const -&> {
    //! Projects each element of an iterator to an iterator and flattens the resulting iterators into one array
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
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private select_many_impl_const(src : auto(ARGT); tt : auto(TT); result_selector) : array<typedecl(result_selector(iter_type(type<TT>))) -const -&> {
    return <- select_many_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, result_selector)
}

[skip_lock_check]
def select_many(var src : iterator<auto(TT)>; result_selector) : iterator<typedecl(result_selector(iter_type(type<TT>))) -const -&> {
    //! Projects each element of an iterator to an iterator and flattens the resulting iterators into one iterator
    return <- select_many_impl(src, type<TT -const -&>, result_selector).to_sequence_move()
}

[skip_lock_check]
def select_many(src : array<auto(TT)>; result_selector) : array<typedecl(result_selector(iter_type(type<TT>))) -const -&> {
    //! Projects each element of an array to an iterator and flattens the resulting iterators into one array
    return <- select_many_impl_const(src, type<TT -const -&>, result_selector)
}

[skip_lock_check]
def select_many_to_array(var src : iterator<auto(TT)>; result_selector) : array<typedecl(result_selector(iter_type(type<TT>))) -const -&> {
    //! Projects each element of an iterator to an iterator and flattens the resulting iterators into one array
    return <- select_many_impl(src, type<TT -const -&>, result_selector)
}

[skip_lock_check, unused_argument(tt)]
def private select_many_impl(var src; tt : auto(TT); collection_selector; result_selector) : array<typedecl(result_selector(iter_type(collection_selector(type<TT>)))) -const -&> {
    //! Projects each element of an iterator to an iterator and flattens the resulting iterators into one array
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
    return <- buffer
}

[skip_lock_check, unused_argument(tt)]
def private select_many_impl_const(src : auto(ARGT); tt : auto(TT); collection_selector; result_selector) : array<typedecl(result_selector(iter_type(collection_selector(type<TT>)))) -const -&> {
    return <- select_many_impl(unsafe(reinterpret<ARGT -const>(src)), type<TT -const -&>, collection_selector, result_selector)
}

[skip_lock_check]
def select_many(var src : iterator<auto(TT)>; collection_selector; result_selector) : iterator<typedecl(result_selector(iter_type(collection_selector(type<TT>)))) -const -&> {
    //! Projects each element of an iterator to an iterator and flattens the resulting iterators into one iterator
    return <- select_many_impl(src, type<TT -const -&>, collection_selector, result_selector).to_sequence_move()
}

[skip_lock_check]
def select_many(src : array<auto(TT)>; collection_selector; result_selector) : array<typedecl(result_selector(iter_type(collection_selector(type<TT>)))) -const -&> {
    //! Projects each element of an array to an iterator and flattens the resulting iterators into one array
    return <- select_many_impl_const(src, type<TT -const -&>, collection_selector, result_selector)
}

[skip_lock_check]
def select_many_to_array(var src : iterator<auto(TT)>; collection_selector; result_selector) : array<typedecl(result_selector(iter_type(collection_selector(type<TT>)))) -const -&> {
    //! Projects each element of an iterator to an iterator and flattens the resulting iterators into one array
    return <- select_many_impl(src, type<TT -const -&>, collection_selector, result_selector)
}

[skip_lock_check, unused_argument(tt, uu)]
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

[skip_lock_check, unused_argument(tt, uu)]
def private zip_impl_const(a : auto(ARGT); tt : auto(TT); b : auto(ARGTS); uu : auto(UU)) : array<tuple<TT -const -&, UU -const -&>> {
    return <- zip_impl(unsafe(reinterpret<ARGT -const>(a)), type<TT -const -&>, unsafe(reinterpret<ARGTS -const>(b)), type<UU -const -&>)
}

[skip_lock_check]
def zip(var a : iterator<auto(TT)>; var b : iterator<auto(UU)>) : iterator<tuple<TT -const -&, UU -const -&>> {
    //! Merges two iterators into an iterator of tuples
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>).to_sequence_move()
}

[skip_lock_check]
def zip(a : array<auto(TT)>; b : array<auto(UU)>) : array<tuple<TT -const -&, UU -const -&>> {
    //! Merges two arrays into an array of tuples
    return <- zip_impl_const(a, type<TT -const -&>, b, type<UU -const -&>)
}

[skip_lock_check]
def zip_to_array(var a : iterator<auto(TT)>; var b : iterator<auto(UU)>) : array<tuple<TT -const -&, UU -const -&>> {
    //! Merges two iterators into an array of tuples
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>)
}

[skip_lock_check, unused_argument(tt, uu)]
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

[skip_lock_check, unused_argument(tt, uu)]
def private zip_impl_const(a : auto(ARGT); tt : auto(TT); b : auto(ARGTS); uu : auto(UU); result_selector : block<(l : TT -&; r : UU -&) : auto>) : array<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    return <- zip_impl(unsafe(reinterpret<ARGT -const>(a)), type<TT -const -&>, unsafe(reinterpret<ARGTS -const>(b)), type<UU -const -&>, result_selector)
}

[skip_lock_check]
def zip(var a : iterator<auto(TT)>; var b : iterator<auto(UU)>; result_selector : block<(l : TT -&; r : UU -&) : auto>) : iterator<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    //! Merges two iterators into an iterator by applying a specified function
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>, result_selector).to_sequence_move()
}

[skip_lock_check]
def zip(a : array<auto(TT)>; b : array<auto(UU)>; result_selector : block<(l : TT -&; r : UU -&) : auto>) : array<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    //! Merges two arrays into an array by applying a specified function
    return <- zip_impl_const(a, type<TT -const -&>, b, type<UU -const -&>, result_selector)
}

[skip_lock_check]
def zip_to_array(var a : iterator<auto(TT)>; var b : iterator<auto(UU)>; result_selector : block<(l : TT -&; r : UU -&) : auto>) : array<typedecl(result_selector(type<TT>, type<UU>)) -const -&> {
    //! Merges two iterators into an array by applying a specified function
    return <- zip_impl(a, type<TT -const -&>, b, type<UU -const -&>, result_selector)
}

[skip_lock_check]
def public order_unique_folded(var a : array<auto(TT)>) : array<TT -const -&> {
    //! sort and remove duplicate elements from an array
    var arr <- order(a)
    unique_inplace(arr)
    return <- arr
}

[skip_lock_check]
def public order_unique_folded(var a : iterator<auto(TT)>) : array<TT -const -&> {
    //! sort and remove duplicate elements from an iterator
    var arr <- to_array(a)
    order_inplace(arr)
    unique_inplace(arr)
    return <- arr
}

[skip_lock_check]
def public order_unique_folded_inplace(var a : array<auto(TT)>) {
    //! sort and remove duplicate elements from an array
    order_inplace(a)
    unique_inplace(a)
}
