Returns a lazy iterator over child elements of *node* whose tag
matches *name*. Intended for use in ``for`` loops:

  ``for (ch in each_child(node, "item")) { ... }``
