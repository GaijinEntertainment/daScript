options gen2
options indenting = 4
options strict_smart_pointers = true

module fold_rules_std shared public

//! The standard declarative fold catalog: one pattern per rewrite, with every condition inside the
//! pattern. A rule whose guard does not hold simply does not match, and the next rule is tried.
//!
//! Requiring this module arms the rules for the requiring module. Under
//! `options disable_algebraic_folding = true` the built-in identity arms stand down and these rules
//! carry the whole load; in a stock build the built-in arms run first each round and these pick up
//! what is left, which is every idiom row.
//!
//! Float laxity is a predicate like any other: `_zero_sign_safe` guards a rewrite whose only effect
//! is the sign of a zero, `_value_safe` one that changes an infinity or NaN result, and
//! `_not_float` keeps self-comparison off the type where a value differs from itself.

require daslib/fold_rules public

[fold_rules]
class StdFolds : FoldRules {
    //! `require daslib/fold_rules_std` is the whole opt-in; write your own `[fold_rules]` class for
    //! domain rules alongside it.
    def override fold(var expr : ExpressionPtr) : ExpressionPtr {   // nolint:STYLE037 - a rule table: one arm per rewrite, nothing to factor
        qmatch(expr) {
            // canonical form: constants to the right of the commutative operators
            if (_const($e(k)) + _not_const($e(x)))  { return qmacro($e(x) + $e(k)) }
            if (_const($e(k)) * _not_const($e(x)))  { return qmacro($e(x) * $e(k)) }
            if (_const($e(k)) & _not_const($e(x)))  { return qmacro($e(x) & $e(k)) }
            if (_const($e(k)) | _not_const($e(x)))  { return qmacro($e(x) | $e(k)) }
            if (_const($e(k)) ^ _not_const($e(x)))  { return qmacro($e(x) ^ $e(k)) }

            // multiply, add, subtract, divide, modulo
            if ($e(x) * 1)                    { return x }
            if (_signed($e(x)) * -1)          { return qmacro(-$e(x)) }
            if (_droppable(_value_safe($e(x))) * 0)    { return zero_like(expr) }
            if (_zero_sign_safe($e(x)) + 0)   { return x }
            if (_zero_sign_safe($e(x)) - 0)   { return x }
            if (0 - _signed($e(x)))           { return qmacro(-$e(x)) }
            if (_droppable(_value_safe($e(x))) - _same(x))  { return zero_like(expr) }
            if ($e(x) / 1)                    { return x }
            if (_droppable(_int($e(x))) % 1)       { return zero_like(expr) }

            // bitwise
            if (_droppable(_int($e(x))) & 0)       { return zero_like(expr) }
            if (_droppable(_int($e(x))) & _same(x))      { return x }
            if (_int($e(x)) | 0)              { return x }
            if (_droppable(_int($e(x))) | _same(x))      { return x }
            if (_int($e(x)) ^ 0)              { return x }
            if (_droppable(_int($e(x))) ^ _same(x))      { return zero_like(expr) }
            if (_int($e(x)) << 0)             { return x }
            if (_int($e(x)) >> 0)             { return x }
            if (_int($e(x)) <<< 0)            { return x }
            if (_int($e(x)) >>> 0)            { return x }

            // boolean
            if (true && $e(r))              { return r }
            if ($e(l) && true)              { return l }
            if (false && _)                 { return qmacro(false) }
            if (_droppable(_) && false)     { return qmacro(false) }
            if (false || $e(r))             { return r }
            if ($e(l) || false)             { return l }
            if (true || _)                  { return qmacro(true) }
            if (_droppable(_) || true)      { return qmacro(true) }

            // self-comparison, never on a type whose value can differ from itself
            if (_droppable(_not_float($e(x))) == _same(x))  { return qmacro(true) }
            if (_droppable(_not_float($e(x))) <= _same(x))  { return qmacro(true) }
            if (_droppable(_not_float($e(x))) >= _same(x))  { return qmacro(true) }
            if (_droppable(_not_float($e(x))) != _same(x))  { return qmacro(false) }
            if (_droppable(_not_float($e(x))) < _same(x))   { return qmacro(false) }
            if (_droppable(_not_float($e(x))) > _same(x))   { return qmacro(false) }

            // unary involution
            if (!!_bool_typed($e(x)))               { return x }
            if (-(-_signed($e(x))))                 { return x }

            // ternary
            if (_ ? true : false)  { return (expr as ExprOp3).subexpr }
            if ($e(c) ? false : true)  { return qmacro(!$e(c)) }
            if (_droppable(_) ? _droppable($e(a)) : _same(a)) { return a }

            // idioms the built-in fold does not carry
            if (pow(_repeatable($e(x)), 2))         { return qmacro($e(x) * $e(x)) }
            if (pow($e(x), 1))                { return x }
            if (pow(_repeatable($e(x)), 0.5))       { return qmacro(sqrt($e(x))) }
            if (abs(abs($e(x))))                    { return qmacro(abs($e(x))) }
            if (abs(-$e(x)))                        { return qmacro(abs($e(x))) }
            if (min(_droppable($e(a)), _same(a)))        { return a }
            if (max(_droppable($e(a)), _same(a)))        { return a }
        }
        return null
    }
}
