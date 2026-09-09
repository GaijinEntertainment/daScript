Calls ``block`` once per module registered under ``group`` - the require paths a
``require [group]`` expands to, sorted by member path - and not at all for a group
nothing joined. ``daslib/module_group``'s ``call_module_group`` reads the group through
this call at macro time; a descriptor joins a group with ``register_module_group``.
