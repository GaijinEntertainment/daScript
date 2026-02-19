implements _select(iterator, expression) shorthand notation
that expands into select(iterator, $(_) => expression)
for example::

  each(foo)._select(_ * 2)
