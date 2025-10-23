implements _take_while(iterator, expression) shorthand notation
that expands into take_while(iterator, $(_) => expression)
for example::
  each(foo)._take_while(_ < 5)
