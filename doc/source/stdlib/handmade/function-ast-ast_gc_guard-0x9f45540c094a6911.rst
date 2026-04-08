Executes a block and then collects all unreachable GC nodes (TypeDecl instances) created during execution. Use this to prevent GC leak warnings in tools that create temporary AST types.
