implements _union_by(iterator1, iterator2, expression) shorthand notation
that expands into union_by(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._union_by(each(foo2), _.id)
