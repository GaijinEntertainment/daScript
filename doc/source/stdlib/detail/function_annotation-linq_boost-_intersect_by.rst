implements _intersect_by(iterator1, iterator2, expression) shorthand notation
that expands into intersect_by(iterator1, iterator2, $(_) => expression)
for example::
  each(foo1)._intersect_by(each(foo2), _.id)
