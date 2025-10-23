implements _where_to_array(iterator, expression) shorthand notation
that expands into where_to_array(iterator, $(_) => expression)
for example::
  each(foo)._where_to_array(_ < 5)
