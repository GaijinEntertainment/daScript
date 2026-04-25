Monadic ``Option<T>`` — represents a value that may or may not be present.
Functional API for modelling "absence" in ordinary value code, where nullable
pointers are inapplicable and sentinel values (``-1``, ``""``) are fragile.

The payload may be any type, including non-copyable ones (``array<T>``,
``table<K;V>``, lambdas): constructors and combinators clone or move on the
non-copyable branch via ``static_if (typeinfo can_copy(...))``.

Construct via ``some(v)`` (clones), ``move_some(v)`` (moves out of a mutable
source), or ``none(type<T>)``; read via ``unwrap`` / ``unwrap_or`` / ``??``;
compose via ``map`` / ``and_then`` / ``filter`` / ``or_else`` through the
``|>`` pipe.

See also :ref:`tutorial_option_and_result` and ``daslib/result`` for the
``Result<T, E>`` counterpart.
