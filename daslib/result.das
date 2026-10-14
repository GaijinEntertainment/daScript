options gen2
options indenting = 4
options strict_smart_pointers = true

module result shared public

require daslib/templates
require daslib/templates_boost
require daslib/typemacro_boost
require daslib/option public

//! Monadic ``Result<T, E>`` type — a value (``ok``) or an error (``err``).
//!
//! Functional API for modelling fallible computations where an
//! error carries meaning (unlike a bare ``Option<T>`` that only says "missing").
//! Both the value side ``T`` and the error side ``E`` may be non-copyable
//! (``array<T>``, ``table<K;V>``, lambdas) — combinators clone or move on the
//! non-copyable branch via ``static_if (typeinfo can_copy(...))``.
//!
//! Construct via ``ok(v, type<E>)`` / ``err(e, type<T>)`` (clone), or
//! ``move_ok(v, type<E>)`` / ``move_err(e, type<T>)`` to move out of a mutable
//! source. Compose via ``map`` / ``map_err`` / ``and_then`` / ``or_else``;
//! extract via ``unwrap`` / ``unwrap_or`` / ``??``.

//! Tagged ``err``/``ok`` variant over payload ``T`` and error ``E``.
//!
//! Resolves to ``variant<err : E; ok : T>`` — a structural type with
//! no module home, so ``Result<T, E>`` reached through any chain of
//! ``require ... public`` collapses to one canonical ``tVariant``
//! candidate (issue #2598). ``err`` is the first case, so
//! ``default<Result<T, E>>`` is an ``err`` holding the zero value of
//! ``E``. Match it with ``ok(x)`` / ``err(e)`` patterns, or use the
//! accessor functions below.
[template_variant(T, E)]
struct template Result {
    @safe_when_uninitialized err : E
    @safe_when_uninitialized ok : T
}

//! Construct ``Result<T, E>`` carrying successful ``v``. Pass the error type as a witness::
//!
//!     let r = ok(42, type<string>)
//!
//! For non-copyable ``T`` the payload is cloned; if you want to move out of a
//! mutable source, use ``move_ok`` instead.
[template(etype)]
def ok(v : auto(TT); etype : auto(EE)) {
    typedef RT = $Result<TT -const; EE -const>
    static_if (typeinfo can_copy(v)) {
        return <- RT(ok = v)
    } else {
        return <- RT(ok <- clone_to_move(v))
    }
}

//! Construct ``Result<T, E>`` by moving the value payload out of a mutable
//! source. After ``move_ok(src, type<E>)`` the source is left empty
//! (``length(src) == 0`` for arrays/tables). Useful for non-copyable
//! payloads where ``ok`` would clone.
[template(etype)]
def move_ok(var v : auto(TT); etype : auto(EE)) {
    typedef RT = $Result<TT -const; EE -const>
    return <- RT(ok <- v)
}

//! Construct ``Result<T, E>`` carrying error ``e``. Pass the value type as a witness::
//!
//!     let r = err("boom", type<int>)
//!
//! For non-copyable ``E`` the error is cloned; if you want to move out of a
//! mutable source, use ``move_err`` instead.
[template(ttype)]
def err(e : auto(EE); ttype : auto(TT)) {
    typedef RT = $Result<TT -const; EE -const>
    static_if (typeinfo can_copy(e)) {
        return <- RT(err = e)
    } else {
        return <- RT(err <- clone_to_move(e))
    }
}

//! Construct ``Result<T, E>`` by moving the error payload out of a mutable
//! source. After ``move_err(src, type<T>)`` the source is left empty
//! (``length(src) == 0`` for arrays/tables). Useful for non-copyable error
//! payloads where ``err`` would clone.
[template(ttype)]
def move_err(var e : auto(EE); ttype : auto(TT)) {
    typedef RT = $Result<TT -const; EE -const>
    return <- RT(err <- e)
}

//! ``true`` iff the result is a successful ``ok``.
def is_ok(r : $Result<auto(TT); auto(EE)>) : bool => r is ok

//! ``true`` iff the result is a failed ``err``.
def is_err(r : $Result<auto(TT); auto(EE)>) : bool => r is err

//! Apply ``f`` to the ok payload. ``err`` stays ``err`` (same ``E``).
def map(r : $Result<auto(TT); auto(EE)>; f : block<(v : TT) : auto(UU)>) {
    typedef OT = $Result<UU -const; EE -const>
    if (r is err) {
        static_if (typeinfo can_copy(type<EE>)) {
            return <- OT(err = r as err)
        } else {
            return <- OT(err <- clone_to_move(r as err))
        }
    }
    static_if (typeinfo can_copy(type<UU>)) {
        return <- OT(ok = invoke(f, r as ok))
    } else {
        return <- OT(ok <- invoke(f, r as ok))
    }
}

//! Apply ``f`` to the err payload. ``ok`` stays ``ok`` (same ``T``).
def map_err(r : $Result<auto(TT); auto(EE)>; f : block<(e : EE) : auto(FF)>) {
    typedef OT = $Result<TT -const; FF -const>
    if (r is ok) {
        static_if (typeinfo can_copy(type<TT>)) {
            return <- OT(ok = r as ok)
        } else {
            return <- OT(ok <- clone_to_move(r as ok))
        }
    }
    static_if (typeinfo can_copy(type<FF>)) {
        return <- OT(err = invoke(f, r as err))
    } else {
        return <- OT(err <- invoke(f, r as err))
    }
}

//! Monadic bind on the ok side — ``f`` returns the next ``Result``.
def and_then(r : $Result<auto(TT); auto(EE)>; f : block<(v : TT) : $Result<auto(UU); EE> >) {
    if (r is ok) return <- invoke(f, r as ok)
    typedef OT = $Result<UU -const; EE -const>
    static_if (typeinfo can_copy(type<EE>)) {
        return <- OT(err = r as err)
    } else {
        return <- OT(err <- clone_to_move(r as err))
    }
}

//! Monadic bind on the err side — ``f`` can recover or re-map the error.
def or_else(r : $Result<auto(TT); auto(EE)>; f : block<(e : EE) : $Result<TT; auto(FF)> >) {
    if (r is err) return <- invoke(f, r as err)
    typedef OT = $Result<TT -const; FF -const>
    static_if (typeinfo can_copy(type<TT>)) {
        return <- OT(ok = r as ok)
    } else {
        return <- OT(ok <- clone_to_move(r as ok))
    }
}

//! Extract the ok payload or panic when ``err``.
def unwrap(r : $Result<auto(TT); auto(EE)>) : TT {
    if (r is err) {
        panic("unwrap called on err")
    }
    static_if (typeinfo can_copy(type<TT>)) {
        return r as ok
    } else {
        return <- clone_to_move(r as ok)
    }
}

//! Destructively extract the ok payload, leaving ``r`` in the err state with
//! the err side defaulted. Mirrors ``move_ok`` on the construction side. Use
//! this instead of ``unwrap`` when ``T`` is non-copyable and you no longer need
//! ``r`` — it avoids the deep clone that ``unwrap`` would otherwise perform.
//! Panics when ``err``.
def move_unwrap(var r : $Result<auto(TT); auto(EE)>) : TT {
    if (r is err) {
        panic("move_unwrap called on err")
    }
    typedef RT = $Result<TT -const; EE -const>
    var value <- r as ok
    var empty : RT
    swap(r, empty)
    return <- value
}

//! Extract the ok payload or return ``d`` when ``err``.
def unwrap_or(r : $Result<auto(TT); auto(EE)>; d : TT) : TT {
    static_if (typeinfo can_copy(type<TT>)) {
        return r is ok ? r as ok : d
    } else {
        return <- clone_to_move(r is ok ? r as ok : d)
    }
}

//! Extract the ok payload or compute a fallback from the error via ``f``.
def unwrap_or_else(r : $Result<auto(TT); auto(EE)>; f : block<(e : EE) : TT>) : TT {
    static_if (typeinfo can_copy(type<TT>)) {
        return r is ok ? r as ok : invoke(f, r as err)
    } else {
        return <- clone_to_move(r is ok ? r as ok : invoke(f, r as err))
    }
}

//! Extract the ok payload or return ``default<T>`` when ``err``.
def unwrap_or_default(r : $Result<auto(TT); auto(EE)>) : TT {
    static_if (typeinfo can_copy(type<TT>)) {
        return r is ok ? r as ok : default<TT>
    } else {
        return <- clone_to_move(r is ok ? r as ok : default<TT>)
    }
}

//! Extract the err payload or panic when ``ok``.
def unwrap_err(r : $Result<auto(TT); auto(EE)>) : EE {
    if (r is ok) {
        panic("unwrap_err called on ok")
    }
    static_if (typeinfo can_copy(type<EE>)) {
        return r as err
    } else {
        return <- clone_to_move(r as err)
    }
}

//! Destructively extract the err payload, leaving ``r`` in the ok state with
//! the ok side defaulted. Mirrors ``move_err`` on the construction side.
//! Panics when ``ok``.
def move_unwrap_err(var r : $Result<auto(TT); auto(EE)>) : EE {
    if (r is ok) {
        panic("move_unwrap_err called on ok")
    }
    typedef RT = $Result<TT -const; EE -const>
    var error <- r as err
    var value : TT -const
    var fresh <- RT(ok <- value)
    swap(r, fresh)
    return <- error
}

//! Extract the ok payload or panic with ``msg`` when ``err``. ``expect_value``
//! rather than ``expect`` because ``expect`` is a reserved keyword.
def expect_value(r : $Result<auto(TT); auto(EE)>; msg : string) : TT {
    if (r is err) {
        panic(msg)
    }
    static_if (typeinfo can_copy(type<TT>)) {
        return r as ok
    } else {
        return <- clone_to_move(r as ok)
    }
}

//! Extract the err payload or panic with ``msg`` when ``ok``.
def expect_err(r : $Result<auto(TT); auto(EE)>; msg : string) : EE {
    if (r is ok) {
        panic(msg)
    }
    static_if (typeinfo can_copy(type<EE>)) {
        return r as err
    } else {
        return <- clone_to_move(r as err)
    }
}

//! Invoke ``f`` with the ok payload when ``ok``; no-op when ``err``.
def if_ok(r : $Result<auto(TT); auto(EE)>; f : block<(v : TT) : void>) : void {
    if (r is ok) {
        invoke(f, r as ok)
    }
}

//! Invoke ``f`` with the err payload when ``err``; no-op when ``ok``.
def if_err(r : $Result<auto(TT); auto(EE)>; f : block<(e : EE) : void>) : void {
    if (r is err) {
        invoke(f, r as err)
    }
}

//! Drop the err side; ``ok`` becomes ``some``, ``err`` becomes ``none``.
def to_option(r : $Result<auto(TT); auto(EE)>) {
    if (r is err) {
        return <- none(type<TT>)
    }
    return <- some(r as ok)
}

//! Drop the ok side; ``err`` becomes ``some``, ``ok`` becomes ``none``.
def err_to_option(r : $Result<auto(TT); auto(EE)>) {
    if (r is ok) {
        return <- none(type<EE>)
    }
    return <- some(r as err)
}

//! Unwrap or default. Mirrors ``T? ?? default`` for value types.
def operator ??(r : $Result<auto(TT); auto(EE)>; d : TT) : TT {
    static_if (typeinfo can_copy(type<TT>)) {
        return r is ok ? r as ok : d
    } else {
        return <- clone_to_move(r is ok ? r as ok : d)
    }
}

//! Structural equality: same tag, and the active payload compares equal.
def operator ==(a, b : $Result<auto(TT); auto(EE)> implicit) : bool {
    if ((a is ok) != (b is ok)) return false
    return a is ok ? (a as ok) == (b as ok) : (a as err) == (b as err)
}

//! Equality against ok payload: ``ok(v) == v``, ``err(...) != v``.
def operator ==(r : $Result<auto(TT); auto(EE)>; v : TT) : bool => r is ok && (r as ok) == v

def operator ==(v : auto(TT); r : $Result<TT; auto(EE)>) : bool => r is ok && (r as ok) == v

def operator !=(r : $Result<auto(TT); auto(EE)>; v : TT) : bool => !(r == v)

def operator !=(v : auto(TT); r : $Result<TT; auto(EE)>) : bool => !(r == v)
