Monadic ``Option<T>`` — represents a value that may or may not be present.
Functional API for modelling "absence" in ordinary value code, where nullable
pointers are inapplicable and sentinel values (``-1``, ``""``) are fragile.
Construct via ``some(v)`` or ``none(type<T>)``; read via ``unwrap`` /
``unwrap_or`` / ``??``; compose via ``map`` / ``and_then`` / ``filter`` /
``or_else`` through the ``|>`` pipe.

See also :ref:`tutorial_option_and_result` and ``daslib/result`` for the
``Result<T, E>`` counterpart.
