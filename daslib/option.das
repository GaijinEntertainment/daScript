options gen2
options indenting = 4
options strict_smart_pointers = true

module option shared public

require daslib/templates
require daslib/templates_boost
require daslib/typemacro_boost

//! Monadic ``Option<T>`` type — represents a value that may or may not be present.
//!
//! Functional API for modelling optional value. The payload may be any type,
//! including non-copyable ones (``array<T>``, ``table<K;V>``, lambdas):
//! constructors and combinators clone or move on the non-copyable branch via
//! ``static_if (typeinfo can_copy(...))``.
//!
//! Construct via ``some(v)`` (clones the payload), ``move_some(v)`` (moves out
//! of a mutable source), or ``none(type<T>)``.
//!
//! See also ``daslib/result`` for a ``Result<T, E>`` counterpart.

//! Tagged ``none``/``some`` variant over payload type ``T``.
//!
//! Resolves to ``variant<none; some : T>`` — a structural type with
//! no module home, so ``Option<T>`` reached through any chain of
//! ``require ... public`` collapses to one canonical ``tVariant``
//! candidate (issue #2598). ``none`` is the first case, so
//! ``default<Option<T>>`` is ``none``. Match it with ``none()`` /
//! ``some(x)`` patterns, or use the accessor functions below.
[template_variant(T)]
struct template Option {
    none : void
    @safe_when_uninitialized some : T
}

//! Construct ``Option<T>`` carrying ``v``. Payload type is inferred from ``v``.
//! For non-copyable ``T`` the payload is cloned; if you want to move out of a
//! mutable source, use ``move_some`` instead.
def some(v : auto(TT)) {
    typedef OT = $Option<TT -const>
    static_if (typeinfo can_copy(v)) {
        return <- OT(some = v)
    } else {
        return <- OT(some <- clone_to_move(v))
    }
}


//! Construct ``Option<T>`` by moving the payload out of a mutable source.
//! After ``move_some(src)`` the source is left empty (``length(src) == 0``
//! for arrays/tables). Useful for non-copyable payloads where ``some`` would
//! clone; for workhorse types (``int``, ``float``, ``string``, …) ``some`` is
//! equivalent and cheaper to read.
def move_some(var v : auto(TT)) {
    typedef OT = $Option<TT -const>
    return <- OT(some <- v)
}

//! Construct an empty ``Option<T>``. Pass the payload type as a witness::
//!
//!     let o = none(type<int>)
[template(t)]
def none(t : auto(TT)) => default<$Option<TT -const> >

//! ``true`` iff the option carries a value.
def is_some(o : $Option<auto(TT)>) : bool => o is some

//! ``true`` iff the option is empty.
def is_none(o : $Option<auto(TT)>) : bool => o is none

//! Apply ``f`` to the payload, producing ``Option<U>``. ``none`` stays ``none``.
def map(o : $Option<auto(TT)>; f : block<(v : TT) : auto(UU)>) {
    typedef UT = $Option<UU -const>
    if (o is none) {
        return <- default<UT>
    }
    static_if (typeinfo can_copy(type<UU>)) {
        return <- UT(some = invoke(f, o as some))
    } else {
        return <- UT(some <- invoke(f, o as some))
    }
}

//! Monadic bind — ``f`` returns the next ``Option``; chains only when ``some``.
def and_then(o : $Option<auto(TT)>; f : block<(v : TT) : $Option<auto(UU)> >) {
    if (o is some) return invoke(f, o as some)
    return default<$Option<UU -const> >
}

//! Keep the payload only if ``p`` holds; otherwise turn ``some`` into ``none``.
def filter(o : $Option<auto(TT)>; p : block<(v : TT) : bool>) {
    if (o is some && invoke(p, o as some)) {
        static_if (typeinfo can_copy(o)) {
            return o
        } else {
            return <- clone_to_move(o)
        }
    }
    return default<$Option<TT -const> >
}

//! Fallback to ``f()`` when ``none``; ``some`` passes through unchanged.
def or_else(o : $Option<auto(TT)>; f : block<() : $Option<TT> >) {
    if (o is some) {
        static_if (typeinfo can_copy(o)) {
            return o
        } else {
            return <- clone_to_move(o)
        }
    }
    return invoke(f)
}

//! Fallback to eager ``v`` when ``none``; ``some`` passes through unchanged.
def or_value(o : $Option<auto(TT)>; v : TT) {
    if (o is some) {
        static_if (typeinfo can_copy(o)) {
            return o
        } else {
            return <- clone_to_move(o)
        }
    }
    return some(v)
}

//! Extract the payload or panic when ``none``.
def unwrap(o : $Option<auto(TT)>) : TT {
    if (o is none) {
        panic("unwrap called on none")
    }
    static_if (typeinfo can_copy(o as some)) {
        return o as some
    } else {
        return <- clone_to_move(o as some)
    }
}

//! Destructively extract the payload, leaving ``o`` empty. Mirrors ``move_some``
//! on the construction side. Use this instead of ``unwrap`` when ``T`` is
//! non-copyable (``array<T>``, ``table<K;V>``, lambdas) and you no longer need
//! ``o`` — it avoids the deep clone that ``unwrap`` would otherwise perform.
//! Panics when ``none``.
def move_unwrap(var o : $Option<auto(TT)>) : TT {
    if (o is none) {
        panic("move_unwrap called on none")
    }
    typedef OT = $Option<TT -const>
    var value <- o as some
    var empty : OT
    swap(o, empty)
    return <- value
}

//! Extract the payload or return ``d`` when ``none``.
def unwrap_or(o : $Option<auto(TT)>; d : TT) : TT {
    static_if (typeinfo can_copy(d)) {
        return o is some ? o as some : d
    } else {
        return <- clone_to_move(o is some ? o as some : d)
    }
}

//! Extract the payload or compute a fallback via ``f`` when ``none``.
def unwrap_or_else(o : $Option<auto(TT)>; f : block<() : TT>) : TT {
    static_if (typeinfo can_copy(o)) {
        return o is some ? o as some : invoke(f)
    } else {
        return <- clone_to_move(o is some ? o as some : invoke(f))
    }
}

//! Extract the payload or return ``default<T>`` when ``none``.
def unwrap_or_default(o : $Option<auto(TT)>) : TT {
    static_if (typeinfo can_copy(o)) {
        return o is some ? o as some : default<TT>
    } else {
        return <- clone_to_move(o is some ? o as some : default<TT>)
    }
}

//! Extract the payload or panic with ``msg`` when ``none``. ``expect_value``
//! rather than ``expect`` because ``expect`` is a reserved keyword in daslang.
def expect_value(o : $Option<auto(TT)>; msg : string) : TT {
    if (o is none) {
        panic(msg)
    }
    static_if (typeinfo can_copy(o)) {
        return o as some
    } else {
        return <- clone_to_move(o as some)
    }
}

//! Invoke ``f`` with the payload when ``some``; no-op when ``none``.
def if_some(o : $Option<auto(TT)>; f : block<(v : TT) : void>) : void {
    if (o is some) {
        invoke(f, o as some)
    }
}

//! Invoke ``f`` when ``none``; no-op when ``some``.
def if_none(o : $Option<auto(TT)>; f : block<() : void>) : void {
    if (o is none) {
        invoke(f)
    }
}

//! Pair two options; ``some`` iff both are ``some``.
def zip(a : $Option<auto(TT)>; b : $Option<auto(UU)>) {
    typedef ZT = $Option<tuple<TT -const; UU -const> >
    if (a is none || b is none) {
        return <- default<ZT>
    }
    var pair : tuple<TT -const; UU -const>
    static_if (typeinfo can_copy(a)) {
        pair._0 = a as some
    } else {
        pair._0 := a as some
    }
    static_if (typeinfo can_copy(b)) {
        pair._1 = b as some
    } else {
        pair._1 := b as some
    }
    return <- ZT(some <- pair)
}

//! Unwrap or default. Mirrors ``T? ?? default`` for value types.
def operator ??(o : $Option<auto(TT)>; d : TT) : TT {
    static_if (typeinfo can_copy(d)) {
        return o is some ? o as some : d
    } else {
        return <- clone_to_move(o is some ? o as some : d)
    }
}

//! Structural equality: same tag, and when ``some``, equal payloads.
def operator ==(a, b : $Option<auto(TT)> implicit) : bool {
    if ((a is some) != (b is some)) return false
    return a is none || (a as some) == (b as some)
}

//! Equality against payload value: ``some(v) == v``, ``none != v``.
def operator ==(o : $Option<auto(TT)>; v : TT) : bool => o is some && (o as some) == v

def operator ==(v : auto(TT); o : $Option<TT>) : bool => o is some && (o as some) == v

def operator !=(o : $Option<auto(TT)>; v : TT) : bool => !(o == v)

def operator !=(v : auto(TT); o : $Option<TT>) : bool => !(o == v)
