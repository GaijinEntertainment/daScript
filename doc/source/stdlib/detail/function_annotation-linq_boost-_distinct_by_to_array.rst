implements _distinct_by_to_array(iterator, expression) shorthand notation
that expands into distinct_by_to_array(iterator, $(_) => expression)
for example::

  each(foo)._distinct_by_to_array(_.id)
