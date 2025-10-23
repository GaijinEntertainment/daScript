implements _unique_by_to_array(iterator, expression) shorthand notation
that expands into unique_by_to_array(iterator, $(_) => expression)
for example::
  each(foo)._unique_by_to_array(_.id)
