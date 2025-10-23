implements _sequence_equal_by(iterator1, iterator2, expression) shorthand notation
that expands into sequence_equal_by(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._sequence_equal_by(each(foo2), _.id)
