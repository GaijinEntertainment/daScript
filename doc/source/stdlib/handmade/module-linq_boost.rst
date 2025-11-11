The LINQ boost module implements collection of helper macros and functions to accompany :ref:`LINQ <stdlib_linq>`.

All functions and symbols are in "linq_boost" module, use require to get access to it. ::

    require daslib/linq_boost

linq_boost provides folding capabilities to optimize LINQ expressions.
Use ``_fold(expression)`` macro to fold LINQ expressions into optimized sequences. For example::

    _fold(each(foo)._where(_ > 5)._select(_ * 2))

expands into a single comprehension that does all operations in one pass.

linq_boost provides shorthand notation for some LINQ functions to reduce boilerplate

Transform operations::

    _where(it, expr)          => where_(it, $(_) => expr)
    _where_to_array(it, expr) => where_to_array(it, $(_) => expr)
    _select(it, expr)         => select(it, $(_) => expr)
    _select_to_array(it, expr) => select_to_array(it, $(_) => expr)

Aggregation operations::

    _min_by(it, expr)         => min_by(it, $(_) => expr)
    _max_by(it, expr)         => max_by(it, $(_) => expr)
    _min_max_by(it, expr)     => min_max_by(it, $(_) => expr)
    _min_max_average_by(it, expr) => min_max_average_by(it, $(_) => expr)

Comparators and keys::

    _sequence_equal_by(it1, it2, expr) => sequence_equal_by(it1, it2, $(_) => expr)

Partitioning data::

    _skip_while(it, expr)     => skip_while(it, $(_) => expr)
    _take_while(it, expr)     => take_while(it, $(_) => expr)

Querying data::

    _all(it, expr)            => all(it, $(_) => expr)
    _any(it, expr)            => any(it, $(_) => expr)

Set operations::

    _distinct_by(it, expr)     => distinct_by(it, $(_) => expr)
    _distinct_by_to_array(it, expr) => distinct_by_to_array(it, $(_) => expr)
    _union_by(it1, it2, expr) => union_by(it1, it2, $(_) => expr)
    _union_by_to_array(it1, it2, expr) => union_by_to_array(it1, it2, $(_) => expr)
    _except_by(it1, it2, expr) => except_by(it1, it2, $(_) => expr)
    _except_by_to_array(it1, it2, expr) => except_by_to_array(it1, it2, $(_) => expr)
    _intersect_by(it1, it2, expr) => intersect_by(it1, it2, $(_) => expr)
    _intersect_by_to_array(it1, it2, expr) => intersect_by_to_array(it1, it2, $(_) => expr)
    _unique_by(it, expr) => unique_by(it, $(_) => expr)
    _unique_by_to_array(it, expr) => unique_by_to_array(it, $(_) => expr)

Sorting data::

    _order_by(it, expr)       => order_by(it, $(_) => expr)
    _order_by_to_array(it, expr) => order_by_to_array(it, $(_) => expr)
    _order_by_descending(it, expr) => order_by_descending(it, $(_) => expr)
    _order_by_descending_to_array(it, expr) => order_by_descending_to_array(it, $(_) => expr)
