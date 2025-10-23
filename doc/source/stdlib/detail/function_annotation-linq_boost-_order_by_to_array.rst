implements _order_by_to_array(iterator, expression) shorthand notation
that expands into order_by_to_array(iterator, $(_) => expression)
for example::
  each(foo)._order_by_to_array(_.id)
