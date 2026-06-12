informational access-detail flags of the `Variable` object. no semantic meaning of their own; consumed by lints and refactoring tools.
Variable appeared where mutability is required by a signature or a mutable-pointee sink, whether or not anything writes it — declaring it const would no longer compile.
