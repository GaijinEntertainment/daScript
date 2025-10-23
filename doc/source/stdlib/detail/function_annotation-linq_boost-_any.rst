implements _any(iterator, expression) shorthand notation
that expands into any(iterator, $(_) => expression)
for example::
  each(foo)._any(_ < 5)
