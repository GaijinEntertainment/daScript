implements _where(iterator, expression) shorthand notation
that expands into where_(iterator, $(_) => expression)
for example::
  each(foo)._where(_ < 5)
