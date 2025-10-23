implements sequence_equal_by(i1, i2, expresion) shorthand notation
that expands into sequence_equal_by(i1, i2, $(_) => expression)
for example::
  each(foo1)._sequence_equal_by(each(foo2), _.id)
