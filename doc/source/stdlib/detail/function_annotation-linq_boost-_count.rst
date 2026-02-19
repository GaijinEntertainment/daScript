implements _count(iterator, expression) shorthand notation
that expands into count(iterator, $(_) => expression)
for example::

  each(foo)._count(_ > 3)
