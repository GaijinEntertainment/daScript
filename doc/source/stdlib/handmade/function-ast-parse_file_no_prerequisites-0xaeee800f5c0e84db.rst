Parses one file alone and stops there: no prerequisite walk, no type inference, no macro run, and the host's module-cache stream is hidden for the duration. The block receives ``(ok, program, issues)`` exactly like ``parse_file``, and the tree lands in ``moduleGroup``, which the caller owns.

Nothing the file requires is loaded. A ``require``, an annotation, a parent class or a reader macro the parse cannot resolve is recorded as an error and walked past, so ``ok`` is false for almost any real file while ``program`` still carries every function the parser built. Only a syntax error ends the parse early, and the functions before it survive. This is the front end for tools that match source shape - a duplicate finder, a lineinfo audit - where compiling the dependency graph would cost seconds per file and change the tree.

The tree is owned by the program and dies with it: keep only plain data past the block, never an ``ExpressionPtr``.
