Parses a daslang file and stops there — no type inference, no optimization, no simulation. The block receives ``(ok, program, issues)`` for the duration of the call, exactly like ``compile_file``.

The resulting AST mirrors the source one-to-one: make-syntax is not lowered, generics are not resolved, constants are not folded and no generated nodes are inserted. That is what source-rewriting tools need — a node's ``at`` still points at the construct the author wrote. A file that does not type-check still yields a usable tree, since nothing past the parse runs.

Modules the file requires are loaded normally (parsed and inferred), so ``require`` resolves; only the named file itself stops at the parse. Set ``codeOfPolicies.version_2_syntax`` to false to parse v1 syntax.

The tree is owned by the program and dies with it: keep only plain data (positions, strings) past the block, never an ``ExpressionPtr``.
