implements _min_by(iterator, expression) shorthand notation
that expands into min_by(iterator, $(_) => expression)
for example::

  each(foo)._min_by(_.value)
