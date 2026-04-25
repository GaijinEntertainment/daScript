Monadic ``Result<T, E>`` — a value (``ok``) or an error (``err``).
Functional API for modelling fallible computations where an error carries
meaning (unlike a bare ``Option<T>`` that only says "missing").

Both the value side ``T`` and the error side ``E`` may be non-copyable
(``array<T>``, ``table<K;V>``, lambdas) — constructors and combinators clone
or move on the non-copyable branch via ``static_if (typeinfo can_copy(...))``.

Construct via ``ok(v, type<E>)`` / ``err(e, type<T>)`` (clone), or
``move_ok(v, type<E>)`` / ``move_err(e, type<T>)`` to move out of a mutable
source; compose via ``map`` / ``map_err`` / ``and_then`` / ``or_else``;
extract via ``unwrap`` / ``unwrap_or`` / ``??``. Bridge to ``Option<T>`` via
``to_option`` / ``err_to_option``.

See also :ref:`tutorial_option_and_result` and ``daslib/option`` for the
``Option<T>`` counterpart.
