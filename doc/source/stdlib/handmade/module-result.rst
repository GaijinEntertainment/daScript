Monadic ``Result<T, E>`` — a value (``ok``) or an error (``err``).
Functional API for modelling fallible computations where an error carries
meaning (unlike a bare ``Option<T>`` that only says "missing"). Construct via
``ok(v, type<E>)`` or ``err(e, type<T>)``; compose via ``map`` / ``map_err`` /
``and_then`` / ``or_else``; extract via ``unwrap`` / ``unwrap_or`` / ``??``.
Bridge to ``Option<T>`` via ``to_option`` / ``err_to_option``.

See also :ref:`tutorial_option_and_result` and ``daslib/option`` for the
``Option<T>`` counterpart.
