implements _distinct_by(iterator, expression) shorthand notation
that expands into distinct_by(iterator, $(_) => expression)
for example::
  each(foo)._distinct_by(_.id)
