implements _select_to_array(iterator, expression) shorthand notation
that expands into select_to_array(iterator, $(_) => expression)
for example::
  each(foo)._select_to_array(_ * 2)
