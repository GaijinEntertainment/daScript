implements _order_by_descending(iterator, expression) shorthand notation
that expands into order_by_descending(iterator, $(_) => expression)
for example::
  each(foo)._order_by_descending(_.id)
