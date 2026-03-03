Returns a lazy iterator over all ``xpath_node`` entries in an
``xpath_node_set``. Intended for use in ``for`` loops.
Because the iterator is named ``each``, the call can be omitted:

  ``for (xn in node_set) { ... }``
