implements _all(iterator, expression) shorthand notation
that expands into all(iterator, $(_) => expression)
for example::
  each(foo)._all(_ < 5)
