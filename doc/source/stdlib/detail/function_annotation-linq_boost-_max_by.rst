implements _max_by(iterator, expression) shorthand notation
that expands into max_by(iterator, $(_) => expression)
for example::

  each(foo)._max_by(_.value)
