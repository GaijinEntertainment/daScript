The MODULE_GROUP module calls into every member of a module group.
``call_module_group("group", "entry", args...)`` expands at compile time to one
``member::entry(args...)`` call per module registered under the group, sorted by
member path, so the module that wrote ``require [group]`` reaches its members
without naming them. A member without the entry is a compile error naming the
call; a group nothing joined expands to nothing.

All functions and symbols are in "module_group" module, use require to get access to it.

.. code-block:: das

    require daslib/module_group
