options gen2
options indenting = 4
options auto_inline_functions = false

// The identity shapes the built-in fold reduces. Kept character-for-character in step with
// _fixture_parity_rules.das, which the parity test checks before it compares any body.

var g_seed = 3

[export, never_inline] def p_mul_one(x : int) : int { return x * 1 }
[export, never_inline] def p_mul_zero(x : int) : int { return x * 0 }
[export, never_inline] def p_mul_neg_one(x : int) : int { return x * -1 }
[export, never_inline] def p_add_zero(x : float) : float { return x + 0.0 }
[export, never_inline] def p_sub_zero(x : int) : int { return x - 0 }
[export, never_inline] def p_sub_self(x : int) : int { return x - x }
[export, never_inline] def p_zero_sub(x : int) : int { return 0 - x }
[export, never_inline] def p_and_zero(x : int) : int { return x & 0 }
[export, never_inline] def p_and_self(x : int) : int { return x & x }
[export, never_inline] def p_or_zero(x : int) : int { return x | 0 }
[export, never_inline] def p_or_self(x : int) : int { return x | x }
[export, never_inline] def p_xor_zero(x : int) : int { return x ^ 0 }
[export, never_inline] def p_xor_self(x : int) : int { return x ^ x }
[export, never_inline] def p_shift_zero(x : int) : int { return x << 0 }
[export, never_inline] def p_div_one(x : int) : int { return x / 1 }
[export, never_inline] def p_not_not(b : bool) : bool { return !!b }
[export, never_inline] def p_neg_neg(x : int) : int { return -(-x) }
[export, never_inline] def p_ternary_id(c : bool) : bool { return c ? true : false }
[export, never_inline] def p_ternary_not(c : bool) : bool { return c ? false : true }
[export, never_inline] def p_ternary_same(c : bool; x : int) : int { return c ? x : x }
[export, never_inline] def p_and_true(b : bool) : bool { return b && true }
[export, never_inline] def p_or_false(b : bool) : bool { return b || false }
[export, never_inline] def p_double_add_zero(x : double) : double { return x + 0.0lf }
