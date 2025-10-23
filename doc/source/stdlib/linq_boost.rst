
.. _stdlib_linq_boost:

=====================
Boost module for LINQ
=====================

.. include:: detail/linq_boost.rst

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

+++++++++++
Call macros
+++++++++++

.. _call-macro-linq_boost-_order_by:

.. das:attribute:: _order_by

implements _order_by(iterator, expression) shorthand notation
that expands into order_by(iterator, $(_) => expression)
for example::
  each(foo)._order_by(_.id)

.. _call-macro-linq_boost-_union_by:

.. das:attribute:: _union_by

implements _union_by(iterator1, iterator2, expression) shorthand notation
that expands into union_by(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._union_by(each(foo2), _.id)

.. _call-macro-linq_boost-_take_while:

.. das:attribute:: _take_while

implements _take_while(iterator, expression) shorthand notation
that expands into take_while(iterator, $(_) => expression)
for example::
  each(foo)._take_while(_ < 5)

.. _call-macro-linq_boost-_all:

.. das:attribute:: _all

implements _all(iterator, expression) shorthand notation
that expands into all(iterator, $(_) => expression)
for example::
  each(foo)._all(_ < 5)

.. _call-macro-linq_boost-_distinct_by_to_array:

.. das:attribute:: _distinct_by_to_array

implements _distinct_by_to_array(iterator, expression) shorthand notation
that expands into distinct_by_to_array(iterator, $(_) => expression)
for example::
  each(foo)._distinct_by_to_array(_.id)

.. _call-macro-linq_boost-_order_by_to_array:

.. das:attribute:: _order_by_to_array

implements _order_by_to_array(iterator, expression) shorthand notation
that expands into order_by_to_array(iterator, $(_) => expression)
for example::
  each(foo)._order_by_to_array(_.id)

.. _call-macro-linq_boost-_except_by:

.. das:attribute:: _except_by

implements _except_by(iterator1, iterator2, expression) shorthand notation
that expands into except_by(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._except_by(each(foo2), _.id)

.. _call-macro-linq_boost-_min_max_average_by:

.. das:attribute:: _min_max_average_by

implements _min_max_average_by(iterator, expression) shorthand notation
that expands into min_max_average_by(iterator, $(_) => expression)
for example::
  each(foo)._min_max_average_by(_.value)

.. _call-macro-linq_boost-_min_by:

.. das:attribute:: _min_by

implements _min_by(iterator, expression) shorthand notation
that expands into min_by(iterator, $(_) => expression)
for example::
  each(foo)._min_by(_.value)

.. _call-macro-linq_boost-_select_to_array:

.. das:attribute:: _select_to_array

implements _select_to_array(iterator, expression) shorthand notation
that expands into select_to_array(iterator, $(_) => expression)
for example::
  each(foo)._select_to_array(_ * 2)

.. _call-macro-linq_boost-_union_by_to_array:

.. das:attribute:: _union_by_to_array

implements _union_by_to_array(iterator1, iterator2, expression) shorthand notation
that expands into union_by_to_array(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._union_by_to_array(each(foo2), _.id)

.. _call-macro-linq_boost-_unique_by_to_array:

.. das:attribute:: _unique_by_to_array

implements _unique_by_to_array(iterator, expression) shorthand notation
that expands into unique_by_to_array(iterator, $(_) => expression)
for example::
  each(foo)._unique_by_to_array(_.id)

.. _call-macro-linq_boost-_max_by:

.. das:attribute:: _max_by

implements _max_by(iterator, expression) shorthand notation
that expands into max_by(iterator, $(_) => expression)
for example::
  each(foo)._max_by(_.value)

.. _call-macro-linq_boost-_any:

.. das:attribute:: _any

implements _any(iterator, expression) shorthand notation
that expands into any(iterator, $(_) => expression)
for example::
  each(foo)._any(_ < 5)

.. _call-macro-linq_boost-_except_by_to_array:

.. das:attribute:: _except_by_to_array

implements _except_by_to_array(iterator1, iterator2, expression) shorthand notation
that expands into except_by_to_array(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._except_by_to_array(each(foo2), _.id)

.. _call-macro-linq_boost-_order_by_descending:

.. das:attribute:: _order_by_descending

implements _order_by_descending(iterator, expression) shorthand notation
that expands into order_by_descending(iterator, $(_) => expression)
for example::
  each(foo)._order_by_descending(_.id)

.. _call-macro-linq_boost-_order_by_descending_to_array:

.. das:attribute:: _order_by_descending_to_array

implements _order_by_descending_to_array(iterator, expression) shorthand notation
that expands into order_by_descending_to_array(iterator, $(_) => expression)
for example::
  each(foo)._order_by_descending_to_array(_.id)

.. _call-macro-linq_boost-_distinct_by:

.. das:attribute:: _distinct_by

implements _distinct_by(iterator, expression) shorthand notation
that expands into distinct_by(iterator, $(_) => expression)
for example::
  each(foo)._distinct_by(_.id)

.. _call-macro-linq_boost-_select:

.. das:attribute:: _select

implements _select(iterator, expression) shorthand notation
that expands into select(iterator, $(_) => expression)
for example::
  each(foo)._select(_ * 2)

.. _call-macro-linq_boost-_intersect_by_to_array:

.. das:attribute:: _intersect_by_to_array

implements _intersect_by_to_array(iterator1, iterator2, expression) shorthand notation
that expands into intersect_by_to_array(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._intersect_by_to_array(each(foo2), _.id)

.. _call-macro-linq_boost-_min_max_by:

.. das:attribute:: _min_max_by

implements _min_max_by(iterator, expression) shorthand notation
that expands into min_max_by(iterator, $(_) => expression)
for example::
  each(foo)._min_max_by(_.value)

.. _call-macro-linq_boost-_intersect_by:

.. das:attribute:: _intersect_by

implements _intersect_by(iterator1, iterator2, expression) shorthand notation
that expands into intersect_by(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._intersect_by(each(foo2), _.id)

.. _call-macro-linq_boost-_where_to_array:

.. das:attribute:: _where_to_array

implements _where_to_array(iterator, expression) shorthand notation
that expands into where_to_array(iterator, $(_) => expression)
for example::
  each(foo)._where_to_array(_ < 5)

.. _call-macro-linq_boost-_skip_while:

.. das:attribute:: _skip_while

implements _skip_while(iterator, expression) shorthand notation
that expands into skip_while(iterator, $(_) => expression)
for example::
  each(foo)._skip_while(_ < 5)

.. _call-macro-linq_boost-_where:

.. das:attribute:: _where

implements _where(iterator, expression) shorthand notation
that expands into where_(iterator, $(_) => expression)
for example::
  each(foo)._where(_ < 5)

.. _call-macro-linq_boost-_unique_by:

.. das:attribute:: _unique_by

implements _unique_by(iterator, expression) shorthand notation
that expands into unique_by(iterator, $(_) => expression)
for example::
  each(foo)._unique_by(_.id)

.. _call-macro-linq_boost-_fold:

.. das:attribute:: _fold

implements _fold(expression) that folds LINQ expressions into optimized sequnences
for example::
  _fold(each(foo)._where(_ > 5)._select(_ * 2))
expands into a single comprehension that does all operations in one pass

.. _call-macro-linq_boost-_sequence_equal_by:

.. das:attribute:: _sequence_equal_by

implements _sequence_equal_by(iterator1, iterator2, expression) shorthand notation
that expands into sequence_equal_by(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._sequence_equal_by(each(foo2), _.id)


