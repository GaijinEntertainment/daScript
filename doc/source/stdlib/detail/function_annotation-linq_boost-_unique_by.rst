implements _unique_by(iterator, expression) shorthand notation
that expands into unique_by(iterator, $(_) => expression)
for example::

  each(foo)._unique_by(_.id)
