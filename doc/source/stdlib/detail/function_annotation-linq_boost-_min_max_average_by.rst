implements _min_max_average_by(iterator, expression) shorthand notation
that expands into min_max_average_by(iterator, $(_) => expression)
for example::
  each(foo)._min_max_average_by(_.value)
