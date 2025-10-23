implements _order_by(iterator, expression) shorthand notation
that expands into order_by(iterator, $(_) => expression)
for example::
  each(foo)._order_by(_.id)
