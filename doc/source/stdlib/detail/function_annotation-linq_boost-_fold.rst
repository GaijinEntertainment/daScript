implements _fold(expression) that folds LINQ expressions into optimized sequnences
for example::
  _fold(each(foo)._where(_ > 5)._select(_ * 2))
expands into a single comprehension that does all operations in one pass
