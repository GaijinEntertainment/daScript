implements _skip_while(iterator, expression) shorthand notation
that expands into skip_while(iterator, $(_) => expression)
for example::
  each(foo)._skip_while(_ < 5)
